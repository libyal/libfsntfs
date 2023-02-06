/*
 * The compression unit data handle functions
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

#include "libfsntfs_compression_unit_data_handle.h"
#include "libfsntfs_compression_unit_descriptor.h"
#include "libfsntfs_data_run.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_mft_attribute.h"

/* Creates compression unit data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_data_handle_initialize(
     libfsntfs_compression_unit_data_handle_t **data_handle,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	libfsntfs_compression_unit_descriptor_t *descriptor = NULL;
	libfsntfs_data_run_t *data_run                      = NULL;
	libfsntfs_mft_attribute_t *safe_mft_attribute       = NULL;
	static char *function                               = "libfsntfs_compression_unit_data_handle_initialize";
	size64_t attribute_data_vcn_size                    = 0;
	size64_t calculated_allocated_data_size             = 0;
	size64_t data_run_size                              = 0;
	size64_t data_segment_size                          = 0;
	size64_t remaining_compression_unit_size            = 0;
	size64_t stored_allocated_data_size                 = 0;
	size_t compression_unit_size                        = 0;
	off64_t attribute_data_vcn_offset                   = 0;
	off64_t calculated_attribute_data_vcn_offset        = 0;
	off64_t data_offset                                 = 0;
	off64_t data_run_offset                             = 0;
	off64_t data_segment_offset                         = 0;
	uint16_t data_flags                                 = 0;
	int attribute_index                                 = 0;
	int data_run_index                                  = 0;
	int descriptor_index                                = 0;
	int entry_index                                     = 0;
	int number_of_data_runs                             = 0;
	int total_data_run_index                            = 0;
	int total_number_of_data_runs                       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *data_segment_type                             = NULL;
#endif

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle value already set.",
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
	if( io_handle->cluster_block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - cluster block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_data_flags(
	     mft_attribute,
	     &data_flags,
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
	if( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported uncompressed attribute data.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_attribute_get_allocated_data_size(
	     mft_attribute,
	     &stored_allocated_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute allocated data size.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_attribute_get_compression_unit_size(
	     mft_attribute,
	     &compression_unit_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compression unit size.",
		 function );

		goto on_error;
	}
	if( ( compression_unit_size == 0 )
	 || ( compression_unit_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compression unit size value out of bounds.",
		 function );

		goto on_error;
	}
	*data_handle = memory_allocate_structure(
	                libfsntfs_compression_unit_data_handle_t );

	if( *data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_handle,
	     0,
	     sizeof( libfsntfs_compression_unit_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		memory_free(
		 *data_handle );

		*data_handle = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *data_handle )->descriptors_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptors array.",
		 function );

		goto on_error;
	}
	safe_mft_attribute = mft_attribute;
	attribute_index    = 0;

	while( safe_mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_data_vcn_range(
		     safe_mft_attribute,
		     (uint64_t *) &attribute_data_vcn_offset,
		     (uint64_t *) &attribute_data_vcn_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute data VCN range.",
			 function );

			goto on_error;
		}
		if( attribute_data_vcn_size != 0xffffffffffffffffULL )
		{
			if( (uint64_t) attribute_data_vcn_offset > (uint64_t) ( ( INT64_MAX / io_handle->cluster_block_size ) - 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data first VCN value out of bounds.",
				 function );

				goto on_error;
			}
			if( attribute_data_vcn_size > (size64_t) ( ( INT64_MAX / io_handle->cluster_block_size ) - 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data last VCN value out of bounds.",
				 function );

				goto on_error;
			}
			if( attribute_data_vcn_offset > (off64_t) attribute_data_vcn_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data first VCN value out of bounds.",
				 function );

				goto on_error;
			}
			attribute_data_vcn_size   += 1;
			attribute_data_vcn_size   -= attribute_data_vcn_offset;
			attribute_data_vcn_offset *= io_handle->cluster_block_size;
			attribute_data_vcn_size   *= io_handle->cluster_block_size;

			if( ( calculated_attribute_data_vcn_offset != 0 )
			 && ( calculated_attribute_data_vcn_offset != attribute_data_vcn_offset ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data VCN offset value out of bounds.",
				 function );

				goto on_error;
			}
			calculated_attribute_data_vcn_offset = attribute_data_vcn_offset + (off64_t) attribute_data_vcn_size;
		}
		if( libfsntfs_mft_attribute_get_number_of_data_runs(
		     safe_mft_attribute,
		     &number_of_data_runs,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of data runs.",
			 function );

			goto on_error;
		}
		total_number_of_data_runs += number_of_data_runs;

		if( libfsntfs_mft_attribute_get_next_attribute(
		     safe_mft_attribute,
		     &safe_mft_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next MFT attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		attribute_index++;
	}
	safe_mft_attribute = mft_attribute;
	attribute_index    = 0;

	while( safe_mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_number_of_data_runs(
		     safe_mft_attribute,
		     &number_of_data_runs,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of data runs.",
			 function );

			goto on_error;
		}
		for( data_run_index = 0;
		     data_run_index < number_of_data_runs;
		     data_run_index++ )
		{
			if( libfsntfs_mft_attribute_get_data_run_by_index(
			     safe_mft_attribute,
			     data_run_index,
			     &data_run,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data run: %d.",
				 function,
				 data_run_index );

				goto on_error;
			}
			if( data_run == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing data run: %d.",
				 function,
				 data_run_index );

				goto on_error;
			}
			data_run_offset = data_run->start_offset;
			data_run_size   = data_run->size;

			calculated_allocated_data_size += data_run->size;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
				{
					data_segment_type = "sparse ";
				}
				else
				{
					data_segment_type = "";
				}
				libcnotify_printf(
				 "%s: %sdata run: %d offset: 0x%08" PRIx64 ", size: %" PRIu64 ".\n",
				 function,
				 data_segment_type,
				 data_run_index,
				 data_run_offset,
				 data_run_size );

				libcnotify_printf(
				 "\n" );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			while( data_run_size > 0 )
			{
				if( descriptor == NULL )
				{
					if( libfsntfs_compression_unit_descriptor_initialize(
					     &descriptor,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create compression unit descriptor.",
						 function );

						goto on_error;
					}
					descriptor->data_run_offset = data_run_offset;
					descriptor->data_offset     = data_offset;

					data_segment_offset = 0;

					if( data_run_size <= compression_unit_size )
					{
						descriptor->compression_unit_size = compression_unit_size;
					}
					else
					{
						descriptor->compression_unit_size = ( data_run_size / compression_unit_size ) * compression_unit_size;
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: uncompressed data run size: %" PRIu64 "\n",
							 function,
							 descriptor->compression_unit_size );
						}
#endif
					}
					remaining_compression_unit_size = descriptor->compression_unit_size;
				}
				if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) == 0 )
				{
					descriptor->data_range_flags = data_run->range_flags;
				}
				/* Sparse data runs mark the end of a compression unit and they
				 * should be at minimum the size of the remaining data in the compression unit
				 */
				else if( remaining_compression_unit_size < compression_unit_size )
				{
					if( ( total_data_run_index == ( total_number_of_data_runs - 1 ) )
					 && ( data_run_size < (size64_t) remaining_compression_unit_size ) )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: last sparse data run: %d size does not align with compression unit size.\n",
							 function,
							 data_run_index );
						}
#endif
						remaining_compression_unit_size = 0;
					}
					descriptor->data_range_flags = LIBFDATA_RANGE_FLAG_IS_COMPRESSED;
				}
				if( data_run_size < remaining_compression_unit_size )
				{
					data_segment_size = data_run_size;
				}
				else
				{
					data_segment_size = remaining_compression_unit_size;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
					{
						data_segment_type = "sparse ";
					}
					else
					{
						data_segment_type = "";
					}
					libcnotify_printf(
					 "%s: compression unit: %d %sdata segment offset: 0x%08" PRIx64 ", size: %" PRIu64 ".\n",
					 function,
					 descriptor_index,
					 data_segment_type,
					 data_segment_offset,
					 data_segment_size );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				if( libfsntfs_compression_unit_descriptor_append_data_segment(
				     descriptor,
				     data_run_offset,
				     data_segment_size,
				     data_run->range_flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append data segment to compressed block descriptor: %d.",
					 function,
					 descriptor_index );

					goto on_error;
				}
				if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) == 0 )
				{
					data_run_offset += data_segment_size;
				}
				data_segment_offset             += data_segment_size;
				data_run_size                   -= data_segment_size;
				remaining_compression_unit_size -= data_segment_size;

				if( remaining_compression_unit_size == 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( libfsntfs_compression_unit_descriptor_print(
						     descriptor,
						     io_handle,
						     descriptor_index,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
							 "%s: unable to print the compressed block descriptor: %d.",
							 function,
							 descriptor_index );

							goto on_error;
						}
					}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

					if( libcdata_array_append_entry(
					     ( *data_handle )->descriptors_array,
					     &entry_index,
					     (intptr_t *) descriptor,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append compressed block descriptor: %d to array.",
						 function,
						 descriptor_index );

						goto on_error;
					}
					data_offset += descriptor->compression_unit_size;

					descriptor = NULL;

					descriptor_index++;
				}
			}
			total_data_run_index++;
		}
		if( libfsntfs_mft_attribute_get_next_attribute(
		     safe_mft_attribute,
		     &safe_mft_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next MFT attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		attribute_index++;
	}
	if( calculated_allocated_data_size != stored_allocated_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: size of data runs: %" PRIu64 " does not match allocated data size: %" PRIu64 ".",
		 function,
		 calculated_allocated_data_size,
		 stored_allocated_data_size );

		goto on_error;
	}
	if( remaining_compression_unit_size != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid remaining compression unit size value out of bounds.",
		 function );

		goto on_error;
	}
	( *data_handle )->compression_unit_size = compression_unit_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( descriptor != NULL )
	{
		libfsntfs_compression_unit_descriptor_free(
		 &descriptor,
		 NULL );
	}
	if( *data_handle != NULL )
	{
		if( ( *data_handle )->descriptors_array != NULL )
		{
			libcdata_array_free(
			 &( ( *data_handle )->descriptors_array ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compression_unit_descriptor_free,
			 NULL );
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( -1 );
}

/* Frees a data handle
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_data_handle_free(
     libfsntfs_compression_unit_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compression_unit_data_handle_free";
	int result            = 1;

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		if( libcdata_array_free(
		     &( ( *data_handle )->descriptors_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compression_unit_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free descriptors array.",
			 function );

			result = -1;
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( result );
}

/* Retrieves the number of descriptors
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_data_handle_get_number_of_descriptors(
     libfsntfs_compression_unit_data_handle_t *data_handle,
     int *number_of_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compression_unit_data_handle_get_number_of_descriptors";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     data_handle->descriptors_array,
	     number_of_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from descriptors array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_data_handle_get_descriptor_by_index(
     libfsntfs_compression_unit_data_handle_t *data_handle,
     int descriptor_index,
     libfsntfs_compression_unit_descriptor_t **descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compression_unit_data_handle_get_descriptor_by_index";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     data_handle->descriptors_array,
	     descriptor_index,
	     (intptr_t **) descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from descriptors array.",
		 function,
		 descriptor_index );

		return( -1 );
	}
	return( 1 );
}

