/*
 * Python object definition of the file entries sequence and iterator
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

#if !defined( _PYFSNTFS_FILE_ENTRIES_H )
#define _PYFSNTFS_FILE_ENTRIES_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_file_entry.h"
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

	/* The file entry object
	 */
	pyfsntfs_file_entry_t *file_entry_object;

	/* The get sub file entry by index callback function
	 */
	PyObject* (*get_sub_file_entry_by_index)(
	             pyfsntfs_file_entry_t *file_entry_object,
	             int sub_file_entry_index );

	/* The (current) sub file entry index
	 */
	int sub_file_entry_index;

	/* The number of sub file entries
	 */
	int number_of_sub_file_entries;
};

extern PyTypeObject pyfsntfs_file_entries_type_object;

PyObject *pyfsntfs_file_entries_new(
           pyfsntfs_file_entry_t *file_entry_object,
           PyObject* (*get_sub_file_entry_by_index)(
                        pyfsntfs_file_entry_t *file_entry_object,
                        int sub_file_entry_index ),
           int number_of_sub_file_entries );

int pyfsntfs_file_entries_init(
     pyfsntfs_file_entries_t *pyfsntfs_file_entries );

void pyfsntfs_file_entries_free(
      pyfsntfs_file_entries_t *pyfsntfs_file_entries );

Py_ssize_t pyfsntfs_file_entries_len(
            pyfsntfs_file_entries_t *pyfsntfs_file_entries );

PyObject *pyfsntfs_file_entries_getitem(
           pyfsntfs_file_entries_t *pyfsntfs_file_entries,
           Py_ssize_t item_index );

PyObject *pyfsntfs_file_entries_iter(
           pyfsntfs_file_entries_t *pyfsntfs_file_entries );

PyObject *pyfsntfs_file_entries_iternext(
           pyfsntfs_file_entries_t *pyfsntfs_file_entries );

#if defined( __cplusplus )
}
#endif

#endif

