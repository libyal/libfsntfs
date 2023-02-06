/*
 * The NTFS index entry header functions
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
#include "libfsntfs_index_entry_header.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

#include "fsntfs_index.h"

const char fsntfs_index_entry_signature[ 4 ] = { 'I', 'N', 'D', 'X' };

/* Creates an index entry header
 * Make sure the value index_entry_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_header_initialize(
     libfsntfs_index_entry_header_t **index_entry_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_entry_header_initialize";

	if( index_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry header.",
		 function );

		return( -1 );
	}
	if( *index_entry_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index entry header value already set.",
		 function );

		return( -1 );
	}
	*index_entry_header = memory_allocate_structure(
	                       libfsntfs_index_entry_header_t );

	if( *index_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index entry header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index_entry_header,
	     0,
	     sizeof( libfsntfs_index_entry_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index entry header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *index_entry_header != NULL )
	{
		memory_free(
		 *index_entry_header );

		*index_entry_header = NULL;
	}
	return( -1 );
}

/* Frees an index entry header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_header_free(
     libfsntfs_index_entry_header_t **index_entry_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_entry_header_free";

	if( index_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry header.",
		 function );

		return( -1 );
	}
	if( *index_entry_header != NULL )
	{
		memory_free(
		 *index_entry_header );

		*index_entry_header = NULL;
	}
	return( 1 );
}

/* Reads the index entry header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_header_read_data(
     libfsntfs_index_entry_header_t *index_entry_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_entry_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( index_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry header.",
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
	if( ( data_size < sizeof( fsntfs_index_entry_header_t ) )
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index entry header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsntfs_index_entry_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (fsntfs_index_entry_header_t *) data )->signature,
	     fsntfs_index_entry_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid index entry signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_index_entry_header_t *) data )->fixup_values_offset,
	 index_entry_header->fixup_values_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_index_entry_header_t *) data )->number_of_fixup_values,
	 index_entry_header->number_of_fixup_values );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsntfs_index_entry_header_t *) data )->signature[ 0 ],
		 ( (fsntfs_index_entry_header_t *) data )->signature[ 1 ],
		 ( (fsntfs_index_entry_header_t *) data )->signature[ 2 ],
		 ( (fsntfs_index_entry_header_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: fixup values offset\t\t: %" PRIu16 "\n",
		 function,
		 index_entry_header->fixup_values_offset );

		libcnotify_printf(
		 "%s: number of fixup values\t\t: %" PRIu16 "\n",
		 function,
		 index_entry_header->number_of_fixup_values );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_index_entry_header_t *) data )->journal_sequence_number,
		 value_64bit );
		libcnotify_printf(
		 "%s: journal sequence number\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_index_entry_header_t *) data )->vcn,
		 value_64bit );
		libcnotify_printf(
		 "%s: VCN\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( index_entry_header->fixup_values_offset < ( sizeof( fsntfs_index_entry_header_t ) + sizeof( fsntfs_index_node_header_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid fix-up values offset value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the fix-up values offset
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_header_get_fixup_values_offset(
     libfsntfs_index_entry_header_t *index_entry_header,
     uint16_t *fixup_values_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_entry_header_get_fixup_values_offset";

	if( index_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry header.",
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
	*fixup_values_offset = index_entry_header->fixup_values_offset;

	return( 1 );
}

/* Retrieves the number of fix-up values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_entry_header_get_number_of_fixup_values(
     libfsntfs_index_entry_header_t *index_entry_header,
     uint16_t *number_of_fixup_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_entry_header_get_number_of_fixup_values";

	if( index_entry_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry header.",
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
	*number_of_fixup_values = index_entry_header->number_of_fixup_values;

	return( 1 );
}

