/*
 * Attribute functions
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

#if !defined( _LIBFSNTFS_ATTRIBUTE_H )
#define _LIBFSNTFS_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_data_run.h"
#include "libfsntfs_extern.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_path_hint.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_internal_attribute libfsntfs_internal_attribute_t;

struct libfsntfs_internal_attribute
{
	/* The MFT attribute
	 */
	libfsntfs_mft_attribute_t *mft_attribute;

	/* The path hint
	 */
	libfsntfs_path_hint_t *path_hint;

	/* The item value
	 */
	intptr_t *value;

	/* The item free value function
	 */
	int (*free_value)(
	       intptr_t **value,
	       libcerror_error_t **error );

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsntfs_attribute_initialize(
     libfsntfs_attribute_t **attribute,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_free(
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_internal_attribute_free(
     libfsntfs_internal_attribute_t **internal_attribute,
     libcerror_error_t **error );

int libfsntfs_internal_attribute_read_value(
     libfsntfs_internal_attribute_t *internal_attribute,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_internal_attribute_get_type(
     libfsntfs_internal_attribute_t *internal_attribute,
     uint32_t *type,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_type(
     libfsntfs_attribute_t *attribute,
     uint32_t *type,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_data_flags(
     libfsntfs_attribute_t *attribute,
     uint16_t *data_flags,
     libcerror_error_t **error );

int libfsntfs_internal_attribute_get_value(
     libfsntfs_internal_attribute_t *internal_attribute,
     intptr_t **value,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_utf8_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_utf8_name(
     libfsntfs_attribute_t *attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_utf16_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_utf16_name(
     libfsntfs_attribute_t *attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_data_vcn_range(
     libfsntfs_attribute_t *attribute,
     uint64_t *data_first_vcn,
     uint64_t *data_last_vcn,
     libcerror_error_t **error );

int libfsntfs_internal_attribute_get_data_size(
     libfsntfs_internal_attribute_t *internal_attribute,
     size64_t *data_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_data_size(
     libfsntfs_attribute_t *attribute,
     size64_t *data_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_valid_data_size(
     libfsntfs_attribute_t *attribute,
     size64_t *valid_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_ATTRIBUTE_H ) */

