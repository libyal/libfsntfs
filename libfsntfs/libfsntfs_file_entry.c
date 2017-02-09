/*
 * File entry functions
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_data_stream.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_directory_entries_tree.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_file_name_attribute.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_reparse_point_attribute.h"
#include "libfsntfs_security_descriptor_values.h"
#include "libfsntfs_standard_information_values.h"
#include "libfsntfs_types.h"
#include "libfsntfs_volume.h"

/* Creates a file entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_initialize(
     libfsntfs_file_entry_t **file_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_t *mft,
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_directory_entry_t *directory_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_initialize";

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
	if( libcdata_btree_initialize(
	     &( internal_file_entry->directory_entries_tree ),
	     LIBFSNTFS_INDEX_TREE_MAXIMUM_NUMBER_OF_SUB_NODES,
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
	if( mft_entry->base_record_file_reference == 0 )
	{
		if( mft_entry->i30_index != NULL )
		{
			if( libfsntfs_mft_entry_read_directory_entries_tree(
			     mft_entry,
			     io_handle,
			     file_io_handle,
			     internal_file_entry->directory_entries_tree,
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
			if( libfsntfs_cluster_block_stream_initialize(
			     &( internal_file_entry->data_cluster_block_stream ),
			     io_handle,
			     mft_entry->data_attribute,
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
			if( libfsntfs_attribute_get_data_size(
			     mft_entry->data_attribute,
			     &( internal_file_entry->data_size ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data attribute data size.",
				 function );

				goto on_error;
			}
		}
	}
	internal_file_entry->file_io_handle            = file_io_handle;
	internal_file_entry->io_handle                 = io_handle;
	internal_file_entry->mft                       = mft;
	internal_file_entry->mft_entry                 = mft_entry;
	internal_file_entry->security_descriptor_index = security_descriptor_index;
	internal_file_entry->directory_entry           = directory_entry;
	internal_file_entry->data_attribute            = mft_entry->data_attribute;
	internal_file_entry->flags                     = flags;

	*file_entry = (libfsntfs_file_entry_t *) internal_file_entry;

	return( 1 );

on_error:
	if( internal_file_entry != NULL )
	{
		if( internal_file_entry->directory_entries_tree != NULL )
		{
			libcdata_btree_free(
			 &( internal_file_entry->directory_entries_tree ),
			 NULL,
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

		/* The file_io_handle, io_handle, mft, security_descriptor_index and mft_entry references are freed elsewhere
		 */
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
		if( internal_file_entry->directory_entries_tree != NULL )
		{
			if( libcdata_btree_free(
			     &( internal_file_entry->directory_entries_tree ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entries array.",
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

		return( -1 );
	}
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

		return( -1 );
	}
	return( result );
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

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the base record file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_base_record_file_reference(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_base_record_file_reference";

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

	if( libfsntfs_mft_entry_get_base_record_file_reference(
	     internal_file_entry->mft_entry,
	     file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve base record file reference from MFT entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the parent file reference
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_parent_file_reference(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_parent_file_reference";

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

	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_file_name_values_get_parent_file_reference(
	     internal_file_entry->directory_entry->file_name_values,
	     file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent reference from directory entry file name value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the parent file reference for a specific $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_parent_file_reference_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_parent_file_reference_by_attribute_index";

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

	if( libfsntfs_mft_entry_get_attribute_by_index(
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

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_parent_file_reference(
	     attribute,
	     file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent reference from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
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

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the creation date and time
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_creation_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *creation_time,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_creation_time";

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
	if( internal_file_entry->mft_entry->standard_information_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_attribute_get_value(
	     internal_file_entry->mft_entry->standard_information_attribute,
	     (intptr_t **) &standard_information_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_values_get_creation_time(
	     standard_information_values,
	     creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time from standard information attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *modification_time,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_modification_time";

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
	if( internal_file_entry->mft_entry->standard_information_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_attribute_get_value(
	     internal_file_entry->mft_entry->standard_information_attribute,
	     (intptr_t **) &standard_information_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_values_get_modification_time(
	     standard_information_values,
	     modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time from standard information attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the access date and time
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_access_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *access_time,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_access_time";

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
	if( internal_file_entry->mft_entry->standard_information_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_attribute_get_value(
	     internal_file_entry->mft_entry->standard_information_attribute,
	     (intptr_t **) &standard_information_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_values_get_access_time(
	     standard_information_values,
	     access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time from standard information attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * This value is retrieved from the $STANDARD_INFORMATION attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_entry_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *entry_modification_time,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_entry_modification_time";

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
	if( internal_file_entry->mft_entry->standard_information_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_attribute_get_value(
	     internal_file_entry->mft_entry->standard_information_attribute,
	     (intptr_t **) &standard_information_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_values_get_entry_modification_time(
	     standard_information_values,
	     entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time from standard information attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_file_attribute_flags";

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
	if( internal_file_entry->mft_entry->standard_information_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_attribute_get_value(
	     internal_file_entry->mft_entry->standard_information_attribute,
	     (intptr_t **) &standard_information_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve standard information attribute value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_standard_information_values_get_file_attribute_flags(
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

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_name_size";

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
	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_file_name_values_get_utf8_name_size(
	     internal_file_entry->directory_entry->file_name_values,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name from directory entry file name value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_name";

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
	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_file_name_values_get_utf8_name(
	     internal_file_entry->directory_entry->file_name_values,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from directory entry file name value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_name_size";

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
	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_file_name_values_get_utf16_name_size(
	     internal_file_entry->directory_entry->file_name_values,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name from directory entry file name value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_name";

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
	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_file_name_values_get_utf16_name(
	     internal_file_entry->directory_entry->file_name_values,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from directory entry file name value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the name attribute index
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_name_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int *attribute_index,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_name_attribute_index";
	uint32_t attribute_type                              = 0;
	int number_of_attributes                             = 0;

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
	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
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
	for( *attribute_index = 0;
	     *attribute_index < number_of_attributes;
	     *attribute_index += 1 )
	{
		if( libfsntfs_mft_entry_get_attribute_by_index(
		     internal_file_entry->mft_entry,
		     *attribute_index,
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d.",
			 function,
			 *attribute_index );

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
			 *attribute_index );

			return( -1 );
		}
		if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME )
		{
			continue;
		}
		if( libfsntfs_attribute_get_value(
		     attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d value.",
			 function,
			 *attribute_index );

			return( -1 );
		}
		if( ( internal_file_entry->directory_entry->file_name_values->name_namespace == file_name_values->name_namespace )
		 && ( internal_file_entry->directory_entry->file_name_values->name_size == file_name_values->name_size )
		 && ( memory_compare(
		       internal_file_entry->directory_entry->file_name_values->name,
		       file_name_values->name,
		       file_name_values->name_size ) == 0 ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves the size of the UTF-8 encoded name for a specific $FILE_NAME attribute
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name_size_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_name_size_by_attribute_index";

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

	if( libfsntfs_mft_entry_get_attribute_by_index(
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

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_utf8_name_size(
	     attribute,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name for a specific $FILE_NAME attribute
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_name_by_attribute_index";

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

	if( libfsntfs_mft_entry_get_attribute_by_index(
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

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_utf8_name(
	     attribute,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name for a specific $FILE_NAME attribute
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name_size_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_name_size_by_attribute_index";

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

	if( libfsntfs_mft_entry_get_attribute_by_index(
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

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_utf16_name_size(
	     attribute,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name for a specific $FILE_NAME attribute
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *attribute                     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_name_by_attribute_index";

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

	if( libfsntfs_mft_entry_get_attribute_by_index(
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

		return( -1 );
	}
	if( libfsntfs_file_name_attribute_get_utf16_name(
	     attribute,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded reparse point substitute name
 * The returned size includes the end of string character
 * This value is retrieved from the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_reparse_point_substitute_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_reparse_point_substitute_name_size";

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
	if( internal_file_entry->mft_entry->reparse_point_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
	     internal_file_entry->mft_entry->reparse_point_attribute,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 substitute name from reparse point attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded reparse point substitute name
 * The size should include the end of string character
 * This value is retrieved from the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_reparse_point_substitute_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_reparse_point_substitute_name";

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
	if( internal_file_entry->mft_entry->reparse_point_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
	     internal_file_entry->mft_entry->reparse_point_attribute,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 substitute name from reparse point attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded reparse point substitute name
 * The returned size includes the end of string character
 * This value is retrieved from the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_reparse_point_substitute_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_reparse_point_substitute_name_size";

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
	if( internal_file_entry->mft_entry->reparse_point_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size(
	     internal_file_entry->mft_entry->reparse_point_attribute,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 substitute name from reparse point attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded reparse point substitute name
 * The size should include the end of string character
 * This value is retrieved from the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_reparse_point_substitute_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_reparse_point_substitute_name";

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
	if( internal_file_entry->mft_entry->reparse_point_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
	     internal_file_entry->mft_entry->reparse_point_attribute,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 substitute name from reparse point attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded reparse point print name
 * The returned size includes the end of string character
 * This value is retrieved from the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_reparse_point_print_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_reparse_point_print_name_size";

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
	if( internal_file_entry->mft_entry->reparse_point_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_reparse_point_attribute_get_utf8_print_name_size(
	     internal_file_entry->mft_entry->reparse_point_attribute,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 print name from reparse point attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded reparse point print name
 * The size should include the end of string character
 * This value is retrieved from the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf8_reparse_point_print_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf8_reparse_point_print_name";

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
	if( internal_file_entry->mft_entry->reparse_point_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_reparse_point_attribute_get_utf8_print_name(
	     internal_file_entry->mft_entry->reparse_point_attribute,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 print name from reparse point attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded reparse point print name
 * The returned size includes the end of string character
 * This value is retrieved from the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_reparse_point_print_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_reparse_point_print_name_size";

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
	if( internal_file_entry->mft_entry->reparse_point_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_reparse_point_attribute_get_utf16_print_name_size(
	     internal_file_entry->mft_entry->reparse_point_attribute,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 print name from reparse point attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded reparse point print name
 * The size should include the end of string character
 * This value is retrieved from the $REPARSE_POINT attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_utf16_reparse_point_print_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_utf16_reparse_point_print_name";

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
	if( internal_file_entry->mft_entry->reparse_point_attribute == NULL )
	{
		return( 0 );
	}
	if( libfsntfs_reparse_point_attribute_get_utf16_print_name(
	     internal_file_entry->mft_entry->reparse_point_attribute,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 print name from reparse point attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the security descriptor (data) size
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_file_entry_get_security_descriptor_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *data_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_security_descriptor_size";
	uint32_t security_descriptor_identifier                              = 0;

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
	if( ( internal_file_entry->mft_entry->security_descriptor_attribute == NULL )
	 && ( internal_file_entry->mft_entry->standard_information_attribute == NULL ) )
	{
		return( 0 );
	}
	if( ( internal_file_entry->mft_entry->security_descriptor_attribute != NULL )
	 && ( internal_file_entry->security_descriptor_index == NULL ) )
	{
		return( 0 );
	}
	if( internal_file_entry->mft_entry->security_descriptor_attribute != NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->security_descriptor_attribute,
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
		if( internal_file_entry->security_descriptor_values == NULL )
		{
			if( libfsntfs_attribute_get_value(
			     internal_file_entry->mft_entry->standard_information_attribute,
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
			if( libfsntfs_security_descriptor_index_get_security_descriptor_by_identifier(
			     internal_file_entry->security_descriptor_index,
			     security_descriptor_identifier,
			     &( internal_file_entry->security_descriptor_values ),
			     error ) != 1 )
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
			/* The file entry takes over management of security_descriptor_values
			 */
		}
		security_descriptor_values = internal_file_entry->security_descriptor_values;
	}
	if( libfsntfs_security_descriptor_values_get_data_size(
	     security_descriptor_values,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor data size.",
		 function );

		goto on_error;
	}
	return( 1 );

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
	libfsntfs_internal_file_entry_t *internal_file_entry                 = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_file_entry_get_security_descriptor";
	uint32_t security_descriptor_identifier                              = 0;

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
	if( ( internal_file_entry->mft_entry->security_descriptor_attribute == NULL )
	 && ( internal_file_entry->mft_entry->standard_information_attribute == NULL ) )
	{
		return( 0 );
	}
	if( ( internal_file_entry->mft_entry->security_descriptor_attribute != NULL )
	 && ( internal_file_entry->security_descriptor_index == NULL ) )
	{
		return( 0 );
	}
	if( internal_file_entry->mft_entry->security_descriptor_attribute != NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->security_descriptor_attribute,
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
		if( internal_file_entry->security_descriptor_values == NULL )
		{
			if( libfsntfs_attribute_get_value(
			     internal_file_entry->mft_entry->standard_information_attribute,
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
			if( libfsntfs_security_descriptor_index_get_security_descriptor_by_identifier(
			     internal_file_entry->security_descriptor_index,
			     security_descriptor_identifier,
			     &( internal_file_entry->security_descriptor_values ),
			     error ) != 1 )
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
			/* The file entry takes over management of security_descriptor_values
			 */
		}
		security_descriptor_values = internal_file_entry->security_descriptor_values;
	}
	if( libfsntfs_security_descriptor_values_get_data(
	     security_descriptor_values,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor data.",
		 function );

		goto on_error;
	}
	return( 1 );

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

		return( -1 );
	}
	return( 1 );
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
	if( libfsntfs_mft_entry_get_attribute_by_index(
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

		return( -1 );
	}
	return( 1 );
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

		return( -1 );
	}
	return( result );
}

/* Determines if the file entry has the default data stream (nameless $DATA attribute)
 * Returns 1 if the file entry has a default data stream, 0 if not or -1 on error
 */
int libfsntfs_file_entry_has_default_data_stream(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_has_default_data_stream";

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
	if( internal_file_entry->mft_entry->data_attribute != NULL )
	{
		return( 1 );
	}
	return( 0 );
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

		return( -1 );
	}
	return( 1 );
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
	libfsntfs_attribute_t *data_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_alternate_data_stream_by_index";

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

		return( -1 );
	}
	if( libfsntfs_data_stream_initialize(
	     alternate_data_stream,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->io_handle,
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

		return( -1 );
	}
	return( 1 );
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
	libfsntfs_attribute_t *data_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
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

		return( -1 );
	}
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
	libfsntfs_attribute_t *data_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
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

		return( -1 );
	}
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
	libfsntfs_attribute_t *data_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
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

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libfsntfs_data_stream_initialize(
	     alternate_data_stream,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->io_handle,
	     data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create alternate data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	libfsntfs_attribute_t *data_attribute                = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
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

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libfsntfs_data_stream_initialize(
	     alternate_data_stream,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->io_handle,
	     data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create alternate data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
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

	if( libcdata_btree_get_number_of_values(
	     internal_file_entry->directory_entries_tree,
	     number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of directory entries from tree.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	libfsntfs_directory_entry_t *directory_entry         = NULL;
	libfsntfs_directory_entry_t *sub_directory_entry     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_entry_t *mft_entry                     = NULL;
	static char *function                                = "libfsntfs_file_entry_get_sub_file_entry_by_index";
	uint64_t mft_entry_index                             = 0;

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
	if( libcdata_btree_get_value_by_index(
	     internal_file_entry->directory_entries_tree,
	     sub_file_entry_index,
	     (intptr_t **) &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry: %d from tree.",
		 function,
		 sub_file_entry_index );

		goto on_error;
	}
	if( libfsntfs_directory_entry_get_mft_entry_index(
	     directory_entry,
	     &mft_entry_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry index.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     internal_file_entry->mft,
	     internal_file_entry->file_io_handle,
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
	if( libfsntfs_directory_entry_clone(
	     &sub_directory_entry,
	     directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub directory entry.",
		 function );

		goto on_error;
	}
	/* libfsntfs_file_entry_initialize takes over management of sub_directory_entry
	 */
	if( libfsntfs_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->mft,
	     internal_file_entry->security_descriptor_index,
	     mft_entry,
	     sub_directory_entry,
	     internal_file_entry->flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub file entry: %d.",
		 function,
		 sub_file_entry_index );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sub_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &sub_directory_entry,
		 NULL );
	}
	return( -1 );
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
	libfsntfs_directory_entry_t *directory_entry         = NULL;
	libfsntfs_directory_entry_t *sub_directory_entry     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_entry_t *mft_entry                     = NULL;
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
	result = libfsntfs_directory_entries_tree_get_directory_entry_by_utf8_name(
	          internal_file_entry->directory_entries_tree,
	          utf8_string,
	          utf8_string_length,
	          &directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libfsntfs_directory_entry_get_mft_entry_index(
	     directory_entry,
	     &mft_entry_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry index.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     internal_file_entry->mft,
	     internal_file_entry->file_io_handle,
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
	if( libfsntfs_directory_entry_clone(
	     &sub_directory_entry,
	     directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub directory entry.",
		 function );

		goto on_error;
	}
	/* libfsntfs_file_entry_initialize takes over management of sub_directory_entry
	 */
	if( libfsntfs_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->mft,
	     internal_file_entry->security_descriptor_index,
	     mft_entry,
	     sub_directory_entry,
	     internal_file_entry->flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub file entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sub_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &sub_directory_entry,
		 NULL );
	}
	return( -1 );
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
	libfsntfs_directory_entry_t *directory_entry         = NULL;
	libfsntfs_directory_entry_t *sub_directory_entry     = NULL;
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_entry_t *mft_entry                     = NULL;
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
	result = libfsntfs_directory_entries_tree_get_directory_entry_by_utf16_name(
	          internal_file_entry->directory_entries_tree,
	          utf16_string,
	          utf16_string_length,
	          &directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libfsntfs_directory_entry_get_mft_entry_index(
	     directory_entry,
	     &mft_entry_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry index.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     internal_file_entry->mft,
	     internal_file_entry->file_io_handle,
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
	if( libfsntfs_directory_entry_clone(
	     &sub_directory_entry,
	     directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub directory entry.",
		 function );

		goto on_error;
	}
	/* libfsntfs_file_entry_initialize takes over management of sub_directory_entry
	 */
	if( libfsntfs_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->mft,
	     internal_file_entry->security_descriptor_index,
	     mft_entry,
	     sub_directory_entry,
	     internal_file_entry->flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub file entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sub_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &sub_directory_entry,
		 NULL );
	}
	return( -1 );
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
	if( internal_file_entry->data_cluster_block_stream == NULL )
	{
		return( 0 );
	}
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

		return( -1 );
	}
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
	static char *function = "libfsntfs_file_entry_read_buffer_at_offset";
	ssize_t read_count    = 0;

	if( libfsntfs_file_entry_seek_offset(
	     file_entry,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libfsntfs_file_entry_read_buffer(
	              file_entry,
	              buffer,
	              buffer_size,
	              error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
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

		return( -1 );
	}
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

		return( -1 );
	}
	return( 1 );
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
	*size = internal_file_entry->data_size;

	return( 1 );
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

	if( internal_file_entry->data_cluster_block_stream == NULL )
	{
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
		*number_of_extents = 0;
	}
	else
	{
		if( libfdata_stream_get_number_of_segments(
		     internal_file_entry->data_cluster_block_stream,
		     number_of_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data cluster block stream number of segments.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
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
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsntfs_file_entry_get_extent_by_index";
	size64_t data_size                                   = 0;
	uint32_t range_flags                                 = 0;
	int segment_file_index                               = 0;

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

	if( internal_file_entry->data_cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid extent index value out of bounds.",
		 function );

		return( -1 );
	}
	if( extent_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent offset.",
		 function );

		return( -1 );
	}
	if( extent_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent size.",
		 function );

		return( -1 );
	}
	if( extent_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent flags.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_segment_by_index(
	     internal_file_entry->data_cluster_block_stream,
	     extent_index,
	     &segment_file_index,
	     extent_offset,
	     extent_size,
	     &range_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data cluster block stream segment: %d.",
		 function,
		 extent_index );

		return( -1 );
	}
	if( libfdata_stream_get_segment_mapped_range(
	     internal_file_entry->data_cluster_block_stream,
	     extent_index,
	     extent_offset,
	     extent_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data cluster block stream segment: %d mapped range.",
		 function,
		 extent_index );

		return( -1 );
	}
	if( ( *extent_offset < 0 )
	 || ( (size64_t) *extent_offset >= internal_file_entry->data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid extent offset value out of bounds.",
		 function );

		return( -1 );
	}
	data_size = internal_file_entry->data_size - *extent_offset;

	if( *extent_size > data_size )
	{
		*extent_size = data_size;
	}
	*extent_flags = 0;

	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		*extent_flags |= LIBFSNTFS_EXTENT_FLAG_IS_SPARSE;
	}
	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		*extent_flags |= LIBFSNTFS_EXTENT_FLAG_IS_COMPRESSED;
	}
	return( 1 );
}

