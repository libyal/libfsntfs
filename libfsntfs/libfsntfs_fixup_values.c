/*
 * Fix-up values functions
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
#include <types.h>

#include "libfsntfs_fixup_values.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

/* Applies the fix-up values to the data
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_fixup_values_apply(
     uint8_t *data,
     size_t data_size,
     uint16_t fixup_values_offset,
     uint16_t number_of_fixup_values,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_fixup_values_apply";
	size_t data_offset              = 0;
	size_t fixup_placeholder_offset = 0;
	size_t fixup_value_offset       = 0;
	size_t fixup_values_size        = 0;
	uint16_t fixup_value_index      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit            = 0;
#endif

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry.",
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
	if( fixup_values_offset >= data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid fix-up values offset value out of bounds.",
		 function );

		return( -1 );
	}
	fixup_values_size = 2 + ( (size_t) number_of_fixup_values * 2 );

	if( ( number_of_fixup_values == 0 )
	 || ( fixup_values_size > ( data_size - fixup_values_offset ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalide number of fix-up values value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: fix-up values data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ fixup_values_offset ] ),
		 fixup_values_size,
		 0 );
	}
#endif
	fixup_placeholder_offset = (size_t) fixup_values_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 &( data[ fixup_placeholder_offset ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: fix-up placeholder value\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
	}
#endif
	fixup_value_offset = fixup_placeholder_offset + 2;
	data_offset        = 510;

	for( fixup_value_index = 0;
	     fixup_value_index < number_of_fixup_values;
	     fixup_value_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( data[ fixup_value_offset ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: fix-up value: %" PRIu16 "\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 fixup_value_index,
			 value_16bit );
		}
#endif
		if( ( data_offset + 1 ) < data_size )
		{
			if( ( data[ data_offset ] != data[ fixup_placeholder_offset ] )
			 && ( data[ data_offset + 1 ] != data[ fixup_placeholder_offset + 1 ] ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					byte_stream_copy_to_uint16_little_endian(
					 &( data[ data_offset ] ),
					 value_16bit );
					libcnotify_printf(
					 "%s: corruption detected - mismatch between placeholder and value at offset: %" PRIzd " (0x%04" PRIx16 ")\n",
					 function,
					 data_offset,
					 value_16bit );
				}
#endif
/* TODO handle error */
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: applying fix-up value: %" PRIu16 "\t\t\t: (offset: %" PRIzd ") 0x%02" PRIx8 "%02" PRIx8 " => (offset: %" PRIzd ") 0x%02" PRIx8 "%02" PRIx8 "\n",
				 function,
				 fixup_value_index,
				 data_offset,
				 data[ data_offset + 1 ],
				 data[ data_offset ],
				 fixup_value_offset,
				 data[ fixup_value_offset + 1 ],
				 data[ fixup_value_offset ] );
			}
#endif
			data[ data_offset ]     = data[ fixup_value_offset ];
			data[ data_offset + 1 ] = data[ fixup_value_offset + 1 ];
		}
		fixup_value_offset += 2;
		data_offset        += 512;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

