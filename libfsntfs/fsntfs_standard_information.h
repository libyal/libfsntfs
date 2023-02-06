/*
 * The NTFS standard information attribute ($STANDARD_INFORMATION) definition
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

#if !defined( _FSNTFS_STANDARD_INFORMATION_H )
#define _FSNTFS_STANDARD_INFORMATION_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_standard_information fsntfs_standard_information_t;

struct fsntfs_standard_information
{
	/* The creation date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t creation_time[ 8 ];

	/* The last modification date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t modification_time[ 8 ];

	/* The entry last modification date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t entry_modification_time[ 8 ];

	/* The last access date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t access_time[ 8 ];

	/* The file attribute flags
	 * Consists of 4 bytes
	 */
	uint8_t file_attribute_flags[ 4 ];

	/* The maximum number of versions
	 * Consists of 4 bytes
	 */
	uint8_t maximum_number_of_versions[ 4 ];

	/* The version number
	 * Consists of 4 bytes
	 */
	uint8_t version_number[ 4 ];

	/* The class identifier
	 * Consists of 4 bytes
	 */
	uint8_t class_identifier[ 4 ];

	/* The owner identifier
	 * Consists of 4 bytes
	 */
	uint8_t owner_identifier[ 4 ];

	/* The security descriptor identifier
	 * Consists of 4 bytes
	 */
	uint8_t security_descriptor_identifier[ 4 ];

	/* The quota charged
	 * Consists of 8 bytes
	 */
	uint8_t quota_charged[ 8 ];

	/* The update sequence number (USN)
	 * Consists of 8 bytes
	 */
	uint8_t update_sequence_number[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_STANDARD_INFORMATION_H ) */

