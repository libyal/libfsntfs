/*
 * Master File Table (MFT) attribute functions
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
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_attribute.h"

#include "fsntfs_mft_attribute.h"

/* Creates MFT attribute
 * Make sure the value mft_attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_initialize(
     libfsntfs_mft_attribute_t **mft_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_initialize";

	if( mft_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute.",
		 function );

		return( -1 );
	}
	if( *mft_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT attribute value already set.",
		 function );

		return( -1 );
	}
	*mft_attribute = memory_allocate_structure(
	                  libfsntfs_mft_attribute_t );

	if( *mft_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mft_attribute,
	     0,
	     sizeof( libfsntfs_mft_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MFT attribute.",
		 function );

		memory_free(
		 *mft_attribute );

		*mft_attribute = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *mft_attribute != NULL )
	{
		if( ( *mft_attribute )->name != NULL )
		{
			memory_free(
			 ( *mft_attribute )->name );
		}
		memory_free(
		 *mft_attribute );

		*mft_attribute = NULL;
	}
	return( -1 );
}

/* Frees MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_free(
     libfsntfs_mft_attribute_t **mft_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_free";

	if( mft_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute.",
		 function );

		return( -1 );
	}
	if( *mft_attribute != NULL )
	{
		memory_free(
		 *mft_attribute );

		*mft_attribute = NULL;
	}
	return( 1 );
}

/* Reads the MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_read_data(
     libfsntfs_mft_attribute_t *mft_attribute,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	const uint8_t *non_resident_data = NULL;
	const uint8_t *resident_data     = NULL;
	static char *function            = "libfsntfs_mft_attribute_read_data";
	size_t data_offset               = 0;
	size_t non_resident_data_size    = 0;
	uint16_t compression_unit_size   = 0;
	uint16_t name_offset             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit             = 0;
	uint32_t value_32bit             = 0;
#endif

	if( mft_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute.",
		 function );

		return( -1 );
	}
	if( mft_attribute->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT attribute - name value already set.",
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
	 mft_attribute->type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_mft_attribute_header_t *) data )->size,
	 mft_attribute->size );

	mft_attribute->non_resident_flag = ( (fsntfs_mft_attribute_header_t *) data )->non_resident_flag;

	mft_attribute->name_size = (uint16_t) ( (fsntfs_mft_attribute_header_t *) data )->name_size;

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_attribute_header_t *) data )->name_offset,
	 name_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_attribute_header_t *) data )->data_flags,
	 mft_attribute->data_flags );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_attribute_header_t *) data )->identifier,
	 mft_attribute->identifier );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 mft_attribute->type,
		 libfsntfs_debug_print_attribute_type(
		  mft_attribute->type ) );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 mft_attribute->size );

		libcnotify_printf(
		 "%s: non resident flag\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 mft_attribute->non_resident_flag );

		libcnotify_printf(
		 "%s: name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_attribute->name_size );

		libcnotify_printf(
		 "%s: name offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 name_offset );

		libcnotify_printf(
		 "%s: data flags\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 mft_attribute->data_flags );
		libfsntfs_debug_print_mft_attribute_data_flags(
		 mft_attribute->data_flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_attribute->identifier );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsntfs_mft_attribute_header_t );

	if( mft_attribute->size > data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( ( mft_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0x0000 )
	 && ( ( mft_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0x0001 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression flags: 0x%04" PRIx16 ".",
		 function,
		 mft_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK );

		goto on_error;
	}
	mft_attribute->name_size *= 2;

	if( ( mft_attribute->non_resident_flag & 0x01 ) == 0 )
	{
		if( data_offset > ( mft_attribute->size - sizeof( fsntfs_mft_attribute_resident_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: unsupported data size value too small.",
			 function );

			goto on_error;
		}
		resident_data = &( data[ data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: MFT attribute resident data:\n",
			 function );
			libcnotify_print_data(
			 resident_data,
			 sizeof( fsntfs_mft_attribute_resident_t ),
			 0 );
		}
#endif
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_mft_attribute_resident_t *) resident_data )->data_size,
		 mft_attribute->data_size );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_resident_t *) resident_data )->data_offset,
		 mft_attribute->data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 mft_attribute->data_size );

			libcnotify_printf(
			 "%s: data offset\t\t\t\t: %" PRIu16 "\n",
			 function,
			 mft_attribute->data_offset );

			libcnotify_printf(
			 "%s: indexed flag\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsntfs_mft_attribute_resident_t *) resident_data )->indexed_flag );

			libcnotify_printf(
			 "%s: padding\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsntfs_mft_attribute_resident_t *) resident_data )->padding );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsntfs_mft_attribute_resident_t );
	}
	else
	{
		non_resident_data_size = sizeof( fsntfs_mft_attribute_non_resident_t );

		if( data_offset > ( mft_attribute->size - non_resident_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: unsupported data size value too small.",
			 function );

			goto on_error;
		}
		non_resident_data = &( data[ data_offset ] );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->compression_unit_size,
		 compression_unit_size );

		if( compression_unit_size != 0 )
		{
			non_resident_data_size = sizeof( fsntfs_mft_attribute_non_resident_compressed_t );

			if( data_size < non_resident_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: unsupported data size value too small.",
				 function );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: MFT attribute non-resident data:\n",
			 function );
			libcnotify_print_data(
			 non_resident_data,
			 non_resident_data_size,
			 0 );
		}
#endif
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->data_first_vcn,
		 mft_attribute->data_first_vcn );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->data_last_vcn,
		 mft_attribute->data_last_vcn );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->data_runs_offset,
		 mft_attribute->data_runs_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->allocated_data_size,
		 mft_attribute->allocated_data_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->data_size,
		 mft_attribute->data_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->valid_data_size,
		 mft_attribute->valid_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data first VCN\t\t\t: %" PRIi64 "\n",
			 function,
			 (int64_t) mft_attribute->data_first_vcn );

			libcnotify_printf(
			 "%s: data last VCN\t\t\t: %" PRIi64 "\n",
			 function,
			 (int64_t) mft_attribute->data_last_vcn );

			libcnotify_printf(
			 "%s: data runs offset\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 mft_attribute->data_runs_offset );

			libcnotify_printf(
			 "%s: compression unit size\t\t: %" PRIu16 " (%" PRIzd ")\n",
			 function,
			 compression_unit_size,
			 mft_attribute->compression_unit_size );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->padding,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: allocated data size\t\t\t: %" PRIu64 "\n",
			 function,
			 mft_attribute->allocated_data_size );

			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 mft_attribute->data_size );

			libcnotify_printf(
			 "%s: valid data size\t\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
			 function,
			 mft_attribute->valid_data_size,
			 mft_attribute->valid_data_size );

			if( compression_unit_size > 0 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (fsntfs_mft_attribute_non_resident_compressed_t *) non_resident_data )->total_data_size,
				 value_64bit );
				libcnotify_printf(
				 "%s: total data size\t: %" PRIu64 "\n",
				 function,
				 value_64bit );
			}
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsntfs_mft_attribute_non_resident_t );

		if( mft_attribute->valid_data_size > mft_attribute->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: valid data size value out of bounds.",
			 function );

			goto on_error;
		}
	}
	if( mft_attribute->name_size > 0 )
	{
		if( ( name_offset < data_offset )
		 || ( name_offset >= mft_attribute->size ) )
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
		if( mft_attribute->name_size > ( mft_attribute->size - name_offset ) )
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
			 (size_t) mft_attribute->name_size,
			 0 );
		}
#endif
		mft_attribute->name = (uint8_t *) memory_allocate(
		                                   sizeof( uint8_t ) * (size_t) mft_attribute->name_size );

		if( mft_attribute->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create attribute name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     mft_attribute->name,
		     &( data[ data_offset ] ),
		     (size_t) mft_attribute->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy attribute name.",
			 function );

			goto on_error;
		}
		data_offset += (size_t) mft_attribute->name_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfsntfs_debug_print_utf16_string_value(
			     function,
			     "name\t\t\t\t\t",
			     mft_attribute->name,
			     (size_t) mft_attribute->name_size,
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
	if( mft_attribute->data_size > 0 )
	{
		if( ( mft_attribute->non_resident_flag & 0x01 ) == 0 )
		{
			if( ( mft_attribute->data_offset < data_offset )
			 || ( mft_attribute->data_offset >= mft_attribute->size ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: data offset value out of bounds.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: data:\n",
				 function );
				libcnotify_print_data(
				 &( data[ mft_attribute->data_offset ] ),
				 (size_t) mft_attribute->data_size,
				 0 );
			}
#endif
			data_offset = (size_t) mft_attribute->data_offset + (size_t) mft_attribute->data_size;
		}
		else
		{
			if( ( mft_attribute->data_runs_offset < data_offset )
			 || ( mft_attribute->data_runs_offset >= mft_attribute->size ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: data runs offset value out of bounds.",
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
					 (size_t) mft_attribute->data_runs_offset - data_offset,
					 0 );
				}
			}
#endif
			data_offset = (size_t) mft_attribute->data_runs_offset;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < mft_attribute->size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) mft_attribute->size - data_offset,
			 0 );
		}
	}
#endif
	return( 1 );

on_error:
	if( mft_attribute->name != NULL )
	{
		memory_free(
		 mft_attribute->name );

		mft_attribute->name = NULL;
	}
	mft_attribute->name_size = 0;

	return( -1 );
}

