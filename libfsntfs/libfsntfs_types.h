/*
 * The internal type definitions
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

#if !defined( _LIBFSNTFS_INTERNAL_TYPES_H )
#define _LIBFSNTFS_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBFSNTFS for local use of libfsntfs
 * The definitions in <libfsntfs/types.h> are copied here
 * for local use of libfsntfs
 */
#if defined( HAVE_LOCAL_LIBFSNTFS )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libfsntfs_attribute {}			libfsntfs_attribute_t;
typedef struct libfsntfs_attribute_list_entry {}	libfsntfs_attribute_list_entry_t;
typedef struct libfsntfs_data_stream {}			libfsntfs_data_stream_t;
typedef struct libfsntfs_file_entry {}			libfsntfs_file_entry_t;
typedef struct libfsntfs_mft_metadata_file {}		libfsntfs_mft_metadata_file_t;
typedef struct libfsntfs_usn_change_journal {}		libfsntfs_usn_change_journal_t;
typedef struct libfsntfs_volume {}			libfsntfs_volume_t;

#else
typedef intptr_t libfsntfs_attribute_t;
typedef intptr_t libfsntfs_attribute_list_entry_t;
typedef intptr_t libfsntfs_data_stream_t;
typedef intptr_t libfsntfs_file_entry_t;
typedef intptr_t libfsntfs_mft_metadata_file_t;
typedef intptr_t libfsntfs_usn_change_journal_t;
typedef intptr_t libfsntfs_volume_t;

#endif /* defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI ) */

#endif /* defined( HAVE_LOCAL_LIBFSNTFS ) */

/* The largest primary (or scalar) available
 * supported by a single load and store instruction
 */
typedef unsigned long int libfsntfs_aligned_t;

#endif /* !defined( _LIBFSNTFS_INTERNAL_TYPES_H ) */

