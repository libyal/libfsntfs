/*
 * Library attribute type test program
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
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

#include "../libfsntfs/libfsntfs_attribute.h"

#if defined( __GNUC__ )

/* Tests the libfsntfs_attribute_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_initialize(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	int result                       = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 1;
	int number_of_memset_fail_tests  = 1;
	int test_number                  = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "attribute",
         attribute );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "attribute",
         attribute );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_initialize(
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

	attribute = (libfsntfs_attribute_t *) 0x12345678UL;

	result = libfsntfs_attribute_initialize(
	          &attribute,
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

	attribute = NULL;

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_attribute_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_attribute_initialize(
		          &attribute,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( attribute != NULL )
			{
				libfsntfs_attribute_free(
				 &attribute,
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
			 "attribute",
			 attribute );

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
		/* Test libfsntfs_attribute_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_attribute_initialize(
		          &attribute,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( attribute != NULL )
			{
				libfsntfs_attribute_free(
				 &attribute,
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
			 "attribute",
			 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) */

/* Tests the libfsntfs_attribute_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_attribute_free(
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

#if defined( __GNUC__ )

/* Tests the libfsntfs_attribute_get_type function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_type(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	uint32_t type                    = 0;
	int result                       = 0;
	int type_is_set                  = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_type(
	          attribute,
	          &type,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	type_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_type(
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

	if( type_is_set != 0 )
	{
		result = libfsntfs_attribute_get_type(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_data_flags function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_data_flags(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	uint16_t data_flags              = 0;
	int data_flags_is_set            = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_data_flags(
	          attribute,
	          &data_flags,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_flags_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_data_flags(
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

	if( data_flags_is_set != 0 )
	{
		result = libfsntfs_attribute_get_data_flags(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_value function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_value(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	intptr_t *value                  = 0;
	int result                       = 0;
	int value_is_set                 = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_value(
	          attribute,
	          &value,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	value_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_value(
	          NULL,
	          &value,
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

	if( value_is_set != 0 )
	{
		result = libfsntfs_attribute_get_value(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_utf8_name_size(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	size_t utf8_name_size            = 0;
	int result                       = 0;
	int utf8_name_size_is_set        = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_utf8_name_size(
	          attribute,
	          &utf8_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_name_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_utf8_name_size(
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

	if( utf8_name_size_is_set != 0 )
	{
		result = libfsntfs_attribute_get_utf8_name_size(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_utf8_name(
     void )
{
	uint8_t utf8_name[ 512 ];

	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	int result                       = 0;
	int utf8_name_is_set             = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_utf8_name(
	          attribute,
	          utf8_name,
	          512,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_name_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_utf8_name(
	          NULL,
	          utf8_name,
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

	if( utf8_name_is_set != 0 )
	{
		result = libfsntfs_attribute_get_utf8_name(
		          attribute,
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

		result = libfsntfs_attribute_get_utf8_name(
		          attribute,
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

		result = libfsntfs_attribute_get_utf8_name(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_utf16_name_size(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	size_t utf16_name_size           = 0;
	int result                       = 0;
	int utf16_name_size_is_set       = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_utf16_name_size(
	          attribute,
	          &utf16_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_name_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_utf16_name_size(
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

	if( utf16_name_size_is_set != 0 )
	{
		result = libfsntfs_attribute_get_utf16_name_size(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_utf16_name(
     void )
{
	uint16_t utf16_name[ 512 ];

	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	int result                       = 0;
	int utf16_name_is_set            = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_utf16_name(
	          attribute,
	          utf16_name,
	          512,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_name_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_utf16_name(
	          NULL,
	          utf16_name,
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

	if( utf16_name_is_set != 0 )
	{
		result = libfsntfs_attribute_get_utf16_name(
		          attribute,
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

		result = libfsntfs_attribute_get_utf16_name(
		          attribute,
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

		result = libfsntfs_attribute_get_utf16_name(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_compression_unit_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_compression_unit_size(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	size_t compression_unit_size     = 0;
	int compression_unit_size_is_set = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_compression_unit_size(
	          attribute,
	          &compression_unit_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	compression_unit_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_compression_unit_size(
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

	if( compression_unit_size_is_set != 0 )
	{
		result = libfsntfs_attribute_get_compression_unit_size(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_data_size(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	size64_t data_size               = 0;
	int data_size_is_set             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_data_size(
	          attribute,
	          &data_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_data_size(
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

	if( data_size_is_set != 0 )
	{
		result = libfsntfs_attribute_get_data_size(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_valid_data_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_valid_data_size(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	size64_t valid_data_size         = 0;
	int result                       = 0;
	int valid_data_size_is_set       = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_valid_data_size(
	          attribute,
	          &valid_data_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	valid_data_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_valid_data_size(
	          NULL,
	          &valid_data_size,
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

	if( valid_data_size_is_set != 0 )
	{
		result = libfsntfs_attribute_get_valid_data_size(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_number_of_data_runs function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_number_of_data_runs(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	int number_of_data_runs          = 0;
	int number_of_data_runs_is_set   = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_number_of_data_runs(
	          attribute,
	          &number_of_data_runs,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_data_runs_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_attribute_get_number_of_data_runs(
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

	if( number_of_data_runs_is_set != 0 )
	{
		result = libfsntfs_attribute_get_number_of_data_runs(
		          attribute,
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
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_chained_attribute function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_chained_attribute(
     void )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_attribute_t *attribute         = NULL;
	libfsntfs_attribute_t *chained_attribute = 0;
	int chained_attribute_is_set             = 0;
	int result                               = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_chained_attribute(
	          attribute,
	          &chained_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	chained_attribute_is_set = result;

	if( chained_attribute_is_set != 0 )
	{
		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "chained_attribute",
		 chained_attribute );

		result = libfsntfs_attribute_free(
		          &chained_attribute,
		          &error );

		FSNTFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSNTFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libfsntfs_attribute_get_chained_attribute(
	          NULL,
	          &chained_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "chained_attribute",
	 chained_attribute );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( chained_attribute_is_set != 0 )
	{
		result = libfsntfs_attribute_get_chained_attribute(
		          attribute,
		          NULL,
		          &error );

		FSNTFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSNTFS_TEST_ASSERT_IS_NULL(
		 "chained_attribute",
		 chained_attribute );

		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Clean up
	 */
	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( chained_attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &chained_attribute,
		 NULL );
	}
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) */

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

#if defined( __GNUC__ )

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_initialize",
	 fsntfs_test_attribute_initialize );

#endif /* defined( __GNUC__ ) */

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_free",
	 fsntfs_test_attribute_free );

#if defined( __GNUC__ ) && defined( TODO )

	/* TODO: add tests for libfsntfs_attribute_compare_by_file_reference */

	/* TODO: add tests for libfsntfs_attribute_read_from_mft */

	/* TODO: add tests for libfsntfs_attribute_read_from_list */

	/* TODO: add tests for libfsntfs_attribute_read_value */

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_type",
	 fsntfs_test_attribute_get_type );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_data_flags",
	 fsntfs_test_attribute_get_data_flags );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_value",
	 fsntfs_test_attribute_get_value );

	/* TODO: add tests for libfsntfs_attribute_has_name */

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_utf8_name_size",
	 fsntfs_test_attribute_get_utf8_name_size );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_utf8_name",
	 fsntfs_test_attribute_get_utf8_name );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_utf16_name_size",
	 fsntfs_test_attribute_get_utf16_name_size );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_utf16_name",
	 fsntfs_test_attribute_get_utf16_name );

	/* TODO: add tests for libfsntfs_attribute_compare_name_with_utf8_string */

	/* TODO: add tests for libfsntfs_attribute_compare_name_with_utf16_string */

	/* TODO: add tests for libfsntfs_attribute_get_data_vcn_range */

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_compression_unit_size",
	 fsntfs_test_attribute_get_compression_unit_size );

	/* TODO: add tests for libfsntfs_attribute_get_file_reference */

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_data_size",
	 fsntfs_test_attribute_get_data_size );

	/* TODO: add tests for libfsntfs_attribute_get_data */

	/* TODO: add tests for libfsntfs_attribute_copy_data */

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_valid_data_size",
	 fsntfs_test_attribute_get_valid_data_size );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_number_of_data_runs",
	 fsntfs_test_attribute_get_number_of_data_runs );

	/* TODO: add tests for libfsntfs_attribute_get_data_run_by_index */

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_get_chained_attribute",
	 fsntfs_test_attribute_get_chained_attribute );

	/* TODO: add tests for libfsntfs_attribute_append_to_chain */

#endif /* defined( __GNUC__ ) && defined( TODO ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

