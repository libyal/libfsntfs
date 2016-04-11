/*
 * Security descriptor index functions
 *
 * Copyright (C) 2010-2016, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_data_stream.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_security_descriptor_index.h"
#include "libfsntfs_security_descriptor_index_value.h"
#include "libfsntfs_security_descriptor_values.h"
#include "libfsntfs_types.h"

#include "fsntfs_secure.h"

/* Creates a security descriptor index
 * Make sure the value security_descriptor_index is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_index_initialize(
     libfsntfs_security_descriptor_index_t **security_descriptor_index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t *data_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_index_initialize";

	if( security_descriptor_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor index.",
		 function );

		return( -1 );
	}
	if( *security_descriptor_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security descriptor index value already set.",
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
	if( data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid $SDS data attribute.",
		 function );

		return( -1 );
	}
	*security_descriptor_index = memory_allocate_structure(
	                              libfsntfs_security_descriptor_index_t );

	if( *security_descriptor_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create security descriptor index.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *security_descriptor_index,
	     0,
	     sizeof( libfsntfs_security_descriptor_index_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear security descriptor index.",
		 function );

		memory_free(
		 security_descriptor_index );

		return( -1 );
	}
	if( libfsntfs_data_stream_initialize(
	     &( ( *security_descriptor_index )->data_stream ),
	     file_io_handle,
	     io_handle,
	     data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create $SDS data stream.",
		 function );

		return( -1 );
	}
	( *security_descriptor_index )->io_handle      = io_handle;
	( *security_descriptor_index )->file_io_handle = file_io_handle;
	( *security_descriptor_index )->mft_entry      = mft_entry;
	( *security_descriptor_index )->data_attribute = data_attribute;

	return( 1 );

on_error:
	if( *security_descriptor_index != NULL )
	{
		if( ( *security_descriptor_index )->data_stream != NULL )
		{
			libfsntfs_data_stream_free(
			 &( ( *security_descriptor_index )->data_stream ),
			 NULL );
		}
		memory_free(
		 *security_descriptor_index );

		*security_descriptor_index = NULL;
	}
	return( -1 );
}

/* Frees a security descriptor index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_index_free(
     libfsntfs_security_descriptor_index_t **security_descriptor_index,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_index_free";
	int result            = 1;

	if( security_descriptor_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor index.",
		 function );

		return( -1 );
	}
	if( *security_descriptor_index != NULL )
	{
		/* The file_io_handle, mft_entry and data_attribute references are freed elsewhere
		 */
		if( libfsntfs_data_stream_free(
		     &( ( *security_descriptor_index )->data_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free $SDS data stream.",
			 function );

			result = -1;
		}
		memory_free(
		 *security_descriptor_index );

		*security_descriptor_index = NULL;
	}
	return( result );
}

int libfsntfs_security_descriptor_index_read(
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libcerror_error_t **error )
{
	libfsntfs_index_value_t *index_value                                         = NULL;
	libfsntfs_security_descriptor_index_value_t *security_descriptor_index_value = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values           = NULL;
	uint8_t *security_descriptor_data                                            = NULL;
	static char *function                                                        = "libfsntfs_security_descriptor_index_read";
	ssize_t read_count                                                           = 0;
	int index_value_entry                                                        = 0;
	int number_of_index_values                                                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                                                         = 0;
	uint32_t value_32bit                                                         = 0;
#endif

	if( security_descriptor_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor index.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_read(
	     security_descriptor_index->mft_entry->sii_index,
	     security_descriptor_index->io_handle,
	     security_descriptor_index->file_io_handle,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read $SII index.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_get_number_of_index_values(
	     security_descriptor_index->mft_entry->sii_index,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of $SII index values.",
		 function );

		goto on_error;
	}
	for( index_value_entry = 0;
	     index_value_entry < number_of_index_values;
	     index_value_entry++ )
	{
		if( libfsntfs_index_get_index_value_by_index(
		     security_descriptor_index->mft_entry->sii_index,
		     security_descriptor_index->file_io_handle,
		     index_value_entry,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve $SII index value: %d.",
			 function,
			 index_value_entry );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: $SII index value: %d key data:\n",
			 function,
			 index_value_entry );
			libcnotify_print_data(
			 index_value->key_data,
			 (size_t) index_value->key_data_size,
			 0 );
		}
#endif
		if( libfsntfs_security_descriptor_index_value_initialize(
		     &security_descriptor_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create security descriptor index value.",
			 function );

			goto on_error;
		}
		if( libfsntfs_security_descriptor_index_value_read(
		     security_descriptor_index_value,
		     index_value->value_data,
		     (size_t) index_value->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read security descriptor index value.",
			 function );

			goto on_error;
		}
/* TODO bounds check of security_descriptor_index_value->data_size < ( 24 + SD header ) */
/* TODO bounds check of security_descriptor_index_value->data_offset */
		security_descriptor_data = memory_allocate(
		                            sizeof( uint8_t ) * security_descriptor_index_value->data_size );

		if( security_descriptor_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create security descriptor data.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     security_descriptor_data,
		     0,
		     sizeof( uint8_t ) * security_descriptor_index_value->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear security descriptor data.",
			 function );

			goto on_error;
		}
		read_count = libfsntfs_data_stream_read_buffer_at_offset(
		              security_descriptor_index->data_stream,
		              security_descriptor_data,
		              (size_t) security_descriptor_index_value->data_size,
		              (off64_t) security_descriptor_index_value->data_offset,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read security descriptor data at offset: 0x%08" PRIx64 ".",
			 function,
			 security_descriptor_index_value->data_offset );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (fsntfs_secure_index_value_t *) security_descriptor_data )->hash,
			 value_32bit );
			libcnotify_printf(
			 "%s: hash\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsntfs_secure_index_value_t *) security_descriptor_data )->identifier,
			 value_32bit );
			libcnotify_printf(
			 "%s: identifier\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsntfs_secure_index_value_t *) security_descriptor_data )->data_offset,
			 value_64bit );
			libcnotify_printf(
			 "%s: data offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsntfs_secure_index_value_t *) security_descriptor_data )->data_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: data size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libfsntfs_security_descriptor_values_initialize(
		     &security_descriptor_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create security descriptor values.",
			 function );

			goto on_error;
		}
		if( libfsntfs_security_descriptor_values_read(
		     security_descriptor_values,
		     &( security_descriptor_data[ sizeof( fsntfs_secure_index_value_t ) ] ),
		     (size_t) security_descriptor_index_value->data_size - sizeof( fsntfs_secure_index_value_t ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read security descriptor values.",
			 function );

			goto on_error;
		}
		memory_free(
		 security_descriptor_data );

		security_descriptor_data = NULL;

		if( libfsntfs_security_descriptor_values_free(
		     &security_descriptor_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free security descriptor values.",
			 function );

			goto on_error;
		}
		if( libfsntfs_security_descriptor_index_value_free(
		     &security_descriptor_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free security descriptor index value.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( security_descriptor_data != NULL )
	{
		memory_free(
		 security_descriptor_data );
	}
	if( security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &security_descriptor_values,
		 NULL );
	}
	if( security_descriptor_index_value != NULL )
	{
		libfsntfs_security_descriptor_index_value_free(
		 &security_descriptor_index_value,
		 NULL );
	}
	return( -1 );
}

