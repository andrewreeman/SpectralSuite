param( [switch]$build = $false )


function Build-Release {
    param($jucerFile, $version)

    # TODO: don't invoke this for every file
    $output = Invoke-WebRequest "http://andrewreeman.github.io/spectral_suite_publish.json"
    $json = $output.Content | ConvertFrom-Json
    
    # loading base file every time as we make mutable changes to this variable's contents
    $basePath = Get-Location | Join-Path -ChildPath "base.jucer"
    $baseProjucer = New-Object -TypeName XML
    $baseProjucer.Load($basePath)    
    $projectVersion = $baseProjucer.JUCERPROJECT.version    
    
    
    if($projectVersion -eq $json.version) {
        Write-Error "You have not updated the version"
        exit
    }
    
    $definesString = $baseProjucer.JUCERPROJECT.defines
    $versionCodeString = $definesString.split("`n") | Where-Object { $_.contains("VersionCode")}
    $projectVersionCode = $versionCodeString.split("=")[1]

    if($projectVersionCode -eq $json.code) {
        Write-Error "You have not updated the version code"
        exit
    }
        
    # recreate
    Projucer.exe --resave $jucerFile | Write-Host

    $solution = [IO.Path]::Combine($jucerFile, "..", "Builds", "VisualStudio2022*", "*.sln")
    $solution = Resolve-Path $solution        
    $projectName = (get-item $solution).BaseName

    $sharedCodeProject = $projectName + "_SharedCode"    
    $vst3Project = $projectName + "_VST3"

    Write-Host "Building $solution for project $sharedCodeProject"    
    devenv.com "$solution" /Clean "Release" /Project "$sharedCodeProject"
    devenv.com "$solution" /Clean "Release" /Project "$vst3Project"

    Write-Output "Building shared code 64 bit project for $projectName"
    devenv.com /Build "Release" /Project "$sharedCodeProject" "$solution"
    
    Write-Output "Building vst3 64 bit project for $projectName"
    devenv.com /Build "Release" /Project "$vst3Project"  "$solution"
        

    $releaseDir = [IO.Path]::Combine($jucerFile, "..", "..", "Release")
    $releaseDir = Resolve-Path $releaseDir    

    $vst3_64 = [IO.Path]::Combine($jucerFile, "..", "Builds", "VisualStudio2022", "x64", "Release", "VST3", $projectName + ".vst3", "Contents", "x86_64-win", "*.vst3")
    $vst3_64Target = [IO.Path]::Combine($releaseDir, $projectName + ".vst3")
    Write-Output "Copying from $vst3_64 to $vst3_64Target"
    Copy-Item $vst3_64 $vst3_64Target
}

$root = Get-Location
$pluginJucerPath = Get-Location | Join-Path -ChildPath "../*/*.jucer"
$pluginJucerPath = Resolve-Path -Path $pluginJucerPath
$pluginJucerPaths = $pluginJucerPath | Where-Object {$_.ToString() -notlike "*SpectralSuiteBuild*"}


$basePath = Get-Location | Join-Path -ChildPath "base.jucer"
$baseProjucer = New-Object -TypeName XML
$baseProjucer.Load($basePath)
$version = $baseProjucer.JUCERPROJECT.version

ForEach ($pluginPath in $pluginJucerPaths) {
   Build-Release $pluginPath $version
}

Set-Location $root
Set-Location "..\spectral-suite"
Remove-Item x64 -Recurse -ErrorAction Ignore

# update product and package code and version number in installer project
# Define the path to the .vdproj file and the version
$vdprojPath = "SpectralSuiteInstaller/SpectralSuiteInstaller.vdproj"

# Generate new GUIDs for ProductCode and PackageCode
$newProductCode = [guid]::NewGuid().ToString().ToUpper()
$newPackageCode = [guid]::NewGuid().ToString().ToUpper()

# Read the .vdproj file content
$vdprojContent = Get-Content -Path $vdprojPath -Raw

# Update the ProductVersion
$vdprojContent = $vdprojContent -replace '("ProductVersion" = "8:)([\d\.]+)(")', "`${1}$version`${3}"

# Update the ProductCode (GUID)
$vdprojContent = $vdprojContent -replace '("ProductCode" = "8:\{)([A-Z0-9\-]+)(\}")', "`${1}$newProductCode`${3}"

# Update the PackageCode (GUID)
$vdprojContent = $vdprojContent -replace '("PackageCode" = "8:\{)([A-Z0-9\-]+)(\}")', "`${1}$newPackageCode`${3}"

# Write the updated content back to the .vdproj file
Set-Content -Path $vdprojPath -Value $vdprojContent

# Output the changes
Write-Output "Updated ProductVersion to $version"
Write-Output "Updated ProductCode to $newProductCode"
Write-Output "Updated PackageCode to $newPackageCode"

# sometimes the exclude filters are ignored on when rebuilding and on the first 'build'. doing this 3 times ensures this eventually works, build after rebuild simply packages files
# using Wix will probably fix this but not worth it yet
devenv.com "spectral-suite.sln" /rebuild Release
devenv.com "spectral-suite.sln" /build Release
devenv.com "spectral-suite.sln" /build Release

Write-Output "Built release files"
Set-Location $root