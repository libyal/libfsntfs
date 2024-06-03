/*
 * Attribute list attribute ($ATTRIBUTE_LIST) functions
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_mft_attribute_list.h"
#include "libfsntfs_mft_attribute_list_entry.h"

#include "fsntfs_mft_attribute_list.h"

/* Creates attribute list
 * Make sure the value attribute_list is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_initialize(
     libfsntfs_mft_attribute_list_t **attribute_list,
     uint64_t base_record_file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_initialize";

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
		 function );

		return( -1 );
	}
	if( *attribute_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute list value already set.",
		 function );

		return( -1 );
	}
	*attribute_list = memory_allocate_structure(
	                   libfsntfs_mft_attribute_list_t );

	if( *attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attribute_list,
	     0,
	     sizeof( libfsntfs_mft_attribute_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute list.",
		 function );

		memory_free(
		 *attribute_list );

		*attribute_list = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *attribute_list )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *attribute_list )->file_references_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file references array.",
		 function );

		goto on_error;
	}
	( *attribute_list )->base_record_file_reference = base_record_file_reference;

	return( 1 );

on_error:
	if( *attribute_list != NULL )
	{
		if( ( *attribute_list )->entries_array != NULL )
		{
			libcdata_array_free(
			 &( ( *attribute_list )->entries_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 *attribute_list );

		*attribute_list = NULL;
	}
	return( -1 );
}

/* Frees attribute list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_free(
     libfsntfs_mft_attribute_list_t **attribute_list,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_free";
	int result            = 1;

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
		 function );

		return( -1 );
	}
	if( *attribute_list != NULL )
	{
		if( libcdata_array_free(
		     &( ( *attribute_list )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_mft_attribute_list_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free entries array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *attribute_list )->file_references_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_mft_attribute_list_file_reference_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file references array.",
			 function );

			result = -1;
		}
		memory_free(
		 *attribute_list );

		*attribute_list = NULL;
	}
	return( result );
}

/* Frees attribute list file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_file_reference_free(
     uint64_t **file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_file_reference_free";

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
	if( *file_reference != NULL )
	{
		memory_free(
		 *file_reference );

		*file_reference = NULL;
	}
	return( 1 );
}

/* Reads the attribute list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_read_data(
     libfsntfs_mft_attribute_list_t *attribute_list,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsntfs_mft_attribute_list_entry_t *mft_attribute_list_entry = NULL;
	static char *function                                          = "libfsntfs_mft_attribute_list_read_data";
	size_t data_offset                                             = 0;
	int attribute_index                                            = 0;
	int entry_index                                                = 0;

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
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
	if( data_size > (size_t) SSIZE_MAX )
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
		 "%s: attribute list data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	while( data_offset < data_size )
	{
		if( libfsntfs_mft_attribute_list_entry_initialize(
		     &mft_attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create MFT attribute list entry.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_attribute_list_entry_read_data(
		     mft_attribute_list_entry,
		     &( data[ data_offset ] ),
		     data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read MFT attribute list entry: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		data_offset += mft_attribute_list_entry->size;

		if( libcdata_array_append_entry(
		     attribute_list->entries_array,
		     &entry_index,
		     (intptr_t *) mft_attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append MFT attribute list entry: %d to array.",
			 function,
			 attribute_index );

			goto on_error;
		}
		mft_attribute_list_entry = NULL;

		attribute_index++;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < data_size )
		{
			libcnotify_printf(
			 "%s: alignment padding:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 data_size - data_offset,
			 0 );
		}
	}
#endif
	return( 1 );

on_error:
	if( mft_attribute_list_entry != NULL )
	{
		libfsntfs_mft_attribute_list_entry_free(
		 &mft_attribute_list_entry,
		 NULL );
	}
	return( -1 );
}

/* Reads the attribute list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_read_from_attribute(
     libfsntfs_mft_attribute_list_t *attribute_list,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_attribute_t *list_attribute,
     libcerror_error_t **error )
{
	uint8_t data[ sizeof( fsntfs_mft_attribute_list_entry_header_t ) + 256 ];

	libfdata_stream_t *cluster_block_stream                        = NULL;
	libfsntfs_mft_attribute_list_entry_t *mft_attribute_list_entry = NULL;
	static char *function                                          = "libfsntfs_mft_attribute_list_read_from_attribute";
	size64_t data_size                                             = 0;
	ssize_t read_count                                             = 0;
	off64_t data_offset                                            = 0;
	int attribute_index                                            = 0;
	int entry_index                                                = 0;

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
		 function );

		return( -1 );
	}
	if( libfsntfs_cluster_block_stream_initialize(
	     &cluster_block_stream,
	     io_handle,
	     list_attribute,
	     NULL,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block stream.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_get_size(
	     cluster_block_stream,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size from cluster block stream.",
		 function );

		goto on_error;
	}
	while( (size64_t) data_offset < data_size )
	{
		if( memory_set(
		     data,
		     0,
		     sizeof( fsntfs_mft_attribute_list_entry_header_t ) + 256 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.",
			 function );

			goto on_error;
		}
		read_count = libfdata_stream_read_buffer_at_offset(
		              cluster_block_stream,
		              (intptr_t *) file_io_handle,
		              data,
		              sizeof( fsntfs_mft_attribute_list_entry_header_t ) + 256,
		              data_offset,
		              0,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute list entry: %d from cluster block stream at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 attribute_index,
			 data_offset,
			 data_offset );

			goto on_error;
		}
		if( libfsntfs_mft_attribute_list_entry_initialize(
		     &mft_attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute list entry: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		if( libfsntfs_mft_attribute_list_entry_read_data(
		     mft_attribute_list_entry,
		     data,
		     sizeof( fsntfs_mft_attribute_list_entry_header_t ) + 256,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute list entry: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		data_offset += mft_attribute_list_entry->size;

		if( libcdata_array_append_entry(
		     attribute_list->entries_array,
		     &entry_index,
		     (intptr_t *) mft_attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append attribute list entry: %d to array.",
			 function,
			 attribute_index );

			goto on_error;
		}
		mft_attribute_list_entry = NULL;

		attribute_index++;
	}
	if( libfdata_stream_free(
	     &cluster_block_stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( mft_attribute_list_entry != NULL )
	{
		libfsntfs_mft_attribute_list_entry_free(
		 &mft_attribute_list_entry,
		 NULL );
	}
	if( cluster_block_stream != NULL )
	{
		libfdata_stream_free(
		 &cluster_block_stream,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of attribute list entries
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_get_number_of_entries(
     libfsntfs_mft_attribute_list_t *attribute_list,
     int *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_get_number_of_entries";

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     attribute_list->entries_array,
	     number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific attribute list entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_get_entry_by_index(
     libfsntfs_mft_attribute_list_t *attribute_list,
     int entry_index,
     libfsntfs_mft_attribute_list_entry_t **mft_attribute_list_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_entry_get_attribute_by_index";

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     attribute_list->entries_array,
	     entry_index,
	     (intptr_t **) mft_attribute_list_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from array.",
		 function,
		 entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Compares attribute lists by their base record file reference
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_compare_by_base_record_file_reference(
     libfsntfs_mft_attribute_list_t *first_attribute_list,
     libfsntfs_mft_attribute_list_t *second_attribute_list,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_mft_attribute_list_compare_by_base_record_file_reference";
	uint64_t first_mft_entry_index  = 0;
	uint64_t second_mft_entry_index = 0;

	if( first_attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first attribute list.",
		 function );

		return( -1 );
	}
	if( second_attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second attribute list.",
		 function );

		return( -1 );
	}
	first_mft_entry_index  = first_attribute_list->base_record_file_reference & 0xffffffffffffUL;
	second_mft_entry_index = second_attribute_list->base_record_file_reference & 0xffffffffffffUL;

	if( first_mft_entry_index < second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_mft_entry_index > second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Retrieves the number of attribute list data file references
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_get_number_of_file_references(
     libfsntfs_mft_attribute_list_t *attribute_list,
     int *number_of_file_references,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_attribute_list_get_number_of_file_references";

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     attribute_list->file_references_array,
	     number_of_file_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific attribute list data file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_get_file_reference_by_index(
     libfsntfs_mft_attribute_list_t *attribute_list,
     int file_reference_index,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	uint64_t *safe_file_reference = NULL;
	static char *function         = "libfsntfs_mft_attribute_list_get_file_reference_by_index";

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
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
	if( libcdata_array_get_entry_by_index(
	     attribute_list->file_references_array,
	     file_reference_index,
	     (intptr_t **) &safe_file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from array.",
		 function,
		 file_reference_index );

		return( -1 );
	}
	*file_reference = *safe_file_reference;

	return( 1 );
}

/* Compares attribute list data file references
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_compare_file_reference(
     uint64_t *first_file_reference,
     uint64_t *second_file_reference,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_mft_attribute_list_compare_file_reference";
	uint64_t first_mft_entry_index  = 0;
	uint64_t second_mft_entry_index = 0;

	if( first_file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first file reference.",
		 function );

		return( -1 );
	}
	if( second_file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second file reference.",
		 function );

		return( -1 );
	}
	first_mft_entry_index  = *first_file_reference & 0xffffffffffffUL;
	second_mft_entry_index = *second_file_reference & 0xffffffffffffUL;

	if( first_mft_entry_index < second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_mft_entry_index > second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Insert an attribute list data file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_attribute_list_insert_file_reference(
     libfsntfs_mft_attribute_list_t *attribute_list,
     uint64_t file_reference,
     libcerror_error_t **error )
{
	uint64_t *safe_file_reference = NULL;
	static char *function         = "libfsntfs_mft_attribute_list_insert_file_reference";
	int entry_index               = 0;
	int result                    = 0;
	intptr_t* entry_find          = NULL;

	if( attribute_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list.",
		 function );

		return( -1 );
	}
	safe_file_reference = (uint64_t *) memory_allocate(
	                                    sizeof( uint64_t ) );

	if( safe_file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file reference.",
		 function );

		goto on_error;
	}
	*safe_file_reference = file_reference;

	result = libcdata_array_get_entry_by_value(
				attribute_list->file_references_array,
				(intptr_t *) safe_file_reference,
				(int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_mft_attribute_list_compare_file_reference,
				&entry_find,
				error
				);
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to find file reference in array.",
		 function );

		goto on_error;
	}
	else if( result == 1 )
	{
		memory_free(
		 safe_file_reference );
		return( 1 );
	}

	result = libcdata_array_append_entry(
	          attribute_list->file_references_array,
	          &entry_index,
	          (intptr_t *) safe_file_reference,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append file reference in array.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		memory_free(
		 safe_file_reference );
	}
	return( 1 );

on_error:
	if( safe_file_reference != NULL )
	{
		memory_free(
		 safe_file_reference );
	}
	return( -1 );
}

