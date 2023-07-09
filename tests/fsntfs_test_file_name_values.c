/*
 * Library file_name_values type test program
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
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_file_name_values.h"
#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_mft_attribute.h"

uint8_t fsntfs_test_file_name_values_data1[ 104 ] = {
	0x30, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x03, 0x00,
	0x4a, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00,
	0xad, 0xca, 0xbc, 0x0c, 0xdc, 0x8e, 0xd0, 0x01, 0xad, 0xca, 0xbc, 0x0c, 0xdc, 0x8e, 0xd0, 0x01,
	0xad, 0xca, 0xbc, 0x0c, 0xdc, 0x8e, 0xd0, 0x01, 0xad, 0xca, 0xbc, 0x0c, 0xdc, 0x8e, 0xd0, 0x01,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x03, 0x24, 0x00, 0x4d, 0x00, 0x46, 0x00,
	0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_file_name_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_initialize(
     void )
{
	libcerror_error_t *error                       = NULL;
	libfsntfs_file_name_values_t *file_name_values = NULL;
	int result                                     = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                = 1;
	int number_of_memset_fail_tests                = 1;
	int test_number                                = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_initialize(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_file_name_values_free(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_initialize(
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

	file_name_values = (libfsntfs_file_name_values_t *) 0x12345678UL;

	result = libfsntfs_file_name_values_initialize(
	          &file_name_values,
	          &error );

	file_name_values = NULL;

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
		/* Test libfsntfs_file_name_values_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_file_name_values_initialize(
		          &file_name_values,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( file_name_values != NULL )
			{
				libfsntfs_file_name_values_free(
				 &file_name_values,
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
			 "file_name_values",
			 file_name_values );

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
		/* Test libfsntfs_file_name_values_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_file_name_values_initialize(
		          &file_name_values,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( file_name_values != NULL )
			{
				libfsntfs_file_name_values_free(
				 &file_name_values,
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
			 "file_name_values",
			 file_name_values );

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
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_file_name_values_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_free(
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

/* Tests the libfsntfs_file_name_values_clone function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_clone(
     void )
{
	uint8_t name_utf16_stream[ 10 ] = {
		't', 0, 'e', 0, 's', 0, 't', 0, 0, 0 };

	libcerror_error_t *error                                   = NULL;
	libfsntfs_file_name_values_t *destination_file_name_values = NULL;
	libfsntfs_file_name_values_t *source_file_name_values      = NULL;
	int result                                                 = 0;

	/* Initialize test
	 */
	result = libfsntfs_file_name_values_initialize(
	          &source_file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "source_file_name_values",
	 source_file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_file_name_values_set_name(
	          source_file_name_values,
	          name_utf16_stream,
	          10,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_clone(
	          &destination_file_name_values,
	          source_file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "destination_file_name_values",
	 destination_file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_file_name_values_free(
	          &destination_file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "destination_file_name_values",
	 destination_file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_file_name_values_clone(
	          &destination_file_name_values,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "destination_file_name_values",
	 destination_file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_clone(
	          NULL,
	          source_file_name_values,
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

	destination_file_name_values = (libfsntfs_file_name_values_t *) 0x12345678UL;

	result = libfsntfs_file_name_values_clone(
	          &destination_file_name_values,
	          source_file_name_values,
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

	destination_file_name_values = NULL;

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	/* Test libfsntfs_file_name_values_clone with malloc failing
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_file_name_values_clone(
	          &destination_file_name_values,
	          source_file_name_values,
	          &error );

	if( fsntfs_test_malloc_attempts_before_fail != -1 )
	{
		fsntfs_test_malloc_attempts_before_fail = -1;

		if( destination_file_name_values != NULL )
		{
			libfsntfs_file_name_values_free(
			 &destination_file_name_values,
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
		 "destination_file_name_values",
		 destination_file_name_values );

		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test libfsntfs_file_name_values_clone with memcpy failing
	 */
	fsntfs_test_memcpy_attempts_before_fail = 0;

	result = libfsntfs_file_name_values_clone(
	          &destination_file_name_values,
	          source_file_name_values,
	          &error );

	if( fsntfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsntfs_test_memcpy_attempts_before_fail = -1;

		if( destination_file_name_values != NULL )
		{
			libfsntfs_file_name_values_free(
			 &destination_file_name_values,
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
		 "destination_file_name_values",
		 destination_file_name_values );

		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfsntfs_file_name_values_free(
	          &source_file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "source_file_name_values",
	 source_file_name_values );

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
	if( destination_file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &destination_file_name_values,
		 NULL );
	}
	if( source_file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &source_file_name_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_file_name_values_set_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_set_name(
     void )
{
	uint8_t name_utf16_stream[ 10 ] = {
		't', 0, 'e', 0, 's', 0, 't', 0, 0, 0 };

	libcerror_error_t *error                       = NULL;
	libfsntfs_file_name_values_t *file_name_values = NULL;
	int result                                     = 0;

	/* Initialize test
	 */
	result = libfsntfs_file_name_values_initialize(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_set_name(
	          file_name_values,
	          name_utf16_stream,
	          10,
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
	result = libfsntfs_file_name_values_set_name(
	          file_name_values,
	          name_utf16_stream,
	          10,
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
	result = libfsntfs_file_name_values_free(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsntfs_file_name_values_initialize(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_set_name(
	          NULL,
	          name_utf16_stream,
	          10,
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

	result = libfsntfs_file_name_values_set_name(
	          file_name_values,
	          NULL,
	          10,
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

	result = libfsntfs_file_name_values_set_name(
	          file_name_values,
	          name_utf16_stream,
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

	/* Test libfsntfs_file_name_values_set_name with malloc failing
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_file_name_values_set_name(
	          file_name_values,
	          name_utf16_stream,
	          10,
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

	/* Test libfsntfs_file_name_values_set_name with memcpy failing
	 */
	fsntfs_test_memcpy_attempts_before_fail = 0;

	result = libfsntfs_file_name_values_set_name(
	          file_name_values,
	          name_utf16_stream,
	          10,
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

	/* Clean up
	 */
	result = libfsntfs_file_name_values_free(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "file_name_values",
	 file_name_values );

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
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_file_name_values_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_read_data(
     void )
{
	libcerror_error_t *error                       = NULL;
	libfsntfs_file_name_values_t *file_name_values = NULL;
	int result                                     = 0;

	/* Initialize test
	 */
	result = libfsntfs_file_name_values_initialize(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_read_data(
	          file_name_values,
	          &( fsntfs_test_file_name_values_data1[ 24 ] ),
	          74,
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
	result = libfsntfs_file_name_values_read_data(
	          file_name_values,
	          &( fsntfs_test_file_name_values_data1[ 24 ] ),
	          74,
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
	result = libfsntfs_file_name_values_free(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsntfs_file_name_values_initialize(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_read_data(
	          NULL,
	          &( fsntfs_test_file_name_values_data1[ 24 ] ),
	          74,
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

	result = libfsntfs_file_name_values_read_data(
	          file_name_values,
	          NULL,
	          74,
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

	result = libfsntfs_file_name_values_read_data(
	          file_name_values,
	          &( fsntfs_test_file_name_values_data1[ 24 ] ),
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

	result = libfsntfs_file_name_values_read_data(
	          file_name_values,
	          &( fsntfs_test_file_name_values_data1[ 24 ] ),
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

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	/* Test libfsntfs_file_name_values_read_data with malloc failing in libfsntfs_file_name_values_set_name
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_file_name_values_read_data(
	          file_name_values,
	          &( fsntfs_test_file_name_values_data1[ 24 ] ),
	          74,
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
	result = libfsntfs_file_name_values_free(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "file_name_values",
	 file_name_values );

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
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_file_name_values_read_from_mft_attribute function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_read_from_mft_attribute(
     void )
{
	libcerror_error_t *error                       = NULL;
	libfsntfs_file_name_values_t *file_name_values = NULL;
	libfsntfs_io_handle_t *io_handle               = NULL;
	libfsntfs_mft_attribute_t *mft_attribute       = NULL;
	int result                                     = 0;

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
	          fsntfs_test_file_name_values_data1,
	          104,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_file_name_values_initialize(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_read_from_mft_attribute(
	          file_name_values,
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
	result = libfsntfs_file_name_values_read_from_mft_attribute(
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

	result = libfsntfs_file_name_values_read_from_mft_attribute(
	          file_name_values,
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
	result = libfsntfs_file_name_values_free(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "file_name_values",
	 file_name_values );

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
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_parent_file_reference function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_parent_file_reference(
     libfsntfs_file_name_values_t *file_name_values )
{
	libcerror_error_t *error       = NULL;
	uint64_t parent_file_reference = 0;
	int result                     = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_parent_file_reference(
	          file_name_values,
	          &parent_file_reference,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_parent_file_reference(
	          NULL,
	          &parent_file_reference,
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

	result = libfsntfs_file_name_values_get_parent_file_reference(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_creation_time(
     libfsntfs_file_name_values_t *file_name_values )
{
	libcerror_error_t *error = NULL;
	uint64_t creation_time   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_creation_time(
	          file_name_values,
	          &creation_time,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_creation_time(
	          NULL,
	          &creation_time,
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

	result = libfsntfs_file_name_values_get_creation_time(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_modification_time function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_modification_time(
     libfsntfs_file_name_values_t *file_name_values )
{
	libcerror_error_t *error   = NULL;
	uint64_t modification_time = 0;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_modification_time(
	          file_name_values,
	          &modification_time,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_modification_time(
	          NULL,
	          &modification_time,
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

	result = libfsntfs_file_name_values_get_modification_time(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_access_time function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_access_time(
     libfsntfs_file_name_values_t *file_name_values )
{
	libcerror_error_t *error = NULL;
	uint64_t access_time     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_access_time(
	          file_name_values,
	          &access_time,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_access_time(
	          NULL,
	          &access_time,
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

	result = libfsntfs_file_name_values_get_access_time(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_entry_modification_time function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_entry_modification_time(
     libfsntfs_file_name_values_t *file_name_values )
{
	libcerror_error_t *error         = NULL;
	uint64_t entry_modification_time = 0;
	int result                       = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_entry_modification_time(
	          file_name_values,
	          &entry_modification_time,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_entry_modification_time(
	          NULL,
	          &entry_modification_time,
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

	result = libfsntfs_file_name_values_get_entry_modification_time(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_file_attribute_flags function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_file_attribute_flags(
     libfsntfs_file_name_values_t *file_name_values )
{
	libcerror_error_t *error      = NULL;
	uint32_t file_attribute_flags = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_file_attribute_flags(
	          file_name_values,
	          &file_attribute_flags,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_file_attribute_flags(
	          NULL,
	          &file_attribute_flags,
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

	result = libfsntfs_file_name_values_get_file_attribute_flags(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_utf8_name_size(
     libfsntfs_file_name_values_t *file_name_values )
{
	libcerror_error_t *error = NULL;
	size_t utf8_name_size    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_utf8_name_size(
	          file_name_values,
	          &utf8_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_utf8_name_size(
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

	result = libfsntfs_file_name_values_get_utf8_name_size(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_utf8_name(
     libfsntfs_file_name_values_t *file_name_values )
{
	uint8_t utf8_name[ 16 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_utf8_name(
	          file_name_values,
	          utf8_name,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_utf8_name(
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

	result = libfsntfs_file_name_values_get_utf8_name(
	          file_name_values,
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

	result = libfsntfs_file_name_values_get_utf8_name(
	          file_name_values,
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

	result = libfsntfs_file_name_values_get_utf8_name(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_utf16_name_size(
     libfsntfs_file_name_values_t *file_name_values )
{
	libcerror_error_t *error = NULL;
	size_t utf16_name_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_utf16_name_size(
	          file_name_values,
	          &utf16_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_utf16_name_size(
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

	result = libfsntfs_file_name_values_get_utf16_name_size(
	          file_name_values,
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

/* Tests the libfsntfs_file_name_values_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_file_name_values_get_utf16_name(
     libfsntfs_file_name_values_t *file_name_values )
{
	uint16_t utf16_name[ 16 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_file_name_values_get_utf16_name(
	          file_name_values,
	          utf16_name,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_file_name_values_get_utf16_name(
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

	result = libfsntfs_file_name_values_get_utf16_name(
	          file_name_values,
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

	result = libfsntfs_file_name_values_get_utf16_name(
	          file_name_values,
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

	result = libfsntfs_file_name_values_get_utf16_name(
	          file_name_values,
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
	libfsntfs_file_name_values_t *file_name_values = NULL;
	int result                                     = 0;
#endif

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_file_name_values_initialize",
	 fsntfs_test_file_name_values_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_file_name_values_free",
	 fsntfs_test_file_name_values_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_file_name_values_clone",
	 fsntfs_test_file_name_values_clone );

	FSNTFS_TEST_RUN(
	 "libfsntfs_file_name_values_set_name",
	 fsntfs_test_file_name_values_set_name );

	FSNTFS_TEST_RUN(
	 "libfsntfs_file_name_values_read_data",
	 fsntfs_test_file_name_values_read_data );

	FSNTFS_TEST_RUN(
	 "libfsntfs_file_name_values_read_from_mft_attribute",
	 fsntfs_test_file_name_values_read_from_mft_attribute );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize file_name_values for tests
	 */
	result = libfsntfs_file_name_values_initialize(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_name_values",
	 file_name_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_file_name_values_read_data(
	          file_name_values,
	          &( fsntfs_test_file_name_values_data1[ 24 ] ),
	          74,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_parent_file_reference",
	 fsntfs_test_file_name_values_get_parent_file_reference,
	 file_name_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_creation_time",
	 fsntfs_test_file_name_values_get_creation_time,
	 file_name_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_modification_time",
	 fsntfs_test_file_name_values_get_modification_time,
	 file_name_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_access_time",
	 fsntfs_test_file_name_values_get_access_time,
	 file_name_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_entry_modification_time",
	 fsntfs_test_file_name_values_get_entry_modification_time,
	 file_name_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_file_attribute_flags",
	 fsntfs_test_file_name_values_get_file_attribute_flags,
	 file_name_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_utf8_name_size",
	 fsntfs_test_file_name_values_get_utf8_name_size,
	 file_name_values )

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_utf8_name",
	 fsntfs_test_file_name_values_get_utf8_name,
	 file_name_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_utf16_name_size",
	 fsntfs_test_file_name_values_get_utf16_name_size,
	 file_name_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_file_name_values_get_utf16_name",
	 fsntfs_test_file_name_values_get_utf16_name,
	 file_name_values );

	/* Clean up
	 */
	result = libfsntfs_file_name_values_free(
	          &file_name_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "file_name_values",
	 file_name_values );

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
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

