/*
 * Python object definition of the libfsntfs file entry
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
#include "pyfsntfs_file_entries.h"
#include "pyfsntfs_file_entry.h"
#include "pyfsntfs_file_name_attribute.h"
#include "pyfsntfs_integer.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_object_identifier_attribute.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_reparse_point_attribute.h"
#include "pyfsntfs_standard_information_attribute.h"
#include "pyfsntfs_unused.h"
#include "pyfsntfs_volume_information_attribute.h"
#include "pyfsntfs_volume_name_attribute.h"

PyMethodDef pyfsntfs_file_entry_object_methods[] = {

	/* Functions to access the file entry data */

	{ "read_buffer",
	  (PyCFunction) pyfsntfs_file_entry_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of file entry data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyfsntfs_file_entry_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> String\n"
	  "\n"
	  "Reads a buffer of file entry data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyfsntfs_file_entry_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the file entry data." },

	{ "get_offset",
	  (PyCFunction) pyfsntfs_file_entry_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Returns the current offset within the file entry data." },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pyfsntfs_file_entry_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> String\n"
	  "\n"
	  "Reads a buffer of file entry data." },

	{ "seek",
	  (PyCFunction) pyfsntfs_file_entry_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the file entry data." },

	{ "tell",
	  (PyCFunction) pyfsntfs_file_entry_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Returns the current offset within the file entry data." },

	/* Functions to access the metadata */

	{ "get_size",
	  (PyCFunction) pyfsntfs_file_entry_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Returns the size data." },

	{ "is_allocated",
	  (PyCFunction) pyfsntfs_file_entry_is_allocated,
	  METH_NOARGS,
	  "is_allocated() -> Boolean\n"
	  "\n"
	  "Determines if the file entry is allocated." },

	{ "has_directory_entries_index",
	  (PyCFunction) pyfsntfs_file_entry_has_directory_entries_index,
	  METH_NOARGS,
	  "has_directory_entries_index() -> Boolean\n"
	  "\n"
	  "Determines if the file entry has a directory entries index." },

	{ "has_default_data_stream",
	  (PyCFunction) pyfsntfs_file_entry_has_default_data_stream,
	  METH_NOARGS,
	  "has_default_data_stream() -> Boolean\n"
	  "\n"
	  "Determines if the file entry has a default data stream." },

	{ "get_file_reference",
	  (PyCFunction) pyfsntfs_file_entry_get_file_reference,
	  METH_NOARGS,
	  "get_file_reference() -> Integer\n"
	  "\n"
	  "Returns the file reference, a combination of MFT entry index and sequence number." },

	{ "get_base_record_file_reference",
	  (PyCFunction) pyfsntfs_file_entry_get_base_record_file_reference,
	  METH_NOARGS,
	  "get_base_record_file_reference() -> Integer\n"
	  "\n"
	  "Returns the base record file reference, a combination of MFT entry index and sequence number." },

	{ "get_parent_file_reference",
	  (PyCFunction) pyfsntfs_file_entry_get_parent_file_reference,
	  METH_NOARGS,
	  "get_parent_file_reference() -> Integer or None\n"
	  "\n"
	  "Returns the parent file reference, a combination of MFT entry index and sequence number." },

	{ "get_parent_file_reference_by_attribute_index",
	  (PyCFunction) pyfsntfs_file_entry_get_parent_file_reference_by_attribute_index,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_parent_file_reference_by_attribute_index(attribute_index) -> Integer\n"
	  "\n"
	  "Returns the parent file reference, a combination of MFT entry index and sequence number." },

	{ "get_journal_sequence_number",
	  (PyCFunction) pyfsntfs_file_entry_get_journal_sequence_number,
	  METH_NOARGS,
	  "get_journal_sequence_number() -> Integer\n"
	  "\n"
	  "Returns the journal sequence number." },

	{ "get_creation_time",
	  (PyCFunction) pyfsntfs_file_entry_get_creation_time,
	  METH_NOARGS,
	  "get_creation_time() -> Datetime or None\n"
	  "\n"
	  "Returns the creation date and time." },

	{ "get_creation_time_as_integer",
	  (PyCFunction) pyfsntfs_file_entry_get_creation_time_as_integer,
	  METH_NOARGS,
	  "get_creation_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the creation date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_modification_time",
	  (PyCFunction) pyfsntfs_file_entry_get_modification_time,
	  METH_NOARGS,
	  "get_modification_time() -> Datetime or None\n"
	  "\n"
	  "Returns the modification date and time." },

	{ "get_modification_time_as_integer",
	  (PyCFunction) pyfsntfs_file_entry_get_modification_time_as_integer,
	  METH_NOARGS,
	  "get_modification_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the modification date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_access_time",
	  (PyCFunction) pyfsntfs_file_entry_get_access_time,
	  METH_NOARGS,
	  "get_access_time() -> Datetime or None\n"
	  "\n"
	  "Returns the access date and time." },

	{ "get_access_time_as_integer",
	  (PyCFunction) pyfsntfs_file_entry_get_access_time_as_integer,
	  METH_NOARGS,
	  "get_access_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the access date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_entry_modification_time",
	  (PyCFunction) pyfsntfs_file_entry_get_entry_modification_time,
	  METH_NOARGS,
	  "get_entry_modification_time() -> Datetime or None\n"
	  "\n"
	  "Returns the entry modification date and time." },

	{ "get_entry_modification_time_as_integer",
	  (PyCFunction) pyfsntfs_file_entry_get_entry_modification_time_as_integer,
	  METH_NOARGS,
	  "get_entry_modification_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the entry modification date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_name",
	  (PyCFunction) pyfsntfs_file_entry_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string or None\n"
	  "\n"
	  "Returns the name." },

	{ "get_name_attribute_index",
	  (PyCFunction) pyfsntfs_file_entry_get_name_attribute_index,
	  METH_NOARGS,
	  "get_name_attribute_index() -> Integer or None\n"
	  "\n"
	  "Returns the attribute index corresponding to the name." },

	{ "get_name_by_attribute_index",
	  (PyCFunction) pyfsntfs_file_entry_get_name_by_attribute_index,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_name_by_attribute_index(attribute_index) -> Unicode string or None\n"
	  "\n"
	  "Returns the name." },

	{ "get_file_attribute_flags",
	  (PyCFunction) pyfsntfs_file_entry_get_file_attribute_flags,
	  METH_NOARGS,
	  "get_file_attribute_flags() -> Integer or None\n"
	  "\n"
	  "Returns the file attribute flags." },

	/* Functions to access the attributes */

	{ "get_number_of_attributes",
	  (PyCFunction) pyfsntfs_file_entry_get_number_of_attributes,
	  METH_NOARGS,
	  "get_number_of_attributes() -> Integer\n"
	  "\n"
	  "Retrieves the number of attributes." },

	{ "get_attribute",
	  (PyCFunction) pyfsntfs_file_entry_get_attribute,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_attribute(attribute_index) -> Object\n"
	  "\n"
	  "Retrieves a specific attribute." },

	/* Functions to access the sub file entries */

	{ "get_number_of_sub_file_entries",
	  (PyCFunction) pyfsntfs_file_entry_get_number_of_sub_file_entries,
	  METH_NOARGS,
	  "get_number_of_sub_file_entries() -> Integer\n"
	  "\n"
	  "Retrieves the number of sub file entries." },

	{ "get_sub_file_entry",
	  (PyCFunction) pyfsntfs_file_entry_get_sub_file_entry,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_file_entry(sub_file_entry_index) -> Object\n"
	  "\n"
	  "Retrieves a specific sub file entry." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsntfs_file_entry_object_get_set_definitions[] = {

	{ "size",
	  (getter) pyfsntfs_file_entry_get_size,
	  (setter) 0,
	  "The size of the data.",
	  NULL },

	{ "file_reference",
	  (getter) pyfsntfs_file_entry_get_file_reference,
	  (setter) 0,
	  "The file reference, a combination of MFT entry index and sequence number.",
	  NULL },

	{ "base_record_file_reference",
	  (getter) pyfsntfs_file_entry_get_base_record_file_reference,
	  (setter) 0,
	  "The base record file reference, a combination of MFT entry index and sequence number.",
	  NULL },

	{ "parent_file_reference",
	  (getter) pyfsntfs_file_entry_get_parent_file_reference,
	  (setter) 0,
	  "The parent file reference, a combination of MFT entry index and sequence number.",
	  NULL },

	{ "journal_sequence_number",
	  (getter) pyfsntfs_file_entry_get_journal_sequence_number,
	  (setter) 0,
	  "The journal sequence number.",
	  NULL },

	{ "creation_time",
	  (getter) pyfsntfs_file_entry_get_creation_time,
	  (setter) 0,
	  "The creation date and time.",
	  NULL },

	{ "modification_time",
	  (getter) pyfsntfs_file_entry_get_modification_time,
	  (setter) 0,
	  "The modification date and time.",
	  NULL },

	{ "access_time",
	  (getter) pyfsntfs_file_entry_get_access_time,
	  (setter) 0,
	  "The access date and time.",
	  NULL },

	{ "entry_modification_time",
	  (getter) pyfsntfs_file_entry_get_entry_modification_time,
	  (setter) 0,
	  "The entry modification date and time.",
	  NULL },

	{ "file_attribute_flags",
	  (getter) pyfsntfs_file_entry_get_file_attribute_flags,
	  (setter) 0,
	  "The file attribute flags.",
	  NULL },

	{ "name",
	  (getter) pyfsntfs_file_entry_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "name_attribute_index",
	  (getter) pyfsntfs_file_entry_get_name_attribute_index,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "number_of_attributes",
	  (getter) pyfsntfs_file_entry_get_number_of_attributes,
	  (setter) 0,
	  "The number of attributes.",
	  NULL },

	{ "number_of_sub_file_entries",
	  (getter) pyfsntfs_file_entry_get_number_of_sub_file_entries,
	  (setter) 0,
	  "The number of sub file entries.",
	  NULL },

	{ "sub_file_entries",
	  (getter) pyfsntfs_file_entry_get_sub_file_entries,
	  (setter) 0,
	  "The sub file entries",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsntfs_file_entry_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs.file_entry",
	/* tp_basicsize */
	sizeof( pyfsntfs_file_entry_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_file_entry_free,
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
	"pyfsntfs file entry object (wraps libfsntfs_file_entry_t)",
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
	pyfsntfs_file_entry_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsntfs_file_entry_object_get_set_definitions,
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
	(initproc) pyfsntfs_file_entry_init,
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

/* Creates a new pyfsntfs file entry object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_new(
           libfsntfs_file_entry_t *file_entry,
           PyObject *parent_object )
{
	pyfsntfs_file_entry_t *pyfsntfs_file_entry = NULL;
	static char *function                      = "pyfsntfs_file_entry_new";

	if( file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	pyfsntfs_file_entry = PyObject_New(
	                       struct pyfsntfs_file_entry,
	                       &pyfsntfs_file_entry_type_object );

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file_entry.",
		 function );

		goto on_error;
	}
	if( pyfsntfs_file_entry_init(
	     pyfsntfs_file_entry ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file_entry.",
		 function );

		goto on_error;
	}
	pyfsntfs_file_entry->file_entry    = file_entry;
	pyfsntfs_file_entry->parent_object = parent_object;

	Py_IncRef(
	 pyfsntfs_file_entry->parent_object );

	return( (PyObject *) pyfsntfs_file_entry );

on_error:
	if( pyfsntfs_file_entry != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_file_entry );
	}
	return( NULL );
}

/* Intializes a file entry object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_file_entry_init(
     pyfsntfs_file_entry_t *pyfsntfs_file_entry )
{
	static char *function = "pyfsntfs_file_entry_init";

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file_entry.",
		 function );

		return( -1 );
	}
	/* Make sure libfsntfs file entry is set to NULL
	 */
	pyfsntfs_file_entry->file_entry = NULL;

	return( 0 );
}

/* Frees a file entry object
 */
void pyfsntfs_file_entry_free(
      pyfsntfs_file_entry_t *pyfsntfs_file_entry )
{
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyfsntfs_file_entry_free";
	int result                  = 0;

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file_entry.",
		 function );

		return;
	}
	if( pyfsntfs_file_entry->file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry - missing libfsntfs file_entry.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsntfs_file_entry );

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

	result = libfsntfs_file_entry_free(
	          &( pyfsntfs_file_entry->file_entry ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file_entry.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pyfsntfs_file_entry->parent_object != NULL )
	{
		Py_DecRef(
		 pyfsntfs_file_entry->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsntfs_file_entry );
}

/* Reads a buffer of file entry data from FSNTFS file(s)
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_read_buffer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyfsntfs_file_entry_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	char *buffer                = NULL;
	ssize_t read_count          = 0;
	int read_size               = -1;

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyfsntfs file_entry.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_file_entry->file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyfsntfs file_entry - missing libfsntfs file_entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &read_size ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( read_size > INT_MAX )
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
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsntfs_file_entry_read_buffer(
	              pyfsntfs_file_entry->file_entry,
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

/* Reads a buffer of file entry data at a specific offset from FSNTFS file(s)
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_read_buffer_at_offset(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyfsntfs_file_entry_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
	off64_t read_offset         = 0;
	ssize_t read_count          = 0;
	int read_size               = 0;

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyfsntfs file_entry.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_file_entry->file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyfsntfs file entry - missing libfsntfs file_entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i|L",
	     keyword_list,
	     &read_size,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( read_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read offset value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsntfs_file_entry_read_buffer_at_offset(
	              pyfsntfs_file_entry->file_entry,
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

/* Seeks a certain offset in the file entry data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_seek_offset(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsntfs_file_entry_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyfsntfs file_entry.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_file_entry->file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyfsntfs file entry - missing libfsntfs file_entry.",
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

	offset = libfsntfs_file_entry_seek_offset(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_offset(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_offset(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_size(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_size(
	          pyfsntfs_file_entry->file_entry,
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

/* Determines if the file entry is allocated (in use)
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_is_allocated(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_file_entry_is_allocated";
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_is_allocated(
	          pyfsntfs_file_entry->file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if file entry is allocated.",
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

/* Determines if the file entry has the directory entries index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_has_directory_entries_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_file_entry_has_directory_entries_index";
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_has_directory_entries_index(
	          pyfsntfs_file_entry->file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if file entry has directory entries index.",
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

/* Determines if the file entry has the default data stream
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_has_default_data_stream(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsntfs_file_entry_has_default_data_stream";
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_has_default_data_stream(
	          pyfsntfs_file_entry->file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if file entry has default data stream.",
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

/* Retrieves the file reference
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_file_reference(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_file_reference";
	uint64_t file_reference  = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_file_reference(
	          pyfsntfs_file_entry->file_entry,
	          &file_reference,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve file reference.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  file_reference );

	return( integer_object );
}

/* Retrieves the base record file reference
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_base_record_file_reference(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_base_record_file_reference";
	uint64_t file_reference  = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_base_record_file_reference(
	          pyfsntfs_file_entry->file_entry,
	          &file_reference,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve base record file reference.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  file_reference );

	return( integer_object );
}

/* Retrieves the parent file reference
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_parent_file_reference(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_parent_file_reference";
	uint64_t file_reference  = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_parent_file_reference(
	          pyfsntfs_file_entry->file_entry,
	          &file_reference,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve parent file reference.",
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
	                  file_reference );

	return( integer_object );
}

/* Retrieves the parent file reference for a specific $FILE_NAME attribute
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_parent_file_reference_by_attribute_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *integer_object    = NULL;
	static char *function       = "pyfsntfs_file_entry_get_parent_file_reference_by_attribute_index";
	static char *keyword_list[] = { "attribute_index", NULL };
	uint64_t file_reference     = 0;
	int attribute_index         = 0;
	int result                  = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &attribute_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_parent_file_reference_by_attribute_index(
	          pyfsntfs_file_entry->file_entry,
	          attribute_index,
	          &file_reference,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve parent file reference from attribute: %d.",
		 function,
		 attribute_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  file_reference );

	return( integer_object );
}

/* Retrieves the journal sequence number
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_journal_sequence_number(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error         = NULL;
	PyObject *integer_object         = NULL;
	static char *function            = "pyfsntfs_file_entry_get_journal_sequence_number";
	uint64_t journal_sequence_number = 0;
	int result                       = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_journal_sequence_number(
	          pyfsntfs_file_entry->file_entry,
	          &journal_sequence_number,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve journal sequence number.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsntfs_integer_unsigned_new_from_64bit(
	                  journal_sequence_number );

	return( integer_object );
}

/* Retrieves the creation date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_creation_time(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyfsntfs_file_entry_get_creation_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_creation_time(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_creation_time_as_integer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_creation_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_creation_time(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_modification_time(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyfsntfs_file_entry_get_modification_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_modification_time(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_modification_time_as_integer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_modification_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_modification_time(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_access_time(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyfsntfs_file_entry_get_access_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_access_time(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_access_time_as_integer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_access_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_access_time(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_entry_modification_time(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyfsntfs_file_entry_get_entry_modification_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_entry_modification_time(
	          pyfsntfs_file_entry->file_entry,
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
PyObject *pyfsntfs_file_entry_get_entry_modification_time_as_integer(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_entry_modification_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_entry_modification_time(
	          pyfsntfs_file_entry->file_entry,
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

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_name(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *name            = NULL;
	static char *function    = "pyfsntfs_file_entry_get_name";
	size_t name_size         = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_utf8_name_size(
	          pyfsntfs_file_entry->file_entry,
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
		 PyExc_IOError,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_utf8_name(
		  pyfsntfs_file_entry->file_entry,
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
			 errors );

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

/* Retrieves the name attribute index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_name_attribute_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_name_attribute_index";
	int attribute_index      = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_name_attribute_index(
	          pyfsntfs_file_entry->file_entry,
	          &attribute_index,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name attribute index.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) attribute_index );
#else
	integer_object = PyInt_FromLong(
	                  (long) attribute_index );
#endif
	return( integer_object );
}

/* Retrieves the name for a specific $FILE_NAME attribute
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_name_by_attribute_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	const char *errors          = NULL;
	uint8_t *name               = NULL;
	static char *function       = "pyfsntfs_file_entry_get_name_by_attribute_index";
	static char *keyword_list[] = { "attribute_index", NULL };
	size_t name_size            = 0;
	int attribute_index         = 0;
	int result                  = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &attribute_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_utf8_name_size_by_attribute_index(
	          pyfsntfs_file_entry->file_entry,
	          attribute_index,
	          &name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
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
	else if( name_size == 0 )
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
		 PyExc_IOError,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_utf8_name_by_attribute_index(
		  pyfsntfs_file_entry->file_entry,
	          attribute_index,
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
			 errors );

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

/* Retrieves the file attribute flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_file_attribute_flags(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error      = NULL;
	PyObject *integer_object      = NULL;
	static char *function         = "pyfsntfs_file_entry_get_file_file_entry_flags";
	uint32_t file_attribute_flags = 0;
	int result                    = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_file_attribute_flags(
	          pyfsntfs_file_entry->file_entry,
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

/* Retrieves the number of attributes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_number_of_attributes(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfsntfs_file_entry_get_number_of_attributes";
	int number_of_attributes = 0;
	int result               = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_number_of_attributes(
	          pyfsntfs_file_entry->file_entry,
	          &number_of_attributes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of attributes.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_attributes );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_attributes );
#endif
	return( integer_object );
}

/* Retrieves a specific attribute by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_attribute_by_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           int attribute_index )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	PyObject *attribute_object       = NULL;
	PyTypeObject *type_object        = NULL;
	static char *function            = "pyfsntfs_file_entry_get_attribute_by_index";
	uint32_t attribute_type          = 0;
	int result                       = 0;

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_attribute_by_index(
	          pyfsntfs_file_entry->file_entry,
	          attribute_index,
	          &attribute,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve attribute: %d.",
		 function,
		 attribute_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_attribute_get_type(
	          attribute,
	          &attribute_type,
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
	switch( attribute_type )
	{
		case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
			type_object = &pyfsntfs_file_name_attribute_type_object;
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			type_object = &pyfsntfs_object_identifier_attribute_type_object;
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			type_object = &pyfsntfs_reparse_point_attribute_type_object;
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			type_object = &pyfsntfs_standard_information_attribute_type_object;
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			type_object = &pyfsntfs_volume_information_attribute_type_object;
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			type_object = &pyfsntfs_volume_name_attribute_type_object;
			break;

		default:
			type_object = &pyfsntfs_attribute_type_object;
			break;
	}
	attribute_object = pyfsntfs_attribute_new(
	                    type_object,
	                    attribute,
	                    pyfsntfs_file_entry );

	if( attribute_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create attribute object.",
		 function );

		goto on_error;
	}
	return( attribute_object );

on_error:
	if( attribute != NULL )
	{
		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific attribute
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_attribute(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *attribute_object  = NULL;
	static char *keyword_list[] = { "attribute_index", NULL };
	int attribute_index         = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &attribute_index ) == 0 )
	{
		return( NULL );
	}
	attribute_object = pyfsntfs_file_entry_get_attribute_by_index(
	                    pyfsntfs_file_entry,
	                    attribute_index );

	return( attribute_object );
}

/* Retrieves the number of sub file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_number_of_sub_file_entries(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error       = NULL;
	PyObject *integer_object       = NULL;
	static char *function          = "pyfsntfs_file_entry_get_number_of_sub_file_entries";
	int number_of_sub_file_entries = 0;
	int result                     = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_number_of_sub_file_entries(
	          pyfsntfs_file_entry->file_entry,
	          &number_of_sub_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_sub_file_entries );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_sub_file_entries );
#endif
	return( integer_object );
}

/* Retrieves a specific sub file entry by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_sub_file_entry_by_index(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           int sub_file_entry_index )
{
	libcerror_error_t *error               = NULL;
	libfsntfs_file_entry_t *sub_file_entry = NULL;
	PyObject *file_entry_object            = NULL;
	static char *function                  = "pyfsntfs_file_entry_get_sub_file_entry_by_index";
	int result                             = 0;

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_sub_file_entry_by_index(
	          pyfsntfs_file_entry->file_entry,
	          sub_file_entry_index,
	          &sub_file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub file entry: %d.",
		 function,
		 sub_file_entry_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	file_entry_object = pyfsntfs_file_entry_new(
	                     sub_file_entry,
	                     pyfsntfs_file_entry->parent_object );

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
	if( sub_file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific sub file entry
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_sub_file_entry(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object = NULL;
	static char *keyword_list[] = { "sub_file_entry_index", NULL };
	int sub_file_entry_index    = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_file_entry_index ) == 0 )
	{
		return( NULL );
	}
	file_entry_object = pyfsntfs_file_entry_get_sub_file_entry_by_index(
	                     pyfsntfs_file_entry,
	                     sub_file_entry_index );

	return( file_entry_object );
}

/* Retrieves a file entries sequence and iterator object for the sub file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_file_entry_get_sub_file_entries(
           pyfsntfs_file_entry_t *pyfsntfs_file_entry,
           PyObject *arguments PYFSNTFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error       = NULL;
	PyObject *file_entries_object  = NULL;
	static char *function          = "pyfsntfs_file_entry_get_sub_file_entries";
	int number_of_sub_file_entries = 0;
	int result                     = 0;

	PYFSNTFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsntfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsntfs_file_entry_get_number_of_sub_file_entries(
	          pyfsntfs_file_entry->file_entry,
	          &number_of_sub_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsntfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	file_entries_object = pyfsntfs_file_entries_new(
	                       pyfsntfs_file_entry,
	                       &pyfsntfs_file_entry_get_sub_file_entry_by_index,
	                       number_of_sub_file_entries );

	if( file_entries_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file entries object.",
		 function );

		return( NULL );
	}
	return( file_entries_object );
}

