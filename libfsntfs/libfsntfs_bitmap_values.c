/*
 * Bitmap attribute ($BITMAP) functions
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
#include <types.h>

#include "libfsntfs_bitmap_values.h"
#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_vector.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"

/* Creates bitmap values
 * Make sure the value bitmap_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_bitmap_values_initialize(
     libfsntfs_bitmap_values_t **bitmap_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_bitmap_values_initialize";

	if( bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bitmap values.",
		 function );

		return( -1 );
	}
	if( *bitmap_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid bitmap values value already set.",
		 function );

		return( -1 );
	}
	*bitmap_values = memory_allocate_structure(
	                  libfsntfs_bitmap_values_t );

	if( *bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create bitmap values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *bitmap_values,
	     0,
	     sizeof( libfsntfs_bitmap_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear bitmap values.",
		 function );

		memory_free(
		 *bitmap_values );

		*bitmap_values = NULL;

		return( -1 );
	}
	if( libcdata_range_list_initialize(
	     &( ( *bitmap_values )->allocated_block_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create allocated block list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *bitmap_values != NULL )
	{
		memory_free(
		 *bitmap_values );

		*bitmap_values = NULL;
	}
	return( -1 );
}

/* Frees bitmap values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_bitmap_values_free(
     libfsntfs_bitmap_values_t **bitmap_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_bitmap_values_free";
	int result            = 1;

	if( bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bitmap values.",
		 function );

		return( -1 );
	}
	if( *bitmap_values != NULL )
	{
		if( libcdata_range_list_free(
		     &( ( *bitmap_values )->allocated_block_list ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free allocated data block list.",
			 function );

			result = -1;
		}
		memory_free(
		 *bitmap_values );

		*bitmap_values = NULL;
	}
	return( result );
}

/* Reads the bitmap values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_bitmap_values_read_data(
     libfsntfs_bitmap_values_t *bitmap_values,
     const uint8_t *data,
     size_t data_size,
     size_t element_data_size,
     off64_t *base_offset,
     libcerror_error_t **error )
{
	static char *function             = "libfsntfs_bitmap_values_read_data";
	size64_t allocated_range_size     = 0;
	size_t data_offset                = 0;
	off64_t allocated_range_offset    = 0;
	off64_t safe_base_offset          = 0;
	uint8_t bit_index                 = 0;
	uint8_t byte_value                = 0;
	uint8_t in_allocated_range        = 0;
	int allocated_element_index       = 0;
	int first_allocated_element_index = 0;
	int result                        = 0;

	if( bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bitmap values.",
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
	if( base_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base offset.",
		 function );

		return( -1 );
	}
	safe_base_offset = *base_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bitmap data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	while( data_offset < data_size )
	{
		byte_value = data[ data_offset ];

		data_offset++;

		for( bit_index = 0;
		     bit_index < 8;
		     bit_index++ )
		{
			if( in_allocated_range == 0 )
			{
				if( ( byte_value & 0x01 ) != 0 )
				{
					in_allocated_range            = 1;
					first_allocated_element_index = allocated_element_index;
				}
			}
			else if( ( byte_value & 0x01 ) == 0 )
			{
				allocated_range_offset = safe_base_offset + ( (off64_t) first_allocated_element_index * element_data_size );
				allocated_range_size   = ( (size64_t) allocated_element_index - first_allocated_element_index ) * element_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( first_allocated_element_index == ( allocated_element_index - 1 ) )
					{
						libcnotify_printf(
						 "%s: allocated element\t\t\t: %d\n",
						 function,
						 first_allocated_element_index );
					}
					else
					{
						libcnotify_printf(
						 "%s: allocated elements\t\t\t: %d - %d\n",
						 function,
						 first_allocated_element_index,
						 allocated_element_index - 1 );
					}
					libcnotify_printf(
					 "%s: allocated block range\t\t: 0x%" PRIx64 " - 0x%" PRIx64 "\n",
					 function,
					 allocated_range_offset,
					 allocated_range_offset + allocated_range_size );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				result = libcdata_range_list_insert_range(
				          bitmap_values->allocated_block_list,
				          allocated_range_offset,
				          allocated_range_size,
				          NULL,
				          NULL,
				          NULL,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append allocated block range to list.",
					 function );

					return( -1 );
				}
				in_allocated_range = 0;
			}
			byte_value >>= 1;

			allocated_element_index++;
		}
	}
	if( in_allocated_range != 0 )
	{
		allocated_range_offset = safe_base_offset + ( (off64_t) first_allocated_element_index * element_data_size );
		allocated_range_size   = ( (size64_t) allocated_element_index - first_allocated_element_index ) * element_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( first_allocated_element_index == ( allocated_element_index - 1 ) )
			{
				libcnotify_printf(
				 "%s: allocated element\t\t\t: %d\n",
				 function,
				 first_allocated_element_index );
			}
			else
			{
				libcnotify_printf(
				 "%s: allocated elements\t\t\t: %d - %d\n",
				 function,
				 first_allocated_element_index,
				 allocated_element_index - 1 );
			}
			libcnotify_printf(
			 "%s: allocated block range\t\t: 0x%" PRIx64 " - 0x%" PRIx64 "\n",
			 function,
			 allocated_range_offset,
			 allocated_range_offset + allocated_range_size );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		result = libcdata_range_list_insert_range(
		          bitmap_values->allocated_block_list,
		          allocated_range_offset,
		          allocated_range_size,
		          NULL,
		          NULL,
		          NULL,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append allocated block range to list.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	*base_offset = safe_base_offset + ( (off64_t) allocated_element_index * element_data_size );

	return( 1 );
}

/* Reads the bitmap values from an MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_bitmap_values_read_from_mft_attribute(
     libfsntfs_bitmap_values_t *bitmap_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     size_t element_data_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfcache_cache_t *cluster_block_cache   = NULL;
	libfdata_vector_t *cluster_block_vector  = NULL;
	libfsntfs_cluster_block_t *cluster_block = NULL;
	uint8_t *data                            = NULL;
	static char *function                    = "libfsntfs_bitmap_values_read_from_mft_attribute";
	size64_t segment_size                    = 0;
	size_t data_size                         = 0;
	off64_t base_offset                      = 0;
	off64_t segment_offset                   = 0;
	uint32_t attribute_type                  = 0;
	uint32_t segment_flags                   = 0;
	int cluster_block_index                  = 0;
	int number_of_cluster_blocks             = 0;
	int number_of_segments                   = 0;
	int result                               = 0;
	int segment_file_index                   = 0;
	int segment_index                        = 0;

	if( bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bitmap values.",
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

		goto on_error;
	}
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		goto on_error;
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

		goto on_error;
	}
	else if( result != 0 )
	{
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

			goto on_error;
		}
		if( libfsntfs_bitmap_values_read_data(
		     bitmap_values,
		     data,
		     data_size,
		     element_data_size,
		     &base_offset,
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
	}
	else if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
	{
		if( libfsntfs_cluster_block_vector_initialize(
		     &cluster_block_vector,
		     io_handle,
		     mft_attribute,
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
		if( libfdata_vector_get_number_of_segments(
		     cluster_block_vector,
		     &number_of_segments,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of cluster block vector segments.",
			 function );

			goto on_error;
		}
		for( segment_index = 0;
		     segment_index < number_of_segments;
		     segment_index++ )
		{
			if( libfdata_vector_get_segment_by_index(
			     cluster_block_vector,
			     segment_index,
			     &segment_file_index,
			     &segment_offset,
			     &segment_size,
			     &segment_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve cluster block vector segment: %d.",
				 function,
				 segment_index );

				goto on_error;
			}
			if( ( segment_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
			{
				cluster_block_index += segment_size / io_handle->cluster_block_size;

				continue;
			}
			while( segment_size > 0 )
			{
				if( cluster_block_index >= number_of_cluster_blocks )
				{
					break;
				}
				if( libfdata_vector_get_element_value_by_index(
				     cluster_block_vector,
				     (intptr_t *) file_io_handle,
				     (libfdata_cache_t *) cluster_block_cache,
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
				if( libfsntfs_bitmap_values_read_data(
				     bitmap_values,
				     cluster_block->data,
				     cluster_block->data_size,
				     element_data_size,
				     &base_offset,
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
				segment_size -= io_handle->cluster_block_size;

				cluster_block_index++;
			}
			if( cluster_block_index >= number_of_cluster_blocks )
			{
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
	return( -1 );
}

