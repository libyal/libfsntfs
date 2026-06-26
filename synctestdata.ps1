# Script that synchronizes the local test data

$TestsInputDirectory = "tests\input"
$TestSet = "public"
$TestFiles = "ntfs.raw"

If (-Not (Test-Path "${TestsInputDirectory}\.fsntfsinfo_bodyfile_fs"))
{
	New-Item -Name "${TestsInputDirectory}\.fsntfsinfo_bodyfile_fs" -ItemType "directory" | Out-Null
	Write-Output "-Bbodyfile -H" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.fsntfsinfo_bodyfile_fs\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.fsntfsinfo_bodyfile_mft"))
{
	New-Item -Name "${TestsInputDirectory}\.fsntfsinfo_bodyfile_mft" -ItemType "directory" | Out-Null
	Write-Output "-Bbodyfile -Eall" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.fsntfsinfo_bodyfile_mft\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.fsntfsinfo_fs"))
{
	New-Item -Name "${TestsInputDirectory}\.fsntfsinfo_fs" -ItemType "directory" | Out-Null
	Write-Output "-H" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.fsntfsinfo_fs\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.fsntfsinfo_mft"))
{
	New-Item -Name "${TestsInputDirectory}\.fsntfsinfo_mft" -ItemType "directory" | Out-Null
	Write-Output "-Eall" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.fsntfsinfo_mft\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.fsntfsinfo_usn"))
{
	New-Item -Name "${TestsInputDirectory}\.fsntfsinfo_usn" -ItemType "directory" | Out-Null
	Write-Output "-U" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.fsntfsinfo_usn\options"
}

If (-Not (Test-Path ${TestsInputDirectory}))
{
	New-Item -Name ${TestsInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestsInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestsInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/log2timeline/dfvfs/refs/heads/main/test_data/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestsInputDirectory}\${TestSet}\${TestFile}"
}
