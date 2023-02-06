/*
 * Memory allocation functions for testing
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#include "fsntfs_test_memory.h"

#if defined( HAVE_FSNTFS_TEST_MEMORY )

static void *(*fsntfs_test_real_malloc)(size_t)                       = NULL;
static void *(*fsntfs_test_real_memcpy)(void *, const void *, size_t) = NULL;
static void *(*fsntfs_test_real_memset)(void *, int, size_t)          = NULL;
static void *(*fsntfs_test_real_realloc)(void *, size_t)              = NULL;

int fsntfs_test_malloc_attempts_before_fail                           = -1;
int fsntfs_test_memcpy_attempts_before_fail                           = -1;
int fsntfs_test_memset_attempts_before_fail                           = -1;
int fsntfs_test_realloc_attempts_before_fail                          = -1;

/* Custom malloc for testing memory error cases
 * Note this function might fail if compiled with optimation
 * Returns a pointer to newly allocated data or NULL
 */
void *malloc(
       size_t size )
{
	void *ptr = NULL;

	if( fsntfs_test_real_malloc == NULL )
	{
		fsntfs_test_real_malloc = dlsym(
		                           RTLD_NEXT,
		                           "malloc" );
	}
	if( fsntfs_test_malloc_attempts_before_fail == 0 )
	{
		fsntfs_test_malloc_attempts_before_fail = -1;

		return( NULL );
	}
	else if( fsntfs_test_malloc_attempts_before_fail > 0 )
	{
		fsntfs_test_malloc_attempts_before_fail--;
	}
	ptr = fsntfs_test_real_malloc(
	       size );

	return( ptr );
}

/* Custom memcpy for testing memory error cases
 * Note this function might fail if compiled with optimation and as a shared libary
 * Returns a pointer to newly allocated data or NULL
 */
void *memcpy(
       void *destination,
       const void *source,
       size_t size )
{
	if( fsntfs_test_real_memcpy == NULL )
	{
		fsntfs_test_real_memcpy = dlsym(
		                           RTLD_NEXT,
		                           "memcpy" );
	}
	if( fsntfs_test_memcpy_attempts_before_fail == 0 )
	{
		fsntfs_test_memcpy_attempts_before_fail = -1;

		return( NULL );
	}
	else if( fsntfs_test_memcpy_attempts_before_fail > 0 )
	{
		fsntfs_test_memcpy_attempts_before_fail--;
	}
	destination = fsntfs_test_real_memcpy(
	               destination,
	               source,
	               size );

	return( destination );
}

/* Custom memset for testing memory error cases
 * Note this function might fail if compiled with optimation and as a shared libary
 * Returns a pointer to newly allocated data or NULL
 */
void *memset(
       void *ptr,
       int constant,
       size_t size )
{
	if( fsntfs_test_real_memset == NULL )
	{
		fsntfs_test_real_memset = dlsym(
		                           RTLD_NEXT,
		                           "memset" );
	}
	if( fsntfs_test_memset_attempts_before_fail == 0 )
	{
		fsntfs_test_memset_attempts_before_fail = -1;

		return( NULL );
	}
	else if( fsntfs_test_memset_attempts_before_fail > 0 )
	{
		fsntfs_test_memset_attempts_before_fail--;
	}
	ptr = fsntfs_test_real_memset(
	       ptr,
	       constant,
	       size );

	return( ptr );
}

/* Custom realloc for testing memory error cases
 * Note this function might fail if compiled with optimation
 * Returns a pointer to reallocated data or NULL
 */
void *realloc(
       void *ptr,
       size_t size )
{
	if( fsntfs_test_real_realloc == NULL )
	{
		fsntfs_test_real_realloc = dlsym(
		                            RTLD_NEXT,
		                            "realloc" );
	}
	if( fsntfs_test_realloc_attempts_before_fail == 0 )
	{
		fsntfs_test_realloc_attempts_before_fail = -1;

		return( NULL );
	}
	else if( fsntfs_test_realloc_attempts_before_fail > 0 )
	{
		fsntfs_test_realloc_attempts_before_fail--;
	}
	ptr = fsntfs_test_real_realloc(
	       ptr,
	       size );

	return( ptr );
}

#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

