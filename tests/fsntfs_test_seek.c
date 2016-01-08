/*
 * Library seek testing program
 *
 * Copyright (C) 2010-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libcstring.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_unused.h"

/* Define to make fsntfs_test_seek generate verbose output
#define FSNTFS_TEST_SEEK_VERBOSE
 */

/* Tests libfsntfs_file_entry_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int fsntfs_test_seek_offset(
     libfsntfs_file_entry_t *file_entry,
     off64_t input_offset,
     int input_whence,
     off64_t output_offset )
{
	libcerror_error_t *error  = NULL;
	const char *whence_string = NULL;
	off64_t result_offset     = 0;
	int result                = 0;

	if( file_entry == NULL )
	{
		return( -1 );
	}
	if( input_whence == SEEK_CUR )
	{
		whence_string = "SEEK_CUR";
	}
	else if( input_whence == SEEK_END )
	{
		whence_string = "SEEK_END";
	}
	else if( input_whence == SEEK_SET )
	{
		whence_string = "SEEK_SET";
	}
	else
	{
		whence_string = "UNKNOWN";
	}
	fprintf(
	 stdout,
	 "Testing seek of offset: %" PRIi64 " and whence: %s\t",
	 input_offset,
	 whence_string );

	result_offset = libfsntfs_file_entry_seek_offset(
	                 file_entry,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset == output_offset )
	{
		result = 1;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( error != NULL )
	{
		if( result != 1 )
		{
			libcerror_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Tests seeking in a file
 * Returns 1 if successful, 0 if not or -1 on error
 */
int fsntfs_test_seek(
     libfsntfs_file_entry_t *file_entry,
     size64_t file_size )
{
	int result = 0;

	if( file_entry == NULL )
	{
		return( -1 );
	}
	if( file_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "File size exceeds maximum.\n" );

		return( -1 );
	}
	/* Test: SEEK_SET offset: 0
	 * Expected result: 0
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          0,
	          SEEK_SET,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <file_size>
	 * Expected result: <file_size>
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          (off64_t) file_size,
	          SEEK_SET,
	          (off64_t) file_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <file_size / 5>
	 * Expected result: <file_size / 5>
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          (off64_t) ( file_size / 5 ),
	          SEEK_SET,
	          (off64_t) ( file_size / 5 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <file_size + 987>
	 * Expected result: <file_size + 987>
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          (off64_t) ( file_size + 987 ),
	          SEEK_SET,
	          (off64_t) ( file_size + 987 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: -987
	 * Expected result: -1
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          -987,
	          SEEK_SET,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: 0
	 * Expected result: <file_size + 987>
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          0,
	          SEEK_CUR,
	          (off64_t) ( file_size + 987 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: <-1 * (file_size + 987)>
	 * Expected result: 0
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          -1 * (off64_t) ( file_size + 987 ),
	          SEEK_CUR,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: <file_size / 3>
	 * Expected result: <file_size / 3>
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          (off64_t) ( file_size / 3 ),
	          SEEK_CUR,
	          (off64_t) ( file_size / 3 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	if( file_size == 0 )
	{
		/* Test: SEEK_CUR offset: <-2 * (file_size / 3)>
		 * Expected result: 0
		 */
		result = fsntfs_test_seek_offset(
		          file_entry,
		          -2 * (off64_t) ( file_size / 3 ),
		          SEEK_CUR,
		          0 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset.\n" );

			return( result );
		}
	}
	else
	{
		/* Test: SEEK_CUR offset: <-2 * (file_size / 3)>
		 * Expected result: -1
		 */
		result = fsntfs_test_seek_offset(
		          file_entry,
		          -2 * (off64_t) ( file_size / 3 ),
		          SEEK_CUR,
		          -1 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset.\n" );

			return( result );
		}
	}
	/* Test: SEEK_END offset: 0
	 * Expected result: <file_size>
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          0,
	          SEEK_END,
	          (off64_t) file_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * file_size>
	 * Expected result: 0
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          -1 * (off64_t) file_size,
	          SEEK_END,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * (file_size / 4)>
	 * Expected result: <file_size - (file_size / 4)>
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          -1 * (off64_t) ( file_size / 4 ),
	          SEEK_END,
	          (off64_t) file_size - (off64_t) ( file_size / 4 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: 542
	 * Expected result: <file_size + 542>
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          542,
	          SEEK_END,
	          (off64_t) ( file_size + 542 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * (file_size + 542)>
	 * Expected result: -1
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          -1 * (off64_t) ( file_size + 542 ),
	          SEEK_END,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: UNKNOWN (88) offset: 0
	 * Expected result: -1
	 */
	result = fsntfs_test_seek_offset(
	          file_entry,
	          0,
	          88,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	return( result );
}

/* Tests seeking in a file entry
 * Returns 1 if successful, 0 if not or -1 on error
 */
int fsntfs_test_seek_file_entry(
     libcstring_system_character_t *source,
     libcerror_error_t **error )
{
	libfsntfs_file_entry_t *file_entry = NULL;
	libfsntfs_volume_t *volume         = NULL;
	size64_t file_size                 = 0;
	int result                         = 0;

	if( libfsntfs_volume_initialize(
	     &volume,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create volume.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libfsntfs_volume_open_wide(
	     volume,
	     source,
	     LIBFSNTFS_OPEN_READ,
	     error ) != 1 )
#else
	if( libfsntfs_volume_open(
	     volume,
	     source,
	     LIBFSNTFS_OPEN_READ,
	     error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open volume.\n" );

		goto on_error;
	}
	result = libfsntfs_volume_get_file_entry_by_utf8_path(
	          volume,
	          (uint8_t *) "\\$UpCase",
	          8,
	          &file_entry,
	          error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve file entry.\n" );

		goto on_error;
	}
	if( libfsntfs_file_entry_get_size(
	     file_entry,
	     &file_size,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve file entry size.\n" );

		goto on_error;
	}
	result = fsntfs_test_seek(
	          file_entry,
	          file_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek in file.\n" );

		goto on_error;
	}
	if( libfsntfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file entry.\n" );

		goto on_error;
	}
	if( libfsntfs_volume_close(
	     volume,
	     error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close volume.\n" );

		goto on_error;
	}
	if( libfsntfs_volume_free(
	     &volume,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free volume.\n" );

		goto on_error;
	}
	return( result );

on_error:
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	if( volume != NULL )
	{
		libfsntfs_volume_close(
		 volume,
		 NULL );
		libfsntfs_volume_free(
		 &volume,
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error              = NULL;
	libcstring_system_character_t *source = NULL;
	int result                            = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		return( EXIT_FAILURE );
	}
	source = argv[ 1 ];

#if defined( HAVE_DEBUG_OUTPUT ) && defined( FSNTFS_TEST_SEEK_VERBOSE )
	libfsntfs_notify_set_verbose(
	 1 );
	libfsntfs_notify_set_stream(
	 stderr,
	 NULL );
#endif
	result = fsntfs_test_seek_file_entry(
	          source,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek in file entry.\n" );

		goto on_error;
	}
/* TODO add compressed file seek test */
/* TODO add ADS seek test */
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );
		libcerror_error_free(
		 &error );
	}
	return( EXIT_FAILURE );
}

