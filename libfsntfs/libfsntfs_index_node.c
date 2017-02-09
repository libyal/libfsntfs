/*
 * Index node functions
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
#include <types.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

#include "fsntfs_index.h"

/* Creates an index node
 * Make sure the value index_node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_node_initialize(
     libfsntfs_index_node_t **index_node,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_node_initialize";

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( *index_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index node value already set.",
		 function );

		return( -1 );
	}
	*index_node = memory_allocate_structure(
	                libfsntfs_index_node_t );

	if( *index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index_node,
	     0,
	     sizeof( libfsntfs_index_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index node.",
		 function );

		memory_free(
		 *index_node );

		*index_node = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *index_node != NULL )
	{
		memory_free(
		 *index_node );

		*index_node = NULL;
	}
	return( -1 );
}

/* Frees an index node
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_node_free(
     libfsntfs_index_node_t **index_node,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_node_free";

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( *index_node != NULL )
	{
		memory_free(
		 *index_node );

		*index_node = NULL;
	}
	return( 1 );
}

/* Reads the index node
 * Returns the number of bytes read if successful or -1 on error
 */
size_t libfsntfs_index_node_read(
        libfsntfs_index_node_t *index_node,
        uint8_t *index_node_data,
        size_t index_node_data_size,
        size_t index_node_data_offset,
        libcerror_error_t **error )
{
	uint8_t *index_node_header_data = NULL;
	static char *function           = "libfsntfs_index_node_read";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit            = 0;
#endif

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( index_node_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node data.",
		 function );

		return( -1 );
	}
	if( index_node_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: index node data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( index_node_data_offset > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: index node data offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( index_node_data_offset >= index_node_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: index node data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( index_node_data_size < sizeof( fsntfs_index_node_header_t ) )
	 || ( index_node_data_offset > ( index_node_data_size - sizeof( fsntfs_index_node_header_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: index node data size value too small.",
		 function );

		return( -1 );
	}
	index_node_header_data = &( index_node_data[ index_node_data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index node header data:\n",
		 function );
		libcnotify_print_data(
		 index_node_header_data,
		 sizeof( fsntfs_index_node_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_node_header_t *) index_node_header_data )->index_values_offset,
	 index_node->index_values_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_node_header_t *) index_node_header_data )->size,
	 index_node->size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_node_header_t *) index_node_header_data )->flags,
	 index_node->flags );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index values offset\t\t\t\t: %" PRIu32 "\n",
		 function,
		 index_node->index_values_offset );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 index_node->size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_index_node_header_t *) index_node_header_data )->allocated_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: allocated size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 index_node->flags );
		libfsntfs_debug_print_index_node_flags(
		 index_node->flags );
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( index_node->size > 0 )
	{
		if( ( (size_t) index_node->size < sizeof( fsntfs_index_node_header_t ) )
		 || ( ( index_node_data_offset + index_node->size ) > index_node_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index node size value out of bounds.",
			 function );

			return( -1 );
		}
		if( ( (size_t) index_node->index_values_offset < sizeof( fsntfs_index_node_header_t ) )
		 || ( index_node->index_values_offset > index_node->size )
		 || ( ( index_node->index_values_offset % 8 ) != 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index values offset value out of bounds.",
			 function );

			return( -1 );
		}
	}
	return( (ssize_t) sizeof( fsntfs_index_node_header_t ) );
}

