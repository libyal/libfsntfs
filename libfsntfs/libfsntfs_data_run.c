/*
 * Data run functions
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

#include "libfsntfs_data_run.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

/* Creates a data run
 * Make sure the value data_run is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_run_initialize(
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_data_run_initialize";

	if( data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
		 function );

		return( -1 );
	}
	if( *data_run != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data run value already set.",
		 function );

		return( -1 );
	}
	*data_run = memory_allocate_structure(
	             libfsntfs_data_run_t );

	if( *data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data run.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_run,
	     0,
	     sizeof( libfsntfs_data_run_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data run.",
		 function );

		memory_free(
		 *data_run );

		*data_run = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *data_run != NULL )
	{
		memory_free(
		 *data_run );

		*data_run = NULL;
	}
	return( -1 );
}

/* Frees a data run
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_run_free(
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_data_run_free";

	if( data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
		 function );

		return( -1 );
	}
	if( *data_run != NULL )
	{
		memory_free(
		 *data_run );

		*data_run = NULL;
	}
	return( 1 );
}

/* Clones a data run
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_run_clone(
     libfsntfs_data_run_t **destination_data_run,
     libfsntfs_data_run_t *source_data_run,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_data_run_clone";

	if( destination_data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
		 function );

		return( -1 );
	}
	if( *destination_data_run != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination data run value already set.",
		 function );

		return( -1 );
	}
	if( source_data_run == NULL )
	{
		*destination_data_run = source_data_run;

		return( 1 );
	}
	*destination_data_run = memory_allocate_structure(
	                         libfsntfs_data_run_t );

	if( *destination_data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destnation data run.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_data_run,
	     source_data_run,
	     sizeof( libfsntfs_data_run_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to copy source data run to destination.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_data_run != NULL )
	{
		memory_free(
		 *destination_data_run );

		*destination_data_run = NULL;
	}
	return( -1 );
}

/* Reads the data run
 * Returns 1 if successful or -1 on error
 */
ssize_t libfsntfs_data_run_read_data(
         libfsntfs_data_run_t *data_run,
         libfsntfs_io_handle_t *io_handle,
         const uint8_t *data,
         size_t data_size,
         uint64_t last_cluster_block_number,
         libcerror_error_t **error )
{
	static char *function               = "libfsntfs_data_run_read_data";
	size_t data_offset                  = 0;
	size_t data_run_data_size           = 0;
	uint64_t number_of_cluster_blocks   = 0;
	int64_t cluster_block_number        = 0;
	uint8_t cluster_block_value_size    = 0;
	uint8_t number_of_blocks_value_size = 0;
	uint8_t value_index                 = 0;
	uint8_t value_size_tuple            = 0;

	if( data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 1 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	value_size_tuple   = data[ 0 ];
	number_of_blocks_value_size = value_size_tuple & 0x0f;
	cluster_block_value_size    = ( value_size_tuple >> 4 ) & 0x0f;
	data_run_data_size          = 1 + number_of_blocks_value_size + cluster_block_value_size;

	if( data_run_data_size > data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data run:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_run_data_size,
		 0 );
	}
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value sizes\t\t\t\t: %" PRIu8 ", %" PRIu8 "\n",
		 function,
		 number_of_blocks_value_size,
		 cluster_block_value_size );
	}
#endif
	/* A number of cluster blocks value size of zero indicates the end of the data runs.
	 */
	if( number_of_blocks_value_size == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n%s: end of data runs (number of cluster blocks value size is zero).\n\n",
			 function );
		}
#endif
		data_run->start_offset = 0;
		data_run->size         = 0;
		data_run->range_flags  = 0;

		return( 1 );
	}
	data_offset = 1;

	/* Determine the number of cluster blocks value
	 */
	number_of_cluster_blocks = 0;

	for( value_index = number_of_blocks_value_size;
	     value_index > 0;
	     value_index-- )
	{
		number_of_cluster_blocks <<= 8;
		number_of_cluster_blocks  |= data[ data_offset + value_index - 1 ];
	}
	data_offset += number_of_blocks_value_size;

	if( cluster_block_value_size == 0 )
	{
		data_run->cluster_block_number = 0;
		data_run->start_offset         = 0;
		data_run->range_flags          = LIBFDATA_RANGE_FLAG_IS_SPARSE;
	}
	else
	{
		/* Determine the cluster block number value
		 */
		if( ( last_cluster_block_number != 0 )
		 && ( ( data[ data_offset + cluster_block_value_size - 1 ] & 0x80 ) != 0 ) )
		{
			cluster_block_number = -1;
		}
		else
		{
			cluster_block_number = 0;
		}
		for( value_index = cluster_block_value_size;
		     value_index > 0;
		     value_index-- )
		{
			cluster_block_number  = (int64_t) ( (uint64_t) cluster_block_number << 8 );
			cluster_block_number |= data[ data_offset + value_index - 1 ];
		}
		data_offset += cluster_block_value_size;

		data_run->cluster_block_number = last_cluster_block_number + cluster_block_number;
		data_run->start_offset         = (off64_t) ( data_run->cluster_block_number * io_handle->cluster_block_size );
		data_run->range_flags          = 0;
	}
	data_run->size = (size64_t) ( number_of_cluster_blocks * io_handle->cluster_block_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of cluster blocks\t\t\t: %" PRIu64 " (size: %" PRIu64 ")\n",
		 function,
		 number_of_cluster_blocks,
		 data_run->size );

		libcnotify_printf(
		 "%s: cluster block number\t\t\t: %" PRIu64 " (%" PRIi64 ") (offset: 0x%08" PRIx64 ")\n",
		 function,
		 data_run->cluster_block_number,
		 cluster_block_number,
		 data_run->start_offset );

		if( ( data_run->range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
		{
			libcnotify_printf(
			 "\tIs sparse\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( (ssize_t) data_offset );
}

