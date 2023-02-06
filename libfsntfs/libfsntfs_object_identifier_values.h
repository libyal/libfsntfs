/*
 * Object identifier attribute ($OBJECT_ID) values functions
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

#if !defined( _LIBFSNTFS_OBJECT_IDENTIFIER_VALUES_H )
#define _LIBFSNTFS_OBJECT_IDENTIFIER_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_object_identifier_values libfsntfs_object_identifier_values_t;

struct libfsntfs_object_identifier_values
{
	/* Droid file identifier
	 */
	uint8_t droid_file_identifier[ 16 ];

	/* Birth droid volume identifier
	 */
	uint8_t birth_droid_volume_identifier[ 16 ];

	/* Birth droid file identifier
	 */
	uint8_t birth_droid_file_identifier[ 16 ];

	/* Birth droid domain identifier
	 */
	uint8_t birth_droid_domain_identifier[ 16 ];
};

int libfsntfs_object_identifier_values_initialize(
     libfsntfs_object_identifier_values_t **object_identifier_values,
     libcerror_error_t **error );

int libfsntfs_object_identifier_values_free(
     libfsntfs_object_identifier_values_t **object_identifier_values,
     libcerror_error_t **error );

int libfsntfs_object_identifier_values_read_data(
     libfsntfs_object_identifier_values_t *object_identifier_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_object_identifier_values_read_from_mft_attribute(
     libfsntfs_object_identifier_values_t *object_identifier_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_OBJECT_IDENTIFIER_VALUES_H ) */

