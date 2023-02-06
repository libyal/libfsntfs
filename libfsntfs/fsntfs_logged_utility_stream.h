/*
 * The NTFS logged utillity stream attribute ($LOGGED_UTILITY_STREAM) definition
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

#if !defined( _FSNTFS_LOGGED_UTILITY_STREAM_H )
#define _FSNTFS_LOGGED_UTILITY_STREAM_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_logged_utility_stream fsntfs_logged_utility_stream_t;

struct fsntfs_logged_utility_stream
{
	/* The type and flags
	 * Consists of 4 bytes
	 */
	uint8_t type[ 4 ];

	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* Padding
	 * Consists of 2 bytes
	 */
	uint8_t padding1[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_LOGGED_UTILITY_STREAM_H ) */

