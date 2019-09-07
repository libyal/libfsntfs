/*
 * Master File Table (MFT) attribute header functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSNTFS_MFT_ATTRIBUTE_HEADER_H )
#define _LIBFSNTFS_MFT_ATTRIBUTE_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_mft_attribute_header libfsntfs_mft_attribute_header_t;

struct libfsntfs_mft_attribute_header
{
	/* The type
	 */
	uint32_t type;

	/* The size
	 */
	uint32_t size;

	/* The non-resident flag
	 */
	uint8_t non_resident_flag;

	/* The name size
	 */
	uint16_t name_size;

	/* The name offset
	 */
	uint16_t name_offset;

	/* The data flags
	 */
	uint16_t data_flags;

	/* The identifier
	 */
	uint16_t identifier;
};

int libfsntfs_mft_attribute_header_initialize(
     libfsntfs_mft_attribute_header_t **mft_attribute_header,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_header_free(
     libfsntfs_mft_attribute_header_t **mft_attribute_header,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_header_read_data(
     libfsntfs_mft_attribute_header_t *mft_attribute_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_MFT_ATTRIBUTE_HEADER_H ) */

