
function recreateProjectFiles {
    Projucer.exe --resave $args[0]
}

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

filter buildRelease {         
    $solution = [IO.Path]::Combine($_, "..", "Builds", "VisualStudio*", "*.sln")
    $solution = Resolve-Path $solution        
    $projectName = (get-item $solution).BaseName

    $sharedCodeProject = $projectName + "_SharedCode"
    $vstLegacyProject = $projectName + "_VST"
    $vst3Project = $projectName + "_VST3"

    echo $sharedCodeProject
    echo $vstLegacyProject
    echo $vst3Project

    devenv /Rebuild "Release" /Project  $sharedCodeProject $solution
    # devenv /Rebuild "Release" /Project $vstLegacyProject $solution
    # devenv /Rebuild "Release" /Project $vst3Project  $solution

    # devenv /Rebuild "Release32" /Project $sharedCodeProject $solution
    # devenv /Rebuild "Release32" /Project $vstLegacyProject $solution
    # devenv /Rebuild "Release32" /Project $vst3Project  $solution          
    
    # devenv /Rebuild "Release" '.\Bin Scrambler.sln'
    # devenv /Rebuild "Release32" /Project 'Bin Scrambler_SharedCode'  '.\Bin Scrambler.sln'
    # devenv /Rebuild "Release32" /Project 'Bin Scrambler_VST'  '.\Bin Scrambler.sln'
    # devenv /Rebuild "Release32" /Project 'Bin Scrambler_VST3'  '.\Bin Scrambler.sln'
    
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

$errors = $pluginJucerPaths | buildRelease
if($errors) {
    echo $errors
    return
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