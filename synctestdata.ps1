# Script that synchronizes the local test data
#
# Version: 20260608

$Repository = "log2timeline/dfvfs"
$TestDataPath = "test_data"
$TestSet = "public"
$TestInputDirectory = "tests\input"
$TestFiles = "ntfs.raw"

If (-Not (Test-Path ${TestInputDirectory}))
{
	New-Item -Name ${TestInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestInputDirectory}\.fsntfsinfo_sh"))
{
	New-Item -Name "${TestInputDirectory}\.fsntfsinfo_sh" -ItemType "directory" | Out-Null
	Write-Output "-H" | Out-File -Encoding ascii -FilePath "${TestInputDirectory}\.fsntfsinfo_sh\options"
}
If (-Not (Test-Path "${TestInputDirectory}\.fsntfsinfo_mft"))
{
	New-Item -Name "${TestInputDirectory}\.fsntfsinfo_mft" -ItemType "directory" | Out-Null
	Write-Output "-Eall" | Out-File -Encoding ascii -FilePath "${TestInputDirectory}\.fsntfsinfo_mft\options"
}
If (-Not (Test-Path "${TestInputDirectory}\.fsntfsinfo_usn"))
{
	New-Item -Name "${TestInputDirectory}\.fsntfsinfo_usn" -ItemType "directory" | Out-Null
	Write-Output "-U" | Out-File -Encoding ascii -FilePath "${TestInputDirectory}\.fsntfsinfo_usn\options"
}
If (-Not (Test-Path "${TestInputDirectory}\.fsntfsinfo_bodyfile_fs"))
{
	New-Item -Name "${TestInputDirectory}\.fsntfsinfo_bodyfile_fs" -ItemType "directory" | Out-Null
	Write-Output "-Bbodyfile -H" | Out-File -Encoding ascii -FilePath "${TestInputDirectory}\.fsntfsinfo_bodyfile_fs\options"
}
If (-Not (Test-Path "${TestInputDirectory}\.fsntfsinfo_bodyfile_mft"))
{
	New-Item -Name "${TestInputDirectory}\.fsntfsinfo_bodyfile_mft" -ItemType "directory" | Out-Null
	Write-Output "-H -Eall" | Out-File -Encoding ascii -FilePath "${TestInputDirectory}\.fsntfsinfo_bodyfile_mft\options"
}
If (-Not (Test-Path "${TestInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/${Repository}/refs/heads/main/${TestDataPath}/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestInputDirectory}\${TestSet}\${TestFile}"
}

