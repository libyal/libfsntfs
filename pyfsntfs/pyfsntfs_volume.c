/*
 * Python object definition of the libfsntfs volume
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
#include <narrow_string.h>
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
#include "pyfsntfs_python.h"
#include "pyfsntfs_string.h"
#include "pyfsntfs_unused.h"
#include "pyfsntfs_usn_change_journal.h"
#include "pyfsntfs_volume.h"
#include "pyfsntfs_volume_file_entries.h"

#if !defined( LIBFSNTFS_HAVE_BFIO )

LIBFSNTFS_EXTERN \
int libfsntfs_volume_open_file_io_handle(
     libfsntfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsntfs_error_t **error );

#endif /* !defined( LIBFSNTFS_HAVE_BFIO ) */

PyMethodDef pyfsntfs_volume_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyfsntfs_volume_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the volume to abort the current activity." },

	/* Functions to access the volume */

	{ "open",
	  (PyCFunction) pyfsntfs_volume_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a volume." },

	{ "open_file_object",
	  (PyCFunction) pyfsntfs_volume_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a volume using a file-like object." },

	{ "close",
	  (PyCFunction) pyfsntfs_volume_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a volume." },

	/* Functions to access the volume values */

	{ "get_bytes_per_sector",
	  (PyCFunction) pyfsntfs_volume_get_bytes_per_sector,
	  METH_NOARGS,
	  "get_bytes_per_sector() -> Integer\n"
	  "\n"
	  "Retrieves the bytes per sector." },

	{ "get_cluster_block_size",
	  (PyCFunction) pyfsntfs_volume_get_cluster_block_size,
	  METH_NOARGS,
	  "get_cluster_block_size() -> Integer\n"
	  "\n"
	  "Retrieves the cluster block size." },

	{ "get_mft_entry_size",
	  (PyCFunction) pyfsntfs_volume_get_mft_entry_size,
	  METH_NOARGS,
	  "get_mft_entry_size() -> Integer\n"
	  "\n"
	  "Retrieves the MFT entry size." },

	{ "get_index_entry_size",
	  (PyCFunction) pyfsntfs_volume_get_index_entry_size,
	  METH_NOARGS,
	  "get_index_entry_size() -> Integer\n"
	  "\n"
	  "Retrieves the index entry size." },

	{ "get_name",
	  (PyCFunction) pyfsntfs_volume_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the name." },

	{ "get_serial_number",
	  (PyCFunction) pyfsntfs_volume_get_serial_number,
	  METH_NOARGS,
	  "get_serial_number() -> Integer\n"
	  "\n"
	  "Retrieves the serial number." },

	/* Functions to access the file entries */

	{ "get_number_of_file_entries",
	  (PyCFunction) pyfsntfs_volume_get_number_of_file_entries,
	  METH_NOARGS,
	  "get_number_of_file_entries() -> Integer\n"
	  "\n"
	  "Retrieves the number of file entries." },

	{ "get_file_entry",
	  (PyCFunction) pyfsntfs_volume_get_file_entry,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_file_entry(file_entry_index) -> Object\n"
	  "\n"
	  "Retrieves a specific file entry." },

	{ "get_root_directory",
	  (PyCFunction) pyfsntfs_volume_get_root_directory,
	  METH_NOARGS,
	  "get_root_directory() -> Object\n"
	  "\n"
	  "Retrieves the root directory." },

	{ "get_file_entry_by_path",
	  (PyCFunction) pyfsntfs_volume_get_file_entry_by_path,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_file_entry_by_path(path) -> Object or None\n"
	  "\n"
	  "Retrieves a file entry specified by the path." },

	{ "get_usn_change_journal",
	  (PyCFunction) pyfsntfs_volume_get_usn_change_journal,
	  METH_NOARGS,
	  "get_usn_change_journal() -> Object or None\n"
	  "\n"
	  "Retrieves the USN change journal." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_volume_object_get_set_definitions[] = {

	{ "bytes_per_sector",
	  (getter) pyfsntfs_volume_get_bytes_per_sector,
	  (setter) 0,
	  "The bytes per sector.",
	  NULL },

	{ "cluster_block_size",
	  (getter) pyfsntfs_volume_get_cluster_block_size,
	  (setter) 0,
	  "The cluster block size.",
	  NULL },

	{ "mft_entry_size",
	  (getter) pyfsntfs_volume_get_mft_entry_size,
	  (setter) 0,
	  "The MFT entry size.",
	  NULL },

	{ "index_entry_size",
	  (getter) pyfsntfs_volume_get_index_entry_size,
	  (setter) 0,
	  "The index entry size.",
	  NULL },

	{ "name",
	  (getter) pyfsntfs_volume_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "serial_number",
	  (getter) pyfsntfs_volume_get_serial_number,
	  (setter) 0,
	  "The serial number.",
	  NULL },

	{ "number_of_file_entries",
	  (getter) pyfsntfs_volume_get_number_of_file_entries,
	  (setter) 0,
	  "The number of file entries.",
	  NULL },

	{ "file_entries",
	  (getter) pyfsntfs_volume_get_file_entries,
	  (setter) 0,
	  "The file entries",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_volume_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.volume",
	/* tp_basicsize */
	sizeof( pyfsntfs_volume_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_volume_free,
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
	"pyfsntfs volume object (wraps libfsntfs_volume_t)",
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
	pyfsntfs_volume_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_volume_object_get_set_definitions,
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
	(initproc) pyfsntfs_volume_init,
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

/* Initializes a volume object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_volume_init(
     pyfsntfs_volume_t *pyfsntfs_volume )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_volume_init";

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	/* Make sure libfsntfs volume is set to NULL
	 */
	pyfsntfs_volume->volume         = NULL;
	pyfsntfs_volume->file_io_handle = NULL;

	if( libfsntfs_volume_initialize(
	     &( pyfsntfs_volume->volume ),
	     &error ) != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a volume object
 */
void pyfsntfs_volume_free(
      pyfsntfs_volume_t *pyfsntfs_volume )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsntfs_volume_free";
	int result                  = 0;

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsntfs_volume );

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
	if( pyfsntfs_volume->file_io_handle != NULL )
	{
		if( pyfsntfs_volume_close(
		     pyfsntfs_volume,
		     NULL ) == NULL )
		{
			return;
		}
	}
	if( pyfsntfs_volume->volume != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_volume_free(
		          &( pyfsntfs_volume->volume ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsntfs volume.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pyfsntfs_volume );
}

/* Signals the volume to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_signal_abort(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_volume_signal_abort";
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_signal_abort(
	          pyfsntfs_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_open(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	const char *filename_narrow  = NULL;
	static char *function        = "pyfsntfs_volume_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
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

		result = libfsntfs_volume_open_wide(
		          pyfsntfs_volume->volume,
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
			 "%s: unable to convert Unicode string to UTF-8.",
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

		result = libfsntfs_volume_open(
		          pyfsntfs_volume->volume,
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
			 "%s: unable to open volume.",
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

		result = libfsntfs_volume_open(
		          pyfsntfs_volume->volume,
		          filename_narrow,
		          LIBFSNTFS_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open volume.",
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

/* Opens a volume using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_open_file_object(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsntfs_volume_open_file_object";
	static char *keyword_list[] = { "file_object", "mode", NULL };
	char *mode                  = NULL;
	int result                  = 0;

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
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
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "read" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing read attribute.",
		 function );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "seek" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing seek attribute.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_volume->file_io_handle != NULL )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		goto on_error;
	}
	if( pyfsntfs_file_object_initialize(
	     &( pyfsntfs_volume->file_io_handle ),
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

	result = libfsntfs_volume_open_file_io_handle(
	          pyfsntfs_volume->volume,
	          pyfsntfs_volume->file_io_handle,
	          LIBFSNTFS_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open volume.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyfsntfs_volume->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pyfsntfs_volume->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Closes a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_close(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_volume_close";
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_close(
	          pyfsntfs_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyfsntfs_volume->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pyfsntfs_volume->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_MemoryError,
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

/* Retrieves the bytes per sector
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_bytes_per_sector(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object  = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfsntfs_volume_get_bytes_per_sector";
	uint16_t bytes_per_sector = 0;
	int result                = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_bytes_per_sector(
	          pyfsntfs_volume->volume,
	          &bytes_per_sector,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) bytes_per_sector );
#else
	integer_object = PyInt_FromLong(
	                  (long) bytes_per_sector );
#endif
	return( integer_object );
}

/* Retrieves the cluster block size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_cluster_block_size(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object    = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsntfs_volume_get_cluster_block_size";
	size32_t cluster_block_size = 0;
	int result                  = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_cluster_block_size(
	          pyfsntfs_volume->volume,
	          &cluster_block_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve cluster block size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) cluster_block_size );

	return( integer_object );
}

/* Retrieves the MFT entry size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_mft_entry_size(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_volume_get_mft_entry_size";
	size32_t mft_entry_size  = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_mft_entry_size(
	          pyfsntfs_volume->volume,
	          &mft_entry_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve MFT entry size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) mft_entry_size );

	return( integer_object );
}

/* Retrieves the index entry size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_index_entry_size(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object  = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfsntfs_volume_get_index_entry_size";
	size32_t index_entry_size = 0;
	int result                = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_index_entry_size(
	          pyfsntfs_volume->volume,
	          &index_entry_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve index entry size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) index_entry_size );

	return( integer_object );
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_name(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *name            = NULL;
	static char *function    = "pyfsntfs_volume_get_name";
	size_t name_size         = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_utf8_name_size(
	          pyfsntfs_volume->volume,
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

	result = libfsntfs_volume_get_utf8_name(
		  pyfsntfs_volume->volume,
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

/* Retrieves the serial number
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_serial_number(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_volume_get_serial_number";
	uint64_t value_64bit     = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_serial_number(
	          pyfsntfs_volume->volume,
	          &value_64bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve serial number.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) value_64bit );

	return( integer_object );
}

/* Retrieves the number of file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_number_of_file_entries(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error        = NULL;
	PyObject *integer_object        = NULL;
	static char *function           = "pyfsntfs_volume_get_number_of_file_entries";
	uint64_t number_of_file_entries = 0;
	int result                      = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_number_of_file_entries(
	          pyfsntfs_volume->volume,
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
PyObject *pyfsntfs_volume_get_file_entry_by_index(
           pyfsntfs_volume_t *pyfsntfs_volume,
           uint64_t file_entry_index )
{
	libcerror_error_t *error           = NULL;
	libfsntfs_file_entry_t *file_entry = NULL;
	PyObject *file_entry_object        = NULL;
	static char *function              = "pyfsntfs_volume_get_file_entry_by_index";
	int result                         = 0;

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_file_entry_by_index(
	          pyfsntfs_volume->volume,
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
	                     (PyObject *) pyfsntfs_volume );

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
PyObject *pyfsntfs_volume_get_file_entry(
           pyfsntfs_volume_t *pyfsntfs_volume,
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
	file_entry_object = pyfsntfs_volume_get_file_entry_by_index(
	                     pyfsntfs_volume,
	                     file_entry_index );

	return( file_entry_object );
}

/* Retrieves a file entries sequence and iterator object for the volume file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_file_entries(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error             = NULL;
	PyObject *volume_file_entries_object = NULL;
	static char *function                = "pyfsntfs_volume_get_file_entries";
	uint64_t number_of_file_entries      = 0;
	int result                           = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_number_of_file_entries(
	          pyfsntfs_volume->volume,
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
	volume_file_entries_object = pyfsntfs_volume_file_entries_new(
	                              pyfsntfs_volume,
	                              &pyfsntfs_volume_get_file_entry_by_index,
	                              number_of_file_entries );

	if( volume_file_entries_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create volume file entries object.",
		 function );

		return( NULL );
	}
	return( volume_file_entries_object );
}

/* Retrieves the file entry specified by the path
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_file_entry_by_path(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object        = NULL;
	PyObject *string_object            = NULL;
	libcerror_error_t *error           = NULL;
	libfsntfs_file_entry_t *file_entry = NULL;
	static char *function              = "pyfsntfs_volume_get_file_entry_by_path";
	static char *keyword_list[]        = { "path", NULL };
	char *path                         = NULL;
	const char *narrow_string          = NULL;
	size_t path_length                 = 0;
	size_t path_size                   = 0;
	int result                         = 0;

#if PY_MAJOR_VERSION <= 2 || ( PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION <= 2 )
	PyObject *utf8_string_object       = NULL;
#endif

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		goto on_error;
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

		goto on_error;
	}
	else if( result != 0 )
	{
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		if( pyfsntfs_string_copy_to_utf8_rfc2279(
		     string_object,
		     (uint8_t **) &path,
		     &path_size,
		     &error ) != 1 )
		{
			pyfsntfs_error_raise(
			 error,
		         PyExc_RuntimeError,
			 "%s: unable to copy string object to UTF-8 string.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		narrow_string = path;
		path_length   = path_size - 1;
#else
		PyErr_Clear();

		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyfsntfs_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert Unicode string to UTF-8.",
			 function );

			goto on_error;
		}
#if PY_MAJOR_VERSION >= 3
		narrow_string = PyBytes_AsString(
		                 utf8_string_object );
#else
		narrow_string = PyString_AsString(
		                 utf8_string_object );
#endif
		path_length = narrow_string_length(
		               narrow_string );
#endif
	}
	else if( result == 0 )
	{
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

			goto on_error;
		}
		else if( result == 0 )
		{
			PyErr_Format(
			 PyExc_TypeError,
			 "%s: unsupported string object type.",
			 function );

			goto on_error;
		}
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		narrow_string = PyBytes_AsString(
		                 string_object );
#else
		narrow_string = PyString_AsString(
		                 string_object );
#endif
		path_length = narrow_string_length(
		               narrow_string );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_file_entry_by_utf8_path(
	           pyfsntfs_volume->volume,
	           (uint8_t *) narrow_string,
	           path_length,
	           &file_entry,
	           &error );

	Py_END_ALLOW_THREADS

#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
	if( path != NULL )
	{
		PyMem_Free(
		 path );

		path = NULL;
	}
#else
	if( utf8_string_object != NULL )
	{
		Py_DecRef(
		 utf8_string_object );

		utf8_string_object = NULL;
	}
#endif
	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve file entry.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Check if the file entry is present
	 */
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	file_entry_object = pyfsntfs_file_entry_new(
	                     file_entry,
	                     (PyObject *) pyfsntfs_volume );

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
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
	if( path != NULL )
	{
		PyMem_Free(
		 path );
	}
#else
	if( utf8_string_object != NULL )
	{
		Py_DecRef(
		 utf8_string_object );
	}
#endif
	return( NULL );
}

/* Retrieves the root directory
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_root_directory(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error               = NULL;
	libfsntfs_file_entry_t *root_directory = NULL;
	PyObject *file_entry_object            = NULL;
	static char *function                  = "pyfsntfs_volume_get_root_directory";
	int result                             = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_root_directory(
	          pyfsntfs_volume->volume,
	          &root_directory,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve root directory.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	file_entry_object = pyfsntfs_file_entry_new(
	                     root_directory,
	                     (PyObject *) pyfsntfs_volume );

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
	if( root_directory != NULL )
	{
		libfsntfs_file_entry_free(
		 &root_directory,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the USN change journal
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_volume_get_usn_change_journal(
           pyfsntfs_volume_t *pyfsntfs_volume,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error                           = NULL;
	libfsntfs_usn_change_journal_t *usn_change_journal = NULL;
	PyObject *usn_change_journal_object                = NULL;
	static char *function                              = "pyfsntfs_volume_get_usn_change_journal";
	int result                                         = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_volume_get_usn_change_journal(
	          pyfsntfs_volume->volume,
	          &usn_change_journal,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve USN change journal.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	usn_change_journal_object = pyfsntfs_usn_change_journal_new(
	                             usn_change_journal,
	                             (PyObject *) pyfsntfs_volume );

	if( usn_change_journal_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create USN change journal object.",
		 function );

		goto on_error;
	}
	return( usn_change_journal_object );

on_error:
	if( usn_change_journal != NULL )
	{
		libfsntfs_usn_change_journal_free(
		 &usn_change_journal,
		 NULL );
	}
	return( NULL );
}

