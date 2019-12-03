/*
 * Attribute list attribute ($ATTRIBUTE_LIST) functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libfsntfs_attribute_list.h"
#include "libfsntfs_attribute_list_entry.h"
#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_vector.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"

/* Reads the attribute list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_read_data(
     libcdata_array_t *attribute_list,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_list_entry_t *attribute_list_entry = NULL;
	static char *function                                  = "libfsntfs_attribute_list_read_data";
	size_t data_offset                                     = 0;
	int attribute_index                                    = 0;
	int entry_index                                        = 0;

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
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
		 "%s: attribute list data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	while( data_offset < data_size )
	{
		if( libfsntfs_attribute_list_entry_initialize(
		     &attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute list entry.",
			 function );

			goto on_error;
		}
		if( libfsntfs_attribute_list_entry_read_data(
		     attribute_list_entry,
		     &( data[ data_offset ] ),
		     data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute list entry: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		data_offset += attribute_list_entry->size;

		if( libcdata_array_append_entry(
		     attribute_list,
		     &entry_index,
		     (intptr_t *) attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append list attribute to array.",
			 function );

			goto on_error;
		}
		attribute_list_entry = NULL;

		attribute_index++;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < data_size )
		{
			libcnotify_printf(
			 "%s: alignment padding:\n",
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
	if( attribute_list_entry != NULL )
	{
		libfsntfs_attribute_list_entry_free(
		 &attribute_list_entry,
		 NULL );
	}
	return( -1 );
}

/* Reads the attribute list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_read_from_attribute(
     libcdata_array_t *attribute_list,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_attribute_t *attribute,
     libcerror_error_t **error )
{
	libfcache_cache_t *cluster_block_cache   = NULL;
	libfdata_vector_t *cluster_block_vector  = NULL;
	libfsntfs_cluster_block_t *cluster_block = NULL;
	uint8_t *data                            = NULL;
	static char *function                    = "libfsntfs_attribute_list_read_from_attribute";
	size64_t data_size                       = 0;
	size_t cluster_block_data_size           = 0;
	uint16_t attribute_data_flags            = 0;
	int cluster_block_index                  = 0;
	int number_of_cluster_blocks             = 0;
	int number_of_data_runs                  = 0;

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_number_of_data_runs(
	     attribute,
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
/* TODO move support of no data run into cluster block vector */
	if( number_of_data_runs == 0 )
	{
		if( libfsntfs_mft_attribute_get_data(
		     attribute,
		     &data,
		     (size_t *) &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute data.",
			 function );

			goto on_error;
		}
		if( data_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( libfsntfs_attribute_list_read_data(
		     attribute_list,
		     data,
		     (size_t) data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute list.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfsntfs_mft_attribute_get_data_flags(
		     attribute,
		     &attribute_data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute data flags.",
			 function );

			goto on_error;
		}
		if( ( attribute_data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compressed attribute data.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_attribute_get_data_size(
		     attribute,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute data size.",
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
			cluster_block_data_size = cluster_block->data_size;

			if( (size64_t) cluster_block_data_size > data_size )
			{
				cluster_block_data_size = (size_t) data_size;
			}
			if( libfsntfs_attribute_list_read_data(
			     attribute_list,
			     cluster_block->data,
			     cluster_block_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read attribute list.",
				 function );

				goto on_error;
			}
			data_size -= cluster_block_data_size;
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

