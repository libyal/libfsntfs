/*
 * Python object wrapper of libfsntfs_data_stream_t
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

#include "pyfsntfs_data_stream.h"
#include "pyfsntfs_error.h"
#include "pyfsntfs_integer.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_unused.h"

PyMethodDef pyfsntfs_data_stream_object_methods[] = {

	/* Functions to access the data stream data */

	{ "read_buffer",
	  (PyCFunction) pyfsntfs_data_stream_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of data stream data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyfsntfs_data_stream_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> String\n"
	  "\n"
	  "Reads a buffer of data stream data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyfsntfs_data_stream_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data stream data." },

	{ "get_offset",
	  (PyCFunction) pyfsntfs_data_stream_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Returns the current offset within the data stream data." },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pyfsntfs_data_stream_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> String\n"
	  "\n"
	  "Reads a buffer of data stream data." },

	{ "seek",
	  (PyCFunction) pyfsntfs_data_stream_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data stream data." },

	{ "tell",
	  (PyCFunction) pyfsntfs_data_stream_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Returns the current offset within the data stream data." },

	/* Functions to access the metadata */

	{ "get_size",
	  (PyCFunction) pyfsntfs_data_stream_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Returns the size data." },

	{ "get_name",
	  (PyCFunction) pyfsntfs_data_stream_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string or None\n"
	  "\n"
	  "Returns the name." },

	/* Functions to access the extents */

	{ "get_number_of_extents",
	  (PyCFunction) pyfsntfs_data_stream_get_number_of_extents,
	  METH_NOARGS,
	  "get_number_of_extents() -> Integer\n"
	  "\n"
	  "Retrieves the number of extents." },

	{ "get_extent",
	  (PyCFunction) pyfsntfs_data_stream_get_extent,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_extent(extent_index) -> Tuple( Integer, Integer, Integer )\n"
	  "\n"
	  "Retrieves a specific extent.\t"
          "The extent is a tuple of offset, size and flags." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_data_stream_object_get_set_definitions[] = {

	{ "size",
	  (getter) pyfsntfs_data_stream_get_size,
	  (setter) 0,
	  "The size of the data.",
	  NULL },

	{ "name",
	  (getter) pyfsntfs_data_stream_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "number_of_extents",
	  (getter) pyfsntfs_data_stream_get_number_of_extents,
	  (setter) 0,
	  "The number of extents.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_data_stream_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.data_stream",
	/* tp_basicsize */
	sizeof( pyfsntfs_data_stream_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_data_stream_free,
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
	"pyfsntfs data stream object (wraps libfsntfs_data_stream_t)",
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
	pyfsntfs_data_stream_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_data_stream_object_get_set_definitions,
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
	(initproc) pyfsntfs_data_stream_init,
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

/* Creates a new data stream object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_new(
           libfsntfs_data_stream_t *data_stream,
           PyObject *parent_object )
{
	pyfsntfs_data_stream_t *pyfsntfs_data_stream = NULL;
	static char *function                        = "pyfsntfs_data_stream_new";

	if( data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data stream.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfsntfs_data_stream = PyObject_New(
	                        struct pyfsntfs_data_stream,
	                        &pyfsntfs_data_stream_type_object );

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize data stream.",
		 function );

		goto on_error;
	}
	pyfsntfs_data_stream->data_stream   = data_stream;
	pyfsntfs_data_stream->parent_object = parent_object;

	if( pyfsntfs_data_stream->parent_object != NULL )
	{
		Py_IncRef(
		 pyfsntfs_data_stream->parent_object );
	}
	return( (PyObject *) pyfsntfs_data_stream );

on_error:
	if( pyfsntfs_data_stream != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_data_stream );
	}
	return( NULL );
}

/* Initializes a data stream object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_data_stream_init(
     pyfsntfs_data_stream_t *pyfsntfs_data_stream )
{
	static char *function = "pyfsntfs_data_stream_init";

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	/* Make sure libfsntfs data stream is set to NULL
	 */
	pyfsntfs_data_stream->data_stream = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of data stream not supported.",
	 function );

	return( -1 );
}

/* Frees a data stream object
 */
void pyfsntfs_data_stream_free(
      pyfsntfs_data_stream_t *pyfsntfs_data_stream )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsntfs_data_stream_free";
	int result                  = 0;

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data stream.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsntfs_data_stream );

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
	if( pyfsntfs_data_stream->data_stream != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_data_stream_free(
		          &( pyfsntfs_data_stream->data_stream ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsntfs data stream.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfsntfs_data_stream->parent_object != NULL )
	{
		Py_DecRef(
		 pyfsntfs_data_stream->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsntfs_data_stream );
}

/* Reads a buffer of data from the data stream
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_read_buffer(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyfsntfs_data_stream_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	char *buffer                = NULL;
	size64_t read_size          = 0;
	ssize_t read_count          = 0;
	int result                  = 0;

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid pyfsntfs data stream.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_stream->data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid pyfsntfs data stream - missing libfsntfs data stream.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &integer_object ) == 0 )
	{
		return( NULL );
	}
	if( integer_object != NULL )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
			  integer_object,
			  (PyObject *) &PyLong_Type );

		if( result == -1 )
		{
			pyfsntfs_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type long.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION < 3
		else if( result == 0 )
		{
			PyErr_Clear();

			result = PyObject_IsInstance(
				  integer_object,
				  (PyObject *) &PyInt_Type );

			if( result == -1 )
			{
				pyfsntfs_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if integer object is of type int.",
				 function );

				return( NULL );
			}
		}
#endif
	}
	if( result != 0 )
	{
		if( pyfsntfs_integer_unsigned_copy_to_64bit(
		     integer_object,
		     (uint64_t *) &read_size,
		     &error ) != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else if( ( integer_object == NULL )
	      || ( integer_object == Py_None ) )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_data_stream_get_size(
			  pyfsntfs_data_stream->data_stream,
			  &read_size,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (size64_t) INT_MAX )
	 || ( read_size > (size64_t) SSIZE_MAX ) )
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
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsntfs_data_stream_read_buffer(
	              pyfsntfs_data_stream->data_stream,
	              (uint8_t *) buffer,
	              (size_t) read_size,
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
	/* Need to resize the string here in case read_size was not fully read.
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

/* Reads a buffer of data at a specific offset from the data stream
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_read_buffer_at_offset(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyfsntfs_data_stream_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
	off64_t read_offset         = 0;
	size64_t read_size          = 0;
	ssize_t read_count          = 0;
	int result                  = 0;

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid pyfsntfs data stream.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_stream->data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid pyfsntfs data stream - missing libfsntfs data stream.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "OL",
	     keyword_list,
	     &integer_object,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pyfsntfs_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION < 3
	else if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pyfsntfs_error_fetch_and_raise(
		         PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( NULL );
		}
	}
#endif
	if( result != 0 )
	{
		if( pyfsntfs_integer_unsigned_copy_to_64bit(
		     integer_object,
		     (uint64_t *) &read_size,
		     &error ) != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else if( integer_object == Py_None )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_data_stream_get_size(
			  pyfsntfs_data_stream->data_stream,
			  &read_size,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (size64_t) INT_MAX )
	 || ( read_size > (size64_t) SSIZE_MAX ) )
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
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsntfs_data_stream_read_buffer_at_offset(
	              pyfsntfs_data_stream->data_stream,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
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
	/* Need to resize the string here in case read_size was not fully read.
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

/* Seeks a certain offset in the data stream data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_seek_offset(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsntfs_data_stream_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid pyfsntfs data stream.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_stream->data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid pyfsntfs data stream - missing libfsntfs data stream.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libfsntfs_data_stream_seek_offset(
	          pyfsntfs_data_stream->data_stream,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

	if( offset == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_get_offset(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_data_stream_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data stream.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_data_stream_get_offset(
	          pyfsntfs_data_stream->data_stream,
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

/* Retrieves the size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_get_size(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_data_stream_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data stream.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_data_stream_get_size(
	          pyfsntfs_data_stream->data_stream,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_get_name(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *name            = NULL;
	static char *function    = "pyfsntfs_data_stream_get_name";
	size_t name_size         = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data stream.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_data_stream_get_utf8_name_size(
	          pyfsntfs_data_stream->data_stream,
	          &name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name size.",
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
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_data_stream_get_utf8_name(
		  pyfsntfs_data_stream->data_stream,
		  name,
		  name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name.",
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

/* Retrieves the number of extents
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_get_number_of_extents(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_data_stream_get_number_of_extents";
	int number_of_extents    = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data stream.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_data_stream_get_number_of_extents(
	          pyfsntfs_data_stream->data_stream,
	          &number_of_extents,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of extents.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_extents );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_extents );
#endif
	return( integer_object );
}

/* Retrieves a specific extent by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_get_extent_by_index(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           int extent_index )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	PyObject *tuple_object   = NULL;
	static char *function    = "pyfsntfs_data_stream_get_extent_by_index";
	off64_t extent_offset    = 0;
	size64_t extent_size     = 0;
	uint32_t extent_flags    = 0;
	int result               = 0;

	if( pyfsntfs_data_stream == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data stream.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_data_stream_get_extent_by_index(
	          pyfsntfs_data_stream->data_stream,
	          extent_index,
	          &extent_offset,
	          &extent_size,
	          &extent_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve extent: %d.",
		 function,
		 extent_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	tuple_object = PyTuple_New(
                        3 );

	integer_object = pyfsntfs_integer_signed_new_from_64bit(
	                  (int64_t) extent_offset );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     0,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) extent_size );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     1,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) extent_flags );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     2,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	return( tuple_object );

on_error:
	if( integer_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) integer_object );
	}
	if( tuple_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) tuple_object );
	}
	return( NULL );
}

/* Retrieves a specific extent
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_data_stream_get_extent(
           pyfsntfs_data_stream_t *pyfsntfs_data_stream,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *sequence_object   = NULL;
	static char *keyword_list[] = { "extent_index", NULL };
	int extent_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &extent_index ) == 0 )
	{
		return( NULL );
	}
	sequence_object = pyfsntfs_data_stream_get_extent_by_index(
	                   pyfsntfs_data_stream,
	                   extent_index );

	return( sequence_object );
}

