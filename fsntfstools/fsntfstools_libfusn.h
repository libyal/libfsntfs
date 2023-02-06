/*
 * The libfusn header wrapper
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

#if !defined( _FSNTFSTOOLS_LIBFUSN_H )
#define _FSNTFSTOOLS_LIBFUSN_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFUSN for local use of libfusn
 */
#if defined( HAVE_LOCAL_LIBFUSN )

#include <libfusn_definitions.h>
#include <libfusn_record.h>
#include <libfusn_types.h>

#else

/* If libtool DLL support is enabled set LIBFUSN_DLL_IMPORT
 * before including libfusn.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT ) && !defined( HAVE_STATIC_EXECUTABLES )
#define LIBFUSN_DLL_IMPORT
#endif

#include <libfusn.h>

#endif /* defined( HAVE_LOCAL_LIBFUSN ) */

#endif /* !defined( _FSNTFSTOOLS_LIBFUSN_H ) */

