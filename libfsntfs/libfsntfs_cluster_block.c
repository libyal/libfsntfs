/*
 * Cluster block functions
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

#include "libfsntfs_cluster_block.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

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
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
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
		memory_free(
		 ( *cluster_block )->data );

		memory_free(
		 *cluster_block );

		*cluster_block = NULL;
	}
	return( 1 );
}

/* Clears a cluster block
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_clear(
     libfsntfs_cluster_block_t *cluster_block,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_clear";

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
	if( cluster_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid cluster block - missing data.",
		 function );

		return( -1 );
	}
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

		return( -1 );
	}
	return( 1 );
}

/* Reads a cluster block
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_read_file_io_handle(
     libfsntfs_cluster_block_t *cluster_block,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_read_file_io_handle";
	ssize_t read_count    = 0;

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading cluster block at offset: %" PRIi64 " (0x%08" PRIx64 ") with size: %" PRIu64 ".\n",
		 function,
		 file_offset,
		 file_offset,
		 cluster_block->data_size );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              cluster_block->data,
	              cluster_block->data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) cluster_block->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read cluster block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

