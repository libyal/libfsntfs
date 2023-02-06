/*
 * Volume information attribute ($VOLUME_INFORMATION) values functions
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

#if !defined( _LIBFSNTFS_VOLUME_INFORMATION_VALUES_H )
#define _LIBFSNTFS_VOLUME_INFORMATION_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_volume_information_values libfsntfs_volume_information_values_t;

struct libfsntfs_volume_information_values
{
	/* The major version (number)
	 */
	uint8_t major_version;

	/* The minor version (number)
	 */
	uint8_t minor_version;

	/* The flags
	 */
	uint16_t flags;
};

int libfsntfs_volume_information_values_initialize(
     libfsntfs_volume_information_values_t **volume_information_values,
     libcerror_error_t **error );

int libfsntfs_volume_information_values_free(
     libfsntfs_volume_information_values_t **volume_information_values,
     libcerror_error_t **error );

int libfsntfs_volume_information_values_read_data(
     libfsntfs_volume_information_values_t *volume_information_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_volume_information_values_read_from_mft_attribute(
     libfsntfs_volume_information_values_t *volume_information_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_VOLUME_INFORMATION_VALUES_H ) */

