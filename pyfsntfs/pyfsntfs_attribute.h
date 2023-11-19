/*
 * Python object wrapper of libfsntfs_attribute_t
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

#if !defined( _PYFSNTFS_ATTRIBUTE_H )
#define _PYFSNTFS_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_attribute pyfsntfs_attribute_t;

struct pyfsntfs_attribute
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsntfs attribute
	 */
	libfsntfs_attribute_t *attribute;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsntfs_attribute_object_methods[];
extern PyTypeObject pyfsntfs_attribute_type_object;

PyObject *pyfsntfs_attribute_new(
           PyTypeObject *type_object,
           libfsntfs_attribute_t *attribute,
           PyObject *parent_object );

int pyfsntfs_attribute_init(
     pyfsntfs_attribute_t *pyfsntfs_attribute );

void pyfsntfs_attribute_free(
      pyfsntfs_attribute_t *pyfsntfs_attribute );

PyObject *pyfsntfs_attribute_get_type(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments );

PyObject *pyfsntfs_attribute_get_name(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments );

PyObject *pyfsntfs_attribute_get_data_size(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments );

PyObject *pyfsntfs_attribute_get_valid_data_size(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSNTFS_ATTRIBUTE_H ) */

