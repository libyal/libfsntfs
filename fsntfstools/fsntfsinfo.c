/*
 * Shows information obtained from a Windows NT File System (NTFS) volume
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsntfstools_getopt.h"
#include "fsntfstools_libcerror.h"
#include "fsntfstools_libclocale.h"
#include "fsntfstools_libcnotify.h"
#include "fsntfstools_libfsntfs.h"
#include "fsntfstools_output.h"
#include "fsntfstools_signal.h"
#include "fsntfstools_unused.h"
#include "info_handle.h"

enum FSNTFSINFO_MODES
{
	FSNTFSINFO_MODE_FILE_ENTRY,
	FSNTFSINFO_MODE_FILE_SYSTEM_HIERARCHY,
	FSNTFSINFO_MODE_MFT_ENTRY,
	FSNTFSINFO_MODE_USN_CHANGE_JOURNAL,
	FSNTFSINFO_MODE_VOLUME
};

info_handle_t *fsntfsinfo_info_handle = NULL;
int fsntfsinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use fsntfsinfo to determine information about a Windows NT\n"
	                 " File System (NTFS) volume.\n\n" );

	fprintf( stream, "Usage: fsntfsinfo [ -B bodyfile ] [ -E mft_entry_index ] [ -F path ]\n"
	                 "                  [ -o offset ] [ -dhHUvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-B:     output file system hierarchy as a bodyfile\n" );
	fprintf( stream, "\t-d:     calculate a MD5 hash of a file entry to include in the bodyfile\n" );
	fprintf( stream, "\t-E:     show information about a specific MFT entry index\n"
	                 "\t        or \"all\".\n" );
	fprintf( stream, "\t-F:     show information about a specific file entry path.\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-H:     shows the file system hierarchy\n" );
	fprintf( stream, "\t-o:     specify the volume offset\n" );
	fprintf( stream, "\t-U:     shows information from the USN change journal ($UsnJrnl)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for fsntfsinfo
 */
void fsntfsinfo_signal_handler(
      fsntfstools_signal_t signal FSNTFSTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "fsntfsinfo_signal_handler";

	FSNTFSTOOLS_UNREFERENCED_PARAMETER( signal )

	fsntfsinfo_abort = 1;

	if( fsntfsinfo_info_handle != NULL )
	{
		if( info_handle_signal_abort(
		     fsntfsinfo_info_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal info handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error                   = NULL;
	system_character_t *option_bodyfile        = NULL;
	system_character_t *option_file_entry      = NULL;
	system_character_t *option_mft_entry_index = NULL;
	system_character_t *option_volume_offset   = NULL;
	system_character_t *source                 = NULL;
	char *program                              = "fsntfsinfo";
	system_integer_t option                    = 0;
	size_t string_length                       = 0;
	uint64_t mft_entry_index                   = 0;
	uint8_t calculate_md5                      = 0;
	int option_mode                            = FSNTFSINFO_MODE_VOLUME;
	int result                                 = 0;
	int verbose                                = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "fsntfstools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( fsntfstools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	fsntfstools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = fsntfstools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "B:dE:F:hHo:UvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'B':
				option_bodyfile = optarg;

				break;

			case (system_integer_t) 'd':
				calculate_md5 = 1;

				break;

			case (system_integer_t) 'E':
				option_mode            = FSNTFSINFO_MODE_MFT_ENTRY;
				option_mft_entry_index = optarg;

				break;

			case (system_integer_t) 'F':
				option_mode       = FSNTFSINFO_MODE_FILE_ENTRY;
				option_file_entry = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'H':
				option_mode = FSNTFSINFO_MODE_FILE_SYSTEM_HIERARCHY;

				break;

			case (system_integer_t) 'o':
				option_volume_offset = optarg;

				break;

			case (system_integer_t) 'U':
				option_mode = FSNTFSINFO_MODE_USN_CHANGE_JOURNAL;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				fsntfstools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libfsntfs_notify_set_stream(
	 stderr,
	 NULL );
	libfsntfs_notify_set_verbose(
	 verbose );

	if( info_handle_initialize(
	     &fsntfsinfo_info_handle,
	     calculate_md5,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize info handle.\n" );

		goto on_error;
	}
	if( option_bodyfile != NULL )
	{
		if( info_handle_set_bodyfile(
		     fsntfsinfo_info_handle,
		     option_bodyfile,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set bodyfile.\n" );

			goto on_error;
		}
	}
	if( option_volume_offset != NULL )
	{
		if( info_handle_set_volume_offset(
		     fsntfsinfo_info_handle,
		     option_volume_offset,
		     &error ) != 1 )
		{
			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			fprintf(
			 stderr,
			 "Unsupported volume offset defaulting to: %" PRIi64 ".\n",
			 fsntfsinfo_info_handle->volume_offset );
		}
	}
/* TODO check for BDE */
	if( info_handle_open_input(
	     fsntfsinfo_info_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	switch( option_mode )
	{
		case FSNTFSINFO_MODE_FILE_ENTRY:
			if( fsntfsinfo_info_handle->input_volume == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to print file entry information.\n" );

				goto on_error;
			}
			if( info_handle_file_entry_fprint_by_path(
			     fsntfsinfo_info_handle,
			     option_file_entry,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print file entry information.\n" );

				goto on_error;
			}
			break;

		case FSNTFSINFO_MODE_FILE_SYSTEM_HIERARCHY:
			if( info_handle_file_system_hierarchy_fprint(
			     fsntfsinfo_info_handle,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print file system hierarchy.\n" );

				goto on_error;
			}
			break;

		case FSNTFSINFO_MODE_MFT_ENTRY:
			if( option_mft_entry_index == NULL )
			{
				fprintf(
				 stderr,
				 "Mising MFT entry index string.\n" );

				goto on_error;
			}
			string_length = system_string_length(
					 option_mft_entry_index );

			if( ( string_length == 3 )
			 && ( system_string_compare(
			       option_mft_entry_index,
			       _SYSTEM_STRING( "all" ),
			       3 ) == 0 ) )
			{
				if( info_handle_mft_entries_fprint(
				     fsntfsinfo_info_handle,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unable to print MFT entries.\n" );

					goto on_error;
				}
			}
			else if( fsntfstools_system_string_copy_decimal_to_integer_64_bit(
			          option_mft_entry_index,
			          string_length + 1,
			          &mft_entry_index,
			          &error ) == 1 )
			{
				if( mft_entry_index > (uint64_t) INT64_MAX )
				{
					fprintf(
					 stderr,
					 "Invalid MFT entry index value out of bounds." );

					goto on_error;
				}
				result = info_handle_mft_entry_fprint(
				          fsntfsinfo_info_handle,
				          mft_entry_index,
				          &error );

				if( result == -1 )
				{
					fprintf(
					 stderr,
					 "Error printing MFT entry: %" PRIu64 ".\n",
					 mft_entry_index );

					goto on_error;
				}
				else if( result == 0 )
				{
					fprintf(
					 stderr,
					 "Unable to print MFT entry: %" PRIu64 ".\n",
					 mft_entry_index );

					goto on_error;
				}
			}
			else
			{
				fprintf(
				 stderr,
				 "Unable to copy MFT entry index string to 64-bit decimal.\n" );

				goto on_error;
			}
			break;

		case FSNTFSINFO_MODE_USN_CHANGE_JOURNAL:
			if( fsntfsinfo_info_handle->input_volume == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to print USN change journal ($UsnJrnl) information.\n" );

				goto on_error;
			}
			if( info_handle_usn_change_journal_fprint(
			     fsntfsinfo_info_handle,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print USN change journal ($UsnJrnl) information.\n" );

				goto on_error;
			}
			break;

		case FSNTFSINFO_MODE_VOLUME:
		default:
			if( info_handle_volume_fprint(
			     fsntfsinfo_info_handle,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print volume information.\n" );

				goto on_error;
			}
			break;
	}
	if( info_handle_close_input(
	     fsntfsinfo_info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( info_handle_free(
	     &fsntfsinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( fsntfsinfo_info_handle != NULL )
	{
		info_handle_free(
		 &fsntfsinfo_info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

