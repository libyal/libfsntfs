/*
 * Debug functions
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

#if !defined( _LIBFSNTFS_DEBUG_H )
#define _LIBFSNTFS_DEBUG_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

void libfsntfs_debug_print_mft_attribute_data_flags(
      uint16_t mft_attribute_data_flags );

void libfsntfs_debug_print_mft_entry_flags(
      uint16_t mft_entry_flags );

void libfsntfs_debug_print_file_attribute_flags(
      uint32_t file_attribute_flags );

void libfsntfs_debug_print_index_node_flags(
      uint32_t index_node_flags );

void libfsntfs_debug_print_index_value_flags(
      uint32_t index_value_flags );

void libfsntfs_debug_print_reparse_point_tag(
      uint32_t tag );

void libfsntfs_debug_print_volume_information_flags(
      uint16_t volume_information_flags );

const char *libfsntfs_debug_print_attribute_type(
             uint32_t attribute_type );

const char *libfsntfs_debug_print_file_name_attribute_name_space(
             uint8_t name_space );

const char *libfsntfs_debug_print_attribute_type(
             uint32_t attribute_type );

const char *libfsntfs_debug_print_collation_type(
             uint32_t collation_type );

const char *libfsntfs_debug_print_compression_method(
             uint32_t compression_method );

int libfsntfs_debug_print_filetime_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfsntfs_debug_print_guid_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfsntfs_debug_print_utf16_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     libcerror_error_t **error );

int libfsntfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_DEBUG_H ) */

