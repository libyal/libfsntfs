/*
 * OSS-Fuzz target for libfsntfs MFT metadata file type
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

/* Opens a MFT metadata file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_open_file_io_handle(
      libfsntfs_mft_metadata_file_t *mft_metadata_file,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libfsntfs_error_t **error );

#endif /* !defined( LIBFSNTFS_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 32 ];

	libbfio_handle_t *file_io_handle                 = NULL;
	libfsntfs_file_entry_t *file_entry               = NULL;
	libfsntfs_mft_metadata_file_t *mft_metadata_file = NULL;
	uint64_t file_reference                          = 0;
	uint64_t number_of_file_entries                  = 0;
	size_t utf8_string_size                          = 0;
	size_t utf16_string_size                         = 0;
	uint16_t flags                                   = 0;
	uint8_t major_version                            = 0;
	uint8_t minor_version                            = 0;

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
	if( libfsntfs_mft_metadata_file_initialize(
	     &mft_metadata_file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfsntfs_mft_metadata_file_open_file_io_handle(
	     mft_metadata_file,
	     file_io_handle,
	     LIBFSNTFS_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_mft_metadata_file_get_utf8_volume_name_size(
	     mft_metadata_file,
	     &utf8_string_size,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_mft_metadata_file_get_utf8_volume_name(
	     mft_metadata_file,
	     utf8_string,
	     64,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_mft_metadata_file_get_utf16_volume_name_size(
	     mft_metadata_file,
	     &utf16_string_size,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_mft_metadata_file_get_utf16_volume_name(
	     mft_metadata_file,
	     utf16_string,
	     32,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_mft_metadata_file_get_volume_version(
	     mft_metadata_file,
	     &major_version,
	     &minor_version,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_mft_metadata_file_get_volume_flags(
	     mft_metadata_file,
	     &flags,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_mft_metadata_file_get_number_of_file_entries(
	     mft_metadata_file,
	     &number_of_file_entries,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs;
	}
	if( number_of_file_entries > 0 )
	{
		if( libfsntfs_mft_metadata_file_get_file_entry_by_index(
		     mft_metadata_file,
		     0,
		     &file_entry,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_file_entry;
		}
		if( libfsntfs_file_entry_get_file_reference(
		     file_entry,
		     &file_reference,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_file_entry;
		}
on_error_libfsntfs_file_entry:
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	libfsntfs_mft_metadata_file_close(
	 mft_metadata_file,
	 NULL );

on_error_libfsntfs:
	libfsntfs_mft_metadata_file_free(
	 &mft_metadata_file,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
