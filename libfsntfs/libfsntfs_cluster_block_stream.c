/*
 * The cluster block stream functions
 *
 * Copyright (C) 2010-2015, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_data_run.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_unused.h"

/* Creates a data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_data_handle_initialize(
     libfsntfs_cluster_block_stream_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_stream_data_handle_initialize";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle value already set.",
		 function );

		return( -1 );
	}
	*data_handle = memory_allocate_structure(
	                libfsntfs_cluster_block_stream_data_handle_t );

	if( *data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_handle,
	     0,
	     sizeof( libfsntfs_cluster_block_stream_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		memory_free(
		 *data_handle );

		*data_handle = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *data_handle != NULL )
	{
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( -1 );
}

/* Frees a data handle
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_data_handle_free(
     libfsntfs_cluster_block_stream_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_stream_data_handle_free";
	int result            = 1;

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		if( ( *data_handle )->cluster_block_vector != NULL )
		{
			if( libfdata_vector_free(
			     &( ( *data_handle )->cluster_block_vector ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free cluster block vector.",
				 function );

				result = -1;
			}
		}
		if( ( *data_handle )->cluster_block_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *data_handle )->cluster_block_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free cluster block cache.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( result );
}

/* Clones (duplicates) the data handle
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_data_handle_clone(
     libfsntfs_cluster_block_stream_data_handle_t **destination_data_handle,
     libfsntfs_cluster_block_stream_data_handle_t *source_data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_stream_data_handle_clone";

	if( destination_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination data handle.",
		 function );

		return( -1 );
	}
	if( *destination_data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination data handle value already set.",
		 function );

		return( -1 );
	}
	if( source_data_handle == NULL )
	{
		*destination_data_handle = NULL;

		return( 1 );
	}
	if( libfsntfs_cluster_block_stream_data_handle_initialize(
	     destination_data_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination data handle.",
		 function );

		goto on_error;
	}
	if( *destination_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination data handle.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_clone(
	     &( ( *destination_data_handle )->cluster_block_vector ),
	     source_data_handle->cluster_block_vector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination cluster block vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_clone(
	     &( ( *destination_data_handle )->cluster_block_cache ),
	     source_data_handle->cluster_block_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination cluster block cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_data_handle != NULL )
	{
		if( ( *destination_data_handle )->cluster_block_vector != NULL )
		{
			libfdata_vector_free(
			 &( ( *destination_data_handle )->cluster_block_vector ),
			 NULL );
		}
		memory_free(
		 *destination_data_handle );

		*destination_data_handle = NULL;
	}
	return( -1 );
}

/* Reads data from the current offset into a buffer
 * Callback for the cluster block stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_cluster_block_stream_data_handle_read_segment_data(
         libfsntfs_cluster_block_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t read_flags,
         libcerror_error_t **error )
{
	libfsntfs_cluster_block_t *cluster_block = NULL;
	static char *function                    = "libfsntfs_cluster_block_stream_data_handle_read_segment_data";
	off64_t element_data_offset              = 0;
	size_t read_size                         = 0;
	size_t segment_data_offset               = 0;
	int element_index                        = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_flags )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment data.",
		 function );

		return( -1 );
	}
	if( segment_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_index_at_offset(
	     data_handle->cluster_block_vector,
	     data_handle->current_offset,
	     &element_index,
	     &element_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cluster block vector element index for offset: 0x%08" PRIx64 ".",
		 function,
		 data_handle->current_offset );

		return( -1 );
	}
	if( element_data_offset > (off64_t) data_handle->cluster_block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element data offset value out of bounds.",
		 function );

		return( -1 );
	}
	while( segment_data_size > 0 )
	{
		if( libfdata_vector_get_element_value_by_index(
		     data_handle->cluster_block_vector,
		     (intptr_t *) file_io_handle,
		     data_handle->cluster_block_cache,
		     element_index,
		     (intptr_t **) &cluster_block,
		     read_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cluster block: %d from vector.",
			 function,
			 element_index );

			return( -1 );
		}
		if( cluster_block == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing cluster block: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		read_size = (size_t) cluster_block->data_size;

		if( element_data_offset > 0 )
		{
			read_size -= element_data_offset;
		}
		if( segment_data_size < (size64_t) read_size )
		{
			read_size = (size_t) segment_data_size;
		}
		if( memory_copy(
		     &( segment_data[ segment_data_offset ] ),
		     &( cluster_block->data[ element_data_offset ] ),
		     read_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data run data.",
			 function );

			return( -1 );
		}
		segment_data_offset += read_size;
		segment_data_size   -= read_size;
		element_data_offset  = 0;

		element_index++;

		data_handle->current_offset += read_size;
	}
	return( (ssize_t) segment_data_offset );
}

/* Seeks a certain offset of the data
 * Callback for the cluster block stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsntfs_cluster_block_stream_data_handle_seek_segment_offset(
         libfsntfs_cluster_block_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_stream_data_handle_seek_segment_offset";

	LIBFSNTFS_UNREFERENCED_PARAMETER( file_io_handle )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_file_index )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment offset value out of bounds.",
		 function );

		return( -1 );
	}
	data_handle->current_offset = segment_offset;

	return( segment_offset );
}

/* Creates a cluster block stream
 * Make sure the value cluster_block_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_initialize(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	libfsntfs_cluster_block_stream_data_handle_t *data_handle = NULL;
	libfsntfs_data_run_t *data_run                            = NULL;
	static char *function                                     = "libfsntfs_cluster_block_stream_initialize";
	off64_t segment_data_offset                               = 0;
	int attribute_index                                       = 0;
	int entry_index                                           = 0;
	int number_of_entries                                     = 0;
	int segment_index                                         = 0;

	if( cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block stream.",
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
	if( libfsntfs_cluster_block_stream_data_handle_initialize(
	     &data_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data handle.",
		 function );

		goto on_error;
	}
	data_handle->cluster_block_size = io_handle->cluster_block_size;

	if( libfdata_vector_initialize(
	     &( data_handle->cluster_block_vector ),
	     (size64_t) data_handle->cluster_block_size,
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_cluster_block_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block vector.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     cluster_block_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_cluster_block_stream_data_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfsntfs_cluster_block_stream_data_handle_clone,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_cluster_block_stream_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsntfs_cluster_block_stream_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create stream.",
		 function );

		goto on_error;
	}
	while( attribute != NULL )
	{
/* TODO check VCN of previous attribute? */
		if( libfsntfs_attribute_get_number_of_data_runs(
		     attribute,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d number of data runs.",
			 function,
			 attribute_index );

			data_handle = NULL;

			goto on_error;
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( libfsntfs_attribute_get_data_run_by_index(
			     attribute,
			     entry_index,
			     &data_run,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d data runs array entry: %d.",
				 function,
				 attribute_index,
				 entry_index );

				data_handle = NULL;

				goto on_error;
			}
			if( data_run == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing attribute: %d data run: %d.",
				 function,
				 attribute_index,
				 entry_index );

				data_handle = NULL;

				goto on_error;
			}
/* TODO can 2 sparse ranges conflict ? */
			if( libfdata_vector_append_segment(
			     data_handle->cluster_block_vector,
			     &segment_index,
			     0,
			     data_run->start_offset,
			     data_run->size,
			     data_run->range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append attribute: %d data run: %d vector segment.",
				 function,
				 attribute_index,
				 entry_index );

				data_handle = NULL;

				goto on_error;
			}
/* TODO handle the last segment that the stream remains bound to data size, since the vector will always map to the next element size */

			/* The data stream segments are mapped directly to the vector segment to ease current offset calculation
			 */
			if( libfdata_stream_append_segment(
			     *cluster_block_stream,
			     &segment_index,
			     0,
			     segment_data_offset,
			     data_run->size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append attribute: %d data runs stream segment: %d.",
				 function,
				 attribute_index,
				 entry_index );

				data_handle = NULL;

				goto on_error;
			}
			segment_data_offset += data_run->size;
		}
		attribute_index++;

		if( libfsntfs_attribute_get_chained_attribute(
		     attribute,
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chained attribute: %d.",
			 function,
			 attribute_index );

			data_handle = NULL;

			goto on_error;
		}
	}
	if( libfcache_cache_initialize(
	     &( data_handle->cluster_block_cache ),
	     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_CLUSTER_BLOCKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *cluster_block_stream != NULL )
	{
		libfdata_stream_free(
		 cluster_block_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		if( data_handle->cluster_block_vector != NULL )
		{
			libfdata_vector_free(
			 &( data_handle->cluster_block_vector ),
			 NULL );
		}
		libfsntfs_cluster_block_stream_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

