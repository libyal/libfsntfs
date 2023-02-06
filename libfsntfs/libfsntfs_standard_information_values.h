/*
 * Standard information attribute ($STANDARD_INFORMATION) values functions
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

#if !defined( _LIBFSNTFS_STANDARD_INFORMATION_VALUES_H )
#define _LIBFSNTFS_STANDARD_INFORMATION_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_standard_information_values libfsntfs_standard_information_values_t;

struct libfsntfs_standard_information_values
{
	/* The creation time
	 */
	uint64_t creation_time;

	/* The modification time
	 */
	uint64_t modification_time;

	/* The access time
	 */
	uint64_t access_time;

	/* The entry modification time
	 */
	uint64_t entry_modification_time;

	/* The file attribute flags
	 */
	uint32_t file_attribute_flags;

	/* The owner identifier
	 */
	uint32_t owner_identifier;

	/* The security descriptor identifier
	 */
	uint32_t security_descriptor_identifier;

	/* The update sequence number (USN)
	 */
	uint64_t update_sequence_number;

	/* Value to indicate the is case sensitive flag is set
	 */
	uint8_t is_case_sensitive;
};

int libfsntfs_standard_information_values_initialize(
     libfsntfs_standard_information_values_t **standard_information_values,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_free(
     libfsntfs_standard_information_values_t **standard_information_values,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_read_data(
     libfsntfs_standard_information_values_t *standard_information_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_read_from_mft_attribute(
     libfsntfs_standard_information_values_t *standard_information_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_get_creation_time(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *filetime,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_get_modification_time(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *filetime,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_get_access_time(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *filetime,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_get_entry_modification_time(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *filetime,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_get_file_attribute_flags(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_get_security_descriptor_identifier(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint32_t *security_descriptor_identifier,
     libcerror_error_t **error );

int libfsntfs_standard_information_values_get_update_sequence_number(
     libfsntfs_standard_information_values_t *standard_information_values,
     uint64_t *update_sequence_number,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_STANDARD_INFORMATION_VALUES_H ) */

