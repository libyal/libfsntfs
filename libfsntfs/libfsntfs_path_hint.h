/*
 * Path hint functions
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

#if !defined( _LIBFSNTFS_PATH_HINT_H )
#define _LIBFSNTFS_PATH_HINT_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_path_hint libfsntfs_path_hint_t;

struct libfsntfs_path_hint
{
	/* The file reference
	 */
	uint64_t file_reference;

	/* The path
	 */
	uint8_t *path;

	/* The path size
	 */
	size_t path_size;
};

int libfsntfs_path_hint_initialize(
     libfsntfs_path_hint_t **path_hint,
     libcerror_error_t **error );

int libfsntfs_path_hint_free(
     libfsntfs_path_hint_t **path_hint,
     libcerror_error_t **error );

int libfsntfs_path_hint_compare_by_file_reference(
     libfsntfs_path_hint_t *first_path_hint,
     libfsntfs_path_hint_t *second_path_hint,
     libcerror_error_t **error );

int libfsntfs_path_hint_get_utf8_path_size(
     libfsntfs_path_hint_t *path_hint,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsntfs_path_hint_get_utf8_path(
     libfsntfs_path_hint_t *path_hint,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsntfs_path_hint_get_utf16_path_size(
     libfsntfs_path_hint_t *path_hint,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsntfs_path_hint_get_utf16_path(
     libfsntfs_path_hint_t *path_hint,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_PATH_HINT_H ) */

