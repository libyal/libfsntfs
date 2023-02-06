/*
 * Master File Table (MFT) attribute functions
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

#include "libfsntfs_data_run.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_extent.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_name.h"

#include "fsntfs_mft_attribute.h"

/* Creates a MFT attribute
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

		goto on_error;
	}
	return( 1 );

on_error:
	if( *mft_attribute != NULL )
	{
		memory_free(
		 *mft_attribute );

		*mft_attribute = NULL;
	}
	return( -1 );
}

/* Frees a MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_free(
     libfsntfs_mft_attribute_t **mft_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_free";
	int result            = 1;

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
		if( ( *mft_attribute )->name != NULL )
		{
			memory_free(
			 ( *mft_attribute )->name );
		}
		if( ( *mft_attribute )->data != NULL )
		{
			memory_free(
			 ( *mft_attribute )->data );
		}
		if( ( *mft_attribute )->data_runs_array != NULL )
		{
			if( libcdata_array_free(
			     &( ( *mft_attribute )->data_runs_array ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_data_run_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data runs array.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *mft_attribute );

		*mft_attribute = NULL;
	}
	return( result );
}

/* Clones a MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_clone(
     libfsntfs_mft_attribute_t **destination_mft_attribute,
     libfsntfs_mft_attribute_t *source_mft_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_clone";

	if( destination_mft_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute.",
		 function );

		return( -1 );
	}
	if( *destination_mft_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination MFT attribute value already set.",
		 function );

		return( -1 );
	}
	if( source_mft_attribute == NULL )
	{
		*destination_mft_attribute = source_mft_attribute;

		return( 1 );
	}
	*destination_mft_attribute = memory_allocate_structure(
	                              libfsntfs_mft_attribute_t );

	if( *destination_mft_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destnation MFT attribute.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_mft_attribute,
	     source_mft_attribute,
	     sizeof( libfsntfs_mft_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to copy source MFT attribute to destination.",
		 function );

		goto on_error;
	}
	( *destination_mft_attribute )->name            = NULL;
	( *destination_mft_attribute )->data            = NULL;
	( *destination_mft_attribute )->data_runs_array = NULL;
	( *destination_mft_attribute )->next_attribute  = NULL;

	if( source_mft_attribute->name != NULL )
	{
		( *destination_mft_attribute )->name = (uint8_t *) memory_allocate(
		                                                    source_mft_attribute->name_size );

		if( ( *destination_mft_attribute )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_mft_attribute )->name,
		     source_mft_attribute->name,
		     source_mft_attribute->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source name to destination.",
			 function );

			goto on_error;
		}
		( *destination_mft_attribute )->name_size = source_mft_attribute->name_size;
	}
	if( source_mft_attribute->data != NULL )
	{
		( *destination_mft_attribute )->data = (uint8_t *) memory_allocate(
		                                                    source_mft_attribute->data_size );

		if( ( *destination_mft_attribute )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_mft_attribute )->data,
		     source_mft_attribute->data,
		     source_mft_attribute->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source data to destination.",
			 function );

			goto on_error;
		}
		( *destination_mft_attribute )->data_size = source_mft_attribute->data_size;
	}
	if( libcdata_array_clone(
	     &( ( *destination_mft_attribute )->data_runs_array ),
	     source_mft_attribute->data_runs_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_data_run_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfsntfs_data_run_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone data runs array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_mft_attribute != NULL )
	{
		if( ( *destination_mft_attribute )->data != NULL )
		{
			memory_free(
			 ( *destination_mft_attribute )->data );
		}
		if( ( *destination_mft_attribute )->data != NULL )
		{
			memory_free(
			 ( *destination_mft_attribute )->data );
		}
		memory_free(
		 *destination_mft_attribute );

		*destination_mft_attribute = NULL;
	}
	return( -1 );
}

/* Reads the MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_read_data(
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsntfs_data_run_t *data_run     = NULL;
	const uint8_t *non_resident_data   = NULL;
	const uint8_t *resident_data       = NULL;
	static char *function              = "libfsntfs_mft_attribute_read_data";
	size_t data_offset                 = 0;
	size_t non_resident_data_size      = 0;
	ssize_t read_count                 = 0;
	uint64_t last_cluster_block_number = 0;
	uint16_t compression_unit_size     = 0;
	uint16_t data_runs_offset          = 0;
	uint16_t name_offset               = 0;
	int data_run_index                 = 0;
	int entry_index                    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit               = 0;
	uint32_t value_32bit               = 0;
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
	if( mft_attribute->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT attribute - data value already set.",
		 function );

		return( -1 );
	}
	if( mft_attribute->data_runs_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT attribute - data runs array value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
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

	if( ( mft_attribute->size < sizeof( fsntfs_mft_attribute_header_t ) )
	 || ( mft_attribute->size > data_size ) )
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
	if( ( mft_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
	{
		if( io_handle->cluster_block_size > 4096 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compression flags: 0x%04" PRIx16 " for volume with cluster block size: %" PRIzd ".",
			 function,
			 mft_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK,
			 io_handle->cluster_block_size );

			goto on_error;
		}
	}
	mft_attribute->name_size *= 2;

	if( ( mft_attribute->non_resident_flag & 0x01 ) == 0 )
	{
		if( data_size < ( data_offset + sizeof( fsntfs_mft_attribute_resident_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: unsupported data size value too small\n",
			 function );

			return( -1 );
		}
		resident_data = &( data[ data_offset ] );

		if( mft_attribute->size < ( data_offset + sizeof( fsntfs_mft_attribute_resident_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid MFT attribute size value out of bounds.",
			 function );

			goto on_error;
		}
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
		if( data_size < ( data_offset + sizeof( fsntfs_mft_attribute_non_resident_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: unsupported data size value too small\n",
			 function );

			return( -1 );
		}
		non_resident_data      = &( data[ data_offset ] );
		non_resident_data_size = sizeof( fsntfs_mft_attribute_non_resident_t );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_non_resident_t *) non_resident_data )->compression_unit_size,
		 compression_unit_size );

		if( compression_unit_size != 0 )
		{
			if( data_size < ( data_offset + sizeof( fsntfs_mft_attribute_non_resident_compressed_t ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: unsupported data size value too small.",
				 function );

				goto on_error;
			}
			non_resident_data_size = sizeof( fsntfs_mft_attribute_non_resident_compressed_t );

			if( compression_unit_size > 31 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compression unit size value out of bounds.",
				 function );

				goto on_error;
			}
			/* The size is calculated as: 2 ^ value
			 */
			mft_attribute->compression_unit_size  = (size_t) 1 << compression_unit_size;
			mft_attribute->compression_unit_size *= io_handle->cluster_block_size;
		}
		else if( ( mft_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: data is flagged as compressed but no compression unit size set.\n",
				 function );
			}
#endif
			mft_attribute->compression_unit_size = 16 * io_handle->cluster_block_size;
		}
		if( mft_attribute->size < ( data_offset + non_resident_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid MFT attribute size value out of bounds.",
			 function );

			goto on_error;
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
		 data_runs_offset );

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
			 data_runs_offset );

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
				 "%s: total data size\t\t\t: %" PRIu64 "\n",
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
			 "%s: unable to create name.",
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
			 "%s: unable to copy name.",
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
	if( ( mft_attribute->non_resident_flag & 0x01 ) == 0 )
	{
		if( mft_attribute->data_size > 0 )
		{
			if( ( mft_attribute->data_offset < data_offset )
			 || ( mft_attribute->data_offset >= mft_attribute->size ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: resident data offset value out of bounds.",
				 function );

				goto on_error;
			}
			if( mft_attribute->data_size > ( mft_attribute->size - mft_attribute->data_offset ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: resident data size value out of bounds.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: resident data:\n",
				 function );
				libcnotify_print_data(
				 &( data[ mft_attribute->data_offset ] ),
				 (size_t) mft_attribute->data_size,
				 0 );
			}
#endif
			mft_attribute->data = (uint8_t *) memory_allocate(
			                                   sizeof( uint8_t ) * (size_t) mft_attribute->data_size );

			if( mft_attribute->data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create resident data.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     mft_attribute->data,
			     &( data[ mft_attribute->data_offset ] ),
			     (size_t) mft_attribute->data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy resident data.",
				 function );

				goto on_error;
			}
			data_offset = (size_t) mft_attribute->data_offset + (size_t) mft_attribute->data_size;
		}
	}
	else
	{
		/* Note that data size is set in the first attribute of a chain
		 * and successive elements contain a size of 0
		 */
		if( ( data_runs_offset < data_offset )
		 || ( data_runs_offset >= mft_attribute->size ) )
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
				 (size_t) data_runs_offset - data_offset,
				 0 );
			}
		}
#endif
		data_offset = (size_t) data_runs_offset;

		if( libcdata_array_initialize(
		     &( mft_attribute->data_runs_array ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data runs array.",
			 function );

			goto on_error;
		}
		while( data_offset < data_size )
		{
			if( libfsntfs_data_run_initialize(
			     &data_run,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create data run: %d.",
				 function,
				 data_run_index );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading data run: %d.\n",
				 function,
				 data_run_index );
			}
#endif
			read_count = libfsntfs_data_run_read_data(
			              data_run,
			              io_handle,
			              &( data[ data_offset ] ),
			              data_size - data_offset,
			              last_cluster_block_number,
			              error );

			if( read_count <= -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read data run: %d.",
				 function,
				 data_run_index );

				goto on_error;
			}
			else if( read_count == 1 )
			{
				if( libfsntfs_data_run_free(
				     &data_run,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free data run: %d.",
					 function,
					 data_run_index );

					goto on_error;
				}
				break;
			}
			data_offset += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( data_run->start_offset == 0 )
				 && ( ( mft_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) == 0 )
				 && ( ( mft_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_SPARSE ) == 0 ) )
				{
					libcnotify_printf(
					 "%s: data run is sparse but no attribute data flags set.\n\n",
					 function );
				}
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) == 0 )
			{
				last_cluster_block_number = data_run->cluster_block_number;
			}
			if( libcdata_array_append_entry(
			     mft_attribute->data_runs_array,
			     &entry_index,
			     (intptr_t *) data_run,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append data run: %d to array.",
				 function,
				 data_run_index );

				goto on_error;
			}
			data_run = NULL;

			data_run_index++;
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
	if( data_run != NULL )
	{
		libfsntfs_data_run_free(
		 &data_run,
		 NULL );
	}
	if( mft_attribute->data_runs_array != NULL )
	{
		libcdata_array_free(
		 &( mft_attribute->data_runs_array ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_data_run_free,
		 NULL );
	}
	if( mft_attribute->data != NULL )
	{
		memory_free(
		 mft_attribute->data );

		mft_attribute->data = NULL;
	}
	if( mft_attribute->name != NULL )
	{
		memory_free(
		 mft_attribute->name );

		mft_attribute->name = NULL;
	}
	mft_attribute->name_size = 0;

	return( -1 );
}

/* Determines if the attribute data is resident
 * Returns 1 if the attribute data is resident, 0 if not or -1 on error
 */
int libfsntfs_mft_attribute_data_is_resident(
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_data_is_resident";

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
	if( ( mft_attribute->non_resident_flag & 0x01 ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_type(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint32_t *type,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_type";

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
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = mft_attribute->type;

	return( 1 );
}

/* Retrieves the data flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_data_flags(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint16_t *data_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_data_flags";

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
	if( data_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data flags.",
		 function );

		return( -1 );
	}
	*data_flags = mft_attribute->data_flags;

	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_data_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_data_size";

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
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = mft_attribute->data_size;

	return( 1 );
}

/* Retrieves the data VCN range
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_mft_attribute_get_data_vcn_range(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint64_t *data_first_vcn,
     uint64_t *data_last_vcn,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_data_vcn_range";

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
	if( data_first_vcn == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data first VCN.",
		 function );

		return( -1 );
	}
	if( data_last_vcn == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data last VCN.",
		 function );

		return( -1 );
	}
	if( ( mft_attribute->non_resident_flag & 0x01 ) != 0 )
	{
		*data_first_vcn = mft_attribute->data_first_vcn;
		*data_last_vcn  = mft_attribute->data_last_vcn;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves the allocated data size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_allocated_data_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint64_t *allocated_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_allocated_data_size";

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
	if( allocated_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocated data size.",
		 function );

		return( -1 );
	}
	*allocated_data_size = mft_attribute->allocated_data_size;

	return( 1 );
}

/* Retrieves the valid data size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_valid_data_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint64_t *valid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_valid_data_size";

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
	if( valid_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid valid data size.",
		 function );

		return( -1 );
	}
	*valid_data_size = mft_attribute->valid_data_size;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_utf8_name_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_utf8_name_size";

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
	if( ( mft_attribute->name == NULL )
	 || ( mft_attribute->name_size == 0 ) )
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
		     mft_attribute->name,
		     (size_t) mft_attribute->name_size,
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
int libfsntfs_mft_attribute_get_utf8_name(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_utf8_name";

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
	if( mft_attribute->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT attribute - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     mft_attribute->name,
	     (size_t) mft_attribute->name_size,
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
int libfsntfs_mft_attribute_get_utf16_name_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_utf16_name_size";

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
	if( ( mft_attribute->name == NULL )
	 || ( mft_attribute->name_size == 0 ) )
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
		     mft_attribute->name,
		     (size_t) mft_attribute->name_size,
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
int libfsntfs_mft_attribute_get_utf16_name(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_utf16_name";

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
	if( mft_attribute->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT attribute - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     mft_attribute->name,
	     (size_t) mft_attribute->name_size,
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
int libfsntfs_mft_attribute_compare_name_with_utf8_string(
     libfsntfs_mft_attribute_t *mft_attribute,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_compare_name_with_utf8_string";
	int result            = 0;

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
	if( mft_attribute->name == NULL )
	{
		return( 0 );
	}
	result = libfsntfs_name_compare_with_utf8_string(
	          mft_attribute->name,
	          mft_attribute->name_size,
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
int libfsntfs_mft_attribute_compare_name_with_utf16_string(
     libfsntfs_mft_attribute_t *mft_attribute,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_compare_name_with_utf16_string";
	int result            = 0;

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
	if( mft_attribute->name == NULL )
	{
		return( 0 );
	}
	result = libfsntfs_name_compare_with_utf16_string(
	          mft_attribute->name,
	          mft_attribute->name_size,
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

/* Retrieves the compression unit size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_compression_unit_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     size_t *compression_unit_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_compression_unit_size";

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
	if( compression_unit_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression unit size.",
		 function );

		return( -1 );
	}
	*compression_unit_size = mft_attribute->compression_unit_size;

	return( 1 );
}

/* Retrieves the resident data
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_resident_data(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint8_t **resident_data,
     size_t *resident_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_resident_data";

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
	if( ( mft_attribute->non_resident_flag & 0x01 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid MFT attribute - non-resident flag is set.",
		 function );

		return( -1 );
	}
	if( mft_attribute->data_size > (uint64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT attribute - data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( resident_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resident data.",
		 function );

		return( -1 );
	}
	if( resident_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resident data size.",
		 function );

		return( -1 );
	}
	*resident_data      = mft_attribute->data;
	*resident_data_size = (size_t) mft_attribute->data_size;

	return( 1 );
}

/* Retrieves the number of data runs
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_number_of_data_runs(
     libfsntfs_mft_attribute_t *mft_attribute,
     int *number_of_data_runs,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_number_of_data_runs";

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
	if( mft_attribute->data_runs_array == NULL )
	{
		if( number_of_data_runs == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of data runs.",
			 function );

			return( -1 );
		}
		*number_of_data_runs = 0;
	}
	else
	{
		if( libcdata_array_get_number_of_entries(
		     mft_attribute->data_runs_array,
		     number_of_data_runs,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of data runs.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves a specific data run
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_data_run_by_index(
     libfsntfs_mft_attribute_t *mft_attribute,
     int data_run_index,
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_data_run_by_index";

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
	if( libcdata_array_get_entry_by_index(
	     mft_attribute->data_runs_array,
	     data_run_index,
	     (intptr_t **) data_run,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data run: %d.",
		 function,
		 data_run_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data extents array
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_data_extents_array(
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_io_handle_t *io_handle,
     libcdata_array_t **data_extents_array,
     libcerror_error_t **error )
{
	libcdata_array_t *safe_data_extents_array    = NULL;
	libfsntfs_data_run_t *data_run               = NULL;
	libfsntfs_extent_t *data_extent              = NULL;
	static char *function                        = "libfsntfs_mft_attribute_get_data_extents_array";
	size64_t attribute_data_vcn_size             = 0;
	size64_t calculated_allocated_data_size      = 0;
	size64_t stored_allocated_data_size          = 0;
	off64_t attribute_data_vcn_offset            = 0;
	off64_t calculated_attribute_data_vcn_offset = 0;
	int attribute_index                          = 0;
	int data_run_index                           = 0;
	int entry_index                              = 0;
	int number_of_data_runs                      = 0;

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
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->cluster_block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - cluster block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( data_extents_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data extents array.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_allocated_data_size(
	     mft_attribute,
	     &stored_allocated_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute allocated data size.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &safe_data_extents_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extents array.",
		 function );

		goto on_error;
	}
	while( mft_attribute != NULL )
	{
		if( mft_attribute->data_runs_array != NULL )
		{
			attribute_data_vcn_offset = mft_attribute->data_first_vcn;
			attribute_data_vcn_size   = mft_attribute->data_last_vcn;

			if( attribute_data_vcn_size != 0xffffffffffffffffULL )
			{
				if( (uint64_t) attribute_data_vcn_offset > (uint64_t) ( ( INT64_MAX / io_handle->cluster_block_size ) - 1 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid attribute data first VCN value out of bounds.",
					 function );

					goto on_error;
				}
				if( attribute_data_vcn_size > (size64_t) ( ( INT64_MAX / io_handle->cluster_block_size ) - 1 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid attribute data last VCN value out of bounds.",
					 function );

					goto on_error;
				}
				if( attribute_data_vcn_offset > (off64_t) attribute_data_vcn_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid attribute data first VCN value out of bounds.",
					 function );

					goto on_error;
				}
				attribute_data_vcn_size   += 1;
				attribute_data_vcn_size   -= attribute_data_vcn_offset;
				attribute_data_vcn_offset *= io_handle->cluster_block_size;
				attribute_data_vcn_size   *= io_handle->cluster_block_size;

				if( ( calculated_attribute_data_vcn_offset != 0 )
				 && ( calculated_attribute_data_vcn_offset != attribute_data_vcn_offset ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid attribute data VCN offset value out of bounds.",
					 function );

					goto on_error;
				}
				calculated_attribute_data_vcn_offset = attribute_data_vcn_offset + (off64_t) attribute_data_vcn_size;
			}
			if( libcdata_array_get_number_of_entries(
			     mft_attribute->data_runs_array,
			     &number_of_data_runs,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d number of data runs.",
				 function,
				 attribute_index );

				goto on_error;
			}
			for( data_run_index = 0;
			     data_run_index < number_of_data_runs;
			     data_run_index++ )
			{
				if( libcdata_array_get_entry_by_index(
				     mft_attribute->data_runs_array,
				     data_run_index,
				     (intptr_t **) &data_run,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve attribute: %d data run: %d.",
					 function,
					 attribute_index,
					 data_run_index );

					goto on_error;
				}
				if( data_run == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing attribute: %d data run: %d.",
					 function,
					 attribute_index,
					 data_run_index );

					goto on_error;
				}
				if( libfsntfs_extent_initialize(
				     &data_extent,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create data extent: %d.",
					 function,
					 data_run_index );

					goto on_error;
				}
				data_extent->start_offset = data_run->start_offset;
				data_extent->size         = data_run->size;
				data_extent->range_flags  = 0;

				if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
				{
					data_extent->range_flags |= LIBFSNTFS_EXTENT_FLAG_IS_SPARSE;
				}
				if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
				{
					data_extent->range_flags |= LIBFSNTFS_EXTENT_FLAG_IS_COMPRESSED;
				}
				if( libcdata_array_append_entry(
				     safe_data_extents_array,
				     &entry_index,
				     (intptr_t *) data_extent,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append data extent: %d to array.",
					 function,
					 data_run_index );

					goto on_error;
				}
				calculated_allocated_data_size += data_run->size;

				data_extent = NULL;
			}
		}
		attribute_index++;

		if( libfsntfs_mft_attribute_get_next_attribute(
		     mft_attribute,
		     &mft_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next MFT attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
	}
	if( calculated_allocated_data_size != stored_allocated_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: size of data runs: %" PRIu64 " does not match allocated data size: %" PRIu64 ".",
		 function,
		 calculated_allocated_data_size,
		 stored_allocated_data_size );

		goto on_error;
	}
	*data_extents_array = safe_data_extents_array;

	return( 1 );

on_error:
	if( data_extent != NULL )
	{
		libfsntfs_extent_free(
		 &data_extent,
		 NULL );
	}
	if( safe_data_extents_array != NULL )
	{
		libcdata_array_free(
		 &safe_data_extents_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_extent_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the next attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_get_next_attribute(
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_mft_attribute_t **next_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_get_next_attribute";

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
	if( next_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next attribute.",
		 function );

		return( -1 );
	}
	*next_attribute = mft_attribute->next_attribute;

	return( 1 );
}

/* Appends the attribute to the attribute chain
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_append_to_chain(
     libfsntfs_mft_attribute_t **first_attribute,
     libfsntfs_mft_attribute_t *additional_attribute,
     libcerror_error_t **error )
{
	libfsntfs_mft_attribute_t *mft_attribute      = NULL;
	libfsntfs_mft_attribute_t *previous_attribute = NULL;
	static char *function                         = "libfsntfs_mft_attribute_append_to_chain";

	if( first_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first attribute.",
		 function );

		return( -1 );
	}
	if( additional_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid additional attribute.",
		 function );

		return( -1 );
	}
	mft_attribute = *first_attribute;

	if( mft_attribute != NULL )
	{
		if( mft_attribute->type != additional_attribute->type )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: unable to chain attributes of different types.",
			 function );

			return( -1 );
		}
	}
	while( mft_attribute != NULL )
	{
		if( mft_attribute == additional_attribute )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chained attribute value out of bounds.",
			 function );

			return( -1 );
		}
		if( mft_attribute->data_first_vcn > additional_attribute->data_first_vcn )
		{
			break;
		}
		previous_attribute = mft_attribute;
		mft_attribute      = mft_attribute->next_attribute;
	}
	if( previous_attribute == NULL )
	{
		additional_attribute->next_attribute = mft_attribute;

		*first_attribute = additional_attribute;
	}
	else
	{
		if( previous_attribute->next_attribute != NULL )
		{
			additional_attribute->next_attribute = previous_attribute->next_attribute;
		}
		previous_attribute->next_attribute = additional_attribute;
	}
	return( 1 );
}

