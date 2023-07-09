/*
 * Python object definition of the libfsntfs MFT metadata file
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
#include "pyfsntfs_file_entry.h"
#include "pyfsntfs_file_object_io_handle.h"
#include "pyfsntfs_integer.h"
#include "pyfsntfs_libbfio.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_mft_metadata_file.h"
#include "pyfsntfs_mft_metadata_file_entries.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_unused.h"

#if !defined( LIBFSNTFS_HAVE_BFIO )

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_open_file_io_handle(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsntfs_error_t **error );

#endif /* !defined( LIBFSNTFS_HAVE_BFIO ) */

PyMethodDef pyfsntfs_mft_metadata_file_object_methods[] = {

	/* Functions to access the MFT metadata file */

	{ "open",
	  (PyCFunction) pyfsntfs_mft_metadata_file_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a MFT metadata file." },

	{ "open_file_object",
	  (PyCFunction) pyfsntfs_mft_metadata_file_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a MFT metadata file using a file-like object." },

	{ "close",
	  (PyCFunction) pyfsntfs_mft_metadata_file_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a MFT metadata file." },

	/* Functions to access the MFT metadata file values */

	{ "get_volume_name",
	  (PyCFunction) pyfsntfs_mft_metadata_file_get_volume_name,
	  METH_NOARGS,
	  "get_volume_name() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the volume name." },

	/* Functions to access the file entries */

	{ "get_number_of_file_entries",
	  (PyCFunction) pyfsntfs_mft_metadata_file_get_number_of_file_entries,
	  METH_NOARGS,
	  "get_number_of_file_entries() -> Integer\n"
	  "\n"
	  "Retrieves the number of file entries." },

	{ "get_file_entry",
	  (PyCFunction) pyfsntfs_mft_metadata_file_get_file_entry,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_file_entry(file_entry_index) -> Object\n"
	  "\n"
	  "Retrieves a specific file entry." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_mft_metadata_file_object_get_set_definitions[] = {

	{ "volume_name",
	  (getter) pyfsntfs_mft_metadata_file_get_volume_name,
	  (setter) 0,
	  "The volume name.",
	  NULL },

	{ "number_of_file_entries",
	  (getter) pyfsntfs_mft_metadata_file_get_number_of_file_entries,
	  (setter) 0,
	  "The number of file entries.",
	  NULL },

	{ "file_entries",
	  (getter) pyfsntfs_mft_metadata_file_get_file_entries,
	  (setter) 0,
	  "The file entries",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_mft_metadata_file_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.mft_metadata_file",
	/* tp_basicsize */
	sizeof( pyfsntfs_mft_metadata_file_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_mft_metadata_file_free,
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
	"pyfsntfs MFT metadata file object (wraps libfsntfs_mft_metadata_file_t)",
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
	pyfsntfs_mft_metadata_file_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_mft_metadata_file_object_get_set_definitions,
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
	(initproc) pyfsntfs_mft_metadata_file_init,
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

/* Initializes a MFT metadata file object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_mft_metadata_file_init(
     pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_mft_metadata_file_init";

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	/* Make sure libfsntfs MFT metadata file is set to NULL
	 */
	pyfsntfs_mft_metadata_file->mft_metadata_file = NULL;
	pyfsntfs_mft_metadata_file->file_io_handle    = NULL;

	if( libfsntfs_mft_metadata_file_initialize(
	     &( pyfsntfs_mft_metadata_file->mft_metadata_file ),
	     &error ) != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize MFT metadata file.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a MFT metadata file object
 */
void pyfsntfs_mft_metadata_file_free(
      pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsntfs_mft_metadata_file_free";
	int result                  = 0;

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid MFT metadata file.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsntfs_mft_metadata_file );

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
	if( pyfsntfs_mft_metadata_file->mft_metadata_file != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_mft_metadata_file_free(
		          &( pyfsntfs_mft_metadata_file->mft_metadata_file ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsntfs MFT metadata file.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pyfsntfs_mft_metadata_file );
}

/* Opens a MFT metadata file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_mft_metadata_file_open(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyfsntfs_mft_metadata_file_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	const char *filename_narrow  = NULL;
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid MFT metadata file.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyfsntfs_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		filename_wide = (wchar_t *) PyUnicode_AsWideCharString(
		                             string_object,
		                             NULL );
#else
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_mft_metadata_file_open_wide(
		          pyfsntfs_mft_metadata_file->mft_metadata_file,
	                  filename_wide,
		          LIBFSNTFS_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		PyMem_Free(
		 filename_wide );
#endif
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyfsntfs_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
				   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_mft_metadata_file_open(
		          pyfsntfs_mft_metadata_file->mft_metadata_file,
	                  filename_narrow,
		          LIBFSNTFS_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open MFT metadata file.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyfsntfs_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   string_object );
#else
		filename_narrow = PyString_AsString(
				   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_mft_metadata_file_open(
		          pyfsntfs_mft_metadata_file->mft_metadata_file,
	                  filename_narrow,
		          LIBFSNTFS_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open MFT metadata file.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a MFT metadata file using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_mft_metadata_file_open_file_object(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	char *mode                  = NULL;
	static char *keyword_list[] = { "file_object", "mode", NULL };
	static char *function       = "pyfsntfs_mft_metadata_file_open_file_object";
	int result                  = 0;

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid MFT metadata file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	if( pyfsntfs_file_object_initialize(
	     &( pyfsntfs_mft_metadata_file->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_mft_metadata_file_open_file_io_handle(
	          pyfsntfs_mft_metadata_file->mft_metadata_file,
	          pyfsntfs_mft_metadata_file->file_io_handle,
	          LIBFSNTFS_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open MFT metadata file.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyfsntfs_mft_metadata_file->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pyfsntfs_mft_metadata_file->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Closes a MFT metadata file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_mft_metadata_file_close(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_mft_metadata_file_close";
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid MFT metadata file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_mft_metadata_file_close(
	          pyfsntfs_mft_metadata_file->mft_metadata_file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close MFT metadata file.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyfsntfs_mft_metadata_file->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pyfsntfs_mft_metadata_file->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the volume name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_mft_metadata_file_get_volume_name(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *volume_name     = NULL;
	static char *function    = "pyfsntfs_mft_metadata_file_get_volume_name";
	size_t volume_name_size  = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid MFT metadata file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_mft_metadata_file_get_utf8_volume_name_size(
	          pyfsntfs_mft_metadata_file->mft_metadata_file,
	          &volume_name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve volume name size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( volume_name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	volume_name = (uint8_t *) PyMem_Malloc(
	                           sizeof( uint8_t ) * volume_name_size );

	if( volume_name == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create volume name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_mft_metadata_file_get_utf8_volume_name(
		  pyfsntfs_mft_metadata_file->mft_metadata_file,
		  volume_name,
		  volume_name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve volume name.",
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
			 (char *) volume_name,
			 (Py_ssize_t) volume_name_size - 1,
			 NULL );

	PyMem_Free(
	 volume_name );

	return( string_object );

on_error:
	if( volume_name != NULL )
	{
		PyMem_Free(
		 volume_name );
	}
	return( NULL );
}

/* Retrieves the number of file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_mft_metadata_file_get_number_of_file_entries(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error        = NULL;
	PyObject *integer_object        = NULL;
	static char *function           = "pyfsntfs_mft_metadata_file_get_number_of_file_entries";
	uint64_t number_of_file_entries = 0;
	int result                      = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid MFT metadata file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_mft_metadata_file_get_number_of_file_entries(
	          pyfsntfs_mft_metadata_file->mft_metadata_file,
	          &number_of_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  number_of_file_entries );

	return( integer_object );
}

/* Retrieves a specific file entry by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_mft_metadata_file_get_file_entry_by_index(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           uint64_t file_entry_index )
{
	libcerror_error_t *error           = NULL;
	libfsntfs_file_entry_t *file_entry = NULL;
	PyObject *file_entry_object        = NULL;
	static char *function              = "pyfsntfs_mft_metadata_file_get_file_entry_by_index";
	int result                         = 0;

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid MFT metadata file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_mft_metadata_file_get_file_entry_by_index(
	          pyfsntfs_mft_metadata_file->mft_metadata_file,
	          file_entry_index,
	          &file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve file entry: %" PRIu64 ".",
		 function,
		 file_entry_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	file_entry_object = pyfsntfs_file_entry_new(
	                     file_entry,
	                     (PyObject *) pyfsntfs_mft_metadata_file );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific file entry
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_mft_metadata_file_get_file_entry(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object = NULL;
	static char *keyword_list[] = { "file_entry_index", NULL };
	int file_entry_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &file_entry_index ) == 0 )
	{
		return( NULL );
	}
	file_entry_object = pyfsntfs_mft_metadata_file_get_file_entry_by_index(
	                     pyfsntfs_mft_metadata_file,
	                     file_entry_index );

	return( file_entry_object );
}

/* Retrieves a file entries sequence and iterator object for the MFT metadata file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_mft_metadata_file_get_file_entries(
           pyfsntfs_mft_metadata_file_t *pyfsntfs_mft_metadata_file,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error                   = NULL;
	PyObject *mft_metadata_file_entries_object = NULL;
	static char *function                      = "pyfsntfs_mft_metadata_file_get_file_entries";
	uint64_t number_of_file_entries            = 0;
	int result                                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_mft_metadata_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid MFT metadata file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_mft_metadata_file_get_number_of_file_entries(
	          pyfsntfs_mft_metadata_file->mft_metadata_file,
	          &number_of_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	mft_metadata_file_entries_object = pyfsntfs_mft_metadata_file_entries_new(
	                                    pyfsntfs_mft_metadata_file,
	                                    &pyfsntfs_mft_metadata_file_get_file_entry_by_index,
	                                    number_of_file_entries );

	if( mft_metadata_file_entries_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create MFT metadata file entries object.",
		 function );

		return( NULL );
	}
	return( mft_metadata_file_entries_object );
}

