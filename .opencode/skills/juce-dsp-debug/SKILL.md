---
name: JUCE DSP memory debugging
description: Hunt memory bugs in the SpectralSuite DSP core (shared/ processors, PhaseBuffer, interactors). Use when output is silent, extreme, NaN, or crashy, or when a resize/prepare sequence is suspected. Workflow is ASan-first with production-faithful harnesses.
---

# JUCE DSP memory debugging

## 1. Suspect shared state first, not the kernel

Per-plugin `spectral_process` kernels are usually innocent. The repeat offenders live in `shared/`:

- `PhaseBuffer::prevPhase` must **always be ≥ the largest `fftSize/2` in use**. Resizing is grow-only; `SpectralAudioProcessorInteractor::prepareToPlay` sizes it before the first block. Any change to the FFT lifecycle must preserve this invariant.
- `StandardFFTProcessor::m_offset` (STFT write cursor) must be valid for the *current* buffer sizes after any resize.
- `prepareToPlay` can run directly at a non-default FFT size (project load), in which case the plugin's `setFftSize` **early-returns** and per-processor resize code never runs. Always test this path, not just runtime switches.

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

## 3. Write a production-faithful reproduction

- Drive the **real `*Interactor`** (`prepareToPlay` → `process` blocks → `setFftSize`), not the processor in isolation. Isolated `setFFTSize` calls skip the `setOffset` / buffer-sizing calls the interactor makes and can prove a bug that production never hits (or miss the real one).
- Cover both FFT styles: `DEFAULT` and PVOC (`usePvoc(true)`). The phase-vocoder path reads `prevPhase`; the default path does not.
- Cover the load path: `prepareToPlay` **directly at a non-default size** (e.g. 4096), not only runtime switches from 2048.
- Assert finiteness of output and buffer-size invariants, not exact samples.

## 4. Prove causality both directions

- Pre-fix code must **crash under ASan** with the exact overflow location (file:line, e.g. `PhaseVocoder.cpp:50` reading `prevPhase[n]`).
- Post-fix code must run **clean** (zero ASan errors, all tests pass).
- Revert *only* the fix and re-run: the exact overflow must return. If it doesn't, the test doesn't isolate the fix.
- Note: an OOB *read* manifests variably in release (silence vs +300 dB vs crash) depending on heap contents — don't require a specific symptom, require the ASan report.

## 5. End-to-end confirmation (optional, see `reaper-reapy`)

For user-facing proof, A/B the before/after plugin binaries in REAPER with bypass renders as anchors. Unit+ASan proof is the acceptance bar; REAPER is corroboration.
