/*
 * Attribute list entry functions
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_attribute_list_entry.h"
#include "libfsntfs_name.h"

#include "fsntfs_mft_attribute_list.h"

/* Creates attribute list entry
 * Make sure the value attribute_list_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_initialize(
     libfsntfs_mft_attribute_list_entry_t **attribute_list_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_initialize";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( *attribute_list_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute list entry value already set.",
		 function );

		return( -1 );
	}
	*attribute_list_entry = memory_allocate_structure(
	                         libfsntfs_mft_attribute_list_entry_t );

	if( *attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute list entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attribute_list_entry,
	     0,
	     sizeof( libfsntfs_mft_attribute_list_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute list entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attribute_list_entry != NULL )
	{
		memory_free(
		 *attribute_list_entry );

		*attribute_list_entry = NULL;
	}
	return( -1 );
}

/* Frees attribute list entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_free(
     libfsntfs_mft_attribute_list_entry_t **attribute_list_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_free";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( *attribute_list_entry != NULL )
	{
		if( ( *attribute_list_entry )->name != NULL )
		{
			memory_free(
			 ( *attribute_list_entry )->name );
		}
		memory_free(
		 *attribute_list_entry );

		*attribute_list_entry = NULL;
	}
	return( 1 );
}

/* Reads the attribute list entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_read_data(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_read_data";
	size_t data_offset    = 0;
	uint8_t name_offset   = 0;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( attribute_list_entry->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute list entry - name value already set.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( fsntfs_mft_attribute_list_entry_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: unsupported data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute list entry header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsntfs_mft_attribute_list_entry_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_mft_attribute_list_entry_header_t *) data )->type,
	 attribute_list_entry->attribute_type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_attribute_list_entry_header_t *) data )->size,
	 attribute_list_entry->size );

	attribute_list_entry->name_size = (uint16_t) ( (fsntfs_mft_attribute_list_entry_header_t *) data )->name_size;

	name_offset = ( (fsntfs_mft_attribute_list_entry_header_t *) data )->name_offset;

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_mft_attribute_list_entry_header_t *) data )->data_first_vcn,
	 attribute_list_entry->data_first_vcn );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_mft_attribute_list_entry_header_t *) data )->file_reference,
	 attribute_list_entry->file_reference );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_attribute_list_entry_header_t *) data )->identifier,
	 attribute_list_entry->identifier );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 attribute_list_entry->attribute_type,
		 libfsntfs_debug_print_attribute_type(
		  attribute_list_entry->attribute_type ) );

		libcnotify_printf(
		 "%s: size\t\t\t: %" PRIu16 "\n",
		 function,
		 attribute_list_entry->size );

		libcnotify_printf(
		 "%s: name size\t\t\t: %" PRIu16 "\n",
		 function,
		 attribute_list_entry->name_size );

		libcnotify_printf(
		 "%s: name offset\t\t: %" PRIu8 "\n",
		 function,
		 name_offset );

		libcnotify_printf(
		 "%s: data first VCN\t\t: %" PRIu64 "\n",
		 function,
		 attribute_list_entry->data_first_vcn );

		libcnotify_printf(
		 "%s: file reference\t\t: %" PRIu64 "-%" PRIu64 "\n",
		 function,
		 attribute_list_entry->file_reference & 0xffffffffffffUL,
		 attribute_list_entry->file_reference >> 48 );

		libcnotify_printf(
		 "%s: identifier\t\t: %" PRIu16 "\n",
		 function,
		 attribute_list_entry->identifier );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsntfs_mft_attribute_list_entry_header_t );

	if( ( attribute_list_entry->size < sizeof( fsntfs_mft_attribute_list_entry_header_t ) )
	 || ( attribute_list_entry->size > data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: size value out of bounds.",
		 function );

		goto on_error;
	}
	attribute_list_entry->name_size *= 2;

	if( attribute_list_entry->name_size > 0 )
	{
		if( ( name_offset < sizeof( fsntfs_mft_attribute_list_entry_header_t ) )
		 || ( name_offset >= attribute_list_entry->size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: name offset value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( data_offset < name_offset )
			{
				libcnotify_printf(
				 "%s: unknown data:\n",
				 function );
				libcnotify_print_data(
				 &( data[ data_offset ] ),
				 (size_t) name_offset - data_offset,
				 0 );
			}
		}
#endif
		if( attribute_list_entry->name_size > ( attribute_list_entry->size - name_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: name size value out of bounds.",
			 function );

			goto on_error;
		}
		data_offset = (size_t) name_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) attribute_list_entry->name_size,
			 0 );
		}
#endif
		attribute_list_entry->name = (uint8_t *) memory_allocate(
		                                          sizeof( uint8_t ) * (size_t) attribute_list_entry->name_size );

		if( attribute_list_entry->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     attribute_list_entry->name,
		     &( data[ data_offset ] ),
		     (size_t) attribute_list_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			goto on_error;
		}
		data_offset += (size_t) attribute_list_entry->name_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfsntfs_debug_print_utf16_string_value(
			     function,
			     "name\t\t\t",
			     attribute_list_entry->name,
			     (size_t) attribute_list_entry->name_size,
			     LIBUNA_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print UTF-16 string value.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < attribute_list_entry->size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) attribute_list_entry->size - data_offset,
			 0 );
		}
		else
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif
	return( 1 );

on_error:
	if( attribute_list_entry->name != NULL )
	{
		memory_free(
		 attribute_list_entry->name );

		attribute_list_entry->name = NULL;
	}
	attribute_list_entry->name_size = 0;

	return( -1 );
}

/* Retrieves the attribute type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_get_attribute_type(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     uint32_t *attribute_type,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_get_attribute_type";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( attribute_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute type.",
		 function );

		return( -1 );
	}
	*attribute_type = attribute_list_entry->attribute_type;

	return( 1 );
}

/* Retrieves the file references as an MFT entry index and sequence number
 * If the value sequence_number is NULL it will be ignored
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_get_file_reference(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_get_file_reference";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file reference.",
		 function );

		return( -1 );
	}
	*file_reference = attribute_list_entry->file_reference;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_get_utf8_name_size(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_get_utf8_name_size";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( ( attribute_list_entry->name == NULL )
	 || ( attribute_list_entry->name_size == 0 ) )
	{
		if( utf8_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string size.",
			 function );

			return( -1 );
		}
		*utf8_string_size = 0;
	}
	else
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     attribute_list_entry->name,
		     (size_t) attribute_list_entry->name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_get_utf8_name(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_get_utf8_name";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( attribute_list_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute list entry - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     attribute_list_entry->name,
	     (size_t) attribute_list_entry->name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_get_utf16_name_size(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_get_utf16_name_size";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( ( attribute_list_entry->name == NULL )
	 || ( attribute_list_entry->name_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf16_stream(
		     attribute_list_entry->name,
		     (size_t) attribute_list_entry->name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_get_utf16_name(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_get_utf16_name";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( attribute_list_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute list entry - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     attribute_list_entry->name,
	     (size_t) attribute_list_entry->name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares the name with an UTF-8 encoded string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_compare_name_with_utf8_string(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_compare_name_with_utf8_string";
	int result            = 0;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute.",
		 function );

		return( -1 );
	}
	if( attribute_list_entry->name == NULL )
	{
		return( 0 );
	}
	result = libfsntfs_name_compare_with_utf8_string(
	          attribute_list_entry->name,
	          attribute_list_entry->name_size,
	          utf8_string,
	          utf8_string_length,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with name.",
		 function );

		return( -1 );
	}
	else if( result == LIBUNA_COMPARE_EQUAL )
	{
		return( 1 );
	}
	return( 0 );
}

/* Compares the name with an UTF-16 encoded string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libfsntfs_mft_attribute_list_entry_compare_name_with_utf16_string(
     libfsntfs_mft_attribute_list_entry_t *attribute_list_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_entry_compare_name_with_utf16_string";
	int result            = 0;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute.",
		 function );

		return( -1 );
	}
	if( attribute_list_entry->name == NULL )
	{
		return( 0 );
	}
	result = libfsntfs_name_compare_with_utf16_string(
	          attribute_list_entry->name,
	          attribute_list_entry->name_size,
	          utf16_string,
	          utf16_string_length,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-16 string with name.",
		 function );

		return( -1 );
	}
	else if( result == LIBUNA_COMPARE_EQUAL )
	{
		return( 1 );
	}
	return( 0 );
}

