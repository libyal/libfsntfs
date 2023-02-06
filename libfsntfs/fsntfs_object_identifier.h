/*
 * The NTFS object identifier attribute ($OBJECT_ID) definition
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

#if !defined( _FSNTFS_OBJECT_IDENTIFIER_H )
#define _FSNTFS_OBJECT_IDENTIFIER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_object_identifier fsntfs_object_identifier_t;

struct fsntfs_object_identifier
{
	/* The droid file identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t droid_file_identifier[ 16 ];

	/* The birth droid volume identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t birth_droid_volume_identifier[ 16 ];

	/* The birth droid file identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t birth_droid_file_identifier[ 16 ];

	/* The birth droid domain identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t birth_droid_domain_identifier[ 16 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_OBJECT_IDENTIFIER_H ) */

