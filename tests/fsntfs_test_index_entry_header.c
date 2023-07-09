/*
 * Library index_entry_header type test program
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

#include "../libfsntfs/libfsntfs_index_entry_header.h"

uint8_t fsntfs_test_index_entry_header_data1[ 24 ] = {
	0x49, 0x4e, 0x44, 0x58, 0x28, 0x00, 0x09, 0x00, 0xc1, 0xa9, 0x1b, 0x19, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_index_entry_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_index_entry_header_initialize(
     void )
{
	libcerror_error_t *error                           = NULL;
	libfsntfs_index_entry_header_t *index_entry_header = NULL;
	int result                                         = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                    = 1;
	int number_of_memset_fail_tests                    = 1;
	int test_number                                    = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_index_entry_header_initialize(
	          &index_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "index_entry_header",
	 index_entry_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_index_entry_header_free(
	          &index_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "index_entry_header",
	 index_entry_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_index_entry_header_initialize(
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

	index_entry_header = (libfsntfs_index_entry_header_t *) 0x12345678UL;

	result = libfsntfs_index_entry_header_initialize(
	          &index_entry_header,
	          &error );

	index_entry_header = NULL;

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
		/* Test libfsntfs_index_entry_header_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_index_entry_header_initialize(
		          &index_entry_header,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( index_entry_header != NULL )
			{
				libfsntfs_index_entry_header_free(
				 &index_entry_header,
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
			 "index_entry_header",
			 index_entry_header );

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
		/* Test libfsntfs_index_entry_header_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_index_entry_header_initialize(
		          &index_entry_header,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( index_entry_header != NULL )
			{
				libfsntfs_index_entry_header_free(
				 &index_entry_header,
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
			 "index_entry_header",
			 index_entry_header );

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
	if( index_entry_header != NULL )
	{
		libfsntfs_index_entry_header_free(
		 &index_entry_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_index_entry_header_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_index_entry_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_index_entry_header_free(
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

/* Tests the libfsntfs_index_entry_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_index_entry_header_read_data(
     void )
{
	libcerror_error_t *error                           = NULL;
	libfsntfs_index_entry_header_t *index_entry_header = NULL;
	int result                                         = 0;

	/* Initialize test
	 */
	result = libfsntfs_index_entry_header_initialize(
	          &index_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "index_entry_header",
	 index_entry_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_index_entry_header_read_data(
	          index_entry_header,
	          fsntfs_test_index_entry_header_data1,
	          24,
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
	result = libfsntfs_index_entry_header_read_data(
	          NULL,
	          fsntfs_test_index_entry_header_data1,
	          24,
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

	result = libfsntfs_index_entry_header_read_data(
	          index_entry_header,
	          NULL,
	          24,
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

	result = libfsntfs_index_entry_header_read_data(
	          index_entry_header,
	          fsntfs_test_index_entry_header_data1,
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

	result = libfsntfs_index_entry_header_read_data(
	          index_entry_header,
	          fsntfs_test_index_entry_header_data1,
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
	result = libfsntfs_index_entry_header_free(
	          &index_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "index_entry_header",
	 index_entry_header );

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
	if( index_entry_header != NULL )
	{
		libfsntfs_index_entry_header_free(
		 &index_entry_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_index_entry_header_get_fixup_values_offset function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_index_entry_header_get_fixup_values_offset(
     libfsntfs_index_entry_header_t *index_entry_header )
{
	libcerror_error_t *error     = NULL;
	uint16_t fixup_values_offset = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libfsntfs_index_entry_header_get_fixup_values_offset(
	          index_entry_header,
	          &fixup_values_offset,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "fixup_values_offset",
	 fixup_values_offset,
	 40 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_index_entry_header_get_fixup_values_offset(
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

	result = libfsntfs_index_entry_header_get_fixup_values_offset(
	          index_entry_header,
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

/* Tests the libfsntfs_index_entry_header_get_number_of_fixup_values function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_index_entry_header_get_number_of_fixup_values(
     libfsntfs_index_entry_header_t *index_entry_header )
{
	libcerror_error_t *error        = NULL;
	uint16_t number_of_fixup_values = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libfsntfs_index_entry_header_get_number_of_fixup_values(
	          index_entry_header,
	          &number_of_fixup_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT16(
	 "number_of_fixup_values",
	 number_of_fixup_values,
	 9 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_index_entry_header_get_number_of_fixup_values(
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

	result = libfsntfs_index_entry_header_get_number_of_fixup_values(
	          index_entry_header,
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

	libcerror_error_t *error                           = NULL;
	libfsntfs_index_entry_header_t *index_entry_header = NULL;
	int result                                         = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_index_entry_header_initialize",
	 fsntfs_test_index_entry_header_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_index_entry_header_free",
	 fsntfs_test_index_entry_header_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_index_entry_header_read_data",
	 fsntfs_test_index_entry_header_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libfsntfs_index_entry_header_initialize(
	          &index_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "index_entry_header",
	 index_entry_header );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_index_entry_header_read_data(
	          index_entry_header,
	          fsntfs_test_index_entry_header_data1,
	          24,
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
	 "libfsntfs_index_entry_header_get_fixup_values_offset",
	 fsntfs_test_index_entry_header_get_fixup_values_offset,
	 index_entry_header );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_index_entry_header_get_number_of_fixup_values",
	 fsntfs_test_index_entry_header_get_number_of_fixup_values,
	 index_entry_header );

	/* Clean up
	 */
	result = libfsntfs_index_entry_header_free(
	          &index_entry_header,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "index_entry_header",
	 index_entry_header );

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
	if( index_entry_header != NULL )
	{
		libfsntfs_index_entry_header_free(
		 &index_entry_header,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

