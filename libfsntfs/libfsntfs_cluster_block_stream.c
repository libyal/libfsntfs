/*
 * The cluster block stream functions
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_compressed_block.h"
#include "libfsntfs_compressed_block_descriptor.h"
#include "libfsntfs_data_run.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_unused.h"

/* Creates a data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_data_handle_initialize(
     libfsntfs_cluster_block_stream_data_handle_t **data_handle,
     libfsntfs_io_handle_t *io_handle,
     size64_t data_size,
     const uint8_t *resident_data,
     size_t compression_unit_size,
     uint16_t data_flags,
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
	if( ( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
	 && ( resident_data == NULL ) )
	{
		if( libcdata_array_initialize(
		     &( ( *data_handle )->compressed_block_descriptors_array ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create compressed block descriptors array.",
			 function );

			goto on_error;
		}
		if( libfdata_vector_initialize(
		     &( ( *data_handle )->compressed_block_vector ),
		     (size64_t) compression_unit_size,
		     (intptr_t *) ( *data_handle )->compressed_block_descriptors_array,
		     NULL,
		     NULL,
		     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_compressed_block_read_element_data,
		     NULL,
		     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create compressed block vector.",
			 function );

			goto on_error;
		}
	}
	( *data_handle )->io_handle             = io_handle;
	( *data_handle )->data_size             = data_size;
	( *data_handle )->resident_data         = resident_data;
	( *data_handle )->compression_unit_size = compression_unit_size;
	( *data_handle )->data_flags            = data_flags;

	return( 1 );

on_error:
	if( *data_handle != NULL )
	{
		if( ( *data_handle )->compressed_block_descriptors_array != NULL )
		{
			libcdata_array_free(
			 &( ( *data_handle )->compressed_block_descriptors_array ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compressed_block_descriptor_free,
			 NULL );
		}
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
		/* IO handle is freed elsewhere
		 */
		if( ( *data_handle )->compressed_block_descriptors_array != NULL )
		{
			if( libcdata_array_free(
			     &( ( *data_handle )->compressed_block_descriptors_array ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compressed_block_descriptor_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free compressed block descriptors array.",
				 function );

				result = -1;
			}
		}
		if( ( *data_handle )->compressed_block_vector != NULL )
		{
			if( libfdata_vector_free(
			     &( ( *data_handle )->compressed_block_vector ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free compressed block vector.",
				 function );

				result = -1;
			}
		}
		if( ( *data_handle )->cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *data_handle )->cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free cache.",
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
	     source_data_handle->io_handle,
	     source_data_handle->data_size,
	     source_data_handle->resident_data,
	     source_data_handle->compression_unit_size,
	     source_data_handle->data_flags,
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
/* TODO clone vectors and array */
	if( libfcache_cache_clone(
	     &( ( *destination_data_handle )->cache ),
	     source_data_handle->cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_data_handle != NULL )
	{
		libfsntfs_cluster_block_stream_data_handle_free(
		 destination_data_handle,
		 NULL );
	}
	return( -1 );
}

/* Reads data from the current offset into a buffer
 * Callback for the cluster block stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_cluster_block_stream_data_handle_read_segment_data(
         libfsntfs_cluster_block_stream_data_handle_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error )
{
	libfsntfs_compressed_block_t *compressed_block = NULL;
	static char *function                          = "libfsntfs_cluster_block_stream_data_handle_read_segment_data";
	off64_t segment_offset                         = 0;
	size64_t segment_size                          = 0;
	ssize_t read_count                             = 0;

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
	if( data_handle->current_segment_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data handle - current segment offset value out of bounds.",
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
	if( data_handle->data_size == 0 )
	{
		return( 0 );
	}
	if( data_handle->resident_data != NULL )
	{
		if( data_handle->data_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data handle - data size value out of bounds.",
			 function );

			return( -1 );
		}
		if( (size64_t) data_handle->current_segment_offset >= data_handle->data_size )
		{
			return( 0 );
		}
		segment_size = data_handle->data_size - data_handle->current_segment_offset;

		if( segment_data_size > segment_size )
		{
			read_count = segment_size;
		}
		else
		{
			read_count = segment_data_size;
		}
		if( read_count > 0 )
		{
			if( memory_copy(
			     segment_data,
			     &( ( data_handle->resident_data )[ data_handle->current_segment_offset ] ),
			     (size_t) read_count ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy resident data.",
				 function );

				return( -1 );
			}
		}
	}
	else if( data_handle->compressed_block_vector != NULL )
	{
		if( libfdata_vector_get_segment_by_index(
		     data_handle->compressed_block_vector,
		     segment_index,
		     &segment_file_index,
		     &segment_offset,
		     &segment_size,
		     &segment_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve compressed block vector element index: %d.",
			 function,
			 segment_index );

			return( -1 );
		}
		if( data_handle->current_segment_offset < segment_offset )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data handle - current segment offset value out of bounds.",
			 function );

			return( -1 );
		}
		segment_offset = data_handle->current_segment_offset - segment_offset;

		if( (size64_t) segment_offset >= segment_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data handle - current segment offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( libfdata_vector_get_element_value_by_index(
		     data_handle->compressed_block_vector,
		     (intptr_t *) file_io_handle,
		     data_handle->cache,
		     segment_index,
		     (intptr_t **) &compressed_block,
		     read_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve compressed block: %d from vector.",
			 function,
			 segment_index );

			return( -1 );
		}
		if( compressed_block == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing compressed block: %d.",
			 function,
			 segment_index );

			return( -1 );
		}
		if( segment_offset >= (off64_t) compressed_block->data_size )
		{
			return( 0 );
		}
		read_count = compressed_block->data_size - segment_offset;

		if( (size64_t) read_count > segment_data_size )
		{
			read_count = segment_data_size;
		}
		if( read_count == 0 )
		{
			return( 0 );
		}
		if( memory_copy(
		     segment_data,
		     &( ( compressed_block->data )[ segment_offset ] ),
		     (size_t) read_count ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy cluster block data.",
			 function );

			return( -1 );
		}
	}
	else if( ( segment_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( memory_set(
		     segment_data,
		     0,
		     segment_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segment data.",
			 function );

			return( -1 );
		}
		read_count = (ssize_t) segment_data_size;
	}
	else
	{
		if( libbfio_handle_seek_offset(
		     file_io_handle,
		     data_handle->current_segment_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek segment offset: 0x%08" PRIx64 ".",
			 function,
			 data_handle->current_segment_offset );

			return( -1 );
		}
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              segment_data,
		              segment_data_size,
		              error );

		if( read_count != (ssize_t) segment_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read segment data.",
			 function );

			return( -1 );
		}
	}
	data_handle->current_segment_offset += read_count;

	return( read_count );
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
	data_handle->current_segment_offset = segment_offset;

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
	libfsntfs_cluster_block_stream_data_handle_t *data_handle            = NULL;
	libfsntfs_compressed_block_descriptor_t *compressed_block_descriptor = NULL;
	libfsntfs_data_run_t *data_run                                       = NULL;
	uint8_t *resident_data                                               = NULL;
	static char *function                                                = "libfsntfs_cluster_block_stream_initialize";
	off64_t attribute_data_vcn_offset                                    = 0;
	off64_t calculated_attribute_data_vcn_offset                         = 0;
	off64_t data_offset                                                  = 0;
	off64_t data_run_offset                                              = 0;
	size64_t attribute_data_size                                         = 0;
	size64_t attribute_data_vcn_size                                     = 0;
	size64_t data_run_size                                               = 0;
	size64_t data_segment_size                                           = 0;
	size64_t data_size                                                   = 0;
	size64_t valid_data_size                                             = 0;
	size_t compression_unit_size                                         = 0;
	size_t remaining_compression_unit_size                               = 0;
	uint16_t attribute_data_flags                                        = 0;
	uint16_t data_flags                                                  = 0;
	int attribute_index                                                  = 0;
	int compressed_block_descriptor_index                                = 0;
	int data_run_index                                                   = 0;
	int element_index                                                    = 0;
	int entry_index                                                      = 0;
	int number_of_data_runs                                              = 0;
	int result                                                           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *compression_unit_data_type                                     = NULL;
#endif

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
	if( libfsntfs_attribute_get_data(
	     attribute,
	     &resident_data,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute data",
		 function );

		goto on_error;
	}
	if( libfsntfs_attribute_get_valid_data_size(
	     attribute,
	     &valid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve valid data size",
		 function );

		goto on_error;
	}
	if( valid_data_size == 0 )
	{
		valid_data_size = data_size;
	}
	if( libfsntfs_attribute_get_data_flags(
	     attribute,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute data flags.",
		 function );

		goto on_error;
	}
	if( ( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
	 || ( resident_data != NULL ) )
	{
		if( valid_data_size != data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: unuspported valid data size value out of bounds.",
			 function );

			goto on_error;
		}
	}
	if( ( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
	 && ( resident_data == NULL ) )
	{
		if( libfsntfs_attribute_get_compression_unit_size(
		     attribute,
		     &compression_unit_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve compression unit size.",
			 function );

			goto on_error;
		}
		if( ( compression_unit_size == 0 )
		 || ( compression_unit_size > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compression unit size value out of bounds.",
			 function );

			goto on_error;
		}
	}
	if( libfsntfs_cluster_block_stream_data_handle_initialize(
	     &data_handle,
	     io_handle,
	     data_size,
	     resident_data,
	     compression_unit_size,
	     data_flags,
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
		if( ( attribute_index > 0 )
		 && ( data_handle->resident_data != NULL ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid data handle - resident data already set.",
			 function );

			data_handle = NULL;

			goto on_error;
		}
		if( (size64_t) data_offset > valid_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data offset value out of bounds.",
			 function );

			data_handle = NULL;

			goto on_error;
		}
		result = libfsntfs_attribute_get_data_vcn_range(
		          attribute,
		          (uint64_t *) &attribute_data_vcn_offset,
		          (uint64_t *) &attribute_data_vcn_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute data VCN range.",
			 function );

			data_handle = NULL;

			goto on_error;
		}
		else if( result == 0 )
		{
			if( libfdata_stream_append_segment(
			     *cluster_block_stream,
			     &element_index,
			     0,
			     0,
			     data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append resident data as cluster block stream segment.",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			data_offset += data_size;
		}
		else if( attribute_data_vcn_size == 0xffffffffffffffffULL )
		{
			if( libfsntfs_attribute_get_data_size(
			     attribute,
			     &attribute_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute data size",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			if( attribute_data_size != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data last VCN value out of bounds.",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			break;
		}
		else
		{
			if( attribute_data_vcn_size > (size64_t) ( ( INT64_MAX / io_handle->cluster_block_size ) - 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data last VCN value out of bounds.",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			if( attribute_data_vcn_offset > (off64_t) attribute_data_vcn_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data first VCN value out of bounds.",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			attribute_data_vcn_size   += 1;
			attribute_data_vcn_size   -= attribute_data_vcn_offset;
			attribute_data_vcn_offset *= io_handle->cluster_block_size;
			attribute_data_vcn_size   *= io_handle->cluster_block_size;

			if( attribute_data_vcn_offset != calculated_attribute_data_vcn_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data VCN offset value out of bounds.",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			if( libfsntfs_attribute_get_data_flags(
			     attribute,
			     &attribute_data_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute data flags.",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			if( data_flags != attribute_data_flags )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data flags value out of bounds.",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			if( libfsntfs_attribute_get_number_of_data_runs(
			     attribute,
			     &number_of_data_runs,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of data runs.",
				 function );

				data_handle = NULL;

				goto on_error;
			}
			for( data_run_index = 0;
			     data_run_index < number_of_data_runs;
			     data_run_index++ )
			{
				if( libfsntfs_attribute_get_data_run_by_index(
				     attribute,
				     data_run_index,
				     &data_run,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve data run: %d.",
					 function,
					 data_run_index );

					data_handle = NULL;

					goto on_error;
				}
				if( data_run == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing data run: %d.",
					 function,
					 data_run_index );

					data_handle = NULL;

					goto on_error;
				}
				data_run_offset = data_run->start_offset;
				data_run_size   = data_run->size;

				if( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) == 0 )
				{
					if( data_run_size > ( valid_data_size - data_offset ) )
					{
						data_run_size = valid_data_size - data_offset;
					}
/* TODO what if data run size is 0 ? */
					if( libfdata_stream_append_segment(
					     *cluster_block_stream,
					     &element_index,
					     0,
					     data_run_offset,
					     data_run_size,
					     data_run->range_flags,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append data run: %d as cluster block stream segment.",
						 function,
						 data_run_index );

						data_handle = NULL;

						goto on_error;
					}
					data_offset += data_run_size;

					if( ( valid_data_size != data_size )
					 && ( (size64_t) data_offset >= valid_data_size ) )
					{
						if( libfdata_stream_append_segment(
						     *cluster_block_stream,
						     &element_index,
						     0,
						     data_offset,
						     data_size - data_offset,
						     LIBFDATA_RANGE_FLAG_IS_SPARSE,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
							 "%s: unable to append data run: %d as cluster block stream segment.",
							 function,
							 data_run_index );

							data_handle = NULL;

							goto on_error;
						}
					}
				}
				else
				{
					while( data_run_size > 0 )
					{
						if( compressed_block_descriptor == NULL )
						{
							if( libfsntfs_compressed_block_descriptor_initialize(
							     &compressed_block_descriptor,
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
								 "%s: unable to create compressed block descriptor.",
								 function );

								data_handle = NULL;

								goto on_error;
							}
							remaining_compression_unit_size = compression_unit_size;
						}
						if( ( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
						 && ( remaining_compression_unit_size < compression_unit_size ) )
						{
							/* A sparse data run marks the end of a compression unit and
							 * should be at minimum the size of the remaining data in the compression unit
							 */
							if( data_run_size < (size64_t) remaining_compression_unit_size )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
								 "%s: invalid sparse data run: %d size value out of bounds.",
								 function,
								 data_run_index );

								data_handle = NULL;

								goto on_error;
							}
							compressed_block_descriptor->data_range_flags = LIBFDATA_RANGE_FLAG_IS_COMPRESSED;
						}
						else
						{
							compressed_block_descriptor->data_range_flags = data_run->range_flags;
						}
						if( data_run_size < remaining_compression_unit_size )
						{
							data_segment_size = data_run_size;
						}
						else
						{
							data_segment_size = (size64_t) remaining_compression_unit_size;
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
							{
								compression_unit_data_type = "sparse ";
							}
							else
							{
								compression_unit_data_type = "";
							}
							libcnotify_printf(
							 "%s: compression unit: %d %sdata segment offset: 0x%08" PRIx64 ", size: %" PRIu64 ".\n",
							 function,
							 compressed_block_descriptor_index,
							 compression_unit_data_type,
							 data_run_offset,
							 data_segment_size );
						}
#endif
						if( libfsntfs_compressed_block_descriptor_append_data_segment(
						     compressed_block_descriptor,
						     data_run_offset,
						     data_segment_size,
						     data_run->range_flags,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
							 "%s: unable to append data segment to compressed block descriptor: %d.",
							 function,
							 compressed_block_descriptor_index );

							data_handle = NULL;

							goto on_error;
						}
						if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) == 0 )
						{
							data_run_offset += data_segment_size;
						}
						data_run_size                   -= data_segment_size;
						remaining_compression_unit_size -= data_segment_size;

						if( remaining_compression_unit_size == 0 )
						{
#if defined( HAVE_DEBUG_OUTPUT )
							if( libcnotify_verbose != 0 )
							{
								if( libfsntfs_compressed_block_descriptor_print(
								     compressed_block_descriptor,
								     io_handle,
								     compressed_block_descriptor_index,
								     error ) != 1 )
								{
									libcerror_error_set(
									 error,
									 LIBCERROR_ERROR_DOMAIN_RUNTIME,
									 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
									 "%s: unable to print the compressed block descriptor: %d.",
									 function,
									 compressed_block_descriptor_index );

									data_handle = NULL;

									goto on_error;
								}
							}
#endif
							if( libfdata_vector_append_segment(
							     data_handle->compressed_block_vector,
							     &element_index,
							     0,
							     data_offset,
							     compression_unit_size,
							     compressed_block_descriptor->data_range_flags,
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
								 "%s: unable to append compression unit to compressed block vector: %d.",
								 function,
								 compressed_block_descriptor_index );

								data_handle = NULL;

								goto on_error;
							}
							if( libcdata_array_append_entry(
							     data_handle->compressed_block_descriptors_array,
							     &entry_index,
							     (intptr_t *) compressed_block_descriptor,
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
								 "%s: unable to append compressed block descriptor: %d to array.",
								 function,
								 compressed_block_descriptor_index );

								data_handle = NULL;

								goto on_error;
							}
							compressed_block_descriptor = NULL;

							/* The data stream segments are mapped directly to the vector elements to ease current offset calculation
							 */
							if( libfdata_stream_append_segment(
							     *cluster_block_stream,
							     &element_index,
							     0,
							     data_offset,
							     compression_unit_size,
							     0,
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
								 "%s: unable to append compressed block: %d as cluster block stream segment.",
								 function,
								 compressed_block_descriptor_index );

								data_handle = NULL;

								goto on_error;
							}
							compressed_block_descriptor_index++;

							/* Make sure to give every segment a unique offset */
							data_offset += compression_unit_size;
						}
					}
				}
			}
			calculated_attribute_data_vcn_offset = attribute_data_vcn_offset + (off64_t) attribute_data_vcn_size;
		}
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
		attribute_index++;
	}
	if( ( number_of_data_runs != 0 )
	 || ( resident_data != NULL ) )
	{
		if( libfdata_stream_set_mapped_size(
		     *cluster_block_stream,
		     data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set mapped cluster block stream size.",
			 function );

			data_handle = NULL;

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( ( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
	 && ( resident_data == NULL ) )
	{
		if( libfcache_cache_initialize(
		     &( data_handle->cache ),
		     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_COMPRESSION_UNITS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create cache.",
			 function );

			data_handle = NULL;

			goto on_error;
		}
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
		libfsntfs_cluster_block_stream_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

