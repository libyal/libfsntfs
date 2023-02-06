/*
 * The NTFS volume header functions
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

#if !defined( _LIBFSNTFS_VOLUME_HEADER_H )
#define _LIBFSNTFS_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_volume_header libfsntfs_volume_header_t;

struct libfsntfs_volume_header
{
	/* The cluster block size
	 */
	size32_t cluster_block_size;

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
};

int libfsntfs_volume_header_initialize(
     libfsntfs_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfsntfs_volume_header_free(
     libfsntfs_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfsntfs_volume_header_read_data(
     libfsntfs_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_volume_header_read_file_io_handle(
     libfsntfs_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsntfs_volume_header_get_bytes_per_sector(
     libfsntfs_volume_header_t *volume_header,
     uint16_t *bytes_per_sector,
     libcerror_error_t **error );

int libfsntfs_volume_header_get_cluster_block_size(
     libfsntfs_volume_header_t *volume_header,
     size32_t *cluster_block_size,
     libcerror_error_t **error );

int libfsntfs_volume_header_get_mft_entry_size(
     libfsntfs_volume_header_t *volume_header,
     size32_t *mft_entry_size,
     libcerror_error_t **error );

int libfsntfs_volume_header_get_index_entry_size(
     libfsntfs_volume_header_t *volume_header,
     size32_t *index_entry_size,
     libcerror_error_t **error );

int libfsntfs_volume_header_get_volume_size(
     libfsntfs_volume_header_t *volume_header,
     size64_t *volume_size,
     libcerror_error_t **error );

int libfsntfs_volume_header_get_volume_serial_number(
     libfsntfs_volume_header_t *volume_header,
     uint64_t *volume_serial_number,
     libcerror_error_t **error );

int libfsntfs_volume_header_get_mft_offset(
     libfsntfs_volume_header_t *volume_header,
     off64_t *mft_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_VOLUME_HEADER_H ) */

