/*
 * Index entry entry functions
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

#include "libfsntfs_data_run.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_index_entry.h"
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

const char fsntfs_index_entry_signature[ 4 ] = "INDX";

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
		if( ( *index_entry )->data != NULL )
		{
			memory_free(
			 ( *index_entry )->data );
		}
		memory_free(
		 *index_entry );

		*index_entry = NULL;
	}
	return( 1 );
}

/* Reads the index entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_read(
     libfsntfs_index_entry_t *index_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t index_entry_size,
     uint32_t index_entry_index LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_index_node_t *index_node          = NULL;
	static char *function                       = "libfsntfs_index_entry_read";
	size_t index_entry_data_offset              = 0;
	size_t index_entry_fixup_offset             = 0;
	size_t index_entry_fixup_placeholder_offset = 0;
	size_t unknown_data_size                    = 0;
	ssize_t read_count                          = 0;
	uint32_t index_node_size                    = 0;
	uint32_t index_values_offset                = 0;
	uint16_t fixup_value_index                  = 0;
	uint16_t fixup_values_offset                = 0;
	uint16_t number_of_fixup_values             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                        = 0;
	uint16_t value_16bit                        = 0;
#endif

	LIBFSNTFS_UNREFERENCED_PARAMETER( index_entry_index )

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
	if( index_entry->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index entry - data value already set.",
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
	if( index_entry_size < ( sizeof( fsntfs_index_entry_header_t ) + sizeof( fsntfs_index_entry_header_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - index entry size value out of bounds.",
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
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek index entry offset: 0x%08" PRIx64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	index_entry->data = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * index_entry_size );

	if( index_entry->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index entry data.",
		 function );

		goto on_error;
	}
	index_entry->data_size = (size_t) index_entry_size;

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              index_entry->data,
	              index_entry->data_size,
	              error );

	if( read_count != (ssize_t) index_entry->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index entry data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index entry header data:\n",
		 function );
		libcnotify_print_data(
		 index_entry->data,
		 sizeof( fsntfs_index_entry_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (fsntfs_index_entry_header_t *) index_entry->data )->signature,
	     fsntfs_index_entry_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid index entry signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_index_entry_header_t *) index_entry->data )->fixup_values_offset,
	 fixup_values_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_index_entry_header_t *) index_entry->data )->number_of_fixup_values,
	 number_of_fixup_values );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsntfs_index_entry_header_t *) index_entry->data )->signature[ 0 ],
		 ( (fsntfs_index_entry_header_t *) index_entry->data )->signature[ 1 ],
		 ( (fsntfs_index_entry_header_t *) index_entry->data )->signature[ 2 ],
		 ( (fsntfs_index_entry_header_t *) index_entry->data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: fixup values offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 fixup_values_offset );

		libcnotify_printf(
		 "%s: number of fixup values\t\t\t: %" PRIu16 "\n",
		 function,
		 number_of_fixup_values );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_index_entry_header_t *) index_entry->data )->journal_sequence_number,
		 value_64bit );
		libcnotify_printf(
		 "%s: journal sequence number\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_index_entry_header_t *) index_entry->data )->vcn,
		 value_64bit );
		libcnotify_printf(
		 "%s: VCN\t\t\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( number_of_fixup_values > 0 )
	{
		if( ( (size_t) fixup_values_offset < ( sizeof( fsntfs_index_entry_header_t ) + sizeof( fsntfs_index_node_header_t ) ) )
		 || ( (size_t) fixup_values_offset >= index_entry->data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: fixup values offset value out of bounds.",
			 function );

			goto on_error;
		}
	}
	index_entry_data_offset += sizeof( fsntfs_index_entry_header_t );

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
		      index_entry->data,
		      index_entry->data_size,
		      index_entry_data_offset,
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
	index_entry_data_offset += read_count;
	index_values_offset      = index_node->index_values_offset + (uint32_t) sizeof( fsntfs_index_entry_header_t );
	index_node_size          = index_node->size;

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
	if( (ssize_t) index_node_size < sizeof( fsntfs_index_node_header_t ) )
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

	if( index_values_offset < fixup_values_offset )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index values offset value exceeds fixup values offset.",
		 function );

		goto on_error;
	}
	if( number_of_fixup_values > 0 )
	{
		if( index_entry_data_offset < fixup_values_offset )
		{
			unknown_data_size = (size_t) fixup_values_offset - index_entry_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unknown data:\n",
				 function );
				libcnotify_print_data(
				 &( index_entry->data[ index_entry_data_offset ] ),
				 unknown_data_size,
				 0 );
			}
#endif
			index_entry_data_offset += unknown_data_size;
			index_node_size         -= (uint32_t) unknown_data_size;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: fixup values data:\n",
			 function );
			libcnotify_print_data(
			 &( index_entry->data[ index_entry_data_offset ] ),
			 2 + ( number_of_fixup_values * 2 ),
			 0 );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( index_entry->data[ index_entry_data_offset ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: fixup placeholder value\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
#endif
		index_entry_fixup_placeholder_offset = index_entry_data_offset;

		index_entry_data_offset += 2;
		index_node_size         -= 2;

		for( fixup_value_index = 0;
		     fixup_value_index < number_of_fixup_values;
		     fixup_value_index++ )
		{
			index_entry_fixup_offset  = fixup_value_index + 1;
			index_entry_fixup_offset *= io_handle->bytes_per_sector;
			index_entry_fixup_offset -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 &( index_entry->data[ index_entry_data_offset ] ),
				 value_16bit );
				libcnotify_printf(
				 "%s: fixup value: %" PRIu16 "\t\t\t\t: 0x%04" PRIx16 "\n",
				 function,
				 fixup_value_index,
				 value_16bit );
			}
#endif
			/* Ignore fixup values outside the available index entry data
			 */
			if( index_entry_fixup_offset >= index_entry->data_size )
			{
				index_entry_data_offset += 2;
				index_node_size         -= 2;

				continue;
			}
			if( ( index_entry->data[ index_entry_fixup_offset ] != index_entry->data[ index_entry_fixup_placeholder_offset ] )
			 && ( index_entry->data[ index_entry_fixup_offset + 1 ] != index_entry->data[ index_entry_fixup_placeholder_offset + 1 ] ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					byte_stream_copy_to_uint16_little_endian(
					 &( index_entry->data[ index_entry_fixup_offset ] ),
					 value_16bit );
					libcnotify_printf(
					 "%s: corruption detected - mismatch between placeholder and value at offset: %" PRIzd " (0x%04" PRIx16 ")\n",
					 function,
					 index_entry_fixup_offset,
					 value_16bit );
				}
#endif
/* TODO handle error */
			}
			index_entry->data[ index_entry_fixup_offset ]     = index_entry->data[ index_entry_data_offset ];
			index_entry->data[ index_entry_fixup_offset + 1 ] = index_entry->data[ index_entry_data_offset + 1 ];

			index_entry_data_offset += 2;
			index_node_size         -= 2;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	if( index_entry_data_offset < (size_t) index_values_offset )
	{
		unknown_data_size = (size_t) index_values_offset - index_entry_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unknown data:\n",
			 function );
			libcnotify_print_data(
			 &( index_entry->data[ index_entry_data_offset ] ),
			 unknown_data_size,
			 0 );
		}
#endif
		index_entry_data_offset += unknown_data_size;
		index_node_size         -= (uint32_t) unknown_data_size;
	}
	index_entry->values_data        = &( index_entry->data[ index_values_offset ] );
	index_entry->values_data_offset = (size_t) index_values_offset;
	index_entry->values_data_size   = (size_t) index_node_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		index_entry_data_offset += (size_t) index_node_size;

		if( index_entry_data_offset < index_entry->data_size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( index_entry->data[ index_entry_data_offset ] ),
			 index_entry->data_size - index_entry_data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif
	return( 1 );

on_error:
	if( index_node != NULL )
	{
		libfsntfs_index_node_free(
		 &index_node,
		 NULL );
	}
	if( index_entry->data != NULL )
	{
		memory_free(
		 index_entry->data );

		index_entry->data = NULL;
	}
	index_entry->data_size = 0;

	return( -1 );
}

/* Reads the index values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_read_index_values(
     libfsntfs_index_entry_t *index_entry,
     off64_t index_entry_offset,
     int *index_value_entry,
     libcdata_array_t **index_values_array,
     libcerror_error_t **error )
{
	libfsntfs_index_value_t *index_value = NULL;
	static char *function                = "libfsntfs_index_entry_read_index_values";
	size_t index_value_data_offset       = 0;
	ssize_t read_count                   = 0;
	int entry_index                      = 0;

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
	if( libcdata_array_initialize(
	     index_values_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index values array.",
		 function );

		goto on_error;
	}
	index_entry_offset += index_entry->values_data_offset;

	while( index_value_data_offset < index_entry->values_data_size )
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
			      index_entry_offset,
			      index_value_entry,
			      index_entry->values_data,
			      index_entry->values_data_size,
			      index_value_data_offset,
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
		index_value_data_offset += read_count;
		index_entry_offset      += read_count;

		if( libcdata_array_append_entry(
		     *index_values_array,
		     &entry_index,
		     (intptr_t *) index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append index value to array.",
			 function );

			goto on_error;
		}
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
		{
			break;
		}
		index_value = NULL;
	}
	index_value = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( index_value_data_offset < index_entry->values_data_size )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index values trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( index_entry->values_data[ index_value_data_offset ] ),
			 index_entry->values_data_size - index_value_data_offset,
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
	if( index_values_array != NULL )
	{
		libcdata_array_free(
		 index_values_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_value_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the index entry
 * Callback function for the index entry vector
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_read_element_data(
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
     off64_t index_entry_offset,
     size64_t index_entry_size,
     uint32_t element_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_index_entry_t *index_entry = NULL;
	static char *function                = "libfsntfs_index_entry_read_element_data";

	LIBFSNTFS_UNREFERENCED_PARAMETER( element_data_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( element_flags )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

	if( (uint64_t) element_index > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: element index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_entry_initialize(
	     &index_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index entry.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_entry_read(
	     index_entry,
	     io_handle,
	     file_io_handle,
	     index_entry_offset,
	     index_entry_size,
	     (uint32_t) element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index entry: %d.",
		 function,
		 element_index );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) index_entry,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_entry_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index entry as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( index_entry != NULL )
	{
		libfsntfs_index_entry_free(
		 &index_entry,
		 NULL );
	}
	return( -1 );
}

