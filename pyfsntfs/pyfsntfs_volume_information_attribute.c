/*
 * Python object definition of the libfsntfs volume information attribute
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
#include "pyfsntfs_datetime.h"
#include "pyfsntfs_error.h"
#include "pyfsntfs_integer.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_unused.h"
#include "pyfsntfs_volume_information_attribute.h"

PyMethodDef pyfsntfs_volume_information_attribute_object_methods[] = {

	/* Functions to access the attribute values */

	{ "get_version",
	  (PyCFunction) pyfsntfs_volume_information_attribute_get_version,
	  METH_NOARGS,
	  "get_version() -> Unicode string or None\n"
	  "\n"
	  "Returns the version." },

	{ "get_flags",
	  (PyCFunction) pyfsntfs_volume_information_attribute_get_flags,
	  METH_NOARGS,
	  "get_flags() -> Integer\n"
	  "\n"
	  "Returns the flags." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_volume_information_attribute_object_get_set_definitions[] = {

	{ "version",
	  (getter) pyfsntfs_volume_information_attribute_get_version,
	  (setter) 0,
	  "The version.",
	  NULL },

	{ "flags",
	  (getter) pyfsntfs_volume_information_attribute_get_flags,
	  (setter) 0,
	  "The flags.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_volume_information_attribute_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.volume_information_attribute",
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
	"pyfsntfs volume information attribute object (wraps libfsntfs_attribute_t type LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION)",
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
	pyfsntfs_volume_information_attribute_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_volume_information_attribute_object_get_set_definitions,
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

/* Retrieves the version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_information_attribute_get_version(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	char version_string[ 4 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_volume_information_attribute_get_version";
	uint8_t major_version    = 0;
	uint8_t minor_version    = 0;
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

	result = libfsntfs_volume_information_attribute_get_version(
	          pyfsntfs_attribute->attribute,
	          &major_version,
	          &minor_version,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve version.",
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
	if( major_version > 9 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid major version value out of bounds.",
		 function );

		return( NULL );
	}
	if( minor_version > 9 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid minor version value out of bounds.",
		 function );

		return( NULL );
	}
	version_string[ 0 ] = '0' + major_version;
	version_string[ 1 ] = '.';
	version_string[ 2 ] = '0' + minor_version;
	version_string[ 3 ] = 0;

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 version_string,
	                 (Py_ssize_t) 3,
	                 NULL );

	return( string_object );
}

/* Retrieves the flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_information_attribute_get_flags(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_volume_information_attribute_get_flags";
	uint16_t flags           = 0;
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

	result = libfsntfs_volume_information_attribute_get_flags(
	          pyfsntfs_attribute->attribute,
	          &flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve flags.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) flags );

	return( integer_object );
}

