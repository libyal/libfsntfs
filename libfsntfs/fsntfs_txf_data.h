/*
 * The Transactional NTFS (TxF) data ($TXF_DATA) logged utility stream
 * attribute ($LOGGED_UTILITY_STREAM) definition
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

#if !defined( _FSNTFS_TXF_DATA_H )
#define _FSNTFS_TXF_DATA_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_txf_data fsntfs_txf_data_t;

struct fsntfs_txf_data
{
	/* Unknown
	 * Consists of 6 bytes
	 */
	uint8_t unknown1[ 6 ];

	/* The resource manager root file reference
	 * Consists of 8 bytes
	 */
	uint8_t rm_root_file_reference[ 8 ];

	/* The USN index
	 * Consists of 8 bytes
	 */
	uint8_t usn_index[ 8 ];

	/* The file identifier
	 * Consists of 8 bytes
	 */
	uint8_t file_identifier[ 8 ];

	/* The file data LSN
	 * Consists of 8 bytes
	 */
	uint8_t data_lsn[ 8 ];

	/* The file system metadata LSN
	 * Consists of 8 bytes
	 */
	uint8_t metadata_lsn[ 8 ];

	/* The directory index LSN
	 * Consists of 8 bytes
	 */
	uint8_t directory_index_lsn[ 8 ];

	/* The flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_TXF_DATA_H ) */

