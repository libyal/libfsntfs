/*
 * Security descriptor index functions
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

#include "libfsntfs_data_stream.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_index.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_security_descriptor_index.h"
#include "libfsntfs_security_descriptor_index_value.h"
#include "libfsntfs_security_descriptor_values.h"
#include "libfsntfs_sds_index_value.h"
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
     libfsntfs_mft_attribute_t *data_attribute,
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
		 *security_descriptor_index );

		*security_descriptor_index = NULL;

		return( -1 );
	}
/* TODO move out of index ? */
	if( libfsntfs_data_stream_initialize(
	     &( ( *security_descriptor_index )->data_stream ),
	     io_handle,
	     file_io_handle,
	     data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create $SDS data stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *security_descriptor_index != NULL )
	{
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
		if( ( *security_descriptor_index )->sii_index != NULL )
		{
			if( libfsntfs_index_free(
			     &( ( *security_descriptor_index )->sii_index ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free $SII index.",
				 function );

				result = -1;
			}
		}
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

/* Reads the security descriptor identifier ($SII) index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_index_read_sii_index(
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error )
{
	static char *function   = "libfsntfs_security_descriptor_index_read_sii_index";
	uint32_t attribute_type = 0;
	uint32_t collation_type = 0;
	int result              = 0;

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
	if( security_descriptor_index->sii_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security descriptor index - $SII index value already set.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_initialize(
	     &( security_descriptor_index->sii_index ),
	     io_handle,
	     (uint8_t *) "$SII",
	     5,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create $SII index.",
		 function );

		goto on_error;
	}
	result = libfsntfs_index_read(
	          security_descriptor_index->sii_index,
	          file_io_handle,
	          mft_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read $SII index.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsntfs_index_get_attribute_type(
		     security_descriptor_index->sii_index,
		     &attribute_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute type from index.",
			 function );

			goto on_error;
		}
		if( attribute_type != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported index attribute type.",
			 function );

			goto on_error;
		}
		if( libfsntfs_index_get_collation_type(
		     security_descriptor_index->sii_index,
		     &collation_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve collation type from index.",
			 function );

			goto on_error;
		}
		if( collation_type != 16 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported index collation type.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( security_descriptor_index->sii_index != NULL )
	{
		libfsntfs_index_free(
		 &( security_descriptor_index->sii_index ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the security descriptor from an index node for a specific identifier
 * This function creates new security descriptor values
 * Returns 1 if successful, 0 if no such security descriptor or -1 on error
 */
int libfsntfs_security_descriptor_index_get_entry_from_index_node_by_identifier(
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     uint32_t security_descriptor_identifier,
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     int recursion_depth,
     libcerror_error_t **error )
{
	uint8_t secure_index_value_data[ sizeof( fsntfs_secure_index_value_t ) ];

	libfsntfs_index_node_t *sub_node                                             = NULL;
	libfsntfs_index_value_t *index_value                                         = NULL;
	libfsntfs_sds_index_value_t *sds_index_value                                 = NULL;
	libfsntfs_security_descriptor_index_value_t *security_descriptor_index_value = NULL;
	libfsntfs_security_descriptor_values_t *safe_security_descriptor_values      = NULL;
	static char *function                                                        = "libfsntfs_security_descriptor_index_get_entry_from_index_node_by_identifier";
	size_t security_descriptor_data_size                                         = 0;
	ssize_t read_count                                                           = 0;
	off64_t index_entry_offset                                                   = 0;
	int compare_result                                                           = 0;
	int index_value_entry                                                        = 0;
	int is_allocated                                                             = 0;
	int number_of_index_values                                                   = 0;
	int result                                                                   = 0;

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
	if( security_descriptor_index->sii_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor index - missing $SII index.",
		 function );

		return( -1 );
	}
	if( security_descriptor_index->sii_index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor index - invalid $SII index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( security_descriptor_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor values.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSNTFS_MAXIMUM_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_node_get_number_of_values(
	     index_node,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values from index node.",
		 function );

		goto on_error;
	}
	for( index_value_entry = 0;
	     index_value_entry < number_of_index_values;
	     index_value_entry++ )
	{
		if( libfsntfs_index_node_get_value_by_index(
		     index_node,
		     index_value_entry,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d from index node.",
			 function,
			 index_value_entry );

			goto on_error;
		}
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
		{
			if( index_value->sub_node_vcn > (uint64_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: node index value: %d sub node VCN value out of bounds.",
				 function,
				 index_value_entry );

				goto on_error;
			}
			is_allocated = libfsntfs_index_sub_node_is_allocated(
			                security_descriptor_index->sii_index,
			                (int) index_value->sub_node_vcn,
			                error );

			if( is_allocated == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if sub node with VCN: %d is allocated.",
				 function,
				 (int) index_value->sub_node_vcn );

				goto on_error;
			}
			else if( is_allocated == 0 )
			{
				continue;
			}
		}
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
		{
			break;
		}
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
		if( libfsntfs_security_descriptor_index_value_read_data(
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
		if( security_descriptor_identifier < security_descriptor_index_value->identifier )
		{
			compare_result = LIBCDATA_COMPARE_LESS;
		}
		else if( security_descriptor_identifier > security_descriptor_index_value->identifier )
		{
			compare_result = LIBCDATA_COMPARE_GREATER;
		}
		else
		{
			compare_result = LIBCDATA_COMPARE_EQUAL;
		}
		if( compare_result != LIBCDATA_COMPARE_EQUAL )
		{
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
		if( compare_result == LIBCDATA_COMPARE_LESS )
		{
			if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
			{
				break;
			}
		}
		else if( compare_result == LIBCDATA_COMPARE_EQUAL )
		{
			break;
		}
	}
	if( compare_result == LIBCDATA_COMPARE_EQUAL )
	{
		if( security_descriptor_index_value->data_size < (size64_t) ( sizeof( fsntfs_secure_index_value_t ) + 20 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported security descriptor stream ($SDS) data size: %" PRIu64 "\n",
			 function,
			 security_descriptor_index_value->data_size );

			return( -1 );
		}
		read_count = libfsntfs_data_stream_read_buffer_at_offset(
		              security_descriptor_index->data_stream,
		              secure_index_value_data,
		              sizeof( fsntfs_secure_index_value_t ),
		              (off64_t) security_descriptor_index_value->data_offset,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read security descriptor stream ($SDS) data at offset: 0x%08" PRIx64 ".",
			 function,
			 security_descriptor_index_value->data_offset );

			goto on_error;
		}
		if( libfsntfs_sds_index_value_initialize(
		     &sds_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create $SDS index value.",
			 function );

			goto on_error;
		}
		if( libfsntfs_sds_index_value_read_data(
		     sds_index_value,
		     secure_index_value_data,
		     sizeof( fsntfs_secure_index_value_t ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read $SDS index value data.",
			 function );

			goto on_error;
		}
/* TODO check index values against secure_index_value */

		if( libfsntfs_security_descriptor_values_initialize(
		     &safe_security_descriptor_values,
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
		security_descriptor_data_size = (size_t) ( security_descriptor_index_value->data_size - sizeof( fsntfs_secure_index_value_t ) );

		if( ( security_descriptor_data_size == 0 )
		 || ( security_descriptor_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid security descriptor values data value out of bounds.",
			 function,
			 index_value_entry );

			goto on_error;
		}
		safe_security_descriptor_values->data = memory_allocate(
		                                         sizeof( uint8_t ) * security_descriptor_data_size );

		if( safe_security_descriptor_values->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create security descriptor values data.",
			 function );

			goto on_error;
		}
		safe_security_descriptor_values->data_size = security_descriptor_data_size;

		if( memory_set(
		     safe_security_descriptor_values->data,
		     0,
		     sizeof( uint8_t ) * safe_security_descriptor_values->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear security descriptor data.",
			 function );

			goto on_error;
		}
		read_count = libfsntfs_data_stream_read_buffer(
		              security_descriptor_index->data_stream,
		              safe_security_descriptor_values->data,
		              safe_security_descriptor_values->data_size,
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
		if( libfsntfs_security_descriptor_values_read_data(
		     safe_security_descriptor_values,
		     safe_security_descriptor_values->data,
		     safe_security_descriptor_values->data_size,
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
		*security_descriptor_values = safe_security_descriptor_values;

		result = 1;
	}
	else if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
	{
		index_entry_offset = (off64_t) ( index_value->sub_node_vcn * security_descriptor_index->sii_index->io_handle->cluster_block_size );

		if( libfsntfs_index_get_sub_node(
		     security_descriptor_index->sii_index,
		     file_io_handle,
		     security_descriptor_index->sii_index->index_node_cache,
		     index_entry_offset,
		     (int) index_value->sub_node_vcn,
		     &sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node with VCN: %d at offset: 0x%08" PRIx64 ".",
			 function,
			 (int) index_value->sub_node_vcn,
			 index_entry_offset );

			goto on_error;
		}
		result = libfsntfs_security_descriptor_index_get_entry_from_index_node_by_identifier(
		          security_descriptor_index,
		          file_io_handle,
		          security_descriptor_index->sii_index->root_node,
		          security_descriptor_identifier,
		          security_descriptor_values,
		          recursion_depth + 1,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to retrieve security descriptor by identifier from index entry with VCN: %d at offset: 0x%08" PRIx64 ".",
			 function,
			 (int) index_value->sub_node_vcn,
			 index_entry_offset );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( safe_security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &safe_security_descriptor_values,
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

/* Retrieves the security descriptor for a specific identifier
 * This function creates new security descriptor values
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_security_descriptor_index_get_entry_by_identifier(
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libbfio_handle_t *file_io_handle,
     uint32_t security_descriptor_identifier,
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_index_get_entry_by_identifier";
	int result            = 0;

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
	result = libfsntfs_security_descriptor_index_get_entry_from_index_node_by_identifier(
	          security_descriptor_index,
	          file_io_handle,
	          security_descriptor_index->sii_index->root_node,
	          security_descriptor_identifier,
	          security_descriptor_values,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor by identifier.",
		 function );

		return( -1 );
	}
	return( result );
}

