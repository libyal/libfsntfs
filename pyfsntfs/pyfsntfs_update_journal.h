/*
 * Python object definition of the libfsntfs update (or change) journal
 *
 * Copyright (C) 2010-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYFSNTFS_UPDATE_JOURNAL_H )
#define _PYFSNTFS_UPDATE_JOURNAL_H

#include <common.h>
#include <types.h>

#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsntfs_update_journal pyfsntfs_update_journal_t;

struct pyfsntfs_update_journal
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsntfs update (or change) journal
	 */
	libfsntfs_update_journal_t *update_journal;

	/* The volume object
	 */
	PyObject *volume_object;
};

extern PyMethodDef pyfsntfs_update_journal_object_methods[];
extern PyTypeObject pyfsntfs_update_journal_type_object;

PyObject *pyfsntfs_update_journal_new(
           libfsntfs_update_journal_t *update_journal,
           PyObject *volume_object );

int pyfsntfs_update_journal_init(
     pyfsntfs_update_journal_t *pyfsntfs_update_journal );

void pyfsntfs_update_journal_free(
      pyfsntfs_update_journal_t *pyfsntfs_update_journal );

PyObject *pyfsntfs_update_journal_read_usn_record(
           pyfsntfs_update_journal_t *pyfsntfs_update_journal,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif

