#$plugins = "BinScrambler", "FrequencyMagnet", "FrequencyShift", "SpectralGate", "SSF"



$basePath = Get-Location | Join-Path -ChildPath "base.jucer"
$pluginJucerPath = Get-Location | Join-Path -ChildPath "../*/*.jucer"
$pluginJucerPath = Resolve-Path -Path $pluginJucerPath
$pluginJucerPaths = $pluginJucerPath | where {$_.ToString() -notlike "*SpectralSuiteBuild*"}
$outPath = Get-Location | Join-Path -ChildPath "out.jucer"
#$XmlWriter = New-Object System.XMl.XmlTextWriter($outPath,$Null)

$baseProjucer = New-Object -TypeName XML
$baseProjucer.Load($basePath)

$projectName = "New title"
$projectDescription = "New description"

$baseProjucer.JUCERPROJECT.name = $projectName
$baseProjucer.JUCERPROJECT.pluginDesc = $projectDescription
$baseProjucer.JUCERPROJECT.MAINGROUP.name = $projectName

$baseProjucer.save($outPath)




# run Projucer.exe --resave project