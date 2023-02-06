/*
 * Extent functions
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

#if !defined( _LIBFSNTFS_EXTENT_H )
#define _LIBFSNTFS_EXTENT_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_extent libfsntfs_extent_t;

struct libfsntfs_extent
{
	/* The start offset
	 */
	off64_t start_offset;

	/* The size
	 */
	size64_t size;

	/* The range flags
	 */
	uint32_t range_flags;
};

int libfsntfs_extent_initialize(
     libfsntfs_extent_t **extent,
     libcerror_error_t **error );

int libfsntfs_extent_free(
     libfsntfs_extent_t **extent,
     libcerror_error_t **error );

int libfsntfs_extent_get_values(
     libfsntfs_extent_t *extent,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_EXTENT_H ) */

