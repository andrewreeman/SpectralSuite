# AGENTS.md

SpectralSuite: a CMake / JUCE 8 suite of 7 audio plugins (AU + VST3) that share a
spectral-processing framework in `shared/`. One root CMake project builds them all.

## Build

```bash
cmake -B build -G <generator>     # from repo root; fetches JUCE 8.0.6 via CPM
cmake --build build --config Release
```

- Single plugin: `cmake --build build --target <target>_All --config Release`.
  The plain `<target>` umbrella only builds SharedCode — it does not link the
  AU/VST3 bundles. CMake targets differ from directory names: `BinScrambler`,
  `FreqMagnet`, `FreqShift`, `Morph`, `PhaseLock`, `Ssf` (SinusoidalShapedFilter),
  `SpecGate`.
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

## CI

`.github/workflows/ci.yml` runs on pushes to `develop`/`main`, on PRs, and on
`v*` tags. Jobs: macOS (`macos-15`, universal AU + VST3, `lipo` + ad-hoc
`codesign` verified, headless tests), Windows (`windows-latest`, VST3 x64 zip +
WiX MSI, install/uninstall tested — opts out of warnings-as-error until the MSVC
baseline is audited), and Ubuntu
(`ubuntu-24.04`: build + headless `SpectralSuiteTests` — the Linux quality gate — and
the shipped Linux VST3 `tar.gz`). The macOS job also builds the `.pkg` installer and a
zip of the bundles (see Release). Every run uploads each file as its own artefact; tag
builds attach them all to the GitHub release. `auval` stays a local release gate.

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
`Dockerfile` `COPY` + artefact-copy list (its TODO notes this), the bundle entries in
`SpectralSuiteBuild/SpectralSuitePackager.pkgproj.template`, and the `PLUGINS` lists in
`SpectralSuiteBuild/pre-install-au.sh` / `post-install-au.sh`.

## Release

- macOS: `SpectralSuiteBuild/buildMacPkg.sh [build-dir]` stages a CMake Release build
  into `release/MacOSX/` and runs `packagesbuild` (the Packages app) on
  `SpectralSuitePackager.pkgproj.template`, whose paths are relative to
  `SpectralSuiteBuild/`. Output: `SpectralSuiteBuild/package/MacOS-SpectralSuite-v<version>.pkg`.
  The plugins are ad-hoc signed only (no paid Apple Developer account), so the pkg is
  unsigned; its post-install scripts clear the quarantine flag. CI pins the Packages
  download by SHA-256 — a new Packages release needs the hash in `ci.yml` updated.
- Windows: `SpectralSuiteBuild/buildWinMsi.ps1 [-BuildDir build]` builds
  `SpectralSuiteBuild/package/SpectralSuite-v<version>-windows-x64.msi` from
  `SpectralSuiteBuild/windows/SpectralSuite.wxs` with WiX 5.0.2 (pinned; later majors
  carry a maintenance-fee EULA). Per-machine install of the VST3 bundle folders to
  `Common Files\VST3\Spectral Suite\`; keep the `UpgradeCode` so upgrades replace older
  installs. The C++ runtime is linked statically (`CMAKE_MSVC_RUNTIME_LIBRARY`), so no
  VC++ redistributable is needed; CI fails if a plugin imports it.
- Linux: the shipped `tar.gz` comes from the CI Ubuntu job. `Dockerfile` +
  `SpectralSuiteBuild/buildLinux.sh` remain for local builds (copies artefacts to
  top-level `release/linux-x86_64`); it configures with no `CMAKE_BUILD_TYPE`.
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
