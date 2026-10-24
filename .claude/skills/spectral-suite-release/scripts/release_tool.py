#!/usr/bin/env python3
"""Release helpers for Spectral Suite.

  release_tool.py check VERSION
      Verify the working tree is ready to release VERSION: CMake version and
      VERSION_CODE, CHANGES.md entry, regenerated installer readme, unused tag.
      Prints every problem found and exits 1 if there are any.

  release_tool.py publish-json VERSION [--release-url URL]
      Print the spectral_suite_publish.json the in-app update check reads,
      built from CMakeLists.txt and the CHANGES.md entry for VERSION.

Run from anywhere inside the repository.
"""

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

DEFAULT_RELEASE_URL = "https://www.andrewreeman.com/spectralsuite"


def repo_root() -> Path:
    out = subprocess.run(["git", "rev-parse", "--show-toplevel"], capture_output=True, text=True, check=True)
    return Path(out.stdout.strip())


def git(root: Path, *args: str) -> subprocess.CompletedProcess:
    return subprocess.run(["git", "-C", str(root), *args], capture_output=True, text=True)


def parse_version(text: str):
    return tuple(int(p) for p in text.split("."))


def cmake_version_and_code(cmake_text: str):
    version = re.search(r"^project\(SpectralSuite VERSION ([0-9]+\.[0-9]+\.[0-9]+)\)", cmake_text, re.M)
    code = re.search(r"^SET\(VERSION_CODE ([0-9]+)\)", cmake_text, re.M | re.I)
    return (version.group(1) if version else None, int(code.group(1)) if code else None)


def changes_entry(changes_text: str, version: str):
    """Return the bullet points under '## Version VERSION', joining wrapped lines."""
    lines = changes_text.splitlines()
    try:
        start = next(i for i, l in enumerate(lines) if l.strip() == f"## Version {version}")
    except StopIteration:
        return None
    notes = []
    for line in lines[start + 1:]:
        if line.startswith("## "):
            break
        stripped = line.strip()
        if not stripped:
            continue
        if stripped.startswith("* "):
            notes.append(stripped[2:].strip())
        elif notes:
            notes[-1] += " " + stripped
    return notes


def previous_release(root: Path, version: str):
    """Newest vX.Y.Z tag other than VERSION, with the VERSION_CODE it shipped."""
    tags = git(root, "tag", "--list", "v*", "--sort=-v:refname").stdout.split()
    for tag in tags:
        if tag == f"v{version}" or not re.fullmatch(r"v[0-9]+\.[0-9]+\.[0-9]+", tag):
            continue
        shown = git(root, "show", f"{tag}:CMakeLists.txt")
        code = cmake_version_and_code(shown.stdout)[1] if shown.returncode == 0 else None
        return tag, code
    return None, None


def check(version: str) -> int:
    root = repo_root()
    problems = []

    cmake_version, code = cmake_version_and_code((root / "CMakeLists.txt").read_text())
    if cmake_version != version:
        problems.append(f"CMakeLists.txt project VERSION is {cmake_version}, expected {version}")

    prev_tag, prev_code = previous_release(root, version)
    if prev_tag:
        if parse_version(version) <= parse_version(prev_tag[1:]):
            problems.append(f"{version} is not newer than the last release {prev_tag}")
        if prev_code is None:
            print(f"note: could not read VERSION_CODE at {prev_tag}; check it by hand")
        elif code != prev_code + 1:
            problems.append(f"VERSION_CODE is {code}, expected {prev_code + 1} ({prev_tag} shipped {prev_code})")

    notes = changes_entry((root / "CHANGES.md").read_text(), version)
    if notes is None:
        problems.append(f"CHANGES.md has no '## Version {version}' entry")
    elif not notes:
        problems.append(f"CHANGES.md entry for {version} has no '* ' bullet points")

    readme = root / "SpectralSuiteBuild" / "PACKAGE_README.html"
    if f'id="version-{version.replace(".", "")}"' not in readme.read_text() and \
       f'id="version-{version}"' not in readme.read_text():
        problems.append("SpectralSuiteBuild/PACKAGE_README.html does not include this version; "
                        "regenerate it with pandoc (see SKILL.md)")

    if git(root, "rev-parse", "-q", "--verify", f"refs/tags/v{version}").returncode == 0:
        problems.append(f"tag v{version} already exists locally")
    remote = git(root, "ls-remote", "--tags", "origin", f"refs/tags/v{version}")
    if remote.returncode == 0 and remote.stdout.strip():
        problems.append(f"tag v{version} already exists on origin")

    dirty = git(root, "status", "--porcelain", "--untracked-files=no").stdout.strip()
    if dirty:
        problems.append("tracked files have uncommitted changes:\n    " + dirty.replace("\n", "\n    "))

    print(f"version {version}, VERSION_CODE {code}, previous release {prev_tag or 'none'} (code {prev_code})")
    if notes:
        print("release notes:")
        for note in notes:
            print(f"  * {note}")
    if problems:
        print("\nNOT READY:")
        for p in problems:
            print(f"  - {p}")
        return 1
    print("\nready")
    return 0


def publish_json(version: str, release_url: str) -> int:
    root = repo_root()
    cmake_version, code = cmake_version_and_code((root / "CMakeLists.txt").read_text())
    if cmake_version != version:
        print(f"CMakeLists.txt is at {cmake_version}, not {version}", file=sys.stderr)
        return 1
    notes = changes_entry((root / "CHANGES.md").read_text(), version)
    if not notes:
        print(f"no CHANGES.md bullet points for {version}", file=sys.stderr)
        return 1
    print(json.dumps({"version": version, "code": code, "release_notes": notes, "release_url": release_url}, indent=2))
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    c = sub.add_parser("check")
    c.add_argument("version")
    p = sub.add_parser("publish-json")
    p.add_argument("version")
    p.add_argument("--release-url", default=DEFAULT_RELEASE_URL)
    args = parser.parse_args()
    if args.command == "check":
        return check(args.version)
    return publish_json(args.version, args.release_url)


if __name__ == "__main__":
    sys.exit(main())
