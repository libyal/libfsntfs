/*
 * Python object definition of the data streams sequence and iterator
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

#if !defined( _PYFSNTFS_DATA_STREAMS_H )
#define _PYFSNTFS_DATA_STREAMS_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_data_stream.h"
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

	/* The file entry object
	 */
	pyfsntfs_file_entry_t *file_entry_object;

	/* The get data stream by index callback function
	 */
	PyObject* (*get_data_stream_by_index)(
	             pyfsntfs_file_entry_t *file_entry_object,
	             int data_stream_index );

	/* The (current) data stream index
	 */
	int data_stream_index;

	/* The number of data streams
	 */
	int number_of_data_streams;
};

extern PyTypeObject pyfsntfs_data_streams_type_object;

PyObject *pyfsntfs_data_streams_new(
           pyfsntfs_file_entry_t *file_entry_object,
           PyObject* (*get_data_stream_by_index)(
                        pyfsntfs_file_entry_t *file_entry_object,
                        int data_stream_index ),
           int number_of_data_streams );

int pyfsntfs_data_streams_init(
     pyfsntfs_data_streams_t *pyfsntfs_data_streams );

void pyfsntfs_data_streams_free(
      pyfsntfs_data_streams_t *pyfsntfs_data_streams );

Py_ssize_t pyfsntfs_data_streams_len(
            pyfsntfs_data_streams_t *pyfsntfs_data_streams );

PyObject *pyfsntfs_data_streams_getitem(
           pyfsntfs_data_streams_t *pyfsntfs_data_streams,
           Py_ssize_t item_index );

PyObject *pyfsntfs_data_streams_iter(
           pyfsntfs_data_streams_t *pyfsntfs_data_streams );

PyObject *pyfsntfs_data_streams_iternext(
           pyfsntfs_data_streams_t *pyfsntfs_data_streams );

#if defined( __cplusplus )
}
#endif

#endif

