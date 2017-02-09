/*
 * The Transactional NTFS (TxF) data ($TXF_DATA) logged utility stream
 * attribute ($LOGGED_UTILITY_STREAM) values functions
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
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_txf_data_values.h"

#include "fsntfs_txf_data.h"

/* Creates TxF data values
 * Make sure the value txf_data_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_txf_data_values_initialize(
     libfsntfs_txf_data_values_t **txf_data_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_txf_data_values_initialize";

	if( txf_data_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid TxF data values.",
		 function );

		return( -1 );
	}
	if( *txf_data_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid TxF data values value already set.",
		 function );

		return( -1 );
	}
	*txf_data_values = memory_allocate_structure(
	                    libfsntfs_txf_data_values_t );

	if( *txf_data_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create TxF data values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *txf_data_values,
	     0,
	     sizeof( libfsntfs_txf_data_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear TxF data values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *txf_data_values != NULL )
	{
		memory_free(
		 *txf_data_values );

		*txf_data_values = NULL;
	}
	return( -1 );
}

/* Frees TxF data values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_txf_data_values_free(
     libfsntfs_txf_data_values_t **txf_data_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_txf_data_values_free";

	if( txf_data_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid TxF data values.",
		 function );

		return( -1 );
	}
	if( *txf_data_values != NULL )
	{
		memory_free(
		 *txf_data_values );

		*txf_data_values = NULL;
	}
	return( 1 );
}

/* Reads the TxF data values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_txf_data_values_read(
     libfsntfs_txf_data_values_t *txf_data_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_txf_data_values_read";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( txf_data_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid TxF data values.",
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
		 "%s: TxF data values:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( data_size != sizeof( fsntfs_txf_data_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported logged utility stream attribute TxF data size: %" PRIzd "\n",
		 function,
		 data_size );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_txf_data_t *) data )->rm_root_file_reference,
	 txf_data_values->rm_root_file_reference );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_txf_data_t *) data )->file_identifier,
	 txf_data_values->file_identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_txf_data_t *) data )->data_lsn,
	 txf_data_values->data_lsn );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_txf_data_t *) data )->metadata_lsn,
	 txf_data_values->metadata_lsn );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_txf_data_t *) data )->directory_index_lsn,
	 txf_data_values->directory_index_lsn );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (fsntfs_txf_data_t *) data )->unknown1,
		 6,
		 0 );

		libcnotify_printf(
		 "%s: resource manager root file reference\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 function,
		 txf_data_values->rm_root_file_reference & 0xffffffffffffUL,
		 txf_data_values->rm_root_file_reference >> 48 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_txf_data_t *) data )->usn_index,
		 value_64bit );
		libcnotify_printf(
		 "%s: update sequence number index\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: file identifier\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 txf_data_values->file_identifier );

		libcnotify_printf(
		 "%s: data log sequence number\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 txf_data_values->data_lsn );

		libcnotify_printf(
		 "%s: metadata log sequence number\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 txf_data_values->metadata_lsn );

		libcnotify_printf(
		 "%s: directory index log sequence number\t\t: 0x%08" PRIx64 "\n",
		 function,
		 txf_data_values->directory_index_lsn );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_txf_data_t *) data )->flags,
		 value_16bit );
		libcnotify_printf(
		 "%s: flags\t\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

