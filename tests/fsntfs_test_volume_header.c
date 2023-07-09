/*
 * Library volume_header type test program
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

#include "fsntfs_test_functions.h"
#include "fsntfs_test_libbfio.h"
#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_volume_header.h"

uint8_t fsntfs_test_volume_header_data1[ 512 ] = {
	0xeb, 0x52, 0x90, 0x4e, 0x54, 0x46, 0x53, 0x20, 0x20, 0x20, 0x20, 0x00, 0x02, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x3f, 0x00, 0x20, 0x00, 0x3f, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0xc0, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xeb, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x23, 0x56, 0xed, 0x50, 0x92, 0xed, 0x50, 0xba,
	0x00, 0x00, 0x00, 0x00, 0xfa, 0x33, 0xc0, 0x8e, 0xd0, 0xbc, 0x00, 0x7c, 0xfb, 0xb8, 0xc0, 0x07,
	0x8e, 0xd8, 0xe8, 0x16, 0x00, 0xb8, 0x00, 0x0d, 0x8e, 0xc0, 0x33, 0xdb, 0xc6, 0x06, 0x0e, 0x00,
	0x10, 0xe8, 0x53, 0x00, 0x68, 0x00, 0x0d, 0x68, 0x6a, 0x02, 0xcb, 0x8a, 0x16, 0x24, 0x00, 0xb4,
	0x08, 0xcd, 0x13, 0x73, 0x05, 0xb9, 0xff, 0xff, 0x8a, 0xf1, 0x66, 0x0f, 0xb6, 0xc6, 0x40, 0x66,
	0x0f, 0xb6, 0xd1, 0x80, 0xe2, 0x3f, 0xf7, 0xe2, 0x86, 0xcd, 0xc0, 0xed, 0x06, 0x41, 0x66, 0x0f,
	0xb7, 0xc9, 0x66, 0xf7, 0xe1, 0x66, 0xa3, 0x20, 0x00, 0xc3, 0xb4, 0x41, 0xbb, 0xaa, 0x55, 0x8a,
	0x16, 0x24, 0x00, 0xcd, 0x13, 0x72, 0x0f, 0x81, 0xfb, 0x55, 0xaa, 0x75, 0x09, 0xf6, 0xc1, 0x01,
	0x74, 0x04, 0xfe, 0x06, 0x14, 0x00, 0xc3, 0x66, 0x60, 0x1e, 0x06, 0x66, 0xa1, 0x10, 0x00, 0x66,
	0x03, 0x06, 0x1c, 0x00, 0x66, 0x3b, 0x06, 0x20, 0x00, 0x0f, 0x82, 0x3a, 0x00, 0x1e, 0x66, 0x6a,
	0x00, 0x66, 0x50, 0x06, 0x53, 0x66, 0x68, 0x10, 0x00, 0x01, 0x00, 0x80, 0x3e, 0x14, 0x00, 0x00,
	0x0f, 0x85, 0x0c, 0x00, 0xe8, 0xb3, 0xff, 0x80, 0x3e, 0x14, 0x00, 0x00, 0x0f, 0x84, 0x61, 0x00,
	0xb4, 0x42, 0x8a, 0x16, 0x24, 0x00, 0x16, 0x1f, 0x8b, 0xf4, 0xcd, 0x13, 0x66, 0x58, 0x5b, 0x07,
	0x66, 0x58, 0x66, 0x58, 0x1f, 0xeb, 0x2d, 0x66, 0x33, 0xd2, 0x66, 0x0f, 0xb7, 0x0e, 0x18, 0x00,
	0x66, 0xf7, 0xf1, 0xfe, 0xc2, 0x8a, 0xca, 0x66, 0x8b, 0xd0, 0x66, 0xc1, 0xea, 0x10, 0xf7, 0x36,
	0x1a, 0x00, 0x86, 0xd6, 0x8a, 0x16, 0x24, 0x00, 0x8a, 0xe8, 0xc0, 0xe4, 0x06, 0x0a, 0xcc, 0xb8,
	0x01, 0x02, 0xcd, 0x13, 0x0f, 0x82, 0x19, 0x00, 0x8c, 0xc0, 0x05, 0x20, 0x00, 0x8e, 0xc0, 0x66,
	0xff, 0x06, 0x10, 0x00, 0xff, 0x0e, 0x0e, 0x00, 0x0f, 0x85, 0x6f, 0xff, 0x07, 0x1f, 0x66, 0x61,
	0xc3, 0xa0, 0xf8, 0x01, 0xe8, 0x09, 0x00, 0xa0, 0xfb, 0x01, 0xe8, 0x03, 0x00, 0xfb, 0xeb, 0xfe,
	0xb4, 0x01, 0x8b, 0xf0, 0xac, 0x3c, 0x00, 0x74, 0x09, 0xb4, 0x0e, 0xbb, 0x07, 0x00, 0xcd, 0x10,
	0xeb, 0xf2, 0xc3, 0x0d, 0x0a, 0x41, 0x20, 0x64, 0x69, 0x73, 0x6b, 0x20, 0x72, 0x65, 0x61, 0x64,
	0x20, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x20, 0x6f, 0x63, 0x63, 0x75, 0x72, 0x72, 0x65, 0x64, 0x00,
	0x0d, 0x0a, 0x4e, 0x54, 0x4c, 0x44, 0x52, 0x20, 0x69, 0x73, 0x20, 0x6d, 0x69, 0x73, 0x73, 0x69,
	0x6e, 0x67, 0x00, 0x0d, 0x0a, 0x4e, 0x54, 0x4c, 0x44, 0x52, 0x20, 0x69, 0x73, 0x20, 0x63, 0x6f,
	0x6d, 0x70, 0x72, 0x65, 0x73, 0x73, 0x65, 0x64, 0x00, 0x0d, 0x0a, 0x50, 0x72, 0x65, 0x73, 0x73,
	0x20, 0x43, 0x74, 0x72, 0x6c, 0x2b, 0x41, 0x6c, 0x74, 0x2b, 0x44, 0x65, 0x6c, 0x20, 0x74, 0x6f,
	0x20, 0x72, 0x65, 0x73, 0x74, 0x61, 0x72, 0x74, 0x0d, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xa0, 0xb3, 0xc9, 0x00, 0x00, 0x55, 0xaa };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_volume_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_initialize(
     void )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_volume_header_t *volume_header = NULL;
	int result                               = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests          = 1;
	int number_of_memset_fail_tests          = 1;
	int test_number                          = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_initialize(
	          &volume_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_header",
	 volume_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_volume_header_free(
	          &volume_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "volume_header",
	 volume_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_volume_header_initialize(
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

	volume_header = (libfsntfs_volume_header_t *) 0x12345678UL;

	result = libfsntfs_volume_header_initialize(
	          &volume_header,
	          &error );

	volume_header = NULL;

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
		/* Test libfsntfs_volume_header_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_volume_header_initialize(
		          &volume_header,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( volume_header != NULL )
			{
				libfsntfs_volume_header_free(
				 &volume_header,
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
			 "volume_header",
			 volume_header );

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
		/* Test libfsntfs_volume_header_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_volume_header_initialize(
		          &volume_header,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( volume_header != NULL )
			{
				libfsntfs_volume_header_free(
				 &volume_header,
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
			 "volume_header",
			 volume_header );

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
	if( volume_header != NULL )
	{
		libfsntfs_volume_header_free(
		 &volume_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_volume_header_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_volume_header_free(
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

/* Tests the libfsntfs_volume_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_read_data(
     void )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_volume_header_t *volume_header = NULL;
	int result                               = 0;

	/* Initialize test
	 */
	result = libfsntfs_volume_header_initialize(
	          &volume_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_header",
	 volume_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
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
	result = libfsntfs_volume_header_read_data(
	          NULL,
	          fsntfs_test_volume_header_data1,
	          512,
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

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          NULL,
	          512,
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

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
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

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
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

	/* Test error case where signature is invalid
	 */
	byte_stream_copy_from_uint64_little_endian(
	 &( fsntfs_test_volume_header_data1[ 3 ] ),
	 0xffffffffffffffffUL );

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	byte_stream_copy_from_uint64_little_endian(
	 &( fsntfs_test_volume_header_data1[ 3 ] ),
	 0x202020205346544eUL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where bytes per sector is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_volume_header_data1[ 11 ] ),
	 0xffff );

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_volume_header_data1[ 11 ] ),
	 512 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where sectors per cluster block is invalid
	 */
	fsntfs_test_volume_header_data1[ 13 ] = 0xc0;

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	fsntfs_test_volume_header_data1[ 13 ] = 0x01;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where cluster block size is invalid
	 */
	fsntfs_test_volume_header_data1[ 13 ] = 0x1f;

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	fsntfs_test_volume_header_data1[ 13 ] = 0x01;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where total number of sectors is invalid
	 */
	byte_stream_copy_from_uint64_little_endian(
	 &( fsntfs_test_volume_header_data1[ 40 ] ),
	 0x0080000000000001UL );

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	byte_stream_copy_from_uint64_little_endian(
	 &( fsntfs_test_volume_header_data1[ 40 ] ),
	 0x0000000000003ec0UL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where MFT entry size is invalid
	 */
	fsntfs_test_volume_header_data1[ 64 ] = 0x00;

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	fsntfs_test_volume_header_data1[ 64 ] = 0x02;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where MFT entry size is invalid
	 */
	fsntfs_test_volume_header_data1[ 64 ] = 0xdf;

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	fsntfs_test_volume_header_data1[ 64 ] = 0x02;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where MFT entry size is invalid
	 */
	fsntfs_test_volume_header_data1[ 64 ] = 0xff;

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	fsntfs_test_volume_header_data1[ 64 ] = 0x02;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where index entry size is invalid
	 */
	fsntfs_test_volume_header_data1[ 68 ] = 0x00;

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	fsntfs_test_volume_header_data1[ 68 ] = 0x02;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where index entry size is invalid
	 */
	fsntfs_test_volume_header_data1[ 68 ] = 0xdf;

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	fsntfs_test_volume_header_data1[ 68 ] = 0x02;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where index entry size is invalid
	 */
	fsntfs_test_volume_header_data1[ 68 ] = 0xff;

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	fsntfs_test_volume_header_data1[ 68 ] = 0x02;

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
	result = libfsntfs_volume_header_free(
	          &volume_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "volume_header",
	 volume_header );

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
	if( volume_header != NULL )
	{
		libfsntfs_volume_header_free(
		 &volume_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_volume_header_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle         = NULL;
	libcerror_error_t *error                 = NULL;
	libfsntfs_volume_header_t *volume_header = NULL;
	int result                               = 0;

	/* Initialize test
	 */
	result = libfsntfs_volume_header_initialize(
	          &volume_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_header",
	 volume_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = fsntfs_test_open_file_io_handle(
	          &file_io_handle,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_read_file_io_handle(
	          volume_header,
	          file_io_handle,
	          0,
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
	result = libfsntfs_volume_header_read_file_io_handle(
	          NULL,
	          file_io_handle,
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

	result = libfsntfs_volume_header_read_file_io_handle(
	          volume_header,
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

	result = libfsntfs_volume_header_read_file_io_handle(
	          volume_header,
	          file_io_handle,
	          -1,
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

	/* Clean up file IO handle
	 */
	result = fsntfs_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data too small
	 */
	result = fsntfs_test_open_file_io_handle(
	          &file_io_handle,
	          fsntfs_test_volume_header_data1,
	          8,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_volume_header_read_file_io_handle(
	          volume_header,
	          file_io_handle,
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

	result = fsntfs_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data invalid
	 */
	result = fsntfs_test_open_file_io_handle(
	          &file_io_handle,
	          fsntfs_test_volume_header_data1,
	          512,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	byte_stream_copy_from_uint64_little_endian(
	 &( fsntfs_test_volume_header_data1[ 3 ] ),
	 0xffffffffffffffffUL );

	result = libfsntfs_volume_header_read_file_io_handle(
	          volume_header,
	          file_io_handle,
	          0,
	          &error );

	byte_stream_copy_from_uint64_little_endian(
	 &( fsntfs_test_volume_header_data1[ 3 ] ),
	 0x202020205346544eUL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = fsntfs_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsntfs_volume_header_free(
	          &volume_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "volume_header",
	 volume_header );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( volume_header != NULL )
	{
		libfsntfs_volume_header_free(
		 &volume_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_volume_header_get_bytes_per_sector function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_get_bytes_per_sector(
     libfsntfs_volume_header_t *volume_header )
{
	libcerror_error_t *error  = NULL;
	uint16_t bytes_per_sector = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_get_bytes_per_sector(
	          volume_header,
	          &bytes_per_sector,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "bytes_per_sector",
	 bytes_per_sector,
	 512 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_volume_header_get_bytes_per_sector(
	          NULL,
	          &bytes_per_sector,
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

	result = libfsntfs_volume_header_get_bytes_per_sector(
	          volume_header,
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

/* Tests the libfsntfs_volume_header_get_cluster_block_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_get_cluster_block_size(
     libfsntfs_volume_header_t *volume_header )
{
	libcerror_error_t *error    = NULL;
	size32_t cluster_block_size = 0;
	int result                  = 0;

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_get_cluster_block_size(
	          volume_header,
	          &cluster_block_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT32(
	 "cluster_block_size",
	 (uint32_t) cluster_block_size,
	 512 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_volume_header_get_cluster_block_size(
	          NULL,
	          &cluster_block_size,
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

	result = libfsntfs_volume_header_get_cluster_block_size(
	          volume_header,
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

/* Tests the libfsntfs_volume_header_get_mft_entry_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_get_mft_entry_size(
     libfsntfs_volume_header_t *volume_header )
{
	libcerror_error_t *error = NULL;
	size32_t mft_entry_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_get_mft_entry_size(
	          volume_header,
	          &mft_entry_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT32(
	 "mft_entry_size",
	 (uint32_t) mft_entry_size,
	 1024 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_volume_header_get_mft_entry_size(
	          NULL,
	          &mft_entry_size,
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

	result = libfsntfs_volume_header_get_mft_entry_size(
	          volume_header,
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

/* Tests the libfsntfs_volume_header_get_index_entry_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_get_index_entry_size(
     libfsntfs_volume_header_t *volume_header )
{
	libcerror_error_t *error  = NULL;
	size32_t index_entry_size = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_get_index_entry_size(
	          volume_header,
	          &index_entry_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT32(
	 "index_entry_size",
	 (uint32_t) index_entry_size,
	 1024 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_volume_header_get_index_entry_size(
	          NULL,
	          &index_entry_size,
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

	result = libfsntfs_volume_header_get_index_entry_size(
	          volume_header,
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

/* Tests the libfsntfs_volume_header_get_volume_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_get_volume_size(
     libfsntfs_volume_header_t *volume_header )
{
	libcerror_error_t *error = NULL;
	size64_t volume_size     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_get_volume_size(
	          volume_header,
	          &volume_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "volume_size",
	 (uint64_t) volume_size,
	 (uint64_t) 8225280 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_volume_header_get_volume_size(
	          NULL,
	          &volume_size,
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

	result = libfsntfs_volume_header_get_volume_size(
	          volume_header,
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

/* Tests the libfsntfs_volume_header_get_volume_serial_number function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_get_volume_serial_number(
     libfsntfs_volume_header_t *volume_header )
{
	libcerror_error_t *error      = NULL;
	uint64_t volume_serial_number = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_get_volume_serial_number(
	          volume_header,
	          &volume_serial_number,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT64(
	 "volume_serial_number",
	 volume_serial_number,
	 (uint64_t) 0xba50ed9250ed5623UL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_volume_header_get_volume_serial_number(
	          NULL,
	          &volume_serial_number,
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

	result = libfsntfs_volume_header_get_volume_serial_number(
	          volume_header,
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

/* Tests the libfsntfs_volume_header_get_mft_offset function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_volume_header_get_mft_offset(
     libfsntfs_volume_header_t *volume_header )
{
	libcerror_error_t *error = NULL;
	off64_t mft_offset       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_volume_header_get_mft_offset(
	          volume_header,
	          &mft_offset,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_INT64(
	 "mft_offset",
	 mft_offset,
	 (int64_t) 0x0029d600UL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_volume_header_get_mft_offset(
	          NULL,
	          &mft_offset,
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

	result = libfsntfs_volume_header_get_mft_offset(
	          volume_header,
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

	libcerror_error_t *error                 = NULL;
	libfsntfs_volume_header_t *volume_header = NULL;
	int result                               = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_volume_header_initialize",
	 fsntfs_test_volume_header_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_volume_header_free",
	 fsntfs_test_volume_header_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_volume_header_read_data",
	 fsntfs_test_volume_header_read_data );

	FSNTFS_TEST_RUN(
	 "libfsntfs_volume_header_read_file_io_handle",
	 fsntfs_test_volume_header_read_file_io_handle );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libfsntfs_volume_header_initialize(
	          &volume_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_header",
	 volume_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_volume_header_read_data(
	          volume_header,
	          fsntfs_test_volume_header_data1,
	          512,
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
	 "libfsntfs_volume_header_get_bytes_per_sector",
	 fsntfs_test_volume_header_get_bytes_per_sector,
	 volume_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_volume_header_get_cluster_block_size",
	 fsntfs_test_volume_header_get_cluster_block_size,
	 volume_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_volume_header_get_mft_entry_size",
	 fsntfs_test_volume_header_get_mft_entry_size,
	 volume_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_volume_header_get_index_entry_size",
	 fsntfs_test_volume_header_get_index_entry_size,
	 volume_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_volume_header_get_volume_size",
	 fsntfs_test_volume_header_get_volume_size,
	 volume_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_volume_header_get_volume_serial_number",
	 fsntfs_test_volume_header_get_volume_serial_number,
	 volume_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_volume_header_get_mft_offset",
	 fsntfs_test_volume_header_get_mft_offset,
	 volume_header );

	/* Clean up
	 */
	result = libfsntfs_volume_header_free(
	          &volume_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "volume_header",
	 volume_header );

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
	if( volume_header != NULL )
	{
		libfsntfs_volume_header_free(
		 &volume_header,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

