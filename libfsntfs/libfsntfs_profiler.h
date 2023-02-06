/*
 * The profiler functions
 *
 * Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSNTFS_PROFILER_H )
#define _LIBFSNTFS_PROFILER_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_PROFILER )

typedef struct libfsntfs_profiler libfsntfs_profiler_t;

struct libfsntfs_profiler
{
	/* The output stream
	 */
	FILE *output_stream;
};

int libfsntfs_profiler_initialize(
     libfsntfs_profiler_t **profiler,
     libcerror_error_t **error );

int libfsntfs_profiler_free(
     libfsntfs_profiler_t **profiler,
     libcerror_error_t **error );

int libfsntfs_profiler_open(
     libfsntfs_profiler_t *profiler,
     const char *filename,
     libcerror_error_t **error );

int libfsntfs_profiler_close(
     libfsntfs_profiler_t *profiler,
     libcerror_error_t **error );

int libfsntfs_profiler_start_timing(
     libfsntfs_profiler_t *profiler,
     int64_t *start_timestamp,
     libcerror_error_t **error );

int libfsntfs_profiler_stop_timing(
     libfsntfs_profiler_t *profiler,
     int64_t start_timestamp,
     const char *name,
     off64_t offset,
     size64_t size,
     libcerror_error_t **error );

#endif /* defined( HAVE_PROFILER ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_PROFILER_H ) */

