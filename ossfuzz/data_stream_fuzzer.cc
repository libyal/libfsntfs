/*
 * OSS-Fuzz target for libfsntfs data_stream type
 *
 * Copyright (C) 2010-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libfsntfs.h"

#if !defined( LIBFSNTFS_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFSNTFS_EXTERN \
int libfsntfs_volume_open_file_io_handle(
      libfsntfs_volume_t *volume,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libfsntfs_error_t **error );

#endif /* !defined( LIBFSNTFS_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	uint8_t buffer[ 512 ];
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 32 ];

	libbfio_handle_t *file_io_handle            = NULL;
	libfsntfs_data_stream_t *data_stream        = NULL;
	libfsntfs_file_entry_t *file_entry          = NULL;
	libfsntfs_file_entry_t *root_directory      = NULL;
	libfsntfs_file_entry_t *sub_file_entry      = NULL;
	libfsntfs_volume_t *volume                  = NULL;
	size64_t extent_size                        = 0;
	size64_t stream_size                        = 0;
	size_t utf8_string_size                     = 0;
	size_t utf16_string_size                    = 0;
	off64_t extent_offset                       = 0;
	off64_t stream_offset                       = 0;
	uint32_t extent_flags                       = 0;
	int number_of_alternate_data_streams        = 0;
	int number_of_extents                       = 0;
	int number_of_sub_file_entries              = 0;
	int read_iterator                           = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfsntfs_volume_initialize(
	     &volume,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfsntfs_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     LIBFSNTFS_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_volume;
	}
	if( libfsntfs_volume_get_root_directory(
	     volume,
	     &root_directory,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_volume;
	}
	if( libfsntfs_file_entry_get_number_of_sub_file_entries(
	     root_directory,
	     &number_of_sub_file_entries,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_file_entry;
	}
	if( number_of_sub_file_entries > 0 )
	{
		if( libfsntfs_file_entry_get_sub_file_entry_by_index(
		     root_directory,
		     0,
		     &sub_file_entry,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_file_entry;
		}
		file_entry = sub_file_entry;
	}
	else
	{
		file_entry = root_directory;
	}
	if( libfsntfs_file_entry_get_number_of_alternate_data_streams(
	     file_entry,
	     &number_of_alternate_data_streams,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_file_entry;
	}
	if( number_of_alternate_data_streams > 0 )
	{
		if( libfsntfs_file_entry_get_alternate_data_stream_by_index(
		     file_entry,
		     0,
		     &data_stream,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_data_stream;
		}
		if( libfsntfs_data_stream_get_utf8_name_size(
		     data_stream,
		     &utf8_string_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_data_stream;
		}
		if( utf8_string_size > 0 )
		{
			if( libfsntfs_data_stream_get_utf8_name(
			     data_stream,
			     utf8_string,
			     64,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_data_stream;
			}
		}
		if( libfsntfs_data_stream_get_utf16_name_size(
		     data_stream,
		     &utf16_string_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_data_stream;
		}
		if( utf16_string_size > 0 )
		{
			if( libfsntfs_data_stream_get_utf16_name(
			     data_stream,
			     utf16_string,
			     32,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_data_stream;
			}
		}
		if( libfsntfs_data_stream_get_offset(
		     data_stream,
		     &stream_offset,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_data_stream;
		}
		if( libfsntfs_data_stream_get_size(
		     data_stream,
		     &stream_size,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_data_stream;
		}
		for( read_iterator = 0;
		     read_iterator < 128;
		     read_iterator++ )
		{
			if( stream_offset >= stream_size )
			{
				break;
			}
			if( libfsntfs_data_stream_read_buffer_at_offset(
			     data_stream,
			     buffer,
			     497,
			     stream_offset,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_data_stream;
			}
			stream_offset += 497;
		}
		if( libfsntfs_data_stream_get_number_of_extents(
		     data_stream,
		     &number_of_extents,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_data_stream;
		}
		if( number_of_extents > 0 )
		{
			if( libfsntfs_data_stream_get_extent_by_index(
			     data_stream,
			     0,
			     &extent_offset,
			     &extent_size,
			     &extent_flags,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_data_stream;
			}
		}
on_error_libfsntfs_data_stream:
		libfsntfs_data_stream_free(
		 &data_stream,
		 NULL );
	}
on_error_libfsntfs_file_entry:
	if( sub_file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	libfsntfs_file_entry_free(
	 &root_directory,
	 NULL );
	libfsntfs_volume_close(
	 volume,
	 NULL );

on_error_libfsntfs_volume:
	libfsntfs_volume_free(
	 &volume,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
