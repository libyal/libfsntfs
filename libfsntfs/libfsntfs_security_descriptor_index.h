/*
 * Security descriptor index functions
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

#if !defined( _LIBFSNTFS_SECURITY_DESCRIPTOR_INDEX_H )
#define _LIBFSNTFS_SECURITY_DESCRIPTOR_INDEX_H

#include <common.h>
#include <types.h>

#include "libfsntfs_data_stream.h"
#include "libfsntfs_index.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_security_descriptor_values.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_security_descriptor_index libfsntfs_security_descriptor_index_t;

struct libfsntfs_security_descriptor_index
{
	/* The $SII index
	 */
	libfsntfs_index_t *sii_index;

	/* The $SDS data stream
	 */
	libfsntfs_data_stream_t *data_stream;
};

int libfsntfs_security_descriptor_index_initialize(
     libfsntfs_security_descriptor_index_t **security_descriptor_index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_attribute_t *data_attribute,
     libcerror_error_t **error );

int libfsntfs_security_descriptor_index_free(
     libfsntfs_security_descriptor_index_t **security_descriptor_index,
     libcerror_error_t **error );

int libfsntfs_security_descriptor_index_read_sii_index(
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error );

int libfsntfs_security_descriptor_index_get_entry_from_index_node_by_identifier(
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     uint32_t security_descriptor_identifier,
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     int recursion_depth,
     libcerror_error_t **error );

int libfsntfs_security_descriptor_index_get_entry_by_identifier(
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libbfio_handle_t *file_io_handle,
     uint32_t security_descriptor_identifier,
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_SECURITY_DESCRIPTOR_INDEX_H ) */

