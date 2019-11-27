/*
 * MFT functions
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_data_run.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_types.h"
#include "libfsntfs_volume_information_attribute.h"

/* Creates a MFT
 * Make sure the value mft is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_initialize(
     libfsntfs_mft_t **mft,
     libfsntfs_io_handle_t *io_handle,
     off64_t file_offset,
     size64_t file_size,
     size64_t mft_entry_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function  = "libfsntfs_mft_initialize";
	uint32_t segment_flags = 0;
	int segment_index      = 0;

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( *mft != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT value already set.",
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
	*mft = memory_allocate_structure(
	        libfsntfs_mft_t );

	if( *mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mft,
	     0,
	     sizeof( libfsntfs_mft_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MFT.",
		 function );

		memory_free(
		 *mft );

		*mft = NULL;

		return( -1 );
	}
	if( libfdata_vector_initialize(
	     &( ( *mft )->mft_entry_vector ),
	     mft_entry_size,
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_mft_entry_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT entry vector.",
		 function );

		goto on_error;
	}
	if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) != 0 )
	{
		segment_flags = LIBFSNTFS_MFT_ENTRY_FLAG_MFT_ONLY;
	}
	if( libfdata_vector_append_segment(
	     ( *mft )->mft_entry_vector,
	     &segment_index,
	     0,
	     file_offset,
	     file_size,
	     segment_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to MFT entry vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *mft )->mft_entry_cache ),
	     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_MFT_ENTRIES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT entry cache.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *mft )->single_mft_entry_cache ),
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create signle MFT entry cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *mft != NULL )
	{
		if( ( *mft )->mft_entry_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *mft )->mft_entry_cache ),
			 NULL );
		}
		if( ( *mft )->mft_entry_vector != NULL )
		{
			libfdata_vector_free(
			 &( ( *mft )->mft_entry_vector ),
			 NULL );
		}
		memory_free(
		 *mft );

		*mft = NULL;
	}
	return( -1 );
}

/* Frees a MFT
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_free(
     libfsntfs_mft_t **mft,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_free";
	int result            = 1;

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( *mft != NULL )
	{
		if( libfdata_vector_free(
		     &( ( *mft )->mft_entry_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MFT entry vector.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *mft )->mft_entry_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MFT entry cache.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *mft )->single_mft_entry_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free single MFT entry cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *mft );

		*mft = NULL;
	}
	return( result );
}

/* Sets the MFT data runs
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_set_data_runs(
     libfsntfs_mft_t *mft,
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error )
{
	libfsntfs_data_run_t *data_run            = NULL;
	libfsntfs_mft_attribute_t *data_attribute = NULL;
	static char *function                     = "libfsntfs_mft_set_data_runs";
	size64_t mft_data_size                    = 0;
	size64_t mft_entry_size                   = 0;
	uint16_t attribute_data_flags             = 0;
	int attribute_index                       = 0;
	int data_run_index                        = 0;
	int number_of_data_runs                   = 0;
	int segment_index                         = 0;

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry.",
		 function );

		return( -1 );
	}
	if( mft_entry->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT entry: 0 - missing data attribute.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_data_size(
	     mft_entry->data_attribute,
	     &mft_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry size.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_data_size(
	     mft->mft_entry_vector,
	     &mft_entry_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry size.",
		 function );

		return( -1 );
	}
	if( mft_entry_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry size value out of bounds.",
		 function );

		return( -1 );
	}
	/* We cannot use the vector here since it uses the allocated size of the data runs.
	 */
	mft->number_of_mft_entries = (uint64_t) ( mft_data_size / mft_entry_size );

	if( mft->number_of_mft_entries > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of MFT entries value out of bounds.",
		 function );

		return( -1 );
	}
	data_attribute = mft_entry->data_attribute;

	while( data_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_data_flags(
		     data_attribute,
		     &attribute_data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute data flags.",
			 function );

			return( -1 );
		}
		if( ( attribute_data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compressed attribute data.",
			 function );

			return( -1 );
		}
		if( libfsntfs_mft_attribute_get_number_of_data_runs(
		     data_attribute,
		     &number_of_data_runs,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of data runs.",
			 function );

			return( -1 );
		}
		for( data_run_index = 0;
		     data_run_index < number_of_data_runs;
		     data_run_index++ )
		{
			if( libfsntfs_mft_attribute_get_data_run_by_index(
			     data_attribute,
			     data_run_index,
			     &data_run,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve MFT entry: 0 data run: %d.",
				 function,
				 data_run_index );

				return( -1 );
			}
			if( data_run == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing MFT entry: 0 data run: %d.",
				 function );

				return( -1 );
			}
/* TODO refactor data_attribute == mft_entry->data_attribute
 * To be clear what it is supposed to check, likely the first data attribute
 * use attribute_index ?
 */
			if( ( data_run_index == 0 )
			 && ( data_attribute == mft_entry->data_attribute ) )
			{
				if( libfdata_vector_set_segment_by_index(
				     mft->mft_entry_vector,
				     0,
				     0,
				     data_run->start_offset,
				     data_run->size,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set segment: 0 to MFT entry vector.",
					 function );

					return( -1 );
				}
			}
			else
			{
				if( libfdata_vector_append_segment(
				     mft->mft_entry_vector,
				     &segment_index,
				     0,
				     data_run->start_offset,
				     data_run->size,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append segment: %d to MFT entry vector.",
					 function,
					 data_run_index );

					return( -1 );
				}
			}
		}
		if( libfsntfs_mft_attribute_get_next_attribute(
		     data_attribute,
		     &data_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next MFT entry: 0 data attribute: %d.",
			 function,
			 attribute_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads a specific MFT entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_read_mft_entry(
     libfsntfs_mft_t *mft,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t mft_entry_index,
     libfsntfs_mft_entry_t *mft_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_read_mft_entry";

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
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
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_entry_read_file_io_handle(
	     mft_entry,
	     file_io_handle,
	     file_offset,
	     io_handle->mft_entry_size,
	     mft_entry_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: %" PRIu32 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( mft_entry->is_empty == 0 )
	{
		if( libfsntfs_mft_entry_read_attributes(
		     mft_entry,
		     io_handle,
		     file_io_handle,
		     mft->mft_entry_vector,
		     flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read MFT entry: %" PRIu32 " attributes.",
			 function,
			 mft_entry_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the number of MFT entries
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_get_number_of_entries(
     libfsntfs_mft_t *mft,
     uint64_t *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_get_number_of_entries";

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
	*number_of_entries = mft->number_of_mft_entries;

	return( 1 );
}

/* Retrieves the MFT entry for a specific index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_get_mft_entry_by_index(
     libfsntfs_mft_t *mft,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_get_mft_entry_by_index";

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( mft_entry_index > mft->number_of_mft_entries )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     mft->mft_entry_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) mft->mft_entry_cache,
	     (int) mft_entry_index,
	     (intptr_t **) mft_entry,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the MFT entry for a specific index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_get_mft_entry_by_index_no_cache(
     libfsntfs_mft_t *mft,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_get_mft_entry_by_index_no_cache";

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( mft_entry_index > mft->number_of_mft_entries )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     mft->mft_entry_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) mft->single_mft_entry_cache,
	     (int) mft_entry_index,
	     (intptr_t **) mft_entry,
	     LIBFDATA_READ_FLAG_IGNORE_CACHE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( libfcache_cache_clear_value_by_index(
	     mft->single_mft_entry_cache,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to clear single MFT entry cache entry: 0.",
		 function );

		return( -1 );
	}
	return( 1 );
}

