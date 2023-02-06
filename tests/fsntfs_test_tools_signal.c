/*
 * Tools signal functions test program
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
#include "fsntfs_test_macros.h"
#include "fsntfs_test_unused.h"

#include "../fsntfstools/fsntfstools_signal.h"

void fsntfs_test_tools_signal_handler_function(
      fsntfstools_signal_t signal FSNTFS_TEST_ATTRIBUTE_UNUSED )
{
	FSNTFS_TEST_UNREFERENCED_PARAMETER( signal )
}

#if defined( WINAPI )

/* Tests the fsntfstools_signal_handler function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_tools_signal_handler(
     void )
{
	BOOL result = 0;

	/* Test regular cases
	 */
	result = fsntfstools_signal_handler(
	          CTRL_C_EVENT );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 (int) TRUE );

	result = fsntfstools_signal_handler(
	          CTRL_LOGOFF_EVENT );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 (int) FALSE );

	return( 1 );

on_error:
	return( 0 );
}

#if defined( _MSC_VER )

	/* TODO add tests for fsntfstools_signal_initialize_memory_debug */

#endif /* defined( _MSC_VER ) */

#endif /* defined( WINAPI ) */

/* Tests the fsntfstools_signal_attach function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_tools_signal_attach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = fsntfstools_signal_attach(
	          fsntfs_test_tools_signal_handler_function,
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
	result = fsntfstools_signal_attach(
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

/* Tests the fsntfstools_signal_detach function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_tools_signal_detach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
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

	FSNTFS_TEST_RUN(
	 "fsntfstools_signal_handler",
	 fsntfs_test_tools_signal_handler )

#if defined( _MSC_VER )

	/* TODO add tests for fsntfstools_signal_initialize_memory_debug */

#endif /* defined( _MSC_VER ) */

#endif /* defined( WINAPI ) */

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

