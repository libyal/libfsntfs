/*
 * Library mft_entry type test program
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

#include "../libfsntfs/libfsntfs_mft_entry.h"

#if defined( __GNUC__ )

/* Tests the libfsntfs_mft_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_initialize(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_mft_entry_t *mft_entry = NULL;
	int result                       = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 1;
	int number_of_memset_fail_tests  = 1;
	int test_number                  = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "mft_entry",
         mft_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "mft_entry",
         mft_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_initialize(
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

	mft_entry = (libfsntfs_mft_entry_t *) 0x12345678UL;

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
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

	mft_entry = NULL;

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_mft_entry_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_mft_entry_initialize(
		          &mft_entry,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( mft_entry != NULL )
			{
				libfsntfs_mft_entry_free(
				 &mft_entry,
				 NULL );
			}
		}
		else
		{
			FSNTFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSNTFS_TEST_ASSERT_IS_NULL(
			 "mft_entry",
			 mft_entry );

			FSNTFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_mft_entry_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_mft_entry_initialize(
		          &mft_entry,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( mft_entry != NULL )
			{
				libfsntfs_mft_entry_free(
				 &mft_entry,
				 NULL );
			}
		}
		else
		{
			FSNTFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSNTFS_TEST_ASSERT_IS_NULL(
			 "mft_entry",
			 mft_entry );

			FSNTFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_free(
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

/* Tests the libfsntfs_mft_entry_get_file_reference function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_file_reference(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_mft_entry_t *mft_entry = NULL;
	uint64_t file_reference          = 0;
	int file_reference_is_set        = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_file_reference(
	          mft_entry,
	          &file_reference,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_reference_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_file_reference(
	          NULL,
	          &file_reference,
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

	if( file_reference_is_set != 0 )
	{
		result = libfsntfs_mft_entry_get_file_reference(
		          mft_entry,
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
	}
	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_base_record_file_reference function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_base_record_file_reference(
     void )
{
	libcerror_error_t *error              = NULL;
	libfsntfs_mft_entry_t *mft_entry      = NULL;
	uint64_t base_record_file_reference   = 0;
	int base_record_file_reference_is_set = 0;
	int result                            = 0;

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_base_record_file_reference(
	          mft_entry,
	          &base_record_file_reference,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	base_record_file_reference_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_base_record_file_reference(
	          NULL,
	          &base_record_file_reference,
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

	if( base_record_file_reference_is_set != 0 )
	{
		result = libfsntfs_mft_entry_get_base_record_file_reference(
		          mft_entry,
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
	}
	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_journal_sequence_number function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_journal_sequence_number(
     void )
{
	libcerror_error_t *error           = NULL;
	libfsntfs_mft_entry_t *mft_entry   = NULL;
	uint64_t journal_sequence_number   = 0;
	int journal_sequence_number_is_set = 0;
	int result                         = 0;

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_journal_sequence_number(
	          mft_entry,
	          &journal_sequence_number,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	journal_sequence_number_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_journal_sequence_number(
	          NULL,
	          &journal_sequence_number,
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

	if( journal_sequence_number_is_set != 0 )
	{
		result = libfsntfs_mft_entry_get_journal_sequence_number(
		          mft_entry,
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
	}
	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_number_of_attributes function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_number_of_attributes(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_mft_entry_t *mft_entry = NULL;
	int number_of_attributes         = 0;
	int number_of_attributes_is_set  = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_number_of_attributes(
	          mft_entry,
	          &number_of_attributes,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_attributes_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_number_of_attributes(
	          NULL,
	          &number_of_attributes,
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

	if( number_of_attributes_is_set != 0 )
	{
		result = libfsntfs_mft_entry_get_number_of_attributes(
		          mft_entry,
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
	}
	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_number_of_alternate_data_attributes function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_number_of_alternate_data_attributes(
     void )
{
	libcerror_error_t *error                       = NULL;
	libfsntfs_mft_entry_t *mft_entry               = NULL;
	int number_of_alternate_data_attributes        = 0;
	int number_of_alternate_data_attributes_is_set = 0;
	int result                                     = 0;

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
	          mft_entry,
	          &number_of_alternate_data_attributes,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_alternate_data_attributes_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
	          NULL,
	          &number_of_alternate_data_attributes,
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

	if( number_of_alternate_data_attributes_is_set != 0 )
	{
		result = libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
		          mft_entry,
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
	}
	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

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

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_initialize",
	 fsntfs_test_mft_entry_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_free",
	 fsntfs_test_mft_entry_free );

	/* TODO: add tests for libfsntfs_mft_entry_check_for_empty_block */

	/* TODO: add tests for libfsntfs_mft_entry_read */

	/* TODO: add tests for libfsntfs_mft_entry_read_header */

	/* TODO: add tests for libfsntfs_mft_entry_read_attributes */

	/* TODO: add tests for libfsntfs_mft_entry_read_attributes_from_attribute_list */

	/* TODO: add tests for libfsntfs_mft_entry_read_directory_entries_tree */

	/* TODO: add tests for libfsntfs_mft_entry_is_empty */

	/* TODO: add tests for libfsntfs_mft_entry_is_allocated */

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_get_file_reference",
	 fsntfs_test_mft_entry_get_file_reference );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_get_base_record_file_reference",
	 fsntfs_test_mft_entry_get_base_record_file_reference );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_get_journal_sequence_number",
	 fsntfs_test_mft_entry_get_journal_sequence_number );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_get_number_of_attributes",
	 fsntfs_test_mft_entry_get_number_of_attributes );

	/* TODO: add tests for libfsntfs_mft_entry_get_attribute_by_index */

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_get_number_of_alternate_data_attributes",
	 fsntfs_test_mft_entry_get_number_of_alternate_data_attributes );

	/* TODO: add tests for libfsntfs_mft_entry_get_alternate_data_attribute_by_index */

	/* TODO: add tests for libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name */

	/* TODO: add tests for libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name */

	/* TODO: add tests for libfsntfs_mft_entry_append_index */

	/* TODO: add tests for libfsntfs_mft_entry_get_index_by_name */

	/* TODO: add tests for libfsntfs_mft_entry_append_attribute */

	/* TODO: add tests for libfsntfs_mft_entry_append_data_attribute */

	/* TODO: add tests for libfsntfs_mft_entry_get_data_attribute_by_name */

	/* TODO: add tests for libfsntfs_mft_entry_append_index_allocation_attribute */

	/* TODO: add tests for libfsntfs_mft_entry_append_index_root_attribute */

	/* TODO: add tests for libfsntfs_mft_entry_set_reparse_point_attribute */

	/* TODO: add tests for libfsntfs_mft_entry_has_directory_entries_index */

	/* TODO: add tests for libfsntfs_mft_entry_read_element_data */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

