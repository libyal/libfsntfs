/*
 * Library directory_entries_tree type test program
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

#include "../libfsntfs/libfsntfs_definitions.h"
#include "../libfsntfs/libfsntfs_directory_entries_tree.h"
#include "../libfsntfs/libfsntfs_directory_entry.h"
#include "../libfsntfs/libfsntfs_index.h"
#include "../libfsntfs/libfsntfs_index_node.h"
#include "../libfsntfs/libfsntfs_index_value.h"
#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_mft_entry.h"

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_directory_entries_tree_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_initialize(
     void )
{
	libcerror_error_t *error                                   = NULL;
	libfsntfs_directory_entries_tree_t *directory_entries_tree = NULL;
	int result                                                 = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                            = 1;
	int number_of_memset_fail_tests                            = 1;
	int test_number                                            = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_directory_entries_tree_initialize(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_directory_entries_tree_free(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_initialize(
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

	directory_entries_tree = (libfsntfs_directory_entries_tree_t *) 0x12345678UL;

	result = libfsntfs_directory_entries_tree_initialize(
	          &directory_entries_tree,
	          &error );

	directory_entries_tree = NULL;

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
		/* Test libfsntfs_directory_entries_tree_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_directory_entries_tree_initialize(
		          &directory_entries_tree,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( directory_entries_tree != NULL )
			{
				libfsntfs_directory_entries_tree_free(
				 &directory_entries_tree,
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
			 "directory_entries_tree",
			 directory_entries_tree );

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
		/* Test libfsntfs_directory_entries_tree_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_directory_entries_tree_initialize(
		          &directory_entries_tree,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( directory_entries_tree != NULL )
			{
				libfsntfs_directory_entries_tree_free(
				 &directory_entries_tree,
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
			 "directory_entries_tree",
			 directory_entries_tree );

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
	if( directory_entries_tree != NULL )
	{
		libfsntfs_directory_entries_tree_free(
		 &directory_entries_tree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_directory_entries_tree_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_free(
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

/* Tests the libfsntfs_directory_entries_tree_insert_index_value function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_insert_index_value(
     void )
{
	libcerror_error_t *error                                   = NULL;
	libfsntfs_directory_entries_tree_t *directory_entries_tree = NULL;
	libfsntfs_index_value_t *index_value                       = NULL;
	int result                                                 = 0;

	/* Initialize test
	 */
	result = libfsntfs_directory_entries_tree_initialize(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_insert_index_value(
	          NULL,
	          0,
	          index_value,
	          0,
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

	result = libfsntfs_directory_entries_tree_insert_index_value(
	          directory_entries_tree,
	          0,
	          NULL,
	          0,
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

	/* Clean up
	 */
	result = libfsntfs_directory_entries_tree_free(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

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
	if( directory_entries_tree != NULL )
	{
		libfsntfs_directory_entries_tree_free(
		 &directory_entries_tree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_directory_entries_tree_read_from_index_node function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_read_from_index_node(
     void )
{
	libbfio_handle_t *file_io_handle                           = NULL;
	libcerror_error_t *error                                   = NULL;
	libfsntfs_directory_entries_tree_t *directory_entries_tree = NULL;
	libfsntfs_index_node_t *index_node                         = NULL;
	int result                                                 = 0;

	/* Initialize test
	 */
	result = libfsntfs_directory_entries_tree_initialize(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_read_from_index_node(
	          NULL,
	          file_io_handle,
	          index_node,
	          0,
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

	result = libfsntfs_directory_entries_tree_read_from_index_node(
	          directory_entries_tree,
	          NULL,
	          index_node,
	          0,
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

	result = libfsntfs_directory_entries_tree_read_from_index_node(
	          directory_entries_tree,
	          file_io_handle,
	          NULL,
	          0,
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

	result = libfsntfs_directory_entries_tree_read_from_index_node(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          LIBFSNTFS_MAXIMUM_RECURSION_DEPTH + 1,
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

	/* Clean up
	 */
	result = libfsntfs_directory_entries_tree_free(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

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
	if( directory_entries_tree != NULL )
	{
		libfsntfs_directory_entries_tree_free(
		 &directory_entries_tree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_directory_entries_tree_read_from_i30_index function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_read_from_i30_index(
     void )
{
	libbfio_handle_t *file_io_handle                           = NULL;
	libcerror_error_t *error                                   = NULL;
	libfsntfs_directory_entries_tree_t *directory_entries_tree = NULL;
	libfsntfs_io_handle_t *io_handle                           = NULL;
	libfsntfs_mft_entry_t *mft_entry                           = NULL;
	int result                                                 = 0;

	/* Initialize test
	 */
	result = libfsntfs_directory_entries_tree_initialize(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */
/* TODO implement test with LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY */

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_read_from_i30_index(
	          NULL,
	          io_handle,
	          file_io_handle,
	          mft_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_read_from_i30_index(
	          directory_entries_tree,
	          NULL,
	          file_io_handle,
	          mft_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_read_from_i30_index(
	          directory_entries_tree,
	          io_handle,
	          NULL,
	          mft_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_read_from_i30_index(
	          directory_entries_tree,
	          io_handle,
	          file_io_handle,
	          NULL,
	          0,
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

	/* Clean up
	 */
	result = libfsntfs_directory_entries_tree_free(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

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
	if( directory_entries_tree != NULL )
	{
		libfsntfs_directory_entries_tree_free(
		 &directory_entries_tree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_directory_entries_tree_get_number_of_entries function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_get_number_of_entries(
     libfsntfs_directory_entries_tree_t *directory_entries_tree )
{
	libcerror_error_t *error = NULL;
	int number_of_entries    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_directory_entries_tree_get_number_of_entries(
	          directory_entries_tree,
	          &number_of_entries,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "number_of_entries",
	 number_of_entries,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_get_number_of_entries(
	          NULL,
	          &number_of_entries,
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

	result = libfsntfs_directory_entries_tree_get_number_of_entries(
	          directory_entries_tree,
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

/* Tests the libfsntfs_directory_entries_tree_get_entry_by_index function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_get_entry_by_index(
     libfsntfs_directory_entries_tree_t *directory_entries_tree )
{
	libbfio_handle_t *file_io_handle             = NULL;
	libcerror_error_t *error                     = NULL;
	libfsntfs_directory_entry_t *directory_entry = NULL;
	int result                                   = 0;

	/* Test regular cases
	 */
/* TODO implement file IO handle
	result = libfsntfs_directory_entries_tree_get_entry_by_index(
	          directory_entries_tree,
	          file_io_handle,
	          0,
	          &directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_get_entry_by_index(
	          NULL,
	          file_io_handle,
	          0,
	          &directory_entry,
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

	result = libfsntfs_directory_entries_tree_get_entry_by_index(
	          directory_entries_tree,
	          NULL,
	          0,
	          &directory_entry,
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

	result = libfsntfs_directory_entries_tree_get_entry_by_index(
	          directory_entries_tree,
	          file_io_handle,
	          -1,
	          &directory_entry,
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

	result = libfsntfs_directory_entries_tree_get_entry_by_index(
	          directory_entries_tree,
	          file_io_handle,
	          0,
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

/* Tests the libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree )
{
	uint8_t name[ 5 ]                            = { 'n', 'a', 'm', 'e', 0 };
	libbfio_handle_t *file_io_handle             = NULL;
	libcerror_error_t *error                     = NULL;
	libfsntfs_directory_entry_t *directory_entry = NULL;
	libfsntfs_index_node_t *index_node           = NULL;
	int result                                   = 0;

	/* Test regular cases
	 */
/* TODO implement file IO handle
	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          name,
	          4,
	          &directory_entry,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          NULL,
	          file_io_handle,
	          index_node,
	          name,
	          4,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          directory_entries_tree,
	          NULL,
	          index_node,
	          name,
	          4,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          NULL,
	          name,
	          4,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          NULL,
	          4,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          name,
	          (size_t) SSIZE_MAX + 1,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          name,
	          4,
	          NULL,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          name,
	          4,
	          &directory_entry,
	          LIBFSNTFS_MAXIMUM_RECURSION_DEPTH + 1,
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

/* Tests the libfsntfs_directory_entries_tree_get_entry_by_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_get_entry_by_utf8_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree )
{
	uint8_t name[ 5 ]                            = { 'n', 'a', 'm', 'e', 0 };
	libbfio_handle_t *file_io_handle             = NULL;
	libcerror_error_t *error                     = NULL;
	libfsntfs_directory_entry_t *directory_entry = NULL;
	libfsntfs_index_t *i30_index                 = NULL;
	int result                                   = 0;

	/* Test regular cases
	 */
/* TODO implement file IO handle
	result = libfsntfs_directory_entries_tree_get_entry_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          name,
	          4,
	          &directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_get_entry_by_utf8_name(
	          NULL,
	          file_io_handle,
	          name,
	          4,
	          &directory_entry,
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

	i30_index = directory_entries_tree->i30_index;

	directory_entries_tree->i30_index = NULL;

	result = libfsntfs_directory_entries_tree_get_entry_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          name,
	          4,
	          &directory_entry,
	          &error );

	directory_entries_tree->i30_index = i30_index;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_directory_entries_tree_get_entry_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          name,
	          4,
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

/* Tests the libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree )
{
	uint16_t name[ 5 ]                           = { 'n', 'a', 'm', 'e', 0 };
	libbfio_handle_t *file_io_handle             = NULL;
	libcerror_error_t *error                     = NULL;
	libfsntfs_directory_entry_t *directory_entry = NULL;
	libfsntfs_index_node_t *index_node           = NULL;
	int result                                   = 0;

	/* Test regular cases
	 */
/* TODO implement file IO handle
	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          name,
	          4,
	          &directory_entry,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          NULL,
	          file_io_handle,
	          index_node,
	          name,
	          4,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          directory_entries_tree,
	          NULL,
	          index_node,
	          name,
	          4,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          NULL,
	          name,
	          4,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          NULL,
	          4,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          name,
	          (size_t) SSIZE_MAX + 1,
	          &directory_entry,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          name,
	          4,
	          NULL,
	          0,
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

	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          index_node,
	          name,
	          4,
	          &directory_entry,
	          LIBFSNTFS_MAXIMUM_RECURSION_DEPTH + 1,
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

/* Tests the libfsntfs_directory_entries_tree_get_entry_by_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entries_tree_get_entry_by_utf16_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree )
{
	uint16_t name[ 5 ]                           = { 'n', 'a', 'm', 'e', 0 };
	libbfio_handle_t *file_io_handle             = NULL;
	libcerror_error_t *error                     = NULL;
	libfsntfs_directory_entry_t *directory_entry = NULL;
	libfsntfs_index_t *i30_index                 = NULL;
	int result                                   = 0;

	/* Test regular cases
	 */
/* TODO implement file IO handle
	result = libfsntfs_directory_entries_tree_get_entry_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          name,
	          4,
	          &directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsntfs_directory_entries_tree_get_entry_by_utf16_name(
	          NULL,
	          file_io_handle,
	          name,
	          4,
	          &directory_entry,
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

	i30_index = directory_entries_tree->i30_index;

	directory_entries_tree->i30_index = NULL;

	result = libfsntfs_directory_entries_tree_get_entry_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          name,
	          4,
	          &directory_entry,
	          &error );

	directory_entries_tree->i30_index = i30_index;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_directory_entries_tree_get_entry_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          name,
	          4,
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
#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	libbfio_handle_t *file_io_handle                           = NULL;
	libcerror_error_t *error                                   = NULL;
	libfsntfs_directory_entries_tree_t *directory_entries_tree = NULL;
	libfsntfs_io_handle_t *io_handle                           = NULL;
	libfsntfs_mft_entry_t *mft_entry                           = NULL;
	int result                                                 = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entries_tree_initialize",
	 fsntfs_test_directory_entries_tree_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entries_tree_free",
	 fsntfs_test_directory_entries_tree_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entries_tree_insert_index_value",
	 fsntfs_test_directory_entries_tree_insert_index_value );

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entries_tree_read_from_index_node",
	 fsntfs_test_directory_entries_tree_read_from_index_node );

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entries_tree_read_from_i30_index",
	 fsntfs_test_directory_entries_tree_read_from_i30_index );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize data stream for tests
	 */
	result = libfsntfs_directory_entries_tree_initialize(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_directory_entries_tree_get_number_of_entries",
	 fsntfs_test_directory_entries_tree_get_number_of_entries,
	 directory_entries_tree );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_directory_entries_tree_get_entry_by_index",
	 fsntfs_test_directory_entries_tree_get_entry_by_index,
	 directory_entries_tree );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name",
	 fsntfs_test_directory_entries_tree_get_entry_from_index_node_by_utf8_name,
	 directory_entries_tree );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_directory_entries_tree_get_entry_by_utf8_name",
	 fsntfs_test_directory_entries_tree_get_entry_by_utf8_name,
	 directory_entries_tree );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name",
	 fsntfs_test_directory_entries_tree_get_entry_from_index_node_by_utf16_name,
	 directory_entries_tree );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_directory_entries_tree_get_entry_by_utf16_name",
	 fsntfs_test_directory_entries_tree_get_entry_by_utf16_name,
	 directory_entries_tree );

	/* TODO add tests for libfsntfs_directory_entries_tree_read_element_data */

	/* Clean up
	 */
	result = libfsntfs_directory_entries_tree_free(
	          &directory_entries_tree,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "directory_entries_tree",
	 directory_entries_tree );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( directory_entries_tree != NULL )
	{
		libfsntfs_directory_entries_tree_free(
		 &directory_entries_tree,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

