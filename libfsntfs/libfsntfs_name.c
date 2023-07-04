/*
 * Name functions
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

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libfsntfs_libcerror.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_name.h"

/* Compares an (other) UTF-16 encoded name with an UTF-16 encoded name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_name_compare(
     const uint8_t *name,
     size_t name_size,
     const uint8_t *other_name,
     size_t other_name_size,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	static char *function                           = "libfsntfs_name_compare";
	libuna_unicode_character_t name_character       = 0;
	libuna_unicode_character_t other_name_character = 0;
	size_t name_index                               = 0;
	size_t other_name_index                         = 0;

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
	if( other_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 encoded other name.",
		 function );

		return( -1 );
	}
	if( other_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 encoded other name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( other_name_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: missing UTF-16 encoded other name value.",
		 function );

		return( -1 );
	}
	/* Check if the UTF-16 streams are terminated with zero bytes
	 */
	if( ( name_size >= 2 )
	 && ( name[ name_size - 2 ] == 0 )
	 && ( name[ name_size - 1 ] == 0 ) )
	{
		name_size -= 2;
	}
	if( ( other_name_size >= 2 )
	 && ( other_name[ other_name_size - 2 ] == 0 )
	 && ( other_name[ other_name_size - 1 ] == 0 ) )
	{
		other_name_size -= 2;
	}
	while( ( name_index < name_size )
	    && ( other_name_index < other_name_size ) )
	{
		if( libuna_unicode_character_copy_from_utf16_stream(
		     &name_character,
		     name,
		     name_size,
		     &name_index,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded name to Unicode character.",
			 function );

			return( -1 );
		}
		if( libuna_unicode_character_copy_from_utf16_stream(
		     &other_name_character,
		     other_name,
		     other_name_size,
		     &other_name_index,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded other name to Unicode character.",
			 function );

			return( -1 );
		}
		if( use_case_folding != 0 )
		{
			name_character       = (libuna_unicode_character_t) towupper( (wint_t) name_character );
			other_name_character = (libuna_unicode_character_t) towupper( (wint_t) other_name_character );
		}
		if( other_name_character < name_character )
		{
			return( LIBUNA_COMPARE_LESS );
		}
		else if( other_name_character > name_character )
		{
			return( LIBUNA_COMPARE_GREATER );
		}
	}
	if( other_name_index < other_name_size )
	{
		return( LIBUNA_COMPARE_GREATER );
	}
	else if( name_index < name_size )
	{
		return( LIBUNA_COMPARE_LESS );
	}
	return( LIBUNA_COMPARE_EQUAL );
}

/* Compares an UTF-16 encoded short name with an UTF-16 encoded (long) name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_name_compare_short(
     const uint8_t *name,
     size_t name_size,
     const uint8_t *short_name,
     size_t short_name_size,
     libcerror_error_t **error )
{
	static char *function                           = "libfsntfs_name_compare_short";
	libuna_unicode_character_t name_character       = 0;
	libuna_unicode_character_t short_name_character = 0;
	size_t last_match_name_index                    = 0;
	size_t last_match_short_name_index              = 0;
	size_t name_index                               = 0;
	size_t short_name_index                         = 0;

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
	if( short_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 encoded short name.",
		 function );

		return( -1 );
	}
	if( short_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 encoded short name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( short_name_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: missing UTF-16 encoded short name value.",
		 function );

		return( -1 );
	}
	/* Check if the UTF-16 streams are terminated with zero bytes
	 */
	if( ( name_size >= 2 )
	 && ( name[ name_size - 2 ] == 0 )
	 && ( name[ name_size - 1 ] == 0 ) )
	{
		name_size -= 2;
	}
	if( ( short_name_size >= 2 )
	 && ( short_name[ short_name_size - 2 ] == 0 )
	 && ( short_name[ short_name_size - 1 ] == 0 ) )
	{
		short_name_size -= 2;
	}
	while( ( name_index < name_size )
	    && ( short_name_index < short_name_size ) )
	{
		if( libuna_unicode_character_copy_from_utf16_stream(
		     &name_character,
		     name,
		     name_size,
		     &name_index,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded name to Unicode character.",
			 function );

			return( -1 );
		}
		/* Ignore:
		 * control characters and space
		 * '"' '*' '+' ',' '.' '/' ':' ';' '<' '=' '>' '?' '\'
		 * non 7-bit ASCII characters
		 */
		if( ( name_character <= 0x20 )
		 || ( name_character == 0x22 )
		 || ( ( name_character >= 0x2a )
		  &&  ( name_character <= 0x2c ) )
		 || ( name_character == 0x2e )
		 || ( name_character == 0x2f )
		 || ( ( name_character >= 0x3a )
		  &&  ( name_character <= 0x3f ) )
		 || ( name_character == 0x5c )
		 || ( name_character >= 0x80 ) )
		{
			continue;
		}
		/* The [ and ] characters are replaced by an underscore (_)
		 */
		if( ( name_character == 0x5b )
		 || ( name_character == 0x5d ) )
		{
			name_character = 0x5f;
		}
		if( libuna_unicode_character_copy_from_utf16_stream(
		     &short_name_character,
		     short_name,
		     short_name_size,
		     &short_name_index,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded short name to Unicode character.",
			 function );

			return( -1 );
		}
		if( short_name_character == 0x7e )
		{
			break;
		}
		name_character       = (libuna_unicode_character_t) towupper( (wint_t) name_character );
		short_name_character = (libuna_unicode_character_t) towupper( (wint_t) short_name_character );

		if( last_match_short_name_index < 4 )
		{
			if( short_name_character < name_character )
			{
				return( LIBUNA_COMPARE_LESS );
			}
			else if( short_name_character > name_character )
			{
				return( LIBUNA_COMPARE_GREATER );
			}
		}
		else if( short_name_character != name_character )
		{
/* TODO handle hex notation */
			break;
		}
		last_match_name_index       = name_index;
		last_match_short_name_index = short_name_index;
	}
	while( ( short_name_index < short_name_size )
	    && ( short_name_character != 0x7e ) )
	{
		if( libuna_unicode_character_copy_from_utf16_stream(
		     &short_name_character,
		     short_name,
		     short_name_size,
		     &short_name_index,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded short name to Unicode character.",
			 function );

			return( -1 );
		}
	}
	if( short_name_character != 0x7e )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid short name - missing tilde character\n",
		 function );

		return( -1 );
	}
	while( short_name_index < short_name_size )
	{
		if( libuna_unicode_character_copy_from_utf16_stream(
		     &short_name_character,
		     short_name,
		     short_name_size,
		     &short_name_index,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded short name to Unicode character.",
			 function );

			return( -1 );
		}
		if( short_name_character == 0x2e )
		{
			break;
		}
		/* The ~ suffix should only consist of the numeric characters 0 - 9
		 */
		if( ( short_name_character < 0x30 )
		 || ( short_name_character > 0x39 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid short name - non-numeric character in tilde suffix.",
			 function );

			return( -1 );
		}
	}
	if( short_name_character == 0x2e )
	{
		name_index = name_size;

		while( name_index >= 2 )
		{
			name_index -= 2;

			if( libuna_unicode_character_copy_from_utf16_stream(
			     &name_character,
			     name,
			     name_size,
			     &name_index,
			     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 encoded name to Unicode character.",
				 function );

				return( -1 );
			}
			if( name_character == 0x2e )
			{
				break;
			}
			name_index -= 2;
		}
	}
	if( name_character == 0x2e )
	{
		while( ( name_index < name_size )
		    && ( short_name_index < short_name_size ) )
		{
			if( libuna_unicode_character_copy_from_utf16_stream(
			     &name_character,
			     name,
			     name_size,
			     &name_index,
			     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 encoded name to Unicode character.",
				 function );

				return( -1 );
			}
			/* Ignore:
			 * control characters and space
			 * '"' '*' '+' ',' '/' ':' ';' '<' '=' '>' '?' '\'
			 * non 7-bit ASCII characters
			 */
			if( ( name_character <= 0x20 )
			 || ( name_character == 0x22 )
			 || ( ( name_character >= 0x2a )
			  &&  ( name_character <= 0x2c ) )
			 || ( name_character == 0x2f )
			 || ( ( name_character >= 0x3a )
			  &&  ( name_character <= 0x3f ) )
			 || ( name_character == 0x5c )
			 || ( name_character >= 0x80 ) )
			{
				continue;
			}
			if( libuna_unicode_character_copy_from_utf16_stream(
			     &short_name_character,
			     short_name,
			     short_name_size,
			     &short_name_index,
			     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 encoded short name to Unicode character.",
				 function );

				return( -1 );
			}
			name_character       = (libuna_unicode_character_t) towupper( (wint_t) name_character );
			short_name_character = (libuna_unicode_character_t) towupper( (wint_t) short_name_character );

			if( short_name_character != name_character )
			{
				break;
			}
		}
	}
	if( short_name_index < short_name_size )
	{
		name_index       = last_match_name_index;
		short_name_index = last_match_short_name_index;

		/* The name was shorter than the short name
		 */
		if( name_index >= name_size )
		{
			return( LIBUNA_COMPARE_GREATER );
		}
		/* Compare the remainder of the name and short name as a case-insenstive string
		 */
		while( ( name_index < name_size )
		    && ( short_name_index < short_name_size ) )
		{
			if( libuna_unicode_character_copy_from_utf16_stream(
			     &name_character,
			     name,
			     name_size,
			     &name_index,
			     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 encoded name to Unicode character.",
				 function );

				return( -1 );
			}
			if( libuna_unicode_character_copy_from_utf16_stream(
			     &short_name_character,
			     short_name,
			     short_name_size,
			     &short_name_index,
			     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 encoded short name to Unicode character.",
				 function );

				return( -1 );
			}
			name_character       = (libuna_unicode_character_t) towupper( (wint_t) name_character );
			short_name_character = (libuna_unicode_character_t) towupper( (wint_t) short_name_character );

			if( short_name_character < name_character )
			{
				return( LIBUNA_COMPARE_LESS );
			}
			else if( short_name_character > name_character )
			{
				return( LIBUNA_COMPARE_GREATER );
			}
		}
	}
	return( LIBUNA_COMPARE_EQUAL );
}

/* Compares an UTF-8 string with an UTF-16 encoded name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_name_compare_with_utf8_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf8_character_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	static char *function                       = "libfsntfs_name_compare_with_utf8_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf8_string_index                    = 0;

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
	/* Check if the UTF-16 stream is terminated with zero bytes
	 */
	if( ( name_size >= 2 )
	 && ( name[ name_size - 2 ] == 0 )
	 && ( name[ name_size - 1 ] == 0 ) )
	{
		name_size -= 2;
	}
	if( ( utf8_string_length >= 1 )
	 && ( utf8_string[ utf8_string_length - 1 ] == 0 ) )
	{
		utf8_string_length -= 1;
	}
	while( ( name_index < name_size )
	    && ( utf8_string_index < utf8_string_length ) )
	{
		if( libuna_unicode_character_copy_from_utf16_stream(
		     &name_character,
		     name,
		     name_size,
		     &name_index,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded name to Unicode character.",
			 function );

			return( -1 );
		}
		/* Using RFC 2279 UTF-8 to support unpaired UTF-16 surrogates
		 */
		if( libuna_unicode_character_copy_from_utf8_rfc2279(
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
			 "%s: unable to copy UTF-8 RFC 2279 string to Unicode character.",
			 function );

			return( -1 );
		}
		if( use_case_folding != 0 )
		{
			name_character   = (libuna_unicode_character_t) towupper( (wint_t) name_character );
			string_character = (libuna_unicode_character_t) towupper( (wint_t) string_character );
		}
		if( string_character < name_character )
		{
			return( LIBUNA_COMPARE_LESS );
		}
		else if( string_character > name_character )
		{
			return( LIBUNA_COMPARE_GREATER );
		}
	}
	if( utf8_string_index < utf8_string_length )
	{
		return( LIBUNA_COMPARE_GREATER );
	}
	else if( name_index < name_size )
	{
		return( LIBUNA_COMPARE_LESS );
	}
	return( LIBUNA_COMPARE_EQUAL );
}

/* Compares an UTF-16 string with an UTF-16 encoded name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_name_compare_with_utf16_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf16_character_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	static char *function                       = "libfsntfs_name_compare_with_utf16_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf16_string_index                   = 0;

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
	/* Check if the UTF-16 stream is terminated with zero bytes
	 */
	if( ( name_size >= 2 )
	 && ( name[ name_size - 2 ] == 0 )
	 && ( name[ name_size - 1 ] == 0 ) )
	{
		name_size -= 2;
	}
	if( ( utf16_string_length >= 1 )
	 && ( utf16_string[ utf16_string_length - 1 ] == 0 ) )
	{
		utf16_string_length -= 1;
	}
	while( ( name_index < name_size )
	    && ( utf16_string_index < utf16_string_length ) )
	{
		if( libuna_unicode_character_copy_from_utf16_stream(
		     &name_character,
		     name,
		     name_size,
		     &name_index,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 encoded name to Unicode character.",
			 function );

			return( -1 );
		}
		/* Using UCS-2 to support unpaired UTF-16 surrogates
		 */
		if( libuna_unicode_character_copy_from_ucs2(
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
			 "%s: unable to copy UCS-2 string to Unicode character.",
			 function );

			return( -1 );
		}
		if( use_case_folding != 0 )
		{
			name_character   = (libuna_unicode_character_t) towupper( (wint_t) name_character );
			string_character = (libuna_unicode_character_t) towupper( (wint_t) string_character );
		}
		if( string_character < name_character )
		{
			return( LIBUNA_COMPARE_LESS );
		}
		else if( string_character > name_character )
		{
			return( LIBUNA_COMPARE_GREATER );
		}
	}
	if( utf16_string_index < utf16_string_length )
	{
		return( LIBUNA_COMPARE_GREATER );
	}
	else if( name_index < name_size )
	{
		return( LIBUNA_COMPARE_LESS );
	}
	return( LIBUNA_COMPARE_EQUAL );
}

