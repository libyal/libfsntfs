/*
 * Update (or change) journal functions
 *
 * Copyright (C) 2010-2015, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_data_stream.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_update_journal.h"
#include "libfsntfs_types.h"

/* Creates an update journal
 * Make sure the value update_journal is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_update_journal_initialize(
     libfsntfs_update_journal_t **update_journal,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_directory_entry_t *directory_entry,
     libfsntfs_attribute_t *data_attribute,
     libcerror_error_t **error )
{
	libfsntfs_internal_update_journal_t *internal_update_journal = NULL;
	static char *function                                        = "libfsntfs_update_journal_initialize";

	if( update_journal == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid update journal.",
		 function );

		return( -1 );
	}
	if( *update_journal != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid update journal value already set.",
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
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid $J data attribute.",
		 function );

		return( -1 );
	}
	internal_update_journal = memory_allocate_structure(
	                           libfsntfs_internal_update_journal_t );

	if( internal_update_journal == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create update journal.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_update_journal,
	     0,
	     sizeof( libfsntfs_internal_update_journal_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear update journal.",
		 function );

		memory_free(
		 internal_update_journal );

		return( -1 );
	}
	internal_update_journal->file_io_handle  = file_io_handle;
	internal_update_journal->io_handle       = io_handle;
	internal_update_journal->mft_entry       = mft_entry;
	internal_update_journal->directory_entry = directory_entry;
	internal_update_journal->data_attribute  = data_attribute;

	*update_journal = (libfsntfs_update_journal_t *) internal_update_journal;

	return( 1 );

on_error:
	if( internal_update_journal != NULL )
	{
		memory_free(
		 internal_update_journal );
	}
	return( -1 );
}

/* Frees an update journal
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_update_journal_free(
     libfsntfs_update_journal_t **update_journal,
     libcerror_error_t **error )
{
	libfsntfs_internal_update_journal_t *internal_update_journal = NULL;
	static char *function                                        = "libfsntfs_update_journal_free";
	int result                                                   = 1;

	if( update_journal == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid update journal.",
		 function );

		return( -1 );
	}
	if( *update_journal != NULL )
	{
		internal_update_journal = (libfsntfs_internal_update_journal_t *) *update_journal;
		*update_journal         = NULL;

		/* The file_io_handle, io_handle, mft_entry and data_attribute references are freed elsewhere
		 */
		if( libfsntfs_directory_entry_free(
		     &( internal_update_journal->directory_entry ),
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
		memory_free(
		 internal_update_journal );
	}
	return( result );
}

