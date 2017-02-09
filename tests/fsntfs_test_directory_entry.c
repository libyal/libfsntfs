/*
 * Library directory_entry type test program
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

#include "fsntfs_test_libcdata.h"
#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_directory_entry.h"

#if defined( __GNUC__ )

/* Tests the libfsntfs_directory_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entry_initialize(
     void )
{
	libcerror_error_t *error                     = NULL;
	libfsntfs_directory_entry_t *directory_entry = NULL;
	int result                                   = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests              = 1;
	int number_of_memset_fail_tests              = 1;
	int test_number                              = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "directory_entry",
         directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsntfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "directory_entry",
         directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsntfs_directory_entry_initialize(
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

	directory_entry = (libfsntfs_directory_entry_t *) 0x12345678UL;

	result = libfsntfs_directory_entry_initialize(
	          &directory_entry,
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

	directory_entry = NULL;

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_directory_entry_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_directory_entry_initialize(
		          &directory_entry,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( directory_entry != NULL )
			{
				libfsntfs_directory_entry_free(
				 &directory_entry,
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
			 "directory_entry",
			 directory_entry );

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
		/* Test libfsntfs_directory_entry_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_directory_entry_initialize(
		          &directory_entry,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( directory_entry != NULL )
			{
				libfsntfs_directory_entry_free(
				 &directory_entry,
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
			 "directory_entry",
			 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_directory_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_directory_entry_free(
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

/* Tests the libfsntfs_directory_entry_clone function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entry_clone(
     void )
{
	libcerror_error_t *error                                 = NULL;
	libfsntfs_directory_entry_t *destination_directory_entry = NULL;
	libfsntfs_directory_entry_t *source_directory_entry      = NULL;
	int result                                               = 0;

	/* Initialize test
	 */
	result = libfsntfs_directory_entry_initialize(
	          &source_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "source_directory_entry",
         source_directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test regular cases
	 */
	result = libfsntfs_directory_entry_clone(
	          &destination_directory_entry,
	          source_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "destination_directory_entry",
         destination_directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsntfs_directory_entry_free(
	          &destination_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "destination_directory_entry",
         destination_directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsntfs_directory_entry_clone(
	          &destination_directory_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "destination_directory_entry",
         destination_directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsntfs_directory_entry_clone(
	          NULL,
	          source_directory_entry,
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
	result = libfsntfs_directory_entry_free(
	          &source_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "source_directory_entry",
         source_directory_entry );

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
	if( destination_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &destination_directory_entry,
		 NULL );
	}
	if( source_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &source_directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_directory_entry_compare function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entry_compare(
     void )
{
	libcerror_error_t *error                            = NULL;
	libfsntfs_directory_entry_t *first_directory_entry  = NULL;
	libfsntfs_directory_entry_t *second_directory_entry = NULL;
	int result                                          = 0;

	/* Initialize test
	 */
	result = libfsntfs_directory_entry_initialize(
	          &first_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "first_directory_entry",
         first_directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsntfs_directory_entry_initialize(
	          &second_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NOT_NULL(
         "second_directory_entry",
         second_directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test regular cases
	 */
	result = libfsntfs_directory_entry_compare(
	          first_directory_entry,
	          second_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBCDATA_COMPARE_EQUAL );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsntfs_directory_entry_compare(
	          NULL,
	          second_directory_entry,
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

	result = libfsntfs_directory_entry_compare(
	          first_directory_entry,
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
	result = libfsntfs_directory_entry_free(
	          &second_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "second_directory_entry",
         second_directory_entry );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsntfs_directory_entry_free(
	          &first_directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSNTFS_TEST_ASSERT_IS_NULL(
         "first_directory_entry",
         first_directory_entry );

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
	if( second_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &second_directory_entry,
		 NULL );
	}
	if( first_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &first_directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_directory_entry_get_mft_entry_index function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_directory_entry_get_mft_entry_index(
     void )
{
	libcerror_error_t *error                     = NULL;
	libfsntfs_directory_entry_t *directory_entry = NULL;
	uint64_t mft_entry_index                     = 0;
	int mft_entry_index_is_set                   = 0;
	int result                                   = 0;

	/* Initialize test
	 */
	result = libfsntfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_directory_entry_get_mft_entry_index(
	          directory_entry,
	          &mft_entry_index,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	mft_entry_index_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_directory_entry_get_mft_entry_index(
	          NULL,
	          &mft_entry_index,
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

	if( mft_entry_index_is_set != 0 )
	{
		result = libfsntfs_directory_entry_get_mft_entry_index(
		          directory_entry,
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
	result = libfsntfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
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
	 "libfsntfs_directory_entry_initialize",
	 fsntfs_test_directory_entry_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entry_free",
	 fsntfs_test_directory_entry_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entry_clone",
	 fsntfs_test_directory_entry_clone );

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entry_compare",
	 fsntfs_test_directory_entry_compare );

	FSNTFS_TEST_RUN(
	 "libfsntfs_directory_entry_get_mft_entry_index",
	 fsntfs_test_directory_entry_get_mft_entry_index );

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

