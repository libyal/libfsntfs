#!/bin/sh
# Script that synchronizes the local test data
#
# Version: 20251217

REPOSITORY="log2timeline/dfvfs";
TEST_DATA_PATH="test_data";
TEST_SET="public";
TEST_INPUT_DIRECTORY="tests/input";
TEST_FILES="ntfs.raw";

mkdir -p "${TEST_INPUT_DIRECTORY}/${TEST_SET}";

mkdir -p "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_fs";
echo "-H" > "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_fs/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_mft";
echo "-Eall" > "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_mft/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_usn";
echo "-U" > "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_usn/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_bodyfile_fs";
echo "-Bbodyfile -H" > "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_bodyfile_fs/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_bodyfile_mft";
echo "-Bbodyfile -Eall" > "${TEST_INPUT_DIRECTORY}/.fsntfsinfo_bodyfile_mft/options";

for TEST_FILE in ${TEST_FILES};
do
	URL="https://raw.githubusercontent.com/${REPOSITORY}/refs/heads/main/${TEST_DATA_PATH}/${TEST_FILE}";

	curl -L -o "${TEST_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL};
done

