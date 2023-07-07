/*
 * Library data_run type test program
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

#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfdata.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_data_run.h"
#include "../libfsntfs/libfsntfs_io_handle.h"

uint8_t fsntfs_test_data_run_data1[ 6 ] = {
	0x11, 0x03, 0x37, 0x01, 0x0d, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_data_run_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_data_run_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfsntfs_data_run_t *data_run  = NULL;
	int result                      = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_data_run_initialize(
	          &data_run,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_run",
	 data_run );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_data_run_free(
	          &data_run,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "data_run",
	 data_run );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_data_run_initialize(
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

	data_run = (libfsntfs_data_run_t *) 0x12345678UL;

	result = libfsntfs_data_run_initialize(
	          &data_run,
	          &error );

	data_run = NULL;

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
		/* Test libfsntfs_data_run_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_data_run_initialize(
		          &data_run,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( data_run != NULL )
			{
				libfsntfs_data_run_free(
				 &data_run,
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
			 "data_run",
			 data_run );

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
		/* Test libfsntfs_data_run_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_data_run_initialize(
		          &data_run,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( data_run != NULL )
			{
				libfsntfs_data_run_free(
				 &data_run,
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
			 "data_run",
			 data_run );

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
	if( data_run != NULL )
	{
		libfsntfs_data_run_free(
		 &data_run,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_data_run_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_data_run_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_data_run_free(
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

/* Tests the libfsntfs_data_run_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_data_run_read_data(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_data_run_t *data_run   = NULL;
	libfsntfs_io_handle_t *io_handle = NULL;
	ssize_t read_count               = 0;
	int result                       = 0;

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

	result = libfsntfs_data_run_initialize(
	          &data_run,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_run",
	 data_run );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_count = libfsntfs_data_run_read_data(
	              data_run,
	              io_handle,
	              fsntfs_test_data_run_data1,
	              6,
	              data_run->cluster_block_number,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 3 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->cluster_block_number",
	 data_run->cluster_block_number,
	 (uint64_t) 55 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->start_offset",
	 (uint64_t) data_run->start_offset,
	 (uint64_t) 55 * 4096 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->size",
	 (uint64_t) data_run->size,
	 (uint64_t) 12288 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT32(
	 "data_run->range_flags",
	 (uint32_t) data_run->range_flags,
	 (uint32_t) 0 );

	read_count = libfsntfs_data_run_read_data(
	              data_run,
	              io_handle,
	              &( fsntfs_test_data_run_data1[ 3 ] ),
	              6 - 3,
	              data_run->cluster_block_number,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 2 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->cluster_block_number",
	 data_run->cluster_block_number,
	 (uint64_t) 0 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->start_offset",
	 (uint64_t) data_run->start_offset,
	 (uint64_t) 0 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->size",
	 (uint64_t) data_run->size,
	 (uint64_t) 53248 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT32(
	 "data_run->range_flags",
	 (uint32_t) data_run->range_flags,
	 (uint32_t) LIBFDATA_RANGE_FLAG_IS_SPARSE );

	read_count = libfsntfs_data_run_read_data(
	              data_run,
	              io_handle,
	              &( fsntfs_test_data_run_data1[ 5 ] ),
	              6 - 5,
	              data_run->cluster_block_number,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->cluster_block_number",
	 data_run->cluster_block_number,
	 (uint64_t) 0 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->start_offset",
	 (uint64_t) data_run->start_offset,
	 (uint64_t) 0 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "data_run->size",
	 (uint64_t) data_run->size,
	 (uint64_t) 0 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT32(
	 "data_run->range_flags",
	 (uint32_t) data_run->range_flags,
	 (uint32_t) 0 );

	/* Test error cases
	 */
	read_count = libfsntfs_data_run_read_data(
	              NULL,
	              io_handle,
	              fsntfs_test_data_run_data1,
	              6,
	              data_run->cluster_block_number,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsntfs_data_run_read_data(
	              data_run,
	              NULL,
	              fsntfs_test_data_run_data1,
	              6,
	              data_run->cluster_block_number,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsntfs_data_run_read_data(
	              data_run,
	              io_handle,
	              NULL,
	              6,
	              data_run->cluster_block_number,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsntfs_data_run_read_data(
	              data_run,
	              io_handle,
	              fsntfs_test_data_run_data1,
	              (size_t) SSIZE_MAX + 1,
	              data_run->cluster_block_number,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsntfs_data_run_read_data(
	              data_run,
	              io_handle,
	              fsntfs_test_data_run_data1,
	              0,
	              data_run->cluster_block_number,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsntfs_data_run_free(
	          &data_run,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "data_run",
	 data_run );

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
	if( data_run != NULL )
	{
		libfsntfs_data_run_free(
		 &data_run,
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

	FSNTFS_TEST_RUN(
	 "libfsntfs_data_run_initialize",
	 fsntfs_test_data_run_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_data_run_free",
	 fsntfs_test_data_run_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_data_run_read_data",
	 fsntfs_test_data_run_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

