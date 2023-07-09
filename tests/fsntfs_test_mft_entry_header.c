/*
 * Library mft_entry_header type test program
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

#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_mft_entry_header.h"

uint8_t fsntfs_test_mft_entry_header_data1[ 48 ] = {
	0x46, 0x49, 0x4c, 0x45, 0x30, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00, 0x98, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t fsntfs_test_mft_entry_header_data2[ 42 ] = {
	0x46, 0x49, 0x4c, 0x45, 0x2a, 0x00, 0x03, 0x00, 0x96, 0x53, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x30, 0x00, 0x01, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_mft_entry_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_header_initialize(
     void )
{
	libcerror_error_t *error                       = NULL;
	libfsntfs_mft_entry_header_t *mft_entry_header = NULL;
	int result                                     = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                = 1;
	int number_of_memset_fail_tests                = 1;
	int test_number                                = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_header_initialize(
	          &mft_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry_header",
	 mft_entry_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_header_free(
	          &mft_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry_header",
	 mft_entry_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_header_initialize(
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

	mft_entry_header = (libfsntfs_mft_entry_header_t *) 0x12345678UL;

	result = libfsntfs_mft_entry_header_initialize(
	          &mft_entry_header,
	          &error );

	mft_entry_header = NULL;

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
		/* Test libfsntfs_mft_entry_header_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_mft_entry_header_initialize(
		          &mft_entry_header,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( mft_entry_header != NULL )
			{
				libfsntfs_mft_entry_header_free(
				 &mft_entry_header,
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
			 "mft_entry_header",
			 mft_entry_header );

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
		/* Test libfsntfs_mft_entry_header_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_mft_entry_header_initialize(
		          &mft_entry_header,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( mft_entry_header != NULL )
			{
				libfsntfs_mft_entry_header_free(
				 &mft_entry_header,
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
			 "mft_entry_header",
			 mft_entry_header );

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
	if( mft_entry_header != NULL )
	{
		libfsntfs_mft_entry_header_free(
		 &mft_entry_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_header_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_header_free(
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

/* Tests the libfsntfs_mft_entry_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_header_read_data(
     void )
{
	libcerror_error_t *error                       = NULL;
	libfsntfs_mft_entry_header_t *mft_entry_header = NULL;
	int result                                     = 0;

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_header_initialize(
	          &mft_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry_header",
	 mft_entry_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_header_read_data(
	          mft_entry_header,
	          fsntfs_test_mft_entry_header_data1,
	          48,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "mft_entry_header->is_bad",
	 mft_entry_header->is_bad,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_header_read_data(
	          mft_entry_header,
	          fsntfs_test_mft_entry_header_data2,
	          42,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "mft_entry_header->is_bad",
	 mft_entry_header->is_bad,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular case where signature is "BAAD"
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( fsntfs_test_mft_entry_header_data1[ 0 ] ),
	 0x44414142UL );

	result = libfsntfs_mft_entry_header_read_data(
	          mft_entry_header,
	          fsntfs_test_mft_entry_header_data1,
	          48,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( fsntfs_test_mft_entry_header_data1[ 0 ] ),
	 0x454c4946UL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "mft_entry_header->is_bad",
	 mft_entry_header->is_bad,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_header_read_data(
	          NULL,
	          fsntfs_test_mft_entry_header_data1,
	          48,
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

	result = libfsntfs_mft_entry_header_read_data(
	          mft_entry_header,
	          NULL,
	          48,
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

	result = libfsntfs_mft_entry_header_read_data(
	          mft_entry_header,
	          fsntfs_test_mft_entry_header_data1,
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

	result = libfsntfs_mft_entry_header_read_data(
	          mft_entry_header,
	          fsntfs_test_mft_entry_header_data1,
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
	byte_stream_copy_from_uint32_little_endian(
	 &( fsntfs_test_mft_entry_header_data1[ 0 ] ),
	 0xffffffffUL );

	result = libfsntfs_mft_entry_header_read_data(
	          mft_entry_header,
	          fsntfs_test_mft_entry_header_data1,
	          48,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( fsntfs_test_mft_entry_header_data1[ 0 ] ),
	 0x454c4946UL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_header_free(
	          &mft_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry_header",
	 mft_entry_header );

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
	if( mft_entry_header != NULL )
	{
		libfsntfs_mft_entry_header_free(
		 &mft_entry_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_header_get_fixup_values_offset function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_header_get_fixup_values_offset(
     libfsntfs_mft_entry_header_t *mft_entry_header )
{
	libcerror_error_t *error     = NULL;
	uint16_t fixup_values_offset = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_header_get_fixup_values_offset(
	          mft_entry_header,
	          &fixup_values_offset,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "fixup_values_offset",
	 fixup_values_offset,
	 48 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_header_get_fixup_values_offset(
	          NULL,
	          &fixup_values_offset,
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

	result = libfsntfs_mft_entry_header_get_fixup_values_offset(
	          mft_entry_header,
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

/* Tests the libfsntfs_mft_entry_header_get_number_of_fixup_values function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_header_get_number_of_fixup_values(
     libfsntfs_mft_entry_header_t *mft_entry_header )
{
	libcerror_error_t *error        = NULL;
	uint16_t number_of_fixup_values = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_header_get_number_of_fixup_values(
	          mft_entry_header,
	          &number_of_fixup_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "number_of_fixup_values",
	 number_of_fixup_values,
	 3 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_header_get_number_of_fixup_values(
	          NULL,
	          &number_of_fixup_values,
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

	result = libfsntfs_mft_entry_header_get_number_of_fixup_values(
	          mft_entry_header,
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

/* Tests the libfsntfs_mft_entry_header_get_attributes_offset function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_header_get_attributes_offset(
     libfsntfs_mft_entry_header_t *mft_entry_header )
{
	libcerror_error_t *error   = NULL;
	uint16_t attributes_offset = 0;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_header_get_attributes_offset(
	          mft_entry_header,
	          &attributes_offset,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "attributes_offset",
	 attributes_offset,
	 56 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_header_get_attributes_offset(
	          NULL,
	          &attributes_offset,
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

	result = libfsntfs_mft_entry_header_get_attributes_offset(
	          mft_entry_header,
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

/* Tests the libfsntfs_mft_entry_header_get_used_entry_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_header_get_used_entry_size(
     libfsntfs_mft_entry_header_t *mft_entry_header )
{
	libcerror_error_t *error = NULL;
	uint16_t used_entry_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_header_get_used_entry_size(
	          mft_entry_header,
	          &used_entry_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "used_entry_size",
	 used_entry_size,
	 408 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_header_get_used_entry_size(
	          NULL,
	          &used_entry_size,
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

	result = libfsntfs_mft_entry_header_get_used_entry_size(
	          mft_entry_header,
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

/* Tests the libfsntfs_mft_entry_header_get_total_entry_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_header_get_total_entry_size(
     libfsntfs_mft_entry_header_t *mft_entry_header )
{
	libcerror_error_t *error  = NULL;
	uint16_t total_entry_size = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_header_get_total_entry_size(
	          mft_entry_header,
	          &total_entry_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "total_entry_size",
	 total_entry_size,
	 1024 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_header_get_total_entry_size(
	          NULL,
	          &total_entry_size,
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

	result = libfsntfs_mft_entry_header_get_total_entry_size(
	          mft_entry_header,
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

	libcerror_error_t *error                       = NULL;
	libfsntfs_mft_entry_header_t *mft_entry_header = NULL;
	int result                                     = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_header_initialize",
	 fsntfs_test_mft_entry_header_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_header_free",
	 fsntfs_test_mft_entry_header_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_header_read_data",
	 fsntfs_test_mft_entry_header_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_header_initialize(
	          &mft_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry_header",
	 mft_entry_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_header_read_data(
	          mft_entry_header,
	          fsntfs_test_mft_entry_header_data1,
	          48,
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
	 "libfsntfs_mft_entry_header_get_fixup_values_offset",
	 fsntfs_test_mft_entry_header_get_fixup_values_offset,
	 mft_entry_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_header_get_number_of_fixup_values",
	 fsntfs_test_mft_entry_header_get_number_of_fixup_values,
	 mft_entry_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_header_get_attributes_offset",
	 fsntfs_test_mft_entry_header_get_attributes_offset,
	 mft_entry_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_header_get_used_entry_size",
	 fsntfs_test_mft_entry_header_get_used_entry_size,
	 mft_entry_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_header_get_total_entry_size",
	 fsntfs_test_mft_entry_header_get_total_entry_size,
	 mft_entry_header );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_header_free(
	          &mft_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry_header",
	 mft_entry_header );

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
	if( mft_entry_header != NULL )
	{
		libfsntfs_mft_entry_header_free(
		 &mft_entry_header,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

