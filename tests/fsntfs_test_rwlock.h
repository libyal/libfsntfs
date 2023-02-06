/*
 * Read/Write lock functions for testing
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

#if !defined( _FSNTFS_TEST_RWLOCK_H )
#define _FSNTFS_TEST_RWLOCK_H

#include <common.h>

#include "fsntfs_test_libfsntfs.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( LIBFSNTFS_HAVE_MULTI_THREAD_SUPPORT ) && defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
#define HAVE_FSNTFS_TEST_RWLOCK		1
#endif

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

extern int fsntfs_test_pthread_rwlock_init_attempts_before_fail;

extern int fsntfs_test_pthread_rwlock_destroy_attempts_before_fail;

extern int fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail;

extern int fsntfs_test_pthread_rwlock_wrlock_attempts_before_fail;

extern int fsntfs_test_pthread_rwlock_unlock_attempts_before_fail;

#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_TEST_RWLOCK_H ) */

