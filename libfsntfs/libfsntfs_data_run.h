/*
 * Data run functions
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

#if !defined( _LIBFSNTFS_DATA_RUN_H )
#define _LIBFSNTFS_DATA_RUN_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_data_run libfsntfs_data_run_t;

struct libfsntfs_data_run
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

int libfsntfs_data_run_initialize(
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error );

int libfsntfs_data_run_free(
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_DATA_RUN_H ) */

