/*
 * Attribute functions
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

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libfsntfs_attribute.h"
#include "libfsntfs_bitmap_values.h"
#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_cluster_block_vector.h"
#include "libfsntfs_data_run.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_logged_utility_stream_values.h"
#include "libfsntfs_object_identifier_values.h"
#include "libfsntfs_reparse_point_values.h"
#include "libfsntfs_security_descriptor_values.h"
#include "libfsntfs_standard_information_values.h"
#include "libfsntfs_txf_data_values.h"
#include "libfsntfs_unused.h"
#include "libfsntfs_volume_information_values.h"
#include "libfsntfs_volume_name_values.h"

#include "fsntfs_attribute_list.h"
#include "fsntfs_mft_attribute.h"

/* Creates an attribute
 * Make sure the value attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_initialize(
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_initialize";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( *attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute value already set.",
		 function );

		return( -1 );
	}
	internal_attribute = memory_allocate_structure(
	                      libfsntfs_internal_attribute_t );

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_attribute,
	     0,
	     sizeof( libfsntfs_internal_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute.",
		 function );

		memory_free(
		 internal_attribute );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_attribute->data_runs_array ),
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
	*attribute = (libfsntfs_attribute_t *) internal_attribute;

	return( 1 );

on_error:
	if( internal_attribute != NULL )
	{
		memory_free(
		 internal_attribute );
	}
	return( -1 );
}

/* Frees an attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_free(
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_attribute_free";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( *attribute != NULL )
	{
		*attribute = NULL;
	}
	return( 1 );
}

/* Frees an attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_attribute_free(
     libfsntfs_internal_attribute_t **internal_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_attribute_free";
	int result            = 1;

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( *internal_attribute != NULL )
	{
		if( ( *internal_attribute )->name != NULL )
		{
			memory_free(
			 ( *internal_attribute )->name );
		}
		if( ( *internal_attribute )->data != NULL )
		{
			memory_free(
			 ( *internal_attribute )->data );
		}
		if( libcdata_array_free(
		     &( ( *internal_attribute )->data_runs_array ),
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
		if( ( *internal_attribute )->value != NULL )
		{
			if( ( *internal_attribute )->free_value != NULL )
			{
				if( ( *internal_attribute )->free_value(
				     &( ( *internal_attribute )->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free attribute value.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 *internal_attribute );

		*internal_attribute = NULL;
	}
	return( result );
}

/* Compares attributes by their file reference
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_attribute_compare_by_file_reference(
     libfsntfs_internal_attribute_t *first_attribute,
     libfsntfs_internal_attribute_t *second_attribute,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_attribute_compare_by_file_reference";
	uint64_t first_mft_entry_index  = 0;
	uint64_t second_mft_entry_index = 0;

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
	if( second_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second attribute.",
		 function );

		return( -1 );
	}
	first_mft_entry_index  = first_attribute->file_reference & 0xffffffffffffUL;
	second_mft_entry_index = second_attribute->file_reference & 0xffffffffffffUL;

	if( first_mft_entry_index < second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_mft_entry_index > second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Reads the attribute from the MFT attributes data in a MFT entry
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libfsntfs_attribute_read_from_mft(
         libfsntfs_attribute_t *attribute,
         libfsntfs_io_handle_t *io_handle,
         uint8_t *mft_entry_data,
         size_t mft_entry_data_size,
         size_t mft_attribute_data_offset,
         uint8_t flags,
         libcerror_error_t **error )
{
	libfsntfs_data_run_t *data_run                     = NULL;
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	uint8_t *mft_attribute_data_run_data               = NULL;
	uint8_t *mft_attribute_header_data                 = NULL;
	uint8_t *mft_attribute_non_resident_data           = NULL;
	uint8_t *mft_attribute_resident_data               = NULL;
	static char *function                              = "libfsntfs_attribute_read_from_mft";
	size64_t data_run_size                             = 0;
	size_t header_data_size                            = 0;
	size_t non_resident_data_size                      = 0;
	size_t unknown_data_size                           = 0;
	off64_t data_block_offset                          = 0;
	off64_t data_run_offset                            = 0;
	uint64_t data_run_number_of_cluster_blocks         = 0;
	uint64_t last_data_run_cluster_block_number        = 0;
	uint32_t attribute_data_size                       = 0;
	uint32_t range_flags                               = 0;
	uint16_t attribute_data_offset                     = 0;
	uint16_t attribute_name_offset                     = 0;
	uint16_t compression_unit_size                     = 0;
	uint16_t data_runs_offset                          = 0;
	uint8_t data_run_value_index                       = 0;
	uint8_t data_run_value_size                        = 0;
	uint8_t data_run_value_size_tuple                  = 0;
	uint8_t non_resident_flag                          = 0;
	int64_t data_run_cluster_block_number              = 0;
	int data_run_index                                 = 0;
	int entry_index                                    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t *value_string                   = NULL;
	size_t value_string_size                           = 0;
	uint64_t value_64bit                               = 0;
	uint32_t value_32bit                               = 0;
	int result                                         = 0;
#endif

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
	if( mft_entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry data.",
		 function );

		return( -1 );
	}
	if( mft_entry_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: MFT entry data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( mft_attribute_data_offset > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: MFT attribute data offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( mft_attribute_data_offset >= mft_entry_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: MFT attribute data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( mft_entry_data_size < sizeof( fsntfs_mft_attribute_header_t ) )
	 || ( mft_attribute_data_offset > ( mft_entry_data_size - sizeof( fsntfs_mft_attribute_header_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MFT attribute data size value too small.",
		 function );

		return( -1 );
	}
	mft_attribute_header_data = &( mft_entry_data[ mft_attribute_data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MFT attribute header data:\n",
		 function );
		libcnotify_print_data(
		 mft_attribute_header_data,
		 sizeof( fsntfs_mft_attribute_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_mft_attribute_header_t *) mft_attribute_header_data )->type,
	 internal_attribute->type );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 internal_attribute->type,
		 libfsntfs_debug_print_attribute_type(
		  internal_attribute->type ) );
	}
#endif
	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_END_OF_ATTRIBUTES )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_mft_attribute_header_t *) mft_attribute_header_data )->size,
		 internal_attribute->size );

		non_resident_flag = ( (fsntfs_mft_attribute_header_t *) mft_attribute_header_data )->non_resident_flag;

		internal_attribute->name_size = (uint16_t) ( (fsntfs_mft_attribute_header_t *) mft_attribute_header_data )->name_size;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_header_t *) mft_attribute_header_data )->name_offset,
		 attribute_name_offset );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_header_t *) mft_attribute_header_data )->data_flags,
		 internal_attribute->data_flags );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_attribute_header_t *) mft_attribute_header_data )->identifier,
		 internal_attribute->identifier );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
			 function,
			 internal_attribute->size );

			libcnotify_printf(
			 "%s: non resident flag\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 non_resident_flag );

			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu16 "\n",
			 function,
			 internal_attribute->name_size );

			libcnotify_printf(
			 "%s: name offset\t\t\t\t: %" PRIu16 "\n",
			 function,
			 attribute_name_offset );

			libcnotify_printf(
			 "%s: data flags\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 internal_attribute->data_flags );
			libfsntfs_debug_print_mft_attribute_data_flags(
			 internal_attribute->data_flags );
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "%s: identifier\t\t\t\t: %" PRIu16 "\n",
			 function,
			 internal_attribute->identifier );

			libcnotify_printf(
			 "\n" );
		}
#endif
/* TODO handle this more elegant */
		internal_attribute->size &= 0x0000ffffUL;

		if( ( mft_attribute_data_offset + internal_attribute->size ) > mft_entry_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: attribute size value too small.",
			 function );

			goto on_error;
		}
		if( internal_attribute->name_size > 0 )
		{
			if( ( mft_attribute_data_offset + attribute_name_offset ) >= mft_entry_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: attribute name offset value out of bounds.",
				 function );

				goto on_error;
			}
		}
		if( ( internal_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
		{
			if( ( internal_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0x0001 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported compression flags: 0x%04" PRIx16 ".",
				 function,
				 internal_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK );

				goto on_error;
			}
			if( io_handle->cluster_block_size > 4096 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported compression flags: 0x%04" PRIx16 " for volume with cluster block size: %" PRIzd ".",
				 function,
				 internal_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK,
				 io_handle->cluster_block_size );

				goto on_error;
			}
		}
		mft_attribute_data_offset += sizeof( fsntfs_mft_attribute_header_t );

		if( ( non_resident_flag & 0x01 ) == 0 )
		{
			if( ( mft_attribute_data_offset + sizeof( fsntfs_mft_attribute_resident_t ) ) > mft_entry_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: MFT attribute data size value too small.",
				 function );

				goto on_error;
			}
			mft_attribute_resident_data = &( mft_entry_data[ mft_attribute_data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: resident attribute data:\n",
				 function );
				libcnotify_print_data(
				 mft_attribute_resident_data,
				 sizeof( fsntfs_mft_attribute_resident_t ),
				 0 );
			}
#endif
			byte_stream_copy_to_uint32_little_endian(
			 ( (fsntfs_mft_attribute_resident_t *) mft_attribute_resident_data )->data_size,
			 attribute_data_size );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsntfs_mft_attribute_resident_t *) mft_attribute_resident_data )->data_offset,
			 attribute_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: data size\t\t\t\t: %" PRIu32 "\n",
				 function,
				 attribute_data_size );

				libcnotify_printf(
				 "%s: data offset\t\t\t\t: %" PRIu16 "\n",
				 function,
				 attribute_data_offset );

				libcnotify_printf(
				 "%s: indexed flag\t\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 ( (fsntfs_mft_attribute_resident_t *) mft_attribute_resident_data )->indexed_flag );

				libcnotify_printf(
				 "%s: padding\t\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 ( (fsntfs_mft_attribute_resident_t *) mft_attribute_resident_data )->padding );

				libcnotify_printf(
				 "\n" );
			}
#endif
			if( attribute_data_offset > 0 )
			{
				header_data_size = sizeof( fsntfs_mft_attribute_header_t )
				                 + sizeof( fsntfs_mft_attribute_resident_t );

				if( ( attribute_data_offset < header_data_size )
				 || ( ( mft_attribute_data_offset - sizeof( fsntfs_mft_attribute_header_t ) ) >= ( mft_entry_data_size - header_data_size ) ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: data offset value out of bounds.",
					 function );

					goto on_error;
				}
				attribute_data_offset += mft_attribute_data_offset - sizeof( fsntfs_mft_attribute_header_t );
			}
			mft_attribute_data_offset += sizeof( fsntfs_mft_attribute_resident_t );

			internal_attribute->allocated_data_size = (size64_t) attribute_data_size;
			internal_attribute->data_size           = (size64_t) attribute_data_size;
			internal_attribute->is_resident         = 1;
		}
		else
		{
			if( ( mft_attribute_data_offset + sizeof( fsntfs_mft_attribute_non_resident_t ) ) > mft_entry_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: MFT attribute data size value too small.",
				 function );

				goto on_error;
			}
			mft_attribute_non_resident_data = &( mft_entry_data[ mft_attribute_data_offset ] );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsntfs_mft_attribute_non_resident_t *) mft_attribute_non_resident_data )->data_first_vcn,
			 internal_attribute->data_first_vcn );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsntfs_mft_attribute_non_resident_t *) mft_attribute_non_resident_data )->data_last_vcn,
			 internal_attribute->data_last_vcn );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsntfs_mft_attribute_non_resident_t *) mft_attribute_non_resident_data )->data_runs_offset,
			 data_runs_offset );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsntfs_mft_attribute_non_resident_t *) mft_attribute_non_resident_data )->compression_unit_size,
			 compression_unit_size );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsntfs_mft_attribute_non_resident_t *) mft_attribute_non_resident_data )->allocated_data_size,
			 internal_attribute->allocated_data_size );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsntfs_mft_attribute_non_resident_t *) mft_attribute_non_resident_data )->data_size,
			 internal_attribute->data_size );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsntfs_mft_attribute_non_resident_t *) mft_attribute_non_resident_data )->valid_data_size,
			 internal_attribute->valid_data_size );

			if( compression_unit_size == 0 )
			{
				non_resident_data_size += sizeof( fsntfs_mft_attribute_non_resident_t );
			}
			else
			{
				non_resident_data_size += sizeof( fsntfs_mft_attribute_non_resident_compressed_t );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: non-resident attribute data:\n",
				 function );
				libcnotify_print_data(
				 mft_attribute_non_resident_data,
				 non_resident_data_size,
				 0 );
			}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: data first VCN\t\t\t: %" PRIi64 "\n",
				 function,
				 (int64_t) internal_attribute->data_first_vcn );

				libcnotify_printf(
				 "%s: data last VCN\t\t\t: %" PRIi64 "\n",
				 function,
				 (int64_t) internal_attribute->data_last_vcn );

				libcnotify_printf(
				 "%s: data runs offset\t\t\t: 0x%04" PRIx16 "\n",
				 function,
				 data_runs_offset );

				libcnotify_printf(
				 "%s: compression unit size\t\t: %" PRIu16 " (%" PRIzd ")\n",
				 function,
				 compression_unit_size,
				 internal_attribute->compression_unit_size );

				byte_stream_copy_to_uint32_little_endian(
				 ( (fsntfs_mft_attribute_non_resident_t *) mft_attribute_non_resident_data )->padding,
				 value_32bit );
				libcnotify_printf(
				 "%s: padding\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 value_32bit );

				libcnotify_printf(
				 "%s: allocated data size\t\t\t: %" PRIu64 "\n",
				 function,
				 internal_attribute->allocated_data_size );

				libcnotify_printf(
				 "%s: data size\t\t\t\t: %" PRIu64 "\n",
				 function,
				 internal_attribute->data_size );

				libcnotify_printf(
				 "%s: valid data size\t\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
				 function,
				 internal_attribute->valid_data_size,
				 internal_attribute->valid_data_size );

				if( compression_unit_size > 0 )
				{
					byte_stream_copy_to_uint64_little_endian(
					 ( (fsntfs_mft_attribute_non_resident_compressed_t *) mft_attribute_non_resident_data )->total_data_size,
					 value_64bit );
					libcnotify_printf(
					 "%s: total data size\t\t\t: %" PRIu64 "\n",
					 function,
					 value_64bit );
				}
				libcnotify_printf(
				 "\n" );
			}
#endif
			if( internal_attribute->valid_data_size > internal_attribute->data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: attribute valid data size value out of bounds.",
				 function );

				goto on_error;
			}
			if( ( internal_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: data is flagged as compressed but no compression unit size set.\n",
					 function );
				}
#endif
				internal_attribute->compression_unit_size = 16 * io_handle->cluster_block_size;
			}
			else if( compression_unit_size != 0 )
			{
/* TODO add bounds checks */
				/* The size is calculated as: 2 ^ value
				 */
				internal_attribute->compression_unit_size  = (size_t) 1 << compression_unit_size;
				internal_attribute->compression_unit_size *= io_handle->cluster_block_size;
			}
			if( data_runs_offset > 0 )
			{
				header_data_size = sizeof( fsntfs_mft_attribute_header_t ) + non_resident_data_size;

				if( ( data_runs_offset < header_data_size )
				 || ( ( mft_attribute_data_offset - sizeof( fsntfs_mft_attribute_header_t ) ) >= ( mft_entry_data_size - header_data_size ) ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: data runs offset value out of bounds.",
					 function );

					goto on_error;
				}
				data_runs_offset += mft_attribute_data_offset - sizeof( fsntfs_mft_attribute_header_t );
			}
			if( compression_unit_size == 0 )
			{
				mft_attribute_data_offset += sizeof( fsntfs_mft_attribute_non_resident_t );
			}
			else
			{
				mft_attribute_data_offset += sizeof( fsntfs_mft_attribute_non_resident_compressed_t );
			}
			internal_attribute->is_resident = 0;
		}
		if( internal_attribute->name_size > 0 )
		{
			if( mft_attribute_data_offset < attribute_name_offset )
			{
				unknown_data_size = (size_t) attribute_name_offset - mft_attribute_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unknown data:\n",
					 function );
					libcnotify_print_data(
					 &( mft_entry_data[ mft_attribute_data_offset ] ),
					 unknown_data_size,
					 0 );
				}
#endif
				mft_attribute_data_offset += unknown_data_size;
			}
			internal_attribute->name_size *= 2;

			if( ( mft_attribute_data_offset + internal_attribute->name_size ) > mft_entry_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: MFT attribute data size value too small.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: name data:\n",
				 function );
				libcnotify_print_data(
				 &( mft_entry_data[ mft_attribute_data_offset ] ),
				 (size_t) internal_attribute->name_size,
				 0 );
			}
#endif
			internal_attribute->name = (uint8_t *) memory_allocate(
			                                        sizeof( uint8_t ) * (size_t) internal_attribute->name_size );

			if( internal_attribute->name == NULL )
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
			     internal_attribute->name,
			     &( mft_entry_data[ mft_attribute_data_offset ] ),
			     (size_t) internal_attribute->name_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy attribute name.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libuna_utf16_string_size_from_utf16_stream(
					  internal_attribute->name,
					  (size_t) internal_attribute->name_size,
					  LIBUNA_ENDIAN_LITTLE,
					  &value_string_size,
					  error );
#else
				result = libuna_utf8_string_size_from_utf16_stream(
					  internal_attribute->name,
					  (size_t) internal_attribute->name_size,
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
					  internal_attribute->name,
					  (size_t) internal_attribute->name_size,
					  LIBUNA_ENDIAN_LITTLE,
					  error );
#else
				result = libuna_utf8_string_copy_from_utf16_stream(
					  (libuna_utf8_character_t *) value_string,
					  value_string_size,
					  internal_attribute->name,
					  (size_t) internal_attribute->name_size,
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

				value_string = NULL;

				libcnotify_printf(
				 "\n" );
			}
#endif
			mft_attribute_data_offset += (size_t) internal_attribute->name_size;
		}
		if( data_runs_offset > 0 )
		{
			if( mft_attribute_data_offset < data_runs_offset )
			{
				unknown_data_size = (size_t) data_runs_offset - mft_attribute_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unknown data:\n",
					 function );
					libcnotify_print_data(
					 &( mft_entry_data[ mft_attribute_data_offset ] ),
					 unknown_data_size,
					 0 );
				}
#endif
				mft_attribute_data_offset += unknown_data_size;
			}
			mft_attribute_data_run_data = &( mft_entry_data[ mft_attribute_data_offset ] );

			while( *mft_attribute_data_run_data != 0 )
			{
				if( ( mft_attribute_data_offset + 1 ) > mft_entry_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: MFT attribute data size value too small.",
					 function );

					goto on_error;
				}
				data_run_value_size_tuple = *mft_attribute_data_run_data;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: data run: %02d data:\n",
					 function,
					 data_run_index );
					libcnotify_print_data(
					 mft_attribute_data_run_data,
					 (size_t) ( 1 + ( data_run_value_size_tuple & 0x0f ) + ( ( data_run_value_size_tuple >> 4 ) & 0x0f ) ),
					 0 );
				}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: data run: %02d value sizes\t\t\t: %" PRIu8 ", %" PRIu8 "\n",
					 function,
					 data_run_index,
					 data_run_value_size_tuple & 0x0f,
					 ( data_run_value_size_tuple >> 4 ) & 0x0f );
				}
#endif
				mft_attribute_data_run_data += 1;
				mft_attribute_data_offset   += 1;

				data_run_number_of_cluster_blocks = 0;
				range_flags                       = 0;

				data_run_value_size = data_run_value_size_tuple & 0x0f;

/* TODO determine if is this a corrupt data run */
				/* Determine the number of cluster blocks value */
				if( data_run_value_size == 0 )
				{
					/* A empty number of cluster blocks value size seems to also
					 * depict the end of the data runs. The data runs after do not
					 * seem to make sense.
					 */
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: detected empty number of cluster blocks value size.\n",
						 function );
					}
#endif
					break;
                                }
				if( ( mft_attribute_data_offset >= mft_entry_data_size )
				 || ( data_run_value_size > ( mft_entry_data_size - mft_attribute_data_offset ) ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: MFT attribute data size value too small.",
					 function );

					goto on_error;
				}
				for( data_run_value_index = data_run_value_size;
				     data_run_value_index > 0;
				     data_run_value_index-- )
				{
					data_run_number_of_cluster_blocks <<= 8;
					data_run_number_of_cluster_blocks  |= mft_attribute_data_run_data[ data_run_value_index - 1 ];
				}
				mft_attribute_data_run_data += data_run_value_size;
				mft_attribute_data_offset   += data_run_value_size;

				/* Determine the cluster block number value */
				data_run_value_size = ( data_run_value_size_tuple >> 4 ) & 0x0f;

				if( data_run_value_size == 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( ( ( internal_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) == 0 )
						 && ( ( internal_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_SPARSE ) == 0 ) )
						{
							libcnotify_printf(
							 "%s: data run is sparse but no flags set.\n",
							 function );
						}
					}
#endif
/* TODO: assuming a data run is sparse even if LIBFSNTFS_ATTRIBUTE_FLAG_SPARSE or LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK is not set ? */
					range_flags |= LIBFDATA_RANGE_FLAG_IS_SPARSE;
				}
				else
				{
					if( ( mft_attribute_data_offset >= mft_entry_data_size )
					 || ( data_run_value_size > ( mft_entry_data_size - mft_attribute_data_offset ) ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: MFT data run value size value out of bounds.",
						 function );

						goto on_error;
					}
					if( ( last_data_run_cluster_block_number != 0 )
					 && ( ( mft_attribute_data_run_data[ data_run_value_size - 1 ] & 0x80 ) != 0 ) )
					{
						data_run_cluster_block_number = -1;
					}
					else
					{
						data_run_cluster_block_number = 0;
					}
					for( data_run_value_index = data_run_value_size;
					     data_run_value_index > 0;
					     data_run_value_index-- )
					{
						data_run_cluster_block_number <<= 8;
						data_run_cluster_block_number  |= mft_attribute_data_run_data[ data_run_value_index - 1 ];
					}
					mft_attribute_data_run_data += data_run_value_size;
					mft_attribute_data_offset   += data_run_value_size;
				}
				if( data_run_value_size == 0 )
				{
					data_run_offset = 0;
				}
				else
				{
					last_data_run_cluster_block_number += data_run_cluster_block_number;

					data_run_offset = (off64_t) ( last_data_run_cluster_block_number * io_handle->cluster_block_size );
				}
				data_run_size = (size64_t) ( data_run_number_of_cluster_blocks * io_handle->cluster_block_size );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: data run: %02d number of cluster blocks\t: %" PRIu64 " (size: %" PRIu64 ")\n",
					 function,
					 data_run_index,
					 data_run_number_of_cluster_blocks,
					 data_run_size );

					libcnotify_printf(
					 "%s: data run: %02d cluster block number\t\t: %" PRIu64 " (%" PRIi64 ") (offset: 0x%08" PRIx64 ")\n",
					 function,
					 data_run_index,
					 last_data_run_cluster_block_number,
					 data_run_cluster_block_number,
					 data_run_offset );

					if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
					{
						libcnotify_printf(
						 "\tIs sparse\n" );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
/* TODO this breaks internal_attribute->data_size is not always larger than data_block_offset
 * seen in multi data attribute MFT

				if( internal_attribute->data_size != 0 )
				{
					if( ( data_block_offset + data_run_size ) >= internal_attribute->data_size )
					{
						data_run_size = internal_attribute->data_size - data_block_offset;
					}
				}
*/
				data_block_offset += data_run_size;

				if( libfsntfs_data_run_initialize(
				     &data_run,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create data run.",
					 function );

					goto on_error;
				}
				data_run->start_offset = data_run_offset;
				data_run->size         = data_run_size;
				data_run->range_flags  = range_flags;

				if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
				{
					if( libcdata_array_append_entry(
					     internal_attribute->data_runs_array,
					     &entry_index,
					     (intptr_t *) data_run,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append data run to array.",
						 function );

						goto on_error;
					}
				}
				data_run = NULL;

				data_run_index++;
			}
		}
		else
		{
			if( mft_attribute_data_offset < attribute_data_offset )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unknown data:\n",
					 function );
					libcnotify_print_data(
					 &( mft_entry_data[ mft_attribute_data_offset ] ),
					 (size_t) attribute_data_offset - mft_attribute_data_offset,
					 0 );
				}
#endif
				mft_attribute_data_offset = attribute_data_offset;
			}
#if SIZEOF_SIZE_T <= 4
			if( attribute_data_size > (size_t) SSIZE_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: attribute data size value out of bounds.",
				 function );

				goto on_error;
			}
#endif
			internal_attribute->data = (uint8_t *) memory_allocate(
			                                        sizeof( uint8_t ) * attribute_data_size );

			if( internal_attribute->data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create attribute data.",
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
				 &( mft_entry_data[ mft_attribute_data_offset ] ),
				 attribute_data_size,
				 0 );
			}
#endif
			if( memory_copy(
			     internal_attribute->data,
			     &( mft_entry_data[ mft_attribute_data_offset ] ),
			     attribute_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy attribute data.",
				 function );

				goto on_error;
			}
			mft_attribute_data_offset += attribute_data_size;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( mft_attribute_data_offset < internal_attribute->size )
			{
				libcnotify_printf(
				 "%s: trailing data:\n",
				 function );
				libcnotify_print_data(
				 &( mft_entry_data[ mft_attribute_data_offset ] ),
				 (size_t) internal_attribute->size - mft_attribute_data_offset,
				 0 );
			}
		}
#endif
	}
	return( (ssize_t) internal_attribute->size );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
#endif
	if( data_run != NULL )
	{
		libfsntfs_data_run_free(
		 &data_run,
		 NULL );
	}
	if( internal_attribute->name != NULL )
	{
		memory_free(
		 internal_attribute->name );

		internal_attribute->name = NULL;
	}
	internal_attribute->name_size = 0;

	libcdata_array_empty(
	 internal_attribute->data_runs_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_data_run_free,
	 NULL );

	return( -1 );
}

/* Reads the attribute from an attributes list
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libfsntfs_attribute_read_from_list(
         libfsntfs_attribute_t *attribute,
         const uint8_t *data,
         size_t data_size,
         size_t data_offset,
         libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	uint8_t *attribute_list_entry_header_data          = NULL;
	static char *function                              = "libfsntfs_attribute_read_from_list";
	size_t unknown_data_size                           = 0;
	uint8_t attribute_name_offset                      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t *value_string                   = NULL;
	size_t value_string_size                           = 0;
	int result                                         = 0;
#endif

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
		 "%s: data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_offset > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: data offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_offset >= data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsntfs_attribute_list_entry_header_t ) )
	 || ( data_offset > ( data_size - sizeof( fsntfs_attribute_list_entry_header_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data size value too small.",
		 function );

		return( -1 );
	}
	attribute_list_entry_header_data = (uint8_t *) &( data[ data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute list entry header data:\n",
		 function );
		libcnotify_print_data(
		 attribute_list_entry_header_data,
		 sizeof( fsntfs_attribute_list_entry_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_attribute_list_entry_header_t *) attribute_list_entry_header_data )->type,
	 internal_attribute->type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_attribute_list_entry_header_t *) attribute_list_entry_header_data )->size,
	 internal_attribute->size );

	internal_attribute->name_size = (uint16_t) ( (fsntfs_attribute_list_entry_header_t *) attribute_list_entry_header_data )->name_size;

	attribute_name_offset = ( (fsntfs_attribute_list_entry_header_t *) attribute_list_entry_header_data )->name_offset;

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_attribute_list_entry_header_t *) attribute_list_entry_header_data )->data_first_vcn,
	 internal_attribute->data_first_vcn );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_attribute_list_entry_header_t *) attribute_list_entry_header_data )->file_reference,
	 internal_attribute->file_reference );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_attribute_list_entry_header_t *) attribute_list_entry_header_data )->identifier,
	 internal_attribute->identifier );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 internal_attribute->type,
		 libfsntfs_debug_print_attribute_type(
		  internal_attribute->type ) );

		libcnotify_printf(
		 "%s: size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 internal_attribute->size );

		libcnotify_printf(
		 "%s: name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 internal_attribute->name_size );

		libcnotify_printf(
		 "%s: name offset\t\t\t\t: %" PRIu8 "\n",
		 function,
		 attribute_name_offset );

		libcnotify_printf(
		 "%s: data first VCN\t\t\t: %" PRIu64 "\n",
		 function,
		 internal_attribute->data_first_vcn );

		libcnotify_printf(
		 "%s: file reference\t\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 function,
		 internal_attribute->file_reference & 0xffffffffffffUL,
		 internal_attribute->file_reference >> 48 );

		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu16 "\n",
		 function,
		 internal_attribute->identifier );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( ( data_offset + internal_attribute->size ) > data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: attribute size value too small.",
		 function );

		goto on_error;
	}
	if( internal_attribute->name_size > 0 )
	{
		if( ( data_offset + attribute_name_offset ) >= data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: attribute name offset value out of bounds.",
			 function );

			goto on_error;
		}
	}
	data_offset += sizeof( fsntfs_attribute_list_entry_header_t );

	if( internal_attribute->name_size > 0 )
	{
		if( data_offset < attribute_name_offset )
		{
			unknown_data_size = (size_t) attribute_name_offset - data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unknown data:\n",
				 function );
				libcnotify_print_data(
				 &( data[ data_offset ] ),
				 unknown_data_size,
				 0 );
			}
#endif
			data_offset += unknown_data_size;
		}
		internal_attribute->name_size *= 2;

		if( ( data_offset + internal_attribute->name_size ) > data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: attribute list data size value too small.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) internal_attribute->name_size,
			 0 );
		}
#endif
		internal_attribute->name = (uint8_t *) memory_allocate(
		                                        sizeof( uint8_t ) * (size_t) internal_attribute->name_size );

		if( internal_attribute->name == NULL )
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
		     internal_attribute->name,
		     &( data[ data_offset ] ),
		     (size_t) internal_attribute->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy attribute name.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_size_from_utf16_stream(
				  internal_attribute->name,
				  (size_t) internal_attribute->name_size,
				  LIBUNA_ENDIAN_LITTLE,
				  &value_string_size,
				  error );
#else
			result = libuna_utf8_string_size_from_utf16_stream(
				  internal_attribute->name,
				  (size_t) internal_attribute->name_size,
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
				  internal_attribute->name,
				  (size_t) internal_attribute->name_size,
				  LIBUNA_ENDIAN_LITTLE,
				  error );
#else
			result = libuna_utf8_string_copy_from_utf16_stream(
				  (libuna_utf8_character_t *) value_string,
				  value_string_size,
				  internal_attribute->name,
				  (size_t) internal_attribute->name_size,
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
			 "%s: name\t\t\t\t: %" PRIs_SYSTEM "\n",
			 function,
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;

			libcnotify_printf(
			 "\n" );
		}
#endif
		data_offset += (size_t) internal_attribute->name_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < internal_attribute->size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) internal_attribute->size - data_offset,
			 0 );
		}
	}
#endif
	return( (ssize_t) internal_attribute->size );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
#endif
	if( internal_attribute->name != NULL )
	{
		memory_free(
		 internal_attribute->name );

		internal_attribute->name = NULL;
	}
	internal_attribute->name_size = 0;

	return( -1 );
}

/* Reads the attribute value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_read_value(
     libfsntfs_attribute_t *attribute,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfcache_cache_t *cluster_block_cache             = NULL;
	libfdata_stream_t *cluster_block_stream            = NULL;
	libfdata_vector_t *cluster_block_vector            = NULL;
	libfsntfs_cluster_block_t *cluster_block           = NULL;
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_read_value";
	int cluster_block_index                            = 0;
	int number_of_cluster_blocks                       = 0;
	int number_of_data_runs                            = 0;
	int result                                         = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	/* Value already set ignore */
	if( internal_attribute->value != NULL )
	{
		return( 1 );
	}
	if( internal_attribute->is_resident != 0 )
	{
		if( internal_attribute->data_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attribute - data size value out of bounds.",
			 function );

			goto on_error;
		}
		switch( internal_attribute->type )
		{
			case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_bitmap_values_free;

				if( libfsntfs_bitmap_values_initialize(
				     (libfsntfs_bitmap_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create bitmap values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_bitmap_values_read(
				     (libfsntfs_bitmap_values_t *) internal_attribute->value,
				     internal_attribute->data,
				     (size_t) internal_attribute->data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read bitmap values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_file_name_values_free;

				if( libfsntfs_file_name_values_initialize(
				     (libfsntfs_file_name_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create file name values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_file_name_values_read(
				     (libfsntfs_file_name_values_t *) internal_attribute->value,
				     internal_attribute->data,
				     (size_t) internal_attribute->data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read file name values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
				result = libuna_utf8_string_compare_with_utf16_stream(
					  (uint8_t *) "$TXF_DATA",
					  9,
					  internal_attribute->name,
					  internal_attribute->name_size,
					  LIBUNA_ENDIAN_LITTLE,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to compare UTF-8 string with attribute name.",
					 function );

					goto on_error;
				}
				else if( result == 1 )
				{
					internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_txf_data_values_free;

					if( libfsntfs_txf_data_values_initialize(
					     (libfsntfs_txf_data_values_t **) &( internal_attribute->value ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create TxF data values.",
						 function );

						goto on_error;
					}
					if( libfsntfs_txf_data_values_read(
					     (libfsntfs_txf_data_values_t *) internal_attribute->value,
					     internal_attribute->data,
					     (size_t) internal_attribute->data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read TxF data values.",
						 function );

						goto on_error;
					}
				}
				else
				{
					internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_logged_utility_stream_values_free;

					if( libfsntfs_logged_utility_stream_values_initialize(
					     (libfsntfs_logged_utility_stream_values_t **) &( internal_attribute->value ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create logged utility stream values.",
						 function );

						goto on_error;
					}
					if( libfsntfs_logged_utility_stream_values_read(
					     (libfsntfs_logged_utility_stream_values_t *) internal_attribute->value,
					     internal_attribute->data,
					     (size_t) internal_attribute->data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read logged utility stream values.",
						 function );

						goto on_error;
					}
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_object_identifier_values_free;

				if( libfsntfs_object_identifier_values_initialize(
				     (libfsntfs_object_identifier_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create object identifier values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_object_identifier_values_read(
				     (libfsntfs_object_identifier_values_t *) internal_attribute->value,
				     internal_attribute->data,
				     (size_t) internal_attribute->data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read object identifier values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_reparse_point_values_free;

				if( libfsntfs_reparse_point_values_initialize(
				     (libfsntfs_reparse_point_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create reparse point values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_reparse_point_values_read(
				     (libfsntfs_reparse_point_values_t *) internal_attribute->value,
				     internal_attribute->data,
				     (size_t) internal_attribute->data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read reparse point values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_security_descriptor_values_free;

				if( libfsntfs_security_descriptor_values_initialize(
				     (libfsntfs_security_descriptor_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create security descriptor values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_security_descriptor_values_read_buffer(
				     (libfsntfs_security_descriptor_values_t *) internal_attribute->value,
				     internal_attribute->data,
				     (size_t) internal_attribute->data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read security descriptor values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_standard_information_values_free;

				if( libfsntfs_standard_information_values_initialize(
				     (libfsntfs_standard_information_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create standard information values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_standard_information_values_read(
				     (libfsntfs_standard_information_values_t *) internal_attribute->value,
				     internal_attribute->data,
				     (size_t) internal_attribute->data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read standard information values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_volume_information_values_free;

				if( libfsntfs_volume_information_values_initialize(
				     (libfsntfs_volume_information_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create volume information values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_volume_information_values_read(
				     (libfsntfs_volume_information_values_t *) internal_attribute->value,
				     internal_attribute->data,
				     (size_t) internal_attribute->data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read volume information values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_volume_name_values_free;

				if( libfsntfs_volume_name_values_initialize(
				     (libfsntfs_volume_name_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create volume name values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_volume_name_values_read(
				     (libfsntfs_volume_name_values_t *) internal_attribute->value,
				     internal_attribute->data,
				     (size_t) internal_attribute->data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read volume name values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			default:
				break;
		}
	}
	else
	{
		switch( internal_attribute->type )
		{
			case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_bitmap_values_free;

				if( libfsntfs_bitmap_values_initialize(
				     (libfsntfs_bitmap_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create bitmap values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_security_descriptor_values_free;

				if( libfsntfs_security_descriptor_values_initialize(
				     (libfsntfs_security_descriptor_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create security descriptor values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
			case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported non-resident attribute.",
				 function );

				goto on_error;

			case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
			default:
				break;
		}
		if( ( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
		 && ( internal_attribute->value != NULL ) )
		{
			if( ( internal_attribute->data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported compressed attribute data.",
				 function );

				goto on_error;
			}
			if( internal_attribute->type == LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR )
			{
				if( libfsntfs_cluster_block_stream_initialize(
				     &cluster_block_stream,
				     io_handle,
				     attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create cluster block stream.",
					 function );

					goto on_error;
				}
				if( libfsntfs_security_descriptor_values_read_stream(
				     (libfsntfs_security_descriptor_values_t *) internal_attribute->value,
				     file_io_handle,
				     cluster_block_stream,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read security descriptor values from stream.",
					 function );

					goto on_error;
				}
				if( libfdata_stream_free(
				     &cluster_block_stream,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free cluster block stream.",
					 function );

					goto on_error;
				}
			}
			else
			{
				if( libcdata_array_get_number_of_entries(
				     internal_attribute->data_runs_array,
				     &number_of_data_runs,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve number of data runs.",
					 function );

					goto on_error;
				}
				if( libfsntfs_cluster_block_vector_initialize(
				     &cluster_block_vector,
				     io_handle,
				     attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create cluster block vector.",
					 function );

					goto on_error;
				}
				if( libfcache_cache_initialize(
				     &cluster_block_cache,
				     1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create cluster block cache.",
					 function );

					goto on_error;
				}
/* TODO refactor read from vector into bitmap values */
				if( libfdata_vector_get_number_of_elements(
				     cluster_block_vector,
				     &number_of_cluster_blocks,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve number of cluster blocks.",
					 function );

					goto on_error;
				}
				for( cluster_block_index = 0;
				     cluster_block_index < number_of_cluster_blocks;
				     cluster_block_index++ )
				{
					if( libfdata_vector_get_element_value_by_index(
					     cluster_block_vector,
					     (intptr_t *) file_io_handle,
					     cluster_block_cache,
					     cluster_block_index,
					     (intptr_t **) &cluster_block,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve cluster block: %d from vector.",
						 function,
						 cluster_block_index );

						goto on_error;
					}
					if( cluster_block == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing cluster block: %d.",
						 function,
						 cluster_block_index );

						goto on_error;
					}
					if( cluster_block->data == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: invalid cluster block: %d - missing data.",
						 function,
						 cluster_block_index );

						goto on_error;
					}
					switch( internal_attribute->type )
					{
						case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
							if( libfsntfs_bitmap_values_read(
							     (libfsntfs_bitmap_values_t *) internal_attribute->value,
							     cluster_block->data,
							     cluster_block->data_size,
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_IO,
								 LIBCERROR_IO_ERROR_READ_FAILED,
								 "%s: unable to read bitmap values.",
								 function );

								goto on_error;
							}
							break;
					}
				}
				if( libfdata_vector_free(
				     &cluster_block_vector,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free cluster block vector.",
					 function );

					goto on_error;
				}
				if( libfcache_cache_free(
				     &cluster_block_cache,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free cluster block cache.",
					 function );

					goto on_error;
				}
			}
		}
	}
	return( 1 );

on_error:
	if( cluster_block_cache != NULL )
	{
		libfcache_cache_free(
		 &cluster_block_cache,
		 NULL );
	}
	if( cluster_block_vector != NULL )
	{
		libfdata_vector_free(
		 &cluster_block_vector,
		 NULL );
	}
	if( cluster_block_stream != NULL )
	{
		libfdata_stream_free(
		 &cluster_block_stream,
		 NULL );
	}
	if( ( internal_attribute->value != NULL )
	 && ( internal_attribute->free_value != NULL ) )
	{
		internal_attribute->free_value(
		 &( internal_attribute->value ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_type(
     libfsntfs_attribute_t *attribute,
     uint32_t *type,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_type";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
	*type = internal_attribute->type;

	return( 1 );
}

/* Retrieves the data flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_data_flags(
     libfsntfs_attribute_t *attribute,
     uint16_t *data_flags,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data_flags";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
	*data_flags = internal_attribute->data_flags;

	return( 1 );
}

/* Retrieves the value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_value(
     libfsntfs_attribute_t *attribute,
     intptr_t **value,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_value";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	*value = internal_attribute->value;

	return( 1 );
}

/* Determines if the attribute has a name
 * Returns 1 if the attribute has a name , 0 if not or -1 on error
 */
int libfsntfs_attribute_has_name(
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_has_name";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->name != NULL )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_utf8_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_utf8_name_size";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( ( internal_attribute->name == NULL )
	 || ( internal_attribute->name_size == 0 ) )
	{
		if( utf8_name_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 name size.",
			 function );

			return( -1 );
		}
		*utf8_name_size = 0;
	}
	else
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     internal_attribute->name,
		     (size_t) internal_attribute->name_size,
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
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_utf8_name(
     libfsntfs_attribute_t *attribute,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_utf8_name";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_name,
	     utf8_name_size,
	     internal_attribute->name,
	     (size_t) internal_attribute->name_size,
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
int libfsntfs_attribute_get_utf16_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_utf16_name_size";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( ( internal_attribute->name == NULL )
	 || ( internal_attribute->name_size == 0 ) )
	{
		if( utf16_name_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 name size.",
			 function );

			return( -1 );
		}
		*utf16_name_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf16_stream(
		     internal_attribute->name,
		     (size_t) internal_attribute->name_size,
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
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_utf16_name(
     libfsntfs_attribute_t *attribute,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_utf16_name";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_name,
	     utf16_name_size,
	     internal_attribute->name,
	     (size_t) internal_attribute->name_size,
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

/* Retrieves the data VCN range
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_attribute_get_data_vcn_range(
     libfsntfs_attribute_t *attribute,
     uint64_t *data_first_vcn,
     uint64_t *data_last_vcn,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data_vcn_range";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
	if( internal_attribute->is_resident != 0 )
	{
		return( 0 );
	}
	*data_first_vcn = internal_attribute->data_first_vcn;
	*data_last_vcn  = internal_attribute->data_last_vcn;

	return( 1 );
}

/* Retrieves the compression unit size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_compression_unit_size(
     libfsntfs_attribute_t *attribute,
     size_t *compression_unit_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_compression_unit_size";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
	*compression_unit_size = internal_attribute->compression_unit_size;

	return( 1 );
}

/* Retrieves the file references as an MFT entry index and sequence number
 * If the value sequence_number is NULL it will be ignored
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_file_reference(
     libfsntfs_attribute_t *attribute,
     uint64_t *mft_entry_index,
     uint16_t *sequence_number,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_file_reference";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( mft_entry_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry index.",
		 function );

		return( -1 );
	}
	*mft_entry_index = internal_attribute->file_reference & 0xffffffffffffUL;

	if( sequence_number != NULL )
	{
		*sequence_number = (uint16_t) ( internal_attribute->file_reference >> 48 );
	}
	return( 1 );
}

/* Compares the name with an UTF-8 encoded string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libfsntfs_attribute_compare_name_with_utf8_string(
     libfsntfs_attribute_t *attribute,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_compare_name_with_utf8_string";
#ifdef TODO_CASE_INSENSITIVE
	libuna_unicode_character_t name_character          = 0;
	libuna_unicode_character_t string_character        = 0;
	size_t name_index                                  = 0;
	size_t utf8_string_index                           = 0;
#endif
	int result                                         = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#ifdef TODO_CASE_INSENSITIVE
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_attribute->name == NULL )
	{
		return( 0 );
	}
	while( name_index < (size_t) internal_attribute->name_size )
	{
		result = libuna_unicode_character_copy_from_utf16_stream(
			  &name_character,
			  internal_attribute->name,
			  (size_t) internal_attribute->name_size,
			  &name_index,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy attribute name to Unicode character.",
			 function );

			return( -1 );
		}
		if( libuna_unicode_character_copy_from_utf8(
		     &string_character,
		     utf8_string,
		     utf8_string_length,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-8 string to Unicode character.",
			 function );

			return( -1 );
		}
		if( towupper( (wint_t) name_character ) != towupper( (wint_t) string_character ) )
		{
			break;
		}
	}
	if( ( name_index == (size_t) internal_attribute->name_size )
	 && ( utf8_string_index == utf8_string_length ) )
	{
		return( 1 );
	}
	return( 0 );
#else
	if( internal_attribute->name == NULL )
	{
		return( 0 );
	}
	result = libuna_utf8_string_compare_with_utf16_stream(
	          utf8_string,
	          utf8_string_length,
	          internal_attribute->name,
	          internal_attribute->name_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with attribute name.",
		 function );

		return( -1 );
	}
	return( result );
#endif
}

/* Compares the name with an UTF-16 encoded string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libfsntfs_attribute_compare_name_with_utf16_string(
     libfsntfs_attribute_t *attribute,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_compare_name_with_utf16_string";
#ifdef TODO_CASE_INSENSITIVE
	libuna_unicode_character_t name_character          = 0;
	libuna_unicode_character_t string_character        = 0;
	size_t name_index                                  = 0;
	size_t utf16_string_index                          = 0;
#endif
	int result                                         = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#ifdef TODO_CASE_INSENSITIVE
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_attribute->name == NULL )
	{
		return( 0 );
	}
	while( name_index < (size_t) internal_attribute->name_size )
	{
		result = libuna_unicode_character_copy_from_utf16_stream(
			  &name_character,
			  internal_attribute->name,
			  (size_t) internal_attribute->name_size,
			  &name_index,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy attribute name to Unicode character.",
			 function );

			return( -1 );
		}
		if( libuna_unicode_character_copy_from_utf16(
		     &string_character,
		     utf16_string,
		     utf16_string_length,
		     &utf16_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 string to Unicode character.",
			 function );

			return( -1 );
		}
		if( towupper( (wint_t) name_character ) != towupper( (wint_t) string_character ) )
		{
			break;
		}
	}
	if( ( name_index == (size_t) internal_attribute->name_size )
	 && ( utf16_string_index == utf16_string_length ) )
	{
		return( 1 );
	}
	return( 0 );
#else
	if( internal_attribute->name == NULL )
	{
		return( 0 );
	}
	result = libuna_utf16_string_compare_with_utf16_stream(
	          utf16_string,
	          utf16_string_length,
	          internal_attribute->name,
	          internal_attribute->name_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-16 string with attribute name.",
		 function );

		return( -1 );
	}
	return( result );
#endif
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_data_size(
     libfsntfs_attribute_t *attribute,
     size64_t *data_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data_size";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
	*data_size = internal_attribute->data_size;

	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_data(
     libfsntfs_attribute_t *attribute,
     uint8_t **data,
     size64_t *data_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
	*data      = internal_attribute->data;
	*data_size = internal_attribute->data_size;

	return( 1 );
}

/* Copies the data at a specific offset to the buffer
 * Returns the number of bytes copied if successful or -1 on error
 */
ssize_t libfsntfs_attribute_copy_data(
         libfsntfs_attribute_t *attribute,
         uint8_t *buffer,
         size_t buffer_size,
         off64_t data_offset,
         libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_copy_data";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute - missing data.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( data_offset < 0 )
	 || ( data_offset > (off64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( (size64_t) data_offset >= internal_attribute->data_size )
	{
		return( 0 );
	}
	if( (size64_t) buffer_size > ( internal_attribute->data_size - (size64_t) data_offset ) )
	{
		buffer_size = (size_t) ( internal_attribute->data_size - (size64_t) data_offset );
	}
	if( memory_copy(
	     buffer,
	     &( internal_attribute->data[ data_offset ] ),
	     buffer_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy attribute data.",
		 function );

		return( -1 );
	}
	return( (ssize_t) buffer_size );
}

/* Retrieves the valid data size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_valid_data_size(
     libfsntfs_attribute_t *attribute,
     size64_t *valid_data_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_valid_data_size";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

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
	*valid_data_size = internal_attribute->valid_data_size;

	return( 1 );
}

/* Retrieves the number of data runs
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_number_of_data_runs(
     libfsntfs_attribute_t *attribute,
     int *number_of_data_runs,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_number_of_data_runs";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( libcdata_array_get_number_of_entries(
	     internal_attribute->data_runs_array,
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
	return( 1 );
}

/* Retrieves a specific data run
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_data_run_by_index(
     libfsntfs_attribute_t *attribute,
     int data_run_index,
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data_run_by_index";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( libcdata_array_get_entry_by_index(
	     internal_attribute->data_runs_array,
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

/* Retrieves the chained attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_chained_attribute(
     libfsntfs_attribute_t *attribute,
     libfsntfs_attribute_t **chained_attribute,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_chained_attribute";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( chained_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chained attribute.",
		 function );

		return( -1 );
	}
	*chained_attribute = internal_attribute->next_attribute;

	return( 1 );
}

/* Appends the attribute to the attribute chain
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_append_to_chain(
     libfsntfs_attribute_t **attribute,
     libfsntfs_attribute_t *chained_attribute,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute          = NULL;
	libfsntfs_internal_attribute_t *internal_chained_attribute  = NULL;
	libfsntfs_internal_attribute_t *previous_internal_attribute = NULL;
	static char *function                                       = "libfsntfs_attribute_append_to_chain";

	if( ( attribute == NULL )
	 || ( *attribute == NULL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) *attribute;

	if( chained_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chained_attribute.",
		 function );

		return( -1 );
	}
	internal_chained_attribute = (libfsntfs_internal_attribute_t *) chained_attribute;

	if( internal_attribute->type != internal_chained_attribute->type )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unable to chain attributes of different types.",
		 function );

		return( -1 );
	}
	while( internal_attribute != NULL )
	{
		if( internal_attribute == internal_chained_attribute )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chained attribute value out of bounds.",
			 function );

			return( -1 );
		}
		if( internal_attribute->data_first_vcn > internal_chained_attribute->data_first_vcn )
		{
			break;
		}
		previous_internal_attribute = internal_attribute;
	        internal_attribute          = (libfsntfs_internal_attribute_t *) internal_attribute->next_attribute;
	}
	if( previous_internal_attribute == internal_chained_attribute )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chained attribute value out of bounds.",
		 function );

		return( -1 );
	}
	if( previous_internal_attribute == NULL )
	{
		internal_chained_attribute->next_attribute = *attribute;
		*attribute                                 = chained_attribute;
	}
	else
	{
		if( previous_internal_attribute->next_attribute != NULL )
		{
			internal_chained_attribute->next_attribute = (libfsntfs_attribute_t *) ( previous_internal_attribute->next_attribute );
		}
		previous_internal_attribute->next_attribute = chained_attribute;
	}
	return( 1 );
}

