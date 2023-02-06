/*
 * Cluster block data functions
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

#include "libfsntfs_cluster_block_data.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_unused.h"

/* Reads data from the current offset into a buffer
 * Callback for the cluster block data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_cluster_block_data_read_segment_data(
         intptr_t *data_handle LIBFSNTFS_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_data_read_segment_data";
	ssize_t read_count    = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( data_handle )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

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
	if( ( segment_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
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
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              segment_data,
		              segment_data_size,
		              error );

		if( read_count < 0 )
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
	return( read_count );
}

/* Seeks a certain offset of the data
 * Callback for the cluster block data stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsntfs_cluster_block_data_seek_segment_offset(
         intptr_t *data_handle LIBFSNTFS_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libfsntfs_cluster_block_data_seek_segment_offset";

	LIBFSNTFS_UNREFERENCED_PARAMETER( data_handle )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_file_index )

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
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     segment_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek segment offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 segment_offset,
		 segment_offset );

		return( -1 );
	}
	return( segment_offset );
}

