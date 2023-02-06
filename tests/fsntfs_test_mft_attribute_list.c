/*
 * Library attribute_list type test program
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

#include "fsntfs_test_libcdata.h"
#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_mft_attribute.h"
#include "../libfsntfs/libfsntfs_mft_attribute_list.h"

uint8_t fsntfs_test_mft_attribute_list_data1[ 368 ] = {
	0x20, 0x00, 0x00, 0x00, 0x70, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
	0x58, 0x01, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x1a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00,
	0x00, 0x00, 0x44, 0x43, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x1a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00,
	0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x28, 0x00, 0x04, 0x1a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x44, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x90, 0x00, 0x00, 0x00, 0x28, 0x00, 0x04, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x11, 0x00, 0x24, 0x00, 0x53, 0x00, 0x44, 0x00,
	0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x28, 0x00, 0x04, 0x1a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00,
	0x10, 0x00, 0x24, 0x00, 0x53, 0x00, 0x49, 0x00, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xa0, 0x00, 0x00, 0x00, 0x28, 0x00, 0x04, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xc8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x24, 0x00, 0x53, 0x00, 0x44, 0x00,
	0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x28, 0x00, 0x04, 0x1a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x24, 0x00, 0x53, 0x00, 0x49, 0x00, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xb0, 0x00, 0x00, 0x00, 0x28, 0x00, 0x04, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xc8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x05, 0x00, 0x24, 0x00, 0x53, 0x00, 0x44, 0x00,
	0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x28, 0x00, 0x04, 0x1a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x06, 0x00, 0x24, 0x00, 0x53, 0x00, 0x49, 0x00, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_mft_attribute_list_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_list_read_data(
     void )
{
	libcerror_error_t *error                   = NULL;
	libfsntfs_mft_attribute_list_t *attribute_list = NULL;
	int result                                 = 0;

	/* Initialize test
	 */
	result = libfsntfs_mft_attribute_list_initialize(
	          &attribute_list,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_list",
	 attribute_list );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_list_read_data(
	          attribute_list,
	          &( fsntfs_test_mft_attribute_list_data1[ 24 ] ),
	          344,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_list_read_data(
	          NULL,
	          &( fsntfs_test_mft_attribute_list_data1[ 24 ] ),
	          344,
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

	result = libfsntfs_mft_attribute_list_read_data(
	          attribute_list,
	          NULL,
	          344,
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

	result = libfsntfs_mft_attribute_list_read_data(
	          attribute_list,
	          &( fsntfs_test_mft_attribute_list_data1[ 24 ] ),
	          (size_t) SSIZE_MAX + 1,
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

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	/* Test libfsntfs_mft_attribute_list_read_data with malloc failing in libfsntfs_mft_attribute_list_entry_initialize
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_mft_attribute_list_read_data(
	          attribute_list,
	          &( fsntfs_test_mft_attribute_list_data1[ 24 ] ),
	          344,
	          &error );

	if( fsntfs_test_malloc_attempts_before_fail != -1 )
	{
		fsntfs_test_malloc_attempts_before_fail = -1;
	}
	else
	{
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
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfsntfs_mft_attribute_list_free(
	          &attribute_list,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute_list",
	 attribute_list );

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
	if( attribute_list != NULL )
	{
		libfsntfs_mft_attribute_list_free(
		 &attribute_list,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_attribute_list_read_from_attribute function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_list_read_from_attribute(
     void )
{
	libcerror_error_t *error                   = NULL;
	libfsntfs_mft_attribute_list_t *attribute_list = NULL;
	libfsntfs_io_handle_t *io_handle           = NULL;
	libfsntfs_mft_attribute_t *mft_attribute   = NULL;
	int result                                 = 0;

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

	result = libfsntfs_mft_attribute_initialize(
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_attribute",
	 mft_attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_list_data1,
	          368,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_list_initialize(
	          &attribute_list,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_list",
	 attribute_list );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_list_read_from_attribute(
	          attribute_list,
	          io_handle,
	          NULL,
	          mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_list_read_from_attribute(
	          NULL,
	          io_handle,
	          NULL,
	          mft_attribute,
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

	result = libfsntfs_mft_attribute_list_read_from_attribute(
	          attribute_list,
	          io_handle,
	          NULL,
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

	/* Clean up
	 */
	result = libfsntfs_mft_attribute_list_free(
	          &attribute_list,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute_list",
	 attribute_list );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_free(
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_attribute",
	 mft_attribute );

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
	if( attribute_list != NULL )
	{
		libfsntfs_mft_attribute_list_free(
		 &attribute_list,
		 NULL );
	}
	if( mft_attribute != NULL )
	{
		libfsntfs_mft_attribute_free(
		 &mft_attribute,
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

/* TODO add tests for libfsntfs_mft_attribute_list_initialize */

/* TODO add tests for libfsntfs_mft_attribute_list_free */

/* TODO add tests for libfsntfs_mft_attribute_list_file_reference_free */

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_attribute_list_read_data",
	 fsntfs_test_mft_attribute_list_read_data );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_attribute_list_read_from_attribute",
	 fsntfs_test_mft_attribute_list_read_from_attribute );

/* TODO add tests for libfsntfs_mft_attribute_list_get_number_of_entries */

/* TODO add tests for libfsntfs_mft_attribute_list_get_entry_by_index */

/* TODO add tests for libfsntfs_mft_attribute_list_compare_by_base_record_file_reference */

/* TODO add tests for libfsntfs_mft_attribute_list_get_number_of_file_references */

/* TODO add tests for libfsntfs_mft_attribute_list_get_file_reference_by_index */

/* TODO add tests for libfsntfs_mft_attribute_list_compare_file_reference */

/* TODO add tests for libfsntfs_mft_attribute_list_insert_file_reference */

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

