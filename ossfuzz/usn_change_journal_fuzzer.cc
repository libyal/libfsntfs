/*
 * OSS-Fuzz target for libfsntfs USN change journal type
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
	uint8_t usn_record[ 512 ];

	libbfio_handle_t *file_io_handle                      = NULL;
	libfsntfs_usn_change_journal_t *usn_change_journal    = NULL;
	libfsntfs_volume_t *volume                            = NULL;
	off64_t journal_offset                                = 0;
	ssize_t read_count                                    = 0;
	int read_iterator                                     = 0;
	int result                                            = 0;

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
	result = libfsntfs_volume_get_usn_change_journal(
	          volume,
	          &usn_change_journal,
	          NULL );

	if( result == -1 )
	{
		goto on_error_libfsntfs;
	}
	if( result == 1 )
	{
		if( libfsntfs_usn_change_journal_get_offset(
		     usn_change_journal,
		     &journal_offset,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_usn_change_journal;
		}
		for( read_iterator = 0;
		     read_iterator < 128;
		     read_iterator++ )
		{
			read_count = libfsntfs_usn_change_journal_read_usn_record(
		                     usn_change_journal,
		                     usn_record,
		                     512,
		                     NULL );

			if( read_count == -1 )
			{
				goto on_error_libfsntfs_usn_change_journal;
			}
			if( read_count == 0 )
			{
				break;
			}
		}
on_error_libfsntfs_usn_change_journal:
		libfsntfs_usn_change_journal_free(
		 &usn_change_journal,
		 NULL );
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
