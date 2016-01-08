/*
 * Info handle
 *
 * Copyright (C) 2010-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _INFO_HANDLE_H )
#define _INFO_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "fsntfstools_libbfio.h"
#include "fsntfstools_libcerror.h"
#include "fsntfstools_libcstring.h"
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

	/* The nofication output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error );

int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error );

int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int info_handle_open_input(
     info_handle_t *info_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     int attribute_index,
     libfsntfs_error_t **error );

int info_handle_data_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error );

int info_handle_file_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error );

int info_handle_object_identifier_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error );

int info_handle_reparse_point_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error );

int info_handle_standard_information_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error );

int info_handle_volume_information_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error );

int info_handle_volume_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error );

int info_handle_file_entry_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     int indentation_level,
     libfsntfs_error_t **error );

int info_handle_mft_entry_fprint(
     info_handle_t *info_handle,
     uint64_t mft_entry_index,
     libfsntfs_error_t **error );

int info_handle_mft_entries_fprint(
     info_handle_t *info_handle,
     libfsntfs_error_t **error );

int info_handle_file_system_hierarchy_fprint(
     info_handle_t *info_handle,
     libfsntfs_error_t **error );

void info_handle_usn_record_update_reason_flags_fprint(
      uint32_t update_reason_flags,
      FILE *notify_stream );

void info_handle_usn_record_update_source_flags_fprint(
      uint32_t update_source_flags,
      FILE *notify_stream );

int info_handle_usn_record_fprint(
     info_handle_t *info_handle,
     libfusn_record_t *usn_record,
     libfsntfs_error_t **error );

int info_handle_usn_change_journal_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

