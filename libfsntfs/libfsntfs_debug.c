/*
 * Debug functions
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdatetime.h"
#include "libfsntfs_libfguid.h"
#include "libfsntfs_libuna.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the MFT attribute data flags
 */
void libfsntfs_debug_print_mft_attribute_data_flags(
      uint16_t mft_attribute_data_flags )
{
	if( ( mft_attribute_data_flags & 0x0001 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs compressed\n" );
	}

	if( ( mft_attribute_data_flags & 0x4000 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs encrypted\n" );
	}
	if( ( mft_attribute_data_flags & 0x8000 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs sparse\n" );
	}
}

/* Prints the MFT entry data flags
 */
void libfsntfs_debug_print_mft_entry_flags(
      uint16_t mft_entry_flags )
{
	if( ( mft_entry_flags & LIBFSNTFS_MFT_ENTRY_FLAG_IN_USE ) != 0 )
	{
		libcnotify_printf(
		 "\tIn use (FILE_RECORD_SEGMENT_IN_USE)\n" );
	}
	if( ( mft_entry_flags & LIBFSNTFS_MFT_ENTRY_FLAG_INDEX_PRESENT ) != 0 )
	{
		libcnotify_printf(
		 "\tHas file name index (FILE_FILE_NAME_INDEX_PRESENT)\n" );
	}
	if( ( mft_entry_flags & 0x0004 ) != 0 )
	{
		libcnotify_printf(
		 "\tUnknown: 0x0004\n" );
	}
	if( ( mft_entry_flags & 0x0008 ) != 0 )
	{
		libcnotify_printf(
		 "\tUnknown: 0x0008\n" );
	}
}

/* Prints the file attribute flags
 */
void libfsntfs_debug_print_file_attribute_flags(
      uint32_t file_attribute_flags )
{
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	{
		libcnotify_printf(
		 "\tIs read-only (FILE_ATTRIBUTE_READ_ONLY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_HIDDEN ) != 0 )
	{
		libcnotify_printf(
		 "\tIs hidden (FILE_ATTRIBUTE_HIDDEN)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 )
	{
		libcnotify_printf(
		 "\tIs system (FILE_ATTRIBUTE_SYSTEM)\n" );
	}

	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	{
		libcnotify_printf(
		 "\tIs directory (FILE_ATTRIBUTE_DIRECTORY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ARCHIVE ) != 0 )
	{
		libcnotify_printf(
		 "\tShould be archived (FILE_ATTRIBUTE_ARCHIVE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	{
		libcnotify_printf(
		 "\tIs device (FILE_ATTRIBUTE_DEVICE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NORMAL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs normal (FILE_ATTRIBUTE_NORMAL)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_TEMPORARY ) != 0 )
	{
		libcnotify_printf(
		 "\tIs temporary (FILE_ATTRIBUTE_TEMPORARY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SPARSE_FILE ) != 0 )
	{
		libcnotify_printf(
		 "\tIs a sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_REPARSE_POINT ) != 0 )
	{
		libcnotify_printf(
		 "\tIs a reparse point or symbolic link (FILE_ATTRIBUTE_FLAG_REPARSE_POINT)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_COMPRESSED ) != 0 )
	{
		libcnotify_printf(
		 "\tIs compressed (FILE_ATTRIBUTE_COMPRESSED)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_OFFLINE ) != 0 )
	{
		libcnotify_printf(
		 "\tIs offline (FILE_ATTRIBUTE_OFFLINE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED ) != 0 )
	{
		libcnotify_printf(
		 "\tContent should not be indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ENCRYPTED ) != 0 )
	{
		libcnotify_printf(
		 "\tIs encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n" );
	}

	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_VIRTUAL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs virtual (FILE_ATTRIBUTE_VIRTUAL)\n" );
	}

	if( ( file_attribute_flags & 0x10000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tUnknown (0x10000000)\n" );
	}
	if( ( file_attribute_flags & 0x20000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs index view (0x20000000)\n" );
	}
}

/* Prints the index node flags
 */
void libfsntfs_debug_print_index_node_flags(
      uint32_t index_node_flags )
{
	if( ( index_node_flags & 0x00000001UL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs branch node\n" );
	}
}

/* Prints the index value flags
 */
void libfsntfs_debug_print_index_value_flags(
      uint32_t index_value_flags )
{
	if( ( index_value_flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
	{
		libcnotify_printf(
		 "\tIs branch node\n" );
	}
	if( ( index_value_flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
	{
		libcnotify_printf(
		 "\tIs last\n" );
	}
}

/* Prints the reparse point type and flags
 */
void libfsntfs_debug_print_reparse_point_tag(
      uint32_t tag )
{
	if( tag == 0x00000000 )
	{
		libcnotify_printf(
		 "\tReserved (IO_REPARSE_TAG_RESERVED_ZERO)\n" );
	}
	if( tag == 0x00000001 )
	{
		libcnotify_printf(
		 "\tReserved (IO_REPARSE_TAG_RESERVED_ONE)\n" );
	}

	if( tag == 0x80000005 )
	{
		libcnotify_printf(
		 "\tHome server drive extender (IO_REPARSE_TAG_DRIVER_EXTENDER)\n" );
	}
	if( tag == 0x80000006 )
	{
		libcnotify_printf(
		 "\tHierarchical Storage Manager (IO_REPARSE_TAG_HSM2)\n" );
	}
	if( tag == 0x80000007 )
	{
		libcnotify_printf(
		 "\tSingle-instance storage (SIS) (IO_REPARSE_TAG_SIS)\n" );
	}

	if( tag == 0x8000000a )
	{
		libcnotify_printf(
		 "\tDistributed File System (DFS) (IO_REPARSE_TAG_DFS)\n" );
	}
	if( tag == 0x8000000b )
	{
		libcnotify_printf(
		 "\tFilter manager test harness (IO_REPARSE_TAG_FILTER_MANAGER)\n" );
	}

	if( tag == 0x80000012 )
	{
		libcnotify_printf(
		 "\tDistributed File System (DFS) (IO_REPARSE_TAG_DFSR)\n" );
	}

	if( tag == 0xa0000003 )
	{
		libcnotify_printf(
		 "\tMount point (IO_REPARSE_TAG_MOUNT_POINT)\n" );
	}

	if( tag == 0xa000000c )
	{
		libcnotify_printf(
		 "\tSymbolic link (IO_REPARSE_TAG_SYMLINK)\n" );
	}

	if( tag == 0xc0000004 )
	{
		libcnotify_printf(
		 "\tHierarchical Storage Manager (IO_REPARSE_TAG_HSM)\n" );
	}
}

/* Prints the volume information flags
 */
void libfsntfs_debug_print_volume_information_flags(
      uint16_t volume_information_flags )
{
	if( ( volume_information_flags & 0x0001 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs dirty (VOLUME_IS_DIRTY)\n" );
	}
	if( ( volume_information_flags & 0x0002 ) != 0 )
	{
		libcnotify_printf(
		 "\t(VOLUME_RESIZE_LOG_FILE)\n" );
	}
	if( ( volume_information_flags & 0x0004 ) != 0 )
	{
		libcnotify_printf(
		 "\t(VOLUME_UPGRADE_ON_MOUNT)\n" );
	}
	if( ( volume_information_flags & 0x0008 ) != 0 )
	{
		libcnotify_printf(
		 "\t(VOLUME_MOUNTED_ON_NT4)\n" );
	}
	if( ( volume_information_flags & 0x0010 ) != 0 )
	{
		libcnotify_printf(
		 "\t(VOLUME_DELETE_USN_UNDERWAY)\n" );
	}
	if( ( volume_information_flags & 0x0020 ) != 0 )
	{
		libcnotify_printf(
		 "\t(VOLUME_REPAIR_OBJECT_ID)\n" );
	}

	if( ( volume_information_flags & 0x4000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(VOLUME_CHKDSK_UNDERWAY)\n" );
	}
	if( ( volume_information_flags & 0x8000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(VOLUME_MODIFIED_BY_CHKDSK)\n" );
	}
}

/* Prints the attribute type
 */
const char *libfsntfs_debug_print_attribute_type(
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

/* Prints the collation type
 */
const char *libfsntfs_debug_print_collation_type(
             uint32_t collation_type )
{
	switch( collation_type )
	{
		case 0x00000000UL:
			return( "COLLATION_BINARY" );

		case 0x00000001UL:
			return( "COLLATION_FILENAME" );

		case 0x00000002UL:
			return( "COLLATION_UNICODE_STRING" );

		case 0x00000010UL:
			return( "COLLATION_NTOFS_ULONG" );

		case 0x00000011UL:
			return( "COLLATION_NTOFS_SID" );

		case 0x00000012UL:
			return( "COLLATION_NTOFS_SECURITY_HASH" );

		case 0x00000013UL:
			return( "COLLATION_NTOFS_ULONGS" );
	}
	return( "Unknown" );
}

/* Prints the compression method
 */
const char *libfsntfs_debug_print_compression_method(
             uint32_t compression_method )
{
	switch( compression_method )
	{
		case 0:
			return( "XPRESS4K" );

		case 1:
			return( "LZX" );

		case 2:
			return( "XPRESS8K" );

		case 3:
			return( "XPRESS16K" );
	}
	return( "Unknown" );
}

/* Prints the file name attribute name space
 */
const char *libfsntfs_debug_print_file_name_attribute_name_space(
             uint8_t name_space )
{
	switch( name_space )
	{
		case LIBFSNTFS_FILE_NAME_SPACE_POSIX:
			return( "POSIX" );

		case LIBFSNTFS_FILE_NAME_SPACE_WINDOWS:
			return( "Windows" );

		case LIBFSNTFS_FILE_NAME_SPACE_DOS:
			return( "DOS" );

		case LIBFSNTFS_FILE_NAME_SPACE_DOS_WINDOWS:
			return( "DOS and Windows" );
	}
	return( "UNKNOWN" );
}

/* Prints a FILETIME value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_debug_print_filetime_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char date_time_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
	static char *function             = "libfsntfs_debug_print_filetime_value";

	if( libfdatetime_filetime_initialize(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create filetime.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_byte_stream(
	     filetime,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to filetime.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_to_utf8_string(
	     filetime,
	     (uint8_t *) date_time_string,
	     32,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s UTC\n",
	 function_name,
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
		 "%s: unable to free filetime.",
		 function );

		goto on_error;
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

/* Prints a GUID/UUID value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_debug_print_guid_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	static char *function       = "libfsntfs_debug_print_guid_value";

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
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
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
	if( libfguid_identifier_copy_to_utf8_string(
	     guid,
	     (uint8_t *) guid_string,
	     48,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 guid_string );

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

/* Prints an UTF-16 string value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_debug_print_utf16_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libfsntfs_debug_print_utf16_string_value";
	size_t string_size         = 0;
	int result                 = 0;

	if( ( byte_stream == NULL )
	 || ( byte_stream_size == 0 ) )
	{
		libcnotify_printf(
		 "%s: %s:\n",
		 function_name,
		 value_name );

		return( 1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
	          byte_stream,
	          byte_stream_size,
	          byte_order,
	          &string_size,
	          error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
	          byte_stream,
	          byte_stream_size,
	          byte_order,
	          &string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of string.",
		 function );

		goto on_error;
	}
	if( string_size > ( (size_t) SSIZE_MAX / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s:",
	 function_name,
	 value_name );

	if( string_size > 0 )
	{
		string = system_string_allocate(
		          string_size );

		if( string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_utf16_string_copy_from_utf16_stream(
		          (libuna_utf16_character_t *) string,
		          string_size,
		          byte_stream,
		          byte_stream_size,
		          byte_order,
		          error );
#else
		result = libuna_utf8_string_copy_from_utf16_stream(
		          (libuna_utf8_character_t *) string,
		          string_size,
		          byte_stream,
		          byte_stream_size,
		          byte_order,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 " %s",
		 string );

		memory_free(
		 string );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );

on_error:
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_debug_print_read_offsets";
	size64_t size         = 0;
	off64_t offset        = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

