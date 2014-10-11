/*
 * File entry functions
 *
 * Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_alternate_data_stream.h"
#include "libfsntfs_attribute.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_types.h"
#include "libfsntfs_volume.h"

/* Creates a file entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_initialize(
     libfsntfs_file_entry_t **file_entry,
     libbfio_handle_t *file_io_handle,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_internal_volume_t *internal_volume,
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_directory_entry_t *directory_entry,
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

		return( -1 );
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
	}
	internal_file_entry->file_io_handle  = file_io_handle;
	internal_file_entry->io_handle       = io_handle;
	internal_file_entry->internal_volume = internal_volume;
	internal_file_entry->mft_entry       = mft_entry;
	internal_file_entry->directory_entry = directory_entry;

	*file_entry = (libfsntfs_file_entry_t *) internal_file_entry;

	return( 1 );

on_error:
	if( internal_file_entry != NULL )
	{
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

		/* The file_io_handle, io_handle, internal_volume, mft_entry and directory_entry references are freed elsewhere
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
		memory_free(
		 internal_file_entry );
	}
	return( result );
}

/* Retrieves the creation date and time
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_creation_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *creation_time,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
	static char *function                                = "libfsntfs_file_entry_get_creation_time";

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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_creation_time(
	     file_name_values,
	     creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *modification_time,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
	static char *function                                = "libfsntfs_file_entry_get_modification_time";

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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_modification_time(
	     file_name_values,
	     modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the access date and time
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_access_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *access_time,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
	static char *function                                = "libfsntfs_file_entry_get_access_time";

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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_access_time(
	     file_name_values,
	     access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_entry_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *entry_modification_time,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
	static char *function                                = "libfsntfs_file_entry_get_entry_modification_time";

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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_entry_modification_time(
	     file_name_values,
	     entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the file attribute flags
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_file_attribute_flags(
     libfsntfs_file_entry_t *file_entry,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
	static char *function                                = "libfsntfs_file_entry_get_file_attribute_flags";

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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_file_attribute_flags(
	     file_name_values,
	     file_attribute_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags from file name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_utf8_name_size(
	     file_name_values,
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

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_utf8_name(
	     file_name_values,
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

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_utf16_name_size(
	     file_name_values,
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

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the $FILE_NAME attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_file_entry_get_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values       = NULL;
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
	if( internal_file_entry->directory_entry != NULL )
	{
		file_name_values = internal_file_entry->directory_entry->file_name_values;
	}
	if( file_name_values == NULL )
	{
		if( libfsntfs_attribute_get_value(
		     internal_file_entry->mft_entry->file_name_attribute,
		     (intptr_t **) &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute value.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_file_name_values_get_utf16_name(
	     file_name_values,
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

/* Determines if the file entry has the default data stream
 * Defined by the default (nameless) $DATA attribute
 * Returns 1 if the default data stream, 0 if not or -1 on error
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

	if( libfsntfs_mft_entry_get_number_of_attributes(
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
     libfsntfs_alternate_data_stream_t **alternate_data_stream,
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
	if( libfsntfs_alternate_data_stream_initialize(
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

/* Retrieves the alternate data stream for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_file_entry_get_alternate_data_stream_by_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_alternate_data_stream_t **alternate_data_stream,
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
	if( libfsntfs_alternate_data_stream_initialize(
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
     libfsntfs_alternate_data_stream_t **alternate_data_stream,
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
	if( libfsntfs_alternate_data_stream_initialize(
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

	if( libfsntfs_mft_entry_get_number_of_directory_entries(
	     internal_file_entry->mft_entry,
	     number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
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
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_entry_t *mft_entry                     = NULL;
	static char *function                                = "libfsntfs_file_entry_get_sub_file_entry_by_index";
	int mft_entry_index                                  = 0;

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
	if( libfsntfs_mft_entry_get_directory_entry_by_index(
	     internal_file_entry->mft_entry,
	     sub_file_entry_index,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry: %d.",
		 function,
		 sub_file_entry_index );

		return( -1 );
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

		return( -1 );
	}
	if( libfsntfs_volume_get_mft_entry_by_index(
	     internal_file_entry->internal_volume,
	     mft_entry_index,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %d.",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing MFT entry: %d.",
		 function,
		 mft_entry );

		return( -1 );
	}
	if( libfsntfs_mft_entry_read_directory_entries_tree(
	     mft_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: %d directory entries tree.",
		 function,
		 mft_entry );

		return( -1 );
	}
	if( libfsntfs_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->io_handle,
	     internal_file_entry->internal_volume,
	     mft_entry,
	     directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub file entry: %d.",
		 function,
		 sub_file_entry_index );

		return( -1 );
	}
	return( 1 );
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
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_entry_t *mft_entry                     = NULL;
	static char *function                                = "libfsntfs_file_entry_get_sub_file_entry_by_utf8_name";
	int mft_entry_index                                  = 0;
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
	result = libfsntfs_mft_entry_get_directory_entry_by_utf8_name(
	          internal_file_entry->mft_entry,
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

		return( -1 );
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

		return( -1 );
	}
	if( libfsntfs_volume_get_mft_entry_by_index(
	     internal_file_entry->internal_volume,
	     mft_entry_index,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %d.",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( libfsntfs_mft_entry_read_directory_entries_tree(
	     mft_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: %d directory entries tree.",
		 function,
		 mft_entry );

		return( -1 );
	}
	if( libfsntfs_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->io_handle,
	     internal_file_entry->internal_volume,
	     mft_entry,
	     directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub file entry.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	libfsntfs_internal_file_entry_t *internal_file_entry = NULL;
	libfsntfs_mft_entry_t *mft_entry                     = NULL;
	static char *function                                = "libfsntfs_file_entry_get_sub_file_entry_by_utf16_name";
	int mft_entry_index                                  = 0;
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
	result = libfsntfs_mft_entry_get_directory_entry_by_utf16_name(
	          internal_file_entry->mft_entry,
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

		return( -1 );
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

		return( -1 );
	}
	if( libfsntfs_volume_get_mft_entry_by_index(
	     internal_file_entry->internal_volume,
	     mft_entry_index,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %d.",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing MFT entry: %d.",
		 function,
		 mft_entry );

		return( -1 );
	}
	if( libfsntfs_mft_entry_read_directory_entries_tree(
	     mft_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: %d directory entries tree.",
		 function,
		 mft_entry );

		return( -1 );
	}
	if( libfsntfs_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->io_handle,
	     internal_file_entry->internal_volume,
	     mft_entry,
	     directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub file entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads data at the current offset
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

	if( internal_file_entry->data_cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing data cluster block stream.",
		 function );

		return( -1 );
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

/* Reads data at a specific offset
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

/* Seeks a certain offset
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

	if( internal_file_entry->data_cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing data cluster block stream.",
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

/* Retrieves the current offset
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

	if( internal_file_entry->data_cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing data cluster block stream.",
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

/* Retrieves the size
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

	if( internal_file_entry->data_cluster_block_stream == NULL )
	{
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
		*size = 0;
	}
	else
	{
		if( libfdata_stream_get_size(
		     internal_file_entry->data_cluster_block_stream,
		     size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data cluster block stream size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

