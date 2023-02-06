/*
 * Python object wrapper of libfsntfs_data_stream_t
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

#if !defined( _PYFSNTFS_DATA_STREAM_H )
#define _PYFSNTFS_DATA_STREAM_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_data_stream pyfsntfs_data_stream_t;

struct pyfsntfs_data_stream
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsntfs data stream
	 */
	libfsntfs_data_stream_t *data_stream;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsntfs_data_stream_object_methods[];
extern PyTypeObject pyfsntfs_data_stream_type_object;

PyObject *pyfsntfs_data_stream_new(
           libfsntfs_data_stream_t *data_stream,
           PyObject *parent_object );

int pyfsntfs_data_stream_init(
     pyfsntfs_data_stream_t *pyfsntfs_data_stream );

void pyfsntfs_data_stream_free(
      pyfsntfs_data_stream_t *pyfsntfs_data_stream );

PyObject *pyfsntfs_data_stream_read_buffer(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_data_stream_read_buffer_at_offset(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_data_stream_seek_offset(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_data_stream_get_offset(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments );

PyObject *pyfsntfs_data_stream_get_size(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments );

PyObject *pyfsntfs_data_stream_get_name(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments );

PyObject *pyfsntfs_data_stream_get_number_of_extents(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments );

PyObject *pyfsntfs_data_stream_get_extent_by_index(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           int extent_index );

PyObject *pyfsntfs_data_stream_get_extent(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSNTFS_DATA_STREAM_H ) */

