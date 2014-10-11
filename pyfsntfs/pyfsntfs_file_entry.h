/*
 * Python object definition of the libfsntfs file entry
 *
 * Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYFSNTFS_FILE_ENTRY_H )
#define _PYFSNTFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_volume.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_file_entry pyfsntfs_file_entry_t;

struct pyfsntfs_file_entry
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsntfs file entry
	 */
	libfsntfs_file_entry_t *file_entry;

	/* The volume object
	 */
	pyfsntfs_volume_t *volume_object;
};

extern PyMethodDef pyfsntfs_file_entry_object_methods[];
extern PyTypeObject pyfsntfs_file_entry_type_object;

PyObject *pyfsntfs_file_entry_new(
           libfsntfs_file_entry_t *file_entry,
           pyfsntfs_volume_t *volume_object );

int pyfsntfs_file_entry_init(
     pyfsntfs_file_entry_t *pyfsntfs_file_entry );

void pyfsntfs_file_entry_free(
      pyfsntfs_file_entry_t *pyfsntfs_file_entry );

PyObject *pyfsntfs_file_entry_read_buffer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_read_buffer_at_offset(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_seek_offset(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_offset(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_size(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_creation_time(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_creation_time_as_integer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_modification_time(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_modification_time_as_integer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_access_time(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_access_time_as_integer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_entry_modification_time(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_entry_modification_time_as_integer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_name(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_number_of_sub_file_entries(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_sub_file_entry_by_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           int sub_file_entry_index );

PyObject *pyfsntfs_file_entry_get_sub_file_entry(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_sub_file_entries(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif

