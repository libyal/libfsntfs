/*
 * Index node header functions
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

#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_index_node_header.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

#include "fsntfs_index.h"

/* Creates an index node header
 * Make sure the value index_node_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_node_header_initialize(
     libfsntfs_index_node_header_t **index_node_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_node_header_initialize";

	if( index_node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node header.",
		 function );

		return( -1 );
	}
	if( *index_node_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index node header value already set.",
		 function );

		return( -1 );
	}
	*index_node_header = memory_allocate_structure(
	                      libfsntfs_index_node_header_t );

	if( *index_node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index node header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index_node_header,
	     0,
	     sizeof( libfsntfs_index_node_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index node header.",
		 function );

		memory_free(
		 *index_node_header );

		*index_node_header = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *index_node_header != NULL )
	{
		memory_free(
		 *index_node_header );

		*index_node_header = NULL;
	}
	return( -1 );
}

/* Frees an index node header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_node_header_free(
     libfsntfs_index_node_header_t **index_node_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_node_header_free";

	if( index_node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node header.",
		 function );

		return( -1 );
	}
	if( *index_node_header != NULL )
	{
		memory_free(
		 *index_node_header );

		*index_node_header = NULL;
	}
	return( 1 );
}

/* Reads the index node header header
 * Returns the number of bytes read if successful or -1 on error
 */
int libfsntfs_index_node_header_read_data(
     libfsntfs_index_node_header_t *index_node_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_node_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( index_node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node header.",
		 function );

		return( -1 );
	}
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
	if( ( data_size < sizeof( fsntfs_index_node_header_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
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
		 "%s: index node header header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsntfs_index_node_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_node_header_t *) data )->index_values_offset,
	 index_node_header->index_values_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_node_header_t *) data )->size,
	 index_node_header->size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_node_header_t *) data )->flags,
	 index_node_header->flags );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index values offset\t\t: %" PRIu32 "\n",
		 function,
		 index_node_header->index_values_offset );

		libcnotify_printf(
		 "%s: size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 index_node_header->size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_index_node_header_t *) data )->allocated_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: allocated size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 index_node_header->flags );
		libfsntfs_debug_print_index_node_flags(
		 index_node_header->flags );
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( index_node_header->size > 0 )
	{
		if( (size_t) index_node_header->size < sizeof( fsntfs_index_node_header_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index node header size value out of bounds.",
			 function );

			return( -1 );
		}
		if( ( (size_t) index_node_header->index_values_offset < sizeof( fsntfs_index_node_header_t ) )
		 || ( index_node_header->index_values_offset > index_node_header->size )
		 || ( ( index_node_header->index_values_offset % 8 ) != 0 ) )
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
	return( 1 );
}

