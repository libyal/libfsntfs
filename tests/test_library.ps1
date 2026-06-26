# Tests library functions and types.

$LibraryTests = "attribute attribute_list_entry bitmap_values buffer_data_handle cluster_block cluster_block_data cluster_block_stream cluster_block_vector compressed_block compressed_block_data_handle compressed_block_vector compressed_data_handle compression compression_unit_data_handle compression_unit_descriptor data_run data_stream directory_entries_tree directory_entry error extent file_entry file_name_attribute file_name_values file_system fixup_values index index_entry index_entry_header index_entry_vector index_node index_node_header index_root_header index_value io_handle logged_utility_stream_values mft mft_attribute mft_attribute_list mft_attribute_list_entry mft_entry mft_entry_header name notify object_identifier_values path_hint profiler reparse_point_attribute reparse_point_values sds_index_value security_descriptor_index security_descriptor_index_value security_descriptor_values standard_information_values txf_data_values usn_change_journal volume_header volume_information_attribute volume_information_values volume_name_attribute volume_name_values"
$LibraryTestsWithInput = "mft_metadata_file support volume"
$OptionSets = "offset" -split " "

. .\test_functions.ps1

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitSuccess}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$ResultRun = RunTestBinary ${TestExecutablesDirectory} "fsntfs_test_${TestName}"

	If ((${ResultRun} -ne ${ExitSuccess}) -And (${ResultRun} -ne ${ExitIgnore}))
	{
		$Result = ${ResultRun}
	}
}

$TestInputs = GenerateTestInputs "libfsntfs" ${OptionSets}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	ForEach ($TestInput in ${TestInputs})
	{
		$ResultRun = RunTestBinaryWithInput ${TestExecutablesDirectory} "fsntfs_test_${TestName}" ${TestInput}

		If ((${ResultRun} -ne ${ExitSuccess}) -And (${ResultRun} -ne ${ExitIgnore}))
		{
			$Result = ${ResultRun}
		}
	}
}

Exit ${Result}
