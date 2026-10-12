#!/bin/bash
# Builds package/MacOS-SpectralSuite-v<version>.pkg from the AU and VST3 bundles
# of a CMake Release build. Requires Packages (packagesbuild):
# http://s.sudre.free.fr/Software/Packages/about.html
#
# Usage: SpectralSuiteBuild/buildMacPkg.sh [cmake-build-dir]   (default: build)

set -euo pipefail
shopt -s nullglob

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
build_dir="$(cd "${1:-$repo_root/build}" && pwd)"
cd "$repo_root/SpectralSuiteBuild"

version=$(sed -nE 's/^project\(SpectralSuite VERSION ([0-9]+\.[0-9]+\.[0-9]+)\).*$/\1/p' ../CMakeLists.txt)
[ -n "$version" ] || { echo "could not read project version" >&2; exit 1; }

components=("$build_dir"/*/*_artefacts/Release/AU/*.component)
vst3s=("$build_dir"/*/*_artefacts/Release/VST3/*.vst3)
[ "${#components[@]}" -eq 7 ] || { echo "expected 7 AU bundles in $build_dir, found ${#components[@]}" >&2; exit 1; }
[ "${#vst3s[@]}" -eq 7 ] || { echo "expected 7 VST3 bundles in $build_dir, found ${#vst3s[@]}" >&2; exit 1; }

# The pkgproj reads the bundles from release/MacOSX, relative to this folder.
rm -rf ../release/MacOSX/AU ../release/MacOSX/VST3
mkdir -p ../release/MacOSX/AU ../release/MacOSX/VST3
cp -R "${components[@]}" ../release/MacOSX/AU/
cp -R "${vst3s[@]}" ../release/MacOSX/VST3/

sed "s/{VERSION}/$version/g" SpectralSuitePackager.pkgproj.template > SpectralSuitePackager.pkgproj
packagesbuild SpectralSuitePackager.pkgproj

pkg="package/MacOS-SpectralSuite-v$version.pkg"
[ -f "$pkg" ] || { echo "packagesbuild did not produce $pkg" >&2; exit 1; }

# Without hostArchitectures, Installer on Apple silicon asks for Rosetta before
# running a package that contains scripts.
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
pkgutil --expand "$pkg" "$tmp/expanded"
if ! grep -q 'hostArchitectures=' "$tmp/expanded/Distribution"; then
    sed -i '' 's#<options/>#<options hostArchitectures="arm64,x86_64"/>#' "$tmp/expanded/Distribution"
    grep -q 'hostArchitectures="arm64,x86_64"' "$tmp/expanded/Distribution" \
        || { echo "could not set hostArchitectures in Distribution" >&2; exit 1; }
    pkgutil --flatten "$tmp/expanded" "$tmp/flat.pkg"
    mv "$tmp/flat.pkg" "$pkg"
fi

echo "$(pwd)/$pkg"
