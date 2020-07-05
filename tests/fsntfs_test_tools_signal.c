/*
 * Tools signal functions test program
 *
 * Copyright (C) 2010-2020, Joachim Metz <joachim.metz@gmail.com>
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
#include "fsntfs_test_macros.h"
#include "fsntfs_test_unused.h"

#include "../fsntfstools/fsntfstools_signal.h"

void fsntfs_test_tools_signal_handler(
      fsntfstools_signal_t signal FSNTFS_TEST_ATTRIBUTE_UNUSED )
{
	FSNTFS_TEST_UNREFERENCED_PARAMETER( signal )
}

/* Tests the fsntfstools_signal_attach and function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_tools_signal_attach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = fsntfstools_signal_attach(
	          fsntfs_test_tools_signal_handler,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	return( 0 );
}

/* Tests the fsntfstools_signal_detach and function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_tools_signal_detach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = fsntfstools_signal_detach(
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	return( 0 );
}

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

#if defined( WINAPI )

	/* TODO add tests for fsntfstools_signal_handler */
#endif

#if defined( WINAPI ) && defined( _MSC_VER )

	/* TODO add tests for fsntfstools_signal_initialize_memory_debug */
#endif

	FSNTFS_TEST_RUN(
	 "fsntfstools_signal_attach",
	 fsntfs_test_tools_signal_attach )

	FSNTFS_TEST_RUN(
	 "fsntfstools_signal_detach",
	 fsntfs_test_tools_signal_detach )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

