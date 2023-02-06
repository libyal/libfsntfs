/*
 * Library usn_change_journal type test program
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsntfs_test_libbfio.h"
#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_directory_entry.h"
#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_usn_change_journal.h"


/* TODO implement */
#ifdef TODO

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_usn_change_journal_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_usn_change_journal_initialize(
     void )
{
	libbfio_handle_t *file_io_handle                   = NULL;
	libcerror_error_t *error                           = NULL;
	libfsntfs_attribute_t *data_attribute              = NULL;
	libfsntfs_directory_entry_t *directory_entry       = NULL;
	libfsntfs_io_handle_t *io_handle                   = NULL;
	libfsntfs_usn_change_journal_t *usn_change_journal = NULL;
	int result                                         = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                    = 1;
	int number_of_memset_fail_tests                    = 1;
	int test_number                                    = 0;
#endif

	/* Initialize test
	 */
	result = libfsntfs_io_handle_initialize(
	          &io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->cluster_block_size = 4096;

	/* Test regular cases
	 */
	result = libfsntfs_usn_change_journal_initialize(
	          &usn_change_journal,
	          io_handle,
	          file_io_handle,
	          directory_entry,
	          data_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "usn_change_journal",
	 usn_change_journal );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_usn_change_journal_free(
	          &usn_change_journal,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "usn_change_journal",
	 usn_change_journal );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_usn_change_journal_initialize(
	          NULL,
	          io_handle,
	          file_io_handle,
	          directory_entry,
	          data_attribute,
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

	usn_change_journal = (libfsntfs_usn_change_journal_t *) 0x12345678UL;

	result = libfsntfs_usn_change_journal_initialize(
	          &usn_change_journal,
	          io_handle,
	          file_io_handle,
	          directory_entry,
	          data_attribute,
	          &error );

	usn_change_journal = NULL;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_usn_change_journal_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_usn_change_journal_initialize(
		          &usn_change_journal,
		          io_handle,
		          file_io_handle,
		          directory_entry,
		          data_attribute,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( usn_change_journal != NULL )
			{
				libfsntfs_usn_change_journal_free(
				 &usn_change_journal,
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
			 "usn_change_journal",
			 usn_change_journal );

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
		/* Test libfsntfs_usn_change_journal_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_usn_change_journal_initialize(
		          &usn_change_journal,
		          io_handle,
		          file_io_handle,
		          directory_entry,
		          data_attribute,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( usn_change_journal != NULL )
			{
				libfsntfs_usn_change_journal_free(
				 &usn_change_journal,
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
			 "usn_change_journal",
			 usn_change_journal );

			FSNTFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfsntfs_internal_attribute_free(
	          (libfsntfs_internal_attribute_t **) &data_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "data_attribute",
	 data_attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_io_handle_free(
	          &io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( usn_change_journal != NULL )
	{
		libfsntfs_usn_change_journal_free(
		 &usn_change_journal,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsntfs_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
#endif /* TODO */

/* Tests the libfsntfs_usn_change_journal_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_usn_change_journal_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_usn_change_journal_free(
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

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

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

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	/* TODO: add tests for libfsntfs_usn_change_journal_initialize */

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_RUN(
	 "libfsntfs_usn_change_journal_free",
	 fsntfs_test_usn_change_journal_free );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	/* TODO: add tests for libfsntfs_usn_change_journal_get_offset */

	/* TODO: add tests for libfsntfs_usn_change_journal_read_usn_record */

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

