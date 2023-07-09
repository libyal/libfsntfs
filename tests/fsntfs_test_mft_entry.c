/*
 * Library mft_entry type test program
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

#include <common.h>
#include <byte_stream.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsntfs_test_functions.h"
#include "fsntfs_test_libbfio.h"
#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_definitions.h"
#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_mft_attribute.h"
#include "../libfsntfs/libfsntfs_mft_entry.h"
#include "../libfsntfs/libfsntfs_mft_entry_header.h"

/* Define to make fsntfs_test_file generate verbose output
#define FSNTFS_TEST_MFT_ENTRY_VERBOSE
 */

uint8_t fsntfs_test_mft_entry_data1[ 1024 ] = {
	0x46, 0x49, 0x4c, 0x45, 0x30, 0x00, 0x03, 0x00, 0x52, 0x51, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00, 0xa8, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x00, 0xe7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x96, 0xd5, 0x86, 0xa0, 0x08, 0x60, 0xd5, 0x01, 0x96, 0xd5, 0x86, 0xa0, 0x08, 0x60, 0xd5, 0x01,
	0x96, 0xd5, 0x86, 0xa0, 0x08, 0x60, 0xd5, 0x01, 0x96, 0xd5, 0x86, 0xa0, 0x08, 0x60, 0xd5, 0x01,
	0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x03, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x00,
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x96, 0xd5, 0x86, 0xa0, 0x08, 0x60, 0xd5, 0x01,
	0x96, 0xd5, 0x86, 0xa0, 0x08, 0x60, 0xd5, 0x01, 0x96, 0xd5, 0x86, 0xa0, 0x08, 0x60, 0xd5, 0x01,
	0x96, 0xd5, 0x86, 0xa0, 0x08, 0x60, 0xd5, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x03, 0x24, 0x00, 0x4d, 0x00, 0x46, 0x00, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x21, 0x04, 0xfa, 0x00, 0x21, 0x3c, 0x85, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xb0, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x05, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x21, 0x01, 0xf9, 0x00, 0x21, 0x01, 0xe7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x21, 0x04, 0xfa, 0x00, 0x21, 0x3c, 0x85, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x01, 0xf9, 0x00, 0x21, 0x01, 0x02, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00 };

uint8_t fsntfs_test_mft_entry_data2[ 1024 ] = {
	0x46, 0x49, 0x4c, 0x45, 0x2a, 0x00, 0x03, 0x00, 0x96, 0x53, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x30, 0x00, 0x01, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x10, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xf6, 0x6e, 0xb6, 0x6b, 0xe8, 0xb3, 0xcb, 0x01,
	0xf6, 0x6e, 0xb6, 0x6b, 0xe8, 0xb3, 0xcb, 0x01, 0xf6, 0x6e, 0xb6, 0x6b, 0xe8, 0xb3, 0xcb, 0x01,
	0xf6, 0x6e, 0xb6, 0x6b, 0xe8, 0xb3, 0xcb, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x03, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x00,
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0xf6, 0x6e, 0xb6, 0x6b, 0xe8, 0xb3, 0xcb, 0x01,
	0xf6, 0x6e, 0xb6, 0x6b, 0xe8, 0xb3, 0xcb, 0x01, 0xf6, 0x6e, 0xb6, 0x6b, 0xe8, 0xb3, 0xcb, 0x01,
	0xf6, 0x6e, 0xb6, 0x6b, 0xe8, 0xb3, 0xcb, 0x01, 0x00, 0xcc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xcc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x03, 0x24, 0x00, 0x4d, 0x00, 0x46, 0x00, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x50, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x68, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x80, 0x48, 0x00, 0x00, 0x00,
	0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x02, 0x00, 0x34, 0x00,
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x89, 0x00, 0x12, 0x00, 0x01, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x05, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x89, 0x00, 0x12, 0x00,
	0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x20, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00,
	0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x20, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00,
	0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x20, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00,
	0x80, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x07, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xe0, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x31, 0x20, 0x21, 0x19, 0x03, 0x12, 0xc6, 0x07, 0x21, 0x21, 0x7a, 0x8a, 0x09, 0x12, 0x40, 0x01,
	0x7e, 0x22, 0x60, 0x02, 0x77, 0x06, 0x00, 0xc1, 0xb0, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x01, 0x41, 0x19, 0x03, 0x00, 0x0d, 0x00,
	0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_mft_entry_check_for_empty_block function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_check_for_empty_block(
     void )
{
	uint8_t mft_entry_data[ 1024 ];

	libcerror_error_t *error = NULL;
	void *memset_result      = NULL;
	int result               = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 mft_entry_data,
	                 0,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_check_for_empty_block(
	          mft_entry_data,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	mft_entry_data[ 1023 ] = 0xff;

	result = libfsntfs_mft_entry_check_for_empty_block(
	          &( mft_entry_data[ 1 ] ),
	          1024 - 1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_check_for_empty_block(
	          fsntfs_test_mft_entry_data1,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_check_for_empty_block(
	          NULL,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_check_for_empty_block(
	          mft_entry_data,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_initialize(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsntfs_mft_entry_t *mft_entry = NULL;
	int result                       = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 5;
	int number_of_memset_fail_tests  = 1;
	int test_number                  = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_initialize(
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	mft_entry = (libfsntfs_mft_entry_t *) 0x12345678UL;

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	mft_entry = NULL;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_mft_entry_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_mft_entry_initialize(
		          &mft_entry,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( mft_entry != NULL )
			{
				libfsntfs_mft_entry_free(
				 &mft_entry,
				 NULL );
			}
		}
		else
		{
			FSNTFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSNTFS_TEST_ASSERT_IS_NULL(
			 "mft_entry",
			 mft_entry );

			FSNTFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_mft_entry_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_mft_entry_initialize(
		          &mft_entry,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( mft_entry != NULL )
			{
				libfsntfs_mft_entry_free(
				 &mft_entry,
				 NULL );
			}
		}
		else
		{
			FSNTFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSNTFS_TEST_ASSERT_IS_NULL(
			 "mft_entry",
			 mft_entry );

			FSNTFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_free(
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_read_data(
     void )
{
	uint8_t mft_entry_data[ 1024 ];

	libcerror_error_t *error         = NULL;
	libfsntfs_mft_entry_t *mft_entry = NULL;
	void *memcpy_result              = NULL;
	void *memset_result              = NULL;
	int result                       = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 mft_entry_data,
	                 0,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular case where MFT entry is empty
	 */
	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	memcpy_result = memory_copy(
	                 mft_entry_data,
	                 fsntfs_test_mft_entry_data1,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memcpy_result",
	 memcpy_result );

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular case where signature is "BAAD"
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( mft_entry_data[ 0 ] ),
	 0x44414142UL );

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( mft_entry_data[ 0 ] ),
	 0x454c4946UL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	memcpy_result = memory_copy(
	                 mft_entry_data,
	                 fsntfs_test_mft_entry_data2,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memcpy_result",
	 memcpy_result );

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	memcpy_result = memory_copy(
	                 mft_entry_data,
	                 fsntfs_test_mft_entry_data1,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memcpy_result",
	 memcpy_result );

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test where signature is invalid
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( mft_entry_data[ 0 ] ),
	 0xffffffffUL );

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( mft_entry_data[ 0 ] ),
	 0x454c4946UL );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	memcpy_result = memory_copy(
	                 mft_entry_data,
	                 fsntfs_test_mft_entry_data1,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memcpy_result",
	 memcpy_result );

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_read_data(
	          NULL,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	mft_entry->header = (libfsntfs_mft_entry_header_t *) 0x12345678UL;

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	mft_entry->header = NULL;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          NULL,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          0,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	/* Test libfsntfs_mft_entry_read_data with malloc failing in libfsntfs_mft_entry_header_initialize
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	if( fsntfs_test_malloc_attempts_before_fail != -1 )
	{
		fsntfs_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		FSNTFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_read_file_io_handle(
     void )
{
	uint8_t mft_entry_data[ 1024 ];

	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libfsntfs_mft_entry_t *mft_entry = NULL;
	void *memcpy_result              = NULL;
	void *memset_result              = NULL;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	memset_result = memory_set(
	                 mft_entry_data,
	                 0,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	result = fsntfs_test_open_file_io_handle(
	          &file_io_handle,
	          mft_entry_data,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_read_file_io_handle(
	          mft_entry,
	          file_io_handle,
	          0,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up file IO handle
	 */
	result = fsntfs_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	memcpy_result = memory_copy(
	                 mft_entry_data,
	                 fsntfs_test_mft_entry_data1,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memcpy_result",
	 memcpy_result );

	result = fsntfs_test_open_file_io_handle(
	          &file_io_handle,
	          mft_entry_data,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_read_file_io_handle(
	          mft_entry,
	          file_io_handle,
	          0,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_read_file_io_handle(
	          mft_entry,
	          file_io_handle,
	          0,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_read_file_io_handle(
	          NULL,
	          file_io_handle,
	          0,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_read_file_io_handle(
	          mft_entry,
	          NULL,
	          0,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_read_file_io_handle(
	          mft_entry,
	          file_io_handle,
	          -1,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test MFT entry size too small
	 */
	result = libfsntfs_mft_entry_read_file_io_handle(
	          mft_entry,
	          file_io_handle,
	          0,
	          8,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	/* Test libfsntfs_mft_entry_read_file_io_handle with malloc failing
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_mft_entry_read_file_io_handle(
	          mft_entry,
	          file_io_handle,
	          0,
	          1024,
	          0,
	          &error );

	if( fsntfs_test_malloc_attempts_before_fail != -1 )
	{
		fsntfs_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		FSNTFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Clean up file IO handle
	 */
	result = fsntfs_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data too small
	 */
	result = fsntfs_test_open_file_io_handle(
	          &file_io_handle,
	          fsntfs_test_mft_entry_data1,
	          8,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_read_file_io_handle(
	          mft_entry,
	          file_io_handle,
	          0,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = fsntfs_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_read_attributes_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_read_attributes_data(
     void )
{
	uint8_t mft_entry_data[ 1024 ];

	libcerror_error_t *error         = NULL;
	libfsntfs_io_handle_t *io_handle = NULL;
	libfsntfs_mft_entry_t *mft_entry = NULL;
	void *memcpy_result              = NULL;
	int result                       = 0;

	/* Initialize test
	 */
	memcpy_result = memory_copy(
	                 mft_entry_data,
	                 fsntfs_test_mft_entry_data1,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memcpy_result",
	 memcpy_result );

	result = libfsntfs_io_handle_initialize(
	          &io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->cluster_block_size = 4096;

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_read_attributes_data(
	          mft_entry,
	          io_handle,
	          mft_entry_data,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_read_attributes_data(
	          NULL,
	          io_handle,
	          mft_entry_data,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_read_attributes_data(
	          mft_entry,
	          io_handle,
	          NULL,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_read_attributes_data(
	          mft_entry,
	          io_handle,
	          mft_entry_data,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_read_attributes_data(
	          mft_entry,
	          io_handle,
	          mft_entry_data,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_io_handle_free(
	          &io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsntfs_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* TODO: add tests for libfsntfs_mft_entry_read_attributes */

/* TODO: add tests for libfsntfs_mft_entry_read_attributes_from_attribute_list */

/* Tests the libfsntfs_mft_entry_is_empty function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_is_empty(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_is_empty(
	          mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_is_empty(
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_is_allocated function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_is_allocated(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_is_allocated(
	          mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_is_allocated(
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_file_reference function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_file_reference(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error = NULL;
	uint64_t file_reference  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_file_reference(
	          mft_entry,
	          &file_reference,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_file_reference(
	          NULL,
	          &file_reference,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_file_reference(
	          mft_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_base_record_file_reference function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_base_record_file_reference(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error            = NULL;
	uint64_t base_record_file_reference = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_base_record_file_reference(
	          mft_entry,
	          &base_record_file_reference,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_base_record_file_reference(
	          NULL,
	          &base_record_file_reference,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_base_record_file_reference(
	          mft_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_journal_sequence_number function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_journal_sequence_number(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error         = NULL;
	uint64_t journal_sequence_number = 0;
	int result                       = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_journal_sequence_number(
	          mft_entry,
	          &journal_sequence_number,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_journal_sequence_number(
	          NULL,
	          &journal_sequence_number,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_journal_sequence_number(
	          mft_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_number_of_attributes function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_number_of_attributes(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error = NULL;
	int number_of_attributes = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_number_of_attributes(
	          mft_entry,
	          &number_of_attributes,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "number_of_attributes",
	 number_of_attributes,
	 4 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_number_of_attributes(
	          NULL,
	          &number_of_attributes,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_number_of_attributes(
	          mft_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_attribute_by_index function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_attribute_by_index(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	int result                               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_attribute_by_index(
	          mft_entry,
	          0,
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_attribute",
	 mft_attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_attribute_by_index(
	          NULL,
	          0,
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_attribute_by_index(
	          mft_entry,
	          -1,
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_attribute_by_index(
	          mft_entry,
	          0,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_number_of_alternate_data_attributes function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_number_of_alternate_data_attributes(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error = NULL;
	int number_of_attributes = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
	          mft_entry,
	          &number_of_attributes,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "number_of_attributes",
	 number_of_attributes,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
	          NULL,
	          &number_of_attributes,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
	          mft_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_mft_entry_get_alternate_data_attribute_by_index function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_mft_entry_get_alternate_data_attribute_by_index(
     libfsntfs_mft_entry_t *mft_entry )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	int result                               = 0;

	/* Test regular cases
	 */
/* TODO change test data and enable test
	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_index(
	          mft_entry,
	          0,
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_attribute",
	 mft_attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_index(
	          NULL,
	          0,
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_index(
	          mft_entry,
	          -1,
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_index(
	          mft_entry,
	          0,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	return( 0 );
}

/* TODO: add tests for libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name */

/* TODO: add tests for libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name */

/* TODO: add tests for libfsntfs_mft_entry_set_attribute_helper_values */

/* TODO: add tests for libfsntfs_mft_entry_set_data_attribute_helper_values */

/* TODO: add tests for libfsntfs_mft_entry_get_data_attribute_by_utf8_name */

/* TODO: add tests for libfsntfs_mft_entry_has_directory_entries_index */

/* TODO: add tests for libfsntfs_mft_entry_read_element_data */

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSNTFS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSNTFS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSNTFS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSNTFS_TEST_ATTRIBUTE_UNUSED )
#endif
{
#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	uint8_t mft_entry_data[ 1024 ];

	libcerror_error_t *error         = NULL;
	libfsntfs_io_handle_t *io_handle = NULL;
	libfsntfs_mft_entry_t *mft_entry = NULL;
	void *memcpy_result              = NULL;
	int result                       = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( HAVE_DEBUG_OUTPUT ) && defined( FSNTFS_TEST_MFT_ENTRY_VERBOSE )
	libfsntfs_notify_set_verbose(
	 1 );
	libfsntfs_notify_set_stream(
	 stderr,
	 NULL );
#endif

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_check_for_empty_block",
	 fsntfs_test_mft_entry_check_for_empty_block );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_initialize",
	 fsntfs_test_mft_entry_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_free",
	 fsntfs_test_mft_entry_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_read_data",
	 fsntfs_test_mft_entry_read_data );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_read_file_io_handle",
	 fsntfs_test_mft_entry_read_file_io_handle );

	FSNTFS_TEST_RUN(
	 "libfsntfs_mft_entry_read_attributes_data",
	 fsntfs_test_mft_entry_read_attributes_data );

	/* TODO: add tests for libfsntfs_mft_entry_read_attributes */

	/* TODO: add tests for libfsntfs_mft_entry_read_attributes_from_attribute_list */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	memcpy_result = memory_copy(
	                 mft_entry_data,
	                 fsntfs_test_mft_entry_data1,
	                 sizeof( uint8_t ) * 1024 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "memcpy_result",
	 memcpy_result );

	result = libfsntfs_io_handle_initialize(
	          &io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->cluster_block_size = 4096;

	result = libfsntfs_mft_entry_initialize(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_read_data(
	          mft_entry,
	          mft_entry_data,
	          1024,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_entry_read_attributes_data(
	          mft_entry,
	          io_handle,
	          mft_entry_data,
	          1024,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_is_empty",
	 fsntfs_test_mft_entry_is_empty,
	 mft_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_is_allocated",
	 fsntfs_test_mft_entry_is_allocated,
	 mft_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_get_file_reference",
	 fsntfs_test_mft_entry_get_file_reference,
	 mft_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_get_base_record_file_reference",
	 fsntfs_test_mft_entry_get_base_record_file_reference,
	 mft_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_get_journal_sequence_number",
	 fsntfs_test_mft_entry_get_journal_sequence_number,
	 mft_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_get_number_of_attributes",
	 fsntfs_test_mft_entry_get_number_of_attributes,
	 mft_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_get_attribute_by_index",
	 fsntfs_test_mft_entry_get_attribute_by_index,
	 mft_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_get_number_of_alternate_data_attributes",
	 fsntfs_test_mft_entry_get_number_of_alternate_data_attributes,
	 mft_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_mft_entry_get_alternate_data_attribute_by_index",
	 fsntfs_test_mft_entry_get_alternate_data_attribute_by_index,
	 mft_entry );

	/* TODO: add tests for libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name */

	/* TODO: add tests for libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name */

	/* TODO: add tests for libfsntfs_mft_entry_set_attribute_helper_values */

	/* TODO: add tests for libfsntfs_mft_entry_set_data_attribute_helper_values */

	/* TODO: add tests for libfsntfs_mft_entry_get_data_attribute_by_utf8_name */

	/* TODO: add tests for libfsntfs_mft_entry_has_directory_entries_index */

	/* TODO: add tests for libfsntfs_mft_entry_read_element_data */

	/* Clean up
	 */
	result = libfsntfs_mft_entry_free(
	          &mft_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_entry",
	 mft_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_io_handle_free(
	          &io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsntfs_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

