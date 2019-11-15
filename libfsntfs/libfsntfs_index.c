/*
 * Index functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_bitmap_values.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_index.h"
#include "libfsntfs_index_entry.h"
#include "libfsntfs_index_entry_vector.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_index_root_header.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_unused.h"

#include "fsntfs_index.h"

/* Creates an index
 * Make sure the value index is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_initialize(
     libfsntfs_index_t **index,
     libfsntfs_io_handle_t *io_handle,
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
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( ( name_size <= 1 )
	 || ( name_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name size value out of bounds.",
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
	( *index )->name = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * name_size );

	if( ( *index )->name == NULL )
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
	     ( *index )->name,
	     name,
	     sizeof( uint8_t ) * name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		goto on_error;
	}
	( *index )->name_size = name_size;
	( *index )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *index != NULL )
	{
		if( ( *index )->name != NULL )
		{
			memory_free(
			 ( *index )->name );
		}
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
		/* The io_handle and index_root_attribute references are freed elsewhere
		 */
		if( ( *index )->root_header != NULL )
		{
			if( libfsntfs_index_root_header_free(
			     &( ( *index )->root_header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root header.",
				 function );

				result = -1;
			}
		}
		if( ( *index )->root_node != NULL )
		{
			if( libfsntfs_index_node_free(
			     &( ( *index )->root_node ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root node.",
				 function );

				result = -1;
			}
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
		if( ( *index )->index_node_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *index )->index_node_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index node cache.",
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
		if( ( *index )->name != NULL )
		{
			memory_free(
			 ( *index )->name );
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
     libfsntfs_mft_attribute_t *attribute,
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
	index->index_root_attribute = attribute;

	return( 1 );
}

/* Sets the $INDEX_ALLOCATION attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_set_index_allocation_attribute(
     libfsntfs_index_t *index,
     libfsntfs_mft_attribute_t *attribute,
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
/* TODO move this into append to chain ? */
	if( index->index_allocation_attribute == NULL )
	{
		index->index_allocation_attribute = attribute;
	}
	else
	{
		if( libfsntfs_mft_attribute_append_to_chain(
		     index->index_allocation_attribute,
		     attribute,
		     &( index->index_allocation_attribute ),
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
     libfsntfs_mft_attribute_t *attribute,
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
/* TODO move this into append to chain ? */
	if( index->bitmap_attribute == NULL )
	{
		index->bitmap_attribute = attribute;
	}
	else
	{
		if( libfsntfs_mft_attribute_append_to_chain(
		     index->bitmap_attribute,
		     attribute,
		     &( index->bitmap_attribute ),
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
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_index_read_index_value_element_data,
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
	     1,
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
/* TODO check bitmap attribute instead of allocation attribute */
	if( index->index_allocation_attribute != NULL )
	{
		if( libfsntfs_index_read_bitmap(
		     index,
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
		     index->io_handle,
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
		     &( index->index_node_cache ),
		     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_INDEX_NODES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index node cache.",
			 function );

			goto on_error;
		}
	}
	if( libfsntfs_index_read_sub_nodes(
	     index,
	     file_io_handle,
	     index->root_node,
	     0,
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
	if( index->index_node_cache != NULL )
	{
		libfcache_cache_free(
		 &( index->index_node_cache ),
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
     libcerror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "libfsntfs_index_read_root";
	size_t data_offset    = 0;
	size_t data_size      = 0;

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
	if( index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( index->root_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index - root node value already set.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_data(
	     index->index_root_attribute,
	     &data,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index root attribute data.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_root_header_initialize(
	     &( index->root_header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root header.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_root_header_read_data(
	     index->root_header,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read root header.",
		 function );

		goto on_error;
	}
	data_offset = sizeof( fsntfs_index_root_header_t );

	if( libfsntfs_index_root_header_get_index_entry_size(
	     index->root_header,
	     &( index->index_entry_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index entry size.",
		 function );

		goto on_error;
	}
/* TODO for directory entries index check if collation type is COLLATION_FILENAME */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( index->index_entry_size != index->io_handle->index_entry_size )
		{
			libcnotify_printf(
			 "%s: mismatch in index entry size (in index root header: %" PRIu32 ", in volume header: %" PRIu32 ").\n",
			 function,
			 index->index_entry_size,
			 index->io_handle->index_entry_size );
		}
	}
#endif
	if( libfsntfs_index_node_initialize(
	     &( index->root_node ),
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
	if( libfsntfs_index_node_read_header(
	     index->root_node,
	     data,
	     data_size,
	     data_offset,
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
	data_offset += sizeof( fsntfs_index_node_header_t );

	index->flags = index->root_node->header->flags;

	if( libfsntfs_index_node_read_values(
	     index->root_node,
	     0,
	     data,
	     data_size,
	     data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node values.",
		 function );

		goto on_error;
	}
	data_offset += index->root_node->header->size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < data_size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
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
	if( index->root_node != NULL )
	{
		libfsntfs_index_node_free(
		 &( index->root_node ),
		 NULL );
	}
	if( index->root_header != NULL )
	{
		libfsntfs_index_root_header_free(
		 &( index->root_header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the index bitmap
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read_bitmap(
     libfsntfs_index_t *index,
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
	if( libfsntfs_bitmap_values_initialize(
	     &bitmap_values,
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
	if( libfsntfs_bitmap_values_read_from_mft_attribute(
	     bitmap_values,
	     index->bitmap_attribute,
	     index->io_handle,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read bitmap values from attribute.",
		 function );

		goto on_error;
	}
/* TODO get a range list from the bitmap values */

	if( libfsntfs_bitmap_values_free(
	     &bitmap_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free bitmap values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( bitmap_values != NULL )
	{
		libfsntfs_bitmap_values_free(
		 &bitmap_values,
		 NULL );
	}
	return( -1 );
}

/* Retrieves an index node
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_get_sub_node(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *index_node_cache,
     off64_t index_entry_offset,
     int sub_node_vcn,
     libfsntfs_index_node_t **index_node,
     libcerror_error_t **error )
{
	static char *function            = "libfsntfs_index_get_sub_node";
	off64_t element_data_offset      = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp = 0;
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
	if( index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_PROFILER )
	if( index->io_handle->profiler != NULL )
	{
		if( libfsntfs_profiler_start_timing(
		     index->io_handle->profiler,
		     &profiler_start_timestamp,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to start timing.",
			 function );

			return( -1 );
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	if( libfdata_vector_get_element_value_at_offset(
	     index->index_entry_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) index_node_cache,
	     index_entry_offset,
	     &element_data_offset,
	     (intptr_t **) index_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index node from index entry with VCN: %d at offset: 0x%08" PRIx64 ".",
		 function,
		 sub_node_vcn,
		 index_entry_offset );

		return( -1 );
	}
#if defined( HAVE_PROFILER )
	if( index->io_handle->profiler != NULL )
	{
		if( libfsntfs_profiler_stop_timing(
		     index->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     index_entry_offset,
		     index->index_entry_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to stop timing.",
			 function );

			return( -1 );
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	return( 1 );
}

/* Reads the index sub nodes
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read_sub_nodes(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfcache_cache_t *sub_node_cache    = NULL;
	libfsntfs_index_node_t *sub_node     = NULL;
	libfsntfs_index_value_t *index_value = NULL;
	static char *function                = "libfsntfs_index_read_sub_nodes";
	off64_t index_entry_offset           = 0;
	uint32_t index_value_flags           = 0;
	int element_file_index               = 0;
	int element_index                    = 0;
	int index_value_entry                = 0;
	int number_of_index_values           = 0;

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
	if( index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSNTFS_MAXIMUM_INDEX_NODE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
        }
	if( libfsntfs_index_node_get_number_of_values(
	     index_node,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values from index node.",
		 function );

		goto on_error;
	}
	/* Use a local cache to prevent cache invalidation of index node
	 * when reading sub nodes.
	 */
	if( libfcache_cache_initialize(
	     &sub_node_cache,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub node cache.",
		 function );

		goto on_error;
	}
	for( index_value_entry = 0;
	     index_value_entry < number_of_index_values;
	     index_value_entry++ )
	{
		if( libfsntfs_index_node_get_value_by_index(
		     index_node,
		     index_value_entry,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d from index node.",
			 function,
			 index_value_entry );

			goto on_error;
		}
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_HAS_SUB_NODE ) != 0 )
		{
			if( index_value->sub_node_vcn > (uint64_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: node index value: %d sub node VCN value out of bounds.",
				 function,
				 index_value_entry );

				goto on_error;
			}
			index_entry_offset = (off64_t) ( index_value->sub_node_vcn * index->io_handle->cluster_block_size );

			if( libfsntfs_index_get_sub_node(
			     index,
			     file_io_handle,
			     sub_node_cache,
			     index_entry_offset,
			     (int) index_value->sub_node_vcn,
			     &sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub node with VCN: %d at offset: 0x%08" PRIx64 ".",
				 function,
				 (int) index_value->sub_node_vcn,
				 index_entry_offset );

				goto on_error;
			}
			if( libfsntfs_index_read_sub_nodes(
			     index,
			     file_io_handle,
			     sub_node,
			     recursion_depth + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read sub nodes of index entry with VCN: %d at offset: 0x%08" PRIx64 ".",
				 function,
				 (int) index_value->sub_node_vcn,
				 index_entry_offset );

				goto on_error;
			}
		}
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
		{
			break;
		}
		/* The element file index value contains the index value entry + 1
		 */
		element_file_index = index_value_entry + 1;
		index_value_flags  = 0;

		if( index_node == index->root_node )
		{
			index_value_flags = LIBFSNTFS_INDEX_VALUE_LIST_FLAG_STORED_IN_ROOT;
		}
		/* Add the index values containing data in a depth first manner
		 * This will create a sorted 'list' of the index values
		 */
		if( libfdata_list_append_element(
		     index->index_value_list,
		     &element_index,
		     element_file_index,
		     index_value->offset,
		     (size64_t) index_value->size,
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
	if( libfcache_cache_free(
	     &sub_node_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sub node cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sub_node_cache != NULL )
	{
		libfcache_cache_free(
		 &sub_node_cache,
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
     libfdata_cache_t *cache,
     int element_file_index,
     off64_t index_value_offset,
     size64_t element_size LIBFSNTFS_ATTRIBUTE_UNUSED,
     uint32_t index_value_flags,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_index_node_t *sub_node     = NULL;
	libfsntfs_index_value_t *index_value = NULL;
	static char *function                = "libfsntfs_index_read_index_value_element_data";
	off64_t index_entry_offset           = 0;
	off64_t sub_node_vcn                 = 0;
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
	if( index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index - missing IO handle.",
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

		return( -1 );
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
	/* The element file index value contains the index value entry + 1
	 */
	element_file_index -= 1;

	if( ( index_value_flags & LIBFSNTFS_INDEX_VALUE_LIST_FLAG_STORED_IN_ROOT ) != 0 )
	{
		if( libfsntfs_index_node_get_value_by_index(
		     index->root_node,
		     element_file_index,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index value: %d from root node.",
			 function,
			 element_file_index );

			return( -1 );
		}
	}
	else
	{
		sub_node_vcn       = index_value_offset / index->index_entry_size;
		index_entry_offset = (off64_t) ( sub_node_vcn * index->io_handle->cluster_block_size );

		if( libfsntfs_index_get_sub_node(
		     index,
		     file_io_handle,
		     index->index_node_cache,
		     index_entry_offset,
		     (int) sub_node_vcn,
		     &sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node with VCN: %d at offset: 0x%08" PRIx64 ".",
			 function,
			 (int) sub_node_vcn,
			 index_entry_offset );

			return( -1 );
		}
		if( libfsntfs_index_node_get_value_by_index(
		     sub_node,
		     element_file_index,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index value: %d from sub node.",
			 function,
			 element_file_index );

			return( -1 );
		}
	}
/* TODO remove free function after pinning to libfcache >= 20191115 */
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) index_value,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_value_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index value as element value.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	     (libfdata_cache_t *) index->index_value_cache,
	     index_value_entry,
	     (intptr_t **) index_value,
	     LIBFDATA_READ_FLAG_IGNORE_CACHE,
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

/* Compares the name with an UTF-8 encoded string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libfsntfs_index_compare_name_with_utf8_string(
     libfsntfs_index_t *index,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_compare_name_with_utf8_string";
	int result            = 0;

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
	result = libuna_utf8_string_compare_with_utf8_stream(
		  utf8_string,
		  utf8_string_length,
		  index->name,
		  index->name_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare index name with UTF-8 string.",
		 function );

		return( -1 );
	}
	else if( result == LIBUNA_COMPARE_EQUAL )
	{
		return( 1 );
	}
	return( 0 );
}

