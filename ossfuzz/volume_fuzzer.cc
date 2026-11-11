/*
 * OSS-Fuzz target for libfsntfs volume type
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
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 32 ];

	libbfio_handle_t *file_io_handle = NULL;
	libfsntfs_volume_t *volume       = NULL;
	uint64_t number_of_file_entries  = 0;
	uint64_t serial_number           = 0;
	size_t utf8_string_size          = 0;
	size_t utf16_string_size         = 0;
	uint32_t cluster_block_size      = 0;
	uint32_t index_entry_size        = 0;
	uint32_t mft_entry_size          = 0;
	uint16_t bytes_per_sector        = 0;
	uint16_t flags                   = 0;
	uint8_t major_version            = 0;
	uint8_t minor_version            = 0;

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
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_has_bitlocker_drive_encryption(
	     volume,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_has_volume_shadow_snapshots(
	     volume,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_bytes_per_sector(
	     volume,
	     &bytes_per_sector,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_cluster_block_size(
	     volume,
	     &cluster_block_size,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_mft_entry_size(
	     volume,
	     &mft_entry_size,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_index_entry_size(
	     volume,
	     &index_entry_size,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_serial_number(
	     volume,
	     &serial_number,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_utf8_name_size(
	     volume,
	     &utf8_string_size,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_utf8_name(
	     volume,
	     utf8_string,
	     64,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_utf16_name_size(
	     volume,
	     &utf16_string_size,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_utf16_name(
	     volume,
	     utf16_string,
	     32,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_version(
	     volume,
	     &major_version,
	     &minor_version,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_flags(
	     volume,
	     &flags,
	     NULL ) == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( libfsntfs_volume_get_number_of_file_entries(
	     volume,
	     &number_of_file_entries,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs;
	}
	libfsntfs_volume_close(
	 volume,
	 NULL );

on_error_libfsntfs:
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
