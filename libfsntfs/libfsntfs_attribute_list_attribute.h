/*
 * Attribute list attribute ($ATTRIBUTE_LIST) functions
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

#if !defined( _LIBFSNTFS_ATTRIBUTE_LIST_ATTRIBUTE_H )
#define _LIBFSNTFS_ATTRIBUTE_LIST_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_extern.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_list_attribute_get_number_of_entries(
     libfsntfs_attribute_t *attribute,
     int *number_of_entries,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_list_attribute_get_entry_by_index(
     libfsntfs_attribute_t *attribute,
     int entry_index,
     libfsntfs_attribute_list_entry_t **attribute_list_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_ATTRIBUTE_LIST_ATTRIBUTE_H ) */

