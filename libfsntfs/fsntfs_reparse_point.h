/*
 * The NTFS reparse point attribute ($REPARSE_POINT) definition
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

#if !defined( _FSNTFS_REPARSE_POINT_H )
#define _FSNTFS_REPARSE_POINT_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_reparse_point fsntfs_reparse_point_t;

struct fsntfs_reparse_point
{
	/* The tag (type and flags)
	 * Consists of 4 bytes
	 */
	uint8_t tag[ 4 ];

	/* The reparse data size
	 * Consists of 2 bytes
	 */
	uint8_t reparse_data_size[ 2 ];

	/* Padding
	 * Consists of 2 bytes
	 */
	uint8_t padding1[ 2 ];
};

typedef struct fsntfs_mount_point_reparse_data fsntfs_mount_point_reparse_data_t;

struct fsntfs_mount_point_reparse_data
{
	/* The substitute name offset
	 * Consists of 2 bytes
	 */
	uint8_t substitute_name_offset[ 2 ];

	/* The substitute name size
	 * Consists of 2 bytes
	 */
	uint8_t substitute_name_size[ 2 ];

	/* The print name offset
	 * Consists of 2 bytes
	 */
	uint8_t print_name_offset[ 2 ];

	/* The print name size
	 * Consists of 2 bytes
	 */
	uint8_t print_name_size[ 2 ];
};

typedef struct fsntfs_symbolic_link_reparse_data fsntfs_symbolic_link_reparse_data_t;

struct fsntfs_symbolic_link_reparse_data
{
	/* The substitute name offset
	 * Consists of 2 bytes
	 */
	uint8_t substitute_name_offset[ 2 ];

	/* The substitute name size
	 * Consists of 2 bytes
	 */
	uint8_t substitute_name_size[ 2 ];

	/* The print name offset
	 * Consists of 2 bytes
	 */
	uint8_t print_name_offset[ 2 ];

	/* The print name size
	 * Consists of 2 bytes
	 */
	uint8_t print_name_size[ 2 ];

	/* The flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_REPARSE_POINT_H ) */

