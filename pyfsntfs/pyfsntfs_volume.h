/*
 * Python object definition of the libfsntfs volume
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

#if !defined( _PYFSNTFS_VOLUME_H )
#define _PYFSNTFS_VOLUME_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_libbfio.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_volume pyfsntfs_volume_t;

struct pyfsntfs_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsntfs volume
	 */
	libfsntfs_volume_t *volume;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyfsntfs_volume_object_methods[];
extern PyTypeObject pyfsntfs_volume_type_object;

int pyfsntfs_volume_init(
     pyfsntfs_volume_t *pyfsntfs_volume );

void pyfsntfs_volume_free(
      pyfsntfs_volume_t *pyfsntfs_volume );

PyObject *pyfsntfs_volume_signal_abort(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_open(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_volume_open_file_object(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_volume_close(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_bytes_per_sector(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_cluster_block_size(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_mft_entry_size(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_index_entry_size(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_name(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_serial_number(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_number_of_file_entries(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_file_entry_by_index(
           pyfsntfs_volume_t *pyfsntfs_volume,
           uint64_t file_entry_index );

PyObject *pyfsntfs_volume_get_file_entry(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_volume_get_file_entries(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_file_entry_by_path(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_volume_get_root_directory(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

PyObject *pyfsntfs_volume_get_usn_change_journal(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSNTFS_VOLUME_H ) */

