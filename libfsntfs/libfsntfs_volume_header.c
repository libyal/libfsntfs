/*
 * The NTFS volume header functions
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

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_volume_header.h"

#include "fsntfs_index.h"
#include "fsntfs_mft_entry.h"
#include "fsntfs_volume_header.h"

/* Creates a volume header
 * Make sure the value volume_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_initialize(
     libfsntfs_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_initialize";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume header value already set.",
		 function );

		return( -1 );
	}
	*volume_header = memory_allocate_structure(
	                  libfsntfs_volume_header_t );

	if( *volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_header,
	     0,
	     sizeof( libfsntfs_volume_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( -1 );
}

/* Frees a volume header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_free(
     libfsntfs_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_free";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_read_data(
     libfsntfs_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                    = "libfsntfs_volume_header_read_data";
	uint64_t mft_cluster_block_number        = 0;
	uint64_t mirror_mft_cluster_block_number = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                     = 0;
	uint16_t value_16bit                     = 0;
#endif

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
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
	if( ( data_size < sizeof( fsntfs_volume_header_t ) )
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
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsntfs_volume_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (fsntfs_volume_header_t *) data )->file_system_signature,
	     fsntfs_volume_file_system_signature,
	     8 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid volume system signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_volume_header_t *) data )->bytes_per_sector,
	 volume_header->bytes_per_sector );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_volume_header_t *) data )->total_number_of_sectors,
	 volume_header->volume_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_volume_header_t *) data )->mft_cluster_block_number,
	 mft_cluster_block_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_volume_header_t *) data )->mirror_mft_cluster_block_number,
	 mirror_mft_cluster_block_number );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_volume_header_t *) data )->mft_entry_size,
	 volume_header->mft_entry_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_volume_header_t *) data )->index_entry_size,
	 volume_header->index_entry_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_volume_header_t *) data )->volume_serial_number,
	 volume_header->volume_serial_number );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: boot entry point\t\t\t: 0x%02x 0x%02x 0x%02x\n",
		 function,
		 ( (fsntfs_volume_header_t *) data )->boot_entry_point[ 0 ],
		 ( (fsntfs_volume_header_t *) data )->boot_entry_point[ 1 ],
		 ( (fsntfs_volume_header_t *) data )->boot_entry_point[ 2 ] );

		libcnotify_printf(
		 "%s: file system signature\t\t: %c%c%c%c%c%c%c%c\n",
		 function,
		 ( (fsntfs_volume_header_t *) data )->file_system_signature[ 0 ],
		 ( (fsntfs_volume_header_t *) data )->file_system_signature[ 1 ],
		 ( (fsntfs_volume_header_t *) data )->file_system_signature[ 2 ],
		 ( (fsntfs_volume_header_t *) data )->file_system_signature[ 3 ],
		 ( (fsntfs_volume_header_t *) data )->file_system_signature[ 4 ],
		 ( (fsntfs_volume_header_t *) data )->file_system_signature[ 5 ],
		 ( (fsntfs_volume_header_t *) data )->file_system_signature[ 6 ],
		 ( (fsntfs_volume_header_t *) data )->file_system_signature[ 7 ] );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu16 "\n",
		 function,
		 volume_header->bytes_per_sector );

		libcnotify_printf(
		 "%s: sectors per cluster block\t\t: %" PRIu8 "\n",
		 function,
		 ( (fsntfs_volume_header_t *) data )->sectors_per_cluster_block );

		libcnotify_printf(
		 "%s: unknown1\n",
		 function );
		libcnotify_print_data(
		 ( (fsntfs_volume_header_t *) data )->unknown1,
		 7,
		 0 );

		libcnotify_printf(
		 "%s: media descriptor\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fsntfs_volume_header_t *) data )->media_descriptor );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_volume_header_t *) data )->unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_volume_header_t *) data )->sectors_per_track,
		 value_16bit );
		libcnotify_printf(
		 "%s: sectors per track\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_volume_header_t *) data )->number_of_heads,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of heads\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_volume_header_t *) data )->number_of_hidden_sectors,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of hidden sectors\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_volume_header_t *) data )->unknown3,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_volume_header_t *) data )->unknown4,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "%s: total number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 volume_header->volume_size );

		libcnotify_printf(
		 "%s: MFT cluster block number\t\t: %" PRIu64 "\n",
		 function,
		 mft_cluster_block_number );

		libcnotify_printf(
		 "%s: mirror MFT cluster block number\t: %" PRIu64 "\n",
		 function,
		 mirror_mft_cluster_block_number );

		libcnotify_printf(
		 "%s: MFT entry size\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->mft_entry_size );

		libcnotify_printf(
		 "%s: index entry size\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->index_entry_size );

		libcnotify_printf(
		 "%s: volume serial number\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 volume_header->volume_serial_number );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_volume_header_t *) data )->checksum,
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: bootcode\n",
		 function );
		libcnotify_print_data(
		 ( (fsntfs_volume_header_t *) data )->bootcode,
		 426,
		 0 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_volume_header_t *) data )->sector_signature,
		 value_16bit );
		libcnotify_printf(
		 "%s: sector signature\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( volume_header->bytes_per_sector != 256 )
	 && ( volume_header->bytes_per_sector != 512 )
	 && ( volume_header->bytes_per_sector != 1024 )
	 && ( volume_header->bytes_per_sector != 2048 )
	 && ( volume_header->bytes_per_sector != 4096 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported bytes per sector: %" PRIu16 ".",
		 function,
		 volume_header->bytes_per_sector );

		return( -1 );
	}
	volume_header->cluster_block_size = ( (fsntfs_volume_header_t *) data )->sectors_per_cluster_block;

	if( volume_header->cluster_block_size > 128 )
	{
		/* The size is calculated as: 2 ^ ( 256 - value )
		 */
		volume_header->cluster_block_size = 256 - volume_header->cluster_block_size;

		if( volume_header->cluster_block_size > 12 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cluster block size value out of bounds.",
			 function );

			return( -1 );
		}
		volume_header->cluster_block_size = 1 << volume_header->cluster_block_size;
	}
	volume_header->cluster_block_size *= volume_header->bytes_per_sector;

	if( ( volume_header->cluster_block_size != 256 )
	 && ( volume_header->cluster_block_size != 512 )
	 && ( volume_header->cluster_block_size != 1024 )
	 && ( volume_header->cluster_block_size != 2048 )
	 && ( volume_header->cluster_block_size != 4096 )
	 && ( volume_header->cluster_block_size != 8192 )
	 && ( volume_header->cluster_block_size != 16384 )
	 && ( volume_header->cluster_block_size != 32768 )
	 && ( volume_header->cluster_block_size != 65536 )
	 && ( volume_header->cluster_block_size != 131072 )
	 && ( volume_header->cluster_block_size != 262144 )
	 && ( volume_header->cluster_block_size != 524288 )
	 && ( volume_header->cluster_block_size != 1048576 )
	 && ( volume_header->cluster_block_size != 2097152 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported cluster block size: %" PRIu32 ".",
		 function,
		 volume_header->cluster_block_size );

		return( -1 );
	}
	if( ( volume_header->mft_entry_size == 0 )
	 || ( volume_header->mft_entry_size > 255 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported MFT entry size: %" PRIu32 ".",
		 function,
		 volume_header->mft_entry_size );

		return( -1 );
	}
	if( volume_header->mft_entry_size < 128 )
	{
		if( volume_header->mft_entry_size >= (size32_t) ( ( UINT32_MAX / volume_header->cluster_block_size ) + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid MFT entry size value out of bounds.",
			 function );

			return( -1 );
		}
		volume_header->mft_entry_size *= volume_header->cluster_block_size;
	}
	else
	{
		/* The size is calculated as: 2 ^ ( 256 - value )
		 */
		volume_header->mft_entry_size = 256 - volume_header->mft_entry_size;

		if( volume_header->mft_entry_size >= 32 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid MFT entry size value out of bounds.",
			 function );

			return( -1 );
		}
		volume_header->mft_entry_size = (uint32_t) 1UL << volume_header->mft_entry_size;
	}
	if( ( (size_t) volume_header->mft_entry_size < sizeof( fsntfs_mft_entry_header_t ) )
	 || ( volume_header->mft_entry_size >= (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry size: %" PRIu32 " value out of bounds.",
		 function,
		 volume_header->mft_entry_size );

		return( -1 );
	}
	if( ( volume_header->index_entry_size == 0 )
	 || ( volume_header->index_entry_size > 255 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported index entry size: %" PRIu32 ".",
		 function,
		 volume_header->index_entry_size );

		return( -1 );
	}
	if( volume_header->index_entry_size < 128 )
	{
		if( volume_header->index_entry_size >= (size32_t) ( ( UINT32_MAX / volume_header->cluster_block_size ) + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index entry size value out of bounds.",
			 function );

			return( -1 );
		}
		volume_header->index_entry_size *= volume_header->cluster_block_size;
	}
	else
	{
		/* The size is calculated as: 2 ^ ( 256 - value )
		 */
		volume_header->index_entry_size = 256 - volume_header->index_entry_size;

		if( volume_header->index_entry_size >= 32 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index entry size value out of bounds.",
			 function );

			return( -1 );
		}
		volume_header->index_entry_size = (uint32_t) 1UL << volume_header->index_entry_size;
	}
	if( (size_t) volume_header->index_entry_size < sizeof( fsntfs_index_entry_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index entry size: %" PRIu32 " value out of bounds.",
		 function,
		 volume_header->index_entry_size );

		return( -1 );
	}
	if( volume_header->volume_size > (size64_t) ( ( UINT64_MAX / volume_header->bytes_per_sector ) + 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid volume size value out of bounds.",
		 function );

		return( -1 );
	}
	volume_header->volume_size *= volume_header->bytes_per_sector;
	volume_header->volume_size += volume_header->bytes_per_sector;

	volume_header->mft_offset = mft_cluster_block_number
	                          * volume_header->cluster_block_size;

	volume_header->mirror_mft_offset = mirror_mft_cluster_block_number
	                                 * volume_header->cluster_block_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: calculated cluster block size\t: %" PRIu32 "\n",
		 function,
		 volume_header->cluster_block_size );

		libcnotify_printf(
		 "%s: calculated MFT entry size\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->mft_entry_size );

		libcnotify_printf(
		 "%s: calculated index entry size\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->index_entry_size );

		libcnotify_printf(
		 "%s: calculated volume size\t\t: %" PRIu64 "\n",
		 function,
		 volume_header->volume_size );

		libcnotify_printf(
		 "%s: calculated MFT offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 volume_header->mft_offset );

		libcnotify_printf(
		 "%s: calculated mirror MFT offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 volume_header->mirror_mft_offset );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_read_file_io_handle(
     libfsntfs_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t volume_header_data[ sizeof( fsntfs_volume_header_t ) ];

	static char *function = "libfsntfs_volume_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              volume_header_data,
	              sizeof( fsntfs_volume_header_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( fsntfs_volume_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsntfs_volume_header_read_data(
	     volume_header,
	     volume_header_data,
	     sizeof( fsntfs_volume_header_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_get_bytes_per_sector(
     libfsntfs_volume_header_t *volume_header,
     uint16_t *bytes_per_sector,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_get_bytes_per_sector";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
	*bytes_per_sector = volume_header->bytes_per_sector;

	return( 1 );
}

/* Retrieves the cluster block size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_get_cluster_block_size(
     libfsntfs_volume_header_t *volume_header,
     size32_t *cluster_block_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_get_cluster_block_size";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( cluster_block_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block size.",
		 function );

		return( -1 );
	}
	*cluster_block_size = volume_header->cluster_block_size;

	return( 1 );
}

/* Retrieves the MFT entry size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_get_mft_entry_size(
     libfsntfs_volume_header_t *volume_header,
     size32_t *mft_entry_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_get_mft_entry_size";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( mft_entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry size.",
		 function );

		return( -1 );
	}
	*mft_entry_size = volume_header->mft_entry_size;

	return( 1 );
}

/* Retrieves the index entry size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_get_index_entry_size(
     libfsntfs_volume_header_t *volume_header,
     size32_t *index_entry_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_get_index_entry_size";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
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
	*index_entry_size = volume_header->index_entry_size;

	return( 1 );
}

/* Retrieves the volume size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_get_volume_size(
     libfsntfs_volume_header_t *volume_header,
     size64_t *volume_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_get_volume_size";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( volume_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume size.",
		 function );

		return( -1 );
	}
	*volume_size = volume_header->volume_size;

	return( 1 );
}

/* Retrieves the volume serial number
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_get_volume_serial_number(
     libfsntfs_volume_header_t *volume_header,
     uint64_t *volume_serial_number,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_get_volume_serial_number";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( volume_serial_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume serial number.",
		 function );

		return( -1 );
	}
	*volume_serial_number = volume_header->volume_serial_number;

	return( 1 );
}

/* Retrieves the MFT offset
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_header_get_mft_offset(
     libfsntfs_volume_header_t *volume_header,
     off64_t *mft_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_header_get_mft_offset";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( mft_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT offset.",
		 function );

		return( -1 );
	}
	*mft_offset = volume_header->mft_offset;

	return( 1 );
}

