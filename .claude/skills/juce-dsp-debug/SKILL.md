---
name: juce-dsp-debug
description: Hunt memory bugs in the SpectralSuite DSP core (shared/ processors, PhaseBuffer, interactors). Use when output is silent, extreme, NaN, or crashy (including a host crash report pointing into a plugin), or when a resize/prepare/project-reload sequence is suspected. Workflow is ASan-first with production-faithful harnesses.
---

# JUCE DSP memory debugging

## 0. Start from the crash report

If a host crashed, read its report before theorising: macOS writes
`~/Library/Logs/DiagnosticReports/<Host>-<date>.ips` (a JSON header line, then a JSON
body with `threads`, `faultingThread` and `usedImages`). Decode the faulting thread's
frames and note which plugin image (AU or VST3, which path) they're in.

- `std::terminate` / `abort` under `JuceAU::Render` or the VST3 process call means an
  **uncaught C++ exception on the audio thread**, not a memory error. Look for
  throwing calls in the process path: bounds-checked `.at(` **and** `->at(` (search
  for both), `std::vector` resizes with a size that can go negative, `std::sto*`.
- `EXC_BAD_ACCESS` / `SIGSEGV` points at a memory error; go to the ASan harness.

## 1. Suspect shared state first, not the kernel

Per-plugin `spectral_process` kernels are usually innocent. The repeat offenders live in `shared/`:

- `PhaseBuffer::prevPhase` must **always be ≥ the largest `fftSize/2` in use**. Resizing is grow-only; `SpectralAudioProcessorInteractor::prepareToPlay` sizes it before the first block. Any change to the FFT lifecycle must preserve this invariant.
- `StandardFFTProcessor::m_offset` (STFT write cursor) must be valid for the *current* buffer sizes after any resize.
- `prepareToPlay` can run directly at a non-default FFT size (project load), in which case the plugin's `setFftSize` **early-returns** and per-processor resize code never runs. Always test this path, not just runtime switches.
- After `prepareToPlay`, every processor's FFT size and the interactor's hop size must match the requested FFT size. Hosts can prepare **again** at a different size with the processor bank already built; `SpectralAudioProcessorInteractor::prepareToPlay` rebuilds a bank built for another size, because `setNumOverlaps` would otherwise keep it and `setFftSize` sees no change. Plugin-side state sized from the FFT size (e.g. Bin Scrambler's index lists) goes out of step with stale processors: that crashed REAPER below the default size and silenced it above.

## 2. Build the ASan harness

Configure a separate build dir with sanitizers (never the release dir):

```bash
cmake -S . -B build-asan -G Ninja -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" \
  -DCMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address" [+ repo toolchain flags]
cmake --build build-asan --target SpectralSuiteTests
ASAN_OPTIONS=detect_leaks=0 ./build-asan/Tests/SpectralSuiteTests_artefacts/Debug/SpectralSuiteTests
```

Repo toolchain flags on this machine (see AGENTS.md): the compiler must be the
`clang -Wno-error` wrapper (e.g. `~/.local/share/reaper-mcp/bin/clang++-nowerror`)
because JUCE 8.0.6 fails the root `-Werror` on the newer SDK, and **every**
`cmake --build` invocation must carry `CPLUS_INCLUDE_PATH` pointing at
`/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1` (plus
`C_INCLUDE_PATH` at the SDK's `usr/include`). A source change can trigger
CMake's regenerate-during-build, which rebuilds `juceaide` — without those env
vars it fails with `'algorithm' file not found`.

## 3. Write a production-faithful reproduction

- Drive the **real `*Interactor`** (`prepareToPlay` → `process` blocks → `setFftSize`), not the processor in isolation. Isolated `setFFTSize` calls skip the `setOffset` / buffer-sizing calls the interactor makes and can prove a bug that production never hits (or miss the real one).
- Cover both FFT styles: `DEFAULT` and PVOC (`usePvoc(true)`). The phase-vocoder path reads `prevPhase`; the default path does not.
- Cover the load path: `prepareToPlay` **directly at a non-default size** (e.g. 4096), not only runtime switches from 2048.
- Cover **both host orders** for a project reload, through the real `SpectralAudioPlugin` with saved state: `setStateInformation` → `prepareToPlay`, and `prepareToPlay` (default) → `setStateInformation` → `prepareToPlay` (REAPER does the second). `Tests/ProjectReloadTests.cpp` is the model. Interactor-only tests that prepare once at the restored size miss the second order.
- Assert the interactor's hop size equals `fftSize / overlaps` after the reload. Output-level checks alone (peak, non-silence) can pass while the processors run at the wrong size.
- The plugin owns its `DependencyFactory` (`new` it, keep a raw pointer for inspection); a stack-allocated factory is a test bug ASan reports as a bad free.
- Assert finiteness of output and buffer-size invariants, not exact samples.

## 4. Prove causality both directions

- Pre-fix code must **crash under ASan** with the exact overflow location (file:line, e.g. `PhaseVocoder.cpp:50` reading `prevPhase[n]`).
- Post-fix code must run **clean** (zero ASan errors, all tests pass).
- Revert *only* the fix and re-run: the exact overflow must return. If it doesn't, the test doesn't isolate the fix.
- Note: an OOB *read* manifests variably in release (silence vs +300 dB vs crash) depending on heap contents — don't require a specific symptom, require the ASan report.

## 5. End-to-end confirmation (optional, see `reaper-reapy`)

For user-facing proof, run the crash scenario in a scripted REAPER session:
playback + rapid parameter toggling + FFT switching + save/reload, with no
crash/hang and a responsive API as the pass bar. The distant API cannot
automate renders on this machine (see `reaper-reapy`'s Rendering section), so
output sanity rests on the unit-test finiteness assertions; leave an explicit
manual-listen item in the local host-validation record (`.vibe/testing/`,
gitignored — Andrew prefers release test records not committed; historical
`Testing/*.md` files from earlier releases remain in the repo).
Unit+ASan proof is the acceptance bar; REAPER is corroboration.
