/*
 * Python object definition of the sequence and iterator object of file entries
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

#if !defined( _PYFSNTFS_FILE_ENTRIES_H )
#define _PYFSNTFS_FILE_ENTRIES_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_file_entries pyfsntfs_file_entries_t;

struct pyfsntfs_file_entries
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyfsntfs_file_entries_type_object;

PyObject *pyfsntfs_file_entries_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyfsntfs_file_entries_init(
     pyfsntfs_file_entries_t *sequence_object );

void pyfsntfs_file_entries_free(
      pyfsntfs_file_entries_t *sequence_object );

Py_ssize_t pyfsntfs_file_entries_len(
            pyfsntfs_file_entries_t *sequence_object );

PyObject *pyfsntfs_file_entries_getitem(
           pyfsntfs_file_entries_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyfsntfs_file_entries_iter(
           pyfsntfs_file_entries_t *sequence_object );

PyObject *pyfsntfs_file_entries_iternext(
           pyfsntfs_file_entries_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSNTFS_FILE_ENTRIES_H ) */

