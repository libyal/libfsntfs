/*
 * Compression functions
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "libfsntfs_compression.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfwnt.h"

/* Decompresses data using the compression method
 * Returns 1 on success, 0 on failure or -1 on error
 */
int libfsntfs_decompress_data(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     int compression_method,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_decompress_data";

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == compressed_data )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data buffer equals uncompressed data buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	switch( compression_method )
	{
		case LIBFSNTFS_COMPRESSION_METHOD_LZNT1:
			if( libfwnt_lznt1_decompress(
			     compressed_data,
			     compressed_data_size,
			     uncompressed_data,
			     uncompressed_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
				 "%s: unable to decompress LZNT1 compressed data.",
				 function );

				return( -1 );
			}
			break;

		case LIBFSNTFS_COMPRESSION_METHOD_LZX:
			if( libfwnt_lzx_decompress(
			     compressed_data,
			     compressed_data_size,
			     uncompressed_data,
			     uncompressed_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
				 "%s: unable to decompress LZX compressed data.",
				 function );

				return( -1 );
			}
			break;

		case LIBFSNTFS_COMPRESSION_METHOD_LZXPRESS_HUFFMAN:
			if( libfwnt_lzxpress_huffman_decompress(
			     compressed_data,
			     compressed_data_size,
			     uncompressed_data,
			     uncompressed_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
				 "%s: unable to decompress LZXPRESS Huffman compressed data.",
				 function );

				return( -1 );
			}
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compression method.",
			 function );

			return( -1 );
	}
	return( 1 );
}

