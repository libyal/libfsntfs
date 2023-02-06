/*
 * Security descriptor attribute ($SECURITY_DESCRIPTOR) values functions
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_libfwnt.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_security_descriptor_values.h"

/* Creates security descriptor values
 * Make sure the value security_descriptor_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_initialize(
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_initialize";

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
	if( *security_descriptor_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security descriptor values value already set.",
		 function );

		return( -1 );
	}
	*security_descriptor_values = memory_allocate_structure(
	                               libfsntfs_security_descriptor_values_t );

	if( *security_descriptor_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create security descriptor values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *security_descriptor_values,
	     0,
	     sizeof( libfsntfs_security_descriptor_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear security descriptor values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *security_descriptor_values != NULL )
	{
		memory_free(
		 *security_descriptor_values );

		*security_descriptor_values = NULL;
	}
	return( -1 );
}

/* Frees security descriptor values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_free(
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_free";

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
	if( *security_descriptor_values != NULL )
	{
		if( ( *security_descriptor_values )->data != NULL )
		{
			memory_free(
			 ( *security_descriptor_values )->data );
		}
		memory_free(
		 *security_descriptor_values );

		*security_descriptor_values = NULL;
	}
	return( 1 );
}

/* Reads the security descriptor values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_read_data(
     libfsntfs_security_descriptor_values_t *security_descriptor_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                              = "libfsntfs_security_descriptor_values_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	libfwnt_security_descriptor_t *security_descriptor = NULL;
#endif

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: security descriptor data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfwnt_security_descriptor_initialize(
		     &security_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create security descriptor.",
			 function );

			goto on_error;
		}
		if( libfwnt_security_descriptor_copy_from_byte_stream(
		     security_descriptor,
		     data,
		     data_size,
		     LIBFWNT_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy security descriptor from byte stream.",
			 function );

			goto on_error;
		}
		if( libfwnt_security_descriptor_free(
		     &security_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free security descriptor.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

#if defined( HAVE_DEBUG_OUTPUT )
on_error:
	if( security_descriptor != NULL )
	{
		libfwnt_security_descriptor_free(
		 &security_descriptor,
		 NULL );
	}
	return( -1 );
#endif
}

/* Reads the security descriptor values from the buffer
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_read_buffer(
     libfsntfs_security_descriptor_values_t *security_descriptor_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_read_buffer";

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
	if( security_descriptor_values->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security descriptor values - data already set.",
		 function );

		return( -1 );
	}
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
	if( ( data_size < 20 )
	 || ( data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid security descriptor data size value out of bounds.",
		 function );

		goto on_error;
	}
	security_descriptor_values->data = (uint8_t *) memory_allocate(
	                                                sizeof( uint8_t ) * data_size );

	if( security_descriptor_values->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	security_descriptor_values->data_size = data_size;

	if( memory_copy(
	     security_descriptor_values->data,
	     data,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to copy data.",
		 function );

		goto on_error;
	}
	if( libfsntfs_security_descriptor_values_read_data(
	     security_descriptor_values,
	     security_descriptor_values->data,
	     security_descriptor_values->data_size,
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
	return( 1 );

on_error:
	if( security_descriptor_values->data != NULL )
	{
		memory_free(
		 security_descriptor_values->data );

		security_descriptor_values->data = NULL;
	}
	security_descriptor_values->data_size = 0;

	return( -1 );
}

/* Reads the security descriptor values from the data stream
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_read_stream(
     libfsntfs_security_descriptor_values_t *security_descriptor_values,
     libbfio_handle_t *file_io_handle,
     libfdata_stream_t *data_stream,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_read_stream";
	size64_t data_size    = 0;
	ssize_t read_count    = 0;

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
	if( security_descriptor_values->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security descriptor values - data already set.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_size(
	     data_stream,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data stream size.",
		 function );

		goto on_error;
	}
	if( ( data_size < 20 )
	 || ( data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid security descriptor data size value out of bounds.",
		 function );

		goto on_error;
	}
	security_descriptor_values->data = (uint8_t *) memory_allocate(
	                                                sizeof( uint8_t ) * (size_t) data_size );

	if( security_descriptor_values->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	security_descriptor_values->data_size = (size_t) data_size;

	read_count = libfdata_stream_read_buffer(
	              data_stream,
	              (intptr_t *) file_io_handle,
	              security_descriptor_values->data,
	              security_descriptor_values->data_size,
	              0,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read security descriptor data.",
		 function );

		goto on_error;
	}
	if( libfsntfs_security_descriptor_values_read_data(
	     security_descriptor_values,
	     security_descriptor_values->data,
	     security_descriptor_values->data_size,
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
	return( 1 );

on_error:
	if( security_descriptor_values->data != NULL )
	{
		memory_free(
		 security_descriptor_values->data );

		security_descriptor_values->data = NULL;
	}
	security_descriptor_values->data_size = 0;

	return( -1 );
}

/* Reads the security descriptor values from an MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_read_from_mft_attribute(
     libfsntfs_security_descriptor_values_t *security_descriptor_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfdata_stream_t *cluster_block_stream = NULL;
	uint8_t *data                           = NULL;
	static char *function                   = "libfsntfs_security_descriptor_values_read_from_mft_attribute";
	size_t data_size                        = 0;
	uint32_t attribute_type                 = 0;
	uint16_t attribute_data_flags           = 0;
	int result                              = 0;

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
	if( libfsntfs_mft_attribute_get_type(
	     mft_attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type from attribute.",
		 function );

		goto on_error;
	}
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		goto on_error;
	}
	result = libfsntfs_mft_attribute_data_is_resident(
	          mft_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if attribute data is resident.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsntfs_mft_attribute_get_resident_data(
		     mft_attribute,
		     &data,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resident data from attribute.",
			 function );

			goto on_error;
		}
		if( libfsntfs_security_descriptor_values_read_buffer(
		     security_descriptor_values,
		     data,
		     data_size,
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
	}
	else if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
	{
		if( libfsntfs_mft_attribute_get_data_flags(
		     mft_attribute,
		     &attribute_data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data flags.",
			 function );

			goto on_error;
		}
		if( ( attribute_data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compressed attribute data.",
			 function );

			goto on_error;
		}
		if( libfsntfs_cluster_block_stream_initialize(
		     &cluster_block_stream,
		     io_handle,
		     mft_attribute,
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
		if( libfsntfs_security_descriptor_values_read_stream(
		     security_descriptor_values,
		     file_io_handle,
		     cluster_block_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read security descriptor values from stream.",
			 function );

			goto on_error;
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
	}
	return( 1 );

on_error:
	if( cluster_block_stream != NULL )
	{
		libfdata_stream_free(
		 &cluster_block_stream,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the security descriptor data size
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_security_descriptor_values_get_data_size(
     libfsntfs_security_descriptor_values_t *security_descriptor_values,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_get_data_size";

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
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( security_descriptor_values->data == NULL )
	{
		return( 0 );
	}
	*data_size = security_descriptor_values->data_size;

	return( 1 );
}

/* Retrieves the security descriptor data size
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_security_descriptor_values_get_data(
     libfsntfs_security_descriptor_values_t *security_descriptor_values,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_get_data";

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
	if( data_size < security_descriptor_values->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( security_descriptor_values->data == NULL )
	{
		return( 0 );
	}
	if( memory_copy(
	     data,
	     security_descriptor_values->data,
	     security_descriptor_values->data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to copy security descriptor data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

