/*
 * Index entry entry functions
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

#include "libfsntfs_fixup_values.h"
#include "libfsntfs_index_entry.h"
#include "libfsntfs_index_entry_header.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

#include "fsntfs_index.h"

/* Creates an index entry
 * Make sure the value index_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_initialize(
     libfsntfs_index_entry_t **index_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_entry_initialize";

	if( index_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry.",
		 function );

		return( -1 );
	}
	if( *index_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index entry value already set.",
		 function );

		return( -1 );
	}
	*index_entry = memory_allocate_structure(
	                libfsntfs_index_entry_t );

	if( *index_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index_entry,
	     0,
	     sizeof( libfsntfs_index_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index entry.",
		 function );

		memory_free(
		 *index_entry );

		*index_entry = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *index_entry != NULL )
	{
		memory_free(
		 *index_entry );

		*index_entry = NULL;
	}
	return( -1 );
}

/* Frees an index entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_free(
     libfsntfs_index_entry_t **index_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_entry_free";
	int result            = 1;

	if( index_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry.",
		 function );

		return( -1 );
	}
	if( *index_entry != NULL )
	{
		if( ( *index_entry )->node != NULL )
		{
			if( libfsntfs_index_node_free(
			     &( ( *index_entry )->node ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index node.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *index_entry );

		*index_entry = NULL;
	}
	return( result );
}

/* Reads the index entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_read_file_io_handle(
     libfsntfs_index_entry_t *index_entry,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t index_entry_size,
     uint32_t index_entry_index,
     libcerror_error_t **error )
{
	libfsntfs_index_entry_header_t *index_entry_header = NULL;
	uint8_t *index_entry_data                          = NULL;
	static char *function                              = "libfsntfs_index_entry_read_file_io_handle";
	size_t data_offset                                 = 0;
	size_t index_node_size                             = 0;
	size_t index_values_offset                         = 0;
	size_t unknown_data_size                           = 0;
	ssize_t read_count                                 = 0;
	off64_t index_value_vcn_offset                     = 0;
	uint16_t fixup_values_offset                       = 0;
	uint16_t number_of_fixup_values                    = 0;

	if( index_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry.",
		 function );

		return( -1 );
	}
	if( index_entry->node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index entry - node value already set.",
		 function );

		return( -1 );
	}
	if( ( index_entry_size < ( sizeof( fsntfs_index_entry_header_t ) + sizeof( fsntfs_index_node_header_t ) ) )
	 || ( index_entry_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - index entry size value out of bounds.",
		 function );

		goto on_error;
	}
	index_entry_data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * index_entry_size );

	if( index_entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index entry data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading index entry: %" PRIu32 " at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 index_entry_index,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              index_entry_data,
	              (size_t) index_entry_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) index_entry_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index entry: %" PRIu32 " data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 index_entry_index,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( libfsntfs_index_entry_header_initialize(
	     &index_entry_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index entry header.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_entry_header_read_data(
	     index_entry_header,
	     index_entry_data,
	     (size_t) index_entry_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index entry header.",
		 function );

		goto on_error;
	}
	data_offset = sizeof( fsntfs_index_entry_header_t );

	if( libfsntfs_index_node_initialize(
	     &( index_entry->node ),
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
	     index_entry->node,
	     index_entry_data,
	     (size_t) index_entry_size,
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

	if( libfsntfs_index_entry_header_get_fixup_values_offset(
	     index_entry_header,
	     &fixup_values_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fix-up values offset.",
		 function );

		goto on_error;
	}
/* TODO check bounds of index_values_offset */
	index_values_offset = (size_t) index_entry->node->header->index_values_offset + sizeof( fsntfs_index_entry_header_t );

	if( fixup_values_offset > index_values_offset )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: fix-up values offset exceeds index values offset.",
		 function );

		goto on_error;
	}
	if( data_offset < fixup_values_offset )
	{
		unknown_data_size = (size_t) fixup_values_offset - data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unknown data:\n",
			 function );
			libcnotify_print_data(
			 &( index_entry_data[ data_offset ] ),
			 unknown_data_size,
			 0 );
		}
#endif
		data_offset += unknown_data_size;
	}
	if( libfsntfs_index_entry_header_get_number_of_fixup_values(
	     index_entry_header,
	     &number_of_fixup_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of fix-up values.",
		 function );

		goto on_error;
	}
	if( number_of_fixup_values > 0 )
	{
		if( libfsntfs_fixup_values_apply(
		     index_entry_data,
		     (size_t) index_entry_size,
		     fixup_values_offset,
		     number_of_fixup_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to apply fix-up values.",
			 function );

			goto on_error;
		}
		data_offset += 2 + ( (size_t) number_of_fixup_values * 2 );
	}
	if( libfsntfs_index_entry_header_free(
	     &index_entry_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free index entry header.",
		 function );

		goto on_error;
	}
	index_value_vcn_offset = (off64_t) index_entry_index * index_entry_size;

	if( libfsntfs_index_node_read_values(
	     index_entry->node,
	     index_value_vcn_offset,
	     index_entry_data,
	     (size_t) index_entry_size,
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
/* TODO refactor */
	index_node_size = (size_t) index_entry->node->header->size - sizeof( fsntfs_index_node_header_t );

	if( data_offset < (size_t) index_values_offset )
	{
		unknown_data_size = (size_t) index_values_offset - data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unknown data:\n",
			 function );
			libcnotify_print_data(
			 &( index_entry_data[ data_offset ] ),
			 unknown_data_size,
			 0 );
		}
#endif
		data_offset     += unknown_data_size;
		index_node_size -= unknown_data_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		data_offset += index_node_size;

		if( data_offset < (size_t) index_entry_size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( index_entry_data[ data_offset ] ),
			 (size_t) index_entry_size - data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif
	memory_free(
	 index_entry_data );

	return( 1 );

on_error:
	if( index_entry->node != NULL )
	{
		libfsntfs_index_node_free(
		 &( index_entry->node ),
		 NULL );
	}
	if( index_entry_header != NULL )
	{
		libfsntfs_index_entry_header_free(
		 &index_entry_header,
		 NULL );
	}
	if( index_entry_data != NULL )
	{
		memory_free(
		 index_entry_data );
	}
	return( -1 );
}

