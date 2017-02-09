/*
 * Volume name attribute ($VOLUME_NAME) values functions
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

#if !defined( _LIBFSNTFS_VOLUME_NAME_VALUES_H )
#define _LIBFSNTFS_VOLUME_NAME_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_volume_name_values libfsntfs_volume_name_values_t;

struct libfsntfs_volume_name_values
{
	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;
};

int libfsntfs_volume_name_values_initialize(
     libfsntfs_volume_name_values_t **volume_name_values,
     libcerror_error_t **error );

int libfsntfs_volume_name_values_free(
     libfsntfs_volume_name_values_t **volume_name_values,
     libcerror_error_t **error );

int libfsntfs_volume_name_values_read(
     libfsntfs_volume_name_values_t *volume_name_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_volume_name_values_read_from_attribute(
     libfsntfs_volume_name_values_t *volume_name_values,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_volume_name_values_get_utf8_name_size(
     libfsntfs_volume_name_values_t *volume_name_values,
     size_t *utf8_name_size,
     libcerror_error_t **error );

int libfsntfs_volume_name_values_get_utf8_name(
     libfsntfs_volume_name_values_t *volume_name_values,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

int libfsntfs_volume_name_values_get_utf16_name_size(
     libfsntfs_volume_name_values_t *volume_name_values,
     size_t *utf16_name_size,
     libcerror_error_t **error );

int libfsntfs_volume_name_values_get_utf16_name(
     libfsntfs_volume_name_values_t *volume_name_values,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_VOLUME_NAME_VALUES_H ) */

