/*
 * OSS-Fuzz target for libfsntfs file_entry type
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

	libbfio_handle_t *file_io_handle       = NULL;
	libfsntfs_file_entry_t *root_directory = NULL;
	libfsntfs_file_entry_t *sub_file_entry = NULL;
	libfsntfs_volume_t *volume             = NULL;
	uint64_t file_reference                = 0;
	uint64_t filetime                      = 0;
	uint64_t i30_size                      = 0;
	uint64_t journal_sequence_number       = 0;
	uint64_t parent_file_reference         = 0;
	size64_t extent_size                   = 0;
	size64_t file_size                     = 0;
	size_t security_descriptor_size        = 0;
	size_t utf8_string_size                = 0;
	size_t utf16_string_size               = 0;
	off64_t extent_offset                  = 0;
	off64_t file_offset                    = 0;
	uint32_t extent_flags                  = 0;
	uint32_t file_attribute_flags          = 0;
	uint32_t i30_file_attribute_flags      = 0;
	int name_attribute_index               = 0;
	int number_of_extents                  = 0;
	int number_of_sub_file_entries         = 0;
	int read_iterator                      = 0;
	int result                             = 0;

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
	if( libfsntfs_file_entry_is_empty(
	     root_directory,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs_root_directory;
	}
	if( libfsntfs_file_entry_is_allocated(
	     root_directory,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs_root_directory;
	}
	if( libfsntfs_file_entry_is_corrupted(
	     root_directory,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs_root_directory;
	}
	if( libfsntfs_file_entry_get_file_reference(
	     root_directory,
	     &file_reference,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_root_directory;
	}
	if( libfsntfs_file_entry_get_number_of_sub_file_entries(
	     root_directory,
	     &number_of_sub_file_entries,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_root_directory;
	}
	if( number_of_sub_file_entries > 0 )
	{
		if( libfsntfs_file_entry_get_sub_file_entry_by_index(
		     root_directory,
		     0,
		     &sub_file_entry,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_root_directory;
		}
		if( libfsntfs_file_entry_is_empty(
		     sub_file_entry,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_is_allocated(
		     sub_file_entry,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_is_corrupted(
		     sub_file_entry,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_file_reference(
		     sub_file_entry,
		     &file_reference,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_base_record_file_reference(
		     sub_file_entry,
		     &file_reference,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_parent_file_reference(
		     sub_file_entry,
		     &parent_file_reference,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_journal_sequence_number(
		     sub_file_entry,
		     &journal_sequence_number,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_creation_time(
		     sub_file_entry,
		     &filetime,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_modification_time(
		     sub_file_entry,
		     &filetime,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_access_time(
		     sub_file_entry,
		     &filetime,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_entry_modification_time(
		     sub_file_entry,
		     &filetime,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_file_attribute_flags(
		     sub_file_entry,
		     &file_attribute_flags,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_i30_file_reference(
		     sub_file_entry,
		     &file_reference,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_i30_creation_time(
		     sub_file_entry,
		     &filetime,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_i30_modification_time(
		     sub_file_entry,
		     &filetime,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_i30_access_time(
		     sub_file_entry,
		     &filetime,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_i30_entry_modification_time(
		     sub_file_entry,
		     &filetime,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_i30_size(
		     sub_file_entry,
		     &i30_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_i30_file_attribute_flags(
		     sub_file_entry,
		     &i30_file_attribute_flags,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_utf8_name_size(
		     sub_file_entry,
		     &utf8_string_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_utf8_name(
		     sub_file_entry,
		     utf8_string,
		     64,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_utf16_name_size(
		     sub_file_entry,
		     &utf16_string_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_utf16_name(
		     sub_file_entry,
		     utf16_string,
		     32,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		result = libfsntfs_file_entry_get_name_attribute_index(
		           sub_file_entry,
		           &name_attribute_index,
		           NULL );

		if( result == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( result == 1 )
		{
			if( libfsntfs_file_entry_get_parent_file_reference_by_attribute_index(
			     sub_file_entry,
			     name_attribute_index,
			     &parent_file_reference,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_utf8_name_size_by_attribute_index(
			     sub_file_entry,
			     name_attribute_index,
			     &utf8_string_size,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_utf8_name_by_attribute_index(
			     sub_file_entry,
			     name_attribute_index,
			     utf8_string,
			     64,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_utf16_name_size_by_attribute_index(
			     sub_file_entry,
			     name_attribute_index,
			     &utf16_string_size,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_utf16_name_by_attribute_index(
			     sub_file_entry,
			     name_attribute_index,
			     utf16_string,
			     32,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_utf8_path_hint_size(
			     sub_file_entry,
			     name_attribute_index,
			     &utf8_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_utf8_path_hint(
			     sub_file_entry,
			     name_attribute_index,
			     utf8_string,
			     64,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_utf16_path_hint_size(
			     sub_file_entry,
			     name_attribute_index,
			     &utf16_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_utf16_path_hint(
			     sub_file_entry,
			     name_attribute_index,
			     utf16_string,
			     32,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
		}
		if( libfsntfs_file_entry_is_symbolic_link(
		     sub_file_entry,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_utf8_symbolic_link_target_size(
		     sub_file_entry,
		     &utf8_string_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_utf8_symbolic_link_target(
		     sub_file_entry,
		     utf8_string,
		     64,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_utf16_symbolic_link_target_size(
		     sub_file_entry,
		     &utf16_string_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_utf16_symbolic_link_target(
		     sub_file_entry,
		     utf16_string,
		     32,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_security_descriptor_size(
		     sub_file_entry,
		     &security_descriptor_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_security_descriptor(
		     sub_file_entry,
		     utf8_string,
		     64,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_has_directory_entries_index(
		     sub_file_entry,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		result = libfsntfs_file_entry_has_default_data_stream(
		           sub_file_entry,
		           NULL );

		if( result == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_has_i30_entry(
		     sub_file_entry,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( libfsntfs_file_entry_get_number_of_sub_file_entries(
		     sub_file_entry,
		     &number_of_sub_file_entries,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_sub_file_entry;
		}
		if( result == 1 )
		{
			if( libfsntfs_file_entry_get_offset(
			     sub_file_entry,
			     &file_offset,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( libfsntfs_file_entry_get_size(
			     sub_file_entry,
			     &file_size,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			for( read_iterator = 0;
			     read_iterator < 128;
			     read_iterator++ )
			{
				if( file_offset >= file_size )
				{
					break;
				}
				if( libfsntfs_file_entry_read_buffer_at_offset(
				     sub_file_entry,
				     buffer,
				     497,
				     file_offset,
				     NULL ) == -1 )
				{
					goto on_error_libfsntfs_sub_file_entry;
				}
				file_offset += 497;
			}
			if( libfsntfs_file_entry_get_number_of_extents(
			     sub_file_entry,
			     &number_of_extents,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_sub_file_entry;
			}
			if( number_of_extents > 0 )
			{
				if( libfsntfs_file_entry_get_extent_by_index(
				     sub_file_entry,
				     0,
				     &extent_offset,
				     &extent_size,
				     &extent_flags,
				     NULL ) != 1 )
				{
					goto on_error_libfsntfs_sub_file_entry;
				}
			}
		}
on_error_libfsntfs_sub_file_entry:
		libfsntfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
on_error_libfsntfs_root_directory:
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
