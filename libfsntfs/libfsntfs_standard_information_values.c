/*
 * Standard information attribute ($STANDARAD_INFORMATION) functions
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
#include <system_string.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdatetime.h"
#include "libfsntfs_standard_information_values.h"

#include "fsntfs_standard_information.h"

/* Creates standard information values
 * Make sure the value standard_information_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_initialize(
     libfsntfs_standard_information_values_t **standard_information_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_initialize";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
		 function );

		return( -1 );
	}
	if( *standard_information_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid standard information values value already set.",
		 function );

		return( -1 );
	}
	*standard_information_values = memory_allocate_structure(
	                                libfsntfs_standard_information_values_t );

	if( *standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create standard information values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *standard_information_values,
	     0,
	     sizeof( libfsntfs_standard_information_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear standard information values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *standard_information_values != NULL )
	{
		memory_free(
		 *standard_information_values );

		*standard_information_values = NULL;
	}
	return( -1 );
}

/* Frees standard information values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_free(
     libfsntfs_standard_information_values_t **standard_information_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_free";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
		 function );

		return( -1 );
	}
	if( *standard_information_values != NULL )
	{
		memory_free(
		 *standard_information_values );

		*standard_information_values = NULL;
	}
	return( 1 );
}

/* Reads the standard information values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_read(
     libfsntfs_standard_information_values_t *standard_information_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function             = "libfsntfs_standard_information_values_read";

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t filetime_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
	uint64_t value_64bit              = 0;
	uint32_t value_32bit              = 0;
	int result                        = 0;
#endif

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: standard information data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( ( data_size != 48 )
	 && ( data_size != 72 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported standard information data size: %" PRIzd "\n",
		 function,
		 data_size );

		goto on_error;
	}
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_standard_information_t *) data )->creation_time,
	 standard_information_values->creation_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_standard_information_t *) data )->modification_time,
	 standard_information_values->modification_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_standard_information_t *) data )->access_time,
	 standard_information_values->access_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_standard_information_t *) data )->entry_modification_time,
	 standard_information_values->entry_modification_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_standard_information_t *) data )->file_attribute_flags,
	 standard_information_values->file_attribute_flags );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
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
		     ( (fsntfs_standard_information_t *) data )->creation_time,
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
		 "%s: creation time\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (fsntfs_standard_information_t *) data )->modification_time,
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
		     ( (fsntfs_standard_information_t *) data )->entry_modification_time,
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
		     ( (fsntfs_standard_information_t *) data )->access_time,
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

		libcnotify_printf(
		 "%s: file attribute flags\t\t: 0x%08" PRIx32 "\n",
		 function,
		 standard_information_values->file_attribute_flags );
		libfsntfs_debug_print_file_attribute_flags(
		 standard_information_values->file_attribute_flags );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_standard_information_t *) data )->maximum_number_of_versions,
		 value_32bit );
		libcnotify_printf(
		 "%s: maximum number of versions\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_standard_information_t *) data )->version_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: version number\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_standard_information_t *) data )->class_identifier,
		 value_32bit );
		libcnotify_printf(
		 "%s: class identifier\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

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
	if( data_size > 48 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_standard_information_t *) data )->owner_identifier,
		 standard_information_values->owner_identifier );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_standard_information_t *) data )->security_descriptor_identifier,
		 standard_information_values->security_descriptor_identifier );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_standard_information_t *) data )->update_sequence_number,
		 standard_information_values->update_sequence_number );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: owner identifier\t\t\t: %" PRIu32 "\n",
			 function,
			 standard_information_values->owner_identifier );

			libcnotify_printf(
			 "%s: security descriptor identifier\t: %" PRIu32 "\n",
			 function,
			 standard_information_values->security_descriptor_identifier );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsntfs_standard_information_t *) data )->quota_charged,
			 value_64bit );
			libcnotify_printf(
			 "%s: quota charged\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: update sequence number\t\t: %" PRIu64 "\n",
			 function,
			 standard_information_values->update_sequence_number );
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
#endif
	return( -1 );
}

/* Retrieves the creation date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_get_creation_time(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *creation_time,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_get_creation_time";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
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
	*creation_time = standard_information_values->creation_time;

	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_get_modification_time(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *modification_time,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_get_modification_time";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
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
	*modification_time = standard_information_values->modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_get_access_time(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *access_time,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_get_access_time";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
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
	*access_time = standard_information_values->access_time;

	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_get_entry_modification_time(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *entry_modification_time,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_get_entry_modification_time";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
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
	*entry_modification_time = standard_information_values->entry_modification_time;

	return( 1 );
}

/* Retrieves the file attribute flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_get_file_attribute_flags(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_get_file_attribute_flags";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
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
	*file_attribute_flags = standard_information_values->file_attribute_flags;

	return( 1 );
}

/* Retrieves the security descriptor identifier
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_get_security_descriptor_identifier(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint32_t *security_descriptor_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_get_security_descriptor_identifier";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
		 function );

		return( -1 );
	}
	if( security_descriptor_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor identifier.",
		 function );

		return( -1 );
	}
	*security_descriptor_identifier = standard_information_values->security_descriptor_identifier;

	return( 1 );
}

/* Retrieves the update sequence number
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_values_get_update_sequence_number(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *update_sequence_number,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_standard_information_values_get_update_sequence_number";

	if( standard_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid standard information values.",
		 function );

		return( -1 );
	}
	if( update_sequence_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid update sequence number.",
		 function );

		return( -1 );
	}
	*update_sequence_number = standard_information_values->update_sequence_number;

	return( 1 );
}

