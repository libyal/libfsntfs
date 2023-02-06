/*
 * Python object definition of the libfsntfs volume information attribute
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

#if !defined( _PYFSNTFS_VOLUME_INFORMATION_ATTRIBUTE_H )
#define _PYFSNTFS_VOLUME_INFORMATION_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_attribute.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern PyMethodDef pyfsntfs_volume_information_attribute_object_methods[];
extern PyTypeObject pyfsntfs_volume_information_attribute_type_object;

PyObject *pyfsntfs_volume_information_attribute_get_version(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments );

PyObject *pyfsntfs_volume_information_attribute_get_flags(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSNTFS_VOLUME_INFORMATION_ATTRIBUTE_H ) */

