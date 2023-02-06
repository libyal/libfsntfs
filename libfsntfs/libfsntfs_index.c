/*
 * Index functions
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
#include "libfsntfs_mft_entry.h"
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
	 || ( name_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
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
		/* The io_handle, index_root_attribute, index_allocation_attribute and bitmap_attribute references are freed elsewhere
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
		if( ( *index )->bitmap_values != NULL )
		{
			if( libfsntfs_bitmap_values_free(
			     &( ( *index )->bitmap_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free bitmap values.",
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

/* Reads the index
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_index_read(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfsntfs_mft_attribute_t *bitmap_attribute           = NULL;
	libfsntfs_mft_attribute_t *index_allocation_attribute = NULL;
	libfsntfs_mft_attribute_t *index_root_attribute       = NULL;
	libfsntfs_mft_attribute_t *mft_attribute              = NULL;
	static char *function                                 = "libfsntfs_index_read";
	uint32_t attribute_type                               = 0;
	uint32_t index_entry_size                             = 0;
	int attribute_index                                   = 0;
	int number_of_attributes                              = 0;
	int result                                            = 0;

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
	if( libfsntfs_mft_entry_get_number_of_attributes(
	     mft_entry,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes.",
		 function );

		return( -1 );
	}
	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libfsntfs_mft_entry_get_attribute_by_index(
		     mft_entry,
		     attribute_index,
		     &mft_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT attribute: %d.",
			 function,
			 attribute_index );

			return( -1 );
		}
		result = libfsntfs_mft_attribute_compare_name_with_utf8_string(
		          mft_attribute,
		          index->name,
		          index->name_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with attribute: %d name.",
			 function,
			 attribute_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libfsntfs_mft_attribute_get_type(
			     mft_attribute,
			     &attribute_type,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute type.",
				 function );

				return( -1 );
			}
			switch( attribute_type )
			{
				case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
					if( libfsntfs_mft_attribute_append_to_chain(
					     &bitmap_attribute,
					     mft_attribute,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append attribute to bitmap attribute chain.",
						 function );

						return( -1 );
					}
					break;

				case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
					if( libfsntfs_mft_attribute_append_to_chain(
					     &index_allocation_attribute,
					     mft_attribute,
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
					break;

				case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
					index_root_attribute = mft_attribute;
					break;

				default:
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: unsupported index attribute type: 0x%08" PRIx32 "\n",
						 function,
						 attribute_type );
					}
#endif
					break;
			}
		}
	}
	if( index_root_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_index_read_root_header(
	     index,
	     index_root_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index root header.",
		 function );

		goto on_error;
	}
	/* The index does not necessarily have a $BITMAP attribute
	 */
	if( bitmap_attribute != NULL )
	{
		if( libfsntfs_index_read_bitmap(
		     index,
		     file_io_handle,
		     bitmap_attribute,
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
	if( libfsntfs_index_read_root_node(
	     index,
	     index_root_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index root node.",
		 function );

		goto on_error;
	}
	/* The index does not necessarily have an $INDEX_ALLOCATION attribute
	 */
	if( index_allocation_attribute != NULL )
	{
		if( libfsntfs_index_root_header_get_index_entry_size(
		     index->root_header,
		     &index_entry_size,
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
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( index_entry_size != index->io_handle->index_entry_size )
			{
				libcnotify_printf(
				 "%s: mismatch in index entry size (in index root header: %" PRIu32 ", in volume header: %" PRIu32 ").",
				 function,
				 index_entry_size,
				 index->io_handle->index_entry_size );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( index_entry_size > index->io_handle->index_entry_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid index entry size: %" PRIu32 " in index root header value exceeds value in volume header: %" PRIu32 ".",
			 function,
			 index_entry_size,
			 index->io_handle->index_entry_size );

			goto on_error;
		}
		if( libfsntfs_index_entry_vector_initialize(
		     &( index->index_entry_vector ),
		     index->io_handle,
		     index_allocation_attribute,
		     index_entry_size,
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
	return( -1 );
}

/* Reads the index root header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read_root_header(
     libfsntfs_index_t *index,
     libfsntfs_mft_attribute_t *index_root_attribute,
     libcerror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "libfsntfs_index_read_root_header";
	size_t data_size      = 0;
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
	if( index->root_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index - root header value already set.",
		 function );

		return( -1 );
	}
	result = libfsntfs_mft_attribute_data_is_resident(
	          index_root_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if index root attribute data is resident.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported non-resident index root attribute.",
		 function );

		return( 1 );
	}
	if( libfsntfs_mft_attribute_get_resident_data(
	     index_root_attribute,
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
	return( 1 );

on_error:
	if( index->root_header != NULL )
	{
		libfsntfs_index_root_header_free(
		 &( index->root_header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the index root node
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_read_root_node(
     libfsntfs_index_t *index,
     libfsntfs_mft_attribute_t *index_root_attribute,
     libcerror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "libfsntfs_index_read_root_node";
	size_t data_offset    = 0;
	size_t data_size      = 0;
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
	result = libfsntfs_mft_attribute_data_is_resident(
	          index_root_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if index root attribute data is resident.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported non-resident index root attribute.",
		 function );

		return( 1 );
	}
	if( libfsntfs_mft_attribute_get_resident_data(
	     index_root_attribute,
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
	data_offset = sizeof( fsntfs_index_root_header_t );

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
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( index->root_node != NULL )
	{
		libfsntfs_index_node_free(
		 &( index->root_node ),
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
     libfsntfs_mft_attribute_t *bitmap_attribute,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function     = "libfsntfs_index_read_bitmap";
	uint32_t index_entry_size = 0;

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
	if( index->bitmap_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index - bitmap values already set.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_root_header_get_index_entry_size(
	     index->root_header,
	     &index_entry_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index entry size from root header.",
		 function );

		goto on_error;
	}
	if( libfsntfs_bitmap_values_initialize(
	     &( index->bitmap_values ),
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
	     index->bitmap_values,
	     bitmap_attribute,
	     index->io_handle,
	     file_io_handle,
	     index_entry_size,
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
	return( 1 );

on_error:
	if( index->bitmap_values != NULL )
	{
		libfsntfs_bitmap_values_free(
		 &( index->bitmap_values ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the index entry size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_get_index_entry_size(
     libfsntfs_index_t *index,
     uint32_t *index_entry_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_get_index_entry_size";

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
	if( libfsntfs_index_root_header_get_index_entry_size(
	     index->root_header,
	     index_entry_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index entry size from root header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the attribute type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_get_attribute_type(
     libfsntfs_index_t *index,
     uint32_t *attribute_type,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_get_attribute_type";

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
	if( libfsntfs_index_root_header_get_attribute_type(
	     index->root_header,
	     attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute type from root header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the collation type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_get_collation_type(
     libfsntfs_index_t *index,
     uint32_t *collation_type,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_get_collation_type";

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
	if( libfsntfs_index_root_header_get_collation_type(
	     index->root_header,
	     collation_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve collation type from root header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Determines if a sub node is allocated
 * Returns 1 if allocated, 0 if not or -1 on error
 */
int libfsntfs_index_sub_node_is_allocated(
     libfsntfs_index_t *index,
     int sub_node_vcn,
     libcerror_error_t **error )
{
	static char *function       = "libfsntfs_index_sub_node_is_allocated";
	off64_t sub_node_vcn_offset = 0;
	int result                  = 0;

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
	if( index->bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index - missing bitmap values.",
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
	sub_node_vcn_offset = (off64_t) sub_node_vcn * index->io_handle->cluster_block_size;

	result = libcdata_range_list_range_is_present(
	          index->bitmap_values->allocated_block_list,
	          sub_node_vcn_offset,
	          index->io_handle->cluster_block_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if range of sub node with VCN: %d is allocated.",
		 function,
		 sub_node_vcn );

		return( -1 );
	}
	return( result );
}

/* Retrieves a sub index node
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
	uint32_t index_entry_size        = 0;
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
		if( libfsntfs_index_root_header_get_index_entry_size(
		     index->root_header,
		     &index_entry_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index entry size.",
			 function );

			return( -1 );
		}
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
		 "%s: unable to retrieve index node from index entry with VCN: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 sub_node_vcn,
		 index_entry_offset,
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
		     index_entry_size,
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

