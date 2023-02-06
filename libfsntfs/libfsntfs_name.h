/*
 * Name functions
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

#if !defined( _LIBFSNTFS_NAME_H )
#define _LIBFSNTFS_NAME_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"
#include "libfsntfs_libuna.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsntfs_name_compare(
     const uint8_t *name,
     size_t name_size,
     const uint8_t *other_name,
     size_t other_name_size,
     uint8_t use_case_folding,
     libcerror_error_t **error );

int libfsntfs_name_compare_short(
     const uint8_t *name,
     size_t name_size,
     const uint8_t *short_name,
     size_t short_name_size,
     libcerror_error_t **error );

int libfsntfs_name_compare_with_utf8_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf8_character_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libcerror_error_t **error );

int libfsntfs_name_compare_with_utf16_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf16_character_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_NAME_H ) */

