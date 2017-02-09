/*
 * Master File Table (MFT) entry functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_attribute_list.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_index.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_types.h"
#include "libfsntfs_unused.h"

#include "fsntfs_mft_entry.h"

const char fsntfs_mft_entry_signature[ 4 ] = "FILE";

/* Checks if a buffer containing the MFT entry is filled with 0-byte values (empty-block)
 * Returns 1 if empty, 0 if not or -1 on error
 */
int libfsntfs_mft_entry_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsntfs_aligned_t *aligned_data_index = NULL;
	libfsntfs_aligned_t *aligned_data_start = NULL;
	uint8_t *data_index                     = NULL;
	uint8_t *data_start                     = NULL;
	static char *function                   = "libfsntfs_mft_entry_check_for_empty_block";

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	data_start = (uint8_t *) data;
	data_index = (uint8_t *) data + 1;
	data_size -= 1;

	/* Only optimize for data larger than the alignment
	 */
	if( data_size > ( 2 * sizeof( libfsntfs_aligned_t ) ) )
	{
		/* Align the data start
		 */
		while( ( (intptr_t) data_start % sizeof( libfsntfs_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_start += 1;
			data_index += 1;
			data_size  -= 1;
		}
		/* Align the data index
		 */
		while( ( (intptr_t) data_index % sizeof( libfsntfs_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_index += 1;
			data_size  -= 1;
		}
		aligned_data_start = (libfsntfs_aligned_t *) data_start;
		aligned_data_index = (libfsntfs_aligned_t *) data_index;

		while( data_size > sizeof( libfsntfs_aligned_t ) )
		{
			if( *aligned_data_start != *aligned_data_index )
			{
				return( 0 );
			}
			aligned_data_index += 1;
			data_size          -= sizeof( libfsntfs_aligned_t );
		}
		data_index = (uint8_t *) aligned_data_index;
	}
	while( data_size != 0 )
	{
		if( *data_start != *data_index )
		{
			return( 0 );
		}
		data_index += 1;
		data_size  -= 1;
	}
	return( 1 );
}

/* Creates a MFT entry
 * Make sure the value mft_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_initialize(
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_initialize";

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
	if( *mft_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT entry value already set.",
		 function );

		return( -1 );
	}
	*mft_entry = memory_allocate_structure(
	              libfsntfs_mft_entry_t );

	if( *mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mft_entry,
	     0,
	     sizeof( libfsntfs_mft_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MFT entry.",
		 function );

		memory_free(
		 *mft_entry );

		*mft_entry = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *mft_entry )->attributes_array ),
	     0,
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
	if( libcdata_array_initialize(
	     &( ( *mft_entry )->alternate_data_attributes_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create alternate data attributes array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *mft_entry )->index_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *mft_entry != NULL )
	{
		if( ( *mft_entry )->index_array != NULL )
		{
			libcdata_array_free(
			 &( ( *mft_entry )->index_array ),
			 NULL,
			 NULL );
		}
		if( ( *mft_entry )->alternate_data_attributes_array != NULL )
		{
			libcdata_array_free(
			 &( ( *mft_entry )->alternate_data_attributes_array ),
			 NULL,
			 NULL );
		}
		if( ( *mft_entry )->attributes_array != NULL )
		{
			libcdata_array_free(
			 &( ( *mft_entry )->attributes_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 *mft_entry );

		*mft_entry = NULL;
	}
	return( -1 );
}

/* Frees a MFT entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_free(
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_free";
	int result            = 1;

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
	if( *mft_entry != NULL )
	{
		if( ( *mft_entry )->data != NULL )
		{
			memory_free(
			 ( *mft_entry )->data );
		}
		/* The specific attribute references point to attributes in the array
		 * and are freed by freeing the array and its values
		 */
		if( libcdata_array_free(
		     &( ( *mft_entry )->attributes_array ),
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
		/* The alternate_data_attributes_array only contains references that are managed
		 * by the attributes_array
		 */
		if( libcdata_array_free(
		     &( ( *mft_entry )->alternate_data_attributes_array ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free alternate data attributes array.",
			 function );

			result = -1;
		}
		/* The standard_information_attribute, object_identifier_attribute,
		 * volume_information_attribute and volume_name_attibute references
		 * are managed by the attributes_array
		 */
		if( libcdata_array_free(
		     &( ( *mft_entry )->index_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index array.",
			 function );

			result = -1;
		}
		memory_free(
		 *mft_entry );

		*mft_entry = NULL;
	}
	return( result );
}

/* Reads a specific MFT entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_read(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     off64_t file_offset,
     uint32_t mft_entry_index,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_read";
	int result            = 0;

	result = libfsntfs_mft_entry_read_header(
	          mft_entry,
	          io_handle,
	          file_io_handle,
	          file_offset,
	          mft_entry_index,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read header.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: MFT entry: %" PRIu32 " is empty.\n",
			 function,
			 mft_entry_index );
		}
#endif
		mft_entry->is_empty = 1;
	}
	else
	{
		if( libfsntfs_mft_entry_read_attributes(
		     mft_entry,
		     io_handle,
		     file_io_handle,
		     mft_entry_vector,
		     flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attributes.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the MFT entry header
 * Returns 1 if successful, 0 if empty or -1 on error
 */
int libfsntfs_mft_entry_read_header(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t mft_entry_index,
     libcerror_error_t **error )
{
	static char *function                     = "libfsntfs_mft_entry_read_header";
	size_t mft_entry_data_offset              = 0;
	size_t mft_entry_fixup_offset             = 0;
	size_t mft_entry_fixup_placeholder_offset = 0;
	size_t unknown_data_size                  = 0;
	ssize_t read_count                        = 0;
	uint16_t fixup_value_index                = 0;
	uint16_t fixup_values_offset              = 0;
	uint16_t number_of_fixup_values           = 0;
	uint16_t total_entry_size                 = 0;
	int result                                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t read_size                          = 0;
	uint16_t value_16bit                      = 0;
#endif

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading MFT entry at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek MFT entry offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	mft_entry->data = (uint8_t *) memory_allocate(
	                               io_handle->mft_entry_size );

	if( mft_entry->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT entry data.",
		 function );

		goto on_error;
	}
	mft_entry->data_size = io_handle->mft_entry_size;

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              mft_entry->data,
	              mft_entry->data_size,
	              error );

	if( read_count != (ssize_t) mft_entry->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry data.",
		 function );

		goto on_error;
	}
	result = libfsntfs_mft_entry_check_for_empty_block(
	          mft_entry->data,
	          mft_entry->data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MFT entry is empty.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		return( 0 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->fixup_values_offset,
	 fixup_values_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( fixup_values_offset > 42 )
		{
			read_size = sizeof( fsntfs_mft_entry_header_t );
		}
		else
		{
			read_size = 42;
		}
		libcnotify_printf(
		 "%s: MFT entry header data:\n",
		 function );
		libcnotify_print_data(
		 mft_entry->data,
		 read_size,
		 0 );
	}
#endif
	if( memory_compare(
	     ( (fsntfs_mft_entry_header_t *) mft_entry->data )->signature,
	     "BAAD",
	     4 ) == 0 )
	{
/* TODO do empty block check on the remainder of the MFT entry? */
		return( 0 );
	}
	if( memory_compare(
	     ( (fsntfs_mft_entry_header_t *) mft_entry->data )->signature,
	     fsntfs_mft_entry_signature,
	     4 ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid MFT entry signature.\n",
			 function );
		}
#endif
		mft_entry->is_corrupted = 1;

/* TODO handle corruption */
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid MFT entry signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->number_of_fixup_values,
	 number_of_fixup_values );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->journal_sequence_number,
	 mft_entry->journal_sequence_number );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->sequence,
	 mft_entry->sequence );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->reference_count,
	 mft_entry->reference_count );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->attributes_offset,
	 mft_entry->attributes_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->flags,
	 mft_entry->flags );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->used_entry_size,
	 mft_entry->used_entry_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->total_entry_size,
	 total_entry_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->base_record_file_reference,
	 mft_entry->base_record_file_reference );

	if( fixup_values_offset > 42 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->index,
		 mft_entry->index );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->signature[ 0 ],
		 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->signature[ 1 ],
		 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->signature[ 2 ],
		 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: fixup values offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 fixup_values_offset );

		libcnotify_printf(
		 "%s: number of fixup values\t\t\t\t: %" PRIu16 "\n",
		 function,
		 number_of_fixup_values );

		libcnotify_printf(
		 "%s: journal sequence number\t\t\t: %" PRIu64 "\n",
		 function,
		 mft_entry->journal_sequence_number );

		libcnotify_printf(
		 "%s: sequence\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry->sequence );

		libcnotify_printf(
		 "%s: reference count\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry->reference_count );

		libcnotify_printf(
		 "%s: attributes offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry->attributes_offset );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 mft_entry->flags );
		libfsntfs_debug_print_mft_entry_flags(
		 mft_entry->flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: used entry size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry->used_entry_size );

		libcnotify_printf(
		 "%s: total entry size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 total_entry_size );

		libcnotify_printf(
		 "%s: base record file reference\t\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 function,
		 mft_entry->base_record_file_reference & 0xffffffffffffUL,
		 mft_entry->base_record_file_reference >> 48 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->first_available_attribute_identifier,
		 value_16bit );
		libcnotify_printf(
		 "%s: first available attribute identifier\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		if( fixup_values_offset > 42 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsntfs_mft_entry_header_t *) mft_entry->data )->unknown1,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%04" PRIu16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: index\t\t\t\t\t\t: %" PRIu32 "\n",
			 function,
			 mft_entry->index );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( fixup_values_offset > 42 )
	{
		mft_entry_data_offset += sizeof( fsntfs_mft_entry_header_t );
	}
	else
	{
		mft_entry_data_offset += 42;
	}
	if( mft_entry->index != mft_entry_index )
	{
		mft_entry->index = mft_entry_index;
	}
	if( ( mft_entry->attributes_offset < sizeof( fsntfs_mft_entry_header_t ) )
	 || ( mft_entry->attributes_offset >= mft_entry->data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: attributes offset value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( mft_entry->data_size != (size_t) total_entry_size )
		{
			libcnotify_printf(
			 "%s: mismatch in total MFT entry size (calculated: %" PRIzd ", stored: %" PRIu16 ").\n",
			 function,
			 mft_entry->data_size,
			 total_entry_size );
		}
	}
#endif
	if( number_of_fixup_values > 0 )
	{
		/* In NT4 the fixup values offset can point to wfixupPattern
		 */
		if( ( fixup_values_offset < 42 )
		 || ( fixup_values_offset >= mft_entry->data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: fixup values offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( fixup_values_offset > mft_entry->attributes_offset )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: fixup values offset exceeds attributes offset.",
			 function );

			goto on_error;
		}
		if( mft_entry_data_offset < fixup_values_offset )
		{
			unknown_data_size = (size_t) fixup_values_offset - mft_entry_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unknown data:\n",
				 function );
				libcnotify_print_data(
				 &( mft_entry->data[ mft_entry_data_offset ] ),
				 unknown_data_size,
				 0 );
			}
#endif
			mft_entry_data_offset += unknown_data_size;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: fixup values data:\n",
			 function );
			libcnotify_print_data(
			 &( mft_entry->data[ mft_entry_data_offset ] ),
			 2 + ( number_of_fixup_values * 2 ),
			 0 );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( mft_entry->data[ mft_entry_data_offset ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: fixup placeholder value\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
#endif
		mft_entry_fixup_placeholder_offset = mft_entry_data_offset;

		mft_entry_data_offset += 2;

		mft_entry_fixup_offset = io_handle->bytes_per_sector - 2;

		for( fixup_value_index = 0;
		     fixup_value_index < number_of_fixup_values;
		     fixup_value_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 &( mft_entry->data[ mft_entry_data_offset ] ),
				 value_16bit );
				libcnotify_printf(
				 "%s: fixup value: %" PRIu16 "\t\t\t\t\t: 0x%04" PRIx16 "\n",
				 function,
				 fixup_value_index,
				 value_16bit );
			}
#endif
			if( mft_entry_fixup_offset < mft_entry->data_size )
			{
				if( ( mft_entry->data[ mft_entry_fixup_offset ] != mft_entry->data[ mft_entry_fixup_placeholder_offset ] )
				 && ( mft_entry->data[ mft_entry_fixup_offset + 1 ] != mft_entry->data[ mft_entry_fixup_placeholder_offset + 1 ] ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						byte_stream_copy_to_uint16_little_endian(
						 &( mft_entry->data[ mft_entry_fixup_offset ] ),
						 value_16bit );
						libcnotify_printf(
						 "%s: corruption detected - mismatch between placeholder and value at offset: %" PRIzd " (0x%04" PRIx16 ")\n",
						 function,
						 mft_entry_fixup_offset,
						 value_16bit );
					}
#endif
/* TODO handle error */
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: applying fixup value: %" PRIu16 "\t\t\t: (offset: %" PRIzd ") 0x%02" PRIx8 "%02" PRIx8 " => (offset: %" PRIzd ") 0x%02" PRIx8 "%02" PRIx8 "\n",
					 function,
					 fixup_value_index,
					 mft_entry_fixup_offset,
					 mft_entry->data[ mft_entry_fixup_offset + 1 ],
					 mft_entry->data[ mft_entry_fixup_offset ],
					 mft_entry_data_offset,
					 mft_entry->data[ mft_entry_data_offset + 1 ],
					 mft_entry->data[ mft_entry_data_offset ] );
				}
#endif
				mft_entry->data[ mft_entry_fixup_offset ]     = mft_entry->data[ mft_entry_data_offset ];
				mft_entry->data[ mft_entry_fixup_offset + 1 ] = mft_entry->data[ mft_entry_data_offset + 1 ];
			}
			mft_entry_data_offset  += 2;
			mft_entry_fixup_offset += io_handle->bytes_per_sector;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( mft_entry_data_offset < mft_entry->attributes_offset )
		{
			libcnotify_printf(
			 "%s: unknown data:\n",
			 function );
			libcnotify_print_data(
			 &( mft_entry->data[ mft_entry_data_offset ] ),
			 (size_t) mft_entry->attributes_offset - mft_entry_data_offset,
			 0 );
		}
	}
#endif
	return( 1 );

on_error:
	if( mft_entry->data != NULL )
	{
		memory_free(
		 mft_entry->data );

		mft_entry->data = NULL;
	}
	mft_entry->data_size = 0;

	return( -1 );
}

/* Reads attributes
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_read_attributes(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     uint8_t flags,
     libcerror_error_t **error )
{
	libcdata_array_t *attribute_list = NULL;
	libfsntfs_attribute_t *attribute = NULL;
	static char *function            = "libfsntfs_mft_entry_read_attributes";
	size_t attributes_data_offset    = 0;
	ssize_t read_count               = 0;
	uint32_t attribute_type          = 0;

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
	if( mft_entry->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT entry - missing data.",
		 function );

		goto on_error;
	}
	if( mft_entry->data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry - data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( mft_entry->attributes_offset < sizeof( fsntfs_mft_entry_header_t ) )
	 || ( mft_entry->attributes_offset >= mft_entry->data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry - attributes offset value out of bounds.",
		 function );

		goto on_error;
	}
	attributes_data_offset = mft_entry->attributes_offset;

	byte_stream_copy_to_uint32_little_endian(
	 &( mft_entry->data[ attributes_data_offset ] ),
	 attribute_type );

	while( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_END_OF_ATTRIBUTES )
	{
		if( libfsntfs_attribute_initialize(
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute.",
			 function );

			goto on_error;
		}
		read_count = libfsntfs_attribute_read_from_mft(
			      attribute,
			      io_handle,
			      mft_entry->data,
			      mft_entry->data_size,
			      attributes_data_offset,
			      flags,
			      error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute.",
			 function );

			goto on_error;
		}
		attributes_data_offset += read_count;

		if( libfsntfs_attribute_get_type(
		     attribute,
		     &attribute_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute type.",
			 function );

			goto on_error;
		}
		if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST )
		{
			if( libcdata_array_initialize(
			     &attribute_list,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create attribute list array.",
				 function );

				goto on_error;
			}
/* TODO if flag is set mark file entry as in complete ? */
			if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
			{
				if( libfsntfs_attribute_list_read_from_attribute(
				     attribute_list,
				     io_handle,
				     file_io_handle,
				     attribute,
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
			}
		}
		if( libfsntfs_mft_entry_append_attribute(
		     mft_entry,
		     io_handle,
		     file_io_handle,
		     attribute,
		     flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append attribute.",
			 function );

			/* Prevent a double free of the attribute */
			attribute = NULL;

			goto on_error;
		}
		attribute = NULL;

		byte_stream_copy_to_uint32_little_endian(
		 &( mft_entry->data[ attributes_data_offset ] ),
		 attribute_type );
	}
	attributes_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( attributes_data_offset != (size_t) mft_entry->used_entry_size )
		{
			libcnotify_printf(
			 "%s: mismatch in used MFT entry size (calculated: %" PRIzd ", stored: %" PRIu16 ").\n",
			 function,
			 attributes_data_offset,
			 mft_entry->used_entry_size );
		}
	}
#endif
	if( attribute_list != NULL )
	{
		if( libfsntfs_mft_entry_read_attributes_from_attribute_list(
		     mft_entry,
		     io_handle,
		     file_io_handle,
		     mft_entry_vector,
		     attribute_list,
		     flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attributes from attribute list.",
			 function );

			goto on_error;
		}
		if( libcdata_array_free(
		     &attribute_list,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_internal_attribute_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attribute list array.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( attribute_list != NULL )
	{
		libcdata_array_free(
		 &attribute_list,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_internal_attribute_free,
		 NULL );
	}
	if( attribute != NULL )
	{
		libfsntfs_internal_attribute_free(
		 (libfsntfs_internal_attribute_t **) &attribute,
		 NULL );
	}
	libcdata_array_empty(
	 mft_entry->index_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_index_free,
	 NULL );

	mft_entry->i30_index = NULL;
	mft_entry->sii_index = NULL;

	libcdata_array_empty(
	 mft_entry->alternate_data_attributes_array,
	 NULL,
	 NULL );

	libcdata_array_empty(
	 mft_entry->attributes_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_internal_attribute_free,
	 NULL );

	return( -1 );
}

/* Reads attributes from an attribute list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_read_attributes_from_attribute_list(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     libcdata_array_t *attribute_list,
     uint8_t flags,
     libcerror_error_t **error )
{
	libcdata_array_t *list_mft_entry_array = NULL;
	libfcache_cache_t *mft_entry_cache     = NULL;
	libfsntfs_attribute_t *attribute       = NULL;
	libfsntfs_attribute_t *list_attribute  = NULL;
	libfsntfs_mft_entry_t *list_mft_entry  = NULL;
	static char *function                  = "libfsntfs_mft_entry_read_attributes_from_attribute_list";
	uint64_t list_mft_entry_index          = 0;
	uint16_t sequence_number               = 0;
	int attribute_index                    = 0;
	int entry_index                        = 0;
	int list_attribute_index               = 0;
	int number_of_attributes               = 0;
	int number_of_list_attributes          = 0;

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
	if( libcdata_array_get_number_of_entries(
	     attribute_list,
	     &number_of_list_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attribute list entries.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &list_mft_entry_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list MFT entry array.",
		 function );

		goto on_error;
	}
	for( list_attribute_index = 0;
	     list_attribute_index < number_of_list_attributes;
	     list_attribute_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     attribute_list,
		     list_attribute_index,
		     (intptr_t **) &list_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute list entry: %d.",
			 function,
			 list_attribute_index );

			goto on_error;
		}
		if( libfsntfs_attribute_get_file_reference(
		     list_attribute,
		     &list_mft_entry_index,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute list entry: %d file reference.",
			 function,
			 list_attribute_index );

			goto on_error;
		}
		/* Ignore the current MFT entry
		 */
		if( list_mft_entry_index == (uint64_t) mft_entry->index )
		{
			continue;
		}
		if( libcdata_array_insert_entry(
		     list_mft_entry_array,
		     &entry_index,
		     (intptr_t *) list_attribute,
		     (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_attribute_compare_by_file_reference,
		     LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert list attribute in list MFT entry array.",
			 function );

			goto on_error;
		}
	}
	/* Use a local cache to prevent cache outs
	 */
	if( libfcache_cache_initialize(
	     &mft_entry_cache,
	     8,
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
	/* Read the list MFT entries
	 */
	if( libcdata_array_get_number_of_entries(
	     list_mft_entry_array,
	     &number_of_list_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of list MFT entries.",
		 function );

		goto on_error;
	}
	for( list_attribute_index = 0;
	     list_attribute_index < number_of_list_attributes;
	     list_attribute_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     list_mft_entry_array,
		     list_attribute_index,
		     (intptr_t **) &list_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list attribute: %d.",
			 function,
			 list_attribute_index );

			goto on_error;
		}
		if( libfsntfs_attribute_get_file_reference(
		     list_attribute,
		     &list_mft_entry_index,
		     &sequence_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list attribute: %d file reference.",
			 function,
			 list_attribute_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data file reference: MFT entry: %" PRIu64 ", sequence: %" PRIu16 "\n",
			 function,
			 list_mft_entry_index,
			 sequence_number );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( list_mft_entry_index > (uint64_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: list MFT entry reference value out of bounds.",
			 function );

			goto on_error;
		}
		if( libfdata_vector_get_element_value_by_index(
		     mft_entry_vector,
		     (intptr_t *) file_io_handle,
		     mft_entry_cache,
		     (int) list_mft_entry_index,
		     (intptr_t **) &list_mft_entry,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
			 function,
			 list_mft_entry_index );

			goto on_error;
		}
		if( list_mft_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing MFT entry: %" PRIu64 ".",
			 function,
			 list_mft_entry_index );

			goto on_error;
		}
		if( libfsntfs_mft_entry_get_number_of_attributes(
		     list_mft_entry,
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
			     list_mft_entry,
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
				 "%s: missing attribute: %d.",
				 function,
				 attribute_index );

				goto on_error;
			}
			if( libcdata_array_set_entry_by_index(
			     list_mft_entry->attributes_array,
			     attribute_index,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set entry: %d in attributes array.",
				 function,
				 attribute_index );

				goto on_error;
			}
/* TODO convert attribute list attributes into readable attributes */
			if( libfsntfs_mft_entry_append_attribute(
			     mft_entry,
			     io_handle,
			     file_io_handle,
			     attribute,
			     flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append attribute.",
				 function );

				goto on_error;
			}
		}
	}
	if( libfcache_cache_free(
	     &mft_entry_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MFT entry cache.",
		 function );

		goto on_error;
	}
	if( libcdata_array_free(
	     &list_mft_entry_array,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free list MFT entry array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( mft_entry_cache != NULL )
	{
		libfcache_cache_free(
		 &mft_entry_cache,
		 NULL );
	}
	if( list_mft_entry_array != NULL )
	{
		libcdata_array_free(
		 &list_mft_entry_array,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Reads the MFT entry directory entry index if available
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_read_directory_entries_tree(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_btree_t *directory_entries_tree,
     uint8_t flags,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                      = NULL;
	libfsntfs_directory_entry_t *directory_entry          = NULL;
	libfsntfs_directory_entry_t *existing_directory_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values        = NULL;
	libfsntfs_index_value_t *index_value                  = NULL;
	static char *function                                 = "libfsntfs_mft_entry_read_directory_entries_tree";
	uint8_t file_name_namespace                           = 0;
	int index_value_entry                                 = 0;
	int number_of_index_values                            = 0;
	int result                                            = 0;
	int value_index                                       = 0;

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
	if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) != 0 )
	{
		return( 1 );
	}
	if( mft_entry->i30_index == NULL )
	{
		return( 1 );
	}
	if( libfsntfs_index_read(
	     mft_entry->i30_index,
	     io_handle,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read $I30 index.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_get_number_of_index_values(
	     mft_entry->i30_index,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of $I30 index values.",
		 function );

		goto on_error;
	}
	for( index_value_entry = 0;
	     index_value_entry < number_of_index_values;
	     index_value_entry++ )
	{
		if( libfsntfs_index_get_index_value_by_index(
		     mft_entry->i30_index,
		     file_io_handle,
		     index_value_entry,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve $I30 index value: %d.",
			 function,
			 index_value_entry );

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
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index value: %03d file reference: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
			 function,
			 index_value_entry,
			 index_value->file_reference & 0xffffffffffffUL,
			 index_value->file_reference >> 48 );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libfsntfs_file_name_values_read(
		     file_name_values,
		     index_value->key_data,
		     (size_t) index_value->key_data_size,
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
		if( file_name_values->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid file name values - missing name.",
			 function );

			goto on_error;
		}
		/* Ignore the file name with the . as its name
		 */
		if( ( file_name_values->name_size == 2 )
		 && ( file_name_values->name[ 0 ] == 0x2e )
		 && ( file_name_values->name[ 1 ] == 0x00 ) )
		{
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
			continue;
		}
		file_name_namespace = file_name_values->name_namespace;

		if( libfsntfs_directory_entry_initialize(
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		directory_entry->file_reference = index_value->file_reference;

		if( file_name_namespace == LIBFSNTFS_FILE_NAME_NAMESPACE_DOS )
		{
			directory_entry->short_file_name_values = file_name_values;
		}
		else
		{
			directory_entry->file_name_values = file_name_values;
		}
		file_name_values = NULL;

		result = libcdata_btree_insert_value(
			  directory_entries_tree,
			  &value_index,
			  (intptr_t *) directory_entry,
			  (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_directory_entry_compare,
			  &upper_node,
			  (intptr_t **) &existing_directory_entry,
			  error ) ;

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert directory entry into tree.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( existing_directory_entry == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing existing directory entry.",
				 function );

				goto on_error;
			}
			if( file_name_namespace == LIBFSNTFS_FILE_NAME_NAMESPACE_DOS )
			{
				if( existing_directory_entry->short_file_name_values == NULL )
				{
					existing_directory_entry->short_file_name_values = directory_entry->short_file_name_values;
					directory_entry->short_file_name_values          = NULL;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				else if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: short file name of existing directory entry already set.\n",
					 function );
				}
#endif
			}
			else
			{
				if( existing_directory_entry->file_name_values == NULL )
				{
					existing_directory_entry->file_name_values = directory_entry->file_name_values;
					directory_entry->file_name_values          = NULL;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				else if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: file name of existing directory entry already set.\n",
					 function );
				}
#endif
			}
			if( libfsntfs_directory_entry_free(
			     &directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entry.",
				 function );

				goto on_error;
			}
		}
		directory_entry = NULL;
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
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

/* Determines if the MFT entry is empty
 * Returns 1 if empty, 0 if not or -1 on error
 */
int libfsntfs_mft_entry_is_empty(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_is_empty";

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
	return( (int) mft_entry->is_empty );
}

/* Determines if the MFT entry is allocated (in use)
 * Returns 1 if allocated, 0 if not or -1 on error
 */
int libfsntfs_mft_entry_is_allocated(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_is_allocated";

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
	if( ( mft_entry->flags & LIBFSNTFS_MFT_ENTRY_FLAG_IN_USE ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_get_file_reference(
     libfsntfs_mft_entry_t *mft_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_get_file_reference";

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
	*file_reference  = (uint64_t) mft_entry->sequence << 48;
	*file_reference |= mft_entry->index;

	return( 1 );
}

/* Retrieves the base record file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_get_base_record_file_reference(
     libfsntfs_mft_entry_t *mft_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_get_base_record_file_reference";

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
	*file_reference = mft_entry->base_record_file_reference;

	return( 1 );
}

/* Retrieves the journal sequence number
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_get_journal_sequence_number(
     libfsntfs_mft_entry_t *mft_entry,
     uint64_t *journal_sequence_number,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_get_journal_sequence_number";

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
	if( journal_sequence_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid journal sequence number.",
		 function );

		return( -1 );
	}
	*journal_sequence_number = mft_entry->journal_sequence_number;

	return( 1 );
}

/* Retrieves the number of attributes
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_get_number_of_attributes(
     libfsntfs_mft_entry_t *mft_entry,
     int *number_of_attributes,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_get_number_of_attributes";

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
	if( libcdata_array_get_number_of_entries(
	     mft_entry->attributes_array,
	     number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from attributes array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_get_attribute_by_index(
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_get_attribute_by_index";

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
	if( libcdata_array_get_entry_by_index(
	     mft_entry->attributes_array,
	     attribute_index,
	     (intptr_t **) attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from attributes array.",
		 function,
		 attribute_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of alternate data attributes
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
     libfsntfs_mft_entry_t *mft_entry,
     int *number_of_attributes,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_get_number_of_alternate_data_attributes";

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
	if( libcdata_array_get_number_of_entries(
	     mft_entry->alternate_data_attributes_array,
	     number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from alternate data attributes array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific alternate data attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_get_alternate_data_attribute_by_index(
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_get_alternate_data_attribute_by_index";

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
	if( libcdata_array_get_entry_by_index(
	     mft_entry->alternate_data_attributes_array,
	     attribute_index,
	     (intptr_t **) attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from alternate data attributes array.",
		 function,
		 attribute_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific alternate data attribute for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such directory entry or -1 on error
 */
int libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function    = "libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name";
	int attribute_index      = 0;
	int number_of_attributes = 0;
	int result               = 0;

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
	if( libcdata_array_get_number_of_entries(
	     mft_entry->alternate_data_attributes_array,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from alternate data attributes array.",
		 function );

		return( -1 );
	}
	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     mft_entry->alternate_data_attributes_array,
		     attribute_index,
		     (intptr_t **) attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from alternative data attributes array.",
			 function,
			 attribute_index );

			return( -1 );
		}
		result = libfsntfs_attribute_compare_name_with_utf8_string(
			  *attribute,
			  utf8_string,
			  utf8_string_length,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with alternative data attribute name.",
			 function );

			goto on_error;
		}
		else if( result == 1 )
		{
			return( 1 );
		}
	}
	*attribute = NULL;

	return( 0 );

on_error:
	*attribute = NULL;

	return( -1 );
}

/* Retrieves a specific alternate data attribute for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such directory entry or -1 on error
 */
int libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function    = "libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name";
	int attribute_index      = 0;
	int number_of_attributes = 0;
	int result               = 0;

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
	if( libcdata_array_get_number_of_entries(
	     mft_entry->alternate_data_attributes_array,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from alternate data attributes array.",
		 function );

		return( -1 );
	}
	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     mft_entry->alternate_data_attributes_array,
		     attribute_index,
		     (intptr_t **) attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from alternative data attributes array.",
			 function,
			 attribute_index );

			return( -1 );
		}
		result = libfsntfs_attribute_compare_name_with_utf16_string(
			  *attribute,
			  utf16_string,
			  utf16_string_length,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with alternative data attribute name.",
			 function );

			goto on_error;
		}
		else if( result == 1 )
		{
			return( 1 );
		}
	}
	*attribute = NULL;

	return( 0 );

on_error:
	*attribute = NULL;

	return( -1 );
}

/* Appends a new index with the specified name
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_append_index(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *name,
     size_t name_size,
     libfsntfs_index_t **index,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_append_index";
	int entry_index       = 0;
	int result            = 0;

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
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_initialize(
	     index,
	     name,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index.",
		 function );

		return( -1 );
	}
	if( libcdata_array_append_entry(
	     mft_entry->index_array,
	     &entry_index,
	     (intptr_t *) *index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append index to array.",
		 function );

		libfsntfs_index_free(
		 index,
		 NULL );

		return( -1 );
	}
	if( mft_entry->i30_index == NULL )
	{
		result = libuna_utf8_string_compare_with_utf16_stream(
			  (uint8_t *) "$I30",
			  4,
			  ( *index )->name,
			  ( *index )->name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare index name with $I30.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			mft_entry->i30_index = *index;
		}
	}
	if( mft_entry->sii_index == NULL )
	{
		result = libuna_utf8_string_compare_with_utf16_stream(
			  (uint8_t *) "$SII",
			  4,
			  ( *index )->name,
			  ( *index )->name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare index name with $SII.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			mft_entry->sii_index = *index;
		}
	}
	return( 1 );
}

/* Retrieves an index with the specified name
 * Returns 1 if successful, 0 if no index was found or -1 on error
 */
int libfsntfs_mft_entry_get_index_by_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *name,
     size_t name_size,
     libfsntfs_index_t **index,
     libcerror_error_t **error )
{
	static char *function       = "libfsntfs_mft_entry_get_index_by_name";
	int index_entry             = 0;
	int number_of_index_entries = 0;
	int result                  = 0;

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
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( mft_entry->i30_index != NULL )
	{
		result = libuna_utf8_string_compare_with_utf16_stream(
			  (uint8_t *) "$I30",
			  4,
			  name,
			  name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare index name with $I30.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			*index = mft_entry->i30_index;

			return( 1 );
		}
	}
	if( mft_entry->sii_index != NULL )
	{
		result = libuna_utf8_string_compare_with_utf16_stream(
			  (uint8_t *) "$SII",
			  4,
			  name,
			  name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare index name with $SII.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			*index = mft_entry->sii_index;

			return( 1 );
		}
	}
	if( libcdata_array_get_number_of_entries(
	     mft_entry->index_array,
	     &number_of_index_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of index entries.",
		 function );

		return( -1 );
	}
	for( index_entry = 0;
	     index_entry < number_of_index_entries;
	     index_entry++ )
	{
		if( libcdata_array_get_entry_by_index(
		     mft_entry->index_array,
		     index_entry,
		     (intptr_t **) index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index: %d.",
			 function,
			 index_entry );

			return( -1 );
		}
		if( *index == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing index: %d.",
			 function,
			 index_entry );

			return( -1 );
		}
		if( ( *index )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid index: %d - missing name.",
			 function,
			 index_entry );

			return( -1 );
		}
		if( ( ( *index )->name_size == name_size )
		 && ( memory_compare(
		       ( *index )->name,
		       name,
		       name_size ) == 0 ) )
		{
			return( 1 );
		}
	}
	*index = NULL;

	return( 0 );
}

/* Appends an attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_append_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_attribute_t *attribute,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfsntfs_index_t *index = NULL;
	static char *function    = "libfsntfs_mft_entry_append_attribute";
	uint32_t attribute_type  = 0;
	int attribute_has_name   = 0;
	int entry_index          = 0;
	int result               = 0;

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
	if( libcdata_array_append_entry(
	     mft_entry->attributes_array,
	     &entry_index,
	     (intptr_t *) attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append attribute to array.",
		 function );

		return( -1 );
	}
	attribute_has_name = libfsntfs_attribute_has_name(
	                      attribute,
	                      error );

	if( attribute_has_name == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if the attribute has a name.",
		 function );

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
		 "%s: unable to retrieve attribute type.",
		 function );

		return( -1 );
	}
	switch( attribute_type )
	{
		case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
			 if( attribute_has_name != 0 )
			{
				result = libfsntfs_mft_entry_get_index_by_name(
					  mft_entry,
					  ( (libfsntfs_internal_attribute_t *) attribute )->name,
					  ( (libfsntfs_internal_attribute_t *) attribute )->name_size,
					  &index,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve index.",
					 function );

					return( -1 );
				}
				else if( result != 0 )
				{
					/* Only set the bitmap if the index exists
					 */
					if( libfsntfs_index_set_bitmap_attribute(
					     index,
					     attribute,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set bitmap attribute in index.",
						 function );

						return( -1 );
					}
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( libfsntfs_attribute_read_value(
				     attribute,
				     io_handle,
				     file_io_handle,
				     flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read attribute value.",
					 function );

					return( -1 );
				}
			}
#endif
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_DATA:
			if( libfsntfs_mft_entry_append_data_attribute(
			     mft_entry,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append data attribute to MFT entry.",
				 function );

				return( -1 );
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
#if defined( HAVE_DEBUG_OUTPUT )
		case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
#endif
		case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
		case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
		case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
		case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			if( libfsntfs_attribute_read_value(
			     attribute,
			     io_handle,
			     file_io_handle,
			     flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read attribute value.",
				 function );

				return( -1 );
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			if( libfsntfs_mft_entry_append_index_allocation_attribute(
			     mft_entry,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append index allocation attribute to MFT entry.",
				 function );

				return( -1 );
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			if( libfsntfs_mft_entry_append_index_root_attribute(
			     mft_entry,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append index root attribute to MFT entry.",
				 function );

				return( -1 );
			}
			break;

		default:
			break;
	}
	switch( attribute_type )
	{
		case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			if( ( mft_entry->object_identifier_attribute == NULL )
			 && ( attribute_has_name == 0 ) )
			{
				mft_entry->object_identifier_attribute = attribute;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			if( ( mft_entry->reparse_point_attribute == NULL )
			 && ( attribute_has_name == 0 ) )
			{
				mft_entry->reparse_point_attribute = attribute;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			if( ( mft_entry->standard_information_attribute == NULL )
			 && ( attribute_has_name == 0 ) )
			{
				mft_entry->standard_information_attribute = attribute;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
			if( ( mft_entry->security_descriptor_attribute == NULL )
			 && ( attribute_has_name == 0 ) )
			{
				mft_entry->security_descriptor_attribute = attribute;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			if( ( mft_entry->volume_information_attribute == NULL )
			 && ( attribute_has_name == 0 ) )
			{
				mft_entry->volume_information_attribute = attribute;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			if( ( mft_entry->volume_name_attibute == NULL )
			 && ( attribute_has_name == 0 ) )
			{
				mft_entry->volume_name_attibute = attribute;
			}
			break;

		default:
			break;
	}
	return( 1 );
}

/* Appends a $DATA attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_append_data_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *data_attribute = NULL;
	static char *function                 = "libfsntfs_mft_entry_append_data_attribute";
	int attribute_index                   = 0;
	int entry_index                       = 0;
	int result                            = 0;

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
	result = libfsntfs_attribute_has_name(
	          attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if the attribute has a name.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( mft_entry->data_attribute == NULL )
		{
			mft_entry->data_attribute = attribute;
		}
		else
		{
			if( libfsntfs_attribute_append_to_chain(
			     &( mft_entry->data_attribute ),
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to chain attribute.",
				 function );

				return( -1 );
			}
		}
	}
	else
	{
		result = libfsntfs_mft_entry_get_data_attribute_by_name(
			  mft_entry,
			  ( (libfsntfs_internal_attribute_t *) attribute )->name,
			  ( (libfsntfs_internal_attribute_t *) attribute )->name_size,
			  &attribute_index,
			  &data_attribute,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			if( libcdata_array_append_entry(
			     mft_entry->alternate_data_attributes_array,
			     &entry_index,
			     (intptr_t *) attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append alternate data attribute to array.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( libfsntfs_attribute_append_to_chain(
			     &data_attribute,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to chain alternate data attribute.",
				 function );

				return( -1 );
			}
			if( libcdata_array_set_entry_by_index(
			     mft_entry->alternate_data_attributes_array,
			     attribute_index,
			     (intptr_t *) data_attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to append set data attribute: %d in array.",
				 function,
				 attribute_index );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Retrieves a data attribute with the specified name
 * Returns 1 if successful, 0 if no attribute was found or -1 on error
 */
int libfsntfs_mft_entry_get_data_attribute_by_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *name,
     size_t name_size,
     int *attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function    = "libfsntfs_mft_entry_get_data_attribute_by_name";
	int number_of_attributes = 0;

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
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( attribute_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute index.",
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
	if( libcdata_array_get_number_of_entries(
	     mft_entry->alternate_data_attributes_array,
	     &number_of_attributes,
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
	for( *attribute_index = 0;
	     *attribute_index < number_of_attributes;
	     *attribute_index += 1 )
	{
		if( libcdata_array_get_entry_by_index(
		     mft_entry->alternate_data_attributes_array,
		     *attribute_index,
		     (intptr_t **) attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve alternate data attribute: %d.",
			 function,
			 *attribute_index );

			return( -1 );
		}
		if( *attribute == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing alternate data attribute: %d.",
			 function,
			 *attribute_index );

			return( -1 );
		}
		if( ( ( (libfsntfs_internal_attribute_t *) *attribute )->name != NULL )
		 && ( ( (libfsntfs_internal_attribute_t *) *attribute )->name_size == name_size )
		 && ( memory_compare(
		       ( (libfsntfs_internal_attribute_t *) *attribute )->name,
		       name,
		       name_size ) == 0 ) )
		{
			return( 1 );
		}
	}
	*attribute = NULL;

	return( 0 );
}

/* Appends an $INDEX_ALLOCATION attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_append_index_allocation_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	libfsntfs_index_t *index = NULL;
	static char *function    = "libfsntfs_mft_entry_append_index_allocation_attribute";
	int result               = 0;

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
	result = libfsntfs_attribute_has_name(
	          attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if the attribute has a name.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing name.",
		 function );

		return( -1 );
	}
	result = libfsntfs_mft_entry_get_index_by_name(
		  mft_entry,
		  ( (libfsntfs_internal_attribute_t *) attribute )->name,
		  ( (libfsntfs_internal_attribute_t *) attribute )->name_size,
		  &index,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfsntfs_mft_entry_append_index(
		     mft_entry,
		     ( (libfsntfs_internal_attribute_t *) attribute )->name,
		     ( (libfsntfs_internal_attribute_t *) attribute )->name_size,
		     &index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append index to MFT entry.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_index_set_index_allocation_attribute(
	     index,
	     attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index allocation attribute in index.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Appends an $INDEX_ROOT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_append_index_root_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error )
{
	libfsntfs_index_t *index = NULL;
	static char *function    = "libfsntfs_mft_entry_append_index_root_attribute";
	int result               = 0;

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
	result = libfsntfs_attribute_has_name(
	          attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if the attribute has a name.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing name.",
		 function );

		return( -1 );
	}
	result = libfsntfs_mft_entry_get_index_by_name(
		  mft_entry,
		  ( (libfsntfs_internal_attribute_t *) attribute )->name,
		  ( (libfsntfs_internal_attribute_t *) attribute )->name_size,
		  &index,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfsntfs_mft_entry_append_index(
		     mft_entry,
		     ( (libfsntfs_internal_attribute_t *) attribute )->name,
		     ( (libfsntfs_internal_attribute_t *) attribute )->name_size,
		     &index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append index to MFT entry.",
			 function );

			return( -1 );
		}
	}
	if( libfsntfs_index_set_index_root_attribute(
	     index,
	     attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index root attribute in index.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Determines if the file entry has the directory entries ($I30) index
 * Returns 1 if the default data stream, 0 if not or -1 on error
 */
int libfsntfs_mft_entry_has_directory_entries_index(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_has_directory_entries_index";

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
	if( mft_entry->i30_index != NULL )
	{
		return( 1 );
	}
	return( 0 );
}

/* Reads the MFT entry
 * Callback function for the MFT entry vector
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_read_element_data(
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size LIBFSNTFS_ATTRIBUTE_UNUSED,
     uint32_t element_flags,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_mft_entry_t *mft_entry = NULL;
	static char *function            = "libfsntfs_mft_entry_read_element_data";
	uint8_t flags                    = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( element_data_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( element_data_size )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

	if( (uint64_t) element_index > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: element index value out of bounds.",
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
	if( ( element_flags & LIBFSNTFS_MFT_ENTRY_FLAG_MFT_ONLY ) != 0 )
	{
		flags = LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY;
	}
	if( libfsntfs_mft_entry_read(
	     mft_entry,
	     io_handle,
	     file_io_handle,
	     vector,
	     element_data_offset,
	     (uint32_t) element_index,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: %d.",
		 function,
		 element_index );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) mft_entry,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_mft_entry_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set MFT entry as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( -1 );
}

