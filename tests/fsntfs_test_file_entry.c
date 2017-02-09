/*
 * Library file_entry type test program
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_file_entry.h"

/* Tests the libfsntfs_file_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_file_entry_free(
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( __GNUC__ )

#endif /* defined( __GNUC__ ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSNTFS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSNTFS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSNTFS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSNTFS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ )

	/* TODO: add tests for libfsntfs_file_entry_initialize */

#endif /* defined( __GNUC__ ) */

	FSNTFS_TEST_RUN(
	 "libfsntfs_file_entry_free",
	 fsntfs_test_file_entry_free );

#if defined( __GNUC__ )

	/* TODO: add tests for libfsntfs_file_entry_is_empty */

	/* TODO: add tests for libfsntfs_file_entry_is_allocated */

	/* TODO: add tests for libfsntfs_file_entry_get_file_reference */

	/* TODO: add tests for libfsntfs_file_entry_get_base_record_file_reference */

	/* TODO: add tests for libfsntfs_file_entry_get_parent_file_reference */

	/* TODO: add tests for libfsntfs_file_entry_get_parent_file_reference_by_attribute_index */

	/* TODO: add tests for libfsntfs_file_entry_get_journal_sequence_number */

	/* TODO: add tests for libfsntfs_file_entry_get_creation_time */

	/* TODO: add tests for libfsntfs_file_entry_get_modification_time */

	/* TODO: add tests for libfsntfs_file_entry_get_access_time */

	/* TODO: add tests for libfsntfs_file_entry_get_entry_modification_time */

	/* TODO: add tests for libfsntfs_file_entry_get_file_attribute_flags */

	/* TODO: add tests for libfsntfs_file_entry_get_utf8_name_size */

	/* TODO: add tests for libfsntfs_file_entry_get_utf8_name */

	/* TODO: add tests for libfsntfs_file_entry_get_utf16_name_size */

	/* TODO: add tests for libfsntfs_file_entry_get_utf16_name */

	/* TODO: add tests for libfsntfs_file_entry_get_name_attribute_index */

	/* TODO: add tests for libfsntfs_file_entry_get_utf8_name_size_by_attribute_index */

	/* TODO: add tests for libfsntfs_file_entry_get_utf8_name_by_attribute_index */

	/* TODO: add tests for libfsntfs_file_entry_get_utf16_name_size_by_attribute_index */

	/* TODO: add tests for libfsntfs_file_entry_get_utf16_name_by_attribute_index */

	/* TODO: add tests for libfsntfs_file_entry_get_utf8_reparse_point_substitute_name_size */

	/* TODO: add tests for libfsntfs_file_entry_get_utf8_reparse_point_substitute_name */

	/* TODO: add tests for libfsntfs_file_entry_get_utf16_reparse_point_substitute_name_size */

	/* TODO: add tests for libfsntfs_file_entry_get_utf16_reparse_point_substitute_name */

	/* TODO: add tests for libfsntfs_file_entry_get_utf8_reparse_point_print_name_size */

	/* TODO: add tests for libfsntfs_file_entry_get_utf8_reparse_point_print_name */

	/* TODO: add tests for libfsntfs_file_entry_get_utf16_reparse_point_print_name_size */

	/* TODO: add tests for libfsntfs_file_entry_get_utf16_reparse_point_print_name */

	/* TODO: add tests for libfsntfs_file_entry_get_security_descriptor_size */

	/* TODO: add tests for libfsntfs_file_entry_get_security_descriptor */

	/* TODO: add tests for libfsntfs_file_entry_get_number_of_attributes */

	/* TODO: add tests for libfsntfs_file_entry_get_attribute_by_index */

	/* TODO: add tests for libfsntfs_file_entry_has_directory_entries_index */

	/* TODO: add tests for libfsntfs_file_entry_has_default_data_stream */

	/* TODO: add tests for libfsntfs_file_entry_get_number_of_alternate_data_streams */

	/* TODO: add tests for libfsntfs_file_entry_get_alternate_data_stream_by_index */

	/* TODO: add tests for libfsntfs_file_entry_has_alternate_data_stream_by_utf8_name */

	/* TODO: add tests for libfsntfs_file_entry_has_alternate_data_stream_by_utf16_name */

	/* TODO: add tests for libfsntfs_file_entry_get_alternate_data_stream_by_utf8_name */

	/* TODO: add tests for libfsntfs_file_entry_get_alternate_data_stream_by_utf16_name */

	/* TODO: add tests for libfsntfs_file_entry_get_number_of_sub_file_entries */

	/* TODO: add tests for libfsntfs_file_entry_get_sub_file_entry_by_index */

	/* TODO: add tests for libfsntfs_file_entry_get_sub_file_entry_by_utf8_name */

	/* TODO: add tests for libfsntfs_file_entry_get_sub_file_entry_by_utf16_name */

	/* TODO: add tests for libfsntfs_file_entry_read_buffer */

	/* TODO: add tests for libfsntfs_file_entry_read_buffer_at_offset */

	/* TODO: add tests for libfsntfs_file_entry_seek_offset */

	/* TODO: add tests for libfsntfs_file_entry_get_offset */

	/* TODO: add tests for libfsntfs_file_entry_get_size */

	/* TODO: add tests for libfsntfs_file_entry_get_number_of_extents */

	/* TODO: add tests for libfsntfs_file_entry_get_extent_by_index */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

