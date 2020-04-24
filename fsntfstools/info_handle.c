/*
 * Info handle
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "fsntfstools_libbfio.h"
#include "fsntfstools_libcerror.h"
#include "fsntfstools_libclocale.h"
#include "fsntfstools_libcnotify.h"
#include "fsntfstools_libfdatetime.h"
#include "fsntfstools_libfguid.h"
#include "fsntfstools_libfwnt.h"
#include "fsntfstools_libfsntfs.h"
#include "fsntfstools_libfusn.h"
#include "info_handle.h"

#if !defined( LIBFSNTFS_HAVE_BFIO )

extern \
int libfsntfs_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

extern \
int libfsntfs_volume_open_file_io_handle(
     libfsntfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

extern \
int libfsntfs_mft_metadata_file_open_file_io_handle(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

#endif /* !defined( LIBFSNTFS_HAVE_BFIO ) */

#define INFO_HANDLE_NOTIFY_STREAM	stdout

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int fsntfstools_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "fsntfstools_system_string_copy_from_64_bit_in_decimal";
	size_t string_index                = 0;
	system_character_t character_value = 0;
	uint8_t maximum_string_index       = 20;
	int8_t sign                        = 1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	*value_64bit = 0;

	if( string[ string_index ] == (system_character_t) '-' )
	{
		string_index++;
		maximum_string_index++;

		sign = -1;
	}
	else if( string[ string_index ] == (system_character_t) '+' )
	{
		string_index++;
		maximum_string_index++;
	}
	while( string_index < string_size )
	{
		if( string[ string_index ] == 0 )
		{
			break;
		}
		if( string_index > (size_t) maximum_string_index )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
			 "%s: string too large.",
			 function );

			return( -1 );
		}
		*value_64bit *= 10;

		if( ( string[ string_index ] >= (system_character_t) '0' )
		 && ( string[ string_index ] <= (system_character_t) '9' ) )
		{
			character_value = (system_character_t) ( string[ string_index ] - (system_character_t) '0' );
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: %" PRIc_SYSTEM " at index: %d.",
			 function,
			 string[ string_index ],
			 string_index );

			return( -1 );
		}
		*value_64bit += character_value;

		string_index++;
	}
	if( sign == -1 )
	{
		*value_64bit *= (uint64_t) -1;
	}
	return( 1 );
}

/* Prints the file attribute flags to the notify stream
 */
void info_handle_file_attribute_flags_fprint(
      uint32_t file_attribute_flags,
      FILE *notify_stream )
{
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs read-only (FILE_ATTRIBUTE_READ_ONLY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_HIDDEN ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs hidden (FILE_ATTRIBUTE_HIDDEN)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs system (FILE_ATTRIBUTE_SYSTEM)\n" );
	}

	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs directory (FILE_ATTRIBUTE_DIRECTORY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ARCHIVE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tShould be archived (FILE_ATTRIBUTE_ARCHIVE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs device (FILE_ATTRIBUTE_DEVICE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NORMAL ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs normal (FILE_ATTRIBUTE_NORMAL)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_TEMPORARY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs temporary (FILE_ATTRIBUTE_TEMPORARY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SPARSE_FILE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs a sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_REPARSE_POINT ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs a reparse point or symbolic link (FILE_ATTRIBUTE_FLAG_REPARSE_POINT)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_COMPRESSED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs compressed (FILE_ATTRIBUTE_COMPRESSED)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_OFFLINE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs offline (FILE_ATTRIBUTE_OFFLINE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tContent should not be indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ENCRYPTED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n" );
	}

	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_VIRTUAL ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs virtual (FILE_ATTRIBUTE_VIRTUAL)\n" );
	}

	if( ( file_attribute_flags & 0x10000000UL ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tUnknown (0x10000000)\n" );
	}
	if( ( file_attribute_flags & 0x20000000UL ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs index view (0x20000000)\n" );
	}
}

/* Retrieves a string containing the access control entry type
 */
const char *info_handle_get_access_control_entry_type(
             uint8_t entry_type )
{
	switch( entry_type )
	{
		case 0x00:
		case 0x04:
		case 0x05:
		case 0x09:
		case 0x0b:
			return( "Access allowed" );

		case 0x01:
		case 0x06:
		case 0x0a:
		case 0x0c:
			return( "Access denied" );

		case 0x02:
		case 0x07:
		case 0x0d:
		case 0x0f:
			return( "System-audit" );

		case 0x03:
		case 0x08:
		case 0x0e:
		case 0x10:
			return( "System-alarm" );

		case 0x11:
			return( "Mandatory label" );
	}
	return( "UNKNOWN" );
}

/* Retrieves the attribute type description
 */
const char *info_handle_get_attribute_type_description(
             uint32_t attribute_type )
{
	switch( attribute_type )
	{
		case LIBFSNTFS_ATTRIBUTE_TYPE_UNUSED:
			return( "Unused" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			return( "$STANDARD_INFORMATION" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST:
			return( "$ATTRIBUTE_LIST" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
			return( "$FILE_NAME" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			return( "$OBJECT_ID" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
			return( "$SECURITY_DESCRIPTOR" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			return( "$VOLUME_NAME" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			return( "$VOLUME_INFORMATION" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_DATA:
			return( "$DATA" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			return( "$INDEX_ROOT" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			return( "$INDEX_ALLOCATION" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
			return( "$BITMAP" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			return( "$REPARSE_POINT" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_EXTENDED_INFORMATION:
			return( "$EA_INFORMATION" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_EXTENDED:
			return( "$EA" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_PROPERTY_SET:
			return( "$PROPERTY_SET" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
			return( "$LOGGED_UTILITY_STREAM" );
	}
	return( "Unknown" );
}

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		memory_free(
		 *info_handle );

		*info_handle = NULL;

		return( -1 );
	}
	if( libbfio_file_range_initialize(
	     &( ( *info_handle )->input_file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file IO handle.",
		 function );

		goto on_error;
	}
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_file_io_handle != NULL )
		{
			libbfio_handle_free(
			 &( ( *info_handle )->input_file_io_handle ),
			 NULL );
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->bodyfile_stream != NULL )
		{
			if( file_stream_close(
			     ( *info_handle )->bodyfile_stream ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close bodyfile stream.",
				 function );

				result = -1;
			}
			( *info_handle )->bodyfile_stream = NULL;
		}
		if( ( *info_handle )->input_volume != NULL )
		{
			if( libfsntfs_volume_free(
			     &( ( *info_handle )->input_volume ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input volume.",
				 function );

				result = -1;
			}
		}
		if( ( *info_handle )->input_mft_metadata_file != NULL )
		{
			if( libfsntfs_mft_metadata_file_free(
			     &( ( *info_handle )->input_mft_metadata_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input MFT metadata file.",
				 function );

				result = -1;
			}
		}
		if( libbfio_handle_free(
		     &( ( *info_handle )->input_file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	info_handle->abort = 1;

	if( info_handle->input_volume != NULL )
	{
		if( libfsntfs_volume_signal_abort(
		     info_handle->input_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input volume to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the bodyfile
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_bodyfile(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_bodyfile";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - bodyfile stream value already set.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	info_handle->bodyfile_stream = file_stream_open_wide(
	                                filename,
	                                L"wb" );
#else
	info_handle->bodyfile_stream = file_stream_open(
	                                filename,
	                                "wb" );
#endif
	if( info_handle->bodyfile_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open bodyfile stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_volume_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( fsntfstools_system_string_copy_from_64_bit_in_decimal(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	info_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_open_input";
	size_t filename_length = 0;
	int result             = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	filename_length = system_string_length(
	                   filename );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file name.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_set(
	     info_handle->input_file_io_handle,
	     info_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set range.",
		 function );

		goto on_error;
	}
	result = libfsntfs_check_volume_signature_file_io_handle(
	          info_handle->input_file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to check volume signature.",
		 function );

		goto on_error;
	}
	if( result != 0 )
	{
		if( libfsntfs_volume_initialize(
		     &( info_handle->input_volume ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input volume.",
			 function );

			goto on_error;
		}
		if( libfsntfs_volume_open_file_io_handle(
		     info_handle->input_volume,
		     info_handle->input_file_io_handle,
		     LIBFSNTFS_OPEN_READ,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open input volume.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libbfio_file_range_set(
		     info_handle->input_file_io_handle,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set range.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_metadata_file_initialize(
		     &( info_handle->input_mft_metadata_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input MFT metadata file.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_metadata_file_open_file_io_handle(
		     info_handle->input_mft_metadata_file,
		     info_handle->input_file_io_handle,
		     LIBFSNTFS_OPEN_READ,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open input MFT metadata file.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( info_handle->input_mft_metadata_file != NULL )
	{
		libfsntfs_mft_metadata_file_free(
		 &( info_handle->input_mft_metadata_file ),
		 NULL );
	}
	if( info_handle->input_volume != NULL )
	{
		libfsntfs_volume_free(
		 &( info_handle->input_volume ),
		 NULL );
	}
	return( -1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_volume != NULL )
	{
		if( libfsntfs_volume_close(
		     info_handle->input_volume,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input volume.",
			 function );

			return( -1 );
		}
	}
	if( info_handle->input_mft_metadata_file != NULL )
	{
		if( libfsntfs_mft_metadata_file_close(
		     info_handle->input_mft_metadata_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input MFT metadata file.",
			 function );

			return( -1 );
		}
	}
	return( 0 );
}

/* Prints a FILETIME value
 * Returns 1 if successful or -1 on error
 */
int info_handle_filetime_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     uint64_t value_64bit,
     libcerror_error_t **error )
{
	system_character_t date_time_string[ 48 ];

	libfdatetime_filetime_t *filetime = NULL;
	static char *function             = "info_handle_filetime_value_fprint";
	int result                        = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "%s: Not set (0)\n",
		 value_name );
	}
	else
	{
		if( libfdatetime_filetime_initialize(
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create FILETIME.",
			 function );

			goto on_error;
		}
		if( libfdatetime_filetime_copy_from_64bit(
		     filetime,
		     value_64bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy 64-bit value to FILETIME.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) date_time_string,
			  48,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) date_time_string,
			  48,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy FILETIME to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%s: %" PRIs_SYSTEM " UTC\n",
		 value_name,
		 date_time_string );

		if( libfdatetime_filetime_free(
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free FILETIME.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	return( -1 );
}

/* Prints a security descriptor
 * Returns 1 if successful or -1 on error
 */
int info_handle_security_descriptor_fprint(
     info_handle_t *info_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfwnt_access_control_entry_t *access_control_entry = NULL;
	libfwnt_access_control_list_t *access_control_list   = NULL;
	libfwnt_security_descriptor_t *security_descriptor   = NULL;
	libfwnt_security_identifier_t *security_identifier   = NULL;
	system_character_t *value_string                     = NULL;
	static char *function                                = "info_handle_security_descriptor_fprint";
	size_t value_string_size                             = 0;
	uint32_t access_mask                                 = 0;
	uint8_t access_control_entry_flags                   = 0;
	uint8_t access_control_entry_type                    = 0;
	int entry_index                                      = 0;
	int number_of_entries                                = 0;
	int result                                           = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfwnt_security_descriptor_initialize(
	     &security_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create security descriptor.",
		 function );

		goto on_error;
	}
	if( libfwnt_security_descriptor_copy_from_byte_stream(
	     security_descriptor,
	     data,
	     data_size,
	     LIBFWNT_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy security descriptor from byte stream.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tSecurity descriptor:\n" );

	result = libfwnt_security_descriptor_get_owner(
		  security_descriptor,
		  &security_identifier,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve owner SID.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfwnt_security_identifier_get_string_size(
		     security_identifier,
		     &value_string_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve owner SID string size.",
			 function );

			goto on_error;
		}
		if( value_string_size > 0 )
		{
			value_string = system_string_allocate(
					value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create owner SID string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfwnt_security_identifier_copy_to_utf16_string(
				  security_identifier,
				  (uint16_t *) value_string,
				  value_string_size,
				  0,
				  error );
#else
			result = libfwnt_security_identifier_copy_to_utf8_string(
				  security_identifier,
				  (uint8_t *) value_string,
				  value_string_size,
				  0,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve owner SID string.",
				 function );

				goto on_error;
			}
			if( libfwnt_security_identifier_free(
			     &security_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free owner SID.",
				 function );

				goto on_error;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\t\tOwner SID\t\t: %" PRIs_SYSTEM "\n",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
	}
	result = libfwnt_security_descriptor_get_group(
		  security_descriptor,
		  &security_identifier,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve group SID.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfwnt_security_identifier_get_string_size(
		     security_identifier,
		     &value_string_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve group SID string size.",
			 function );

			goto on_error;
		}
		if( value_string_size > 0 )
		{
			value_string = system_string_allocate(
					value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create group SID string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfwnt_security_identifier_copy_to_utf16_string(
				  security_identifier,
				  (uint16_t *) value_string,
				  value_string_size,
				  0,
				  error );
#else
			result = libfwnt_security_identifier_copy_to_utf8_string(
				  security_identifier,
				  (uint8_t *) value_string,
				  value_string_size,
				  0,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve group SID string.",
				 function );

				goto on_error;
			}
			if( libfwnt_security_identifier_free(
			     &security_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free group SID.",
				 function );

				goto on_error;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\t\tGroup SID\t\t: %" PRIs_SYSTEM "\n",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
	}
	result = libfwnt_security_descriptor_get_discretionary_acl(
		  security_descriptor,
		  &access_control_list,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve discretionary access control list (ACL).",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\tDiscretionary ACL:\n" );

		if( libfwnt_access_control_list_get_number_of_entries(
		     access_control_list,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of discretionary access control entries (ACE).",
			 function );

			goto on_error;
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( libfwnt_access_control_list_get_entry_by_index(
			     access_control_list,
			     entry_index,
			     &access_control_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve discretionary access control entry (ACE): %d.",
				 function,
				 entry_index );

				goto on_error;
			}
/* TODO print ACE */
			if( libfwnt_access_control_entry_free(
			     &access_control_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free discretionary access control entry (ACE): %d.",
				 function,
				 entry_index );

				goto on_error;
			}
		}
		if( libfwnt_access_control_list_free(
		     &access_control_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free discretionary access control list.",
			 function );

			goto on_error;
		}
	}
	result = libfwnt_security_descriptor_get_system_acl(
		  security_descriptor,
		  &access_control_list,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve system access control list (ACL).",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\tSystem ACL:\n" );

		if( libfwnt_access_control_list_get_number_of_entries(
		     access_control_list,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of system access control entries (ACE).",
			 function );

			goto on_error;
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( libfwnt_access_control_list_get_entry_by_index(
			     access_control_list,
			     entry_index,
			     &access_control_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve system access control entry (ACE): %d.",
				 function,
				 entry_index );

				goto on_error;
			}
			if( libfwnt_access_control_entry_get_type(
			     access_control_entry,
			     &access_control_entry_type,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve system access control entry (ACE): %d type.",
				 function,
				 entry_index );


				goto on_error;
			}
			if( libfwnt_access_control_entry_get_flags(
			     access_control_entry,
			     &access_control_entry_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve system access control entry (ACE): %d flags.",
				 function,
				 entry_index );


				goto on_error;
			}
			result = libfwnt_access_control_entry_get_access_mask(
			          access_control_entry,
			          &access_mask,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve access mask.",
				 function );

				goto on_error;
			}
/* TODO ignore ACE without access mask? */
			result = libfwnt_access_control_entry_get_security_identifier(
			          access_control_entry,
			          &security_identifier,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve security identifier.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				if( libfwnt_security_identifier_get_string_size(
				     security_identifier,
				     &value_string_size,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve security identifier string size.",
					 function );

					goto on_error;
				}
				if( value_string_size > 0 )
				{
					value_string = system_string_allocate(
							value_string_size );

					if( value_string == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create security identifier string.",
						 function );

						goto on_error;
					}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libfwnt_security_identifier_copy_to_utf16_string(
						  security_identifier,
						  (uint16_t *) value_string,
						  value_string_size,
						  0,
						  error );
#else
					result = libfwnt_security_identifier_copy_to_utf8_string(
						  security_identifier,
						  (uint8_t *) value_string,
						  value_string_size,
						  0,
						  error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve security identifier string.",
						 function );

						goto on_error;
					}
					if( libfwnt_security_identifier_free(
					     &security_identifier,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free security identifier.",
						 function );

						goto on_error;
					}
					fprintf(
					 info_handle->notify_stream,
					 "\t\t\t%s, flags: 0x%" PRIx8 ", access mask: 0x%04" PRIx32 ", SID: %" PRIs_SYSTEM "\n",
					 info_handle_get_access_control_entry_type(
					  access_control_entry_type ),
					 access_control_entry_flags,
					 access_mask,
					 value_string );

					memory_free(
					 value_string );

					value_string = NULL;
				}
			}
			if( libfwnt_access_control_entry_free(
			     &access_control_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free system access control entry (ACE): %d.",
				 function,
				 entry_index );

				goto on_error;
			}
		}
		if( libfwnt_access_control_list_free(
		     &access_control_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free system access control list.",
			 function );

			goto on_error;
		}
	}
	if( libfwnt_security_descriptor_free(
	     &security_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free security descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( access_control_entry != NULL )
	{
		libfwnt_access_control_entry_free(
		 &access_control_entry,
		 NULL );
	}
	if( access_control_list != NULL )
	{
		libfwnt_access_control_list_free(
		 &access_control_list,
		 NULL );
	}
	if( security_identifier != NULL )
	{
		libfwnt_security_identifier_free(
		 &security_identifier,
		 NULL );
	}
	if( security_descriptor != NULL )
	{
		libfwnt_security_descriptor_free(
		 &security_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Prints attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_attribute_t *attribute,
     int attribute_index,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_attribute_fprint";
	size_t value_string_size         = 0;
	uint32_t attribute_type          = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Attribute: %d\n",
	 attribute_index + 1 );

	if( libfsntfs_attribute_get_type(
	     attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute type.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tType\t\t\t\t: %s (0x%08" PRIx32 ")\n",
	 info_handle_get_attribute_type_description(
	  attribute_type ),
	 attribute_type );

	switch( attribute_type )
	{
		case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
		case LIBFSNTFS_ATTRIBUTE_TYPE_DATA:
		case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			break;

		default:
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_attribute_get_utf16_name_size(
		          attribute,
		          &value_string_size,
		          error );
#else
		result = libfsntfs_attribute_get_utf8_name_size(
		          attribute,
		          &value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string size.",
			 function );

			goto on_error;
		}
		if( value_string_size > 0 )
		{
			value_string = system_string_allocate(
			                value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create name string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_attribute_get_utf16_name(
			          attribute,
			          (uint16_t *) value_string,
			          value_string_size,
			          error );
#else
			result = libfsntfs_attribute_get_utf8_name(
			          attribute,
			          (uint8_t *) value_string,
			          value_string_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name string.",
				 function );

				goto on_error;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\tName\t\t\t\t: %" PRIs_SYSTEM "\n",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
		break;
	}
	switch( attribute_type )
	{
		case LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST:
			if( info_handle_attribute_list_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print attribute list attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
			if( info_handle_bitmap_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print bitmap attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_DATA:
			if( info_handle_data_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print data attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
			if( info_handle_file_name_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file name attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			if( info_handle_object_identifier_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print object identifier attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			if( info_handle_reparse_point_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print reparse point attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
			if( info_handle_security_descriptor_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print security descriptor attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			if( info_handle_standard_information_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print standard information attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			if( info_handle_volume_information_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print volume information attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			if( info_handle_volume_name_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print volume name attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		default:
			break;
	}
	if( ( info_handle->input_mft_metadata_file != NULL )
	 && ( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME ) )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tPath hint\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_file_entry_get_utf16_path_hint_size(
		          file_entry,
		          attribute_index,
		          &value_string_size,
		          error );
#else
		result = libfsntfs_file_entry_get_utf8_path_hint_size(
		          file_entry,
		          attribute_index,
		          &value_string_size,
		          error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve path hint string size.",
			 function );

			goto on_error;
		}
		if( ( result != 0 )
		 && ( value_string_size > 0 ) )
		{
			value_string = system_string_allocate(
			                value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create path hint string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_file_entry_get_utf16_path_hint(
			          file_entry,
			          attribute_index,
			          (uint16_t *) value_string,
			          value_string_size,
			          error );
#else
			result = libfsntfs_file_entry_get_utf8_path_hint(
			          file_entry,
			          attribute_index,
			          (uint8_t *) value_string,
			          value_string_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve path hint string.",
				 function );

				goto on_error;
			}
			fprintf(
			 info_handle->notify_stream,
			 "%" PRIs_SYSTEM "",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints $ATTRIBUTE_LIST attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_attribute_list_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	libfsntfs_attribute_list_entry_t *attribute_list_entry = NULL;
	system_character_t *value_string                       = NULL;
	static char *function                                  = "info_handle_attribute_list_attribute_fprint";
	size64_t data_size                                     = 0;
	size_t value_string_size                               = 0;
	uint64_t data_first_vcn                                = 0;
	uint64_t data_last_vcn                                 = 0;
	uint64_t file_reference                                = 0;
	uint32_t attribute_type                                = 0;
	int entry_index                                        = 0;
	int number_of_entries                                  = 0;
	int result                                             = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	result = libfsntfs_attribute_get_data_vcn_range(
	          attribute,
	          &data_first_vcn,
	          &data_last_vcn,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data VCN range.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( ( data_first_vcn == data_last_vcn )
		 || ( data_last_vcn == 0xffffffffffffffffUL ) )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tData VCN\t\t\t: %" PRIu64 "\n",
			 data_first_vcn );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tData VCN range\t\t\t: %" PRIu64 " - %" PRIu64 "\n",
			 data_first_vcn,
			 data_last_vcn );
		}
	}
	if( data_first_vcn == 0 )
	{
		if( libfsntfs_attribute_get_data_size(
		     attribute,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tData size\t\t\t: %" PRIu64 " bytes\n",
		 data_size );
	}
	if( libfsntfs_attribute_list_attribute_get_number_of_entries(
	     attribute,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attribute list entries.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of entries\t\t: %d\n",
	 number_of_entries );

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfsntfs_attribute_list_attribute_get_entry_by_index(
		     attribute,
		     entry_index,
		     &attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute list entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( libfsntfs_attribute_list_entry_get_attribute_type(
		     attribute_list_entry,
		     &attribute_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute type.",
			 function );

			goto on_error;
		}
		if( libfsntfs_attribute_list_entry_get_file_reference(
		     attribute_list_entry,
		     &file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file reference.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_attribute_list_entry_get_utf16_name_size(
		          attribute_list_entry,
		          &value_string_size,
		          error );
#else
		result = libfsntfs_attribute_list_entry_get_utf8_name_size(
		          attribute_list_entry,
		          &value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string size.",
			 function );

			goto on_error;
		}
		if( value_string_size > 0 )
		{
			value_string = system_string_allocate(
			                value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create name string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_attribute_list_entry_get_utf16_name(
			          attribute_list_entry,
			          (uint16_t *) value_string,
			          value_string_size,
			          error );
#else
			result = libfsntfs_attribute_list_entry_get_utf8_name(
			          attribute_list_entry,
			          (uint8_t *) value_string,
			          value_string_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name string.",
				 function );

				goto on_error;
			}
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tEntry: %d\t\t\t: %s (0x%08" PRIx32 ")",
		 entry_index,
		 info_handle_get_attribute_type_description(
		  attribute_type ),
		 attribute_type );

		if( value_string != NULL )
		{
			fprintf(
			 info_handle->notify_stream,
			 " %" PRIs_SYSTEM "",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
		fprintf(
		 info_handle->notify_stream,
		 " in file reference: %" PRIu64 "-%" PRIu64 "\n",
		 file_reference & 0xffffffffffffUL,
		 file_reference >> 48 );

		if( libfsntfs_attribute_list_entry_free(
		     &attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attribute list entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( attribute_list_entry != NULL )
	{
		libfsntfs_attribute_list_entry_free(
		 &attribute_list_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints $BITMAP attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_bitmap_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_bitmap_attribute_fprint";
	size64_t data_size               = 0;
	size_t value_string_size         = 0;
	uint64_t data_first_vcn          = 0;
	uint64_t data_last_vcn           = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	result = libfsntfs_attribute_get_data_vcn_range(
	          attribute,
	          &data_first_vcn,
	          &data_last_vcn,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data VCN range.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( ( data_first_vcn == data_last_vcn )
		 || ( data_last_vcn == 0xffffffffffffffffUL ) )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tData VCN\t\t\t: %" PRIu64 "\n",
			 data_first_vcn );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tData VCN range\t\t\t: %" PRIu64 " - %" PRIu64 "\n",
			 data_first_vcn,
			 data_last_vcn );
		}
	}
	if( data_first_vcn == 0 )
	{
		if( libfsntfs_attribute_get_data_size(
		     attribute,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tData size\t\t\t: %" PRIu64 " bytes\n",
		 data_size );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_attribute_get_utf16_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_attribute_get_utf8_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name string size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_attribute_get_utf16_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_attribute_get_utf8_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tName\t\t\t\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints $DATA attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_data_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_data_attribute_fprint";
	size64_t data_size               = 0;
	size_t value_string_size         = 0;
	uint64_t data_first_vcn          = 0;
	uint64_t data_last_vcn           = 0;
	uint16_t data_flags              = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	result = libfsntfs_attribute_get_data_vcn_range(
	          attribute,
	          &data_first_vcn,
	          &data_last_vcn,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data VCN range.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( ( data_first_vcn == data_last_vcn )
		 || ( data_last_vcn == 0xffffffffffffffffUL ) )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tData VCN\t\t\t: %" PRIu64 "\n",
			 data_first_vcn );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tData VCN range\t\t\t: %" PRIu64 " - %" PRIu64 "\n",
			 data_first_vcn,
			 data_last_vcn );
		}
	}
	if( data_first_vcn == 0 )
	{
		if( libfsntfs_attribute_get_data_size(
		     attribute,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tData size\t\t\t: %" PRIu64 " bytes\n",
		 data_size );
	}
	if( libfsntfs_attribute_get_data_flags(
	     attribute,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tData flags\t\t\t: 0x%04" PRIx16 "\n",
	 data_flags );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_attribute_get_utf16_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_attribute_get_utf8_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name string size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_attribute_get_utf16_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_attribute_get_utf8_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tName\t\t\t\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints $FILE_NAME attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_file_name_attribute_fprint";
	size_t value_string_size         = 0;
	uint64_t value_64bit             = 0;
	uint32_t value_32bit             = 0;
	uint8_t value_8bit               = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_parent_file_reference(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent file reference.",
		 function );

		goto on_error;
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent file reference\t\t: %" PRIu64 "\n",
		 value_64bit );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent file reference\t\t: %" PRIu64 "-%" PRIu64 "\n",
		 value_64bit & 0xffffffffffffUL,
		 value_64bit >> 48 );
	}
	if( libfsntfs_file_name_attribute_get_creation_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tCreation time\t\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_attribute_get_modification_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tModification time\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_attribute_get_access_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tAccess time\t\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_attribute_get_entry_modification_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tEntry modification time\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_attribute_get_file_attribute_flags(
	     attribute,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile attribute flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_file_attribute_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );

	if( libfsntfs_file_name_attribute_get_name_space(
	     attribute,
	     &value_8bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name space.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tName space\t\t\t: " );

	switch( value_8bit )
	{
		case 0:
			fprintf(
			 info_handle->notify_stream,
			 "POSIX (%" PRIu8 ")",
			 value_8bit );

			break;

		case 1:
			fprintf(
			 info_handle->notify_stream,
			 "Windows (%" PRIu8 ")",
			 value_8bit );

			break;

		case 2:
			fprintf(
			 info_handle->notify_stream,
			 "DOS (%" PRIu8 ")",
			 value_8bit );

			break;

		case 3:
			fprintf(
			 info_handle->notify_stream,
			 "DOS and Windows (%" PRIu8 ")",
			 value_8bit );

			break;

		default:
			fprintf(
			 info_handle->notify_stream,
			 "%" PRIu8 "",
			 value_8bit );

			break;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_file_name_attribute_get_utf16_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_file_name_attribute_get_utf8_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name string size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_file_name_attribute_get_utf16_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_file_name_attribute_get_utf8_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tName\t\t\t\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints $OBJECT_ID attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_object_identifier_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	uint8_t guid_data[ 16 ];

	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	static char *function       = "info_handle_object_identifier_attribute_fprint";
	int result                  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	if( libfsntfs_object_identifier_attribute_get_droid_file_identifier(
	     attribute,
	     guid_data,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve droid file identifier.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     guid_data,
	     16,
	     LIBFGUID_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfguid_identifier_copy_to_utf16_string(
		  guid,
		  (uint16_t *) guid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#else
	result = libfguid_identifier_copy_to_utf8_string(
		  guid,
		  (uint8_t *) guid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tDroid file identifier\t\t: %" PRIs_SYSTEM "\n",
	 guid_string );

	result = libfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier(
	          attribute,
	          guid_data,
	          16,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve birth droid volume identifier.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     guid_data,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tBirth droid volume identifier\t: %" PRIs_SYSTEM "\n",
		 guid_string );
	}
	result = libfsntfs_object_identifier_attribute_get_birth_droid_file_identifier(
	          attribute,
	          guid_data,
	          16,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve birth droid file identifier.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     guid_data,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tBirth droid file identifier\t: %" PRIs_SYSTEM "\n",
		 guid_string );
	}
	result = libfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier(
	          attribute,
	          guid_data,
	          16,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve birth droid domain identifier.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     guid_data,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tBirth droid domain identifier\t: %" PRIs_SYSTEM "\n",
		 guid_string );
	}
	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints $REPARSE_POINT attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_reparse_point_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_reparse_point_attribute_fprint";
	size_t value_string_size         = 0;
	uint32_t value_32bit             = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_reparse_point_attribute_get_tag(
	     attribute,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve tag.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tTag\t\t\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );

	result = libfsntfs_reparse_point_attribute_get_compression_method(
	          attribute,
	          &value_32bit,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compression method.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tCompression method\t\t: " );

		switch( value_32bit )
		{
			case 0:
				fprintf(
				 info_handle->notify_stream,
				 "XPRESS4K (%" PRIu32 ")",
				 value_32bit );

				break;

			case 1:
				fprintf(
				 info_handle->notify_stream,
				 "LZX (%" PRIu32 ")",
				 value_32bit );

				break;

			case 2:
				fprintf(
				 info_handle->notify_stream,
				 "XPRESS8K (%" PRIu32 ")",
				 value_32bit );

				break;

			case 3:
				fprintf(
				 info_handle->notify_stream,
				 "XPRESS16K (%" PRIu32 ")",
				 value_32bit );

				break;

			default:
				fprintf(
				 info_handle->notify_stream,
				 "%" PRIu32 "",
				 value_32bit );

				break;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
/* TODO sanitize substitute name ? */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve substitute name string size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create substitute name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve substitute name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tSubstitute name\t\t\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_reparse_point_attribute_get_utf16_print_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_reparse_point_attribute_get_utf8_print_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve print name string size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create print name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_reparse_point_attribute_get_utf16_print_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_reparse_point_attribute_get_utf8_print_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve print name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tPrint name\t\t\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints $SECURITY_DESCRIPTOR attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_security_descriptor_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	static char *function = "info_handle_security_descriptor_attribute_fprint";
	uint8_t *data         = NULL;
	size_t data_size      = 0;
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	result = libfsntfs_security_descriptor_attribute_get_security_descriptor_size(
	          attribute,
	          &data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor size.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		data = (uint8_t *) memory_allocate(
		                    sizeof( uint8_t ) * data_size );

		if( data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( libfsntfs_security_descriptor_attribute_get_security_descriptor(
		     attribute,
		     data,
		     data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve security descriptor.",
			 function );

			goto on_error;
		}
		if( info_handle_security_descriptor_fprint(
		     info_handle,
		     data,
		     data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print security descriptor.",
			 function );

			goto on_error;
		}
		memory_free(
		 data );

		data = NULL;
	}
	return( 1 );

on_error:
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	return( -1 );
}

/* Prints $STANDARD_INFORMATION attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_standard_information_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	static char *function = "info_handle_standard_information_attribute_fprint";
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_attribute_get_creation_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		return( -1 );
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tCreation time\t\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_attribute_get_modification_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		return( -1 );
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tModification time\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_attribute_get_access_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		return( -1 );
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tAccess time\t\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_attribute_get_entry_modification_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time.",
		 function );

		return( -1 );
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tEntry modification time\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		return( -1 );
	}
	result = libfsntfs_standard_information_attribute_get_owner_identifier(
	          attribute,
	          &value_32bit,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve owner identifier.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tOwner identifier\t\t: %" PRIu32 "\n",
		 value_32bit );
	}
	result = libfsntfs_standard_information_attribute_get_security_descriptor_identifier(
	          attribute,
	          &value_32bit,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor identifier.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tSecurity descriptor identifier\t: %" PRIu32 "\n",
		 value_32bit );
	}
	result = libfsntfs_standard_information_attribute_get_update_sequence_number(
	          attribute,
	          &value_64bit,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update sequence number.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tUpdate sequence number\t\t: %" PRIu64 "\n",
		 value_64bit );
	}
	if( libfsntfs_standard_information_attribute_get_file_attribute_flags(
	     attribute,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile attribute flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_file_attribute_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );

	return( 1 );
}

/* Prints $VOLUME_INFORMATION attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_information_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	static char *function = "info_handle_volume_information_attribute_fprint";
	uint16_t value_16bit  = 0;
	uint8_t major_version = 0;
	uint8_t minor_version = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_volume_information_attribute_get_version(
	     attribute,
	     &major_version,
	     &minor_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve version.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tVersion\t\t\t\t: %" PRIu8 ".%" PRIu8 "\n",
	 major_version,
	 minor_version );

	if( libfsntfs_volume_information_attribute_get_flags(
	     attribute,
	     &value_16bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve flags.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFlags\t\t\t\t: 0x%04" PRIx16 "\n",
	 value_16bit );

	return( 1 );
}

/* Prints $VOLUME_NAME attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_volume_name_attribute_fprint";
	size_t value_string_size         = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_volume_name_attribute_get_utf16_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_volume_name_attribute_get_utf8_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name string size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_volume_name_attribute_get_utf16_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_volume_name_attribute_get_utf8_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tName\t\t\t\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints a file entry value
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_file_entry_value_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	system_character_t *file_entry_name  = NULL;
	uint8_t *security_descriptor_data    = NULL;
	static char *function                = "info_handle_file_entry_value_fprint";
	size64_t size                        = 0;
	size_t file_entry_name_size          = 0;
	size_t security_descriptor_data_size = 0;
	uint64_t access_time                 = 0;
	uint64_t creation_time               = 0;
	uint64_t entry_modification_time     = 0;
	uint64_t file_reference              = 0;
	uint64_t modification_time           = 0;
	uint64_t parent_file_reference       = 0;
	uint32_t file_attribute_flags        = 0;
	int result                           = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_file_reference(
	     file_entry,
	     &file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file reference.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile reference\t\t\t: %" PRIu64 "-%" PRIu64 "\n",
	 file_reference & 0xffffffffffffUL,
	 file_reference >> 48 );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_file_entry_get_utf16_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#else
	result = libfsntfs_file_entry_get_utf8_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( file_entry_name_size > 0 ) )
	{
		file_entry_name = system_string_allocate(
		                   file_entry_name_size );

		if( file_entry_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file entry name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_file_entry_get_utf16_name(
		          file_entry,
		          (uint16_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#else
		result = libfsntfs_file_entry_get_utf8_name(
		          file_entry,
		          (uint8_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tName\t\t\t\t: %" PRIs_SYSTEM "\n",
		 file_entry_name );
	}
	if( libfsntfs_file_entry_get_parent_file_reference(
	     file_entry,
	     &parent_file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent file reference.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tParent file reference\t\t: " );

	if( parent_file_reference == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Not set (0)" );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIu64 "-%" PRIu64 "",
		 parent_file_reference & 0xffffffffffffUL,
		 parent_file_reference >> 48 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfsntfs_file_entry_get_size(
	     file_entry,
	     &size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tSize\t\t\t\t: %" PRIu64 "\n",
	 size );

	if( libfsntfs_file_entry_get_creation_time(
	     file_entry,
	     &creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tCreation time\t\t\t",
	     creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_get_modification_time(
	     file_entry,
	     &modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tModification time\t\t",
	     modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_get_access_time(
	     file_entry,
	     &access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tAccess time\t\t\t",
	     access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_get_entry_modification_time(
	     file_entry,
	     &entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tEntry modification time\t\t",
	     entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_get_file_attribute_flags(
	     file_entry,
	     &file_attribute_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile attribute flags\t\t: 0x%08" PRIx32 "\n",
	 file_attribute_flags );
	info_handle_file_attribute_flags_fprint(
	 file_attribute_flags,
	 info_handle->notify_stream );

#ifdef TODO
	result = libfsntfs_file_entry_get_security_descriptor_size(
	          file_entry,
	          &security_descriptor_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor size.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		security_descriptor_data = (uint8_t *) memory_allocate(
		                                        sizeof( uint8_t ) * security_descriptor_data_size );

		if( security_descriptor_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create security descriptor data.",
			 function );

			goto on_error;
		}
		if( libfsntfs_file_entry_get_security_descriptor(
		     file_entry,
		     security_descriptor_data,
		     security_descriptor_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve security descriptor.",
			 function );

			goto on_error;
		}
		if( info_handle_security_descriptor_fprint(
		     info_handle,
		     security_descriptor_data,
		     security_descriptor_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print security descriptor.",
			 function );

			goto on_error;
		}
		memory_free(
		 security_descriptor_data );

		security_descriptor_data = NULL;
	}
#endif
/* TODO print attributes + ADS ? */

	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );

		file_entry_name = NULL;
	}
	return( 1 );

on_error:
	if( security_descriptor_data != NULL )
	{
		memory_free(
		 security_descriptor_data );
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

/* Prints a $FILE_NAME attribute to a bodyfile
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_bodyfile_file_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_attribute_t *attribute,
     const system_character_t *path,
     const system_character_t *file_entry_name,
     libcerror_error_t **error )
{
	char file_mode_string[ 13 ]      = { '-', '/', '-', 'r', 'w', 'x', 'r', 'w', 'x', 'r', 'w', 'x', 0 };

	static char *function            = "info_handle_bodyfile_file_name_attribute_fprint";
	size64_t size                    = 0;
	uint64_t access_time             = 0;
	uint64_t creation_time           = 0;
	uint64_t entry_modification_time = 0;
	uint64_t file_reference          = 0;
	uint64_t modification_time       = 0;
	uint32_t file_attribute_flags    = 0;
	uint32_t group_identifier        = 0;
	uint32_t owner_identifier        = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_file_reference(
	     file_entry,
	     &file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file reference.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_creation_time(
	     attribute,
	     &creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_modification_time(
	     attribute,
	     &modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_access_time(
	     attribute,
	     &access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_entry_modification_time(
	     attribute,
	     &entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_file_attribute_flags(
	     attribute,
	     &file_attribute_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_data_size(
	     attribute,
	     &size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		return( -1 );
	}
	result = libfsntfs_file_entry_has_directory_entries_index(
		  file_entry,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file entry has directory entries index.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		file_mode_string[ 0 ] = 'd';
		file_mode_string[ 2 ] = 'd';
	}
	else
	{
		file_mode_string[ 0 ] = 'r';
		file_mode_string[ 2 ] = 'r';
	}
	if( ( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	 || ( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 ) )
	{
		file_mode_string[ 4 ]  = '-';
		file_mode_string[ 7 ]  = '-';
		file_mode_string[ 10 ] = '-';
	}
	/* Colums in a Sleuthkit 3.x and later bodyfile
	 * MD5|name|inode|mode_as_string|UID|GID|size|atime|mtime|ctime|crtime
	 */
	fprintf(
	 info_handle->bodyfile_stream,
	 "0|" );

	if( path != NULL )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 "%" PRIs_SYSTEM "",
		 path );
	}
	if( file_entry_name != NULL )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 "%" PRIs_SYSTEM "",
		 file_entry_name );
	}
/* TODO determine Sleuthkit metadata address https://wiki.sleuthkit.org/index.php?title=Metadata_Address */
/* TODO determine $FILE_NAME attribute address */

	fprintf(
	 info_handle->bodyfile_stream,
	 " ($FILE_NAME)|%" PRIu64 "|%s|%" PRIu32 "|%" PRIu32 "|%" PRIu64 "|%.9f|%.9f|%.9f|%.9f\n",
	 file_reference & 0xffffffffffffUL,
	 file_mode_string,
	 owner_identifier,
	 group_identifier,
	 size,
	 (double) ( access_time - 116444736000000000L ) / 10000000,
	 (double) ( modification_time - 116444736000000000L ) / 10000000,
	 (double) ( entry_modification_time - 116444736000000000L ) / 10000000,
	 (double) ( creation_time - 116444736000000000L ) / 10000000 );

	return( 1 );
}

/* Prints a $INDEX_ROOT attribute to a bodyfile
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_bodyfile_index_root_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_attribute_t *attribute,
     const system_character_t *path,
     const system_character_t *file_entry_name,
     const system_character_t *attribute_name,
     size_t attribute_name_size,
     libcerror_error_t **error )
{
	char file_mode_string[ 13 ]      = { '-', '/', '-', 'r', 'w', 'x', 'r', 'w', 'x', 'r', 'w', 'x', 0 };

	static char *function            = "info_handle_bodyfile_index_root_attribute_fprint";
	size64_t size                    = 0;
	uint64_t access_time             = 0;
	uint64_t creation_time           = 0;
	uint64_t entry_modification_time = 0;
	uint64_t file_reference          = 0;
	uint64_t modification_time       = 0;
	uint32_t file_attribute_flags    = 0;
	uint32_t group_identifier        = 0;
	uint32_t owner_identifier        = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_file_reference(
	     file_entry,
	     &file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file reference.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_creation_time(
	     file_entry,
	     &creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_modification_time(
	     file_entry,
	     &modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_access_time(
	     file_entry,
	     &access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_entry_modification_time(
	     file_entry,
	     &entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_file_attribute_flags(
	     file_entry,
	     &file_attribute_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_data_size(
	     attribute,
	     &size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		return( -1 );
	}
	result = libfsntfs_file_entry_has_directory_entries_index(
		  file_entry,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file entry has directory entries index.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		file_mode_string[ 0 ] = 'd';
		file_mode_string[ 2 ] = 'd';
	}
	else
	{
		file_mode_string[ 0 ] = 'r';
		file_mode_string[ 2 ] = 'r';
	}
	if( ( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	 || ( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 ) )
	{
		file_mode_string[ 4 ]  = '-';
		file_mode_string[ 7 ]  = '-';
		file_mode_string[ 10 ] = '-';
	}
	/* Colums in a Sleuthkit 3.x and later bodyfile
	 * MD5|name|inode|mode_as_string|UID|GID|size|atime|mtime|ctime|crtime
	 */
	fprintf(
	 info_handle->bodyfile_stream,
	 "0|" );

	if( path != NULL )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 "%" PRIs_SYSTEM "",
		 path );
	}
	if( file_entry_name != NULL )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 "%" PRIs_SYSTEM "",
		 file_entry_name );
	}
	if( ( attribute_name != NULL )
         && ( ( attribute_name_size != 5 )
	  ||  ( narrow_string_compare(
	       attribute_name,
	       "$I30",
	       4 ) != 0 ) ) )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 ":%" PRIs_SYSTEM "",
		 attribute_name );
	}
/* TODO determine Sleuthkit metadata address https://wiki.sleuthkit.org/index.php?title=Metadata_Address */
/* TODO determine $INDEX_ROOT attribute address */

	fprintf(
	 info_handle->bodyfile_stream,
	 "|%" PRIu64 "|%s|%" PRIu32 "|%" PRIu32 "|%" PRIu64 "|%.9f|%.9f|%.9f|%.9f\n",
	 file_reference & 0xffffffffffffUL,
	 file_mode_string,
	 owner_identifier,
	 group_identifier,
	 size,
	 (double) ( access_time - 116444736000000000L ) / 10000000,
	 (double) ( modification_time - 116444736000000000L ) / 10000000,
	 (double) ( entry_modification_time - 116444736000000000L ) / 10000000,
	 (double) ( creation_time - 116444736000000000L ) / 10000000 );

	return( 1 );
}

/* Prints a file entry value to a bodyfile
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_bodyfile_file_entry_value_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_data_stream_t *alternate_data_stream,
     const system_character_t *path,
     const system_character_t *file_entry_name,
     const system_character_t *data_stream_name,
     libcerror_error_t **error )
{
	char file_mode_string[ 13 ]      = { '-', '/', '-', 'r', 'w', 'x', 'r', 'w', 'x', 'r', 'w', 'x', 0 };

	static char *function            = "info_handle_bodyfile_file_entry_value_fprint";
	size64_t size                    = 0;
	uint64_t access_time             = 0;
	uint64_t creation_time           = 0;
	uint64_t entry_modification_time = 0;
	uint64_t file_reference          = 0;
	uint64_t modification_time       = 0;
	uint32_t file_attribute_flags    = 0;
	uint32_t group_identifier        = 0;
	uint32_t owner_identifier        = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_file_reference(
	     file_entry,
	     &file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file reference.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_creation_time(
	     file_entry,
	     &creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_modification_time(
	     file_entry,
	     &modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_access_time(
	     file_entry,
	     &access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_entry_modification_time(
	     file_entry,
	     &entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_get_file_attribute_flags(
	     file_entry,
	     &file_attribute_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		return( -1 );
	}
	if( alternate_data_stream != NULL )
	{
		if( libfsntfs_data_stream_get_size(
		     alternate_data_stream,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libfsntfs_file_entry_get_size(
		     file_entry,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size.",
			 function );

			return( -1 );
		}
	}
	result = libfsntfs_file_entry_has_directory_entries_index(
		  file_entry,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file entry has directory entries index.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		file_mode_string[ 0 ] = 'd';
		file_mode_string[ 2 ] = 'd';
	}
	else
	{
		file_mode_string[ 0 ] = 'r';
		file_mode_string[ 2 ] = 'r';
	}
	if( ( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	 || ( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 ) )
	{
		file_mode_string[ 4 ]  = '-';
		file_mode_string[ 7 ]  = '-';
		file_mode_string[ 10 ] = '-';
	}
	/* Colums in a Sleuthkit 3.x and later bodyfile
	 * MD5|name|inode|mode_as_string|UID|GID|size|atime|mtime|ctime|crtime
	 */
	fprintf(
	 info_handle->bodyfile_stream,
	 "0|" );

	if( path != NULL )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 "%" PRIs_SYSTEM "",
		 path );
	}
	if( file_entry_name != NULL )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 "%" PRIs_SYSTEM "",
		 file_entry_name );
	}
	if( data_stream_name != NULL )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 ":%" PRIs_SYSTEM "",
		 data_stream_name );
	}
/* TODO determine Sleuthkit metadata address https://wiki.sleuthkit.org/index.php?title=Metadata_Address */
/* TODO determine $DATA or $INDEX_ROOT attribute address */

	fprintf(
	 info_handle->bodyfile_stream,
	 "|%" PRIu64 "|%s|%" PRIu32 "|%" PRIu32 "|%" PRIu64 "|%.9f|%.9f|%.9f|%.9f\n",
	 file_reference & 0xffffffffffffUL,
	 file_mode_string,
	 owner_identifier,
	 group_identifier,
	 size,
	 (double) ( access_time - 116444736000000000L ) / 10000000,
	 (double) ( modification_time - 116444736000000000L ) / 10000000,
	 (double) ( entry_modification_time - 116444736000000000L ) / 10000000,
	 (double) ( creation_time - 116444736000000000L ) / 10000000 );

	return( 1 );
}

/* Prints the MFT entry to a bodyfile
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_bodyfile_mft_entry_fprint(
     info_handle_t *info_handle,
     uint64_t mft_entry_index,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute    = NULL;
	libfsntfs_file_entry_t *file_entry  = NULL;
	system_character_t *path_hint       = NULL;
	static char *function               = "info_handle_bodyfile_mft_entry_fprint";
	size_t path_hint_size               = 0;
	uint64_t base_record_file_reference = 0;
	uint32_t attribute_type             = 0;
	int attribute_index                 = 0;
	int is_empty                        = 0;
	int number_of_attributes            = 0;
	int result                          = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_mft_metadata_file != NULL )
	{
		result = libfsntfs_mft_metadata_file_get_file_entry_by_index(
		          info_handle->input_mft_metadata_file,
		          mft_entry_index,
		          &file_entry,
		          error );
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_file_entry_by_index(
		          info_handle->input_volume,
		          mft_entry_index,
		          &file_entry,
		          error );
	}
	if( result != 1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		fprintf(
		 info_handle->notify_stream,
		 "Error reading MFT entry: %" PRIu64 "\n\n",
		 mft_entry_index );

		return( 0 );
	}
	is_empty = libfsntfs_file_entry_is_empty(
	            file_entry,
	            error );

	if( is_empty == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file entry is empty.",
		 function );

		goto on_error;
	}
	else if( is_empty == 0 )
	{
		if( libfsntfs_file_entry_get_base_record_file_reference(
		     file_entry,
		     &base_record_file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve base record file reference.",
			 function );

			goto on_error;
		}
		if( base_record_file_reference == 0 )
		{
			if( libfsntfs_file_entry_get_number_of_attributes(
			     file_entry,
			     &number_of_attributes,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of attributes.",
				 function );

				goto on_error;
			}
			for( attribute_index = 0;
			     attribute_index < number_of_attributes;
			     attribute_index++ )
			{
				if( libfsntfs_file_entry_get_attribute_by_index(
				     file_entry,
				     attribute_index,
				     &attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve attribute: %d.",
					 function,
					 attribute_index );

					goto on_error;
				}
				if( libfsntfs_attribute_get_type(
				     attribute,
				     &attribute_type,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve attribute type.",
					 function );

					goto on_error;
				}
				if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libfsntfs_file_entry_get_utf16_path_hint_size(
					          file_entry,
					          attribute_index,
					          &path_hint_size,
					          error );
#else
					result = libfsntfs_file_entry_get_utf8_path_hint_size(
					          file_entry,
					          attribute_index,
					          &path_hint_size,
					          error );
#endif
					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve path hint string size.",
						 function );

						goto on_error;
					}
					if( ( result != 0 )
					 && ( path_hint_size > 0 ) )
					{
						path_hint = system_string_allocate(
						             path_hint_size );

						if( path_hint == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_MEMORY,
							 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
							 "%s: unable to create path hint string.",
							 function );

							goto on_error;
						}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
						result = libfsntfs_file_entry_get_utf16_path_hint(
						          file_entry,
						          attribute_index,
						          (uint16_t *) path_hint,
						          path_hint_size,
						          error );
#else
						result = libfsntfs_file_entry_get_utf8_path_hint(
						          file_entry,
						          attribute_index,
						          (uint8_t *) path_hint,
						          path_hint_size,
						          error );
#endif
						if( result != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve path hint string.",
							 function );

							goto on_error;
						}
					}
					if( info_handle_file_entry_fprint(
					     info_handle,
					     file_entry,
					     attribute,
					     path_hint,
					     NULL,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print file entry.",
						 function );

						goto on_error;
					}
					if( path_hint != NULL )
					{
						memory_free(
						 path_hint );

						path_hint = NULL;
					}
				}
				if( libfsntfs_attribute_free(
				     &attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free attribute: %d.",
					 function,
					 attribute_index );

					goto on_error;
				}
			}
		}
	}
	if( libfsntfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( path_hint != NULL )
	{
		memory_free(
		 path_hint );
	}
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints file entry as part of the file system hierarchy or to a bodyfile
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_entry_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_attribute_t *file_name_attribute,
     const system_character_t *path,
     const system_character_t *file_entry_name,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute               = NULL;
	libfsntfs_data_stream_t *alternate_data_stream = NULL;
	system_character_t *attribute_name             = NULL;
	system_character_t *data_stream_name           = NULL;
	static char *function                          = "info_handle_file_entry_fprint";
	size_t attribute_name_size                     = 0;
	size_t data_stream_name_size                   = 0;
	uint32_t attribute_type                        = 0;
	int alternate_data_stream_index                = 0;
	int attribute_index                            = 0;
	int has_default_data_stream                    = 0;
	int has_index_root_attributes                  = 0;
	int number_of_alternate_data_streams           = 0;
	int number_of_attributes                       = 0;
	int result                                     = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	result = libfsntfs_file_entry_has_default_data_stream(
		  file_entry,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file entry has default data stream.",
		 function );

		goto on_error;
	}
	has_default_data_stream = result;

	if( libfsntfs_file_entry_get_number_of_alternate_data_streams(
	     file_entry,
	     &number_of_alternate_data_streams,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of alternate data streams.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_get_number_of_attributes(
	     file_entry,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes.",
		 function );

		goto on_error;
	}
	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libfsntfs_file_entry_get_attribute_by_index(
		     file_entry,
		     attribute_index,
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		if( libfsntfs_attribute_get_type(
		     attribute,
		     &attribute_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute type.",
			 function );

			goto on_error;
		}
		if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT )
		{
			/* Mimic Sleuthkit behavior printing indexes
			 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_attribute_get_utf16_name_size(
			          attribute,
			          &attribute_name_size,
			          error );
#else
			result = libfsntfs_attribute_get_utf8_name_size(
			          attribute,
			          &attribute_name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute name string size.",
				 function );

				goto on_error;
			}
			if( attribute_name_size > 0 )
			{
				attribute_name = system_string_allocate(
				                  attribute_name_size );

				if( attribute_name == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create name string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libfsntfs_attribute_get_utf16_name(
				          attribute,
				          (uint16_t *) attribute_name,
				          attribute_name_size,
				          error );
#else
				result = libfsntfs_attribute_get_utf8_name(
				          attribute,
				          (uint8_t *) attribute_name,
				          attribute_name_size,
				          error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve attribute name string.",
					 function );

					goto on_error;
				}
			}
			if( info_handle->bodyfile_stream != NULL )
			{
				if( info_handle_bodyfile_index_root_attribute_fprint(
				     info_handle,
				     file_entry,
				     attribute,
				     path,
				     file_entry_name,
				     attribute_name,
				     attribute_name_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print file entry.",
					 function );

					goto on_error;
				}
			}
			else
			{
				fprintf(
				 info_handle->notify_stream,
				 "%" PRIs_SYSTEM "",
				 path );

				if( file_entry_name != NULL )
				{
					fprintf(
					 info_handle->notify_stream,
					 "%" PRIs_SYSTEM "",
					 file_entry_name );
				}
				if( ( attribute_name != NULL )
				 && ( ( attribute_name_size != 5 )
				  ||  ( narrow_string_compare(
				       attribute_name,
				       "$I30",
				       4 ) != 0 ) ) )
				{
					fprintf(
					 info_handle->notify_stream,
					 ":%" PRIs_SYSTEM "",
					 attribute_name );
				}
				fprintf(
				 info_handle->notify_stream,
				 "\n" );
			}
			if( attribute_name != NULL )
			{
				memory_free(
				 attribute_name );

				attribute_name = NULL;
			}
			has_index_root_attributes = 1;
		}
		if( libfsntfs_attribute_free(
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
	}
	/* Do not print the name of file entries that have an ADS but no default data stream
	 * and file entries that have $INDEX_ROOT attributes.
	 */
	if( ( has_index_root_attributes == 0 )
	 && ( ( has_default_data_stream != 0 )
	  ||  ( number_of_alternate_data_streams == 0 ) ) )
	{
		if( info_handle->bodyfile_stream != NULL )
		{
			if( info_handle_bodyfile_file_entry_value_fprint(
			     info_handle,
			     file_entry,
			     NULL,
			     path,
			     file_entry_name,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file entry.",
				 function );

				goto on_error;
			}
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "%" PRIs_SYSTEM "",
			 path );

			if( file_entry_name != NULL )
			{
				fprintf(
				 info_handle->notify_stream,
				 "%" PRIs_SYSTEM "",
				 file_entry_name );
			}
			fprintf(
			 info_handle->notify_stream,
			 "\n" );
		}
	}
	for( alternate_data_stream_index = 0;
	     alternate_data_stream_index < number_of_alternate_data_streams;
	     alternate_data_stream_index++ )
	{
		if( libfsntfs_file_entry_get_alternate_data_stream_by_index(
		     file_entry,
		     alternate_data_stream_index,
		     &alternate_data_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve alternate data stream: %d.",
			 function,
			 alternate_data_stream_index );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_data_stream_get_utf16_name_size(
		          alternate_data_stream,
		          &data_stream_name_size,
		          error );
#else
		result = libfsntfs_data_stream_get_utf8_name_size(
		          alternate_data_stream,
		          &data_stream_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve alternate data stream: %d name string size.",
			 function,
			 alternate_data_stream_index );

			goto on_error;
		}
		if( data_stream_name_size > 0 )
		{
			data_stream_name = system_string_allocate(
			                    data_stream_name_size );

			if( data_stream_name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create alternate data stream: %d name string.",
				 function,
				 alternate_data_stream_index );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_data_stream_get_utf16_name(
				  alternate_data_stream,
				  (uint16_t *) data_stream_name,
				  data_stream_name_size,
				  error );
#else
			result = libfsntfs_data_stream_get_utf8_name(
				  alternate_data_stream,
				  (uint8_t *) data_stream_name,
				  data_stream_name_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve alternate data stream: %d name string.",
				 function,
				 alternate_data_stream_index );

				goto on_error;
			}
			if( info_handle->bodyfile_stream != NULL )
			{
				if( info_handle_bodyfile_file_entry_value_fprint(
				     info_handle,
				     file_entry,
				     alternate_data_stream,
				     path,
				     file_entry_name,
				     data_stream_name,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print file entry.",
					 function );

					goto on_error;
				}
			}
			else
			{
				fprintf(
				 info_handle->notify_stream,
				 "%" PRIs_SYSTEM "",
				 path );

				if( file_entry_name != NULL )
				{
					fprintf(
					 info_handle->notify_stream,
					 "%" PRIs_SYSTEM ":%" PRIs_SYSTEM "",
					 file_entry_name,
					 data_stream_name );
				}
				fprintf(
				 info_handle->notify_stream,
				 "\n" );
			}
			memory_free(
			 data_stream_name );

			data_stream_name = NULL;
		}
		if( libfsntfs_data_stream_free(
		     &alternate_data_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free alternate data stream: %d.",
			 function,
			 alternate_data_stream );

			goto on_error;
		}
	}
	if( ( info_handle->bodyfile_stream != NULL )
	 && ( file_name_attribute != NULL ) )
	{
		if( info_handle_bodyfile_file_name_attribute_fprint(
		     info_handle,
		     file_entry,
		     file_name_attribute,
		     path,
		     file_entry_name,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print $FILE_NAME attribute.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	if( data_stream_name != NULL )
	{
		memory_free(
		 data_stream_name );
	}
	if( alternate_data_stream != NULL )
	{
		libfsntfs_data_stream_free(
		 &alternate_data_stream,
		 NULL );
	}
	if( attribute_name != NULL )
	{
		memory_free(
		 attribute_name );
	}
	return( -1 );
}

/* Prints file entry information as part of the file system hierarchy
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_system_hierarchy_fprint_file_entry(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     const system_character_t *path,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *file_name_attribute = NULL;
	libfsntfs_file_entry_t *sub_file_entry     = NULL;
	system_character_t *file_entry_name        = NULL;
	system_character_t *sub_path               = NULL;
	static char *function                      = "info_handle_file_system_hierarchy_fprint_file_entry";
	size_t file_entry_name_size                = 0;
	size_t path_length                         = 0;
	size_t sub_path_size                       = 0;
	int name_attribute_index                   = 0;
	int number_of_sub_file_entries             = 0;
	int result                                 = 0;
	int sub_file_entry_index                   = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_file_entry_get_utf16_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#else
	result = libfsntfs_file_entry_get_utf8_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( file_entry_name_size > 0 ) )
	{
		file_entry_name = system_string_allocate(
		                   file_entry_name_size );

		if( file_entry_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file entry name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_file_entry_get_utf16_name(
		          file_entry,
		          (uint16_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#else
		result = libfsntfs_file_entry_get_utf8_name(
		          file_entry,
		          (uint8_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry name string.",
			 function );

			goto on_error;
		}
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		result = libfsntfs_file_entry_get_name_attribute_index(
		          file_entry,
		          &name_attribute_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name attribute index.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libfsntfs_file_entry_get_attribute_by_index(
			     file_entry,
			     name_attribute_index,
			     &file_name_attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve $FILE_NAME attribute: %d.",
				 function,
				 name_attribute_index );

				goto on_error;
			}
		}
	}
	if( ( info_handle->bodyfile_stream != NULL )
	 || ( file_entry_name != NULL ) )
	{
		if( info_handle_file_entry_fprint(
		     info_handle,
		     file_entry,
		     file_name_attribute,
		     path,
		     file_entry_name,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print file entry.",
			 function );

			goto on_error;
		}
	}
	if( file_name_attribute != NULL )
	{
		if( libfsntfs_attribute_free(
		     &file_name_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attribute: %d.",
			 function,
			 name_attribute_index );

			goto on_error;
		}
	}
	if( libfsntfs_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		goto on_error;
	}
	if( number_of_sub_file_entries > 0 )
	{
		sub_path_size = path_length + 1;

		if( file_entry_name != NULL )
		{
			sub_path_size += file_entry_name_size;
		}
		sub_path = system_string_allocate(
		            sub_path_size );

		if( sub_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create sub path.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     sub_path,
		     path,
		     path_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy path to sub path.",
			 function );

			goto on_error;
		}
		if( file_entry_name != NULL )
		{
			if( system_string_copy(
			     &( sub_path[ path_length ] ),
			     file_entry_name,
			     file_entry_name_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy file entry name to sub path.",
				 function );

				goto on_error;
			}
			sub_path[ sub_path_size - 2 ] = (system_character_t) LIBFSNTFS_SEPARATOR;
		}
		sub_path[ sub_path_size - 1 ] = (system_character_t) 0;

		for( sub_file_entry_index = 0;
		     sub_file_entry_index < number_of_sub_file_entries;
		     sub_file_entry_index++ )
		{
			if( libfsntfs_file_entry_get_sub_file_entry_by_index(
			     file_entry,
			     sub_file_entry_index,
			     &sub_file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( info_handle_file_system_hierarchy_fprint_file_entry(
			     info_handle,
			     sub_file_entry,
			     sub_path,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file entry: %d information.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( libfsntfs_file_entry_free(
			     &sub_file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
		}
		if( sub_path != NULL )
		{
			memory_free(
			 sub_path );

			sub_path = NULL;
		}
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );

		file_entry_name = NULL;
	}
	return( 1 );

on_error:
	if( sub_file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	if( sub_path != NULL )
	{
		memory_free(
		 sub_path );
	}
	if( file_name_attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &file_name_attribute,
		 NULL );
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

/* Prints the MFT entry information
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_mft_entry_fprint(
     info_handle_t *info_handle,
     uint64_t mft_entry_index,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute    = NULL;
	libfsntfs_file_entry_t *file_entry  = NULL;
	static char *function               = "info_handle_mft_entry_fprint";
	uint64_t base_record_file_reference = 0;
	uint64_t value_64bit                = 0;
	int attribute_index                 = 0;
	int is_allocated                    = 0;
	int is_corrupted                    = 0;
	int is_empty                        = 0;
	int number_of_attributes            = 0;
	int result                          = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_mft_metadata_file != NULL )
	{
		result = libfsntfs_mft_metadata_file_get_file_entry_by_index(
		          info_handle->input_mft_metadata_file,
		          mft_entry_index,
		          &file_entry,
		          error );
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_file_entry_by_index(
		          info_handle->input_volume,
		          mft_entry_index,
		          &file_entry,
		          error );
	}
	if( result != 1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		fprintf(
		 info_handle->notify_stream,
		 "Error reading MFT entry: %" PRIu64 "\n\n",
		 mft_entry_index );

		return( 0 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "MFT entry: %" PRIu64 " information:\n",
	 mft_entry_index );

	is_empty = libfsntfs_file_entry_is_empty(
	            file_entry,
	            error );

	if( is_empty == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file entry is empty.",
		 function );

		goto on_error;
	}
	else if( is_empty != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tIs empty\n" );

		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	else
	{
		is_allocated = libfsntfs_file_entry_is_allocated(
		                file_entry,
		                error );

		if( is_allocated == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if file entry is allocated.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tIs allocated\t\t\t: " );

		if( is_allocated == 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "false" );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "true" );
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );

		if( libfsntfs_file_entry_get_file_reference(
		     file_entry,
		     &value_64bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file reference.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tFile reference\t\t\t: %" PRIu64 "-%" PRIu64 "\n",
		 value_64bit & 0xffffffffffffUL,
		 value_64bit >> 48 );

		if( libfsntfs_file_entry_get_base_record_file_reference(
		     file_entry,
		     &base_record_file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve base record file reference.",
			 function );

			goto on_error;
		}
		if( base_record_file_reference == 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tBase record file reference\t: Not set (0)\n" );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tBase record file reference\t: %" PRIu64 "-%" PRIu64 "\n",
			 base_record_file_reference & 0xffffffffffffUL,
			 base_record_file_reference >> 48 );
		}
		if( libfsntfs_file_entry_get_journal_sequence_number(
		     file_entry,
		     &value_64bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve journal sequence number.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tJournal sequence number\t\t: %" PRIu64 "\n",
		 value_64bit );

		if( libfsntfs_file_entry_get_number_of_attributes(
		     file_entry,
		     &number_of_attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of attributes.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tNumber of attributes\t\t: %d\n",
		 number_of_attributes );

		is_corrupted = libfsntfs_file_entry_is_corrupted(
		                file_entry,
		                error );

		if( is_corrupted == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if file entry is corrupted.",
			 function );

			goto on_error;
		}
		else if( is_corrupted != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tIs corrupted\n" );
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );

		for( attribute_index = 0;
		     attribute_index < number_of_attributes;
		     attribute_index++ )
		{
			if( libfsntfs_file_entry_get_attribute_by_index(
			     file_entry,
			     attribute_index,
			     &attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d.",
				 function,
				 attribute_index );

				goto on_error;
			}
			if( info_handle_attribute_fprint(
			     info_handle,
			     file_entry,
			     attribute,
			     attribute_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			if( libfsntfs_attribute_free(
			     &attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attribute: %d.",
				 function,
				 attribute_index );

				goto on_error;
			}
		}
	}
	if( libfsntfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the MFT entries information
 * Returns 1 if successful or -1 on error
 */
int info_handle_mft_entries_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function           = "info_handle_mft_entries_fprint";
	uint64_t file_entry_index       = 0;
	uint64_t number_of_file_entries = 0;
	int result                      = 0;

	if( info_handle->input_mft_metadata_file != NULL )
	{
		result = libfsntfs_mft_metadata_file_get_number_of_file_entries(
		          info_handle->input_mft_metadata_file,
		          &number_of_file_entries,
		          error );
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_number_of_file_entries(
		          info_handle->input_volume,
		          &number_of_file_entries,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of file entries.",
		 function );

		return( -1 );
	}
	if( number_of_file_entries > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of file entries value out of bounds.",
		 function );

		return( -1 );
	}
	for( file_entry_index = 0;
	     file_entry_index < number_of_file_entries;
	     file_entry_index++ )
	{
		if( info_handle->bodyfile_stream != NULL )
		{
			result = info_handle_bodyfile_mft_entry_fprint(
			          info_handle,
			          file_entry_index,
			          error );
		}
		else
		{
			result = info_handle_mft_entry_fprint(
			          info_handle,
			          file_entry_index,
			          error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print MFT entry: %" PRIu64 ".",
			 function,
			 file_entry_index );

			return( -1 );
		}
		else if( result == 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Unable to print MFT entry: %" PRIu64 ".\n\n",
			 file_entry_index );
		}
	}
	return( 1 );
}

/* Prints the file entry information for a specific path
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_entry_fprint_by_path(
     info_handle_t *info_handle,
     const system_character_t *path,
     libcerror_error_t **error )
{
	libfsntfs_file_entry_t *file_entry = NULL;
	static char *function              = "info_handle_file_entry_fprint_by_path";
	size_t data_stream_name_index      = 0;
	size_t data_stream_name_length     = 0;
	size_t path_index                  = 0;
	size_t path_length                 = 0;
	int result                         = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	for( path_index = path_length - 1;
	     path_index > 0;
	     path_index-- )
	{
		if( path[ path_index ] == (system_character_t) ':' )
		{
			data_stream_name_index = path_index + 1;

			break;
		}
		else if( path[ path_index ] == (system_character_t) LIBFSNTFS_SEPARATOR )
		{
			break;
		}
		data_stream_name_length++;
	}
	if( data_stream_name_index > 0 )
	{
		path_length -= data_stream_name_length + 1;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_volume_get_file_entry_by_utf16_path(
	          info_handle->input_volume,
	          (uint16_t *) path,
	          path_length,
	          &file_entry,
	          error );
#else
	result = libfsntfs_volume_get_file_entry_by_utf8_path(
	          info_handle->input_volume,
	          (uint8_t *) path,
	          path_length,
	          &file_entry,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: file entry not found.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "Windows NT File System information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "File entry:\n" );

/* TODO remove data stream name */
	fprintf(
	 info_handle->notify_stream,
	 "\tPath\t\t\t\t: %" PRIs_SYSTEM "\n",
	 path );

	if( info_handle_file_entry_value_fprint(
	     info_handle,
	     file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print file entry.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the file system hierarchy information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_system_hierarchy_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libfsntfs_file_entry_t *file_entry = NULL;
	static char *function              = "info_handle_file_system_hierarchy_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Windows NT File System information:\n\n" );

		fprintf(
		 info_handle->notify_stream,
		 "File system hierarchy:\n" );
	}
	if( libfsntfs_volume_get_root_directory(
	     info_handle->input_volume,
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root directory file entry.",
		 function );

		goto on_error;
	}
	if( info_handle_file_system_hierarchy_fprint_file_entry(
	     info_handle,
	     file_entry,
	     _SYSTEM_STRING( "\\" ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print root directory file entry information.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the USN change journal ($UsnJrnl) record update reason to the notify stream
 */
void info_handle_usn_record_update_reason_flags_fprint(
      uint32_t update_reason_flags,
      FILE *notify_stream )
{
/* TODO add description */
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_DATA_OVERWRITE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_DATA_OVERWRITE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_DATA_EXTEND ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_DATA_EXTEND)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_DATA_TRUNCATION ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_DATA_TRUNCATION)\n" );
	}

	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_NAMED_DATA_OVERWRITE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_NAMED_DATA_OVERWRITE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_NAMED_DATA_EXTEND ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_NAMED_DATA_EXTEND)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_NAMED_DATA_TRUNCATION ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_NAMED_DATA_TRUNCATION)\n" );
	}

	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_FILE_CREATE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_FILE_CREATE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_FILE_DELETE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_FILE_DELETE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_EXTENDED_ATTRIBUTE_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_EA_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_SECURITY_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_SECURITY_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_RENAME_OLD_NAME ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_RENAME_OLD_NAME)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_RENAME_NEW_NAME ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_RENAME_NEW_NAME)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_INDEXABLE_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_INDEXABLE_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_BASIC_INFO_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_BASIC_INFO_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_HARD_LINK_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_HARD_LINK_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_COMPRESSION_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_COMPRESSION_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_ENCRYPTION_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_ENCRYPTION_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_OBJECT_IDENTIFIER_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_OBJECT_IDENTIFIER_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_REPARSE_POINT_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_REPARSE_POINT_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_STREAM_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_STREAM_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_TRANSACTED_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_TRANSACTED_CHANGE)\n" );
	}

	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_CLOSE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_CLOSE)\n" );
	}
}

/* Prints the USN change journal ($UsnJrnl) record update source flags to the notify stream
 */
void info_handle_usn_record_update_source_flags_fprint(
      uint32_t update_source_flags,
      FILE *notify_stream )
{
/* TODO add description */
	if( ( update_source_flags & LIBFUSN_UPDATE_SOURCE_FLAG_DATA_MANAGEMENT ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_SOURCE_DATA_MANAGEMENT)\n" );
	}
	if( ( update_source_flags & LIBFUSN_UPDATE_SOURCE_FLAG_AUXILIARY_DATA ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_SOURCE_AUXILIARY_DATA)\n" );
	}
	if( ( update_source_flags & LIBFUSN_UPDATE_SOURCE_FLAG_REPLICATION_MANAGEMENT ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_SOURCE_REPLICATION_MANAGEMENT)\n" );
	}
}

/* Prints the USN change journal ($UsnJrnl) record information
 * Returns 1 if successful or -1 on error
 */
int info_handle_usn_record_fprint(
     info_handle_t *info_handle,
     libfusn_record_t *usn_record,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_usn_record_fprint";
	size_t value_string_size         = 0;
	uint64_t value_64bit             = 0;
	uint32_t value_32bit             = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "USN record:\n" );

	if( libfusn_record_get_update_time(
	     usn_record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update time.",
		 function );

		goto on_error;
	}
	if( info_handle_filetime_value_fprint(
	     info_handle,
	     "\tUpdate time\t\t\t",
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print FILETIME value.",
		 function );

		goto on_error;
	}
	if( libfusn_record_get_update_sequence_number(
	     usn_record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update sequence number.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tUpdate sequence number\t\t: %" PRIu64 "\n",
	 value_64bit );

	if( libfusn_record_get_update_reason_flags(
	     usn_record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update reason flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tUpdate reason flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_usn_record_update_reason_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfusn_record_get_update_source_flags(
	     usn_record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update source flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tUpdate source flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_usn_record_update_source_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfusn_record_get_utf16_name_size(
	          usn_record,
	          &value_string_size,
	          error );
#else
	result = libfusn_record_get_utf8_name_size(
	          usn_record,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name string size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfusn_record_get_utf16_name(
		          usn_record,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfusn_record_get_utf8_name(
		          usn_record,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfusn_record_get_file_reference(
	     usn_record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file reference.",
		 function );

		goto on_error;
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tFile reference\t\t\t: %" PRIu64 "\n",
		 value_64bit );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tFile reference\t\t\t: %" PRIu64 "-%" PRIu64 "\n",
		 value_64bit & 0xffffffffffffUL,
		 value_64bit >> 48 );
	}
	if( libfusn_record_get_parent_file_reference(
	     usn_record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent file reference.",
		 function );

		goto on_error;
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent file reference\t\t\t: %" PRIu64 "\n",
		 value_64bit );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent file reference\t\t: %" PRIu64 "-%" PRIu64 "\n",
		 value_64bit & 0xffffffffffffUL,
		 value_64bit >> 48 );
	}
	if( libfusn_record_get_file_attribute_flags(
	     usn_record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile attribute flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_file_attribute_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints the USN change journal ($UsnJrnl) entry information
 * Returns 1 if successful or -1 on error
 */
int info_handle_usn_change_journal_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libfsntfs_usn_change_journal_t *usn_change_journal = NULL;
	libfusn_record_t *usn_record                       = NULL;
	uint8_t *buffer                                    = NULL;
	static char *function                              = "info_handle_usn_change_journal_fprint";
	size_t journal_block_size                          = 0x1000;
	ssize_t read_count                                 = 0;
	uint32_t usn_record_size                           = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Windows NT File System information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "USN change journal: \\$Extend\\$UsnJrnl\n\n" );

/* TODO get journal block size from USN change journal */
	if( ( journal_block_size == 0 )
	 || ( journal_block_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid journal block size value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfsntfs_volume_get_usn_change_journal(
	     info_handle->input_volume,
	     &usn_change_journal,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve USN change journal.",
		 function );

		goto on_error;
	}
	buffer = (uint8_t *) memory_allocate(
	                      sizeof( uint8_t ) * journal_block_size );

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create buffer.",
		 function );

		goto on_error;
	}
	do
	{
		read_count = libfsntfs_usn_change_journal_read_usn_record(
			      usn_change_journal,
			      buffer,
			      journal_block_size,
			      error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read USN record data.",
			 function );

			goto on_error;
		}
		else if( read_count > 0 )
		{
			if( libfusn_record_initialize(
			     &usn_record,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize USN record.",
				 function );

				goto on_error;
			}
			if( libfusn_record_copy_from_byte_stream(
			     usn_record,
			     buffer,
			     read_count,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to copy USN record from byte stream.",
				 function );

				goto on_error;
			}
			if( libfusn_record_get_size(
			     usn_record,
			     &usn_record_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve USN record size.",
				 function );

				goto on_error;
			}
			if( info_handle_usn_record_fprint(
			     info_handle,
			     usn_record,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print USN record information.",
				 function );

				goto on_error;
			}
			if( libfusn_record_free(
			     &usn_record,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free USN record.",
				 function );

				goto on_error;
			}
		}
	}
	while( read_count > 0 );

	memory_free(
	 buffer );

	buffer = NULL;

	if( libfsntfs_usn_change_journal_free(
	     &usn_change_journal,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free USN change journal.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( usn_record != NULL )
	{
		libfusn_record_free(
		 &usn_record,
		 NULL );
	}
	if( buffer != NULL )
	{
		memory_free(
		 buffer );
	}
	if( usn_change_journal != NULL )
	{
		libfsntfs_usn_change_journal_free(
		 &usn_change_journal,
		 NULL );
	}
	return( -1 );
}

/* Prints the volume information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	system_character_t *volume_name = NULL;
	static char *function           = "info_handle_volume_fprint";
	size32_t cluster_block_size     = 0;
	size32_t index_entry_size       = 0;
	size32_t mft_entry_size         = 0;
	size_t volume_name_size         = 0;
	uint64_t serial_number          = 0;
	uint16_t bytes_per_sector       = 0;
	uint8_t major_version           = 0;
	uint8_t minor_version           = 0;
	int result                      = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Windows NT File System information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "Volume information:\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t: " );

	if( info_handle->input_mft_metadata_file != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_mft_metadata_file_get_utf16_volume_name_size(
		          info_handle->input_mft_metadata_file,
		          &volume_name_size,
		          error );
#else
		result = libfsntfs_mft_metadata_file_get_utf8_volume_name_size(
		          info_handle->input_mft_metadata_file,
		          &volume_name_size,
		          error );
#endif
	}
	else if( info_handle->input_volume != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_volume_get_utf16_name_size(
		          info_handle->input_volume,
		          &volume_name_size,
		          error );
#else
		result = libfsntfs_volume_get_utf8_name_size(
		          info_handle->input_volume,
		          &volume_name_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume name string size.",
		 function );

		goto on_error;
	}
	if( volume_name_size > 0 )
	{
		volume_name = system_string_allocate(
		               volume_name_size );

		if( volume_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create volume name string.",
			 function );

			goto on_error;
		}
		if( info_handle->input_mft_metadata_file != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_mft_metadata_file_get_utf16_volume_name(
			          info_handle->input_mft_metadata_file,
			          (uint16_t *) volume_name,
			          volume_name_size,
			          error );
#else
			result = libfsntfs_mft_metadata_file_get_utf8_volume_name(
			          info_handle->input_mft_metadata_file,
			          (uint8_t *) volume_name,
			          volume_name_size,
			          error );
#endif
		}
		else if( info_handle->input_volume != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_volume_get_utf16_name(
			          info_handle->input_volume,
			          (uint16_t *) volume_name,
			          volume_name_size,
			          error );
#else
			result = libfsntfs_volume_get_utf8_name(
			          info_handle->input_volume,
			          (uint8_t *) volume_name,
			          volume_name_size,
			          error );
#endif
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 volume_name );

		memory_free(
		 volume_name );

		volume_name = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( info_handle->input_mft_metadata_file != NULL )
	{
		result = libfsntfs_mft_metadata_file_get_volume_version(
		          info_handle->input_mft_metadata_file,
		          &major_version,
		          &minor_version,
		          error );
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_version(
		          info_handle->input_volume,
		          &major_version,
		          &minor_version,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve version.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tVersion\t\t\t\t: %" PRIu8 ".%" PRIu8 "\n",
	 major_version,
	 minor_version );

	if( info_handle->input_mft_metadata_file != NULL )
	{
/* TODO */
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_serial_number(
		          info_handle->input_volume,
		          &serial_number,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve serial number.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tSerial number\t\t\t: %08" PRIx64 "\n",
	 serial_number );

	if( info_handle->input_mft_metadata_file != NULL )
	{
/* TODO */
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_bytes_per_sector(
		          info_handle->input_volume,
		          &bytes_per_sector,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tBytes per sector\t\t: %" PRIu16 "\n",
	 bytes_per_sector );

	if( info_handle->input_mft_metadata_file != NULL )
	{
/* TODO */
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_cluster_block_size(
		          info_handle->input_volume,
		          &cluster_block_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cluster block size.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tCluster block size\t\t: %" PRIu32 "\n",
	 cluster_block_size );

	if( info_handle->input_mft_metadata_file != NULL )
	{
/* TODO */
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_mft_entry_size(
		          info_handle->input_volume,
		          &mft_entry_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry size.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tMFT entry size\t\t\t: %" PRIu32 "\n",
	 mft_entry_size );

	if( info_handle->input_mft_metadata_file != NULL )
	{
/* TODO */
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_index_entry_size(
		          info_handle->input_volume,
		          &index_entry_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index entry size.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tIndex entry size\t\t: %" PRIu32 "\n",
	 index_entry_size );

/* TODO print more info */

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( volume_name != NULL )
	{
		memory_free(
		 volume_name );
	}
	return( -1 );
}

