/*
 * Security descriptor attribute ($SECURITY_DESCRIPTOR) values functions
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_libfwnt.h"
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
int libfsntfs_security_descriptor_values_read(
     libfsntfs_security_descriptor_values_t *security_descriptor_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                              = "libfsntfs_security_descriptor_values_read";

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
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( security_descriptor != NULL )
	{
		libfwnt_security_descriptor_free(
		 &security_descriptor,
		 NULL );
	}
#endif
	return( -1 );
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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( data_size < 20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported security descriptor data size: %" PRIzd "\n",
		 function,
		 data_size );

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
	if( libfsntfs_security_descriptor_values_read(
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
	if( data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( data_size < 20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported security descriptor data size: %" PRIzd "\n",
		 function,
		 data_size );

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
	if( libfsntfs_security_descriptor_values_read(
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
	}
	security_descriptor_values->data_size = 0;

	return( -1 );
}

/* Retrieves the security descriptor data size
 * Returns 1 if successful or -1 on error
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
	*data_size = security_descriptor_values->data_size;

	return( 1 );
}

/* Retrieves the security descriptor data size
 * Returns 1 if successful or -1 on error
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

