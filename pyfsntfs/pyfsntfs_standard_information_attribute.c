/*
 * Python object definition of the libfsntfs standard information attribute
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
#include "pyfsntfs_standard_information_attribute.h"
#include "pyfsntfs_unused.h"

PyMethodDef pyfsntfs_standard_information_attribute_object_methods[] = {

	/* Functions to access the attribute values */

	{ "get_creation_time",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_creation_time,
	  METH_NOARGS,
	  "get_creation_time() -> Datetime or None\n"
	  "\n"
	  "Returns the creation date and time." },

	{ "get_creation_time_as_integer",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_creation_time_as_integer,
	  METH_NOARGS,
	  "get_creation_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the creation date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_modification_time",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_modification_time,
	  METH_NOARGS,
	  "get_modification_time() -> Datetime or None\n"
	  "\n"
	  "Returns the modification date and time." },

	{ "get_modification_time_as_integer",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_modification_time_as_integer,
	  METH_NOARGS,
	  "get_modification_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the modification date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_access_time",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_access_time,
	  METH_NOARGS,
	  "get_access_time() -> Datetime or None\n"
	  "\n"
	  "Returns the access date and time." },

	{ "get_access_time_as_integer",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_access_time_as_integer,
	  METH_NOARGS,
	  "get_access_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the access date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_entry_modification_time",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_entry_modification_time,
	  METH_NOARGS,
	  "get_entry_modification_time() -> Datetime or None\n"
	  "\n"
	  "Returns the entry modification date and time." },

	{ "get_entry_modification_time_as_integer",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_entry_modification_time_as_integer,
	  METH_NOARGS,
	  "get_entry_modification_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the entry modification date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_file_attribute_flags",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_file_attribute_flags,
	  METH_NOARGS,
	  "get_file_attribute_flags() -> Integer\n"
	  "\n"
	  "Returns the file attribute flags." },

	{ "get_owner_identifier",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_owner_identifier,
	  METH_NOARGS,
	  "get_owner_identifier() -> Integer or None\n"
	  "\n"
	  "Returns the owner identifier." },

	{ "get_security_descriptor_identifier",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_security_descriptor_identifier,
	  METH_NOARGS,
	  "get_security_descriptor_identifier() -> Integer or None\n"
	  "\n"
	  "Returns the security descriptor identifier." },

	{ "get_update_sequence_number",
	  (PyCFunction) pyfsntfs_standard_information_attribute_get_update_sequence_number,
	  METH_NOARGS,
	  "get_update_sequence_number() -> Integer or None\n"
	  "\n"
	  "Returns the update sequence number." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_standard_information_attribute_object_get_set_definitions[] = {

	{ "creation_time",
	  (getter) pyfsntfs_standard_information_attribute_get_creation_time,
	  (setter) 0,
	  "The creation date and time.",
	  NULL },

	{ "modification_time",
	  (getter) pyfsntfs_standard_information_attribute_get_modification_time,
	  (setter) 0,
	  "The modification date and time.",
	  NULL },

	{ "access_time",
	  (getter) pyfsntfs_standard_information_attribute_get_access_time,
	  (setter) 0,
	  "The access date and time.",
	  NULL },

	{ "entry_modification_time",
	  (getter) pyfsntfs_standard_information_attribute_get_entry_modification_time,
	  (setter) 0,
	  "The entry modification date and time.",
	  NULL },

	{ "file_attribute_flags",
	  (getter) pyfsntfs_standard_information_attribute_get_file_attribute_flags,
	  (setter) 0,
	  "The file attribute flags.",
	  NULL },

	{ "owner_identifier",
	  (getter) pyfsntfs_standard_information_attribute_get_owner_identifier,
	  (setter) 0,
	  "The owner identifier.",
	  NULL },

	{ "security_descriptor_identifier",
	  (getter) pyfsntfs_standard_information_attribute_get_security_descriptor_identifier,
	  (setter) 0,
	  "The security descriptor identifier.",
	  NULL },

	{ "update_sequence_number",
	  (getter) pyfsntfs_standard_information_attribute_get_update_sequence_number,
	  (setter) 0,
	  "The update sequence number.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_standard_information_attribute_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.standard_information_attribute",
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
	"pyfsntfs standard information attribute object (wraps libfsntfs_attribute_t type LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION)",
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
	pyfsntfs_standard_information_attribute_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_standard_information_attribute_object_get_set_definitions,
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

/* Retrieves the creation date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_creation_time(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyfsntfs_standard_information_attribute_get_creation_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_standard_information_attribute_get_creation_time(
	          pyfsntfs_attribute->attribute,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve creation time.",
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
	date_time_object = pyfsntfs_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

/* Retrieves the creation date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_creation_time_as_integer(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_standard_information_attribute_get_creation_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_standard_information_attribute_get_creation_time(
	          pyfsntfs_attribute->attribute,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve creation time.",
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
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  filetime );

	return( integer_object );
}

/* Retrieves the modification date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_modification_time(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyfsntfs_standard_information_attribute_get_modification_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_standard_information_attribute_get_modification_time(
	          pyfsntfs_attribute->attribute,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve modification time.",
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
	date_time_object = pyfsntfs_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

/* Retrieves the modification date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_modification_time_as_integer(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_standard_information_attribute_get_modification_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_standard_information_attribute_get_modification_time(
	          pyfsntfs_attribute->attribute,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve modification time.",
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
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  filetime );

	return( integer_object );
}

/* Retrieves the access date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_access_time(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyfsntfs_standard_information_attribute_get_access_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_standard_information_attribute_get_access_time(
	          pyfsntfs_attribute->attribute,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve access time.",
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
	date_time_object = pyfsntfs_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

/* Retrieves the access date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_access_time_as_integer(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_standard_information_attribute_get_access_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_standard_information_attribute_get_access_time(
	          pyfsntfs_attribute->attribute,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve access time.",
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
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  filetime );

	return( integer_object );
}

/* Retrieves the entry modification date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_entry_modification_time(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyfsntfs_standard_information_attribute_get_entry_modification_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_standard_information_attribute_get_entry_modification_time(
	          pyfsntfs_attribute->attribute,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve entry modification time.",
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
	date_time_object = pyfsntfs_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

/* Retrieves the entry modification date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_entry_modification_time_as_integer(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_standard_information_attribute_get_entry_modification_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_standard_information_attribute_get_entry_modification_time(
	          pyfsntfs_attribute->attribute,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve entry modification time.",
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
	integer_object = pyfsntfs_integer_signed_new_from_64bit(
	                  filetime );

	return( integer_object );
}

/* Retrieves the file attribute flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_file_attribute_flags(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error      = NULL;
	PyObject *integer_object      = NULL;
	static char *function         = "pyfsntfs_standard_information_attribute_get_file_attribute_flags";
	uint32_t file_attribute_flags = 0;
	int result                    = 0;

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

	result = libfsntfs_standard_information_attribute_get_file_attribute_flags(
	          pyfsntfs_attribute->attribute,
	          &file_attribute_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) file_attribute_flags );

	return( integer_object );
}

/* Retrieves the owner identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_owner_identifier(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error  = NULL;
	PyObject *integer_object  = NULL;
	static char *function     = "pyfsntfs_standard_information_attribute_get_owner_identifier";
	uint32_t owner_identifier = 0;
	int result                = 0;

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

	result = libfsntfs_standard_information_attribute_get_owner_identifier(
	          pyfsntfs_attribute->attribute,
	          &owner_identifier,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve owner identifier.",
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
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) owner_identifier );

	return( integer_object );
}

/* Retrieves the security identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_security_descriptor_identifier(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error                = NULL;
	PyObject *integer_object                = NULL;
	static char *function                   = "pyfsntfs_standard_information_attribute_get_security_descriptor_identifier";
	uint32_t security_descriptor_identifier = 0;
	int result                              = 0;

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

	result = libfsntfs_standard_information_attribute_get_security_descriptor_identifier(
	          pyfsntfs_attribute->attribute,
	          &security_descriptor_identifier,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve security descriptor identifier.",
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
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) security_descriptor_identifier );

	return( integer_object );
}

/* Retrieves the update sequence number
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_standard_information_attribute_get_update_sequence_number(
           pyfsntfs_attribute_t *pyfsntfs_attribute,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error        = NULL;
	PyObject *integer_object        = NULL;
	static char *function           = "pyfsntfs_standard_information_attribute_get_update_sequence_number";
	uint64_t update_sequence_number = 0;
	int result                      = 0;

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

	result = libfsntfs_standard_information_attribute_get_update_sequence_number(
	          pyfsntfs_attribute->attribute,
	          &update_sequence_number,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve update sequence number.",
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
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  update_sequence_number );

	return( integer_object );
}

