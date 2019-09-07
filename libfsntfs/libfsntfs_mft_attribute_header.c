/*
 * Master File Table (MFT) attribute header functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_mft_attribute_header.h"

#include "fsntfs_mft_attribute.h"

/* Creates MFT attribute header
 * Make sure the value mft_attribute_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_header_initialize(
     libfsntfs_mft_attribute_header_t **mft_attribute_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_header_initialize";

	if( mft_attribute_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute header.",
		 function );

		return( -1 );
	}
	if( *mft_attribute_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT attribute header value already set.",
		 function );

		return( -1 );
	}
	*mft_attribute_header = memory_allocate_structure(
	                         libfsntfs_mft_attribute_header_t );

	if( *mft_attribute_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT attribute header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mft_attribute_header,
	     0,
	     sizeof( libfsntfs_mft_attribute_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MFT attribute header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *mft_attribute_header != NULL )
	{
		memory_free(
		 *mft_attribute_header );

		*mft_attribute_header = NULL;
	}
	return( -1 );
}

/* Frees MFT attribute header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_header_free(
     libfsntfs_mft_attribute_header_t **mft_attribute_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_header_free";

	if( mft_attribute_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute header.",
		 function );

		return( -1 );
	}
	if( *mft_attribute_header != NULL )
	{
		memory_free(
		 *mft_attribute_header );

		*mft_attribute_header = NULL;
	}
	return( 1 );
}

/* Reads the MFT attribute header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_header_read_data(
     libfsntfs_mft_attribute_header_t *mft_attribute_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_header_read_data";

	if( mft_attribute_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute header.",
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
	if( data_size < sizeof( fsntfs_mft_attribute_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: unsupported data size value too small\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MFT attribute header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsntfs_mft_attribute_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_mft_attribute_header_t *) data )->type,
	 mft_attribute_header->type );

	if( mft_attribute_header->type != LIBFSNTFS_ATTRIBUTE_TYPE_END_OF_ATTRIBUTES )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_mft_attribute_header_t *) data )->size,
		 mft_attribute_header->size );

		mft_attribute_header->non_resident_flag = ( (fsntfs_mft_attribute_header_t *) data )->non_resident_flag;

		mft_attribute_header->name_size = (uint16_t) ( (fsntfs_mft_attribute_header_t *) data )->name_size;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_header_t *) data )->name_offset,
		 mft_attribute_header->name_offset );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_header_t *) data )->data_flags,
		 mft_attribute_header->data_flags );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_header_t *) data )->identifier,
		 mft_attribute_header->identifier );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 mft_attribute_header->type,
		 libfsntfs_debug_print_attribute_type(
		  mft_attribute_header->type ) );

		if( mft_attribute_header->type != LIBFSNTFS_ATTRIBUTE_TYPE_END_OF_ATTRIBUTES )
		{
			libcnotify_printf(
			 "%s: size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 mft_attribute_header->size );

			libcnotify_printf(
			 "%s: non resident flag\t\t: 0x%02" PRIx8 "\n",
			 function,
			 mft_attribute_header->non_resident_flag );

			libcnotify_printf(
			 "%s: name size\t\t\t: %" PRIu16 "\n",
			 function,
			 mft_attribute_header->name_size );

			libcnotify_printf(
			 "%s: name offset\t\t\t: %" PRIu16 "\n",
			 function,
			 mft_attribute_header->name_offset );

			libcnotify_printf(
			 "%s: data flags\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 mft_attribute_header->data_flags );
			libfsntfs_debug_print_mft_attribute_data_flags(
			 mft_attribute_header->data_flags );
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "%s: identifier\t\t\t: %" PRIu16 "\n",
			 function,
			 mft_attribute_header->identifier );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	mft_attribute_header->name_size *= 2;

	return( 1 );
}

