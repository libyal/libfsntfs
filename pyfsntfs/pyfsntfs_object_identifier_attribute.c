/*
 * Python object definition of the libfsntfs object identifier attribute
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

#include "pyfsntfs_attribute.h"
#include "pyfsntfs_error.h"
#include "pyfsntfs_guid.h"
#include "pyfsntfs_integer.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_unused.h"
#include "pyfsntfs_object_identifier_attribute.h"

PyMethodDef pyfsntfs_object_identifier_attribute_object_methods[] = {

	/* Functions to access the attribute values */

	{ "get_droid_file_identifier",
	  (PyCFunction) pyfsntfs_object_identifier_attribute_get_droid_file_identifier,
	  METH_NOARGS,
	  "get_droid_file_identifier() -> Unicode string or None\n"
	  "\n"
	  "Returns the droid file identifier (GUID)." },

	{ "get_birth_droid_volume_identifier",
	  (PyCFunction) pyfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier,
	  METH_NOARGS,
	  "get_birth_droid_volume_identifier() -> Unicode string or None\n"
	  "\n"
	  "Returns the birth droid volume identifier (GUID)." },

	{ "get_birth_droid_file_identifier",
	  (PyCFunction) pyfsntfs_object_identifier_attribute_get_birth_droid_file_identifier,
	  METH_NOARGS,
	  "get_birth_droid_file_identifier() -> Unicode string or None\n"
	  "\n"
	  "Returns the birth droid file identifier (GUID)." },

	{ "get_birth_droid_domain_identifier",
	  (PyCFunction) pyfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier,
	  METH_NOARGS,
	  "get_birth_droid_domain_identifier() -> Unicode string or None\n"
	  "\n"
	  "Returns the birth droid domain identifier (GUID)." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_object_identifier_attribute_object_get_set_definitions[] = {

	{ "droid_file_identifier",
	  (getter) pyfsntfs_object_identifier_attribute_get_droid_file_identifier,
	  (setter) 0,
	  "The droid file identifier.",
	  NULL },

	{ "birth_droid_volume_identifier",
	  (getter) pyfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier,
	  (setter) 0,
	  "The birth droid volume identifier.",
	  NULL },

	{ "birth_droid_file_identifier",
	  (getter) pyfsntfs_object_identifier_attribute_get_birth_droid_file_identifier,
	  (setter) 0,
	  "The birth droid file identifier.",
	  NULL },

	{ "birth_droid_domain_identifier",
	  (getter) pyfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier,
	  (setter) 0,
	  "The birth droid domain identifier.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_object_identifier_attribute_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.object_identifier_attribute",
	/* tp_basicsize */
	sizeof( pyfsntfs_attribute_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	0,
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
	"pyfsntfs object identifier attribute object (wraps libfsntfs_attribute_t type LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER)",
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
	pyfsntfs_object_identifier_attribute_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_object_identifier_attribute_object_get_set_definitions,
	/* tp_base */
	&pyfsntfs_attribute_type_object,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	0,
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

/* Retrieves the droid file identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_object_identifier_attribute_get_droid_file_identifier(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	uint8_t guid_data[ 16 ];

	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	static char *function    = "pyfsntfs_object_identifier_attribute_get_droid_file_identifier";
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

	result = libfsntfs_object_identifier_attribute_get_droid_file_identifier(
	          pyfsntfs_attribute->attribute,
	          guid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve droid file identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	string_object = pyfsntfs_string_new_from_guid(
			 guid_data,
			 16 );

	return( string_object );
}

/* Retrieves the birth droid volume identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	uint8_t guid_data[ 16 ];

	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	static char *function    = "pyfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier";
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

	result = libfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier(
	          pyfsntfs_attribute->attribute,
	          guid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve birth droid volume identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	string_object = pyfsntfs_string_new_from_guid(
			 guid_data,
			 16 );

	return( string_object );
}

/* Retrieves the birth droid file identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_object_identifier_attribute_get_birth_droid_file_identifier(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	uint8_t guid_data[ 16 ];

	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	static char *function    = "pyfsntfs_object_identifier_attribute_get_birth_droid_file_identifier";
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

	result = libfsntfs_object_identifier_attribute_get_birth_droid_file_identifier(
	          pyfsntfs_attribute->attribute,
	          guid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve birth droid file identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	string_object = pyfsntfs_string_new_from_guid(
			 guid_data,
			 16 );

	return( string_object );
}

/* Retrieves the birth droid domain identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	uint8_t guid_data[ 16 ];

	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	static char *function    = "pyfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier";
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

	result = libfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier(
	          pyfsntfs_attribute->attribute,
	          guid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve birth droid domain identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	string_object = pyfsntfs_string_new_from_guid(
			 guid_data,
			 16 );

	return( string_object );
}

