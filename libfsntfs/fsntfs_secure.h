/*
 * The NTFS $Secure definitions
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

#if !defined( _FSNTFS_SECURE_H )
#define _FSNTFS_SECURE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_secure_hash_index_key fsntfs_secure_hash_index_key_t;

struct fsntfs_secure_hash_index_key
{
	/* The hash
	 * Consists of 4 bytes
	 */
	uint8_t hash[ 4 ];

	/* The identifier
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];
};

typedef struct fsntfs_secure_identifier_index_key fsntfs_secure_identifier_index_key_t;

struct fsntfs_secure_identifier_index_key
{
	/* The identifier
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];
};

typedef struct fsntfs_secure_index_value fsntfs_secure_index_value_t;

struct fsntfs_secure_index_value
{
	/* The hash
	 * Consists of 4 bytes
	 */
	uint8_t hash[ 4 ];

	/* The identifier
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The data offset
	 * Consists of 8 bytes
	 */
	uint8_t data_offset[ 8 ];

	/* The data size
	 * Consists of 4 bytes
	 */
	uint8_t data_size[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_SECURE_H ) */

