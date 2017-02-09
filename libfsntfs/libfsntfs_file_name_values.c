/*
 * File name attribute ($FILE_NAME) values functions
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
#include <byte_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdatetime.h"
#include "libfsntfs_libuna.h"

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

/* Reads the file name values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_read(
     libfsntfs_file_name_values_t *file_name_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function             = "libfsntfs_file_name_values_read";
	size_t data_offset                = 0;
	uint16_t name_size                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t filetime_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
	system_character_t *value_string  = NULL;
	size_t value_string_size          = 0;
	uint64_t value_64bit              = 0;
	uint32_t value_32bit              = 0;
	int result                        = 0;
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

	file_name_values->name_namespace = ( (fsntfs_file_name_t *) data )->name_namespace;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: parent file reference\t\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 function,
		 file_name_values->parent_file_reference & 0xffffffffffffUL,
		 file_name_values->parent_file_reference >> 48 );

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
		     ( (fsntfs_file_name_t *) data )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime from byte stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: creation time\t\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (fsntfs_file_name_t *) data )->modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime from byte stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: modification time\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (fsntfs_file_name_t *) data )->entry_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime from byte stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: entry modification time\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (fsntfs_file_name_t *) data )->access_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime from byte stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: access time\t\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_file_name_t *) data )->allocated_file_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: allocated file size\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_file_name_t *) data )->file_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: file size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: file attribute flags\t\t\t: 0x%08" PRIx32 "\n",
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
		 "%s: extended data\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 name_size );

		libcnotify_printf(
		 "%s: name namespace\t\t\t\t: %" PRIu8 " (%s)\n",
		 function,
		 file_name_values->name_namespace,
		 libfsntfs_debug_print_file_name_attribute_namespace(
		  file_name_values->name_namespace ) );

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
	}
#endif
	data_offset = sizeof( fsntfs_file_name_t );

	if( name_size > 0 )
	{
		name_size *= 2;

		if( libfsntfs_file_name_values_set_name(
		     file_name_values,
		     &( data[ data_offset ] ),
		     name_size,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_size_from_utf16_stream(
				  file_name_values->name,
				  file_name_values->name_size,
				  LIBUNA_ENDIAN_LITTLE,
				  &value_string_size,
				  error );
#else
			result = libuna_utf8_string_size_from_utf16_stream(
				  file_name_values->name,
				  file_name_values->name_size,
				  LIBUNA_ENDIAN_LITTLE,
				  &value_string_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size of name string.",
				 function );

				goto on_error;
			}
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
			result = libuna_utf16_string_copy_from_utf16_stream(
				  (libuna_utf16_character_t *) value_string,
				  value_string_size,
				  file_name_values->name,
				  file_name_values->name_size,
				  LIBUNA_ENDIAN_LITTLE,
				  error );
#else
			result = libuna_utf8_string_copy_from_utf16_stream(
				  (libuna_utf8_character_t *) value_string,
				  value_string_size,
				  file_name_values->name,
				  file_name_values->name_size,
				  LIBUNA_ENDIAN_LITTLE,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name string.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: name\t\t\t\t\t: %" PRIs_SYSTEM "\n",
			 function,
			 value_string );

			memory_free(
			 value_string );

			libcnotify_printf(
			 "\n" );
		}
		data_offset += file_name_values->name_size;
#endif

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
#endif
	if( file_name_values->name != NULL )
	{
		memory_free(
		 file_name_values->name );

		file_name_values->name = NULL;
	}
	file_name_values->name_size = 0;

	return( -1 );
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
     uint64_t *creation_time,
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
	if( creation_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid creation time.",
		 function );

		return( -1 );
	}
	*creation_time = file_name_values->creation_time;

	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_modification_time(
     libfsntfs_file_name_values_t *file_name_values,
     uint64_t *modification_time,
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
	if( modification_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid modification time.",
		 function );

		return( -1 );
	}
	*modification_time = file_name_values->modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_access_time(
     libfsntfs_file_name_values_t *file_name_values,
     uint64_t *access_time,
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
	if( access_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access time.",
		 function );

		return( -1 );
	}
	*access_time = file_name_values->access_time;

	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_entry_modification_time(
     libfsntfs_file_name_values_t *file_name_values,
     uint64_t *entry_modification_time,
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
	if( entry_modification_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry modification time.",
		 function );

		return( -1 );
	}
	*entry_modification_time = file_name_values->entry_modification_time;

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

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_utf8_name_size(
     libfsntfs_file_name_values_t *file_name_values,
     size_t *utf8_name_size,
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
	     LIBUNA_ENDIAN_LITTLE,
	     utf8_name_size,
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
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_utf8_name(
     libfsntfs_file_name_values_t *file_name_values,
     uint8_t *utf8_name,
     size_t utf8_name_size,
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
	     utf8_name,
	     utf8_name_size,
	     file_name_values->name,
	     (size_t) file_name_values->name_size,
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
int libfsntfs_file_name_values_get_utf16_name_size(
     libfsntfs_file_name_values_t *file_name_values,
     size_t *utf16_name_size,
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
	     LIBUNA_ENDIAN_LITTLE,
	     utf16_name_size,
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
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_get_utf16_name(
     libfsntfs_file_name_values_t *file_name_values,
     uint16_t *utf16_name,
     size_t utf16_name_size,
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
	     utf16_name,
	     utf16_name_size,
	     file_name_values->name,
	     (size_t) file_name_values->name_size,
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

/* Sets the name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_name_values_set_name(
     libfsntfs_file_name_values_t *file_name_values,
     const uint8_t *name,
     uint16_t name_size,
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
	file_name_values->name = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * (size_t) name_size );

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
	     name,
	     (size_t) name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		goto on_error;
	}
	file_name_values->name_size = name_size;

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

/* Compares a (long-named) file name values with a short-named file name values
 * Returns 1 if (possibly) equal, 0 if not or -1 on error
 */
int libfsntfs_file_name_values_compare_short_name(
     libfsntfs_file_name_values_t *file_name_values,
     libfsntfs_file_name_values_t *short_file_name_values,
     libcerror_error_t **error )
{
	static char *function   = "libfsntfs_file_name_values_compare_short_name";
	uint8_t in_tilde_suffix = 0;
	int name_index          = 0;
	int name_iterator       = 0;
	int short_name_index    = 0;

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
	if( file_name_values->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file name values - missing name.",
		 function );

		return( -1 );
	}
	if( short_file_name_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid short file name values.",
		 function );

		return( -1 );
	}
	if( short_file_name_values->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid short file name values - missing name.",
		 function );

		return( -1 );
	}
	for( short_name_index = 0;
	     short_name_index < short_file_name_values->name_size;
	     short_name_index += 2 )
	{
		if( in_tilde_suffix != 0 )
		{
			/* Check for a . in the short name
			 */
			if( short_file_name_values->name[ short_name_index ] == 0x2e )
			{
				in_tilde_suffix = 0;

				/* Find the last . in the (long) name
				 */
				name_iterator = file_name_values->name_size - 2;

				while( name_iterator >= name_index )
				{
					if( file_name_values->name[ name_iterator ] == 0x2e )
					{
						break;
					}
					name_iterator -= 2;
				}
				if( name_iterator < name_index )
				{
					break;
				}
				name_index = name_iterator;
			}
			else
			{
				/* The ~ suffix should only consist of the numeric characters 0 - 9
				 */
				if( ( short_file_name_values->name[ short_name_index ] < 0x30 )
				 || ( short_file_name_values->name[ short_name_index ] > 0x39 ) )
				{
#if defined( HAVE_VERBOSE_OUTPUT )
					libcnotify_printf(
					 "%s: detected invalid character: %c in short name ~ suffix\n",
					 function,
					 (char) short_file_name_values->name[ short_name_index ] );
#endif
					break;
				}
				continue;
			}
		}
		/* Check for a ~ in the short name
		 */
		if( short_file_name_values->name[ short_name_index ] == 0x7e )
		{
			in_tilde_suffix = 1;

			continue;
		}
		while( name_index < file_name_values->name_size )
		{
			/* Ignore any non ASCII characters in the (long) name
			 */
			if( file_name_values->name[ name_index + 1 ] != 0 )
			{
				name_index += 2;
			}
			/* Ignore control characters and spaces
			 */
			else if( file_name_values->name[ name_index ] <= 0x20 )
			{
				name_index += 2;
			}
			/* Ignore '"' '*' '+' ',' '/' ':' ';' '<' '=' '>' '?' '\'
			 */
			else if( ( file_name_values->name[ name_index ] == 0x22 )
			      || ( ( file_name_values->name[ name_index ] >= 0x2a )
				&& ( file_name_values->name[ name_index ] <= 0x2c ) )
			      || ( file_name_values->name[ name_index ] == 0x2f )
			      || ( ( file_name_values->name[ name_index ] >= 0x3a )
				&& ( file_name_values->name[ name_index ] <= 0x3f ) )
			      || ( file_name_values->name[ name_index ] == 0x5c ) )
			{
				name_index += 2;
			}
			else
			{
				break;
			}
		}
		if( name_index >= file_name_values->name_size )
		{
			break;
		}
		/* Check for a-z in the (long) name
		 */
		if( ( file_name_values->name[ name_index ] >= 0x61 )
		 && ( file_name_values->name[ name_index ] <= 0x7a ) )
		{
			if( ( file_name_values->name[ name_index ] - 0x20 ) != short_file_name_values->name[ short_name_index ] )
			{
				break;
			}
		}
		else
		{
			if( file_name_values->name[ name_index ] != short_file_name_values->name[ short_name_index ] )
			{
				break;
			}
		}
		name_index += 2;
	}
	if( short_name_index == short_file_name_values->name_size )
	{
		if( in_tilde_suffix != 0 )
		{
			/* Find the last . in the (long) name
			 */
			name_iterator = file_name_values->name_size - 2;

			while( name_iterator >= name_index )
			{
				if( file_name_values->name[ name_iterator ] == 0x2e )
				{
					break;
				}
				name_iterator -= 2;
			}
			/* If no . was found (as far as we can tell) the name matches the short name
			 */
			if( name_iterator < name_index )
			{
				name_index = file_name_values->name_size;
			}
		}
		if( name_index == file_name_values->name_size )
		{
			return( 1 );
		}
	}
	return( 0 );
}

