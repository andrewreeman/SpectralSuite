filter updateFromBase {     

    # loading base file every time as we make mutable changes to this variable's contents
    $basePath = Get-Location | Join-Path -ChildPath "base.jucer"
    $baseProjucer = New-Object -TypeName XML
    $baseProjucer.Load($basePath)

    $pluginJucerXml = New-Object -TypeName XML
    $pluginJucerXml.Load($_)
    $projectName = $pluginJucerXml.JUCERPROJECT.name
    $projectDescription = $pluginJucerXml.JUCERPROJECT.pluginDesc
    
    if(!$projectDescription) {
        echo "You must provide a description for $projectName"
        return        
    }
    $baseProjucer.JUCERPROJECT.name = $projectName
    $baseProjucer.JUCERPROJECT.pluginDesc = $projectDescription
    $baseProjucer.JUCERPROJECT.MAINGROUP.name = $projectName

    # add source code to base project template    
    $projectSourceCode = $pluginJucerXml.JUCERPROJECT.MAINGROUP.GROUP | Where-Object { $_.Name -eq "Source" }           
    $ignoredResult = $baseProjucer.JUCERPROJECT.MAINGROUP.PrependChild( $baseProjucer.ImportNode($projectSourceCode, $true) )
        
    $output = $baseProjucer.save($_)      
}

function Build-Release {
    param($jucerFile, $version)
    
    # recreate
    Projucer.exe --resave $jucerFile | Write-Host

    $solution = [IO.Path]::Combine($jucerFile, "..", "Builds", "VisualStudio*", "*.sln")
    $solution = Resolve-Path $solution        
    $projectName = (get-item $solution).BaseName

    $sharedCodeProject = $projectName + "_SharedCode"
    $vstLegacyProject = $projectName + "_VST"
    $vst3Project = $projectName + "_VST3"

    Write-Host "Building $solution"    
    devenv.com $solution /Clean "Release" /Project $sharedCodeProject | Write-Host    
    devenv.com $solution /Clean "Release" /Project $vstLegacyProject | Write-Host
    devenv.com $solution /Clean "Release" /Project $vst3Project | Write-Host    
    devenv.com $solution /Clean "Release32|Win32" /Project $sharedCodeProject | Write-Host
    devenv.com $solution /Clean "Release32|Win32" /Project $vstLegacyProject | Write-Host   

    devenv.com /Build "Release" /Project $sharedCodeProject $solution | Write-Host
    devenv.com /Build "Release" /Project $vstLegacyProject $solution | Write-Host
    devenv.com /Build "Release" /Project $vst3Project  $solution | Write-Host    

    devenv.com /Build "Release32|Win32" /Project $sharedCodeProject $solution | Write-Host    
    devenv.com /Build "Release32|Win32" /Project $vstLegacyProject $solution | Write-Host    

    $releaseDir = [IO.Path]::Combine($jucerFile, "..", "..", "Release")
    $releaseDir = Resolve-Path $releaseDir
    
    $vst64 = [IO.Path]::Combine($jucerFile, "..", "Builds", "VisualStudio*", "x64", "Release", "VST", "*.dll")        
    $vst64Target = [IO.Path]::Combine($releaseDir, $projectName + "_$version.dll")
    cp $vst64 $vst64Target

    $vst3_64 = [IO.Path]::Combine($jucerFile, "..", "Builds", "VisualStudio*", "x64", "Release", "VST3", "*.vst3")        
    $vst3_64Target = [IO.Path]::Combine($releaseDir, $projectName + "_$version.vst3")
    cp $vst3_64 $vst3_64Target

    $vst32 = [IO.Path]::Combine($jucerFile, "..", "Builds", "VisualStudio*", "Win32", "Release32", "VST", "*.dll")    
    $vst32Target = [IO.Path]::Combine($releaseDir, $projectName + "_Win32_$version.dll")
    cp $vst32 $vst32Target    
}


$pluginJucerPath = Get-Location | Join-Path -ChildPath "../*/*.jucer"
$pluginJucerPath = Resolve-Path -Path $pluginJucerPath
$pluginJucerPaths = $pluginJucerPath | where {$_.ToString() -notlike "*SpectralSuiteBuild*"}

$errors = $pluginJucerPaths | updateFromBase
if($errors) {
    echo $errors
    return
}

echo "Updated projucer files"
$basePath = Get-Location | Join-Path -ChildPath "base.jucer"
$baseProjucer = New-Object -TypeName XML
$baseProjucer.Load($basePath)
$version = $baseProjucer.JUCERPROJECT.version

ForEach ($pluginPath in $pluginJucerPaths) {
    Build-Release $pluginPath $version
}

echo "Built release files"



#$XmlWriter = New-Object System.XMl.XmlTextWriter($outPath,$Null)

# $projectName = "New title"
# $projectDescription = "New description"

# $baseProjucer.JUCERPROJECT.name = $projectName
# $baseProjucer.JUCERPROJECT.pluginDesc = $projectDescription
# $baseProjucer.JUCERPROJECT.MAINGROUP.name = $projectName

# $baseProjucer.save($outPath)




# run Projucer.exe --resave project