/*
 * Reparse point attribute ($REPARSE_POINT) functions
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
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_reparse_point_values.h"

#include "fsntfs_reparse_point.h"

/* Creates reparse point values
 * Make sure the value reparse_point_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_values_initialize(
     libfsntfs_reparse_point_values_t **reparse_point_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_initialize";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( *reparse_point_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid reparse point values value already set.",
		 function );

		return( -1 );
	}
	*reparse_point_values = memory_allocate_structure(
	                         libfsntfs_reparse_point_values_t );

	if( *reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create reparse point values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *reparse_point_values,
	     0,
	     sizeof( libfsntfs_reparse_point_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear reparse point values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *reparse_point_values != NULL )
	{
		memory_free(
		 *reparse_point_values );

		*reparse_point_values = NULL;
	}
	return( -1 );
}

/* Frees reparse point values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_values_free(
     libfsntfs_reparse_point_values_t **reparse_point_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_free";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( *reparse_point_values != NULL )
	{
		if( ( *reparse_point_values )->reparse_data != NULL )
		{
			memory_free(
			 ( *reparse_point_values )->reparse_data );
		}
		memory_free(
		 *reparse_point_values );

		*reparse_point_values = NULL;
	}
	return( 1 );
}

/* Reads the reparse point values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_values_read_data(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function            = "libfsntfs_reparse_point_values_read_data";
	uint32_t flags                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t *value_string = NULL;
	size_t value_string_size         = 0;
	uint32_t value_32bit             = 0;
	int result                       = 0;
#endif

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
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
		 "%s: reparse point data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_reparse_point_t *) data )->tag,
	 reparse_point_values->tag );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_reparse_point_t *) data )->reparse_data_size,
	 reparse_point_values->reparse_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: tag\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 reparse_point_values->tag );
		libfsntfs_debug_print_reparse_point_tag(
		 reparse_point_values->tag );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: reparse data size\t\t: %" PRIu16 "\n",
		 function,
		 reparse_point_values->reparse_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( reparse_point_values->reparse_data_size > 0 )
	{
		if( ( sizeof( fsntfs_reparse_point_t ) > data_size )
		 || ( (size_t) reparse_point_values->reparse_data_size > ( data_size - sizeof( fsntfs_reparse_point_t ) ) )
		 || ( (size_t) reparse_point_values->reparse_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid reparse data size value out of bounds.",
			 function );

			goto on_error;
		}
		reparse_point_values->reparse_data = (uint8_t *) memory_allocate(
		                                                  sizeof( uint8_t ) * reparse_point_values->reparse_data_size );

		if( reparse_point_values->reparse_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create reparse data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     reparse_point_values->reparse_data,
		     &( data[ sizeof( fsntfs_reparse_point_t ) ] ),
		     (size_t) reparse_point_values->reparse_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy reparse data.",
			 function );

			goto on_error;
		}
	}
	if( reparse_point_values->tag == 0x80000017 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( reparse_point_values->reparse_data[ 12 ] ),
		 reparse_point_values->compression_method );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( reparse_point_values->reparse_data[ 0 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: external version\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( reparse_point_values->reparse_data[ 4 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: external provider\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( reparse_point_values->reparse_data[ 8 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: internal version\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: compression method\t\t: %" PRIu32 " (%s)\n",
			 function,
			 reparse_point_values->compression_method,
			 libfsntfs_debug_print_compression_method(
			  reparse_point_values->compression_method ) );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( ( reparse_point_values->tag == 0xa0000003 )
	      || ( reparse_point_values->tag == 0xa000000c ) )
	{
		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mount_point_reparse_data_t *) reparse_point_values->reparse_data )->substitute_name_offset,
		 reparse_point_values->substitute_name_offset );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mount_point_reparse_data_t *) reparse_point_values->reparse_data )->substitute_name_size,
		 reparse_point_values->substitute_name_size );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mount_point_reparse_data_t *) reparse_point_values->reparse_data )->print_name_offset,
		 reparse_point_values->print_name_offset );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mount_point_reparse_data_t *) reparse_point_values->reparse_data )->print_name_size,
		 reparse_point_values->print_name_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: substitute name offset\t: 0x%04" PRIx16 "\n",
			 function,
			 reparse_point_values->substitute_name_offset );

			libcnotify_printf(
			 "%s: substitute name size\t\t: %" PRIu16 "\n",
			 function,
			 reparse_point_values->substitute_name_size );

			libcnotify_printf(
			 "%s: print name offset\t\t: 0x%04" PRIx16 "\n",
			 function,
			 reparse_point_values->print_name_offset );

			libcnotify_printf(
			 "%s: print name size\t\t: %" PRIu16 "\n",
			 function,
			 reparse_point_values->print_name_size );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	if( reparse_point_values->tag == 0xa0000003 )
	{
		reparse_point_values->substitute_name_offset += sizeof( fsntfs_mount_point_reparse_data_t );
		reparse_point_values->print_name_offset      += sizeof( fsntfs_mount_point_reparse_data_t );
	}
	else if( reparse_point_values->tag == 0xa000000c )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_symbolic_link_reparse_data_t *) reparse_point_values->reparse_data )->flags,
		 flags );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: flags\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 flags );
		}
#endif
		reparse_point_values->substitute_name_offset += sizeof( fsntfs_symbolic_link_reparse_data_t );
		reparse_point_values->print_name_offset      += sizeof( fsntfs_symbolic_link_reparse_data_t );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( reparse_point_values->tag != 0x80000017 )
		 && ( reparse_point_values->tag != 0xa0000003 )
		 && ( reparse_point_values->tag != 0xa000000c ) )
		{
			libcnotify_printf(
			 "%s: unusupported reparse point tag: 0x%08" PRIx32 "\n",
			 function,
			 reparse_point_values->tag );
		}
	}
#endif
	if( reparse_point_values->substitute_name_size > 0 )
	{
		if( reparse_point_values->substitute_name_offset >= reparse_point_values->reparse_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid substitute name offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( reparse_point_values->substitute_name_size > ( reparse_point_values->reparse_data_size - reparse_point_values->substitute_name_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid substitute name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_size_from_utf16_stream(
			          &( reparse_point_values->reparse_data[ reparse_point_values->substitute_name_offset ] ),
			          (size_t) reparse_point_values->substitute_name_size,
			          LIBUNA_ENDIAN_LITTLE,
			          &value_string_size,
			          error );
#else
			result = libuna_utf8_string_size_from_utf16_stream(
			          &( reparse_point_values->reparse_data[ reparse_point_values->substitute_name_offset ] ),
			          (size_t) reparse_point_values->substitute_name_size,
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
				 "%s: unable to determine size of substitute name string.",
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
				 "%s: unable to create substitute name string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_copy_from_utf16_stream(
			          (libuna_utf16_character_t *) value_string,
			          value_string_size,
			          &( reparse_point_values->reparse_data[ reparse_point_values->substitute_name_offset ] ),
			          (size_t) reparse_point_values->substitute_name_size,
			          LIBUNA_ENDIAN_LITTLE,
			          error );
#else
			result = libuna_utf8_string_copy_from_utf16_stream(
			          (libuna_utf8_character_t *) value_string,
			          value_string_size,
			          &( reparse_point_values->reparse_data[ reparse_point_values->substitute_name_offset ] ),
			          (size_t) reparse_point_values->substitute_name_size,
			          LIBUNA_ENDIAN_LITTLE,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set substitute name string.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: substitute name\t\t: %" PRIs_SYSTEM "\n",
			 function,
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	if( reparse_point_values->print_name_size > 0 )
	{
		if( reparse_point_values->print_name_offset >= reparse_point_values->reparse_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid print name offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( reparse_point_values->print_name_size > ( reparse_point_values->reparse_data_size - reparse_point_values->print_name_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid print name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_size_from_utf16_stream(
			          &( reparse_point_values->reparse_data[ reparse_point_values->print_name_offset ] ),
			          (size_t) reparse_point_values->print_name_size,
			          LIBUNA_ENDIAN_LITTLE,
			          &value_string_size,
			          error );
#else
			result = libuna_utf8_string_size_from_utf16_stream(
			          &( reparse_point_values->reparse_data[ reparse_point_values->print_name_offset ] ),
			          (size_t) reparse_point_values->print_name_size,
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
				 "%s: unable to determine size of print name string.",
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
				 "%s: unable to create print name string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_copy_from_utf16_stream(
			          (libuna_utf16_character_t *) value_string,
			          value_string_size,
			          &( reparse_point_values->reparse_data[ reparse_point_values->print_name_offset ] ),
			          (size_t) reparse_point_values->print_name_size,
			          LIBUNA_ENDIAN_LITTLE,
			          error );
#else
			result = libuna_utf8_string_copy_from_utf16_stream(
			          (libuna_utf8_character_t *) value_string,
			          value_string_size,
			          &( reparse_point_values->reparse_data[ reparse_point_values->print_name_offset ] ),
			          (size_t) reparse_point_values->print_name_size,
			          LIBUNA_ENDIAN_LITTLE,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set print name string.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: print name\t\t\t: %" PRIs_SYSTEM "\n",
			 function,
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
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
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
#endif
	if( reparse_point_values->reparse_data != NULL )
	{
		memory_free(
		 reparse_point_values->reparse_data );

		reparse_point_values->reparse_data = NULL;
	}
	reparse_point_values->reparse_data_size = 0;

	return( -1 );
}

/* Reads the reparse point values from an MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_values_read_from_mft_attribute(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	uint8_t *data           = NULL;
	static char *function   = "libfsntfs_reparse_point_values_read_from_mft_attribute";
	size_t data_size        = 0;
	uint32_t attribute_type = 0;
	int result              = 0;

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
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
	if( libfsntfs_reparse_point_values_read_data(
	     reparse_point_values,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read reparse point values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the tag
 * The tag is a combination of the type and flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_values_get_tag(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint32_t *tag,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_tag";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid tag.",
		 function );

		return( -1 );
	}
	*tag = reparse_point_values->tag;

	return( 1 );
}

/* Retrieves the Windows Overlay Filter (WOF) compression method
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_compression_method(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint32_t *compression_method,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_compression_method";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( compression_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid compression method.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->tag == 0x80000017 )
	{
		*compression_method = reparse_point_values->compression_method;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves the size of the UTF-8 encoded substitute name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_utf8_substitute_name_size(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_utf8_substitute_name_size";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->substitute_name_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     &( reparse_point_values->reparse_data[ reparse_point_values->substitute_name_offset ] ),
	     (size_t) reparse_point_values->substitute_name_size,
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
	return( 1 );
}

/* Retrieves the UTF-8 encoded substitute name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_utf8_substitute_name(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_utf8_substitute_name";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->substitute_name_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     &( reparse_point_values->reparse_data[ reparse_point_values->substitute_name_offset ] ),
	     (size_t) reparse_point_values->substitute_name_size,
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

/* Retrieves the size of the UTF-16 encoded substitute name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_utf16_substitute_name_size(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_utf16_substitute_name_size";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->substitute_name_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     &( reparse_point_values->reparse_data[ reparse_point_values->substitute_name_offset ] ),
	     (size_t) reparse_point_values->substitute_name_size,
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
	return( 1 );
}

/* Retrieves the UTF-16 encoded substitute name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_utf16_substitute_name(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_utf16_substitute_name";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->substitute_name_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     &( reparse_point_values->reparse_data[ reparse_point_values->substitute_name_offset ] ),
	     (size_t) reparse_point_values->substitute_name_size,
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

/* Retrieves the size of the UTF-8 encoded print name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_utf8_print_name_size(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_utf8_print_name_size";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->print_name_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     &( reparse_point_values->reparse_data[ reparse_point_values->print_name_offset ] ),
	     (size_t) reparse_point_values->print_name_size,
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
	return( 1 );
}

/* Retrieves the UTF-8 encoded print name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_utf8_print_name(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_utf8_print_name";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->print_name_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     &( reparse_point_values->reparse_data[ reparse_point_values->print_name_offset ] ),
	     (size_t) reparse_point_values->print_name_size,
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

/* Retrieves the size of the UTF-16 encoded print name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_utf16_print_name_size(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_utf16_print_name_size";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->print_name_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     &( reparse_point_values->reparse_data[ reparse_point_values->print_name_offset ] ),
	     (size_t) reparse_point_values->print_name_size,
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
	return( 1 );
}

/* Retrieves the UTF-16 encoded print name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_values_get_utf16_print_name(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_get_utf16_print_name";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( reparse_point_values->print_name_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     &( reparse_point_values->reparse_data[ reparse_point_values->print_name_offset ] ),
	     (size_t) reparse_point_values->print_name_size,
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

