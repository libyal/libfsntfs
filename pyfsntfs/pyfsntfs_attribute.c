/*
 * Python object definition of the libfsntfs attribute
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include "pyfsntfs_attribute.h"
#include "pyfsntfs_datetime.h"
#include "pyfsntfs_error.h"
#include "pyfsntfs_integer.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libcstring.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_unused.h"

PyMethodDef pyfsntfs_attribute_object_methods[] = {

	/* Functions to access the attribute values */

	{ "get_type",
	  (PyCFunction) pyfsntfs_attribute_get_type,
	  METH_NOARGS,
	  "get_type() -> Integer\n"
	  "\n"
	  "Retrieves the type." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_attribute_object_get_set_definitions[] = {

	{ "type",
	  (getter) pyfsntfs_attribute_get_type,
	  (setter) 0,
	  "The attribute type.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_attribute_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.attribute",
	/* tp_basicsize */
	sizeof( pyfsntfs_attribute_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_attribute_free,
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
	"pyfsntfs attribute object (wraps libfsntfs_attribute_t)",
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
	pyfsntfs_attribute_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_attribute_object_get_set_definitions,
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
	(initproc) pyfsntfs_attribute_init,
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

/* Creates a new pyfsntfs attribute object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_attribute_new(
           libfsntfs_attribute_t *attribute,
           pyfsntfs_file_entry_t *file_entry_object )
{
	pyfsntfs_attribute_t *pyfsntfs_attribute = NULL;
	static char *function                    = "pyfsntfs_attribute_new";

	if( attribute == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	pyfsntfs_attribute = PyObject_New(
	                      struct pyfsntfs_attribute,
	                      &pyfsntfs_attribute_type_object );

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize attribute.",
		 function );

		goto on_error;
	}
	if( pyfsntfs_attribute_init(
	     pyfsntfs_attribute ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize attribute.",
		 function );

		goto on_error;
	}
	pyfsntfs_attribute->attribute         = attribute;
	pyfsntfs_attribute->file_entry_object = file_entry_object;

	Py_IncRef(
	 (PyObject *) pyfsntfs_attribute->file_entry_object );

	return( (PyObject *) pyfsntfs_attribute );

on_error:
	if( pyfsntfs_attribute != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_attribute );
	}
	return( NULL );
}

/* Intializes an attribute object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_attribute_init(
     pyfsntfs_attribute_t *pyfsntfs_attribute )
{
	static char *function = "pyfsntfs_attribute_init";

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	/* Make sure libfsntfs attribute is set to NULL
	 */
	pyfsntfs_attribute->attribute = NULL;

	return( 0 );
}

/* Frees an attribute object
 */
void pyfsntfs_attribute_free(
      pyfsntfs_attribute_t *pyfsntfs_attribute )
{
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyfsntfs_attribute_free";
	int result                  = 0;

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid attribute.",
		 function );

		return;
	}
	if( pyfsntfs_attribute->attribute == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid attribute - missing libfsntfs attribute.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsntfs_attribute );

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
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_attribute_free(
	          &( pyfsntfs_attribute->attribute ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free attribute.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pyfsntfs_attribute->file_entry_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_attribute->file_entry_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsntfs_attribute );
}

/* Retrieves the type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_attribute_get_type(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_attribute_get_type";
	uint32_t type            = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_attribute_get_type(
	          pyfsntfs_attribute->attribute,
	          &type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) type );

	return( integer_object );
}

