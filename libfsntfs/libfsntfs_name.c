/*
 * Name functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libfsntfs_libcerror.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_name.h"

/* Compares an UTF-8 string with an UTF-16 encoded attribute, file entry or index name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_name_compare_with_utf8_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf8_character_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function                       = "libfsntfs_name_compare_with_utf8_string";
	int result                                  = 0;

#ifdef TODO_CASE_INSENSITIVE
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf8_string_index                    = 0;
#endif

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 encoded name.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 encoded name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( name_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: missing UTF-16 encoded name value.",
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
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
#ifdef TODO_CASE_INSENSITIVE
	while( name_index < (size_t) name_size )
	{
		result = libuna_unicode_character_copy_from_utf16_stream(
			  &name_character,
			  name,
			  name_size,
			  &name_index,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded name to Unicode character.",
			 function );

			return( -1 );
		}
		if( libuna_unicode_character_copy_from_utf8(
		     &string_character,
		     utf8_string,
		     utf8_string_length,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-8 string to Unicode character.",
			 function );

			return( -1 );
		}
		if( towupper( (wint_t) name_character ) != towupper( (wint_t) string_character ) )
		{
			break;
		}
	}
	if( ( name_index == name_size )
	 && ( utf8_string_index == utf8_string_length ) )
	{
		return( 1 );
	}
	return( 0 );
#else
	result = libuna_utf8_string_compare_with_utf16_stream(
	          utf8_string,
	          utf8_string_length,
	          name,
	          name_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with UTF-16 encoded name.",
		 function );

		return( -1 );
	}
	return( result );

#endif /* TODO_CASE_INSENSITIVE */
}

/* Compares an UTF-16 string with an UTF-16 encoded attribute, file entry or index name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_name_compare_with_utf16_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf16_character_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function                       = "libfsntfs_name_compare_with_utf16_string";
	int result                                  = 0;

#ifdef TODO_CASE_INSENSITIVE
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf16_string_index                   = 0;
#endif

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 encoded name.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 encoded name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( name_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: missing UTF-16 encoded name value.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
#ifdef TODO_CASE_INSENSITIVE
	while( name_index < (size_t) name_size )
	{
		result = libuna_unicode_character_copy_from_utf16_stream(
			  &name_character,
			  name,
			  name_size,
			  &name_index,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded name to Unicode character.",
			 function );

			return( -1 );
		}
		if( libuna_unicode_character_copy_from_utf16(
		     &string_character,
		     utf16_string,
		     utf16_string_length,
		     &utf16_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 string to Unicode character.",
			 function );

			return( -1 );
		}
		if( towupper( (wint_t) name_character ) != towupper( (wint_t) string_character ) )
		{
			break;
		}
	}
	if( ( name_index == name_size )
	 && ( utf16_string_index == utf16_string_length ) )
	{
		return( 1 );
	}
	return( 0 );
#else
	result = libuna_utf16_string_compare_with_utf16_stream(
	          utf16_string,
	          utf16_string_length,
	          name,
	          name_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-16 string with UTF-16 encoded name.",
		 function );

		return( -1 );
	}
	return( result );

#endif /* TODO_CASE_INSENSITIVE */
}

