/*
 * Python bindings module for libfsntfs (pyfsntfs)
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfsntfs.h"
#include "pyfsntfs_attribute.h"
#include "pyfsntfs_attribute_types.h"
#include "pyfsntfs_attributes.h"
#include "pyfsntfs_data_stream.h"
#include "pyfsntfs_data_streams.h"
#include "pyfsntfs_error.h"
#include "pyfsntfs_file_attribute_flags.h"
#include "pyfsntfs_file_entries.h"
#include "pyfsntfs_file_entry.h"
#include "pyfsntfs_file_name_attribute.h"
#include "pyfsntfs_file_object_io_handle.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_mft_metadata_file.h"
#include "pyfsntfs_mft_metadata_file_entries.h"
#include "pyfsntfs_object_identifier_attribute.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_reparse_point_attribute.h"
#include "pyfsntfs_security_descriptor_attribute.h"
#include "pyfsntfs_standard_information_attribute.h"
#include "pyfsntfs_unused.h"
#include "pyfsntfs_usn_change_journal.h"
#include "pyfsntfs_volume.h"
#include "pyfsntfs_volume_file_entries.h"
#include "pyfsntfs_volume_information_attribute.h"
#include "pyfsntfs_volume_name_attribute.h"

#if !defined( LIBFSNTFS_HAVE_BFIO )
LIBFSNTFS_EXTERN \
int libfsntfs_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libfsntfs_error_t **error );
#endif

/* The pyfsntfs module methods
 */
PyMethodDef pyfsntfs_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pyfsntfs_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_volume_signature",
	  (PyCFunction) pyfsntfs_check_volume_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_volume_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a volume has a NTFS volume signature." },

	{ "check_volume_signature_file_object",
	  (PyCFunction) pyfsntfs_check_volume_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_volume_signature(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a volume has a NTFS volume signature using a file-like object." },

	{ "open",
	  (PyCFunction) pyfsntfs_volume_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a volume." },

	{ "open_file_object",
	  (PyCFunction) pyfsntfs_volume_new_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a volume using a file-like object." },

	/* Sentinel */
	{ NULL,
	  NULL,
	  0,
	  NULL}
};

/* Retrieves the pyfsntfs/libfsntfs version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_get_version(
           PyObject *self PYFSNTFS_ATTRIBUTE_UNUSED,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( self )
	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libfsntfs_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

/* Checks if the volume has a NTFS volume signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_check_volume_signature(
           PyObject *self PYFSNTFS_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyfsntfs_check_volume_signature";
	static char *keyword_list[]  = { "filename", NULL };
	const char *filename_narrow  = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	PYFSNTFS_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &string_object ) == 0 )
	{
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
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libfsntfs_check_volume_signature_wide(
		          filename_wide,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			PyErr_Format(
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

		result = libfsntfs_check_volume_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result == -1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check volume signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
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

		result = libfsntfs_check_volume_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check volume signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Checks if the volume has a NTFS volume signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_check_volume_signature_file_object(
           PyObject *self PYFSNTFS_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pyfsntfs_check_volume_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pyfsntfs_file_object_initialize(
	     &file_io_handle,
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

	result = libfsntfs_check_volume_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check volume signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

#if PY_MAJOR_VERSION >= 3

/* The pyfsntfs module definition
 */
PyModuleDef pyfsntfs_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pyfsntfs",
	/* m_doc */
	"Python libfsntfs module (pyfsntfs).",
	/* m_size */
	-1,
	/* m_methods */
	pyfsntfs_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pyfsntfs module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pyfsntfs(
                void )
#else
PyMODINIT_FUNC initpyfsntfs(
                void )
#endif
{
	PyObject *module                                         = NULL;
	PyTypeObject *attribute_type_object                      = NULL;
	PyTypeObject *attribute_types_type_object                = NULL;
	PyTypeObject *attributes_type_object                     = NULL;
	PyTypeObject *data_stream_type_object                    = NULL;
	PyTypeObject *data_streams_type_object                   = NULL;
	PyTypeObject *file_attribute_flags_type_object           = NULL;
	PyTypeObject *file_entries_type_object                   = NULL;
	PyTypeObject *file_entry_type_object                     = NULL;
	PyTypeObject *file_name_attribute_type_object            = NULL;
	PyTypeObject *mft_metadata_file_type_object              = NULL;
	PyTypeObject *mft_metadata_file_entries_type_object      = NULL;
	PyTypeObject *object_identifier_attribute_type_object    = NULL;
	PyTypeObject *reparse_point_attribute_type_object        = NULL;
	PyTypeObject *security_descriptor_attribute_type_object  = NULL;
	PyTypeObject *standard_information_attribute_type_object = NULL;
	PyTypeObject *usn_change_journal_type_object             = NULL;
	PyTypeObject *volume_type_object                         = NULL;
	PyTypeObject *volume_file_entries_type_object            = NULL;
	PyTypeObject *volume_information_attribute_type_object   = NULL;
	PyTypeObject *volume_name_attribute_type_object          = NULL;
	PyGILState_STATE gil_state                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libfsntfs_notify_set_stream(
	 stderr,
	 NULL );
	libfsntfs_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pyfsntfs_module_definition );
#else
	module = Py_InitModule3(
	          "pyfsntfs",
	          pyfsntfs_module_methods,
	          "Python libfsntfs module (pyfsntfs)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the volume type object
	 */
	pyfsntfs_volume_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_volume_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_volume_type_object );

	volume_type_object = &pyfsntfs_volume_type_object;

	PyModule_AddObject(
	 module,
	 "volume",
	 (PyObject *) volume_type_object );

	/* Setup the MFT metadata file type object
	 */
	pyfsntfs_mft_metadata_file_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_mft_metadata_file_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_mft_metadata_file_type_object );

	mft_metadata_file_type_object = &pyfsntfs_mft_metadata_file_type_object;

	PyModule_AddObject(
	 module,
	 "mft_metadata_file",
	 (PyObject *) mft_metadata_file_type_object );

	/* Setup the file entry type object
	 */
	pyfsntfs_file_entry_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_file_entry_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_file_entry_type_object );

	file_entry_type_object = &pyfsntfs_file_entry_type_object;

	PyModule_AddObject(
	 module,
	 "file_entry",
	 (PyObject *) file_entry_type_object );

	/* Setup the data stream type object
	 */
	pyfsntfs_data_stream_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_data_stream_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_data_stream_type_object );

	data_stream_type_object = &pyfsntfs_data_stream_type_object;

	PyModule_AddObject(
	 module,
	 "data_stream",
	 (PyObject *) data_stream_type_object );

	/* Setup the attribute type object
	 */
	pyfsntfs_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_attribute_type_object );

	attribute_type_object = &pyfsntfs_attribute_type_object;

	PyModule_AddObject(
	 module,
	 "attribute",
	 (PyObject *) attribute_type_object );

	/* Setup the $FILE_NAME attribute type object
	 */
	pyfsntfs_file_name_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_file_name_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_file_name_attribute_type_object );

	file_name_attribute_type_object = &pyfsntfs_attribute_type_object;

	PyModule_AddObject(
	 module,
	 "file_name_attribute",
	 (PyObject *) file_name_attribute_type_object );

	/* Setup the $OBJECT_IDENTIFIER attribute type object
	 */
	pyfsntfs_object_identifier_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_object_identifier_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_object_identifier_attribute_type_object );

	object_identifier_attribute_type_object = &pyfsntfs_attribute_type_object;

	PyModule_AddObject(
	 module,
	 "object_identifier_attribute",
	 (PyObject *) object_identifier_attribute_type_object );

	/* Setup the $REPARSE_POINT attribute type object
	 */
	pyfsntfs_reparse_point_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_reparse_point_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_reparse_point_attribute_type_object );

	reparse_point_attribute_type_object = &pyfsntfs_attribute_type_object;

	PyModule_AddObject(
	 module,
	 "reparse_point_attribute",
	 (PyObject *) reparse_point_attribute_type_object );

	/* Setup the $SECURITY_DESCRIPTOR attribute type object
	 */
	pyfsntfs_security_descriptor_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_security_descriptor_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_security_descriptor_attribute_type_object );

	security_descriptor_attribute_type_object = &pyfsntfs_attribute_type_object;

	PyModule_AddObject(
	 module,
	 "security_descriptor_attribute",
	 (PyObject *) security_descriptor_attribute_type_object );

	/* Setup the $STANDARD_INFORMATION attribute type object
	 */
	pyfsntfs_standard_information_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_standard_information_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_standard_information_attribute_type_object );

	standard_information_attribute_type_object = &pyfsntfs_attribute_type_object;

	PyModule_AddObject(
	 module,
	 "standard_information_attribute",
	 (PyObject *) standard_information_attribute_type_object );

	/* Setup the $VOLUME_INFORMATION attribute type object
	 */
	pyfsntfs_volume_information_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_volume_information_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_volume_information_attribute_type_object );

	volume_information_attribute_type_object = &pyfsntfs_attribute_type_object;

	PyModule_AddObject(
	 module,
	 "volume_information_attribute",
	 (PyObject *) volume_information_attribute_type_object );

	/* Setup the $VOLUME_NAME attribute type object
	 */
	pyfsntfs_volume_name_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_volume_name_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_volume_name_attribute_type_object );

	volume_name_attribute_type_object = &pyfsntfs_attribute_type_object;

	PyModule_AddObject(
	 module,
	 "volume_name_attribute",
	 (PyObject *) volume_name_attribute_type_object );

	/* Setup the USN change journal type object
	 */
	pyfsntfs_usn_change_journal_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_usn_change_journal_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_usn_change_journal_type_object );

	usn_change_journal_type_object = &pyfsntfs_usn_change_journal_type_object;

	PyModule_AddObject(
	 module,
	 "usn_change_journal",
	 (PyObject *) usn_change_journal_type_object );

	/* Setup the attributes type object
	 */
	pyfsntfs_attributes_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_attributes_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_attributes_type_object );

	attributes_type_object = &pyfsntfs_attributes_type_object;

	PyModule_AddObject(
	 module,
	 "_attributes",
	 (PyObject *) attributes_type_object );

	/* Setup the data streams type object
	 */
	pyfsntfs_data_streams_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_data_streams_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_data_streams_type_object );

	data_streams_type_object = &pyfsntfs_data_streams_type_object;

	PyModule_AddObject(
	 module,
	 "_data_streams",
	 (PyObject *) data_streams_type_object );

	/* Setup the file entries type object
	 */
	pyfsntfs_file_entries_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_file_entries_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_file_entries_type_object );

	file_entries_type_object = &pyfsntfs_file_entries_type_object;

	PyModule_AddObject(
	 module,
	 "_file_entries",
	 (PyObject *) file_entries_type_object );

	/* Setup the MFT metadata file entries type object
	 */
	pyfsntfs_mft_metadata_file_entries_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_mft_metadata_file_entries_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_mft_metadata_file_entries_type_object );

	mft_metadata_file_entries_type_object = &pyfsntfs_mft_metadata_file_entries_type_object;

	PyModule_AddObject(
	 module,
	 "_mft_metadata_file_entries",
	 (PyObject *) mft_metadata_file_entries_type_object );

	/* Setup the volume file entries type object
	 */
	pyfsntfs_volume_file_entries_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsntfs_volume_file_entries_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_volume_file_entries_type_object );

	volume_file_entries_type_object = &pyfsntfs_volume_file_entries_type_object;

	PyModule_AddObject(
	 module,
	 "_volume_file_entries",
	 (PyObject *) volume_file_entries_type_object );

	/* Setup the attribute types type object
	 */
	pyfsntfs_attribute_types_type_object.tp_new = PyType_GenericNew;

	if( pyfsntfs_attribute_types_init_type(
	     &pyfsntfs_attribute_types_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyfsntfs_attribute_types_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_attribute_types_type_object );

	attribute_types_type_object = &pyfsntfs_attribute_types_type_object;

	PyModule_AddObject(
	 module,
	 "attribute_types",
	 (PyObject *) attribute_types_type_object );

	/* Setup the file attribute flags type object
	 */
	pyfsntfs_file_attribute_flags_type_object.tp_new = PyType_GenericNew;

	if( pyfsntfs_file_attribute_flags_init_type(
	     &pyfsntfs_file_attribute_flags_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyfsntfs_file_attribute_flags_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsntfs_file_attribute_flags_type_object );

	file_attribute_flags_type_object = &pyfsntfs_file_attribute_flags_type_object;

	PyModule_AddObject(
	 module,
	 "file_attribute_flags",
	 (PyObject *) file_attribute_flags_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}

