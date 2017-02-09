/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBFSNTFS_IO_HANDLE_H )
#define _LIBFSNTFS_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *fsntfs_volume_file_system_signature;

typedef struct libfsntfs_io_handle libfsntfs_io_handle_t;

struct libfsntfs_io_handle
{
	/* The cluster block size
	 */
	size_t cluster_block_size;

	/* The number of bytes per sector
	 */
	uint16_t bytes_per_sector;

	/* The volume size
	 */
	size64_t volume_size;

	/* The volume serial number
	 */
	uint64_t volume_serial_number;

	/* The MFT entry size
	 */
	uint32_t mft_entry_size;

	/* The index entry size
	 */
	uint32_t index_entry_size;

	/* The offset of the MFT
	 */
	off64_t mft_offset;

	/* The offset of the mirror MFT
	 */
	off64_t mirror_mft_offset;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfsntfs_io_handle_initialize(
     libfsntfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsntfs_io_handle_free(
     libfsntfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsntfs_io_handle_clear(
     libfsntfs_io_handle_t *io_handle,
     libcerror_error_t **error );

int libfsntfs_io_handle_read_volume_header(
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_IO_HANDLE_H ) */

