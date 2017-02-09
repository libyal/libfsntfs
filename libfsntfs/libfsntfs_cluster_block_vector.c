/*
 * The cluster block vector functions
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_vector.h"
#include "libfsntfs_data_run.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_unused.h"

/* Creates a cluster block vector
 * Make sure the value cluster_block_vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_vector_initialize(
     libfdata_vector_t **cluster_block_vector,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	libfsntfs_data_run_t *data_run = NULL;
	static char *function          = "libfsntfs_cluster_block_vector_initialize";
	int attribute_index            = 0;
	int entry_index                = 0;
	int number_of_entries          = 0;
	int segment_index              = 0;
	uint16_t attribute_data_flags  = 0;

	if( cluster_block_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block vector.",
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
	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_data_flags(
	     attribute,
	     &attribute_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute data flags.",
		 function );

		goto on_error;
	}
	if( ( attribute_data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compressed attribute data.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_initialize(
	     cluster_block_vector,
	     (size64_t) io_handle->cluster_block_size,
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_cluster_block_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block vector.",
		 function );

		goto on_error;
	}
	while( attribute != NULL )
	{
/* TODO check VCN of previous attribute? */
		if( libfsntfs_attribute_get_number_of_data_runs(
		     attribute,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d number of data runs.",
			 function,
			 attribute_index );

			goto on_error;
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( libfsntfs_attribute_get_data_run_by_index(
			     attribute,
			     entry_index,
			     &data_run,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d data runs array entry: %d.",
				 function,
				 attribute_index,
				 entry_index );

				goto on_error;
			}
			if( data_run == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing attribute: %d data run: %d.",
				 function,
				 attribute_index,
				 entry_index );

				goto on_error;
			}
			if( libfdata_vector_append_segment(
			     *cluster_block_vector,
			     &segment_index,
			     0,
			     data_run->start_offset,
			     data_run->size,
			     data_run->range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append attribute: %d data run: %d vector segment.",
				 function,
				 attribute_index,
				 entry_index );

				goto on_error;
			}
		}
		attribute_index++;

		if( libfsntfs_attribute_get_chained_attribute(
		     attribute,
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chained attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *cluster_block_vector != NULL )
	{
		libfdata_vector_free(
		 cluster_block_vector,
		 NULL );
	}
	return( -1 );
}

