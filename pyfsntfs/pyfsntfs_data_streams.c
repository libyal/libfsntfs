/*
 * Python object definition of the data streams sequence and iterator
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfsntfs_data_streams.h"
#include "pyfsntfs_file_entry.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

PySequenceMethods pyfsntfs_data_streams_sequence_methods = {
	/* sq_length */
	(lenfunc) pyfsntfs_data_streams_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyfsntfs_data_streams_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pyfsntfs_data_streams_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs._data_streams",
	/* tp_basicsize */
	sizeof( pyfsntfs_data_streams_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_data_streams_free,
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
	&pyfsntfs_data_streams_sequence_methods,
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
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pyfsntfs data streams sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyfsntfs_data_streams_iter,
	/* tp_iternext */
	(iternextfunc) pyfsntfs_data_streams_iternext,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
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
	(initproc) pyfsntfs_data_streams_init,
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

/* Creates a new data streams object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_data_streams_new(
           pyfsntfs_file_entry_t *file_entry_object,
           PyObject* (*get_data_stream_by_index)(
                        pyfsntfs_file_entry_t *file_entry_object,
                        int data_stream_index ),
           int number_of_data_streams )
{
	pyfsntfs_data_streams_t *pyfsntfs_data_streams = NULL;
	static char *function                          = "pyfsntfs_data_streams_new";

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry object.",
		 function );

		return( NULL );
	}
	if( get_data_stream_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get data stream by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the data streams values are initialized
	 */
	pyfsntfs_data_streams = PyObject_New(
	                         struct pyfsntfs_data_streams,
	                         &pyfsntfs_data_streams_type_object );

	if( pyfsntfs_data_streams == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize data streams.",
		 function );

		goto on_error;
	}
	if( pyfsntfs_data_streams_init(
	     pyfsntfs_data_streams ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize data streams.",
		 function );

		goto on_error;
	}
	pyfsntfs_data_streams->file_entry_object        = file_entry_object;
	pyfsntfs_data_streams->get_data_stream_by_index = get_data_stream_by_index;
	pyfsntfs_data_streams->number_of_data_streams   = number_of_data_streams;

	Py_IncRef(
	 (PyObject *) pyfsntfs_data_streams->file_entry_object );

	return( (PyObject *) pyfsntfs_data_streams );

on_error:
	if( pyfsntfs_data_streams != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_data_streams );
	}
	return( NULL );
}

/* Intializes a data streams object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_data_streams_init(
     pyfsntfs_data_streams_t *pyfsntfs_data_streams )
{
	static char *function = "pyfsntfs_data_streams_init";

	if( pyfsntfs_data_streams == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data_streams.",
		 function );

		return( -1 );
	}
	/* Make sure the data streams values are initialized
	 */
	pyfsntfs_data_streams->file_entry_object        = NULL;
	pyfsntfs_data_streams->get_data_stream_by_index = NULL;
	pyfsntfs_data_streams->data_stream_index        = 0;
	pyfsntfs_data_streams->number_of_data_streams   = 0;

	return( 0 );
}

/* Frees a data streams object
 */
void pyfsntfs_data_streams_free(
      pyfsntfs_data_streams_t *pyfsntfs_data_streams )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyfsntfs_data_streams_free";

	if( pyfsntfs_data_streams == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsntfs_data_streams );

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
	if( pyfsntfs_data_streams->file_entry_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_data_streams->file_entry_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsntfs_data_streams );
}

/* The data streams len() function
 */
Py_ssize_t pyfsntfs_data_streams_len(
            pyfsntfs_data_streams_t *pyfsntfs_data_streams )
{
	static char *function = "pyfsntfs_data_streams_len";

	if( pyfsntfs_data_streams == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pyfsntfs_data_streams->number_of_data_streams );
}

/* The data streams getitem() function
 */
PyObject *pyfsntfs_data_streams_getitem(
           pyfsntfs_data_streams_t *pyfsntfs_data_streams,
           Py_ssize_t item_index )
{
	PyObject *data_stream_object = NULL;
	static char *function        = "pyfsntfs_data_streams_getitem";

	if( pyfsntfs_data_streams == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_streams->get_data_stream_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams - missing get data stream by index function.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_streams->number_of_data_streams < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams - invalid number of data streams.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pyfsntfs_data_streams->number_of_data_streams ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	data_stream_object = pyfsntfs_data_streams->get_data_stream_by_index(
	                      pyfsntfs_data_streams->file_entry_object,
	                      (int) item_index );

	return( data_stream_object );
}

/* The data streams iter() function
 */
PyObject *pyfsntfs_data_streams_iter(
           pyfsntfs_data_streams_t *pyfsntfs_data_streams )
{
	static char *function = "pyfsntfs_data_streams_iter";

	if( pyfsntfs_data_streams == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyfsntfs_data_streams );

	return( (PyObject *) pyfsntfs_data_streams );
}

/* The data streams iternext() function
 */
PyObject *pyfsntfs_data_streams_iternext(
           pyfsntfs_data_streams_t *pyfsntfs_data_streams )
{
	PyObject *data_stream_object = NULL;
	static char *function        = "pyfsntfs_data_streams_iternext";

	if( pyfsntfs_data_streams == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_streams->get_data_stream_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams - missing get data stream by index function.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_streams->data_stream_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams - invalid data stream index.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_streams->number_of_data_streams < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid data streams - invalid number of data streams.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_data_streams->data_stream_index >= pyfsntfs_data_streams->number_of_data_streams )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	data_stream_object = pyfsntfs_data_streams->get_data_stream_by_index(
	                      pyfsntfs_data_streams->file_entry_object,
	                      pyfsntfs_data_streams->data_stream_index );

	if( data_stream_object != NULL )
	{
		pyfsntfs_data_streams->data_stream_index++;
	}
	return( data_stream_object );
}

