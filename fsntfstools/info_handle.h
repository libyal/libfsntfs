/*
 * Info handle
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

#if !defined( _INFO_HANDLE_H )
#define _INFO_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "fsntfstools_libbfio.h"
#include "fsntfstools_libcerror.h"
#include "fsntfstools_libfsntfs.h"
#include "fsntfstools_libfusn.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct info_handle info_handle_t;

struct info_handle
{
	/* The MFT entry index
	 */
	uint64_t entry_index;

	/* The volume offset
	 */
	off64_t volume_offset;

	/* The libbfio input file IO handle
	 */
	libbfio_handle_t *input_file_io_handle;

	/* The libfsntfs input volume
	 */
	libfsntfs_volume_t *input_volume;

	/* The libfsntfs input MFT metadata file
	 */
	libfsntfs_mft_metadata_file_t *input_mft_metadata_file;

	/* Value to indicate if the MD5 hash should be calculated
	 */
	uint8_t calculate_md5;

	/* The bodyfile output stream
	 */
	FILE *bodyfile_stream;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int fsntfstools_system_string_copy_decimal_to_integer_64_bit(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error );

void info_handle_file_attribute_flags_fprint(
      uint32_t file_attribute_flags,
      FILE *notify_stream );

void info_handle_volume_information_flags_fprint(
      uint16_t volume_information_flags,
      FILE *notify_stream );

const char *info_handle_get_access_control_entry_type(
             uint8_t entry_type );

const char *info_handle_get_attribute_type_description(
             uint32_t attribute_type );

int info_handle_initialize(
     info_handle_t **info_handle,
     uint8_t calculate_md5,
     libcerror_error_t **error );

int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error );

int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_set_bodyfile(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_file_entry_calculate_md5(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     char *md5_string,
     size_t md5_string_size,
     libcerror_error_t **error );

int info_handle_name_value_fprint(
     info_handle_t *info_handle,
     const system_character_t *value_string,
     size_t value_string_length,
     libcerror_error_t **error );

int info_handle_filetime_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     uint64_t value_64bit,
     libcerror_error_t **error );

int info_handle_security_descriptor_fprint(
     info_handle_t *info_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int info_handle_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_attribute_t *attribute,
     int attribute_index,
     libcerror_error_t **error );

int info_handle_attribute_list_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_bitmap_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_data_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_file_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_object_identifier_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_reparse_point_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_security_descriptor_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_standard_information_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_volume_information_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_volume_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int info_handle_file_entry_value_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error );

int info_handle_bodyfile_name_value_fprint(
     info_handle_t *info_handle,
     const system_character_t *value_string,
     size_t value_string_length,
     libcerror_error_t **error );

int info_handle_bodyfile_file_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_attribute_t *attribute,
     const system_character_t *path,
     size_t path_length,
     const system_character_t *file_entry_name,
     size_t file_entry_name_length,
     libcerror_error_t **error );

int info_handle_bodyfile_index_root_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_attribute_t *attribute,
     const system_character_t *path,
     size_t path_length,
     const system_character_t *file_entry_name,
     size_t file_entry_name_length,
     const system_character_t *attribute_name,
     size_t attribute_name_size,
     libcerror_error_t **error );

int info_handle_bodyfile_file_entry_value_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_data_stream_t *alternate_data_stream,
     const system_character_t *path,
     size_t path_length,
     const system_character_t *file_entry_name,
     size_t file_entry_name_length,
     const system_character_t *data_stream_name,
     libcerror_error_t **error );

int info_handle_bodyfile_mft_entry_fprint(
     info_handle_t *info_handle,
     uint64_t mft_entry_index,
     libcerror_error_t **error );

int info_handle_file_entry_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     libfsntfs_attribute_t *file_name_attribute,
     const system_character_t *path,
     const system_character_t *file_entry_name,
     libcerror_error_t **error );

int info_handle_file_system_hierarchy_fprint_file_entry(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     const system_character_t *path,
     libcerror_error_t **error );

int info_handle_mft_entry_fprint(
     info_handle_t *info_handle,
     uint64_t mft_entry_index,
     libcerror_error_t **error );

int info_handle_mft_entries_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_file_entry_fprint_by_path(
     info_handle_t *info_handle,
     const system_character_t *path,
     libcerror_error_t **error );

int info_handle_file_system_hierarchy_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

void info_handle_usn_record_update_reason_flags_fprint(
      uint32_t update_reason_flags,
      FILE *notify_stream );

void info_handle_usn_record_update_source_flags_fprint(
      uint32_t update_source_flags,
      FILE *notify_stream );

int info_handle_usn_record_fprint(
     info_handle_t *info_handle,
     libfusn_record_t *usn_record,
     libcerror_error_t **error );

int info_handle_usn_change_journal_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _INFO_HANDLE_H ) */

