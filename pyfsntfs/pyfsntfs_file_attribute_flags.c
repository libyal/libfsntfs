/*
 * Python object definition of the libfsntfs file attribute flags
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

#include "pyfsntfs_file_attribute_flags.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_unused.h"

PyTypeObject pyfsntfs_file_attribute_flags_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.file_attribute_flags",
	/* tp_basicsize */
	sizeof( pyfsntfs_file_attribute_flags_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_file_attribute_flags_free,
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
	"pyfsntfs file attribute flags object (wraps LIBFSNTFS_FILE_ATTRIBUTE_FLAGS)",
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
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
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
	(initproc) pyfsntfs_file_attribute_flags_init,
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

/* Initializes the type object
 * Returns 1 if successful or -1 on error
 */
int pyfsntfs_file_attribute_flags_init_type(
     PyTypeObject *type_object )
{
	PyObject *value_object = NULL;

	if( type_object == NULL )
	{
		return( -1 );
	}
	type_object->tp_dict = PyDict_New();

	if( type_object->tp_dict == NULL )
	{
		return( -1 );
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_READ_ONLY );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_READ_ONLY );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "READ_ONLY",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_HIDDEN );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_HIDDEN );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "HIDDEN",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SYSTEM );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SYSTEM );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SYSTEM",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DIRECTORY );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DIRECTORY );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "DIRECTORY",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ARCHIVE );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ARCHIVE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "ARCHIVE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DEVICE );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DEVICE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "DEVICE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NORMAL );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NORMAL );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "NORMAL",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_TEMPORARY );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_TEMPORARY );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "TEMPORARY",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SPARSE_FILE );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SPARSE_FILE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SPARSE_FILE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_REPARSE_POINT );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_REPARSE_POINT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "REPARSE_POINT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_COMPRESSED );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_COMPRESSED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "COMPRESSED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_OFFLINE );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_OFFLINE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "OFFLINE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "NOT_CONTENT_INDEXED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ENCRYPTED );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ENCRYPTED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "ENCRYPTED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_VIRTUAL );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_FILE_ATTRIBUTE_FLAG_VIRTUAL );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VIRTUAL",
	     value_object ) != 0 )
	{
		goto on_error;
	}
	return( 1 );

on_error:
	if( type_object->tp_dict != NULL )
	{
		Py_DecRef(
		 type_object->tp_dict );

		type_object->tp_dict = NULL;
	}
	return( -1 );
}

/* Creates a new file attribute flags object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_attribute_flags_new(
           void )
{
	pyfsntfs_file_attribute_flags_t *definitions_object = NULL;
	static char *function                               = "pyfsntfs_file_attribute_flags_new";

	definitions_object = PyObject_New(
	                      struct pyfsntfs_file_attribute_flags,
	                      &pyfsntfs_file_attribute_flags_type_object );

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create definitions object.",
		 function );

		goto on_error;
	}
	if( pyfsntfs_file_attribute_flags_init(
	     definitions_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize definitions object.",
		 function );

		goto on_error;
	}
	return( (PyObject *) definitions_object );

on_error:
	if( definitions_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) definitions_object );
	}
	return( NULL );
}

/* Initializes a file attribute flags object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_file_attribute_flags_init(
     pyfsntfs_file_attribute_flags_t *definitions_object )
{
	static char *function = "pyfsntfs_file_attribute_flags_init";

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid definitions object.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Frees a file attribute flags object
 */
void pyfsntfs_file_attribute_flags_free(
      pyfsntfs_file_attribute_flags_t *definitions_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyfsntfs_file_attribute_flags_free";

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid definitions object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           definitions_object );

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
	ob_type->tp_free(
	 (PyObject*) definitions_object );
}

