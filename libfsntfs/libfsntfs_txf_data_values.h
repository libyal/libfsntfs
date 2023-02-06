/*
 * The Transactional NTFS (TxF) data ($TXF_DATA) logged utility stream
 * attribute ($LOGGED_UTILITY_STREAM) values functions
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

#if !defined( _LIBFSNTFS_TXF_DATA_VALUES_H )
#define _LIBFSNTFS_TXF_DATA_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_txf_data_values libfsntfs_txf_data_values_t;

struct libfsntfs_txf_data_values
{
	/* The resource manager root file reference
	 */
	uint64_t rm_root_file_reference;

	/* The file identifier
	 */
	uint64_t file_identifier;

	/* The file data lsn
	 */
	uint64_t data_lsn;

	/* The file system metadata lsn
	 */
	uint64_t metadata_lsn;

	/* The directory index lsn
	 */
	uint64_t directory_index_lsn;
};

int libfsntfs_txf_data_values_initialize(
     libfsntfs_txf_data_values_t **txf_data_values,
     libcerror_error_t **error );

int libfsntfs_txf_data_values_free(
     libfsntfs_txf_data_values_t **txf_data_values,
     libcerror_error_t **error );

int libfsntfs_txf_data_values_read_data(
     libfsntfs_txf_data_values_t *txf_data_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_txf_data_values_read_from_mft_attribute(
     libfsntfs_txf_data_values_t *txf_data_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_TXF_DATA_VALUES_H ) */

