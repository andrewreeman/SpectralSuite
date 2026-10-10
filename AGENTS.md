# AGENTS.md

SpectralSuite: a CMake / JUCE 8 suite of 7 audio plugins (AU + VST3) that share a
spectral-processing framework in `shared/`. One root CMake project builds them all.

## Build

```bash
cmake -B build -G <generator>     # from repo root; fetches JUCE 8.0.6 via CPM
cmake --build build --config Release
```

- Single plugin: `cmake --build build --target <target> --config Release`.
  CMake targets differ from directory names: `BinScrambler`, `FreqMagnet`,
  `FreqShift`, `Morph`, `PhaseLock`, `Ssf` (SinusoidalShapedFilter), `SpecGate`.
- `CMAKE_COMPILE_WARNING_AS_ERROR ON` — warnings fail the build.
- macOS builds universal (`arm64 x86_64`).
- `libs/juce` and all `cmake-build-*` / `build` dirs are generated and gitignored.
  First configure downloads JUCE; do not commit it.
- Local-toolchain note (this machine only): the CommandLineTools install lacks its own
  libc++ headers, so configure with `CPLUS_INCLUDE_PATH` pointed at the SDK's
  `usr/include/c++/v1`; JUCE 8.0.6 also fails the root `-Werror` on the newer SDK
  (harfbuzz, `NSEventTypeMouseCancelled`), so local builds use a `clang -Wno-error`
  wrapper. Neither workaround touches repo source.

## Tests

- Automated acceptance tests live in `Tests/` as headless `juce::UnitTest` classes
  (category `SpectralSuite`), run by `Tests/TestRunner.cpp` and registered with CTest:
  ```bash
  cmake --build build --target SpectralSuiteTests
  ctest --test-dir build -R SpectralSuiteTests
  # or run the binary directly to see every sub-test:
  ./build/Tests/SpectralSuiteTests_artefacts/SpectralSuiteTests
  ```
- They test the pure DSP core (`spectral_process` kernels, `utilities`, state machines,
  `PitchInfoParser`), so when adding a `*FFTProcessor.cpp` that should be covered, add it
  to `Tests/CMakeLists.txt`. `FftResizeTests` / `FftResizeProductionTests` cover runtime
  FFT-size changes through the real `BinScramblerInteractor`. Tests are deliberately
  **not** part of the Docker/release image.
- Manual host checks are recorded in `Testing/*.md` (AU/VST3 in REAPER/FL Studio/Logic).
  scripted REAPER testing goes through the `reaper-reapy` skill. On macOS, validate AU
  registration with `./auValidate.sh` (`auval -strict` per plugin).
- `Tests/` intentionally omits `juce_recommended_warning_flags` and sets
  `COMPILE_WARNING_AS_ERROR OFF` (the root enables `-Werror`, which breaks JUCE's bundled
  harfbuzz and fights int-indexed test loops). Plugins keep the full warning set.
- Because the production tests include `SpectralAudioPlugin.h`, `Tests/` also links a
  `SpectralSuiteTestsData` binary-data target and compiles the BinScrambler
  interactor + parameters sources. Any new test that pulls in UI/resource headers needs
  the same treatment.

## Architecture

- `shared/` is the framework, but **not a library**. Its sources are injected into
  every plugin via `${SHARED_SOURCES}` in `shared/shared.cmake`. Any new shared
  `.cpp`/`.h` must be added to that list or plugins won't compile.
- Each plugin dir has `Source/` with four roles:
  - `*AudioPlugin` — JUCE `AudioProcessor`; wires a `SpectralAudioPlugin::DependencyFactory`.
  - `*Parameters` — wraps `AudioProcessorValueTreeState`.
  - `*Interactor` — `SpectralAudioProcessorInteractor`; FFT setup, hop/overlap, per-channel processing.
  - `*FFTProcessor` — `StandardFFTProcessor` subclass; implement `spectral_process(in, out, bins)`.
- FFT size / style / overlap / window switching is owned by `SpectralAudioPlugin` and
  runs on a background `FftSwitcherThread`; changes are refused while playback is active.
- `PhaseBuffer::prevPhase` must always be ≥ the largest `fftSize/2` in use; resizing is
  grow-only and `prepareToPlay` sizes it before the first block. `prepareToPlay` can run
  directly at a non-default FFT size (project load), in which case per-processor
  `setFFTSize` never runs — never assume it does.
- Each plugin gets a unique 4-char `pluginCode` in its `add_plugin(...)` call;
  manufacturer is always `STPW`. GarageBand needs exactly one uppercase char (see root `CMakeLists.txt`).
- Version is defined once in root `CMakeLists.txt` (`project(... VERSION ...)` and
  `VERSION_CODE`). Bump `VERSION_CODE` or the in-app update check treats new builds as old.
  `SpectralSuiteBuild/Base.jucer` mirrors it for the legacy release path only.
- README claims the Steinberg VST SDK must be copied into `shared/` — that is stale;
  JUCE 8 bundles the VST3 SDK and nothing references those folders.

## Adding a plugin

`makeSpectral.py` copies boilerplate from `SpectralSuiteBuild/Base*` + `UiContainer`
and renames it. It requires `-p`, `-n` **and `-c` (plugin code)** — the README usage
omits `-c`. After running it, wire up manually: `add_subdirectory(<Dir>)` in the root
`CMakeLists.txt`, a matching `<Dir>/CMakeLists.txt` calling `add_plugin(...)`, and the
`Dockerfile` `COPY` + artefact-copy list (its TODO notes this).

## Release

- Linux: `Dockerfile` + `SpectralSuiteBuild/buildLinux.sh` (Docker build, copies
  artefacts to top-level `release/linux-x86_64`).
- Legacy Projucer flow: `SpectralSuiteBuild/ssutil.rb` requires a `projucer` CLI alias
  in the shell; `ssutil.ps1` is the Windows equivalent.
- `moveReleaseFiles.sh` / `win-moveReleaseFiles.ps1` collect artefacts from
  `cmake-build-release/` and `spectral-suite-win/` respectively — those exact build
  directory names are assumed.

## Conventions

- `.editorconfig`: 4-space indent, LF, final newline, trimmed trailing whitespace.
- Comments describe the current state, never history: no bug numbers, no "previously
  X did Y", no restating what the code already says. An issue number belongs only on
  the specific test method that encodes the regression — never on a generically-named
  file or class. A general comment is fine where behavior is genuinely unexpected, kept
  to the *why*.
- `release-backup/` and `.Dockerfile-bak.swp` are stray local artefacts, not source.
