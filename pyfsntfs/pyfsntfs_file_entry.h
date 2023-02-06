/*
 * Python object wrapper of libfsntfs_file_entry_t
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

#if !defined( _PYFSNTFS_FILE_ENTRY_H )
#define _PYFSNTFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

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

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsntfs_file_entry_object_methods[];
extern PyTypeObject pyfsntfs_file_entry_type_object;

PyObject *pyfsntfs_file_entry_new(
           libfsntfs_file_entry_t *file_entry,
           PyObject *parent_object );

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

PyObject *pyfsntfs_file_entry_get_number_of_extents(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_extent_by_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           int extent_index );

PyObject *pyfsntfs_file_entry_get_extent(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_is_empty(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_is_allocated(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_has_directory_entries_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_has_default_data_stream(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_file_reference(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_base_record_file_reference(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_parent_file_reference(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_parent_file_reference_by_attribute_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_journal_sequence_number(
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

PyObject *pyfsntfs_file_entry_get_name_attribute_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_name_by_attribute_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_name(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_file_attribute_flags(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_path_hint(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_symbolic_link_target(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_security_descriptor_data(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_number_of_attributes(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_attribute_by_index(
           PyObject *pyfsntfs_file_entry,
           int attribute_index );

PyObject *pyfsntfs_file_entry_get_attribute(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_attributes(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_number_of_alternate_data_streams(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_alternate_data_stream_by_index(
           PyObject *pyfsntfs_file_entry,
           int alternate_data_stream_index );

PyObject *pyfsntfs_file_entry_get_alternate_data_stream(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_alternate_data_streams(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_has_alternate_data_stream_by_name(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_alternate_data_stream_by_name(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsntfs_file_entry_get_number_of_sub_file_entries(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments );

PyObject *pyfsntfs_file_entry_get_sub_file_entry_by_index(
           PyObject *pyfsntfs_file_entry,
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

#endif /* !defined( _PYFSNTFS_FILE_ENTRY_H ) */

