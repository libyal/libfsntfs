/*
 * Standard information attribute ($STANDARD_INFORMATION) functions
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

#if !defined( _LIBFSNTFS_STANDARD_INFORMATION_ATTRIBUTE_H )
#define _LIBFSNTFS_STANDARD_INFORMATION_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_extern.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBFSNTFS_EXTERN \
int libfsntfs_standard_information_attribute_get_creation_time(
     libfsntfs_attribute_t *attribute,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_standard_information_attribute_get_modification_time(
     libfsntfs_attribute_t *attribute,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_standard_information_attribute_get_access_time(
     libfsntfs_attribute_t *attribute,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_standard_information_attribute_get_entry_modification_time(
     libfsntfs_attribute_t *attribute,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_standard_information_attribute_get_file_attribute_flags(
     libfsntfs_attribute_t *attribute,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_standard_information_attribute_get_owner_identifier(
     libfsntfs_attribute_t *attribute,
     uint32_t *owner_identifier,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_standard_information_attribute_get_security_descriptor_identifier(
     libfsntfs_attribute_t *attribute,
     uint32_t *security_descriptor_identifier,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_standard_information_attribute_get_update_sequence_number(
     libfsntfs_attribute_t *attribute,
     uint64_t *update_sequence_number,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_STANDARD_INFORMATION_ATTRIBUTE_H ) */

