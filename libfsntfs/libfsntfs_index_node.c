/*
 * Index node functions
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

#include "libfsntfs_definitions.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_index_node_header.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_libcdata.h"
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
	if( libcdata_array_initialize(
	     &( ( *index_node )->values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		goto on_error;
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
	int result            = 1;

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
		if( ( *index_node )->header != NULL )
		{
			if( libfsntfs_index_node_header_free(
			     &( ( *index_node )->header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index node header.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( ( *index_node )->values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free values array.",
			 function );

			result = -1;
		}
		memory_free(
		 *index_node );

		*index_node = NULL;
	}
	return( result );
}

/* Reads the index node header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_node_read_header(
     libfsntfs_index_node_t *index_node,
     const uint8_t *data,
     size_t data_size,
     size_t data_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_node_read_header";

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
	if( index_node->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index node - header value already set.",
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
	if( libfsntfs_index_node_header_initialize(
	     &( index_node->header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index node header.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_node_header_read_data(
	     index_node->header,
	     &( data[ data_offset ] ),
	     data_size - data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node header.",
		 function );

		goto on_error;
	}
	index_node->header_data_offset = data_offset;

	return( 1 );

on_error:
	if( index_node->header != NULL )
	{
		libfsntfs_index_node_header_free(
		 &( index_node->header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the index node values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_node_read_values(
     libfsntfs_index_node_t *index_node,
     off64_t index_value_vcn_offset,
     const uint8_t *data,
     size_t data_size,
     size_t data_offset,
     libcerror_error_t **error )
{
	libfsntfs_index_value_t *index_value = NULL;
	static char *function                = "libfsntfs_index_node_read_values";
	size_t index_node_size               = 0;
	size_t index_values_offset           = 0;
	size_t unknown_data_size             = 0;
	ssize_t read_count                   = 0;
	uint32_t index_value_flags           = 0;
	int entry_index                      = 0;
	int index_value_entry                = 0;

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
	if( index_node->header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid index node - missing header value.",
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
	index_values_offset = (size_t) index_node->header->index_values_offset;

	if( index_values_offset > ( data_size - index_node->header_data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index values offset value out of bounds.",
		 function );

		return( -1 );
	}
	index_values_offset += index_node->header_data_offset;

	index_node_size = index_node->header->size;

	if( ( index_node_size < sizeof( fsntfs_index_node_header_t ) )
	 || ( index_node_size > data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index node size value out of bounds.",
		 function );

		goto on_error;
	}
	index_node_size -= sizeof( fsntfs_index_node_header_t );

	if( index_node_size == 0 )
	{
		return( 1 );
	}
	if( data_offset < index_values_offset )
	{
		unknown_data_size = (size_t) index_values_offset - data_offset;

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

		if( unknown_data_size > index_node_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index values offset value out of bounds.",
			 function );

			goto on_error;
		}
		index_node_size -= (uint32_t) unknown_data_size;
	}
	while( index_node_size > 0 )
	{
		if( libfsntfs_index_value_initialize(
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index value.",
			 function );

			goto on_error;
		}
		read_count = libfsntfs_index_value_read(
		              index_value,
		              index_value_vcn_offset + data_offset,
		              &index_value_entry,
		              data,
		              data_size,
		              data_offset,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read index value.",
			 function );

			goto on_error;
		}
		data_offset     += read_count;
		index_node_size -= (uint32_t) read_count;

		index_value_flags = index_value->flags;

		if( libcdata_array_append_entry(
		     index_node->values_array,
		     &entry_index,
		     (intptr_t *) index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append index value to values array.",
			 function );

			goto on_error;
		}
		index_value = NULL;

		if( ( index_value_flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
		{
			break;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( index_node_size > 0 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) index_node_size,
			 0 );
		}
		data_offset += index_node_size;
	}
#endif
	return( 1 );

on_error:
	if( index_value != NULL )
	{
		libfsntfs_index_value_free(
		 &index_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_node_get_number_of_values(
     libfsntfs_index_node_t *index_node,
     int *number_of_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_node_get_number_of_values";

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
	if( libcdata_array_get_number_of_entries(
	     index_node->values_array,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_node_get_value_by_index(
     libfsntfs_index_node_t *index_node,
     int value_entry,
     libfsntfs_index_value_t **value,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_node_get_value_by_index";

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
	if( libcdata_array_get_entry_by_index(
	     index_node->values_array,
	     value_entry,
	     (intptr_t **) value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( 1 );
}

