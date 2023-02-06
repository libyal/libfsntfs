/*
 * The NTFS volume information attribute ($VOLUME_INFORMATION) definition
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

#if !defined( _FSNTFS_VOLUME_INFORMATION_H )
#define _FSNTFS_VOLUME_INFORMATION_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_volume_information fsntfs_volume_information_t;

struct fsntfs_volume_information
{
	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];

	/* The major version (number)
	 * Consists of 1 byte
	 */
	uint8_t major_version;

	/* The minor version (number)
	 * Consists of 1 byte
	 */
	uint8_t minor_version;

	/* The flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_VOLUME_INFORMATION_H ) */

