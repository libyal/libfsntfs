/*
 * OSS-Fuzz target for libfsntfs attribute type
 *
 * Copyright (C) 2010-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libfsntfs.h"

#if !defined( LIBFSNTFS_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFSNTFS_EXTERN \
int libfsntfs_volume_open_file_io_handle(
      libfsntfs_volume_t *volume,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libfsntfs_error_t **error );

#endif /* !defined( LIBFSNTFS_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	uint8_t guid[ 16 ];
	uint8_t security_descriptor[ 64 ];
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 32 ];

	libbfio_handle_t *file_io_handle            = NULL;
	libfsntfs_attribute_t *attribute            = NULL;
	libfsntfs_file_entry_t *file_entry          = NULL;
	libfsntfs_file_entry_t *root_directory      = NULL;
	libfsntfs_file_entry_t *sub_file_entry      = NULL;
	libfsntfs_volume_t *volume                  = NULL;
	uint64_t update_sequence_number             = 0;
	uint64_t parent_file_reference              = 0;
	uint64_t data_first_vcn                     = 0;
	uint64_t data_last_vcn                      = 0;
	uint64_t filetime                           = 0;
	size64_t valid_data_size                    = 0;
	size64_t data_size                          = 0;
	size_t utf8_string_size                     = 0;
	size_t utf16_string_size                    = 0;
	uint32_t file_attribute_flags                = 0;
	uint32_t owner_identifier                    = 0;
	uint32_t compression_method                  = 0;
	uint32_t security_descriptor_identifier     = 0;
	uint32_t tag                                = 0;
	uint32_t attribute_type                      = 0;
	uint16_t flags                              = 0;
	uint16_t data_flags                         = 0;
	uint8_t name_space                          = 0;
	uint8_t major_version                       = 0;
	uint8_t minor_version                       = 0;
	int attribute_index                         = 0;
	int number_of_attributes                    = 0;
	int number_of_sub_file_entries              = 0;
	int result                                  = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfsntfs_volume_initialize(
	     &volume,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfsntfs_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     LIBFSNTFS_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_volume;
	}
	if( libfsntfs_volume_get_root_directory(
	     volume,
	     &root_directory,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_volume;
	}
	if( libfsntfs_file_entry_get_number_of_sub_file_entries(
	     root_directory,
	     &number_of_sub_file_entries,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_file_entry;
	}
	if( number_of_sub_file_entries > 0 )
	{
		if( libfsntfs_file_entry_get_sub_file_entry_by_index(
		     root_directory,
		     0,
		     &sub_file_entry,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_file_entry;
		}
		file_entry = sub_file_entry;
	}
	else
	{
		file_entry = root_directory;
	}
	if( libfsntfs_file_entry_get_number_of_attributes(
	     file_entry,
	     &number_of_attributes,
	     NULL ) != 1 )
	{
		goto on_error_libfsntfs_file_entry;
	}
	for( attribute_index = 0;
	     ( attribute_index < number_of_attributes )
	  && ( attribute_index < 16 );
	     attribute_index++ )
	{
		result = libfsntfs_file_entry_get_attribute_by_index(
		           file_entry,
		           attribute_index,
		           &attribute,
		           NULL );

		if( result == -1 )
		{
			goto on_error_libfsntfs_attribute;
		}
		if( result != 1 )
		{
			continue;
		}
		if( libfsntfs_attribute_get_type(
		     attribute,
		     &attribute_type,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_attribute;
		}
		if( libfsntfs_attribute_get_data_flags(
		     attribute,
		     &data_flags,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_attribute;
		}
		if( libfsntfs_attribute_get_utf8_name_size(
		     attribute,
		     &utf8_string_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_attribute;
		}
		if( utf8_string_size > 0 )
		{
			if( libfsntfs_attribute_get_utf8_name(
			     attribute,
			     utf8_string,
			     64,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}
		if( libfsntfs_attribute_get_utf16_name_size(
		     attribute,
		     &utf16_string_size,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_attribute;
		}
		if( utf16_string_size > 0 )
		{
			if( libfsntfs_attribute_get_utf16_name(
			     attribute,
			     utf16_string,
			     32,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}
		if( libfsntfs_attribute_get_data_vcn_range(
		     attribute,
		     &data_first_vcn,
		     &data_last_vcn,
		     NULL ) == -1 )
		{
			goto on_error_libfsntfs_attribute;
		}
		if( libfsntfs_attribute_get_data_size(
		     attribute,
		     &data_size,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_attribute;
		}
		if( libfsntfs_attribute_get_valid_data_size(
		     attribute,
		     &valid_data_size,
		     NULL ) != 1 )
		{
			goto on_error_libfsntfs_attribute;
		}
		if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
		{
			if( libfsntfs_standard_information_attribute_get_creation_time(
			     attribute,
			     &filetime,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_standard_information_attribute_get_modification_time(
			     attribute,
			     &filetime,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_standard_information_attribute_get_access_time(
			     attribute,
			     &filetime,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_standard_information_attribute_get_entry_modification_time(
			     attribute,
			     &filetime,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_standard_information_attribute_get_file_attribute_flags(
			     attribute,
			     &file_attribute_flags,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_standard_information_attribute_get_owner_identifier(
			     attribute,
			     &owner_identifier,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_standard_information_attribute_get_security_descriptor_identifier(
			     attribute,
			     &security_descriptor_identifier,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_standard_information_attribute_get_update_sequence_number(
			     attribute,
			     &update_sequence_number,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}
		else if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME )
		{
			if( libfsntfs_file_name_attribute_get_parent_file_reference(
			     attribute,
			     &parent_file_reference,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_creation_time(
			     attribute,
			     &filetime,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_modification_time(
			     attribute,
			     &filetime,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_access_time(
			     attribute,
			     &filetime,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_entry_modification_time(
			     attribute,
			     &filetime,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_file_attribute_flags(
			     attribute,
			     &file_attribute_flags,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_name_space(
			     attribute,
			     &name_space,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_utf8_name_size(
			     attribute,
			     &utf8_string_size,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_utf8_name(
			     attribute,
			     utf8_string,
			     64,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_utf16_name_size(
			     attribute,
			     &utf16_string_size,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_file_name_attribute_get_utf16_name(
			     attribute,
			     utf16_string,
			     32,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}
		else if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER )
		{
			if( libfsntfs_object_identifier_attribute_get_droid_file_identifier(
			     attribute,
			     guid,
			     16,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier(
			     attribute,
			     guid,
			     16,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_object_identifier_attribute_get_birth_droid_file_identifier(
			     attribute,
			     guid,
			     16,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier(
			     attribute,
			     guid,
			     16,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}
		else if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR )
		{
			if( libfsntfs_security_descriptor_attribute_get_security_descriptor_size(
			     attribute,
			     &utf8_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_security_descriptor_attribute_get_security_descriptor(
			     attribute,
			     security_descriptor,
			     64,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}
		else if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME )
		{
			if( libfsntfs_volume_name_attribute_get_utf8_name_size(
			     attribute,
			     &utf8_string_size,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_volume_name_attribute_get_utf8_name(
			     attribute,
			     utf8_string,
			     64,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_volume_name_attribute_get_utf16_name_size(
			     attribute,
			     &utf16_string_size,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_volume_name_attribute_get_utf16_name(
			     attribute,
			     utf16_string,
			     32,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}
		else if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION )
		{
			if( libfsntfs_volume_information_attribute_get_version(
			     attribute,
			     &major_version,
			     &minor_version,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_volume_information_attribute_get_flags(
			     attribute,
			     &flags,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}
		else if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
		{
			if( libfsntfs_reparse_point_attribute_get_tag(
			     attribute,
			     &tag,
			     NULL ) != 1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_compression_method(
			     attribute,
			     &compression_method,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
			     attribute,
			     &utf8_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
			     attribute,
			     utf8_string,
			     64,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size(
			     attribute,
			     &utf16_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
			     attribute,
			     utf16_string,
			     32,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_utf8_print_name_size(
			     attribute,
			     &utf8_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_utf8_print_name(
			     attribute,
			     utf8_string,
			     64,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_utf16_print_name_size(
			     attribute,
			     &utf16_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
			if( libfsntfs_reparse_point_attribute_get_utf16_print_name(
			     attribute,
			     utf16_string,
			     32,
			     NULL ) == -1 )
			{
				goto on_error_libfsntfs_attribute;
			}
		}

		libfsntfs_attribute_free(
		 &attribute,
		 NULL );
	}
on_error_libfsntfs_attribute:
	libfsntfs_attribute_free(
	 &attribute,
	 NULL );

on_error_libfsntfs_file_entry:
	if( sub_file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	libfsntfs_file_entry_free(
	 &root_directory,
	 NULL );
	libfsntfs_volume_close(
	 volume,
	 NULL );

on_error_libfsntfs_volume:
	libfsntfs_volume_free(
	 &volume,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
