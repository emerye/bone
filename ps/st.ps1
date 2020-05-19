<# Move pdf files into seperate folders based on name

#>
Set-StrictMode -Version 1
$newName = ""

$report_folder = "c:\users\andy\downloads"

Get-WmiObject -Class win32_OperatingSystem | Select-Object Caption
Write-Output ""

$files = Get-ChildItem -Path $report_folder\*.pdf 

foreach($name in $files) {
    Write-Output ("File: " + $name.Name);
    $newName = $name.Name.Substring(5) | Write-Output;
    Write-Output("");
}

$dirs = Get-ChildItem -Path $report_folder -Depth 0 -Directory
foreach($dir in $dirs) {
    Write-Output ("Directory: " + $dir.Name)
}

#Write-Host -Object ('The key that was pressed was: {0}' -f [System.Console]::ReadKey().Key.ToString());

