/*
 * Python object definition of the attributes sequence and iterator
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

#include "pyfsntfs_attributes.h"
#include "pyfsntfs_file_entry.h"
#include "pyfsntfs_libcerror.h"
#include "pyfsntfs_libfsntfs.h"
#include "pyfsntfs_python.h"

PySequenceMethods pyfsntfs_attributes_sequence_methods = {
	/* sq_length */
	(lenfunc) pyfsntfs_attributes_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyfsntfs_attributes_getitem,
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

PyTypeObject pyfsntfs_attributes_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsntfs._attributes",
	/* tp_basicsize */
	sizeof( pyfsntfs_attributes_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsntfs_attributes_free,
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
	&pyfsntfs_attributes_sequence_methods,
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
	"internal pyfsntfs attributes sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyfsntfs_attributes_iter,
	/* tp_iternext */
	(iternextfunc) pyfsntfs_attributes_iternext,
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
	(initproc) pyfsntfs_attributes_init,
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

/* Creates a new attributes object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_attributes_new(
           pyfsntfs_file_entry_t *file_entry_object,
           PyObject* (*get_attribute_by_index)(
                        pyfsntfs_file_entry_t *file_entry_object,
                        int attribute_index ),
           int number_of_attributes )
{
	pyfsntfs_attributes_t *pyfsntfs_attributes = NULL;
	static char *function                      = "pyfsntfs_attributes_new";

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry object.",
		 function );

		return( NULL );
	}
	if( get_attribute_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get attribute by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the attributes values are initialized
	 */
	pyfsntfs_attributes = PyObject_New(
	                       struct pyfsntfs_attributes,
	                       &pyfsntfs_attributes_type_object );

	if( pyfsntfs_attributes == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize attributes.",
		 function );

		goto on_error;
	}
	if( pyfsntfs_attributes_init(
	     pyfsntfs_attributes ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize attributes.",
		 function );

		goto on_error;
	}
	pyfsntfs_attributes->file_entry_object      = file_entry_object;
	pyfsntfs_attributes->get_attribute_by_index = get_attribute_by_index;
	pyfsntfs_attributes->number_of_attributes   = number_of_attributes;

	Py_IncRef(
	 (PyObject *) pyfsntfs_attributes->file_entry_object );

	return( (PyObject *) pyfsntfs_attributes );

on_error:
	if( pyfsntfs_attributes != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_attributes );
	}
	return( NULL );
}

/* Intializes an attributes object
 * Returns 0 if successful or -1 on error
 */
int pyfsntfs_attributes_init(
     pyfsntfs_attributes_t *pyfsntfs_attributes )
{
	static char *function = "pyfsntfs_attributes_init";

	if( pyfsntfs_attributes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes.",
		 function );

		return( -1 );
	}
	/* Make sure the attributes values are initialized
	 */
	pyfsntfs_attributes->file_entry_object      = NULL;
	pyfsntfs_attributes->get_attribute_by_index = NULL;
	pyfsntfs_attributes->attribute_index        = 0;
	pyfsntfs_attributes->number_of_attributes   = 0;

	return( 0 );
}

/* Frees an attributes object
 */
void pyfsntfs_attributes_free(
      pyfsntfs_attributes_t *pyfsntfs_attributes )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyfsntfs_attributes_free";

	if( pyfsntfs_attributes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsntfs_attributes );

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
	if( pyfsntfs_attributes->file_entry_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsntfs_attributes->file_entry_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsntfs_attributes );
}

/* The attributes len() function
 */
Py_ssize_t pyfsntfs_attributes_len(
            pyfsntfs_attributes_t *pyfsntfs_attributes )
{
	static char *function = "pyfsntfs_attributes_len";

	if( pyfsntfs_attributes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pyfsntfs_attributes->number_of_attributes );
}

/* The attributes getitem() function
 */
PyObject *pyfsntfs_attributes_getitem(
           pyfsntfs_attributes_t *pyfsntfs_attributes,
           Py_ssize_t item_index )
{
	PyObject *attribute_object = NULL;
	static char *function      = "pyfsntfs_attributes_getitem";

	if( pyfsntfs_attributes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_attributes->get_attribute_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes - missing get attribute by index function.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_attributes->number_of_attributes < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes - invalid number of attributes.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pyfsntfs_attributes->number_of_attributes ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	attribute_object = pyfsntfs_attributes->get_attribute_by_index(
	                    pyfsntfs_attributes->file_entry_object,
	                    (int) item_index );

	return( attribute_object );
}

/* The attributes iter() function
 */
PyObject *pyfsntfs_attributes_iter(
           pyfsntfs_attributes_t *pyfsntfs_attributes )
{
	static char *function = "pyfsntfs_attributes_iter";

	if( pyfsntfs_attributes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyfsntfs_attributes );

	return( (PyObject *) pyfsntfs_attributes );
}

/* The attributes iternext() function
 */
PyObject *pyfsntfs_attributes_iternext(
           pyfsntfs_attributes_t *pyfsntfs_attributes )
{
	PyObject *attribute_object = NULL;
	static char *function      = "pyfsntfs_attributes_iternext";

	if( pyfsntfs_attributes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_attributes->get_attribute_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes - missing get attribute by index function.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_attributes->attribute_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes - invalid attribute index.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_attributes->number_of_attributes < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid attributes - invalid number of attributes.",
		 function );

		return( NULL );
	}
	if( pyfsntfs_attributes->attribute_index >= pyfsntfs_attributes->number_of_attributes )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	attribute_object = pyfsntfs_attributes->get_attribute_by_index(
	                    pyfsntfs_attributes->file_entry_object,
	                    pyfsntfs_attributes->attribute_index );

	if( attribute_object != NULL )
	{
		pyfsntfs_attributes->attribute_index++;
	}
	return( attribute_object );
}

