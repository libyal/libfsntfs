/*
 * Python object definition of the libfsntfs reparse point attribute
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
#include "pyfsntfs_reparse_point_attribute.h"
#include "pyfsntfs_unused.h"

PyMethodDef pyfsntfs_reparse_point_attribute_object_methods[] = {

	/* Functions to access the attribute values */

	{ "get_tag",
	  (PyCFunction) pyfsntfs_reparse_point_attribute_get_tag,
	  METH_NOARGS,
	  "get_tag() -> Integer\n"
	  "\n"
	  "Returns the tag." },

	{ "get_substitute_name",
	  (PyCFunction) pyfsntfs_reparse_point_attribute_get_substitute_name,
	  METH_NOARGS,
	  "get_substitute_name() -> Unicode string or None\n"
	  "\n"
	  "Returns the substitute name." },

	{ "get_print_name",
	  (PyCFunction) pyfsntfs_reparse_point_attribute_get_print_name,
	  METH_NOARGS,
	  "get_print_name() -> Unicode string or None\n"
	  "\n"
	  "Returns the print name." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_reparse_point_attribute_object_get_set_definitions[] = {

	{ "tag",
	  (getter) pyfsntfs_reparse_point_attribute_get_tag,
	  (setter) 0,
	  "The type and flags.",
	  NULL },

	{ "substitute_name",
	  (getter) pyfsntfs_reparse_point_attribute_get_substitute_name,
	  (setter) 0,
	  "The substitute name.",
	  NULL },

	{ "print_name",
	  (getter) pyfsntfs_reparse_point_attribute_get_print_name,
	  (setter) 0,
	  "The print name.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_reparse_point_attribute_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.reparse_point_attribute",
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
	"pyfsntfs reparse point attribute object (wraps libfsntfs_attribute_t type LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT)",
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
	pyfsntfs_reparse_point_attribute_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_reparse_point_attribute_object_get_set_definitions,
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

/* Retrieves the type and flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_reparse_point_attribute_get_tag(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_reparse_point_attribute_get_tag";
	uint32_t tag             = 0;
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

	result = libfsntfs_reparse_point_attribute_get_tag(
	          pyfsntfs_attribute->attribute,
	          &tag,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve type and flags.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) tag );

	return( integer_object );
}

/* Retrieves the substitute name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_reparse_point_attribute_get_substitute_name(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *name            = NULL;
	static char *function    = "pyfsntfs_reparse_point_attribute_get_substitute_name";
	size_t name_size         = 0;
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

	result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
	          pyfsntfs_attribute->attribute,
	          &name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve substitute name size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	name = (uint8_t *) PyMem_Malloc(
	                    sizeof( uint8_t ) * name_size );

	if( name == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create substitute name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
		  pyfsntfs_attribute->attribute,
		  name,
		  name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve substitute name.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) name,
			 (Py_ssize_t) name_size - 1,
			 NULL );

	PyMem_Free(
	 name );

	return( string_object );

on_error:
	if( name != NULL )
	{
		PyMem_Free(
		 name );
	}
	return( NULL );
}

/* Retrieves the print name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_reparse_point_attribute_get_print_name(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *name            = NULL;
	static char *function    = "pyfsntfs_reparse_point_attribute_get_print_name";
	size_t name_size         = 0;
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

	result = libfsntfs_reparse_point_attribute_get_utf8_print_name_size(
	          pyfsntfs_attribute->attribute,
	          &name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve print name size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	name = (uint8_t *) PyMem_Malloc(
	                    sizeof( uint8_t ) * name_size );

	if( name == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create print name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_reparse_point_attribute_get_utf8_print_name(
		  pyfsntfs_attribute->attribute,
		  name,
		  name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve print name.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) name,
			 (Py_ssize_t) name_size - 1,
			 NULL );

	PyMem_Free(
	 name );

	return( string_object );

on_error:
	if( name != NULL )
	{
		PyMem_Free(
		 name );
	}
	return( NULL );
}

