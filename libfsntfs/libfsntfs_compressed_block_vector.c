/*
 * Compressed block vector functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_compressed_block.h"
#include "libfsntfs_compressed_block_vector.h"
#include "libfsntfs_compression_unit_data_handle.h"
#include "libfsntfs_compression_unit_descriptor.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"

/* Creates a compressed block vector
 * Make sure the value compressed_block_vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_vector_initialize(
     libfdata_vector_t **compressed_block_vector,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	libfdata_vector_t *safe_compressed_block_vector       = NULL;
	libfsntfs_compression_unit_descriptor_t *descriptor   = NULL;
	libfsntfs_compression_unit_data_handle_t *data_handle = NULL;
	static char *function                                 = "libfsntfs_compressed_block_vector_initialize";
	int descriptor_index                                  = 0;
	int number_of_descriptors                             = 0;
	int segment_index                                     = 0;

	if( compressed_block_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block vector.",
		 function );

		return( -1 );
	}
	if( *compressed_block_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid compressed block vector value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_compression_unit_data_handle_initialize(
	     &data_handle,
	     io_handle,
	     mft_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compression unit data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_initialize(
	     &safe_compressed_block_vector,
	     (size64_t) data_handle->compression_unit_size,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compression_unit_data_handle_free,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_compressed_block_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compressed block vector.",
		 function );

		goto on_error;
	}
	if( libfsntfs_compression_unit_data_handle_get_number_of_descriptors(
	     data_handle,
	     &number_of_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of descriptors.",
		 function );

		data_handle = NULL;

		goto on_error;
	}
	for( descriptor_index = 0;
	     descriptor_index < number_of_descriptors;
	     descriptor_index++ )
	{
		if( libfsntfs_compression_unit_data_handle_get_descriptor_by_index(
		     data_handle,
		     descriptor_index,
		     &descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor: %d.",
			 function,
			 descriptor_index );

			data_handle = NULL;

			goto on_error;
		}
		if( descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing descriptor: %d.",
			 function,
			 descriptor_index );

			data_handle = NULL;

			goto on_error;
		}
		if( libfdata_vector_append_segment(
		     safe_compressed_block_vector,
		     &segment_index,
		     0,
		     descriptor->data_offset,
		     data_handle->compression_unit_size,
		     descriptor->data_range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append compression unit: %d segment to compressed block vector.",
			 function,
			 descriptor );

			data_handle = NULL;

			goto on_error;
		}
	}
	*compressed_block_vector = safe_compressed_block_vector;

	return( 1 );

on_error:
	if( safe_compressed_block_vector != NULL )
	{
		libfdata_vector_free(
		 &safe_compressed_block_vector,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfsntfs_compression_unit_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

