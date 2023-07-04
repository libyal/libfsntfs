/*
 * Library buffer_data_handle type test program
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_buffer_data_handle.h"

uint8_t fsntfs_test_buffer_data_handle_data1[ 16 ] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_buffer_data_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_buffer_data_handle_initialize(
     void )
{
	libcerror_error_t *error                           = NULL;
	libfsntfs_buffer_data_handle_t *buffer_data_handle = NULL;
	int result                                         = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                    = 1;
	int number_of_memset_fail_tests                    = 1;
	int test_number                                    = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_buffer_data_handle_initialize(
	          &buffer_data_handle,
	          fsntfs_test_buffer_data_handle_data1,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "buffer_data_handle",
	 buffer_data_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_buffer_data_handle_free(
	          &buffer_data_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "buffer_data_handle",
	 buffer_data_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_buffer_data_handle_initialize(
	          &buffer_data_handle,
	          NULL,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "buffer_data_handle",
	 buffer_data_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_buffer_data_handle_free(
	          &buffer_data_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "buffer_data_handle",
	 buffer_data_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_buffer_data_handle_initialize(
	          NULL,
	          fsntfs_test_buffer_data_handle_data1,
	          16,
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

	buffer_data_handle = (libfsntfs_buffer_data_handle_t *) 0x12345678UL;

	result = libfsntfs_buffer_data_handle_initialize(
	          &buffer_data_handle,
	          fsntfs_test_buffer_data_handle_data1,
	          16,
	          &error );

	buffer_data_handle = NULL;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_buffer_data_handle_initialize(
	          &buffer_data_handle,
	          NULL,
	          16,
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

	result = libfsntfs_buffer_data_handle_initialize(
	          &buffer_data_handle,
	          fsntfs_test_buffer_data_handle_data1,
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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_buffer_data_handle_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_buffer_data_handle_initialize(
		          &buffer_data_handle,
		          fsntfs_test_buffer_data_handle_data1,
		          16,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( buffer_data_handle != NULL )
			{
				libfsntfs_buffer_data_handle_free(
				 &buffer_data_handle,
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
			 "buffer_data_handle",
			 buffer_data_handle );

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
		/* Test libfsntfs_buffer_data_handle_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_buffer_data_handle_initialize(
		          &buffer_data_handle,
		          fsntfs_test_buffer_data_handle_data1,
		          16,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( buffer_data_handle != NULL )
			{
				libfsntfs_buffer_data_handle_free(
				 &buffer_data_handle,
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
			 "buffer_data_handle",
			 buffer_data_handle );

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
	if( buffer_data_handle != NULL )
	{
		libfsntfs_buffer_data_handle_free(
		 &buffer_data_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_buffer_data_handle_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_buffer_data_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_buffer_data_handle_free(
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

/* Tests the libfsntfs_buffer_data_handle_read_segment_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_buffer_data_handle_read_segment_data(
     void )
{
	uint8_t segment_data[ 16 ];

	libcerror_error_t *error                           = NULL;
	libfsntfs_buffer_data_handle_t *buffer_data_handle = NULL;
	ssize_t read_count                                 = 0;
	off64_t offset                                     = 0;
	int result                                         = 0;

	/* Initialize test
	 */
	result = libfsntfs_buffer_data_handle_initialize(
	          &buffer_data_handle,
	          fsntfs_test_buffer_data_handle_data1,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "buffer_data_handle",
	 buffer_data_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              buffer_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              12,
	              0,
	              0,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) 12 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          segment_data,
	          fsntfs_test_buffer_data_handle_data1,
	          12 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Read buffer on last segment boundary
	 */
	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              buffer_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              8,
	              0,
	              0,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) 4 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          segment_data,
	          &( fsntfs_test_buffer_data_handle_data1[ 12 ] ),
	          4 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Read buffer beyond last segment
	 */
	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              buffer_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	offset = libfsntfs_buffer_data_handle_seek_segment_offset(
	          buffer_data_handle,
	          NULL,
	          0,
	          0,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              NULL,
	              NULL,
	              0,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	buffer_data_handle->current_offset = -1;

	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              buffer_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	buffer_data_handle->current_offset = 0;

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              buffer_data_handle,
	              NULL,
	              -1,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              buffer_data_handle,
	              NULL,
	              0,
	              0,
	              NULL,
	              16,
	              0,
	              0,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              buffer_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              0,
	              &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSNTFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libfsntfs_buffer_data_handle_read_segment_data with memcpy failing
	 */
	fsntfs_test_memcpy_attempts_before_fail = 0;

	read_count = libfsntfs_buffer_data_handle_read_segment_data(
	              buffer_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              12,
	              0,
	              0,
	              &error );

	if( fsntfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsntfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSNTFS_TEST_ASSERT_EQUAL_INT64(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	/* Clean up
	 */
	result = libfsntfs_buffer_data_handle_free(
	          &buffer_data_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "buffer_data_handle",
	 buffer_data_handle );

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
	if( buffer_data_handle != NULL )
	{
		libfsntfs_buffer_data_handle_free(
		 &buffer_data_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_buffer_data_handle_seek_segment_offset function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_buffer_data_handle_seek_segment_offset(
     void )
{
	libcerror_error_t *error                           = NULL;
	libfsntfs_buffer_data_handle_t *buffer_data_handle = NULL;
	off64_t offset                                     = 0;
	int result                                         = 0;

	/* Initialize test
	 */
	result = libfsntfs_buffer_data_handle_initialize(
	          &buffer_data_handle,
	          fsntfs_test_buffer_data_handle_data1,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "buffer_data_handle",
	 buffer_data_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	offset = libfsntfs_buffer_data_handle_seek_segment_offset(
	          buffer_data_handle,
	          NULL,
	          0,
	          0,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libfsntfs_buffer_data_handle_seek_segment_offset(
	          NULL,
	          NULL,
	          0,
	          0,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libfsntfs_buffer_data_handle_seek_segment_offset(
	          buffer_data_handle,
	          NULL,
	          -1,
	          0,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libfsntfs_buffer_data_handle_seek_segment_offset(
	          buffer_data_handle,
	          NULL,
	          0,
	          0,
	          -1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsntfs_buffer_data_handle_free(
	          &buffer_data_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "buffer_data_handle",
	 buffer_data_handle );

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
	if( buffer_data_handle != NULL )
	{
		libfsntfs_buffer_data_handle_free(
		 &buffer_data_handle,
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
	 "libfsntfs_buffer_data_handle_initialize",
	 fsntfs_test_buffer_data_handle_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_buffer_data_handle_free",
	 fsntfs_test_buffer_data_handle_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_buffer_data_handle_read_segment_data",
	 fsntfs_test_buffer_data_handle_read_segment_data );

	FSNTFS_TEST_RUN(
	 "libfsntfs_buffer_data_handle_seek_segment_offset",
	 fsntfs_test_buffer_data_handle_seek_segment_offset );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

