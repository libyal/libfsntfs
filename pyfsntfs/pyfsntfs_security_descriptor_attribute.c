/*
 * Python object definition of the libfsntfs security descriptor attribute
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
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_security_descriptor_attribute.h"
#include "pyfsntfs_unused.h"

PyMethodDef pyfsntfs_security_descriptor_attribute_object_methods[] = {

	/* Functions to access the attribute values */

	{ "get_data",
	  (PyCFunction) pyfsntfs_security_descriptor_attribute_get_data,
	  METH_NOARGS,
	  "get_data() -> String\n"
	  "\n"
	  "Returns the security descriptor data." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_security_descriptor_attribute_object_get_set_definitions[] = {

	{ "data",
	  (getter) pyfsntfs_security_descriptor_attribute_get_data,
	  (setter) 0,
	  "The security descriptor data.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_security_descriptor_attribute_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.security_descriptor_attribute",
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
	"pyfsntfs security descriptor attribute object (wraps libfsntfs_attribute_t type LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION)",
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
	pyfsntfs_security_descriptor_attribute_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_security_descriptor_attribute_object_get_set_definitions,
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

/* Retrieves the security descriptor data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_security_descriptor_attribute_get_data(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error            = NULL;
	PyObject *string_object             = NULL;
	static char *function               = "pyfsntfs_security_descriptor_attribute_get_data";
	char *security_desciptor_data       = NULL;
	size_t security_desciptor_data_size = 0;
	int result                          = 0;

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_security_descriptor_attribute_get_security_descriptor_size(
	          pyfsntfs_attribute->attribute,
	          &security_desciptor_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine security descriptor data size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 security_desciptor_data_size );

	security_desciptor_data = PyBytes_AsString(
	                           string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 security_desciptor_data_size );

	security_desciptor_data = PyString_AsString(
	                           string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_security_descriptor_attribute_get_security_descriptor(
	          pyfsntfs_attribute->attribute,
	          (uint8_t *) security_desciptor_data,
	          security_desciptor_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve security descriptor data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

