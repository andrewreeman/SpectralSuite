# Builds package\SpectralSuite-v<version>-windows-x64.msi from the VST3 bundles of
# a CMake Release build. Requires the WiX 5 .NET tool and its UI extension:
#   dotnet tool install --global wix --version 5.0.2
#   wix extension add --global WixToolset.UI.wixext/5.0.2
#
# Usage: SpectralSuiteBuild\buildWinMsi.ps1 [-BuildDir build]

param([string]$BuildDir = "build")
$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$BuildDir = Resolve-Path $BuildDir

$match = Select-String -Path (Join-Path $repoRoot "CMakeLists.txt") -Pattern 'project\(SpectralSuite VERSION ([0-9]+\.[0-9]+\.[0-9]+)\)'
if (-not $match) { throw "could not read project version" }
$version = $match.Matches[0].Groups[1].Value

$stage = Join-Path $PSScriptRoot "package\windows"
Remove-Item -Recurse -Force $stage -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path (Join-Path $stage "VST3") | Out-Null

$bundles = @(Get-ChildItem -Path (Join-Path $BuildDir "*\*_artefacts\Release\VST3\*.vst3") -Directory)
if ($bundles.Count -ne 7) { throw "expected 7 VST3 bundles in $BuildDir, found $($bundles.Count)" }
$bundles | Copy-Item -Recurse -Destination (Join-Path $stage "VST3")

# WixUI shows the license as RTF: the project license followed by third-party notices.
$text = (Get-Content (Join-Path $repoRoot "LICENSE.md") -Raw) + "`n`n" + (Get-Content (Join-Path $repoRoot "NOTICE.txt") -Raw)
$escaped = $text.Replace('\', '\\').Replace('{', '\{').Replace('}', '\}') -replace "`r?`n", "\par`n"
$licenseRtf = Join-Path $stage "License.rtf"
Set-Content -Path $licenseRtf -Encoding ascii -Value "{\rtf1\ansi\deff0{\fonttbl{\f0 Segoe UI;}}\f0\fs18 $escaped}"

$msi = Join-Path $PSScriptRoot "package\SpectralSuite-v$version-windows-x64.msi"
wix build (Join-Path $PSScriptRoot "windows\SpectralSuite.wxs") `
    -arch x64 `
    -ext WixToolset.UI.wixext `
    -d "Version=$version" `
    -d "BundleDir=$(Join-Path $stage 'VST3')" `
    -d "LicenseRtf=$licenseRtf" `
    -o $msi
if ($LASTEXITCODE -ne 0) { throw "wix build failed" }

Write-Output $msi
