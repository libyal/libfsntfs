/*
 * Python object wrapper of libfsntfs_mft_metadata_file_t
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

#if !defined( _PYFSNTFS_MFT_METADATA_FILE_H )
#define _PYFSNTFS_MFT_METADATA_FILE_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libbfio.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_mft_metadata_file pyfsntfs_mft_metadata_file_t;

struct pyfsntfs_mft_metadata_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsntfs MFT metadata file
	 */
	libfsntfs_mft_metadata_file_t *mft_metadata_file;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyfsntfs_mft_metadata_file_object_methods[];
extern PyTypeObject pyfsntfs_mft_metadata_file_type_object;

int pyfsntfs_mft_metadata_file_init(
     pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file );

void pyfsntfs_mft_metadata_file_free(
      pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file );

PyObject *pyfsntfs_mft_metadata_file_open(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_mft_metadata_file_open_file_object(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_mft_metadata_file_close(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments );

PyObject *pyfsntfs_mft_metadata_file_get_volume_name(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments );

PyObject *pyfsntfs_mft_metadata_file_get_number_of_file_entries(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments );

PyObject *pyfsntfs_mft_metadata_file_get_file_entry_by_index(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           uint64_t file_entry_index );

PyObject *pyfsntfs_mft_metadata_file_get_file_entry(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_mft_metadata_file_get_file_entries(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSNTFS_MFT_METADATA_FILE_H ) */

