/*
 * Library security_descriptor_index_value type test program
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

#include "../libfsntfs/libfsntfs_security_descriptor_index_value.h"

uint8_t fsntfs_test_security_descriptor_index_value_data1[ 20 ] = {
	0xcb, 0xc6, 0xfe, 0x32, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x78, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_security_descriptor_index_value_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_index_value_initialize(
     void )
{
	libcerror_error_t *error                                                     = NULL;
	libfsntfs_security_descriptor_index_value_t *security_descriptor_index_value = NULL;
	int result                                                                   = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                                              = 1;
	int number_of_memset_fail_tests                                              = 1;
	int test_number                                                              = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_index_value_initialize(
	          &security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_index_value",
	 security_descriptor_index_value );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_security_descriptor_index_value_free(
	          &security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_index_value",
	 security_descriptor_index_value );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_security_descriptor_index_value_initialize(
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

	security_descriptor_index_value = (libfsntfs_security_descriptor_index_value_t *) 0x12345678UL;

	result = libfsntfs_security_descriptor_index_value_initialize(
	          &security_descriptor_index_value,
	          &error );

	security_descriptor_index_value = NULL;

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
		/* Test libfsntfs_security_descriptor_index_value_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_security_descriptor_index_value_initialize(
		          &security_descriptor_index_value,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( security_descriptor_index_value != NULL )
			{
				libfsntfs_security_descriptor_index_value_free(
				 &security_descriptor_index_value,
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
			 "security_descriptor_index_value",
			 security_descriptor_index_value );

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
		/* Test libfsntfs_security_descriptor_index_value_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_security_descriptor_index_value_initialize(
		          &security_descriptor_index_value,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( security_descriptor_index_value != NULL )
			{
				libfsntfs_security_descriptor_index_value_free(
				 &security_descriptor_index_value,
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
			 "security_descriptor_index_value",
			 security_descriptor_index_value );

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
	if( security_descriptor_index_value != NULL )
	{
		libfsntfs_security_descriptor_index_value_free(
		 &security_descriptor_index_value,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_security_descriptor_index_value_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_index_value_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_security_descriptor_index_value_free(
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

/* Tests the libfsntfs_security_descriptor_index_value_compare function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_index_value_compare(
     void )
{
	libcerror_error_t *error                                                            = NULL;
	libfsntfs_security_descriptor_index_value_t *first_security_descriptor_index_value  = NULL;
	libfsntfs_security_descriptor_index_value_t *second_security_descriptor_index_value = NULL;
	int result                                                                          = 0;

	/* Initialize test
	 */
	result = libfsntfs_security_descriptor_index_value_initialize(
	          &first_security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "first_security_descriptor_index_value",
	 first_security_descriptor_index_value );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_security_descriptor_index_value_initialize(
	          &second_security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "second_security_descriptor_index_value",
	 second_security_descriptor_index_value );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	first_security_descriptor_index_value->identifier  = 1;
	second_security_descriptor_index_value->identifier = 1;

	result = libfsntfs_security_descriptor_index_value_compare(
	          first_security_descriptor_index_value,
	          second_security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBCDATA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	first_security_descriptor_index_value->identifier  = 1;
	second_security_descriptor_index_value->identifier = 2;

	result = libfsntfs_security_descriptor_index_value_compare(
	          first_security_descriptor_index_value,
	          second_security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBCDATA_COMPARE_LESS );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	first_security_descriptor_index_value->identifier  = 2;
	second_security_descriptor_index_value->identifier = 1;

	result = libfsntfs_security_descriptor_index_value_compare(
	          first_security_descriptor_index_value,
	          second_security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBCDATA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_security_descriptor_index_value_compare(
	          NULL,
	          second_security_descriptor_index_value,
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

	result = libfsntfs_security_descriptor_index_value_compare(
	          first_security_descriptor_index_value,
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
	result = libfsntfs_security_descriptor_index_value_free(
	          &second_security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "second_security_descriptor_index_value",
	 second_security_descriptor_index_value );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_security_descriptor_index_value_free(
	          &first_security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "first_security_descriptor_index_value",
	 first_security_descriptor_index_value );

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
	if( second_security_descriptor_index_value != NULL )
	{
		libfsntfs_security_descriptor_index_value_free(
		 &second_security_descriptor_index_value,
		 NULL );
	}
	if( first_security_descriptor_index_value != NULL )
	{
		libfsntfs_security_descriptor_index_value_free(
		 &first_security_descriptor_index_value,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_security_descriptor_index_value_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_index_value_read_data(
     void )
{
	libcerror_error_t *error                                         = NULL;
	libfsntfs_security_descriptor_index_value_t *security_descriptor_index_value = NULL;
	int result                                                       = 0;

	/* Initialize test
	 */
	result = libfsntfs_security_descriptor_index_value_initialize(
	          &security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_index_value",
	 security_descriptor_index_value );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_index_value_read_data(
	          security_descriptor_index_value,
	          fsntfs_test_security_descriptor_index_value_data1,
	          20,
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
	result = libfsntfs_security_descriptor_index_value_read_data(
	          NULL,
	          fsntfs_test_security_descriptor_index_value_data1,
	          20,
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

	result = libfsntfs_security_descriptor_index_value_read_data(
	          security_descriptor_index_value,
	          NULL,
	          20,
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

	result = libfsntfs_security_descriptor_index_value_read_data(
	          security_descriptor_index_value,
	          fsntfs_test_security_descriptor_index_value_data1,
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

	result = libfsntfs_security_descriptor_index_value_read_data(
	          security_descriptor_index_value,
	          fsntfs_test_security_descriptor_index_value_data1,
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
	result = libfsntfs_security_descriptor_index_value_free(
	          &security_descriptor_index_value,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_index_value",
	 security_descriptor_index_value );

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
	if( security_descriptor_index_value != NULL )
	{
		libfsntfs_security_descriptor_index_value_free(
		 &security_descriptor_index_value,
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
	 "libfsntfs_security_descriptor_index_value_initialize",
	 fsntfs_test_security_descriptor_index_value_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_index_value_free",
	 fsntfs_test_security_descriptor_index_value_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_index_value_compare",
	 fsntfs_test_security_descriptor_index_value_compare );

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_index_value_read_data",
	 fsntfs_test_security_descriptor_index_value_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

