/*
 * Library mft type test program
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

#include "../libfsntfs/libfsntfs_mft.h"

#if defined( __GNUC__ )

/* Tests the libfsntfs_mft_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_mft_free(
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

	/* TODO: add tests for libfsntfs_mft_initialize */

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_free",
	 fsntfs_test_mft_free );

	/* TODO: add tests for libfsntfs_mft_set_data_runs */

	/* TODO: add tests for libfsntfs_mft_read_mft_entry */

	/* TODO: add tests for libfsntfs_mft_get_utf8_volume_name_size */

	/* TODO: add tests for libfsntfs_mft_get_utf8_volume_name */

	/* TODO: add tests for libfsntfs_mft_get_utf16_volume_name_size */

	/* TODO: add tests for libfsntfs_mft_get_utf16_volume_name */

	/* TODO: add tests for libfsntfs_mft_get_volume_version */

	/* TODO: add tests for libfsntfs_mft_get_number_of_entries */

	/* TODO: add tests for libfsntfs_mft_get_mft_entry_by_index */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

