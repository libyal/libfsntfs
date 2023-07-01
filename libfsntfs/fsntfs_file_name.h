/*
 * The NTFS file name attribute ($FILE_NAME) definition
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

#if !defined( _FSNTFS_FILE_NAME_H )
#define _FSNTFS_FILE_NAME_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_file_name fsntfs_file_name_t;

struct fsntfs_file_name
{
	/* The parent file reference
	 * Consists of 8 bytes
	 */
	uint8_t parent_file_reference[ 8 ];

	/* The creation date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t creation_time[ 8 ];

	/* The last modification date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t modification_time[ 8 ];

	/* The entry last modification date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t entry_modification_time[ 8 ];

	/* The last access date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t access_time[ 8 ];

	/* The allocated (or reserved) data size
	 * Consists of 8 bytes
	 */
	uint8_t allocated_data_size[ 8 ];

	/* The data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* The file attribute flags
	 * Consists of 4 bytes
	 */
	uint8_t file_attribute_flags[ 4 ];

	/* The extended data
	 * Consists of 4 bytes
	 */
	uint8_t extended_data[ 4 ];

	/* The name size
	 * Consists of 1 byte
	 * Contains the number of characters without the end-of-string character
	 */
	uint8_t name_size;

	/* The name space
	 * Consists of 1 byte
	 */
	uint8_t name_space;
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_FILE_NAME_H ) */

