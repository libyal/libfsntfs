/*
 * String functions
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

#include "pyfsntfs_error.h"
#include "pyfsntfs_libuna.h"
#include "pyfsntfs_python.h"
#include "pyfsntfs_string.h"

#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3

/* Creates a new string object from an UTF-8 RFC 2279 encoded string
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsntfs_string_new_from_utf8_rfc2279(
           const uint8_t *utf8_string,
           size_t utf8_string_size )
{
	PyObject *string_object                              = NULL;
	libcerror_error_t *error                             = NULL;
	uint32_t *utf32_string                               = NULL;
	static char *function                                = "pyfsntfs_string_new_from_utf8_rfc2279";
	libuna_unicode_character_t largest_unicode_character = 255;
	libuna_unicode_character_t unicode_character         = 0;
	size_t utf32_string_index                            = 0;
	size_t utf32_string_length                           = 0;
	size_t utf8_string_index                             = 0;

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid UTF-8 string.",
		 function );

		return( NULL );
	}
	if( ( utf8_string_size == 0 )
	 || ( utf8_string_size > (size_t) ( SSIZE_MAX / 4 ) ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		return( NULL );
	}
	utf32_string = (uint32_t *) PyMem_Malloc(
	                             sizeof( uint32_t ) * utf8_string_size );

	if( utf32_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-32 string.",
		 function );

		goto on_error;
	}
	/* Using RFC 2279 UTF-8 to support unpaired UTF-16 surrogates
	 */
	while( utf8_string_index < utf8_string_size )
	{
		if( libuna_unicode_character_copy_from_utf8_rfc2279(
		     &unicode_character,
		     (libuna_utf8_character_t *) utf8_string,
		     utf8_string_size,
		     &utf8_string_index,
		     &error ) != 1 )
		{
			pyfsntfs_error_raise(
			 error,
			 PyExc_RuntimeError,
			 "%s: unable to copy Unicode character from string.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		if( unicode_character == 0 )
		{
			break;
		}
		utf32_string[ utf32_string_index++ ] = unicode_character;

		if( ( unicode_character >= 0x0000d800UL )
		 && ( unicode_character <= 0x0000dfffUL ) )
		{
			largest_unicode_character = 0x0010ffffUL;
		}
		else if( unicode_character > largest_unicode_character )
		{
			largest_unicode_character = unicode_character;
		}
	}
	utf32_string_length = utf32_string_index;

	if( largest_unicode_character > 65535 )
	{
		largest_unicode_character = 0x0010ffffUL;
	}
	else if( largest_unicode_character > 255 )
	{
		largest_unicode_character = 0x0000ffffUL;
	}
	/* Pass the string length to PyUnicode_New otherwise it the end of string
	 * character is part of the string. The largest Unicode character is needed
	 * to ensure Python Unicode strings are "canonical", otherwise string
	 * comparison can fail.
	 */
	string_object = PyUnicode_New(
	                 utf32_string_length, largest_unicode_character);

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create string object.",
		 function );

		goto on_error;
	}
	for( utf32_string_index = 0;
	     utf32_string_index < utf32_string_length;
	     utf32_string_index++ )
	{
		unicode_character = utf32_string[ utf32_string_index ];

		if( PyUnicode_WriteChar(
		     string_object,
		     (Py_ssize_t) utf32_string_index,
		     (Py_UCS4) unicode_character ) == -1 )
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to copy Unicode character: 0x%08" PRIx32 " at index: %" PRIzd " to string object.",
			 function,
			 unicode_character,
			 utf32_string_index );

			goto on_error;
		}
	}
	PyMem_Free(
	 utf32_string );

	return( string_object );

on_error:
	if( string_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) string_object );
	}
	if( utf32_string != NULL )
	{
		PyMem_Free(
		 utf32_string );
	}
	return( NULL );
}

/* Copies a Python Unicode string object to an UTF-8 RFC 2279 encoded string
 * Returns 1 if successful or -1 on error
 */
int pyfsntfs_string_copy_to_utf8_rfc2279(
     PyObject *string_object,
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *safe_utf8_string    = NULL;
	static char *function        = "pyfsntfs_string_copy_to_utf8_rfc2279";
	Py_UCS4 unicode_character    = 0;
	Py_ssize_t string_index      = 0;
	Py_ssize_t string_length     = 0;
	size_t safe_utf8_string_size = 0;
	size_t utf8_string_index     = 0;

	if( string_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string object.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	PyErr_Clear();

	string_length = PyUnicode_GetLength(
	                 string_object );

	if( ( string_length == 0 )
	 || ( (size_t) string_length > (size_t) ( SSIZE_MAX - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string length value out of bounds.",
		 function );

		goto on_error;
	}
	safe_utf8_string_size = string_length + 1;

	safe_utf8_string = (uint8_t *) PyMem_Malloc(
	                                sizeof( uint8_t ) * safe_utf8_string_size );

	if( safe_utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	/* Using RFC 2279 UTF-8 to support unpaired UTF-16 surrogates
	 */
	for( string_index = 0;
	     string_index < string_length;
	     string_index++ )
	{
		unicode_character = PyUnicode_ReadChar(
		                     string_object,
		                     string_index );

		if( libuna_unicode_character_copy_to_utf8_rfc2279(
		     (libuna_unicode_character_t) unicode_character,
		     (libuna_utf8_character_t *) safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy Unicode character to UTF-8 string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ string_index ] = 0;

	*utf8_string      = safe_utf8_string;
	*utf8_string_size = safe_utf8_string_size;

	return( 1 );

on_error:
	if( safe_utf8_string != NULL )
	{
		PyMem_Free(
		 safe_utf8_string );
	}
	return( -1 );
}

#endif /* PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3 */

