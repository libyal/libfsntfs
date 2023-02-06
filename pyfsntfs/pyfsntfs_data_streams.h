/*
 * Python object definition of the sequence and iterator object of data streams
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

#if !defined( _PYFSNTFS_DATA_STREAMS_H )
#define _PYFSNTFS_DATA_STREAMS_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_data_streams pyfsntfs_data_streams_t;

struct pyfsntfs_data_streams
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

extern PyTypeObject pyfsntfs_data_streams_type_object;

PyObject *pyfsntfs_data_streams_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyfsntfs_data_streams_init(
     pyfsntfs_data_streams_t *sequence_object );

void pyfsntfs_data_streams_free(
      pyfsntfs_data_streams_t *sequence_object );

Py_ssize_t pyfsntfs_data_streams_len(
            pyfsntfs_data_streams_t *sequence_object );

PyObject *pyfsntfs_data_streams_getitem(
           pyfsntfs_data_streams_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyfsntfs_data_streams_iter(
           pyfsntfs_data_streams_t *sequence_object );

PyObject *pyfsntfs_data_streams_iternext(
           pyfsntfs_data_streams_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSNTFS_DATA_STREAMS_H ) */

