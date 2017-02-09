/*
 * Cluster block functions
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_cluster_block.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_types.h"
#include "libfsntfs_unused.h"

/* Creates a cluster block
 * Make sure the value cluster_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_initialize(
     libfsntfs_cluster_block_t **cluster_block,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_initialize";

	if( cluster_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block.",
		 function );

		return( -1 );
	}
	if( *cluster_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid cluster block value already set.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	*cluster_block = memory_allocate_structure(
	                  libfsntfs_cluster_block_t );

	if( *cluster_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create cluster block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *cluster_block,
	     0,
	     sizeof( libfsntfs_cluster_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear cluster block.",
		 function );

		memory_free(
		 *cluster_block );

		*cluster_block = NULL;

		return( -1 );
	}
	if( data_size > 0 )
	{
		( *cluster_block )->data = (uint8_t *) memory_allocate(
		                                        sizeof( uint8_t ) * data_size );

		if( ( *cluster_block )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *cluster_block )->data_size = data_size;
	}
	return( 1 );

on_error:
	if( *cluster_block != NULL )
	{
		memory_free(
		 *cluster_block );

		*cluster_block = NULL;
	}
	return( -1 );
}

/* Frees a cluster block
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_free(
     libfsntfs_cluster_block_t **cluster_block,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_free";

	if( cluster_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block.",
		 function );

		return( -1 );
	}
	if( *cluster_block != NULL )
	{
		if( ( *cluster_block )->data != NULL )
		{
			memory_free(
			 ( *cluster_block )->data );
		}
		memory_free(
		 *cluster_block );

		*cluster_block = NULL;
	}
	return( 1 );
}

/* Reads a cluster block
 * Callback function for the cluster block vector
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_read_element_data(
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index LIBFSNTFS_ATTRIBUTE_UNUSED,
     int element_data_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
     off64_t cluster_block_offset,
     size64_t cluster_block_size,
     uint32_t range_flags,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_cluster_block_t *cluster_block = NULL;
	static char *function                    = "libfsntfs_cluster_block_read_element_data";
	ssize_t read_count                       = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( element_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( element_data_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

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
	if( ( cluster_block_size == 0 )
	 || ( cluster_block_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid cluster block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_cluster_block_initialize(
	     &cluster_block,
	     (size_t) cluster_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block.",
		 function );

		goto on_error;
	}
	if( cluster_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing cluster block.",
		 function );

		goto on_error;
	}
	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( memory_set(
		     cluster_block->data,
		     0,
		     cluster_block->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear cluster block data.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( cluster_block_size == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cluster block size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading cluster block at offset: 0x%08" PRIx64 " with size: %" PRIu64 ".\n",
			 function,
			 cluster_block_offset,
			 cluster_block_size );
		}
#endif
		if( libbfio_handle_seek_offset(
		     file_io_handle,
		     cluster_block_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: 0x%08" PRIx64 ".",
			 function,
			 cluster_block_offset );

			goto on_error;
		}
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              cluster_block->data,
		              cluster_block->data_size,
		              error );

		if( read_count != (ssize_t) cluster_block->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read cluster block.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) cluster_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_cluster_block_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set cluster block as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( cluster_block != NULL )
	{
		libfsntfs_cluster_block_free(
		 &cluster_block,
		 NULL );
	}
	return( -1 );
}

