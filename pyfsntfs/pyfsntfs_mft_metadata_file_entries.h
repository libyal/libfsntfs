/*
 * Python object definition of the MFT metadata file entries sequence and iterator
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

#if !defined( _PYFSNTFS_MFT_METADATA_FILE_ENTRIES_H )
#define _PYFSNTFS_MFT_METADATA_FILE_ENTRIES_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_file_entry.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_mft_metadata_file.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_mft_metadata_file_entries pyfsntfs_mft_metadata_file_entries_t;

struct pyfsntfs_mft_metadata_file_entries
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The MFT metadata file object
	 */
	pyfsntfs_mft_metadata_file_t *mft_metadata_file_object;

	/* The get file entry by index callback function
	 */
	PyObject* (*get_file_entry_by_index)(
	             pyfsntfs_mft_metadata_file_t *mft_metadata_file_object,
	             uint64_t file_entry_index );

	/* The (current) file entry index
	 */
	uint64_t file_entry_index;

	/* The number of file entries
	 */
	uint64_t number_of_file_entries;
};

extern PyTypeObject pyfsntfs_mft_metadata_file_entries_type_object;

PyObject *pyfsntfs_mft_metadata_file_entries_new(
           pyfsntfs_mft_metadata_file_t *mft_metadata_file,
           PyObject* (*get_file_entry_by_index)(
                        pyfsntfs_mft_metadata_file_t *mft_metadata_file_object,
                        uint64_t file_entry_index ),
           uint64_t number_of_file_entries );

int pyfsntfs_mft_metadata_file_entries_init(
     pyfsntfs_mft_metadata_file_entries_t *pyfsntfs_mft_metadata_file_entries );

void pyfsntfs_mft_metadata_file_entries_free(
      pyfsntfs_mft_metadata_file_entries_t *pyfsntfs_mft_metadata_file_entries );

Py_ssize_t pyfsntfs_mft_metadata_file_entries_len(
            pyfsntfs_mft_metadata_file_entries_t *pyfsntfs_mft_metadata_file_entries );

PyObject *pyfsntfs_mft_metadata_file_entries_getitem(
           pyfsntfs_mft_metadata_file_entries_t *pyfsntfs_mft_metadata_file_entries,
           Py_ssize_t item_index );

PyObject *pyfsntfs_mft_metadata_file_entries_iter(
           pyfsntfs_mft_metadata_file_entries_t *pyfsntfs_mft_metadata_file_entries );

PyObject *pyfsntfs_mft_metadata_file_entries_iternext(
           pyfsntfs_mft_metadata_file_entries_t *pyfsntfs_mft_metadata_file_entries );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSNTFS_MFT_METADATA_FILE_ENTRIES_H ) */

