/*
 * Index functions
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_bitmap_values.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_index.h"
#include "libfsntfs_index_entry.h"
#include "libfsntfs_index_entry_vector.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_unused.h"

#include "fsntfs_index.h"

/* Creates an index
 * Make sure the value index is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_initialize(
     libfsntfs_index_t **index,
     const uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_initialize";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( *index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index value already set.",
		 function );

		return( -1 );
	}
	*index = memory_allocate_structure(
	          libfsntfs_index_t );

	if( *index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index,
	     0,
	     sizeof( libfsntfs_index_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index.",
		 function );

		memory_free(
		 *index );

		*index = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *index )->root_values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root values array.",
		 function );

		goto on_error;
	}
	( *index )->name      = name;
	( *index )->name_size = name_size;

	return( 1 );

on_error:
	if( *index != NULL )
	{
		memory_free(
		 *index );

		*index = NULL;
	}
	return( -1 );
}

/* Frees an index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_free(
     libfsntfs_index_t **index,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_free";
	int result            = 1;

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( *index != NULL )
	{
		if( libcdata_array_free(
		     &( ( *index )->root_values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free root values array.",
			 function );

			result = -1;
		}
		if( ( *index )->index_entry_vector != NULL )
		{
			if( libfdata_vector_free(
			     &( ( *index )->index_entry_vector ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index entry vector.",
				 function );

				result = -1;
			}
		}
		if( ( *index )->index_entry_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *index )->index_entry_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index entry cache.",
				 function );

				result = -1;
			}
		}
		if( ( *index )->index_value_list != NULL )
		{
			if( libfdata_list_free(
			     &( ( *index )->index_value_list ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index value list.",
				 function );

				result = -1;
			}
		}
		if( ( *index )->index_value_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *index )->index_value_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index value cache.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *index );

		*index = NULL;
	}
	return( result );
}

/* Sets the $INDEX_ROOT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_set_index_root_attribute(
     libfsntfs_index_t *index,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_set_index_root_attribute";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
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
	if( index->index_root_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index - index root attribute already set.",
		 function );

		return( -1 );
	}
	index->index_root_attribute = attribute;

	return( 1 );
}

/* Sets the $INDEX_ALLOCATION attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_set_index_allocation_attribute(
     libfsntfs_index_t *index,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_set_index_allocation_attribute";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
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
	if( index->index_allocation_attribute == NULL )
	{
		index->index_allocation_attribute = attribute;
	}
	else
	{
		if( libfsntfs_attribute_append_to_chain(
		     &( index->index_allocation_attribute ),
		     attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append attribute to index allocation attribute chain.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the $BITMAP attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_set_bitmap_attribute(
     libfsntfs_index_t *index,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_set_bitmap_attribute";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
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
	if( index->bitmap_attribute == NULL )
	{
		index->bitmap_attribute = attribute;
	}
	else
	{
		if( libfsntfs_attribute_append_to_chain(
		     &( index->bitmap_attribute ),
		     attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append attribute to index bitmap attribute chain.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read(
     libfsntfs_index_t *index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_read";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( index->is_read != 0 )
	{
		return( 1 );
	}
	if( index->index_value_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index - index value list value already set.",
		 function );

		return( -1 );
	}
	if( index->index_value_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index - index value cache value already set.",
		 function );

		return( -1 );
	}
	if( index->index_root_attribute == NULL )
	{
		return( 1 );
	}
	if( libfdata_list_initialize(
	     &( index->index_value_list ),
	     (intptr_t *) index,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_index_read_index_value_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index value list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( index->index_value_cache ),
	     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_INDEX_VALUES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index value cache.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_read_root(
	     index,
	     io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index root.",
		 function );

		goto on_error;
	}
/* TODO add support for bitmap ? */
#if defined( HAVE_DEBUG_OUTPUT )
	/* The index does not necessarily have a $BITMAP attribute
	 */
	if( index->index_allocation_attribute != NULL )
	{
		if( libfsntfs_index_read_bitmap(
		     index,
		     io_handle,
		     file_io_handle,
		     flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read index bitmap.",
			 function );

			goto on_error;
		}
	}
#endif
	/* The index does not necessarily have an $INDEX_ALLOCATION attribute
	 */
	if( index->index_allocation_attribute != NULL )
	{
		if( libfsntfs_index_entry_vector_initialize(
		     &( index->index_entry_vector ),
		     io_handle,
		     index->index_allocation_attribute,
		     index->index_entry_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index entry vector.",
			 function );

			goto on_error;
		}
		if( libfcache_cache_initialize(
		     &( index->index_entry_cache ),
		     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_INDEX_ENTRIES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index entry cache.",
			 function );

			goto on_error;
		}
	}
	if( libfsntfs_index_read_sub_nodes(
	     index,
	     io_handle,
	     file_io_handle,
	     index->root_values_array,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sub nodes.",
		 function );

		goto on_error;
	}
	index->is_read = 1;

	return( 1 );

on_error:
	if( index->index_entry_cache != NULL )
	{
		libfcache_cache_free(
		 &( index->index_entry_cache ),
		 NULL );
	}
	if( index->index_entry_vector != NULL )
	{
		libfdata_vector_free(
		 &( index->index_entry_vector ),
		 NULL );
	}
	if( index->index_value_cache != NULL )
	{
		libfcache_cache_free(
		 &( index->index_value_cache ),
		 NULL );
	}
	if( index->index_value_list != NULL )
	{
		libfdata_list_free(
		 &( index->index_value_list ),
		 NULL );
	}
	return( -1 );
}

/* Reads the index root
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read_root(
     libfsntfs_index_t *index,
     libfsntfs_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	libfsntfs_index_node_t *index_node      = NULL;
	libfsntfs_index_value_t *index_value    = NULL;
	uint8_t *index_root_attribute_data      = NULL;
	static char *function                   = "libfsntfs_index_read_root";
	size64_t index_root_attribute_data_size = 0;
	size_t attribute_data_offset            = 0;
	size_t unknown_data_size                = 0;
	ssize_t read_count                      = 0;
	uint32_t index_node_size                = 0;
	uint32_t index_values_offset            = 0;
	int entry_index                         = 0;
	int index_value_entry                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                    = 0;
#endif

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( index->index_root_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid index - missing index root attribute.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_data(
	     index->index_root_attribute,
	     &index_root_attribute_data,
	     &index_root_attribute_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index root attribute data.",
		 function );

		return( -1 );
	}
	if( index_root_attribute_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid index root attribute data.",
		 function );

		return( -1 );
	}
	if( ( index_root_attribute_data_size < (size64_t) sizeof( fsntfs_index_root_header_t ) )
	 || ( index_root_attribute_data_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index root attribute data size value out of bounds.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index root header data:\n",
		 function );
		libcnotify_print_data(
		 index_root_attribute_data,
		 sizeof( fsntfs_index_root_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_root_header_t *) index_root_attribute_data )->attribute_type,
	 index->attribute_type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_root_header_t *) index_root_attribute_data )->index_entry_size,
	 index->index_entry_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute type\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 index->attribute_type,
		 libfsntfs_debug_print_attribute_type(
		  index->attribute_type ) );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_index_root_header_t *) index_root_attribute_data )->collating_type,
		 value_32bit );
		libcnotify_printf(
		 "%s: collating type\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: index entry size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 index->index_entry_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_index_root_header_t *) index_root_attribute_data )->index_entry_number_of_cluster_blocks,
		 value_32bit );
		libcnotify_printf(
		 "%s: index entry number of cluster blocks\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( index->index_entry_size != io_handle->index_entry_size )
		{
			libcnotify_printf(
			 "%s: mismatch in index entry size (in index root: %" PRIu32 ", in volume header: %" PRIu32 ").\n",
			 function,
			 index->index_entry_size,
			 io_handle->index_entry_size );
		}
	}
#endif
	attribute_data_offset = sizeof( fsntfs_index_root_header_t );

	if( libfsntfs_index_node_initialize(
	     &index_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index node.",
		 function );

		goto on_error;
	}
	read_count = libfsntfs_index_node_read(
		      index_node,
		      index_root_attribute_data,
		      index_root_attribute_data_size,
		      attribute_data_offset,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node.",
		 function );

		goto on_error;
	}
	attribute_data_offset += read_count;
	index_values_offset    = index_node->index_values_offset + (uint32_t) sizeof( fsntfs_index_root_header_t );
	index_node_size        = index_node->size;
	index->flags           = index_node->flags;

	if( libfsntfs_index_node_free(
	     &index_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free index node.",
		 function );

		goto on_error;
	}
	if( index_node_size > 0 )
	{
		if( (size_t) index_node_size < sizeof( fsntfs_index_node_header_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index node size value out of bounds.",
			 function );

			goto on_error;
		}
		index_node_size -= (uint32_t) sizeof( fsntfs_index_node_header_t );

		if( attribute_data_offset < index_values_offset )
		{
			unknown_data_size = (size_t) index_values_offset - attribute_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: index root unknown data:\n",
				 function );
				libcnotify_print_data(
				 &( index_root_attribute_data[ attribute_data_offset ] ),
				 unknown_data_size,
				 0 );
			}
#endif
			attribute_data_offset += unknown_data_size;
			index_node_size       -= (uint32_t) unknown_data_size;
		}
		if( ( attribute_data_offset + (size_t) index_node_size ) > index_root_attribute_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index node size value out of bounds.",
			 function );

			goto on_error;
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
			              (off64_t) attribute_data_offset,
			              &index_value_entry,
			              index_root_attribute_data,
			              index_root_attribute_data_size,
			              attribute_data_offset,
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
			attribute_data_offset += read_count;
			index_node_size       -= (uint32_t) read_count;

			if( libcdata_array_append_entry(
			     index->root_values_array,
			     &entry_index,
			     (intptr_t *) index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append index value to root values array.",
				 function );

				goto on_error;
			}
			if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
			{
				index_value = NULL;

				break;
			}
			index_value = NULL;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( index_node_size > 0 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: index values trailing data:\n",
				 function );
				libcnotify_print_data(
				 &( index_root_attribute_data[ attribute_data_offset ] ),
				 (size_t) index_node_size,
				 0 );
			}
			attribute_data_offset += index_node_size;
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( attribute_data_offset < index_root_attribute_data_size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( index_root_attribute_data[ attribute_data_offset ] ),
			 (size_t) index_root_attribute_data_size - attribute_data_offset,
			 0 );
		}
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
	if( index_node != NULL )
	{
		libfsntfs_index_node_free(
		 &index_node,
		 NULL );
	}
	return( -1 );
}

/* Reads the index bitmap
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read_bitmap(
     libfsntfs_index_t *index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfsntfs_bitmap_values_t *bitmap_values = NULL;
	static char *function                    = "libfsntfs_index_read_bitmap";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_value(
	     index->bitmap_attribute,
	     (intptr_t **) &bitmap_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute value.",
		 function );

		return( -1 );
	}
	if( bitmap_values == NULL )
	{
		if( libfsntfs_attribute_read_value(
		     index->bitmap_attribute,
		     io_handle,
		     file_io_handle,
		     flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute value.",
			 function );

			return( -1 );
		}
		if( libfsntfs_attribute_get_value(
		     index->bitmap_attribute,
		     (intptr_t **) &bitmap_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
/* TODO get a range list from the bitmap values */

	return( 1 );
}

/* Reads the index sub nodes
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read_sub_nodes(
     libfsntfs_index_t *index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_array_t *node_index_values_array,
     libcerror_error_t **error )
{
	libcdata_array_t *index_values_array      = NULL;
	libfsntfs_index_entry_t *index_entry      = NULL;
	libfsntfs_index_value_t *node_index_value = NULL;
	static char *function                     = "libfsntfs_index_read_sub_nodes";
	off64_t element_data_offset               = 0;
	off64_t index_entry_offset                = 0;
	uint32_t index_value_flags                = 0;
	int element_index                         = 0;
	int element_file_index                    = 0;
	int index_value_entry                     = 0;
	int number_of_node_index_values           = 0;
	int node_index_value_entry                = 0;

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
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
	if( libcdata_array_get_number_of_entries(
	     node_index_values_array,
	     &number_of_node_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of node index values.",
		 function );

		goto on_error;
	}
	for( node_index_value_entry = 0;
	     node_index_value_entry < number_of_node_index_values;
	     node_index_value_entry++ )
	{
		if( libcdata_array_get_entry_by_index(
		     node_index_values_array,
		     node_index_value_entry,
		     (intptr_t **) &node_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node index value: %d.",
			 function,
			 node_index_value_entry );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfsntfs_index_value_print(
			     node_index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print index value.",
				 function );

				goto on_error;
			}
		}
#endif
		if( ( node_index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_HAS_SUB_NODE ) != 0 )
		{
			if( node_index_value->sub_node_vcn > (uint64_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: node index value: %d sub node VCN value out of bounds.",
				 function,
				 node_index_value_entry );

				goto on_error;
			}
			index_entry_offset = (off64_t) ( node_index_value->sub_node_vcn * io_handle->cluster_block_size );

			if( libfdata_vector_get_element_value_at_offset(
			     index->index_entry_vector,
			     (intptr_t *) file_io_handle,
			     index->index_entry_cache,
			     index_entry_offset,
			     &element_data_offset,
			     (intptr_t **) &index_entry,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve index entry with VCN: %d at offset: 0x%08" PRIx64 ".",
				 function,
				 (int) node_index_value->sub_node_vcn,
				 index_entry_offset );

				goto on_error;
			}
			if( libfsntfs_index_entry_read_index_values(
			     index_entry,
			     index_entry_offset,
			     &index_value_entry,
			     &index_values_array,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read index values array from index entry with VCN: %d at offset: 0x%08" PRIx64 ".",
				 function,
				 (int) node_index_value->sub_node_vcn,
				 index_entry_offset );

				goto on_error;
			}
			if( libfsntfs_index_read_sub_nodes(
			     index,
			     io_handle,
			     file_io_handle,
			     index_values_array,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read sub nodes of index entry with VCN: %d at offset: 0x%08" PRIx64 ".",
				 function,
				 (int) node_index_value->sub_node_vcn,
				 index_entry_offset );

				goto on_error;
			}
			if( libcdata_array_free(
			     &index_values_array,
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_value_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free values array.",
				 function );

				goto on_error;
			}
		}
		if( ( node_index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
		{
			break;
		}
		if( node_index_values_array == index->root_values_array )
		{
			/* The element file index value contains the root index value entry + 1
			 */
			element_file_index = node_index_value_entry + 1;
			index_value_flags  = LIBFSNTFS_INDEX_VALUE_LIST_FLAG_STORED_IN_ROOT;
		}
		else
		{
			element_file_index = 0;
			index_value_flags  = 0;
		}
		/* Add the index values containing data in a depth first manner
		 * This will create a sorted 'list' of the index values
		 */
		if( libfdata_list_append_element(
		     index->index_value_list,
		     &element_index,
		     element_file_index,
		     node_index_value->offset,
		     (size64_t) node_index_value->size,
		     index_value_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append index value to list.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( index_values_array != NULL )
	{
		libcdata_array_free(
		 &index_values_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_value_free,
		 NULL );
	}
	return( -1 );
}

/* Reads an index value
 * Callback function for the index value list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read_index_value_element_data(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfcache_cache_t *cache,
     int element_file_index,
     off64_t index_value_offset,
     size64_t element_size LIBFSNTFS_ATTRIBUTE_UNUSED,
     uint32_t index_value_flags,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_index_entry_t *index_entry = NULL;
	libfsntfs_index_value_t *index_value = NULL;
	static char *function                = "libfsntfs_index_read_index_value_element_data";
	off64_t index_entry_data_offset      = 0;
	ssize_t read_count                   = 0;
	uint8_t write_flags                  = 0;
	int index_value_entry                = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( element_size )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( libfdata_list_element_get_element_index(
	     element,
	     &index_value_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element index.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading index value: %03d at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
		 function,
		 index_value_entry,
		 index_value_offset,
		 index_value_offset );
	}
#endif
	if( ( index_value_flags & LIBFSNTFS_INDEX_VALUE_LIST_FLAG_STORED_IN_ROOT ) != 0 )
	{
		/* The element file index value contains the root index value entry + 1
		 */
		element_file_index -= 1;

		if( libcdata_array_get_entry_by_index(
		     index->root_values_array,
		     element_file_index,
		     (intptr_t **) &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index value: %d from root values array.",
			 function,
			 element_file_index );

			goto on_error;
		}
		write_flags = LIBFDATA_LIST_ELEMENT_VALUE_FLAG_NON_MANAGED;
	}
	else
	{
		if( libfdata_vector_get_element_value_at_offset(
		     index->index_entry_vector,
		     (intptr_t *) file_io_handle,
		     index->index_entry_cache,
		     index_value_offset,
		     &index_entry_data_offset,
		     (intptr_t **) &index_entry,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index entry at offset: 0x%08" PRIx64 ".",
			 function,
			 index_value_offset );

			goto on_error;
		}
		if( index_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing index entry at offset: 0x%08" PRIx64 ".",
			 function,
			 index_value_offset );

			return( -1 );
		}
		if( index_entry_data_offset > (off64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index value data offset value out of bounds.",
			 function );

			goto on_error;
		}
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
			      index_value_offset,
			      &index_value_entry,
			      index_entry->data,
			      index_entry->data_size,
			      (size_t) index_entry_data_offset,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read index value at offset: 0x%08" PRIx64 ".",
			 function,
			 index_value_offset );

			goto on_error;
		}
		write_flags = LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED;
	}
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) index_value,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_value_free,
	     write_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index value as element value.",
		 function );

		goto on_error;
	}
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

/* Retrieves the number of index values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_get_number_of_index_values(
     libfsntfs_index_t *index,
     int *number_of_index_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_get_number_of_index_values";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( index->index_value_list == NULL )
	{
		if( number_of_index_values == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of index values.",
			 function );

			return( -1 );
		}
		*number_of_index_values = 0;
	}
	else
	{
		if( libfdata_list_get_number_of_elements(
		     index->index_value_list,
		     number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of index values.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves a specific index value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_get_index_value_by_index(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     int index_value_entry,
     libfsntfs_index_value_t **index_value,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_get_index_value_by_index";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     index->index_value_list,
	     (intptr_t *) file_io_handle,
	     index->index_value_cache,
	     index_value_entry,
	     (intptr_t **) index_value,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index value: %d.",
		 function,
		 index_value_entry );

		return( -1 );
	}
	return( 1 );
}

