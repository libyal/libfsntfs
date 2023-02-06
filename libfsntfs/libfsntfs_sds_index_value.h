/*
 * $SDS index value functions
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

#if !defined( _LIBFSNTFS_SDS_INDEX_VALUE_H )
#define _LIBFSNTFS_SDS_INDEX_VALUE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_sds_index_value libfsntfs_sds_index_value_t;

struct libfsntfs_sds_index_value
{
	/* Dummy
	 */
	int dummy;
};

int libfsntfs_sds_index_value_initialize(
     libfsntfs_sds_index_value_t **sds_index_value,
     libcerror_error_t **error );

int libfsntfs_sds_index_value_free(
     libfsntfs_sds_index_value_t **sds_index_value,
     libcerror_error_t **error );

int libfsntfs_sds_index_value_read_data(
     libfsntfs_sds_index_value_t *sds_index_value,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_SDS_INDEX_VALUE_H ) */

