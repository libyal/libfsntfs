/*
 * Python object definition of the libfsntfs update (or change) journal
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfsntfs_error.h"
#include "pyfsntfs_integer.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_unused.h"
#include "pyfsntfs_usn_change_journal.h"

PyMethodDef pyfsntfs_usn_change_journal_object_methods[] = {

	/* Functions to access the USN change journal */

	{ "get_offset",
	  (PyCFunction) pyfsntfs_usn_change_journal_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Returns the current offset within the USN change journal data." },

	{ "read_usn_record",
	  (PyCFunction) pyfsntfs_usn_change_journal_read_usn_record,
	  METH_NOARGS,
	  "read_usn_record() -> String\n"
	  "\n"
	  "Reads USN record data." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_usn_change_journal_object_get_set_definitions[] = {

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_usn_change_journal_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.usn_change_journal",
	/* tp_basicsize */
	sizeof( pyfsntfs_usn_change_journal_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_usn_change_journal_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyfsntfs USN change journal object (wraps libfsntfs_usn_change_journal_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pyfsntfs_usn_change_journal_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_usn_change_journal_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyfsntfs_usn_change_journal_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new USN change journal object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_usn_change_journal_new(
           libfsntfs_usn_change_journal_t *usn_change_journal,
           PyObject *parent_object )
{
	pyfsntfs_usn_change_journal_t *pyfsntfs_usn_change_journal = NULL;
	static char *function                                      = "pyfsntfs_usn_change_journal_new";

	if( usn_change_journal == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid USN change journal.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfsntfs_usn_change_journal = PyObject_New(
	                               struct pyfsntfs_usn_change_journal,
	                               &pyfsntfs_usn_change_journal_type_object );

	if( pyfsntfs_usn_change_journal == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize USN change journal.",
		 function );

		goto on_error;
	}
	pyfsntfs_usn_change_journal->usn_change_journal = usn_change_journal;
	pyfsntfs_usn_change_journal->parent_object      = parent_object;

	if( pyfsntfs_usn_change_journal->parent_object != NULL )
	{
		Py_IncRef(
		 pyfsntfs_usn_change_journal->parent_object );
	}
	return( (PyObject *) pyfsntfs_usn_change_journal );

on_error:
	if( pyfsntfs_usn_change_journal != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_usn_change_journal );
	}
	return( NULL );
}

/* Initializes an USN change journal object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_usn_change_journal_init(
     pyfsntfs_usn_change_journal_t *pyfsntfs_usn_change_journal )
{
	static char *function = "pyfsntfs_usn_change_journal_init";

	if( pyfsntfs_usn_change_journal == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid USN change journal.",
		 function );

		return( -1 );
	}
	/* Make sure libfsntfs USN change journal is set to NULL
	 */
	pyfsntfs_usn_change_journal->usn_change_journal = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of USN change journal not supported.",
	 function );

	return( -1 );
}

/* Frees an USN change journal object
 */
void pyfsntfs_usn_change_journal_free(
      pyfsntfs_usn_change_journal_t *pyfsntfs_usn_change_journal )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsntfs_usn_change_journal_free";
	int result                  = 0;

	if( pyfsntfs_usn_change_journal == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid USN change journal.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsntfs_usn_change_journal );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyfsntfs_usn_change_journal->usn_change_journal != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_usn_change_journal_free(
		          &( pyfsntfs_usn_change_journal->usn_change_journal ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsntfs USN change journal.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfsntfs_usn_change_journal->parent_object != NULL )
	{
		Py_DecRef(
		 pyfsntfs_usn_change_journal->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsntfs_usn_change_journal );
}

/* Retrieves the offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_usn_change_journal_get_offset(
           pyfsntfs_usn_change_journal_t *pyfsntfs_usn_change_journal,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_usn_change_journal_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_usn_change_journal == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid USN change journal.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_usn_change_journal_get_offset(
	          pyfsntfs_usn_change_journal->usn_change_journal,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_signed_new_from_64bit(
	                  (int64_t) offset );

	return( integer_object );
}

/* Reads an USN record
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyfsntfs_usn_change_journal_read_usn_record(
           pyfsntfs_usn_change_journal_t *pyfsntfs_usn_change_journal,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error  = NULL;
	PyObject *string_object   = NULL;
	static char *function     = "pyfsntfs_usn_change_journal_read_usn_record";
	char *usn_record_data     = NULL;
	size_t journal_block_size = 0x1000;
	ssize_t read_count        = 0;

	if( pyfsntfs_usn_change_journal == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid USN change journal.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_usn_change_journal->usn_change_journal == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid USN change journal - missing libfsntfs USN change journal.",
		 function );

		return( NULL );
	}
/* TODO get journal block size from USN change journal */
	if( ( journal_block_size == 0 )
	 || ( journal_block_size > (size_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 journal_block_size );

	usn_record_data = PyBytes_AsString(
	                   string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 journal_block_size );

	usn_record_data = PyString_AsString(
	                   string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsntfs_usn_change_journal_read_usn_record(
	              pyfsntfs_usn_change_journal->usn_change_journal,
	              (uint8_t *) usn_record_data,
	              (size_t) journal_block_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case journal_block_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

