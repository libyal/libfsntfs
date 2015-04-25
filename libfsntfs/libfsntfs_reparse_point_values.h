/*
 * Reparse point attribute ($REPARSE_POINT) values functions
 *
 * Copyright (C) 2010-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSNTFS_REPARSE_POINT_VALUES_H )
#define _LIBFSNTFS_REPARSE_POINT_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_reparse_point_values libfsntfs_reparse_point_values_t;

struct libfsntfs_reparse_point_values
{
	/* The type and flags
	 */
	uint32_t type_and_flags;

	/* The reparse data
	 */
	uint8_t *reparse_data;

	/* The reparse data size
	 */
	size_t reparse_data_size;
};

int libfsntfs_reparse_point_values_initialize(
     libfsntfs_reparse_point_values_t **reparse_point_values,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_free(
     libfsntfs_reparse_point_values_t **reparse_point_values,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_read(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_type_and_flags(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint32_t *type_and_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

