/*
 * Compressed block functions
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

#include "libfsntfs_compressed_block.h"
#include "libfsntfs_libcerror.h"

/* Creates a compressed block
 * Make sure the value compressed_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_initialize(
     libfsntfs_compressed_block_t **compressed_block,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_initialize";

	if( compressed_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block.",
		 function );

		return( -1 );
	}
	if( *compressed_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid compressed block value already set.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	*compressed_block = memory_allocate_structure(
	                     libfsntfs_compressed_block_t );

	if( *compressed_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *compressed_block,
	     0,
	     sizeof( libfsntfs_compressed_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear compressed block.",
		 function );

		memory_free(
		 *compressed_block );

		*compressed_block = NULL;

		return( -1 );
	}
	( *compressed_block )->data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * data_size );

	if( ( *compressed_block )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	( *compressed_block )->data_size = data_size;

	return( 1 );

on_error:
	if( *compressed_block != NULL )
	{
		memory_free(
		 *compressed_block );

		*compressed_block = NULL;
	}
	return( -1 );
}

/* Frees a compressed block
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_free(
     libfsntfs_compressed_block_t **compressed_block,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_free";

	if( compressed_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block.",
		 function );

		return( -1 );
	}
	if( *compressed_block != NULL )
	{
		if( ( *compressed_block )->data != NULL )
		{
			memory_free(
			 ( *compressed_block )->data );
		}
		memory_free(
		 *compressed_block );

		*compressed_block = NULL;
	}
	return( 1 );
}

