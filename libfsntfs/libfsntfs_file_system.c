/*
 * File system functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_vector.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_file_system.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft.h"
#include "libfsntfs_mft_attribute_list.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_name.h"
#include "libfsntfs_path_hint.h"
#include "libfsntfs_security_descriptor_index.h"
#include "libfsntfs_security_descriptor_values.h"

/* Creates a file system
 * Make sure the value file_system is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_system_initialize(
     libfsntfs_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_system_initialize";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system value already set.",
		 function );

		return( -1 );
	}
	*file_system = memory_allocate_structure(
	                libfsntfs_file_system_t );

	if( *file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system,
	     0,
	     sizeof( libfsntfs_file_system_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system.",
		 function );

		memory_free(
		 *file_system );

		*file_system = NULL;

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( ( *file_system )->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( *file_system != NULL )
	{
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( -1 );
}

/* Frees a file system
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_system_free(
     libfsntfs_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_system_free";
	int result            = 1;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( ( *file_system )->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( ( *file_system )->path_hints_tree != NULL )
		{
			if( libcdata_btree_free(
			     &( ( *file_system )->path_hints_tree ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_path_hint_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free path hints tree.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->security_descriptor_index != NULL )
		{
			if( libfsntfs_security_descriptor_index_free(
			     &( ( *file_system )->security_descriptor_index ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free security descriptor index.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->mft != NULL )
		{
			if( libfsntfs_mft_free(
			     &( ( *file_system )->mft ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MFT.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
}

/* Reads the MFT
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_system_read_mft(
     libfsntfs_file_system_t *file_system,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t mft_offset,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfsntfs_data_run_t *data_run                 = NULL;
	libfsntfs_mft_attribute_t *data_attribute      = NULL;
	libfsntfs_mft_attribute_t *last_data_attribute = NULL;
	libfsntfs_mft_entry_t *mft_entry               = NULL;
	static char *function                          = "libfsntfs_file_system_read_mft";
	size64_t mft_size                              = 0;
	uint64_t file_reference                        = 0;
	uint64_t number_of_mft_entries                 = 0;
	uint16_t attribute_data_flags                  = 0;
	int attribute_index                            = 0;
	int data_run_index                             = 0;
	int file_reference_index                       = 0;
	int number_of_data_runs                        = 0;
	int number_of_file_entries                     = 0;
	int segment_index                              = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->mft != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - MFT value already set.",
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
	if( mft_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( mft_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid MFT size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_entry_initialize(
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT entry.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_entry_read_file_io_handle(
	     mft_entry,
	     file_io_handle,
	     mft_offset,
	     io_handle->mft_entry_size,
	     LIBFSNTFS_MFT_ENTRY_INDEX_MFT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: %d.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_MFT );

		goto on_error;
	}
	if( mft_entry->is_empty == 0 )
	{
		if( libfsntfs_mft_entry_read_attributes_data(
		     mft_entry,
		     io_handle,
		     mft_entry->data,
		     mft_entry->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attributes of MFT entry: %d.",
			 function,
			 LIBFSNTFS_MFT_ENTRY_INDEX_MFT );

			goto on_error;
		}
	}
	if( libfsntfs_mft_attribute_get_data_flags(
	     mft_entry->data_attribute,
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
	if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) != 0 )
	{
		if( libbfio_handle_get_size(
		     file_io_handle,
		     &mft_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve $MFT metadata file size.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfsntfs_mft_attribute_get_data_size(
		     mft_entry->data_attribute,
		     &mft_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size of MFT entry: %d.",
			 function,
			 LIBFSNTFS_MFT_ENTRY_INDEX_MFT );

			goto on_error;
		}
	}
	if( libfsntfs_mft_initialize(
	     &( file_system->mft ),
	     io_handle,
	     (size64_t) io_handle->mft_entry_size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT.",
		 function );

		goto on_error;
	}
	if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) != 0 )
	{
		if( libfdata_vector_append_segment(
		     file_system->mft->mft_entry_vector,
		     &segment_index,
		     0,
		     0,
		     mft_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append $MFT metadata file range to MFT entry vector.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfsntfs_mft_attribute_get_number_of_data_runs(
		     mft_entry->data_attribute,
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
			     mft_entry->data_attribute,
			     data_run_index,
			     &data_run,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to data run: %d.",
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
				 data_run_index );

				goto on_error;
			}
			if( libfdata_vector_append_segment(
			     file_system->mft->mft_entry_vector,
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
				 "%s: unable to append data run: %d to MFT entry vector.",
				 function,
				 data_run_index );

				goto on_error;
			}
		}
		last_data_attribute = mft_entry->data_attribute;
	}
	if( ( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
	 && ( mft_entry->list_attribute != NULL ) )
	{
		if( libfsntfs_mft_entry_read_attribute_list(
		     mft_entry,
		     io_handle,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute list.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_attribute_list_get_number_of_file_references(
		     mft_entry->attribute_list,
		     &number_of_file_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of attribute list data MFT entries.",
			 function );

			goto on_error;
		}
		for( file_reference_index = 0;
		     file_reference_index < number_of_file_entries;
		     file_reference_index++ )
		{
			if( libfsntfs_mft_attribute_list_get_file_reference_by_index(
			     mft_entry->attribute_list,
			     file_reference_index,
			     &file_reference,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute list data MFT entry: %d.",
				 function,
				 file_reference_index );

				goto on_error;
			}
			if( libfsntfs_mft_entry_read_attribute_list_data_mft_entry_by_index(
			     mft_entry,
			     io_handle,
			     file_io_handle,
			     file_system->mft->mft_entry_vector,
			     file_system->mft->mft_entry_cache,
			     file_reference,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read attribute list data MFT entry: %" PRIu64 "-%" PRIu64 ".",
				 function,
				 file_reference & 0xffffffffffffUL,
				 file_reference >> 48 );

				goto on_error;
			}
			if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
			{
				if( libfsntfs_mft_attribute_get_next_attribute(
				     last_data_attribute,
				     &data_attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve next data attribute: %d.",
					 function,
					 attribute_index );

					goto on_error;
				}
				if( data_attribute != NULL )
				{
					attribute_index++;

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

						goto on_error;
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
							 data_run_index );

							goto on_error;
						}
						if( libfdata_vector_append_segment(
						     file_system->mft->mft_entry_vector,
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
							 "%s: unable to append data run: %d to MFT entry vector.",
							 function,
							 data_run_index );

							goto on_error;
						}
					}
					last_data_attribute = data_attribute;
				}
			}
		}
	}
	number_of_mft_entries = (uint64_t) ( mft_size / io_handle->mft_entry_size );

	if( number_of_mft_entries > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of MFT entries value out of bounds.",
		 function );

		goto on_error;
	}
	file_system->mft->number_of_mft_entries = number_of_mft_entries;

	if( libfsntfs_mft_entry_free(
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MFT entry.",
		 function );

		goto on_error;
	}
	if( libcdata_btree_initialize(
	     &( file_system->path_hints_tree ),
	     LIBFSNTFS_INDEX_TREE_MAXIMUM_NUMBER_OF_SUB_NODES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create path hints B-tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system->path_hints_tree != NULL )
	{
		libcdata_btree_free(
		 &( file_system->path_hints_tree ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_path_hint_free,
		 NULL );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( -1 );
}

/* Reads the bitmap file entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_system_read_bitmap(
     libfsntfs_file_system_t *file_system,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfcache_cache_t *cluster_block_cache   = NULL;
	libfdata_vector_t *cluster_block_vector  = NULL;
	libfsntfs_cluster_block_t *cluster_block = NULL;
	libfsntfs_mft_entry_t *mft_entry         = NULL;
	static char *function                    = "libfsntfs_file_system_read_bitmap";
	size_t cluster_block_data_offset         = 0;
	off64_t bitmap_offset                    = 0;
	off64_t start_offset                     = 0;
	uint32_t value_32bit                     = 0;
	uint8_t bit_index                        = 0;
	int cluster_block_index                  = 0;
	int number_of_cluster_blocks             = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
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
	if( libfsntfs_mft_get_mft_entry_by_index(
	     file_system->mft,
	     file_io_handle,
	     LIBFSNTFS_MFT_ENTRY_INDEX_BITMAP,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %d.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_BITMAP );

		goto on_error;
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing MFT entry: %d.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_BITMAP );

		goto on_error;
	}
	if( mft_entry->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT entry: %d - missing data attribute.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_BITMAP );

		goto on_error;
	}
	if( libfsntfs_cluster_block_vector_initialize(
	     &cluster_block_vector,
	     io_handle,
	     mft_entry->data_attribute,
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
	if( libfcache_cache_initialize(
	     &cluster_block_cache,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block cache.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_get_number_of_elements(
	     cluster_block_vector,
	     &number_of_cluster_blocks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of cluster blocks.",
		 function );

		goto on_error;
	}
	for( cluster_block_index = 0;
	     cluster_block_index < number_of_cluster_blocks;
	     cluster_block_index++ )
	{
		if( libfdata_vector_get_element_value_by_index(
		     cluster_block_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cluster_block_cache,
		     cluster_block_index,
		     (intptr_t **) &cluster_block,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cluster block: %d from vector.",
			 function,
			 cluster_block_index );

			goto on_error;
		}
		if( cluster_block == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing cluster block: %d.",
			 function,
			 cluster_block_index );

			goto on_error;
		}
		if( cluster_block->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid cluster block: %d - missing data.",
			 function,
			 cluster_block_index );

			goto on_error;
		}
		if( ( ( cluster_block->data_size % 4 ) != 0 )
		 || ( cluster_block->data_size > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: cluster block: %d data size value out of bounds.",
			 function,
			 cluster_block_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: bitmap segment: %d data:\n",
			 function,
			 cluster_block_index );
			libcnotify_print_data(
			 cluster_block->data,
			 cluster_block->data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		cluster_block_data_offset = 0;
		start_offset              = -1;

		while( cluster_block_data_offset < cluster_block->data_size )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( cluster_block->data[ cluster_block_data_offset ] ),
			 value_32bit );

			for( bit_index = 0;
			     bit_index < 32;
			     bit_index++ )
			{
				if( ( value_32bit & 0x00000001UL ) == 0 )
				{
					if( start_offset >= 0 )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: offset range\t\t\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (0x%08" PRIx64 ")\n",
							 function,
							 start_offset,
							 bitmap_offset,
							 bitmap_offset - start_offset );
						}
#endif
/*
						if( libfsntfs_offset_list_append_offset(
						     offset_list,
						     start_offset,
						     bitmap_offset - start_offset,
						     1,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
							 "%s: unable to append offset range to offset list.",
							 function );

							goto on_error;
						}
*/
						start_offset = -1;
					}
				}
				else
				{
					if( start_offset < 0 )
					{
						start_offset = bitmap_offset;
					}
				}
				bitmap_offset += io_handle->cluster_block_size;

				value_32bit >>= 1;
			}
			cluster_block_data_offset += 4;
		}
		if( start_offset >= 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: offset range\t\t\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (0x%08" PRIx64 ")\n",
				 function,
				 start_offset,
				 bitmap_offset,
				 bitmap_offset - start_offset );
			}
#endif
/* TODO
			if( libfsntfs_offset_list_append_offset(
			     offset_list,
			     start_offset,
			     bitmap_offset - start_offset,
			     1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append offset range to offset list.",
				 function );

				goto on_error;
			}
*/
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	if( libfdata_vector_free(
	     &cluster_block_vector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_free(
	     &cluster_block_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( cluster_block_cache != NULL )
	{
		libfcache_cache_free(
		 &cluster_block_cache,
		 NULL );
	}
	if( cluster_block_vector != NULL )
	{
		libfdata_vector_free(
		 &cluster_block_vector,
		 NULL );
	}
	return( -1 );
}

/* Reads the security descriptors
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_system_read_security_descriptors(
     libfsntfs_file_system_t *file_system,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsntfs_file_name_values_t *file_name_values = NULL;
	libfsntfs_mft_attribute_t *data_attribute      = NULL;
	libfsntfs_mft_attribute_t *mft_attribute       = NULL;
	libfsntfs_mft_entry_t *mft_entry               = NULL;
	static char *function                          = "libfsntfs_file_system_read_security_descriptors";
	int result                                     = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->security_descriptor_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - security descriptor index value already set.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     file_system->mft,
	     file_io_handle,
	     LIBFSNTFS_MFT_ENTRY_INDEX_SECURE,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %d.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_SECURE );

		goto on_error;
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing MFT entry: %d.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_SECURE );

		goto on_error;
	}
	if( libfsntfs_mft_entry_get_attribute_by_index(
	     mft_entry,
	     mft_entry->file_name_attribute_index,
	     &mft_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute: %d from MFT entry: %d.",
		 function,
		 mft_entry->file_name_attribute_index,
		 LIBFSNTFS_MFT_ENTRY_INDEX_SECURE );

		goto on_error;
	}
	if( libfsntfs_file_name_values_initialize(
	     &file_name_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file name values.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_values_read_from_mft_attribute(
	     file_name_values,
	     mft_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file name values from attribute: %d from MFT entry: %d.",
		 function,
		 mft_entry->file_name_attribute_index,
		 LIBFSNTFS_MFT_ENTRY_INDEX_SECURE );

		goto on_error;
	}
	result = libfsntfs_name_compare_with_utf8_string(
	          file_name_values->name,
	          file_name_values->name_size,
	          (uint8_t *) "$Secure",
	          7,
	          1,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with data attribute name.",
		 function );

		goto on_error;
	}
	else if( result == LIBUNA_COMPARE_EQUAL )
	{
		if( libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name(
		     mft_entry,
		     (uint8_t *) "$SDS",
		     4,
		     &data_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve $SDS data attribute.",
			 function );

			goto on_error;
		}
		if( libfsntfs_security_descriptor_index_initialize(
		     &( file_system->security_descriptor_index ),
		     io_handle,
		     file_io_handle,
		     data_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create security descriptor index.",
			 function );

			goto on_error;
		}
		if( libfsntfs_security_descriptor_index_read_sii_index(
		     file_system->security_descriptor_index,
		     io_handle,
		     file_io_handle,
		     mft_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read security descriptor identifier ($SII) index.",
			 function );

			goto on_error;
		}
	}
	if( libfsntfs_file_name_values_free(
	     &file_name_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file name values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system->security_descriptor_index != NULL )
	{
		libfsntfs_security_descriptor_index_free(
		 &( file_system->security_descriptor_index ),
		 NULL );
	}
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of MFT entries
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_system_get_number_of_mft_entries(
     libfsntfs_file_system_t *file_system,
     uint64_t *number_of_mft_entries,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_system_get_number_of_mft_entries";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_get_number_of_entries(
	     file_system->mft,
	     number_of_mft_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of MFT entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the MFT entry for a specific index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_system_get_mft_entry_by_index(
     libfsntfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_system_get_mft_entry_by_index";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     file_system->mft,
	     file_io_handle,
	     mft_entry_index,
	     mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIi64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the MFT entry for a specific index
 * This function creates new MFT entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_system_get_mft_entry_by_index_no_cache(
     libfsntfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_system_get_mft_entry_by_index_no_cache";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_get_mft_entry_by_index_no_cache(
	     file_system->mft,
	     file_io_handle,
	     mft_entry_index,
	     mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIi64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the security descriptor for a specific identifier
 * This function creates new security descriptor values
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_system_get_security_descriptor_values_by_identifier(
     libfsntfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t security_descriptor_identifier,
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_file_system_get_security_descriptor_values_by_identifier";
	int result            = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->security_descriptor_index != NULL )
	{
		result = libfsntfs_security_descriptor_index_get_entry_by_identifier(
		          file_system->security_descriptor_index,
		          file_io_handle,
		          security_descriptor_identifier,
		          security_descriptor_values,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve security descriptor from index for identifier: %" PRIu32 ".",
			 function,
			 security_descriptor_identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the path hint of a specific file reference
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_system_get_path_hint(
     libfsntfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t file_reference,
     libfsntfs_path_hint_t **path_hint,
     int recursion_depth,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node               = NULL;
	libfsntfs_file_name_values_t *file_name_values = NULL;
	libfsntfs_mft_attribute_t *mft_attribute       = NULL;
	libfsntfs_mft_entry_t *mft_entry               = NULL;
	libfsntfs_path_hint_t *existing_path_hint      = NULL;
	libfsntfs_path_hint_t *lookup_path_hint        = NULL;
	libfsntfs_path_hint_t *parent_path_hint        = NULL;
	libfsntfs_path_hint_t *safe_path_hint          = NULL;
	uint8_t *parent_path                           = NULL;
	static char *function                          = "libfsntfs_file_system_get_path_hint";
	size_t name_size                               = 0;
	size_t parent_path_size                        = 0;
	uint64_t mft_entry_index                       = 0;
	uint64_t mft_entry_file_reference              = 0;
	uint64_t parent_file_reference                 = 0;
	uint64_t parent_mft_entry_index                = 0;
	uint32_t attribute_type                        = 0;
	uint16_t mft_entry_sequence_number             = 0;
	uint16_t sequence_number                       = 0;
	int attribute_index                            = 0;
	int number_of_attributes                       = 0;
	int result                                     = 0;
	int value_index                                = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path hint.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSNTFS_MAXIMUM_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	mft_entry_index = file_reference & 0xffffffffffffUL;

	if( libfsntfs_file_system_get_mft_entry_by_index(
	     file_system,
	     file_io_handle,
	     mft_entry_index,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		goto on_error;
	}
	if( libfsntfs_mft_entry_get_file_reference(
	     mft_entry,
	     &mft_entry_file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		goto on_error;
	}
	if( mft_entry_file_reference != file_reference )
	{
		result = libfsntfs_mft_entry_is_allocated(
		          mft_entry,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
			 function,
			 mft_entry_index );

			goto on_error;
		}
		else if( result != 0 )
		{
			return( 0 );
		}
		/* The sequence number is updated when the MFT entry is deleted
		 */
		sequence_number           = (uint16_t) ( file_reference >> 48 );
		mft_entry_sequence_number = (uint16_t) ( mft_entry_file_reference >> 48 );

		if( sequence_number != ( mft_entry_sequence_number - 1 ) )
		{
			return( 0 );
		}
	}
	if( libfsntfs_path_hint_initialize(
	     &lookup_path_hint,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create path hint.",
		 function );

		goto on_error;
	}
	lookup_path_hint->file_reference = mft_entry_file_reference;

	result = libcdata_btree_get_value_by_value(
	          file_system->path_hints_tree,
	          (intptr_t *) lookup_path_hint,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_path_hint_compare_by_file_reference,
	          &upper_node,
	          (intptr_t **) path_hint,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve path hint from tree.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libfsntfs_mft_entry_get_number_of_attributes(
		     mft_entry,
		     &number_of_attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of attributes.",
			 function );

			goto on_error;
		}
		for( attribute_index = 0;
		     attribute_index < number_of_attributes;
		     attribute_index++ )
		{
			if( libfsntfs_mft_entry_get_attribute_by_index(
			     mft_entry,
			     attribute_index,
			     &mft_attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d.",
				 function,
				 attribute_index );

				goto on_error;
			}
			if( libfsntfs_mft_attribute_get_type(
			     mft_attribute,
			     &attribute_type,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d type.",
				 function,
				 attribute_index );

				goto on_error;
			}
			if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME )
			{
				continue;
			}
			if( libfsntfs_file_name_values_initialize(
			     &file_name_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file name values.",
				 function );

				goto on_error;
			}
			if( libfsntfs_file_name_values_read_from_mft_attribute(
			     file_name_values,
			     mft_attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read file name values.",
				 function );

				goto on_error;
			}
			if( libfsntfs_file_name_values_get_parent_file_reference(
			     file_name_values,
			     &parent_file_reference,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve parent file reference.",
				 function );

				goto on_error;
			}
			parent_mft_entry_index = parent_file_reference & 0xffffffffffffUL;

			if( file_name_values->name_space != LIBFSNTFS_FILE_NAME_SPACE_DOS )
			{
				parent_path      = NULL;
				parent_path_size = 0;

				if( ( mft_entry_index != LIBFSNTFS_MFT_ENTRY_INDEX_ROOT_DIRECTORY )
				 && ( parent_mft_entry_index == LIBFSNTFS_MFT_ENTRY_INDEX_ROOT_DIRECTORY ) )
				{
					parent_path      = (uint8_t *) "";
					parent_path_size = 1;
				}
				else if( ( parent_mft_entry_index != 0 )
				      && ( parent_mft_entry_index != mft_entry_index ) )
				{
					result = libfsntfs_file_system_get_path_hint(
					          file_system,
					          file_io_handle,
					          parent_file_reference,
					          &parent_path_hint,
					          recursion_depth + 1,
					          error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve path hint for MFT entry: %" PRIu64 ".",
						 function,
						 parent_mft_entry_index );

						goto on_error;
					}
					else if( result == 0 )
					{
						parent_path      = (uint8_t *) "$Orphan";
						parent_path_size = 8;
					}
					else
					{
						parent_path      = parent_path_hint->path;
						parent_path_size = parent_path_hint->path_size;
					}
				}
				if( libfsntfs_file_name_values_get_utf8_name_size(
				     file_name_values,
				     &name_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve size of UTF-8 name.",
					 function );

					goto on_error;
				}
				if( libfsntfs_path_hint_initialize(
				     &safe_path_hint,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create path hint.",
					 function );

					goto on_error;
				}
				safe_path_hint->file_reference = file_reference;
				safe_path_hint->path_size      = parent_path_size + name_size;

				safe_path_hint->path = (uint8_t *) memory_allocate(
								    sizeof( uint8_t ) * safe_path_hint->path_size );

				if( safe_path_hint->path == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create path.",
					 function );

					goto on_error;
				}
				if( ( parent_path != NULL )
				 && ( parent_path_size > 0 ) )
				{
					if( memory_copy(
					     safe_path_hint->path,
					     parent_path,
					     parent_path_size ) == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
						 "%s: unable to copy parent path to path.",
						 function );

						goto on_error;
					}
					safe_path_hint->path[ parent_path_size - 1 ] = '\\';
				}
				if( name_size > 0 )
				{
					if( libfsntfs_file_name_values_get_utf8_name(
					     file_name_values,
					     &( safe_path_hint->path[ parent_path_size ] ),
					     name_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve UTF-8 name.",
						 function );

						goto on_error;
					}
				}
				if( mft_entry_index == LIBFSNTFS_MFT_ENTRY_INDEX_ROOT_DIRECTORY )
				{
					safe_path_hint->path[ 0 ] = '\\';
				}
				result = libcdata_btree_insert_value(
					  file_system->path_hints_tree,
					  &value_index,
					  (intptr_t *) safe_path_hint,
					  (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_path_hint_compare_by_file_reference,
					  &upper_node,
					  (intptr_t **) &existing_path_hint,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to insert path hint into tree.",
					 function );

					goto on_error;
				}
				else if( result == 0 )
				{
					if( libfsntfs_path_hint_free(
					     &safe_path_hint,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free path hint.",
						 function );

						goto on_error;
					}
					*path_hint = existing_path_hint;
				}
				else
				{
					*path_hint = safe_path_hint;

					safe_path_hint = NULL;
				}
				result = 1;
			}
			if( libfsntfs_file_name_values_free(
			     &file_name_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file name values.",
				 function );

				goto on_error;
			}
		}
	}
	if( libfsntfs_path_hint_free(
	     &lookup_path_hint,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free path hint.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( safe_path_hint != NULL )
	{
		libfsntfs_path_hint_free(
		 &safe_path_hint,
		 NULL );
	}
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	if( lookup_path_hint != NULL )
	{
		libfsntfs_path_hint_free(
		 &lookup_path_hint,
		 NULL );
	}
	return( -1 );
}

