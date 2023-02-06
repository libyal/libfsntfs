/*
 * Python object definition of the libfsntfs attribute types
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

#include "pyfsntfs_attribute_types.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_unused.h"

PyTypeObject pyfsntfs_attribute_types_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.attribute_types",
	/* tp_basicsize */
	sizeof( pyfsntfs_attribute_types_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_attribute_types_free,
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
	"pyfsntfs attribute types object (wraps LIBFSNTFS_ATTRIBUTE_TYPES)",
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
	(initproc) pyfsntfs_attribute_types_init,
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
int pyfsntfs_attribute_types_init_type(
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
	                LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$STANDARD_INFORMATION",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$ATTRIBUTE_LIST",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$FILE_NAME",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$OBJECT_ID",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$SECURITY_DESCRIPTOR",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$VOLUME_NAME",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$VOLUME_INFORMATION",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_DATA );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_DATA );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$DATA",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$INDEX_ROOT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$INDEX_ALLOCATION",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$BITMAP",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$REPARSE_POINT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_EXTENDED_INFORMATION );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_EXTENDED_INFORMATION );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$EA_INFORMATION",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_EXTENDED );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_EXTENDED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$EA",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_PROPERTY_SET );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_PROPERTY_SET );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$PROPERTY_SET",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM );
#else
	value_object = PyInt_FromLong(
	                LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "$LOGGED_UTILITY_STREAM",
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

/* Creates a new attribute types object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_attribute_types_new(
           void )
{
	pyfsntfs_attribute_types_t *definitions_object = NULL;
	static char *function                          = "pyfsntfs_attribute_types_new";

	definitions_object = PyObject_New(
	                      struct pyfsntfs_attribute_types,
	                      &pyfsntfs_attribute_types_type_object );

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create definitions object.",
		 function );

		goto on_error;
	}
	if( pyfsntfs_attribute_types_init(
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

/* Initializes an attribute types object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_attribute_types_init(
     pyfsntfs_attribute_types_t *definitions_object )
{
	static char *function = "pyfsntfs_attribute_types_init";

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

/* Frees an attribute types object
 */
void pyfsntfs_attribute_types_free(
      pyfsntfs_attribute_types_t *definitions_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyfsntfs_attribute_types_free";

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

