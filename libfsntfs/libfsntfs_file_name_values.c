/*
 * File name attribute ($FILE_NAME) values functions
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdatetime.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_name.h"

#include "fsntfs_file_name.h"

/* Creates file name values
 * Make sure the value file_name_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_initialize(
     libfsntfs_file_name_values_t **file_name_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_initialize";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( *file_name_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file name values value already set.",
		 function );

		return( -1 );
	}
	*file_name_values = memory_allocate_structure(
	                     libfsntfs_file_name_values_t );

	if( *file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file name values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_name_values,
	     0,
	     sizeof( libfsntfs_file_name_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file name values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_name_values != NULL )
	{
		memory_free(
		 *file_name_values );

		*file_name_values = NULL;
	}
	return( -1 );
}

/* Frees file name values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_free(
     libfsntfs_file_name_values_t **file_name_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_free";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( *file_name_values != NULL )
	{
		if( ( *file_name_values )->name != NULL )
		{
			memory_free(
			 ( *file_name_values )->name );
		}
		memory_free(
		 *file_name_values );

		*file_name_values = NULL;
	}
	return( 1 );
}

/* Clones file name values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_clone(
     libfsntfs_file_name_values_t **destination_file_name_values,
     libfsntfs_file_name_values_t *source_file_name_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_clone";

	if( destination_file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( *destination_file_name_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination file name values value already set.",
		 function );

		return( -1 );
	}
	if( source_file_name_values == NULL )
	{
		*destination_file_name_values = source_file_name_values;

		return( 1 );
	}
	*destination_file_name_values = memory_allocate_structure(
	                                 libfsntfs_file_name_values_t );

	if( *destination_file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination file name values.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_file_name_values,
	     source_file_name_values,
	     sizeof( libfsntfs_file_name_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to copy source file name values to destination.",
		 function );

		goto on_error;
	}
	( *destination_file_name_values )->name      = NULL;
	( *destination_file_name_values )->name_size = 0;

	if( libfsntfs_file_name_values_set_name(
	     *destination_file_name_values,
	     source_file_name_values->name,
	     source_file_name_values->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set destination name.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_file_name_values != NULL )
	{
		memory_free(
		 *destination_file_name_values );

		*destination_file_name_values = NULL;
	}
	return( -1 );
}

/* Sets the name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_set_name(
     libfsntfs_file_name_values_t *file_name_values,
     const uint8_t *utf16_stream,
     size_t utf16_stream_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_set_name";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( file_name_values->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file name values - name value already set.",
		 function );

		return( -1 );
	}
	if( utf16_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 stream.",
		 function );

		return( -1 );
	}
	if( ( utf16_stream_size == 0 )
	 || ( utf16_stream_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 stream size value out of bounds.",
		 function );

		return( -1 );
	}
	file_name_values->name = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * (size_t) utf16_stream_size );

	if( file_name_values->name == NULL )
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
	     file_name_values->name,
	     utf16_stream,
	     utf16_stream_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		goto on_error;
	}
	file_name_values->name_size = utf16_stream_size;

	return( 1 );

on_error:
	if( file_name_values->name != NULL )
	{
		memory_free(
		 file_name_values->name );

		file_name_values->name = NULL;
	}
	file_name_values->name_size = 0;

	return( -1 );
}

/* Reads the file name values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_read_data(
     libfsntfs_file_name_values_t *file_name_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_read_data";
	size_t data_offset    = 0;
	uint16_t name_size    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
#endif

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( file_name_values->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file name values - name already set.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( fsntfs_file_name_t ) )
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
		 "%s: file name data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_file_name_t *) data )->parent_file_reference,
	 file_name_values->parent_file_reference );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_file_name_t *) data )->creation_time,
	 file_name_values->creation_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_file_name_t *) data )->modification_time,
	 file_name_values->modification_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_file_name_t *) data )->access_time,
	 file_name_values->access_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_file_name_t *) data )->entry_modification_time,
	 file_name_values->entry_modification_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_file_name_t *) data )->file_attribute_flags,
	 file_name_values->file_attribute_flags );

	name_size = (uint16_t) ( (fsntfs_file_name_t *) data )->name_size;

	file_name_values->name_space = ( (fsntfs_file_name_t *) data )->name_space;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: parent file reference\t\t: %" PRIu64 "-%" PRIu64 "\n",
		 function,
		 file_name_values->parent_file_reference & 0xffffffffffffUL,
		 file_name_values->parent_file_reference >> 48 );

		if( libfsntfs_debug_print_filetime_value(
		     function,
		     "creation time\t\t\t",
		     ( (fsntfs_file_name_t *) data )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
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
		if( libfsntfs_debug_print_filetime_value(
		     function,
		     "modification time\t\t\t",
		     ( (fsntfs_file_name_t *) data )->modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
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
		if( libfsntfs_debug_print_filetime_value(
		     function,
		     "entry modification time\t\t",
		     ( (fsntfs_file_name_t *) data )->entry_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
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
		if( libfsntfs_debug_print_filetime_value(
		     function,
		     "access time\t\t\t",
		     ( (fsntfs_file_name_t *) data )->access_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
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
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_file_name_t *) data )->allocated_data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: allocated data size\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_file_name_t *) data )->data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: file attribute flags\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_name_values->file_attribute_flags );
		libfsntfs_debug_print_file_attribute_flags(
		 file_name_values->file_attribute_flags );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_file_name_t *) data )->extended_data,
		 value_32bit );
		libcnotify_printf(
		 "%s: extended data\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 name_size );

		libcnotify_printf(
		 "%s: name space\t\t\t: %" PRIu8 " (%s)\n",
		 function,
		 file_name_values->name_space,
		 libfsntfs_debug_print_file_name_attribute_name_space(
		  file_name_values->name_space ) );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsntfs_file_name_t );

	if( name_size > 0 )
	{
		name_size *= 2;

		if( (size_t) name_size > ( data_size - data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
		if( libfsntfs_file_name_values_set_name(
		     file_name_values,
		     &( data[ data_offset ] ),
		     (size_t) name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfsntfs_debug_print_utf16_string_value(
			     function,
			     "name\t\t\t\t",
			     file_name_values->name,
			     file_name_values->name_size,
			     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
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
		data_offset += file_name_values->name_size;

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

/* TODO debug print alignment padding */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < data_size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 data_size - data_offset,
			 0 );
		}
	}
#endif
	return( 1 );

on_error:
	if( file_name_values->name != NULL )
	{
		memory_free(
		 file_name_values->name );

		file_name_values->name = NULL;
	}
	file_name_values->name_size = 0;

	return( -1 );
}

/* Reads the file name values from an MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_read_from_mft_attribute(
     libfsntfs_file_name_values_t *file_name_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	uint8_t *data           = NULL;
	static char *function   = "libfsntfs_file_name_values_read_from_mft_attribute";
	size_t data_size        = 0;
	uint32_t attribute_type = 0;
	int result              = 0;

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_type(
	     mft_attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type from attribute.",
		 function );

		return( -1 );
	}
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_mft_attribute_data_is_resident(
	          mft_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if attribute data is resident.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported non-resident attribute.",
		 function );

		return( 1 );
	}
	if( libfsntfs_mft_attribute_get_resident_data(
	     mft_attribute,
	     &data,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resident data from attribute.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_name_values_read_data(
	     file_name_values,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file name values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the parent file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_parent_file_reference(
     libfsntfs_file_name_values_t *file_name_values,
     uint64_t *parent_file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_parent_file_reference";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( parent_file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent file reference.",
		 function );

		return( -1 );
	}
	*parent_file_reference = file_name_values->parent_file_reference;

	return( 1 );
}

/* Retrieves the creation date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_creation_time(
     libfsntfs_file_name_values_t *file_name_values,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_creation_time";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = file_name_values->creation_time;

	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_modification_time(
     libfsntfs_file_name_values_t *file_name_values,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_modification_time";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = file_name_values->modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_access_time(
     libfsntfs_file_name_values_t *file_name_values,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_access_time";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = file_name_values->access_time;

	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_entry_modification_time(
     libfsntfs_file_name_values_t *file_name_values,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_entry_modification_time";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = file_name_values->entry_modification_time;

	return( 1 );
}

/* Retrieves the file attribute flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_file_attribute_flags(
     libfsntfs_file_name_values_t *file_name_values,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_file_attribute_flags";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( file_attribute_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file attribute flags.",
		 function );

		return( -1 );
	}
	*file_attribute_flags = file_name_values->file_attribute_flags;

	return( 1 );
}

/* Retrieves the name space
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_name_space(
     libfsntfs_file_name_values_t *file_name_values,
     uint8_t *name_space,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_name_space";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( name_space == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name space.",
		 function );

		return( -1 );
	}
	*name_space = file_name_values->name_space;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_utf8_name_size(
     libfsntfs_file_name_values_t *file_name_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_utf8_name_size";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     file_name_values->name,
	     (size_t) file_name_values->name_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
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
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_utf8_name(
     libfsntfs_file_name_values_t *file_name_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_utf8_name";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     file_name_values->name,
	     (size_t) file_name_values->name_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: string data:\n",
		 function );
		libcnotify_print_data(
		 utf8_string,
		 utf8_string_size,
		 0 );
	}
#endif
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_utf16_name_size(
     libfsntfs_file_name_values_t *file_name_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_utf16_name_size";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     file_name_values->name,
	     (size_t) file_name_values->name_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
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
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_utf16_name(
     libfsntfs_file_name_values_t *file_name_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_name_values_get_utf16_name";

	if( file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file name values.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     file_name_values->name,
	     (size_t) file_name_values->name_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
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

