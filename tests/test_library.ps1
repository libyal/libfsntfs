# Tests library functions and types.
#
# Version: 20200427

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$LibraryTests = "attribute attribute_list_entry bitmap_values buffer_data_handle cluster_block cluster_block_data cluster_block_stream cluster_block_vector compressed_block compressed_block_data_handle compressed_block_vector compressed_data_handle compression compression_unit_data_handle compression_unit_descriptor data_run data_stream directory_entries_tree directory_entry error extent file_entry file_name_attribute file_name_values file_system fixup_values index index_entry index_entry_header index_entry_vector index_node index_node_header index_root_header index_value io_handle logged_utility_stream_values mft mft_attribute mft_attribute_list mft_attribute_list_entry mft_entry mft_entry_header name notify object_identifier_values path_hint profiler reparse_point_attribute reparse_point_values sds_index_value security_descriptor_index security_descriptor_index_value security_descriptor_values standard_information_values txf_data_values usn_change_journal volume_header volume_information_attribute volume_information_values volume_name_attribute volume_name_values"
$LibraryTestsWithInput = "mft_metadata_file support volume"
$OptionSets = "offset"

$InputGlob = "*"

Function GetTestExecutablesDirectory
{
	$TestExecutablesDirectory = ""

	ForEach (${VSDirectory} in "msvscpp vs2008 vs2010 vs2012 vs2013 vs2015 vs2017 vs2019" -split " ")
	{
		ForEach (${VSConfiguration} in "Release VSDebug" -split " ")
		{
			ForEach (${VSPlatform} in "Win32 x64" -split " ")
			{
				$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}\${VSPlatform}"

				If (Test-Path ${TestExecutablesDirectory})
				{
					Return ${TestExecutablesDirectory}
				}
			}
			$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}"

			If (Test-Path ${TestExecutablesDirectory})
			{
				Return ${TestExecutablesDirectory}
			}
		}
	}
	Return ${TestExecutablesDirectory}
}

Function ReadIgnoreList
{
	param( [string]$TestProfileDirectory )

	$IgnoreFile = "${TestProfileDirectory}\ignore"
	$IgnoreList = ""

	If (Test-Path -Path ${IgnoreFile} -PathType "Leaf")
	{
		$IgnoreList = Get-Content -Path ${IgnoreFile} | Where {$_ -notmatch '^#.*'}
	}
	Return $IgnoreList
}

Function RunTest
{
	param( [string]$TestType )

	$TestDescription = "Testing: ${TestName}"
	$TestExecutable = "${TestExecutablesDirectory}\fsntfs_test_${TestName}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType "Leaf"))
	{
		Write-Host "${TestDescription} (" -nonewline
		Write-Host "SKIP" -foreground Cyan -nonewline
		Write-Host ")"

		Return ${ExitIgnore}
	}
	$Output = Invoke-Expression ${TestExecutable}
	$Result = ${LastExitCode}

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} (" -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host "FAIL" -foreground Red -nonewline
	}
	Else
	{
		Write-Host "PASS" -foreground Green -nonewline
	}
	Write-Host ")"

	Return ${Result}
}

Function RunTestWithInput
{
	param( [string]$TestType )

	$TestDescription = "Testing: ${TestName}"
	$TestExecutable = "${TestExecutablesDirectory}\fsntfs_test_${TestName}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType "Leaf"))
	{
		Write-Host "${TestDescription} (" -nonewline
		Write-Host "SKIP" -foreground Cyan -nonewline
		Write-Host ")"

		Return ${ExitIgnore}
	}
	$TestProfileDirectory = "input\.libfsntfs"

	If (-Not (Test-Path -Path ${TestProfileDirectory} -PathType "Container"))
	{
		New-Item -ItemType "directory" -Path ${TestProfileDirectory}
	}
	$IgnoreList = ReadIgnoreList ${TestProfileDirectory}

	$Result = ${ExitSuccess}

	ForEach ($TestSetInputDirectory in Get-ChildItem -Path "input" -Exclude ".*")
	{
		If (-Not (Test-Path -Path ${TestSetInputDirectory} -PathType "Container"))
		{
			Continue
		}
		If (${TestSetInputDirectory} -Contains ${IgnoreList})
		{
			Continue
		}
		$TestSetName = ${TestSetInputDirectory}.Name

		If (Test-Path -Path "${TestProfileDirectory}\${TestSetName}\files" -PathType "Leaf")
		{
			$InputFiles = Get-Content -Path "${TestProfileDirectory}\${TestSetName}\files" | Where {$_ -ne ""}
		}
		Else
		{
			$InputFiles = Get-ChildItem -Path ${TestSetInputDirectory} -Include ${InputGlob}
		}
		ForEach ($InputFile in ${InputFiles})
		{
			$TestedWithOptions = $False

			ForEach ($OptionSet in ${OptionSets} -split " ")
			{
				$InputFileName = ${InputFile}.Name
				$TestDataOptionFile = "${TestProfileDirectory}\${TestSetName}\${InputFileName}.${OptionSet}"

				If (-Not (Test-Path -Path "${TestDataOptionFile}" -PathType "Leaf"))
				{
					Continue
				}
				$InputOptions = Get-content -Path "${TestDataOptionFile}" -First 1

				$Output = Invoke-Expression "${TestExecutable} ${InputOptions} ${InputFile}"
				$Result = $LastExitCode

				If (${Result} -ne ${ExitSuccess})
				{
					Break
				}
				$TestedWithOptions = $True
			}
			If ((${Result} -eq ${ExitSuccess}) -And (-Not (${TestedWithOptions})))
			{
				$Output = Invoke-Expression "${TestExecutable} ${InputFile}"
				$Result = ${LastExitCode}
			}
			If (${Result} -ne ${ExitSuccess})
			{
				Break
			}
		}
		If (${Result} -ne ${ExitSuccess})
		{
			Break
		}
	}
	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} (" -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host "FAIL" -foreground Red -nonewline
	}
	Else
	{
		Write-Host "PASS" -foreground Green -nonewline
	}
	Write-Host ")"

	Return ${Result}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Host "Missing test executables directory." -foreground Red

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTest ${TestName}

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	If (Test-Path -Path "input" -PathType "Container")
	{
		$Result = RunTestWithInput ${TestName}
	}
	Else
	{
		$Result = RunTest ${TestName}
	}
	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Exit ${Result}

