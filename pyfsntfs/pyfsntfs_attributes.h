/*
 * Python object definition of the attributes sequence and iterator
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

#if !defined( _PYFSNTFS_ATTRIBUTES_H )
#define _PYFSNTFS_ATTRIBUTES_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_attribute.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_attributes pyfsntfs_attributes_t;

struct pyfsntfs_attributes
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The file entry object
	 */
	pyfsntfs_file_entry_t *file_entry_object;

	/* The get attribute by index callback function
	 */
	PyObject* (*get_attribute_by_index)(
	             pyfsntfs_file_entry_t *file_entry_object,
	             int attribute_index );

	/* The (current) attribute index
	 */
	int attribute_index;

	/* The number of attributes
	 */
	int number_of_attributes;
};

extern PyTypeObject pyfsntfs_attributes_type_object;

PyObject *pyfsntfs_attributes_new(
           pyfsntfs_file_entry_t *file_entry_object,
           PyObject* (*get_attribute_by_index)(
                        pyfsntfs_file_entry_t *file_entry_object,
                        int attribute_index ),
           int number_of_attributes );

int pyfsntfs_attributes_init(
     pyfsntfs_attributes_t *pyfsntfs_attributes );

void pyfsntfs_attributes_free(
      pyfsntfs_attributes_t *pyfsntfs_attributes );

Py_ssize_t pyfsntfs_attributes_len(
            pyfsntfs_attributes_t *pyfsntfs_attributes );

PyObject *pyfsntfs_attributes_getitem(
           pyfsntfs_attributes_t *pyfsntfs_attributes,
           Py_ssize_t item_index );

PyObject *pyfsntfs_attributes_iter(
           pyfsntfs_attributes_t *pyfsntfs_attributes );

PyObject *pyfsntfs_attributes_iternext(
           pyfsntfs_attributes_t *pyfsntfs_attributes );

#if defined( __cplusplus )
}
#endif

#endif

