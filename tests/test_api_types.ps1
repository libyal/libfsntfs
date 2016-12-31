# Library API type testing script
#
# Version: 20161110

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$TestPrefix = Split-Path -path ${Pwd}.Path -parent
$TestPrefix = Split-Path -path ${TestPrefix} -leaf
$TestPrefix = ${TestPrefix}.Substring(3)

$TestTypes = "attribute bitmap_values cluster_block compressed_block_descriptor data_run data_stream directory_entry file_entry file_name_values index index_entry index_node index_value io_handle logged_utility_stream_values mft mft_entry mft_metadata_file object_identifier_values reparse_point_values security_descriptor_index security_descriptor_index_value security_descriptor_values standard_information_values txf_data_values usn_change_journal volume_information_values volume_name_values"
$TestTypesWithInput = "volume"

$TestToolDirectory = "..\msvscpp\Release"

Function TestAPIType
{
	param( [string]$TestType )

	$TestDescription = "Testing API type: ${TestType}"
	$TestExecutable = "${TestToolDirectory}\${TestPrefix}_test_${TestType}.exe"

	$Output = Invoke-Expression ${TestExecutable}
	$Result = ${LastExitCode}

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} " -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host " (FAIL)"
	}
	Else
	{
		Write-Host " (PASS)"
	}
	Return ${Result}
}

If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2010\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	Write-Host "Missing test tool directory." -foreground Red

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestType} in ${TestTypes} -split " ")
{
	$Result = TestAPIType ${TestType}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Foreach (${TestType} in ${TestTypesWithInput} -split " ")
{
	$Result = TestAPIType ${TestType}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${Result}

