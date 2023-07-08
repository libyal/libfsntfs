/*
 * File entry functions
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_data_stream.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_directory_entries_tree.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_extent.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_file_name_attribute.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_path_hint.h"
#include "libfsntfs_reparse_point_attribute.h"
#include "libfsntfs_security_descriptor_values.h"
#include "libfsntfs_standard_information_values.h"
#include "libfsntfs_types.h"

/* Creates a file entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_initialize(
     libfsntfs_file_entry_t **file_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_file_system_t *file_system,
     uint64_t mft_entry_index,
     libfsntfs_directory_entry_t *directory_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *reparse_point_attribute           = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry     = NULL;
	libfsntfs_mft_attribute_t *data_extents_attribute        = NULL;
	libfsntfs_mft_attribute_t *wof_compressed_data_attribute = NULL;
	libfsntfs_mft_entry_t *mft_entry                         = NULL;
	static char *function                                    = "libfsntfs_file_entry_initialize";
	uint64_t base_record_file_reference                      = 0;
	uint32_t compression_method                              = 0;
	int result                                               = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
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
	internal_file_entry = memory_allocate_structure(
	                       libfsntfs_internal_file_entry_t );

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_entry,
	     0,
	     sizeof( libfsntfs_internal_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file entry.",
		 function );

		memory_free(
		 internal_file_entry );

		return( -1 );
	}
	if( libfsntfs_file_system_get_mft_entry_by_index_no_cache(
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
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
	{
		result = libfsntfs_mft_entry_get_base_record_file_reference(
		          mft_entry,
		          &base_record_file_reference,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve base record file reference.",
			 function );

			goto on_error;
		}
		if( ( result == 1 )
		 && ( base_record_file_reference == 0 ) )
		{
			if( mft_entry->has_i30_index != 0 )
			{
				if( libfsntfs_directory_entries_tree_initialize(
				     &( internal_file_entry->directory_entries_tree ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create directory entries tree.",
					 function );

					goto on_error;
				}
				if( libfsntfs_directory_entries_tree_read_from_i30_index(
				     internal_file_entry->directory_entries_tree,
				     io_handle,
				     file_io_handle,
				     mft_entry,
				     flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read MFT entry: %" PRIu32 " directory entries tree.",
					 function,
					 mft_entry->index );

					goto on_error;
				}
			}
			if( mft_entry->data_attribute != NULL )
			{
				if( mft_entry->wof_compressed_data_attribute != NULL )
				{
					result = libfsntfs_internal_file_entry_get_reparse_point_attribute(
						  internal_file_entry,
						  mft_entry,
						  &reparse_point_attribute,
						  error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve reparse point attribute.",
						 function );

						goto on_error;
					}
					else if( result != 0 )
					{
						if( libfsntfs_reparse_point_attribute_get_compression_method(
						     reparse_point_attribute,
						     &compression_method,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve compression method from $REPARSE_POINT attribute.",
							 function );

							goto on_error;
						}
						wof_compressed_data_attribute = mft_entry->wof_compressed_data_attribute;
					}
				}
				if( wof_compressed_data_attribute == NULL )
				{
					data_extents_attribute = mft_entry->data_attribute;
				}
				else
				{
					data_extents_attribute = wof_compressed_data_attribute;
				}
				if( libfsntfs_mft_attribute_get_data_extents_array(
				     data_extents_attribute,
				     io_handle,
				     &( internal_file_entry->extents_array ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create extents array.",
					 function );

					goto on_error;
				}
				if( libfsntfs_cluster_block_stream_initialize(
				     &( internal_file_entry->data_cluster_block_stream ),
				     io_handle,
				     mft_entry->data_attribute,
				     wof_compressed_data_attribute,
				     compression_method,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create data cluster block stream.",
					 function );

					goto on_error;
				}
			}
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_entry->read_write_lock ),
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
	internal_file_entry->io_handle       = io_handle;
	internal_file_entry->file_io_handle  = file_io_handle;
	internal_file_entry->file_system     = file_system;
	internal_file_entry->mft_entry       = mft_entry;
	internal_file_entry->directory_entry = directory_entry;
	internal_file_entry->data_attribute  = mft_entry->data_attribute;
	internal_file_entry->flags           = flags;

	*file_entry = (libfsntfs_file_entry_t *) internal_file_entry;

	return( 1 );

on_error:
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	if( internal_file_entry != NULL )
	{
		if( internal_file_entry->data_cluster_block_stream != NULL )
		{
			libfdata_stream_free(
			 &( internal_file_entry->data_cluster_block_stream ),
			 NULL );
		}
		if( internal_file_entry->directory_entries_tree != NULL )
		{
			libfsntfs_directory_entries_tree_free(
			 &( internal_file_entry->directory_entries_tree ),
			 NULL );
		}
		if( internal_file_entry->extents_array != NULL )
		{
			libcdata_array_free(
			 &( internal_file_entry->extents_array ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_extent_free,
			 NULL );
		}
		memory_free(
		 internal_file_entry );
	}
	return( -1 );
}

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_free(
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_free";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		internal_file_entry = (libfsntfs_internal_file_entry_t *) *file_entry;
		*file_entry         = NULL;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_entry->read_write_lock ),
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
		/* The file_io_handle, io_handle and file_system references are freed elsewhere
		 */
		if( internal_file_entry->data_cluster_block_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( internal_file_entry->data_cluster_block_stream ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data cluster block stream.",
				 function );

				result = -1;
			}
		}
		/* The reparse_point_attribute, security_descriptor_attribute and standard_information_attribute references are managed by the attributes_array
		 */
		if( internal_file_entry->attributes_array != NULL )
		{
			if( libcdata_array_free(
			     &( internal_file_entry->attributes_array ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_internal_attribute_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attributes array.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( internal_file_entry->extents_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_extent_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extents array.",
			 function );

			result = -1;
		}
		if( internal_file_entry->security_descriptor_values != NULL )
		{
			if( libfsntfs_security_descriptor_values_free(
			     &( internal_file_entry->security_descriptor_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free security descriptor values.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->directory_entries_tree != NULL )
		{
			if( libfsntfs_directory_entries_tree_free(
			     &( internal_file_entry->directory_entries_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entries tree.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->directory_entry != NULL )
		{
			if( libfsntfs_directory_entry_free(
			     &( internal_file_entry->directory_entry ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entry.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->mft_entry != NULL )
		{
			if( libfsntfs_mft_entry_free(
			     &( internal_file_entry->mft_entry ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MFT entry.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_file_entry );
	}
	return( result );
}

/* Determines if the file entry is empty
 * Returns 1 if empty, 0 if not or -1 on error
 */
int libfsntfs_file_entry_is_empty(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_is_empty";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_is_empty(
	          internal_file_entry->mft_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MFT entry is empty.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if the file entry is allocated (MFT entry in use flag is set)
 * Returns 1 if allocated, 0 if not or -1 on error
 */
int libfsntfs_file_entry_is_allocated(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_is_allocated";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_is_allocated(
	          internal_file_entry->mft_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MFT entry is allocated.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if the file entry is corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libfsntfs_file_entry_is_corrupted(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_is_corrupted";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_is_corrupted(
	          internal_file_entry->mft_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MFT entry is corrupted.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_file_entry_get_attribute_by_index(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *safe_attribute    = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	static char *function                    = "libfsntfs_internal_file_entry_get_attribute_by_index";
	uint32_t attribute_type                  = 0;
	int number_of_attributes                 = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
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
	if( internal_file_entry->attributes_array == NULL )
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
		if( libcdata_array_initialize(
		     &( internal_file_entry->attributes_array ),
		     number_of_attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attributes array.",
			 function );

			goto on_error;
		}
	}
	if( libcdata_array_get_entry_by_index(
	     internal_file_entry->attributes_array,
	     attribute_index,
	     (intptr_t **) attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute: %d from array.",
		 function,
		 attribute_index );

		goto on_error;
	}
	if( *attribute == NULL )
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
			 "%s: unable to retrieve attribute: %d from MFT entry.",
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
		if( libfsntfs_attribute_initialize(
		     &safe_attribute,
		     mft_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST )
		{
			( (libfsntfs_internal_attribute_t *) safe_attribute )->value = (intptr_t *) mft_entry->attribute_list;
		}
		else
		{
			if( libfsntfs_internal_attribute_read_value(
			     (libfsntfs_internal_attribute_t *) safe_attribute,
			     internal_file_entry->io_handle,
			     internal_file_entry->file_io_handle,
			     internal_file_entry->flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read value of attribute: %d.",
				 function,
				 attribute_index );

				goto on_error;
			}
		}
		if( libcdata_array_set_entry_by_index(
		     internal_file_entry->attributes_array,
		     attribute_index,
		     (intptr_t *) safe_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set atribute: %d in array.",
			 function,
			 attribute_index );

			goto on_error;
		}
		*attribute = safe_attribute;
	}
	return( 1 );

on_error:
	if( safe_attribute != NULL )
	{
		libfsntfs_internal_attribute_free(
		 (libfsntfs_internal_attribute_t **) &safe_attribute,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_internal_file_entry_get_reparse_point_attribute(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_file_entry_get_reparse_point_attribute";

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing MFT entry.",
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
	if( mft_entry->reparse_point_attribute_index == -1 )
	{
		return( 0 );
	}
	if( internal_file_entry->reparse_point_attribute == NULL )
	{
		if( libfsntfs_internal_file_entry_get_attribute_by_index(
		     internal_file_entry,
		     mft_entry,
		     mft_entry->reparse_point_attribute_index,
		     &( internal_file_entry->reparse_point_attribute ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d.",
			 function,
			 mft_entry->reparse_point_attribute_index );

			return( -1 );
		}
	}
	*attribute = internal_file_entry->reparse_point_attribute;

	return( 1 );
}

/* Retrieves the $SECURITY_DESCRIPTOR attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_internal_file_entry_get_security_descriptor_attribute(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_file_entry_get_security_descriptor_attribute";

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing MFT entry.",
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
	if( internal_file_entry->mft_entry->security_descriptor_attribute_index == -1 )
	{
		return( 0 );
	}
	if( internal_file_entry->security_descriptor_attribute == NULL )
	{
		if( libfsntfs_internal_file_entry_get_attribute_by_index(
		     internal_file_entry,
		     internal_file_entry->mft_entry,
		     internal_file_entry->mft_entry->security_descriptor_attribute_index,
		     &( internal_file_entry->security_descriptor_attribute ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d.",
			 function,
			 internal_file_entry->mft_entry->security_descriptor_attribute_index );

			return( -1 );
		}
	}
	*attribute = internal_file_entry->security_descriptor_attribute;

	return( 1 );
}

/* Retrieves the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_internal_file_entry_get_standard_information_attribute(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_file_entry_get_standard_information_attribute";

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing MFT entry.",
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
	if( internal_file_entry->mft_entry->standard_information_attribute_index == -1 )
	{
		return( 0 );
	}
	if( internal_file_entry->standard_information_attribute == NULL )
	{
		if( libfsntfs_internal_file_entry_get_attribute_by_index(
		     internal_file_entry,
		     internal_file_entry->mft_entry,
		     internal_file_entry->mft_entry->standard_information_attribute_index,
		     &( internal_file_entry->standard_information_attribute ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d.",
			 function,
			 internal_file_entry->mft_entry->standard_information_attribute_index );

			return( -1 );
		}
	}
	*attribute = internal_file_entry->standard_information_attribute;

	return( 1 );
}

/* Retrieves the file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_file_reference(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_file_reference";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file reference.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		if( libfsntfs_directory_entry_get_file_reference(
		     internal_file_entry->directory_entry,
		     file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve reference from directory entry.",
			 function );

			result = -1;
		}
	}
	else
	{
		if( libfsntfs_mft_entry_get_file_reference(
		     internal_file_entry->mft_entry,
		     file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file reference from MFT entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the base record file reference
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_base_record_file_reference(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_base_record_file_reference";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_get_base_record_file_reference(
	          internal_file_entry->mft_entry,
	          file_reference,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve base record file reference from MFT entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the parent file reference
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_parent_file_reference(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *parent_file_reference,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_parent_file_reference";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_directory_entry_get_parent_file_reference(
	     internal_file_entry->directory_entry,
	     parent_file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent reference from directory entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the parent file reference for a specific $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_parent_file_reference_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint64_t *parent_file_reference,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_parent_file_reference_by_attribute_index";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_attribute_by_index(
	     internal_file_entry,
	     internal_file_entry->mft_entry,
	     attribute_index,
	     &attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute: %d.",
		 function,
		 attribute_index );

		result = -1;
	}
	else if( libfsntfs_file_name_attribute_get_parent_file_reference(
	          attribute,
	          parent_file_reference,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent reference from $FILE_NAME attribute.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the journal sequence number
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_journal_sequence_number(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *journal_sequence_number,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_journal_sequence_number";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_mft_entry_get_journal_sequence_number(
	     internal_file_entry->mft_entry,
	     journal_sequence_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve journal sequence number from MFT entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the creation date and time
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_creation_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *standard_information_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_creation_time";
	int result                                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_standard_information_attribute(
	          internal_file_entry,
	          &standard_information_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_internal_attribute_get_value(
		     (libfsntfs_internal_attribute_t *) standard_information_attribute,
		     (intptr_t **) &standard_information_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from standard information attribute.",
			 function );

			result = -1;
		}
		else if( libfsntfs_standard_information_values_get_creation_time(
		          standard_information_values,
		          filetime,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve creation time from standard information attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the (file) modification (last written) date and time
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *standard_information_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_modification_time";
	int result                                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_standard_information_attribute(
	          internal_file_entry,
	          &standard_information_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_internal_attribute_get_value(
		     (libfsntfs_internal_attribute_t *) standard_information_attribute,
		     (intptr_t **) &standard_information_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve standard information attribute value.",
			 function );

			result = -1;
		}
		else if( libfsntfs_standard_information_values_get_modification_time(
		          standard_information_values,
		          filetime,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve modification time from standard information attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the access date and time
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_access_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *standard_information_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_access_time";
	int result                                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_standard_information_attribute(
	          internal_file_entry,
	          &standard_information_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_internal_attribute_get_value(
		     (libfsntfs_internal_attribute_t *) standard_information_attribute,
		     (intptr_t **) &standard_information_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve standard information attribute value.",
			 function );

			result = -1;
		}
		else if( libfsntfs_standard_information_values_get_access_time(
		          standard_information_values,
		          filetime,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve access time from standard information attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the (file system entry) modification date and time
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_entry_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *standard_information_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_entry_modification_time";
	int result                                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_standard_information_attribute(
	          internal_file_entry,
	          &standard_information_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_internal_attribute_get_value(
		     (libfsntfs_internal_attribute_t *) standard_information_attribute,
		     (intptr_t **) &standard_information_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve standard information attribute value.",
			 function );

			result = -1;
		}
		else if( libfsntfs_standard_information_values_get_entry_modification_time(
		          standard_information_values,
		          filetime,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry modification time from standard information attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the file attribute flags
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_file_attribute_flags(
     libfsntfs_file_entry_t *file_entry,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *standard_information_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_file_attribute_flags";
	int result                                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_standard_information_attribute(
	          internal_file_entry,
	          &standard_information_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_internal_attribute_get_value(
		     (libfsntfs_internal_attribute_t *) standard_information_attribute,
		     (intptr_t **) &standard_information_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve standard information attribute value.",
			 function );

			result = -1;
		}
		else if( libfsntfs_standard_information_values_get_file_attribute_flags(
		          standard_information_values,
		          file_attribute_flags,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file attribute flags from standard information attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_name_size";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsntfs_directory_entry_get_utf8_name_size(
		          internal_file_entry->directory_entry,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-8 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The size should include the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_name";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsntfs_directory_entry_get_utf8_name(
		          internal_file_entry->directory_entry,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_name_size";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsntfs_directory_entry_get_utf16_name_size(
		          internal_file_entry->directory_entry,
		          utf16_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-16 name from directory.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The size should include the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_name";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsntfs_directory_entry_get_utf16_name(
		          internal_file_entry->directory_entry,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the $FILE_NAME attribute index
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_name_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int *attribute_index,
     libcerror_error_t **error )
{
	uint8_t lookup_name_root[ 2 ]                        = { '.', 0 };

	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	uint8_t *lookup_name                                 = NULL;
	static char *function                                = "libfsntfs_file_entry_get_name_attribute_index";
	size_t lookup_name_size                              = 0;
	uint32_t attribute_type                              = 0;
	uint8_t lookup_name_space                            = 0;
	int number_of_attributes                             = 0;
	int safe_attribute_index                             = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( attribute_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute index",
		 function );

		return( -1 );
	}
/* TODO add thread lock suport */

	if( internal_file_entry->directory_entry == NULL )
	{
		lookup_name       = lookup_name_root;
		lookup_name_size  = 2;
		lookup_name_space = LIBFSNTFS_FILE_NAME_SPACE_DOS_WINDOWS;
	}
	else
	{
		if( internal_file_entry->directory_entry->file_name_values == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid file entry - invalid directory entry - missing file name values.",
			 function );

			return( -1 );
		}
		lookup_name       = internal_file_entry->directory_entry->file_name_values->name;
		lookup_name_size  = internal_file_entry->directory_entry->file_name_values->name_size;
		lookup_name_space = internal_file_entry->directory_entry->file_name_values->name_space;
	}
	if( libfsntfs_mft_entry_get_number_of_attributes(
	     internal_file_entry->mft_entry,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes.",
		 function );

		return( -1 );
	}
	for( safe_attribute_index = 0;
	     safe_attribute_index < number_of_attributes;
	     safe_attribute_index++ )
	{
		if( libfsntfs_internal_file_entry_get_attribute_by_index(
		     internal_file_entry,
		     internal_file_entry->mft_entry,
		     safe_attribute_index,
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d.",
			 function,
			 safe_attribute_index );

			return( -1 );
		}
		if( libfsntfs_attribute_get_type(
		     attribute,
		     &attribute_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d type.",
			 function,
			 safe_attribute_index );

			return( -1 );
		}
		if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME )
		{
			continue;
		}
		if( libfsntfs_internal_attribute_get_value(
		     (libfsntfs_internal_attribute_t *) attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d value.",
			 function,
			 safe_attribute_index );

			return( -1 );
		}
		if( ( lookup_name_space == file_name_values->name_space )
		 && ( lookup_name_size == file_name_values->name_size )
		 && ( memory_compare(
		       lookup_name,
		       file_name_values->name,
		       file_name_values->name_size ) == 0 ) )
		{
			*attribute_index = safe_attribute_index;

			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves the size of the UTF-8 encoded name for a specific $FILE_NAME attribute
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name_size_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_name_size_by_attribute_index";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_attribute_by_index(
	     internal_file_entry,
	     internal_file_entry->mft_entry,
	     attribute_index,
	     &attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute: %d.",
		 function,
		 attribute_index );

		result = -1;
	}
	else if( libfsntfs_file_name_attribute_get_utf8_name_size(
	          attribute,
	          utf8_string_size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name from $FILE_NAME attribute.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name for a specific $FILE_NAME attribute
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_name_by_attribute_index";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_attribute_by_index(
	     internal_file_entry,
	     internal_file_entry->mft_entry,
	     attribute_index,
	     &attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute: %d.",
		 function,
		 attribute_index );

		result = -1;
	}
	else if( libfsntfs_file_name_attribute_get_utf8_name(
	          attribute,
	          utf8_string,
	          utf8_string_size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from $FILE_NAME attribute.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name for a specific $FILE_NAME attribute
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name_size_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_name_size_by_attribute_index";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_attribute_by_index(
	     internal_file_entry,
	     internal_file_entry->mft_entry,
	     attribute_index,
	     &attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute: %d.",
		 function,
		 attribute_index );

		result = -1;
	}
	else if( libfsntfs_file_name_attribute_get_utf16_name_size(
	          attribute,
	          utf16_string_size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name from $FILE_NAME attribute.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name for a specific $FILE_NAME attribute
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_name_by_attribute_index";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_attribute_by_index(
	     internal_file_entry,
	     internal_file_entry->mft_entry,
	     attribute_index,
	     &attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute: %d.",
		 function,
		 attribute_index );

		result = -1;
	}
	else if( libfsntfs_file_name_attribute_get_utf16_name(
	          attribute,
	          utf16_string,
	          utf16_string_size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from $FILE_NAME attribute.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the path hint for a specific $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_file_entry_get_path_hint(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     int attribute_index,
     libfsntfs_path_hint_t **path_hint,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute               = NULL;
	libfsntfs_file_name_values_t *file_name_values = NULL;
	libfsntfs_path_hint_t *parent_path_hint        = NULL;
	libfsntfs_path_hint_t *safe_path_hint          = NULL;
	uint8_t *parent_path                           = NULL;
	static char *function                          = "libfsntfs_internal_file_entry_get_path_hint";
	size_t name_size                               = 0;
	size_t parent_path_size                        = 0;
	uint64_t file_reference                        = 0;
	uint64_t mft_entry_index                       = 0;
	uint64_t parent_file_reference                 = 0;
	uint64_t parent_mft_entry_index                = 0;
	int result                                     = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
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
/* TODO cache path hint in attribute */
	if( libfsntfs_internal_file_entry_get_attribute_by_index(
	     internal_file_entry,
	     internal_file_entry->mft_entry,
	     attribute_index,
	     &attribute,
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
	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing attribute?: %d.",
		 function,
		 attribute_index );

		goto on_error;
	}
	if( ( (libfsntfs_internal_attribute_t *) attribute )->path_hint == NULL )
	{
		if( libfsntfs_mft_entry_get_file_reference(
		     internal_file_entry->mft_entry,
		     &file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file reference from MFT entry.",
			 function );

			goto on_error;
		}
		mft_entry_index = file_reference & 0xffffffffffffUL;

		if( libfsntfs_file_name_attribute_get_parent_file_reference(
		     attribute,
		     &parent_file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve parent reference from $FILE_NAME attribute.",
			 function );

			result = -1;
		}
		parent_mft_entry_index = parent_file_reference & 0xffffffffffffUL;

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
			          internal_file_entry->file_system,
			          internal_file_entry->file_io_handle,
			          parent_file_reference,
			          &parent_path_hint,
			          0,
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
		if( libfsntfs_file_name_attribute_get_utf8_name_size(
		     attribute,
		     &name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-8 name from $FILE_NAME attribute.",
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
			if( libfsntfs_file_name_attribute_get_utf8_name(
			     attribute,
			     &( safe_path_hint->path[ parent_path_size ] ),
			     name_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve UTF-8 name from $FILE_NAME attribute.",
				 function );

				goto on_error;
			}
		}
		if( mft_entry_index == LIBFSNTFS_MFT_ENTRY_INDEX_ROOT_DIRECTORY )
		{
			safe_path_hint->path[ 0 ] = '\\';
		}
		( (libfsntfs_internal_attribute_t *) attribute )->path_hint = safe_path_hint;
	}
	*path_hint = ( (libfsntfs_internal_attribute_t *) attribute )->path_hint;

	return( 1 );

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
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded path hint for a specific $FILE_NAME attribute
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_path_hint_size(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_path_hint_t *path_hint                     = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_path_hint_size";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_path_hint(
	     internal_file_entry,
	     attribute_index,
	     &path_hint,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve path hint.",
		 function );

		result = -1;
	}
	else
	{
		if( libfsntfs_path_hint_get_utf8_path_size(
		     path_hint,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-8 path hint.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded path hint
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_path_hint(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_path_hint_t *path_hint                     = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_path_hint";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_path_hint(
	     internal_file_entry,
	     attribute_index,
	     &path_hint,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve path hint.",
		 function );

		result = -1;
	}
	else
	{
		if( libfsntfs_path_hint_get_utf8_path(
		     path_hint,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 path hint.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded path hint for a specific $FILE_NAME attribute
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_path_hint_size(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_path_hint_t *path_hint                     = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_path_hint_size";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_path_hint(
	     internal_file_entry,
	     attribute_index,
	     &path_hint,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve path hint.",
		 function );

		result = -1;
	}
	else
	{
		if( libfsntfs_path_hint_get_utf16_path_size(
		     path_hint,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-16 path hint.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded path hint
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_path_hint(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_path_hint_t *path_hint                     = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_path_hint";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_path_hint(
	     internal_file_entry,
	     attribute_index,
	     &path_hint,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve path hint.",
		 function );

		result = -1;
	}
	else
	{
		if( libfsntfs_path_hint_get_utf16_path(
		     path_hint,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 path hint.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded symbolic link target
 * The returned size includes the end of string character
 * This value is retrieved from a symbolic link $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_symbolic_link_target_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *reparse_point_attribute       = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_symbolic_link_target_size";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_reparse_point_attribute(
	          internal_file_entry,
	          internal_file_entry->mft_entry,
	          &reparse_point_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve reparse point attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
		          reparse_point_attribute,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-8 substitute name from reparse point attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * This value is retrieved from a symbolic link $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_symbolic_link_target(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *reparse_point_attribute       = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_symbolic_link_target";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_reparse_point_attribute(
	          internal_file_entry,
	          internal_file_entry->mft_entry,
	          &reparse_point_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve reparse point attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
		          reparse_point_attribute,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 substitute name from reparse point attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded symbolic link target
 * The returned size includes the end of string character
 * This value is retrieved from a symbolic link $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_symbolic_link_target_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *reparse_point_attribute       = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_symbolic_link_target_size";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_reparse_point_attribute(
	          internal_file_entry,
	          internal_file_entry->mft_entry,
	          &reparse_point_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve reparse point attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size(
		          reparse_point_attribute,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-16 substitute name from reparse point attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * This value is retrieved from a symbolic link $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_symbolic_link_target(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *reparse_point_attribute       = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_symbolic_link_target";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_reparse_point_attribute(
	          internal_file_entry,
	          internal_file_entry->mft_entry,
	          &reparse_point_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve reparse point attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		result = libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
		          reparse_point_attribute,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 substitute name from reparse point attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the security descriptor (data) size
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_security_descriptor_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *data_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *security_descriptor_attribute                 = NULL;
	libfsntfs_attribute_t *standard_information_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_security_descriptor_size";
	uint32_t security_descriptor_identifier                              = 0;
	int result                                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

/* TODO add thread lock suport */

	result = libfsntfs_internal_file_entry_get_security_descriptor_attribute(
	          internal_file_entry,
	          &security_descriptor_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor attribute.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfsntfs_internal_attribute_get_value(
		     (libfsntfs_internal_attribute_t *) security_descriptor_attribute,
		     (intptr_t **) &security_descriptor_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve security descriptor attribute value.",
			 function );

			goto on_error;
		}
	}
	else
	{
		result = libfsntfs_internal_file_entry_get_standard_information_attribute(
		          internal_file_entry,
		          &standard_information_attribute,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve standard information attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( internal_file_entry->security_descriptor_values == NULL )
		{
			if( libfsntfs_internal_attribute_get_value(
			     (libfsntfs_internal_attribute_t *) standard_information_attribute,
			     (intptr_t **) &standard_information_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve standard information attribute value.",
				 function );

				goto on_error;
			}
			if( libfsntfs_standard_information_values_get_security_descriptor_identifier(
			     standard_information_values,
			     &security_descriptor_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve security descriptor identifier from standard information attribute.",
				 function );

				goto on_error;
			}
			result = libfsntfs_file_system_get_security_descriptor_values_by_identifier(
			          internal_file_entry->file_system,
			          internal_file_entry->file_io_handle,
			          security_descriptor_identifier,
			          &( internal_file_entry->security_descriptor_values ),
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

				goto on_error;
			}
			else if( result == 0 )
			{
				return( 0 );
			}
			/* file_entry takes over management of security_descriptor_values
			 */
		}
		security_descriptor_values = internal_file_entry->security_descriptor_values;
	}
	result = libfsntfs_security_descriptor_values_get_data_size(
	          security_descriptor_values,
	          data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor data size.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( internal_file_entry->security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &( internal_file_entry->security_descriptor_values ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the security descriptor (data)
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_security_descriptor(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *security_descriptor_attribute                 = NULL;
	libfsntfs_attribute_t *standard_information_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_security_descriptor";
	uint32_t security_descriptor_identifier                              = 0;
	int result                                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

/* TODO add thread lock suport */

	result = libfsntfs_internal_file_entry_get_security_descriptor_attribute(
	          internal_file_entry,
	          &security_descriptor_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor attribute.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfsntfs_internal_attribute_get_value(
		     (libfsntfs_internal_attribute_t *) security_descriptor_attribute,
		     (intptr_t **) &security_descriptor_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve security descriptor attribute value.",
			 function );

			goto on_error;
		}
	}
	else
	{
		result = libfsntfs_internal_file_entry_get_standard_information_attribute(
		          internal_file_entry,
		          &standard_information_attribute,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve standard information attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( internal_file_entry->security_descriptor_values == NULL )
		{
			if( libfsntfs_internal_attribute_get_value(
			     (libfsntfs_internal_attribute_t *) standard_information_attribute,
			     (intptr_t **) &standard_information_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve standard information attribute value.",
				 function );

				goto on_error;
			}
			if( libfsntfs_standard_information_values_get_security_descriptor_identifier(
			     standard_information_values,
			     &security_descriptor_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve security descriptor identifier from standard information attribute.",
				 function );

				goto on_error;
			}
			result = libfsntfs_file_system_get_security_descriptor_values_by_identifier(
			          internal_file_entry->file_system,
			          internal_file_entry->file_io_handle,
			          security_descriptor_identifier,
			          &( internal_file_entry->security_descriptor_values ),
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

				goto on_error;
			}
			else if( result == 0 )
			{
				return( 0 );
			}
			/* file_entry takes over management of security_descriptor_values
			 */
		}
		security_descriptor_values = internal_file_entry->security_descriptor_values;
	}
	result = libfsntfs_security_descriptor_values_get_data(
	          security_descriptor_values,
	          data,
	          data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor data.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( internal_file_entry->security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &( internal_file_entry->security_descriptor_values ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of attributes
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_number_of_attributes(
     libfsntfs_file_entry_t *file_entry,
     int *number_of_attributes,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_number_of_attributes";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_mft_entry_get_number_of_attributes(
	     internal_file_entry->mft_entry,
	     number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the attribute for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_attribute_by_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_attribute_by_index";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

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
	if( *attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_file_entry_get_attribute_by_index(
	     internal_file_entry,
	     internal_file_entry->mft_entry,
	     attribute_index,
	     attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute: %d.",
		 function,
		 attribute_index );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if the file entry has the directory entries ($I30) index
 * Returns 1 if the file entry has a directory entries index, 0 if not or -1 on error
 */
int libfsntfs_file_entry_has_directory_entries_index(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_has_directory_entries_index";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_has_directory_entries_index(
	          internal_file_entry->mft_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MFT entry has an directory entries index.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if the file entry has a default data stream (nameless $DATA attribute)
 * Returns 1 if the file entry has a default data stream, 0 if not or -1 on error
 */
int libfsntfs_file_entry_has_default_data_stream(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_has_default_data_stream";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing MFT entry.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->mft_entry->data_attribute != NULL )
	{
		result = 1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if the file entry is a symbolic link
 * Returns 1 if the file entry is a symbolic link, 0 if not or -1 on error
 */
int libfsntfs_file_entry_is_symbolic_link(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *reparse_point_attribute       = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_is_symbolic_link";
	uint32_t reparse_point_tag                           = 0;
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_file_entry_get_reparse_point_attribute(
	          internal_file_entry,
	          internal_file_entry->mft_entry,
	          &reparse_point_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve reparse point attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_reparse_point_attribute_get_tag(
		     reparse_point_attribute,
		     &reparse_point_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve tag from reparse point attribute.",
			 function );

			result = -1;
		}
		if( reparse_point_tag == 0xa000000cUL )
		{
			result = 1;
		}
		else
		{
			result = 0;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of alternate data streams
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_number_of_alternate_data_streams(
     libfsntfs_file_entry_t *file_entry,
     int *number_of_alternate_data_streams,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_number_of_alternate_data_streams";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
	     internal_file_entry->mft_entry,
	     number_of_alternate_data_streams,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of alternate data attributes.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the alternate data stream for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_alternate_data_stream_by_index(
     libfsntfs_file_entry_t *file_entry,
     int alternate_data_stream_index,
     libfsntfs_data_stream_t **alternate_data_stream,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_attribute_t *data_attribute            = NULL;
	static char *function                                = "libfsntfs_file_entry_get_alternate_data_stream_by_index";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	if( *alternate_data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid alternate data stream value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_mft_entry_get_alternate_data_attribute_by_index(
	     internal_file_entry->mft_entry,
	     alternate_data_stream_index,
	     &data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve alternate data attribute: %d.",
		 function,
		 alternate_data_stream_index );

		result = -1;
	}
	if( result == 1 )
	{
		if( libfsntfs_data_stream_initialize(
		     alternate_data_stream,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     data_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create alternate data stream: %d.",
			 function,
			 alternate_data_stream_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if there is an alternate data stream for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
int libfsntfs_file_entry_has_alternate_data_stream_by_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_attribute_t *data_attribute            = NULL;
	static char *function                                = "libfsntfs_file_entry_has_alternate_data_stream_by_utf8_name";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name(
	          internal_file_entry->mft_entry,
	          utf8_string,
	          utf8_string_length,
	          &data_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve alternate data attribute.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if there is an alternate data stream for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
int libfsntfs_file_entry_has_alternate_data_stream_by_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_attribute_t *data_attribute            = NULL;
	static char *function                                = "libfsntfs_file_entry_has_alternate_data_stream_by_utf16_name";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name(
	          internal_file_entry->mft_entry,
	          utf16_string,
	          utf16_string_length,
	          &data_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve alternate data attribute.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the alternate data stream for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_file_entry_get_alternate_data_stream_by_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_data_stream_t **alternate_data_stream,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_attribute_t *data_attribute            = NULL;
	static char *function                                = "libfsntfs_file_entry_get_alternate_data_stream_by_utf8_name";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	if( *alternate_data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid alternate data stream value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name(
	          internal_file_entry->mft_entry,
	          utf8_string,
	          utf8_string_length,
	          &data_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve alternate data attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_data_stream_initialize(
		     alternate_data_stream,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     data_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create alternate data stream.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the alternate data stream for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_file_entry_get_alternate_data_stream_by_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_data_stream_t **alternate_data_stream,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_attribute_t *data_attribute            = NULL;
	static char *function                                = "libfsntfs_file_entry_get_alternate_data_stream_by_utf16_name";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	if( *alternate_data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid alternate data stream value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name(
	          internal_file_entry->mft_entry,
	          utf16_string,
	          utf16_string_length,
	          &data_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve alternate data attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_data_stream_initialize(
		     alternate_data_stream,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     data_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create alternate data stream.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_number_of_sub_file_entries(
     libfsntfs_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_number_of_sub_file_entries";
	int result                                           = 1;
	int safe_number_of_sub_file_entries                  = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( number_of_sub_file_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub file entries.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entries_tree != NULL )
	{
		if( libfsntfs_directory_entries_tree_get_number_of_entries(
		     internal_file_entry->directory_entries_tree,
		     &safe_number_of_sub_file_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from directory entries tree.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( result == 1 )
	{
		*number_of_sub_file_entries = safe_number_of_sub_file_entries;
	}
	return( result );
}

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_sub_file_entry_by_index(
     libfsntfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsntfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsntfs_directory_entry_t *sub_directory_entry     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_sub_file_entry_by_index";
	uint64_t mft_entry_index                             = 0;
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_directory_entries_tree_get_entry_by_index(
	     internal_file_entry->directory_entries_tree,
	     internal_file_entry->file_io_handle,
	     sub_file_entry_index,
	     &sub_directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from directory entries tree.",
		 function,
		 sub_file_entry_index );

		result = -1;
	}
	else
	{
		if( libfsntfs_directory_entry_get_mft_entry_index(
		     sub_directory_entry,
		     &mft_entry_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry index.",
			 function );

			result = -1;
		}
		/* sub_file_entry takes over management of sub_directory_entry
		 */
		else if( libfsntfs_file_entry_initialize(
			  sub_file_entry,
			  internal_file_entry->io_handle,
			  internal_file_entry->file_io_handle,
			  internal_file_entry->file_system,
			  mft_entry_index,
			  sub_directory_entry,
			  internal_file_entry->flags,
			  error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub file entry: %d with MFT entry: %" PRIu64 ".",
			 function,
			 sub_file_entry_index,
			 mft_entry_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		result = -1;
	}
#endif
	if( result == -1 )
	{
		if( sub_directory_entry != NULL )
		{
			libfsntfs_directory_entry_free(
			 &sub_directory_entry,
			 NULL );
		}
	}
	return( result );
}

/* Retrieves the sub file entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsntfs_directory_entry_t *sub_directory_entry     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_sub_file_entry_by_utf8_name";
	uint64_t mft_entry_index                             = 0;
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_directory_entries_tree_get_entry_by_utf8_name(
	          internal_file_entry->directory_entries_tree,
	          internal_file_entry->file_io_handle,
	          utf8_string,
	          utf8_string_length,
	          &sub_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_directory_entry_get_mft_entry_index(
		     sub_directory_entry,
		     &mft_entry_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry index.",
			 function );

			result = -1;
		}
		/* sub_file_entry takes over management of sub_directory_entry
		 */
		else if( libfsntfs_file_entry_initialize(
		          sub_file_entry,
		          internal_file_entry->io_handle,
		          internal_file_entry->file_io_handle,
		          internal_file_entry->file_system,
		          mft_entry_index,
		          sub_directory_entry,
		          internal_file_entry->flags,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub file entry with MFT entry: %" PRIu64 ".",
			 function,
			 mft_entry_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		result = -1;
	}
#endif
	if( result == -1 )
	{
		if( sub_directory_entry != NULL )
		{
			libfsntfs_directory_entry_free(
			 &sub_directory_entry,
			 NULL );
		}
	}
	return( result );
}

/* Retrieves the sub file entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsntfs_directory_entry_t *sub_directory_entry     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_sub_file_entry_by_utf16_name";
	uint64_t mft_entry_index                             = 0;
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_directory_entries_tree_get_entry_by_utf16_name(
	          internal_file_entry->directory_entries_tree,
	          internal_file_entry->file_io_handle,
	          utf16_string,
	          utf16_string_length,
	          &sub_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_directory_entry_get_mft_entry_index(
		     sub_directory_entry,
		     &mft_entry_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry index.",
			 function );

			result = -1;
		}
		/* sub_file_entry takes over management of sub_directory_entry
		 */
		else if( libfsntfs_file_entry_initialize(
		          sub_file_entry,
		          internal_file_entry->io_handle,
		          internal_file_entry->file_io_handle,
		          internal_file_entry->file_system,
		          mft_entry_index,
		          sub_directory_entry,
		          internal_file_entry->flags,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub file entry with MFT entry: %" PRIu64 ".",
			 function,
			 mft_entry_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		result = -1;
	}
#endif
	if( result == -1 )
	{
		if( sub_directory_entry != NULL )
		{
			libfsntfs_directory_entry_free(
			 &sub_directory_entry,
			 NULL );
		}
	}
	return( result );
}

/* Reads data at the current offset from the default data stream (nameless $DATA attribute)
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_file_entry_read_buffer(
         libfsntfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_read_buffer";
	ssize_t read_count                                   = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing data attribute.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libfdata_stream_read_buffer(
	              internal_file_entry->data_cluster_block_stream,
	              (intptr_t *) internal_file_entry->file_io_handle,
	              buffer,
	              buffer_size,
	              0,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from data cluster block stream.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads data at a specific offset from the default data stream (nameless $DATA attribute)
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_file_entry_read_buffer_at_offset(
         libfsntfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_read_buffer_at_offset";
	ssize_t read_count                                   = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing data attribute.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libfdata_stream_read_buffer_at_offset(
	              internal_file_entry->data_cluster_block_stream,
	              (intptr_t *) internal_file_entry->file_io_handle,
	              buffer,
	              buffer_size,
	              offset,
	              0,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from data cluster block stream.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Seeks a certain offset in the default data stream (nameless $DATA attribute)
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsntfs_file_entry_seek_offset(
         libfsntfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_seek_offset";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing data attribute.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	offset = libfdata_stream_seek_offset(
	          internal_file_entry->data_cluster_block_stream,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in data cluster block stream.",
		 function );

		offset = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset of the default data stream (nameless $DATA attribute)
 * Returns the offset if successful or -1 on error
 */
int libfsntfs_file_entry_get_offset(
     libfsntfs_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_offset";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing data attribute.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfdata_stream_get_offset(
	     internal_file_entry->data_cluster_block_stream,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset from data cluster block stream.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the default data stream (nameless $DATA attribute)
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_size(
     libfsntfs_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_size";
	size64_t safe_size                                   = 0;
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->data_cluster_block_stream != NULL )
	{
		if( libfdata_stream_get_size(
		     internal_file_entry->data_cluster_block_stream,
		     &safe_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data attribute data size.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( result == 1 )
	{
		*size = safe_size;
	}
	return( result );
}

/* Retrieves the number of extents (decoded data runs) of the default data stream (nameless $DATA attribute)
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_number_of_extents(
     libfsntfs_file_entry_t *file_entry,
     int *number_of_extents,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_number_of_extents";
	int result                                           = 1;
	int safe_number_of_extents                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

	if( number_of_extents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of extents.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->extents_array != NULL )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_file_entry->extents_array,
		     &safe_number_of_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of extents.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( result == 1 )
	{
		*number_of_extents = safe_number_of_extents;
	}
	return( result );
}

/* Retrieves a specific extent (decoded data run) of the default data stream (nameless $DATA attribute)
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_extent_by_index(
     libfsntfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error )
{
	libfsntfs_extent_t *data_extent                      = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_extent_by_index";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsntfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_array_get_entry_by_index(
	     internal_file_entry->extents_array,
	     extent_index,
	     (intptr_t **) &data_extent,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent: %d.",
		 function,
		 extent_index );

		result = -1;
	}
	if( result == 1 )
	{
		if( libfsntfs_extent_get_values(
		     data_extent,
		     extent_offset,
		     extent_size,
		     extent_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d values.",
			 function,
			 extent_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

