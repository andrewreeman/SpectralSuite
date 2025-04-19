# Remove the test-release directory if it exists
Remove-Item -Recurse -Force -ErrorAction SilentlyContinue test-release

# Create the necessary directories
New-Item -ItemType Directory -Force -Path "test-release/Windows/VST3"

# Copy VST3 files to the target directory
Get-ChildItem -Path "spectral-suite-win/*/*_artefacts/Release/VST3/*.vst3" |
    ForEach-Object { Copy-Item -Recurse -Path $_.FullName -Destination "test-release/Windows/VST3/" }