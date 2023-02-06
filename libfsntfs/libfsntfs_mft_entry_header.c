/*
 * Master File Table (MFT) entry header functions
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

#include "libfsntfs_debug.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_mft_entry_header.h"

#include "fsntfs_mft_entry.h"

/* Creates MFT entry header
 * Make sure the value mft_entry_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_initialize(
     libfsntfs_mft_entry_header_t **mft_entry_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_initialize";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( *mft_entry_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT entry header value already set.",
		 function );

		return( -1 );
	}
	*mft_entry_header = memory_allocate_structure(
	                     libfsntfs_mft_entry_header_t );

	if( *mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT entry header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mft_entry_header,
	     0,
	     sizeof( libfsntfs_mft_entry_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MFT entry header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *mft_entry_header != NULL )
	{
		memory_free(
		 *mft_entry_header );

		*mft_entry_header = NULL;
	}
	return( -1 );
}

/* Frees MFT entry header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_free(
     libfsntfs_mft_entry_header_t **mft_entry_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_free";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( *mft_entry_header != NULL )
	{
		memory_free(
		 *mft_entry_header );

		*mft_entry_header = NULL;
	}
	return( 1 );
}

/* Reads the MFT entry header
 * Returns 1 if successful, 0 if FILE signature is not present or -1 on error
 */
int libfsntfs_mft_entry_header_read_data(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsntfs_mft_entry_header_read_data";
	size_t header_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit    = 0;
#endif

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
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
	if( ( data_size < 2 )
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
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->fixup_values_offset,
	 mft_entry_header->fixup_values_offset );

	if( mft_entry_header->fixup_values_offset > 42 )
	{
		header_data_size = sizeof( fsntfs_mft_entry_header_t );
	}
	else
	{
		header_data_size = 42;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MFT entry header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_data_size,
		 0 );
	}
#endif
	if( data_size < header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_compare(
	     ( (fsntfs_mft_entry_header_t *) data )->signature,
	     "BAAD",
	     4 ) == 0 )
	{
		mft_entry_header->is_bad = 1;

		return( 0 );
	}
	mft_entry_header->is_bad = 0;

	if( memory_compare(
	     ( (fsntfs_mft_entry_header_t *) data )->signature,
	     "FILE",
	     4 ) != 0 )
	{
		return( 0 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->number_of_fixup_values,
	 mft_entry_header->number_of_fixup_values );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->journal_sequence_number,
	 mft_entry_header->journal_sequence_number );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->sequence,
	 mft_entry_header->sequence );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->reference_count,
	 mft_entry_header->reference_count );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->attributes_offset,
	 mft_entry_header->attributes_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->flags,
	 mft_entry_header->flags );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->used_entry_size,
	 mft_entry_header->used_entry_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->total_entry_size,
	 mft_entry_header->total_entry_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_mft_entry_header_t *) data )->base_record_file_reference,
	 mft_entry_header->base_record_file_reference );

	if( header_data_size > 42 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_mft_entry_header_t *) data )->index,
		 mft_entry_header->index );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsntfs_mft_entry_header_t *) data )->signature[ 0 ],
		 ( (fsntfs_mft_entry_header_t *) data )->signature[ 1 ],
		 ( (fsntfs_mft_entry_header_t *) data )->signature[ 2 ],
		 ( (fsntfs_mft_entry_header_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: fix-up values offset\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry_header->fixup_values_offset );

		libcnotify_printf(
		 "%s: number of fix-up values\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry_header->number_of_fixup_values );

		libcnotify_printf(
		 "%s: journal sequence number\t\t\t: %" PRIu64 "\n",
		 function,
		 mft_entry_header->journal_sequence_number );

		libcnotify_printf(
		 "%s: sequence\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry_header->sequence );

		libcnotify_printf(
		 "%s: reference count\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry_header->reference_count );

		libcnotify_printf(
		 "%s: attributes offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry_header->attributes_offset );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 mft_entry_header->flags );
		libfsntfs_debug_print_mft_entry_flags(
		 mft_entry_header->flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: used entry size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry_header->used_entry_size );

		libcnotify_printf(
		 "%s: total entry size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 mft_entry_header->total_entry_size );

		libcnotify_printf(
		 "%s: base record file reference\t\t: %" PRIu64 "-%" PRIu64 "\n",
		 function,
		 mft_entry_header->base_record_file_reference & 0xffffffffffffUL,
		 mft_entry_header->base_record_file_reference >> 48 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_mft_entry_header_t *) data )->first_available_attribute_identifier,
		 value_16bit );
		libcnotify_printf(
		 "%s: first available attribute identifier\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		if( header_data_size > 42 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsntfs_mft_entry_header_t *) data )->unknown1,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%04" PRIu16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: index\t\t\t\t\t: %" PRIu32 "\n",
			 function,
			 mft_entry_header->index );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( mft_entry_header->fixup_values_offset < header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid fix-up values offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( mft_entry_header->attributes_offset < header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid attributes offset value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the fix-up values offset
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_get_fixup_values_offset(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *fixup_values_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_get_fixup_values_offset";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( fixup_values_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fix-up values offset.",
		 function );

		return( -1 );
	}
	*fixup_values_offset = mft_entry_header->fixup_values_offset;

	return( 1 );
}

/* Retrieves the number of fix-up values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_get_number_of_fixup_values(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *number_of_fixup_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_get_number_of_fixup_values";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( number_of_fixup_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of fix-up values.",
		 function );

		return( -1 );
	}
	*number_of_fixup_values = mft_entry_header->number_of_fixup_values;

	return( 1 );
}

/* Retrieves the journal sequence number
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_get_journal_sequence_number(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint64_t *journal_sequence_number,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_get_journal_sequence_number";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
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
	*journal_sequence_number = mft_entry_header->journal_sequence_number;

	return( 1 );
}

/* Retrieves the reference count
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_get_reference_count(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *reference_count,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_get_reference_count";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( reference_count == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reference count.",
		 function );

		return( -1 );
	}
	*reference_count = mft_entry_header->reference_count;

	return( 1 );
}

/* Retrieves the attributes offset
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_get_attributes_offset(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *attributes_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_get_attributes_offset";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( attributes_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes offset.",
		 function );

		return( -1 );
	}
	*attributes_offset = mft_entry_header->attributes_offset;

	return( 1 );
}

/* Retrieves the used entry size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_get_used_entry_size(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *used_entry_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_get_used_entry_size";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( used_entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid used entry size.",
		 function );

		return( -1 );
	}
	*used_entry_size = mft_entry_header->used_entry_size;

	return( 1 );
}

/* Retrieves the total entry size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_get_total_entry_size(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *total_entry_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_get_total_entry_size";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( total_entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid total entry size.",
		 function );

		return( -1 );
	}
	*total_entry_size = mft_entry_header->total_entry_size;

	return( 1 );
}

/* Retrieves the base record file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_entry_header_get_base_record_file_reference(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint64_t *base_record_file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_header_get_base_record_file_reference";

	if( mft_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry header.",
		 function );

		return( -1 );
	}
	if( base_record_file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base record file reference.",
		 function );

		return( -1 );
	}
	*base_record_file_reference = mft_entry_header->base_record_file_reference;

	return( 1 );
}

