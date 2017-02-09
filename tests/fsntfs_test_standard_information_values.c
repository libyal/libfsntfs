/*
 * Library standard_information_values type test program
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

#include "../libfsntfs/libfsntfs_standard_information_values.h"

#if defined( __GNUC__ )

/* Tests the libfsntfs_standard_information_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_initialize(
     void )
{
	libcerror_error_t *error                                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	int result                                                           = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                                      = 1;
	int number_of_memset_fail_tests                                      = 1;
	int test_number                                                      = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "standard_information_values",
         standard_information_values );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsntfs_standard_information_values_free(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "standard_information_values",
         standard_information_values );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_initialize(
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

	standard_information_values = (libfsntfs_standard_information_values_t *) 0x12345678UL;

	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
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

	standard_information_values = NULL;

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_standard_information_values_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_standard_information_values_initialize(
		          &standard_information_values,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( standard_information_values != NULL )
			{
				libfsntfs_standard_information_values_free(
				 &standard_information_values,
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
			 "standard_information_values",
			 standard_information_values );

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
		/* Test libfsntfs_standard_information_values_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_standard_information_values_initialize(
		          &standard_information_values,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( standard_information_values != NULL )
			{
				libfsntfs_standard_information_values_free(
				 &standard_information_values,
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
			 "standard_information_values",
			 standard_information_values );

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
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_standard_information_values_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_free(
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

/* Tests the libfsntfs_standard_information_values_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_get_creation_time(
     void )
{
	libcerror_error_t *error                                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	uint64_t creation_time                                               = 0;
	int creation_time_is_set                                             = 0;
	int result                                                           = 0;

	/* Initialize test
	 */
	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "standard_information_values",
	 standard_information_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_standard_information_values_get_creation_time(
	          standard_information_values,
	          &creation_time,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	creation_time_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_get_creation_time(
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

	if( creation_time_is_set != 0 )
	{
		result = libfsntfs_standard_information_values_get_creation_time(
		          standard_information_values,
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
	result = libfsntfs_standard_information_values_free(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "standard_information_values",
	 standard_information_values );

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
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_standard_information_values_get_modification_time function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_get_modification_time(
     void )
{
	libcerror_error_t *error                                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	uint64_t modification_time                                           = 0;
	int modification_time_is_set                                         = 0;
	int result                                                           = 0;

	/* Initialize test
	 */
	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "standard_information_values",
	 standard_information_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_standard_information_values_get_modification_time(
	          standard_information_values,
	          &modification_time,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	modification_time_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_get_modification_time(
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

	if( modification_time_is_set != 0 )
	{
		result = libfsntfs_standard_information_values_get_modification_time(
		          standard_information_values,
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
	result = libfsntfs_standard_information_values_free(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "standard_information_values",
	 standard_information_values );

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
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_standard_information_values_get_access_time function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_get_access_time(
     void )
{
	libcerror_error_t *error                                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	uint64_t access_time                                                 = 0;
	int access_time_is_set                                               = 0;
	int result                                                           = 0;

	/* Initialize test
	 */
	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "standard_information_values",
	 standard_information_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_standard_information_values_get_access_time(
	          standard_information_values,
	          &access_time,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	access_time_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_get_access_time(
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

	if( access_time_is_set != 0 )
	{
		result = libfsntfs_standard_information_values_get_access_time(
		          standard_information_values,
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
	result = libfsntfs_standard_information_values_free(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "standard_information_values",
	 standard_information_values );

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
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_standard_information_values_get_entry_modification_time function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_get_entry_modification_time(
     void )
{
	libcerror_error_t *error                                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	uint64_t entry_modification_time                                     = 0;
	int entry_modification_time_is_set                                   = 0;
	int result                                                           = 0;

	/* Initialize test
	 */
	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "standard_information_values",
	 standard_information_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_standard_information_values_get_entry_modification_time(
	          standard_information_values,
	          &entry_modification_time,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	entry_modification_time_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_get_entry_modification_time(
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

	if( entry_modification_time_is_set != 0 )
	{
		result = libfsntfs_standard_information_values_get_entry_modification_time(
		          standard_information_values,
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
	result = libfsntfs_standard_information_values_free(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "standard_information_values",
	 standard_information_values );

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
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_standard_information_values_get_file_attribute_flags function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_get_file_attribute_flags(
     void )
{
	libcerror_error_t *error                                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	uint32_t file_attribute_flags                                        = 0;
	int file_attribute_flags_is_set                                      = 0;
	int result                                                           = 0;

	/* Initialize test
	 */
	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "standard_information_values",
	 standard_information_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_standard_information_values_get_file_attribute_flags(
	          standard_information_values,
	          &file_attribute_flags,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_attribute_flags_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_get_file_attribute_flags(
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

	if( file_attribute_flags_is_set != 0 )
	{
		result = libfsntfs_standard_information_values_get_file_attribute_flags(
		          standard_information_values,
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
	result = libfsntfs_standard_information_values_free(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "standard_information_values",
	 standard_information_values );

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
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_standard_information_values_get_security_descriptor_identifier function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_get_security_descriptor_identifier(
     void )
{
	libcerror_error_t *error                                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	uint32_t security_descriptor_identifier                              = 0;
	int result                                                           = 0;
	int security_descriptor_identifier_is_set                            = 0;

	/* Initialize test
	 */
	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "standard_information_values",
	 standard_information_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_standard_information_values_get_security_descriptor_identifier(
	          standard_information_values,
	          &security_descriptor_identifier,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	security_descriptor_identifier_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_get_security_descriptor_identifier(
	          NULL,
	          &security_descriptor_identifier,
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

	if( security_descriptor_identifier_is_set != 0 )
	{
		result = libfsntfs_standard_information_values_get_security_descriptor_identifier(
		          standard_information_values,
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
	result = libfsntfs_standard_information_values_free(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "standard_information_values",
	 standard_information_values );

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
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_standard_information_values_get_update_sequence_number function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_standard_information_values_get_update_sequence_number(
     void )
{
	libcerror_error_t *error                                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	uint64_t update_sequence_number                                      = 0;
	int result                                                           = 0;
	int update_sequence_number_is_set                                    = 0;

	/* Initialize test
	 */
	result = libfsntfs_standard_information_values_initialize(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "standard_information_values",
	 standard_information_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_standard_information_values_get_update_sequence_number(
	          standard_information_values,
	          &update_sequence_number,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	update_sequence_number_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_standard_information_values_get_update_sequence_number(
	          NULL,
	          &update_sequence_number,
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

	if( update_sequence_number_is_set != 0 )
	{
		result = libfsntfs_standard_information_values_get_update_sequence_number(
		          standard_information_values,
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
	result = libfsntfs_standard_information_values_free(
	          &standard_information_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "standard_information_values",
	 standard_information_values );

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
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
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
	 "libfsntfs_standard_information_values_initialize",
	 fsntfs_test_standard_information_values_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_standard_information_values_free",
	 fsntfs_test_standard_information_values_free );

	/* TODO: add tests for libfsntfs_standard_information_values_read */

	FSNTFS_TEST_RUN(
	 "libfsntfs_standard_information_values_get_creation_time",
	 fsntfs_test_standard_information_values_get_creation_time );

	FSNTFS_TEST_RUN(
	 "libfsntfs_standard_information_values_get_modification_time",
	 fsntfs_test_standard_information_values_get_modification_time );

	FSNTFS_TEST_RUN(
	 "libfsntfs_standard_information_values_get_access_time",
	 fsntfs_test_standard_information_values_get_access_time );

	FSNTFS_TEST_RUN(
	 "libfsntfs_standard_information_values_get_entry_modification_time",
	 fsntfs_test_standard_information_values_get_entry_modification_time );

	FSNTFS_TEST_RUN(
	 "libfsntfs_standard_information_values_get_file_attribute_flags",
	 fsntfs_test_standard_information_values_get_file_attribute_flags );

	FSNTFS_TEST_RUN(
	 "libfsntfs_standard_information_values_get_security_descriptor_identifier",
	 fsntfs_test_standard_information_values_get_security_descriptor_identifier );

	FSNTFS_TEST_RUN(
	 "libfsntfs_standard_information_values_get_update_sequence_number",
	 fsntfs_test_standard_information_values_get_update_sequence_number );

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

