/*
 * Library mft_attribute type test program
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
#include <byte_stream.h>
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

#include "../libfsntfs/libfsntfs_data_run.h"
#include "../libfsntfs/libfsntfs_extent.h"
#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_mft_attribute.h"

uint8_t fsntfs_test_mft_attribute_data1[ 88 ] = {
	0x90, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x00, 0x11, 0x00,
	0x38, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x44, 0x00, 0x48, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x10, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t fsntfs_test_mft_attribute_data2[ 72 ] = {
	0x80, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x0a, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xa8, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x0a, 0x00, 0x00, 0x00, 0x00,
	0x33, 0x80, 0xaa, 0x00, 0x00, 0x00, 0x0c, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_mft_attribute_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_initialize(
     void )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	int result                               = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests          = 1;
	int number_of_memset_fail_tests          = 1;
	int test_number                          = 0;
#endif

	/* Test regular cases
	 */
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

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_initialize(
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

	mft_attribute = (libfsntfs_mft_attribute_t *) 0x12345678UL;

	result = libfsntfs_mft_attribute_initialize(
	          &mft_attribute,
	          &error );

	mft_attribute = NULL;

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
		/* Test libfsntfs_mft_attribute_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_mft_attribute_initialize(
		          &mft_attribute,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( mft_attribute != NULL )
			{
				libfsntfs_mft_attribute_free(
				 &mft_attribute,
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
			 "mft_attribute",
			 mft_attribute );

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
		/* Test libfsntfs_mft_attribute_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_mft_attribute_initialize(
		          &mft_attribute,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( mft_attribute != NULL )
			{
				libfsntfs_mft_attribute_free(
				 &mft_attribute,
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
			 "mft_attribute",
			 mft_attribute );

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
	if( mft_attribute != NULL )
	{
		libfsntfs_mft_attribute_free(
		 &mft_attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_attribute_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_free(
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

/* Tests the libfsntfs_mft_attribute_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_read_data(
     void )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_io_handle_t *io_handle         = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	int result                               = 0;

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

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
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

	/* Initialize test
	 */
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

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data2,
	          72,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
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

	/* Initialize test
	 */
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

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_read_data(
	          NULL,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
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

	mft_attribute->name = (uint8_t *) 0x12345678UL;

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	mft_attribute->name = NULL;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          NULL,
	          fsntfs_test_mft_attribute_data1,
	          88,
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

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          NULL,
	          88,
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

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
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

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
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

	/* Test error case where resident data is missing
	 */
	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
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

	/* Test error case where non-resident data is missing
	 */
	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data2,
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

	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data2[ 34 ] ),
	 0x0001 );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data2,
	          48,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data2[ 34 ] ),
	 0x0000 );

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

	/* Test libfsntfs_mft_attribute_read_data with malloc failing
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
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
#if defined( OPTIMIZATION_DISABLED )

	/* Test libfsntfs_mft_attribute_read_data with memcpy failing
	 */
	fsntfs_test_memcpy_attempts_before_fail = 0;

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	if( fsntfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsntfs_test_memcpy_attempts_before_fail = -1;
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
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Test error case where size value is invalid
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( fsntfs_test_mft_attribute_data1[ 4 ] ),
	 0xffffffffUL );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( fsntfs_test_mft_attribute_data1[ 4 ] ),
	 0x00000058UL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where compression flags (in data flags) value is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data1[ 12 ] ),
	 0x00ff );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data1[ 12 ] ),
	 0x0000 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where non-resident valid data size value is invalid
	 */
	byte_stream_copy_from_uint64_little_endian(
	 &( fsntfs_test_mft_attribute_data2[ 56 ] ),
	 0xffffffffffffffffULL );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data2,
	          72,
	          &error );

	byte_stream_copy_from_uint64_little_endian(
	 &( fsntfs_test_mft_attribute_data2[ 56 ] ),
	 0x000000000000a0a8ULL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where name offset value is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data1[ 10 ] ),
	 0xffff );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data1[ 10 ] ),
	 0x0018 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where name size value is invalid
	 */
	fsntfs_test_mft_attribute_data1[ 9 ] = 0xff;

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	fsntfs_test_mft_attribute_data1[ 9 ] = 0x04;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where resident data offset value is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data1[ 20 ] ),
	 0xffff );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data1[ 20 ] ),
	 0x0020 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where non-resident data runs offset value is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data2[ 32 ] ),
	 0xffff );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_mft_attribute_data2,
	          72,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_mft_attribute_data2[ 32 ] ),
	 0x0040 );

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

/* Tests the libfsntfs_mft_attribute_data_is_resident function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_data_is_resident(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_data_is_resident(
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
	result = libfsntfs_mft_attribute_data_is_resident(
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

/* Tests the libfsntfs_mft_attribute_get_type function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_type(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	libcerror_error_t *error = NULL;
	uint32_t type            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_type(
	          mft_attribute,
	          &type,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT32(
	 "type",
	 type,
	 (uint32_t) 0x00000090UL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_type(
	          NULL,
	          &type,
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

	result = libfsntfs_mft_attribute_get_type(
	          mft_attribute,
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

/* Tests the libfsntfs_mft_attribute_get_data_flags function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_data_flags(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	libcerror_error_t *error = NULL;
	uint16_t data_flags      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_data_flags(
	          mft_attribute,
	          &data_flags,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "data_flags",
	 data_flags,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_data_flags(
	          NULL,
	          &data_flags,
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

	result = libfsntfs_mft_attribute_get_data_flags(
	          mft_attribute,
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

/* Tests the libfsntfs_mft_attribute_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_utf8_name_size(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	uint8_t *name            = NULL;
	libcerror_error_t *error = NULL;
	size_t utf8_name_size    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_utf8_name_size(
	          mft_attribute,
	          &utf8_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_name_size",
	 utf8_name_size,
	 (size_t) 5 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name = mft_attribute->name;

	mft_attribute->name = NULL;

	result = libfsntfs_mft_attribute_get_utf8_name_size(
	          mft_attribute,
	          &utf8_name_size,
	          &error );

	mft_attribute->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_name_size",
	 utf8_name_size,
	 (size_t) 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
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

	result = libfsntfs_mft_attribute_get_utf8_name_size(
	          mft_attribute,
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

	name = mft_attribute->name;

	mft_attribute->name = NULL;

	result = libfsntfs_mft_attribute_get_utf8_name_size(
	          mft_attribute,
	          NULL,
	          &error );

	mft_attribute->name = name;

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

/* Tests the libfsntfs_mft_attribute_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_utf8_name(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	uint8_t utf8_name[ 16 ];

	uint8_t *name            = NULL;
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_utf8_name(
	          mft_attribute,
	          utf8_name,
	          16,
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
	result = libfsntfs_mft_attribute_get_utf8_name(
	          NULL,
	          utf8_name,
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

	name = mft_attribute->name;

	mft_attribute->name = NULL;

	result = libfsntfs_mft_attribute_get_utf8_name(
	          mft_attribute,
	          utf8_name,
	          16,
	          &error );

	mft_attribute->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_attribute_get_utf8_name(
	          mft_attribute,
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

	result = libfsntfs_mft_attribute_get_utf8_name(
	          mft_attribute,
	          utf8_name,
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

	result = libfsntfs_mft_attribute_get_utf8_name(
	          mft_attribute,
	          utf8_name,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_attribute_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_utf16_name_size(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	uint8_t *name            = NULL;
	libcerror_error_t *error = NULL;
	size_t utf16_name_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_utf16_name_size(
	          mft_attribute,
	          &utf16_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_name_size",
	 utf16_name_size,
	 (size_t) 5 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name = mft_attribute->name;

	mft_attribute->name = NULL;

	result = libfsntfs_mft_attribute_get_utf16_name_size(
	          mft_attribute,
	          &utf16_name_size,
	          &error );

	mft_attribute->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_name_size",
	 utf16_name_size,
	 (size_t) 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
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

	result = libfsntfs_mft_attribute_get_utf16_name_size(
	          mft_attribute,
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

	name = mft_attribute->name;

	mft_attribute->name = NULL;

	result = libfsntfs_mft_attribute_get_utf16_name_size(
	          mft_attribute,
	          NULL,
	          &error );

	mft_attribute->name = name;

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

/* Tests the libfsntfs_mft_attribute_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_utf16_name(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	uint16_t utf16_name[ 16 ];

	uint8_t *name            = NULL;
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_utf16_name(
	          mft_attribute,
	          utf16_name,
	          16,
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
	result = libfsntfs_mft_attribute_get_utf16_name(
	          NULL,
	          utf16_name,
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

	name = mft_attribute->name;

	mft_attribute->name = NULL;

	result = libfsntfs_mft_attribute_get_utf16_name(
	          mft_attribute,
	          utf16_name,
	          16,
	          &error );

	mft_attribute->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_attribute_get_utf16_name(
	          mft_attribute,
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

	result = libfsntfs_mft_attribute_get_utf16_name(
	          mft_attribute,
	          utf16_name,
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

	result = libfsntfs_mft_attribute_get_utf16_name(
	          mft_attribute,
	          utf16_name,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_attribute_compare_name_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_compare_name_with_utf8_string(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	uint8_t utf8_name1[ 5 ]  = { '$', 'S', 'D', 'H', 0 };
	uint8_t utf8_name2[ 6 ]  = { 'b', 'o', 'g', 'u', 's', 0 };
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_compare_name_with_utf8_string(
	          mft_attribute,
	          utf8_name1,
	          5,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_compare_name_with_utf8_string(
	          mft_attribute,
	          utf8_name2,
	          6,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_compare_name_with_utf8_string(
	          NULL,
	          utf8_name1,
	          5,
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

	result = libfsntfs_mft_attribute_compare_name_with_utf8_string(
	          mft_attribute,
	          NULL,
	          5,
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

	result = libfsntfs_mft_attribute_compare_name_with_utf8_string(
	          mft_attribute,
	          utf8_name1,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_attribute_compare_name_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_compare_name_with_utf16_string(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	uint16_t utf16_name1[ 5 ] = { '$', 'S', 'D', 'H', 0 };
	uint16_t utf16_name2[ 6 ] = { 'b', 'o', 'g', 'u', 's', 0 };
	libcerror_error_t *error  = NULL;
	int result                = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_compare_name_with_utf16_string(
	          mft_attribute,
	          utf16_name1,
	          5,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_compare_name_with_utf16_string(
	          mft_attribute,
	          utf16_name2,
	          6,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_compare_name_with_utf16_string(
	          NULL,
	          utf16_name1,
	          5,
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

	result = libfsntfs_mft_attribute_compare_name_with_utf16_string(
	          mft_attribute,
	          NULL,
	          5,
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

	result = libfsntfs_mft_attribute_compare_name_with_utf16_string(
	          mft_attribute,
	          utf16_name1,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_attribute_get_compression_unit_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_compression_unit_size(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	libcerror_error_t *error     = NULL;
	size_t compression_unit_size = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_compression_unit_size(
	          mft_attribute,
	          &compression_unit_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "compression_unit_size",
	 compression_unit_size,
	 (size_t) 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_compression_unit_size(
	          NULL,
	          &compression_unit_size,
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

	result = libfsntfs_mft_attribute_get_compression_unit_size(
	          mft_attribute,
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

/* Tests the libfsntfs_mft_attribute_get_resident_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_resident_data(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	libcerror_error_t *error = NULL;
	uint8_t *data            = NULL;
	size_t data_size         = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_resident_data(
	          mft_attribute,
	          &data,
	          &data_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "data",
	 data );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "data_size",
	 data_size,
	 (size_t) 56 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_resident_data(
	          NULL,
	          &data,
	          &data_size,
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

	result = libfsntfs_mft_attribute_get_resident_data(
	          mft_attribute,
	          NULL,
	          &data_size,
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

	result = libfsntfs_mft_attribute_get_resident_data(
	          mft_attribute,
	          &data,
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

/* Tests the libfsntfs_mft_attribute_get_number_of_data_runs function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_number_of_data_runs(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	libcerror_error_t *error = NULL;
	int number_of_data_runs  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_number_of_data_runs(
	          mft_attribute,
	          &number_of_data_runs,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "number_of_data_runs",
	 number_of_data_runs,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_number_of_data_runs(
	          NULL,
	          &number_of_data_runs,
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

	result = libfsntfs_mft_attribute_get_number_of_data_runs(
	          mft_attribute,
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

/* Tests the libfsntfs_mft_attribute_get_data_run_by_index function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_data_run_by_index(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	libcerror_error_t *error       = NULL;
	libfsntfs_data_run_t *data_run = NULL;
	int result                     = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_attribute_get_data_run_by_index(
	          mft_attribute,
	          0,
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

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_data_run_by_index(
	          NULL,
	          0,
	          &data_run,
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

	result = libfsntfs_mft_attribute_get_data_run_by_index(
	          mft_attribute,
	          -1,
	          &data_run,
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

	result = libfsntfs_mft_attribute_get_data_run_by_index(
	          mft_attribute,
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

/* Tests the libfsntfs_mft_attribute_get_data_extents_array function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_attribute_get_data_extents_array(
     libfsntfs_mft_attribute_t *mft_attribute )
{
	libcdata_array_t *data_extents_array = NULL;
	libcerror_error_t *error             = NULL;
	libfsntfs_io_handle_t *io_handle     = NULL;
	int number_of_data_extents           = 0;
	int result                           = 0;

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
	result = libfsntfs_mft_attribute_get_data_extents_array(
	          mft_attribute,
	          io_handle,
	          &data_extents_array,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_extents_array",
	 data_extents_array );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_array_get_number_of_entries(
	          data_extents_array,
	          &number_of_data_extents,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "number_of_data_extents",
	 number_of_data_extents,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_array_free(
	          &data_extents_array,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_extent_free,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "data_extents_array",
	 data_extents_array );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_attribute_get_data_extents_array(
	          NULL,
	          io_handle,
	          &data_extents_array,
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

	result = libfsntfs_mft_attribute_get_data_extents_array(
	          mft_attribute,
	          NULL,
	          &data_extents_array,
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

	result = libfsntfs_mft_attribute_get_data_extents_array(
	          mft_attribute,
	          io_handle,
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
	if( data_extents_array != NULL )
	{
		libcdata_array_free(
		 &data_extents_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_extent_free,
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
#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	libcerror_error_t *error                  = NULL;
	libfsntfs_io_handle_t *io_handle          = NULL;
	libfsntfs_mft_attribute_t *mft_attribute1 = NULL;
	libfsntfs_mft_attribute_t *mft_attribute2 = NULL;
	int result                                = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_attribute_initialize",
	 fsntfs_test_mft_attribute_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_attribute_free",
	 fsntfs_test_mft_attribute_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_attribute_read_data",
	 fsntfs_test_mft_attribute_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

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
	          &mft_attribute1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_attribute1",
	 mft_attribute1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute1,
	          io_handle,
	          fsntfs_test_mft_attribute_data1,
	          88,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_initialize(
	          &mft_attribute2,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_attribute2",
	 mft_attribute2 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute2,
	          io_handle,
	          fsntfs_test_mft_attribute_data2,
	          72,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_data_is_resident",
	 fsntfs_test_mft_attribute_data_is_resident,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_type",
	 fsntfs_test_mft_attribute_get_type,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_data_flags",
	 fsntfs_test_mft_attribute_get_data_flags,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_utf8_name_size",
	 fsntfs_test_mft_attribute_get_utf8_name_size,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_utf8_name",
	 fsntfs_test_mft_attribute_get_utf8_name,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_utf16_name_size",
	 fsntfs_test_mft_attribute_get_utf16_name_size,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_utf16_name",
	 fsntfs_test_mft_attribute_get_utf16_name,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_compare_name_with_utf8_string",
	 fsntfs_test_mft_attribute_compare_name_with_utf8_string,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_compare_name_with_utf16_string",
	 fsntfs_test_mft_attribute_compare_name_with_utf16_string,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_compression_unit_size",
	 fsntfs_test_mft_attribute_get_compression_unit_size,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_resident_data",
	 fsntfs_test_mft_attribute_get_resident_data,
	 mft_attribute1 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_number_of_data_runs",
	 fsntfs_test_mft_attribute_get_number_of_data_runs,
	 mft_attribute2 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_data_run_by_index",
	 fsntfs_test_mft_attribute_get_data_run_by_index,
	 mft_attribute2 );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_attribute_get_data_extents_array",
	 fsntfs_test_mft_attribute_get_data_extents_array,
	 mft_attribute2 );

	/* TODO add tests for libfsntfs_mft_attribute_get_next_attribute */

	/* TODO add tests for libfsntfs_mft_attribute_append_to_chain */

	/* Clean up
	 */
	result = libfsntfs_mft_attribute_free(
	          &mft_attribute2,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_attribute2",
	 mft_attribute2 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_free(
	          &mft_attribute1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_attribute1",
	 mft_attribute1 );

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
	if( mft_attribute2 != NULL )
	{
		libfsntfs_mft_attribute_free(
		 &mft_attribute2,
		 NULL );
	}
	if( mft_attribute1 != NULL )
	{
		libfsntfs_mft_attribute_free(
		 &mft_attribute1,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsntfs_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

