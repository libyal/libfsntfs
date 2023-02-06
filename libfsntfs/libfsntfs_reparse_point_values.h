/*
 * Reparse point attribute ($REPARSE_POINT) values functions
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

#if !defined( _LIBFSNTFS_REPARSE_POINT_VALUES_H )
#define _LIBFSNTFS_REPARSE_POINT_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_reparse_point_values libfsntfs_reparse_point_values_t;

struct libfsntfs_reparse_point_values
{
	/* The tag
	 */
	uint32_t tag;

	/* The compression method
	 */
	uint32_t compression_method;

	/* The substitute name offset
	 */
	uint16_t substitute_name_offset;

	/* The substitute name size
	 */
	uint16_t substitute_name_size;

	/* The print name offset
	 */
	uint16_t print_name_offset;

	/* The print name size
	 */
	uint16_t print_name_size;

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

int libfsntfs_reparse_point_values_read_data(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_read_from_mft_attribute(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_tag(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint32_t *tag,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_compression_method(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint32_t *compression_method,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_utf8_substitute_name_size(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_utf8_substitute_name(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_utf16_substitute_name_size(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_utf16_substitute_name(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_utf8_print_name_size(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_utf8_print_name(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_utf16_print_name_size(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsntfs_reparse_point_values_get_utf16_print_name(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_REPARSE_POINT_VALUES_H ) */

