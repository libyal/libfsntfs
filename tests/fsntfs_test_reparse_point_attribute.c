/*
 * Library reparse_point_attributes functions test program
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

#include "../libfsntfs/libfsntfs_attribute.h"
#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_mft_attribute.h"

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )
#include "../libfsntfs/libfsntfs_reparse_point_attribute.h"
#endif

uint8_t fsntfs_test_reparse_point_attribute_data1[ 88 ] = {
	0xc0, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x3c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xa0, 0x34, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x00, 0x1a, 0x00, 0x10, 0x00, 0x5c, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x5c, 0x00,
	0x43, 0x00, 0x3a, 0x00, 0x5c, 0x00, 0x55, 0x00, 0x73, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00,
	0x00, 0x00, 0x43, 0x00, 0x3a, 0x00, 0x5c, 0x00, 0x55, 0x00, 0x73, 0x00, 0x65, 0x00, 0x72, 0x00,
	0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_reparse_point_attribute_get_tag function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_tag(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	uint32_t tag             = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_tag(
	          attribute,
	          &tag,
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
	result = libfsntfs_reparse_point_attribute_get_tag(
	          NULL,
	          &tag,
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

	result = libfsntfs_reparse_point_attribute_get_tag(
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_utf8_substitute_name_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error             = NULL;
	size_t utf8_substitute_name_size     = 0;
	int result                           = 0;
	int utf8_substitute_name_size_is_set = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
	          attribute,
	          &utf8_substitute_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_substitute_name_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
	          NULL,
	          &utf8_substitute_name_size,
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

	if( utf8_substitute_name_size_is_set != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_reparse_point_attribute_get_utf8_substitute_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_utf8_substitute_name(
     libfsntfs_attribute_t *attribute )
{
	uint8_t utf8_substitute_name[ 512 ];

	libcerror_error_t *error        = NULL;
	int result                      = 0;
	int utf8_substitute_name_is_set = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
	          attribute,
	          utf8_substitute_name,
	          512,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_substitute_name_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
	          NULL,
	          utf8_substitute_name,
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

	if( utf8_substitute_name_is_set != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
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

		result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
		          attribute,
		          utf8_substitute_name,
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

		result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
		          attribute,
		          utf8_substitute_name,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_utf16_substitute_name_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error              = NULL;
	size_t utf16_substitute_name_size     = 0;
	int result                            = 0;
	int utf16_substitute_name_size_is_set = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size(
	          attribute,
	          &utf16_substitute_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_substitute_name_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size(
	          NULL,
	          &utf16_substitute_name_size,
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

	if( utf16_substitute_name_size_is_set != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size(
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_reparse_point_attribute_get_utf16_substitute_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_utf16_substitute_name(
     libfsntfs_attribute_t *attribute )
{
	uint16_t utf16_substitute_name[ 512 ];

	libcerror_error_t *error         = NULL;
	int result                       = 0;
	int utf16_substitute_name_is_set = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
	          attribute,
	          utf16_substitute_name,
	          512,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_substitute_name_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
	          NULL,
	          utf16_substitute_name,
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

	if( utf16_substitute_name_is_set != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
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

		result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
		          attribute,
		          utf16_substitute_name,
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

		result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
		          attribute,
		          utf16_substitute_name,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_reparse_point_attribute_get_utf8_print_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_utf8_print_name_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error        = NULL;
	size_t utf8_print_name_size     = 0;
	int result                      = 0;
	int utf8_print_name_size_is_set = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf8_print_name_size(
	          attribute,
	          &utf8_print_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_print_name_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf8_print_name_size(
	          NULL,
	          &utf8_print_name_size,
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

	if( utf8_print_name_size_is_set != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf8_print_name_size(
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_reparse_point_attribute_get_utf8_print_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_utf8_print_name(
     libfsntfs_attribute_t *attribute )
{
	uint8_t utf8_print_name[ 512 ];

	libcerror_error_t *error   = NULL;
	int result                 = 0;
	int utf8_print_name_is_set = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf8_print_name(
	          attribute,
	          utf8_print_name,
	          512,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_print_name_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf8_print_name(
	          NULL,
	          utf8_print_name,
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

	if( utf8_print_name_is_set != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf8_print_name(
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

		result = libfsntfs_reparse_point_attribute_get_utf8_print_name(
		          attribute,
		          utf8_print_name,
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

		result = libfsntfs_reparse_point_attribute_get_utf8_print_name(
		          attribute,
		          utf8_print_name,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_reparse_point_attribute_get_utf16_print_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_utf16_print_name_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error         = NULL;
	size_t utf16_print_name_size     = 0;
	int result                       = 0;
	int utf16_print_name_size_is_set = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf16_print_name_size(
	          attribute,
	          &utf16_print_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_print_name_size_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf16_print_name_size(
	          NULL,
	          &utf16_print_name_size,
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

	if( utf16_print_name_size_is_set != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf16_print_name_size(
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_reparse_point_attribute_get_utf16_print_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_reparse_point_attribute_get_utf16_print_name(
     libfsntfs_attribute_t *attribute )
{
	uint16_t utf16_print_name[ 512 ];

	libcerror_error_t *error    = NULL;
	int result                  = 0;
	int utf16_print_name_is_set = 0;

	/* Test regular cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf16_print_name(
	          attribute,
	          utf16_print_name,
	          512,
	          &error );

	FSNTFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_print_name_is_set = result;

	/* Test error cases
	 */
	result = libfsntfs_reparse_point_attribute_get_utf16_print_name(
	          NULL,
	          utf16_print_name,
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

	if( utf16_print_name_is_set != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf16_print_name(
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

		result = libfsntfs_reparse_point_attribute_get_utf16_print_name(
		          attribute,
		          utf16_print_name,
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

		result = libfsntfs_reparse_point_attribute_get_utf16_print_name(
		          attribute,
		          utf16_print_name,
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
	libfsntfs_attribute_t *attribute         = NULL;
	libfsntfs_io_handle_t *io_handle         = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	int result                               = 0;
#endif

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize attribute for tests
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
	          fsntfs_test_reparse_point_attribute_data1,
	          88,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_initialize(
	          &attribute,
	          mft_attribute,
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

	result = libfsntfs_internal_attribute_read_value(
	          (libfsntfs_internal_attribute_t *) attribute,
	          io_handle,
	          NULL,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_tag",
	 fsntfs_test_reparse_point_attribute_get_tag,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size",
	 fsntfs_test_reparse_point_attribute_get_utf8_substitute_name_size,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_utf8_substitute_name",
	 fsntfs_test_reparse_point_attribute_get_utf8_substitute_name,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size",
	 fsntfs_test_reparse_point_attribute_get_utf16_substitute_name_size,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_utf16_substitute_name",
	 fsntfs_test_reparse_point_attribute_get_utf16_substitute_name,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_utf8_print_name_size",
	 fsntfs_test_reparse_point_attribute_get_utf8_print_name_size,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_utf8_print_name",
	 fsntfs_test_reparse_point_attribute_get_utf8_print_name,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_utf16_print_name_size",
	 fsntfs_test_reparse_point_attribute_get_utf16_print_name_size,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_reparse_point_attribute_get_utf16_print_name",
	 fsntfs_test_reparse_point_attribute_get_utf16_print_name,
	 attribute );

	/* Clean up
	 */
	result = libfsntfs_internal_attribute_free(
	          (libfsntfs_internal_attribute_t **) &attribute,
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

	result = libfsntfs_mft_attribute_free(
	          &mft_attribute,
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
	if( attribute != NULL )
	{
		libfsntfs_internal_attribute_free(
	         (libfsntfs_internal_attribute_t **) &attribute,
	         &error );
	}
	if( mft_attribute != NULL )
	{
		libfsntfs_mft_attribute_free(
	         &mft_attribute,
	         &error );
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

