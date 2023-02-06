/*
 * Volume information attribute ($VOLUME_INFORMATION) functions
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

#if !defined( _LIBFSNTFS_VOLUME_INFORMATION_ATTRIBUTE_H )
#define _LIBFSNTFS_VOLUME_INFORMATION_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_extern.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBFSNTFS_EXTERN \
int libfsntfs_volume_information_attribute_get_version(
     libfsntfs_attribute_t *attribute,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_information_attribute_get_flags(
     libfsntfs_attribute_t *attribute,
     uint16_t *flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_VOLUME_INFORMATION_ATTRIBUTE_H ) */

