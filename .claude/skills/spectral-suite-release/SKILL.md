---
name: spectral-suite-release
description: Publish a new Spectral Suite version end to end — version bump, VERSION_CODE, CHANGES.md, release/x.y.z branch, CI-built installers and archives for macOS, Windows and Linux, the vX.Y.Z tag, GitHub release notes, and the spectral_suite_publish.json the in-app update check reads. Use whenever the user wants to release, ship, cut, tag or publish Spectral Suite ("let's do 2.3.0", "ship the PhaseLock fix", "prepare a release", "what's needed for the next version"), even if they only mention one piece such as bumping the version or updating the update-check JSON.
---

# Releasing Spectral Suite

A release is a `vX.Y.Z` tag on `main`. Pushing it makes CI build everything and attach
five files to a GitHub release:

| File | Built by |
|---|---|
| `SpectralSuite-vX.Y.Z-macos-universal.pkg` | macOS job (Packages, `SpectralSuiteBuild/buildMacPkg.sh`) |
| `SpectralSuite-vX.Y.Z-macos-universal.zip` | macOS job |
| `SpectralSuite-vX.Y.Z-windows-x64.msi` | Windows job (WiX, `SpectralSuiteBuild/buildWinMsi.ps1`) |
| `SpectralSuite-vX.Y.Z-windows-x64-VST3.zip` | Windows job |
| `SpectralSuite-vX.Y.Z-linux-x86_64-VST3.tar.gz` | Ubuntu job |

Users then hear about it through the in-app update check, which reads
`https://www.andrewreeman.com/spectral_suite_publish.json` and compares its `code`
with the plugin's compiled-in `VERSION_CODE`. The website is not in this repo: the user
uploads that file by hand.

**Merging to `main` and pushing the tag is the point of no return.** Everything before
it is local or on a branch and can be redone. Do steps 1–3 on your own, run step 4
(manual testing) with the user, then stop at step 5 and get an explicit yes before
publishing. A request like "release 2.3.0" is
permission to prepare; it is not approval to publish.

`scripts/release_tool.py` (next to this file) does the exact checks; call it rather
than re-deriving them:

```bash
python3 .claude/skills/spectral-suite-release/scripts/release_tool.py check X.Y.Z
python3 .claude/skills/spectral-suite-release/scripts/release_tool.py publish-json X.Y.Z
```

## 1. Decide the version and gather the changes

```bash
git fetch origin --tags
git tag --list 'v*' --sort=-v:refname | head -1          # last release
git log --oneline <last-tag>..origin/develop
gh pr list --state merged --base develop --limit 50 --json number,title,mergedAt
```

If the user didn't name a version, propose one: patch for fixes only, minor for new
features or parameters, and say why. Draft the changelog bullets from the merged PRs
in user-facing language — what a musician notices ("Fixed Phase Lock crashing when
toggled quickly"), not issue numbers or internals. CI, refactors and tests usually
don't belong unless they change something users can see.

## 2. Prepare the release branch

```bash
git switch -c release/X.Y.Z origin/develop
```

- `CMakeLists.txt`: set `project(SpectralSuite VERSION X.Y.Z)` and increment
  `SET(VERSION_CODE n)` by exactly one. The update check compares only the code, so a
  release without the bump is invisible to existing users.
- `CHANGES.md`: add the new entry at the top, under the `% Spectral Suite` title line,
  in the existing format (`## Version X.Y.Z` then `    * ` bullets).
- Regenerate the macOS installer's readme page, which shows the changelog:
  `(cd SpectralSuiteBuild && pandoc ../CHANGES.md -f markdown -t html -s -o PACKAGE_README.html)`
- Leave the `.jucer` files alone: they belong to the retired Projucer flow and nothing
  ships from them.

Run `release_tool.py check X.Y.Z`. It checks version, code, changelog, readme, that the
tag is unused and the tree is clean (commit first; it ignores untracked files). Fix
whatever it lists, then:

```bash
git commit -am "Release X.Y.Z"
git push -u origin release/X.Y.Z
gh pr create --base main --head release/X.Y.Z --title "Release X.Y.Z" --body-file <notes>
```

The PR is what runs CI on the release branch (branch pushes alone only trigger CI on
`develop`/`master`).

## 3. Get CI green

The full run takes about 30 minutes (macOS is the slowest). Wait in the background
rather than polling in short loops, e.g.
`gh run watch <run-id> --exit-status > /dev/null 2>&1; gh run view <run-id>`.

CI does more than compile: it installs and upgrades the macOS pkg, installs and
uninstalls the MSI, checks the plugins don't need the VC++ runtime, and runs the
headless tests on macOS and Linux. Fix any failure on the release branch and push
again. Then download and sanity-check the artefacts:

```bash
gh run download <run-id> -D /tmp/spectral-suite-X.Y.Z
ls -R /tmp/spectral-suite-X.Y.Z    # five files, all carrying X.Y.Z
```

## 4. Manual testing (the user, with the CI build)

CI can't open a DAW, so the user tests the release-branch build by hand before
anything is published. Your part is to set it up, keep the record, and chase
findings — the user does the listening and clicking. They may test over several
sessions; carry on recording as results arrive.

**Set up**

- Give the user the run link and which artefacts to install: the macOS
  `-installer` pkg (download it in a browser, so the Gatekeeper "Open Anyway"
  path users hit is tested too) and the Windows `-installer` MSI. Installing
  needs their admin password, so they install, not you.
- Make sure nothing shadows the build under test. Development builds in
  `~/Library/Audio/Plug-Ins/{Components,VST3}` (CMake doesn't copy there, but
  earlier debugging sessions did) win over or duplicate the `/Library` install —
  move them aside, don't delete them. After installing, confirm the receipt
  (`pkgutil --pkg-info com.stepwise.pkg.specsuite.AU`) shows the new version
  and today's install time.

**Record** results in `.vibe/testing/vX-Y-Z.md` (dashes for dots). It's
gitignored on purpose — the user doesn't want release test records committed.
Note the CI run and commit under test, then follow the shape of
`Testing/TestingTemplate.md` (host → format → each plugin: parameters work,
presets load/save) plus:

- `./auValidate.sh` (`auval -strict` on all 7 AUs) — must pass
- REAPER on macOS, AU and VST3, all 7 plugins
- Runtime FFT size / overlap / style / window changes during playback, and a
  project saved at a non-default FFT size reloading cleanly (#97)
- Phase Lock: rapid toggling and automation (#101); a short listen, including
  for the brief silence when FFT settings change mid-playback
- Other hosts the user has (FL Studio, Ableton) — load all 7
- Windows: a quick load-and-play in REAPER or FL Studio
- Linux: no manual test; the headless tests in the Ubuntu CI job are the gate

Which items are required versus nice-to-have is the user's call; ask rather
than block on FL Studio or Ableton if they don't have them to hand.

**Findings**

- Something to fix for this release: branch from `release/X.Y.Z`, PR back into
  it, have the user verify with that PR's CI build, then record it. The fix
  reaches `develop` when the release is merged back.
- Something that can wait: find or open the GitHub issue (check for an existing
  one first) with what was seen and, if you can tell from the code, the cause.
- Note every finding and its outcome in the record either way.

## 5. Confirm before publishing

Stop and show the user, in one message:

- the version and `VERSION_CODE` (old → new)
- the CHANGES.md entry as it will appear
- the CI run link and the five artefact names
- the manual testing summary from `.vibe/testing/vX-Y-Z.md`: what passed,
  findings and how each was resolved, and anything not tested
- what happens next: merge the PR to `main`, push `vX.Y.Z`, CI publishes the release

Ask directly whether to publish. Wait for a clear yes.

## 6. Publish

```bash
gh pr merge <pr> --merge                        # merge commit, like earlier releases
git fetch origin main
git tag -a vX.Y.Z origin/main -m "Spectral Suite X.Y.Z"
git push origin vX.Y.Z
```

The tag push starts a CI run whose release job creates the GitHub release and
attaches the five files. When it finishes:

```bash
gh release view vX.Y.Z --json assets -q '.assets[].name'   # expect the five files
gh release edit vX.Y.Z --notes-file <release-notes.md>
```

Release notes template:

```markdown
## What's new
* <CHANGES.md bullets>

## Installing
- **macOS** (Apple Silicon and Intel): `SpectralSuite-vX.Y.Z-macos-universal.pkg`
  installs the AU and VST3 plugins. The installer isn't signed by Apple, so the first
  time macOS blocks it: open System Settings → Privacy & Security and choose
  "Open Anyway". The `.zip` has the same plugins for manual install.
- **Windows** (64-bit): `SpectralSuite-vX.Y.Z-windows-x64.msi` installs the VST3 plugins
  to `C:\Program Files\Common Files\VST3\Spectral Suite`. If SmartScreen warns, choose
  "More info" → "Run anyway". The `.zip` has the same plugins for manual install.
- **Linux** (x86_64): extract the `.tar.gz` and copy the `.vst3` folders to `~/.vst3`.
```

Then bring `develop` up to date with the release merge:

```bash
gh pr create --base develop --head main --title "Merge release X.Y.Z back into develop" --body "Release X.Y.Z"
```

and merge it once CI passes.

## 7. Update the in-app update check

Generate the JSON and hand it to the user to upload:

```bash
python3 .claude/skills/spectral-suite-release/scripts/release_tool.py publish-json X.Y.Z
```

The notes come from CHANGES.md; the user may tidy the wording. Keep `release_url` as
the website page unless they say otherwise. Once they've uploaded it, confirm the
live file matches:

```bash
curl -sS https://www.andrewreeman.com/spectral_suite_publish.json
```

Also remind them to check that the website's download links point at the new
release's file names.

## Things that go wrong

- **Forgot `VERSION_CODE`** — the check script catches it. If a release already went
  out without it, the fix is a new patch release with the bump; tags are not moved.
- **Packages checksum failure in the macOS job** — a new Packages version was
  published. Update `PACKAGES_SHA256` in `.github/workflows/ci.yml` after verifying
  the new download's signature (`pkgutil --check-signature`).
- **Tag pushed but release job failed** — fix on `main`, then re-run the failed jobs
  from the Actions page (`gh run rerun <run-id> --failed`). Don't delete and re-push a
  published tag; people may already have downloaded it.
- **macOS signing** — the plugins are ad-hoc signed and the pkg is unsigned. There is
  no paid Apple Developer account, so don't suggest notarization as a fix.
