/*
 * The NTFS index root header functions
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
#include "libfsntfs_index_root_header.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

#include "fsntfs_index.h"

/* Creates an index root header
 * Make sure the value index_root_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_root_header_initialize(
     libfsntfs_index_root_header_t **index_root_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_root_header_initialize";

	if( index_root_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index root header.",
		 function );

		return( -1 );
	}
	if( *index_root_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index root header value already set.",
		 function );

		return( -1 );
	}
	*index_root_header = memory_allocate_structure(
	                      libfsntfs_index_root_header_t );

	if( *index_root_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index root header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index_root_header,
	     0,
	     sizeof( libfsntfs_index_root_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index root header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *index_root_header != NULL )
	{
		memory_free(
		 *index_root_header );

		*index_root_header = NULL;
	}
	return( -1 );
}

/* Frees an index root header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_root_header_free(
     libfsntfs_index_root_header_t **index_root_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_root_header_free";

	if( index_root_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index root header.",
		 function );

		return( -1 );
	}
	if( *index_root_header != NULL )
	{
		memory_free(
		 *index_root_header );

		*index_root_header = NULL;
	}
	return( 1 );
}

/* Reads the index root header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_root_header_read_data(
     libfsntfs_index_root_header_t *index_root_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_root_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( index_root_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index root header.",
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
	if( ( data_size < sizeof( fsntfs_index_root_header_t ) )
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
		 "%s: index root header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsntfs_index_root_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_root_header_t *) data )->attribute_type,
	 index_root_header->attribute_type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_root_header_t *) data )->collation_type,
	 index_root_header->collation_type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_index_root_header_t *) data )->index_entry_size,
	 index_root_header->index_entry_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute type\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 index_root_header->attribute_type,
		 libfsntfs_debug_print_attribute_type(
		  index_root_header->attribute_type ) );

		libcnotify_printf(
		 "%s: collation type\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 index_root_header->collation_type,
		 libfsntfs_debug_print_collation_type(
		  index_root_header->collation_type ) );

		libcnotify_printf(
		 "%s: index entry size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 index_root_header->index_entry_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_index_root_header_t *) data )->index_entry_number_of_cluster_blocks,
		 value_32bit );
		libcnotify_printf(
		 "%s: index entry number of cluster blocks\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Retrieves the index entry size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_root_header_get_index_entry_size(
     libfsntfs_index_root_header_t *index_root_header,
     uint32_t *index_entry_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_root_header_get_index_entry_size";

	if( index_root_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index root header.",
		 function );

		return( -1 );
	}
	if( index_entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry size.",
		 function );

		return( -1 );
	}
	*index_entry_size = index_root_header->index_entry_size;

	return( 1 );
}

/* Retrieves the attribute type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_root_header_get_attribute_type(
     libfsntfs_index_root_header_t *index_root_header,
     uint32_t *attribute_type,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_root_header_get_attribute_type";

	if( index_root_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index root header.",
		 function );

		return( -1 );
	}
	if( attribute_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute type.",
		 function );

		return( -1 );
	}
	*attribute_type = index_root_header->attribute_type;

	return( 1 );
}

/* Retrieves the collation type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_index_root_header_get_collation_type(
     libfsntfs_index_root_header_t *index_root_header,
     uint32_t *collation_type,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_index_root_header_get_collation_type";

	if( index_root_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index root header.",
		 function );

		return( -1 );
	}
	if( collation_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid collation type.",
		 function );

		return( -1 );
	}
	*collation_type = index_root_header->collation_type;

	return( 1 );
}

