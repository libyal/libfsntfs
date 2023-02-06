/*
 * Library attribute type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_rwlock.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_attribute.h"
#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_mft_attribute.h"

/* $BITMAP attribute
 */
uint8_t fsntfs_test_attribute_data1[ 40 ] = {
	0xb0, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x24, 0x00, 0x49, 0x00, 0x33, 0x00, 0x30, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_attribute_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_initialize(
     void )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_attribute_t *attribute         = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	int result                               = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests          = 2;
	int number_of_memset_fail_tests          = 1;
	int test_number                          = 0;
#endif

	/* Initialize test
	 */
	result = libfsntfs_mft_attribute_initialize(
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

	/* Test regular cases
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_internal_attribute_free(
	          (libfsntfs_internal_attribute_t **) &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_initialize(
	          NULL,
	          mft_attribute,
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

	attribute = (libfsntfs_attribute_t *) 0x12345678UL;

	result = libfsntfs_attribute_initialize(
	          &attribute,
	          mft_attribute,
	          &error );

	attribute = NULL;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_initialize(
	          &attribute,
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

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_attribute_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_attribute_initialize(
		          &attribute,
		          mft_attribute,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( attribute != NULL )
			{
				libfsntfs_internal_attribute_free(
				 (libfsntfs_internal_attribute_t **) &attribute,
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
			 "attribute",
			 attribute );

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
		/* Test libfsntfs_attribute_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_attribute_initialize(
		          &attribute,
		          mft_attribute,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( attribute != NULL )
			{
				libfsntfs_internal_attribute_free(
				 (libfsntfs_internal_attribute_t **) &attribute,
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
			 "attribute",
			 attribute );

			FSNTFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfsntfs_mft_attribute_free(
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_attribute",
	 mft_attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_internal_attribute_free(
		 (libfsntfs_internal_attribute_t **) &attribute,
		 NULL );
	}
	if( mft_attribute != NULL )
	{
		libfsntfs_mft_attribute_free(
		 &mft_attribute,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

/* Tests the libfsntfs_attribute_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_free(
     void )
{
	libfsntfs_attribute_t *attribute = NULL;
	libcerror_error_t *error         = NULL;
	int result                       = 0;

	/* Test regular cases
	 */
	attribute = (libfsntfs_attribute_t *) 0x12345678UL;

	result = libfsntfs_attribute_free(
	          &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_free(
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

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_internal_attribute_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_internal_attribute_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_internal_attribute_free(
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

/* Tests the libfsntfs_internal_attribute_read_value function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_internal_attribute_read_value(
     void )
{
	libcerror_error_t *error                 = NULL;
	libfsntfs_attribute_t *attribute         = NULL;
	libfsntfs_io_handle_t *io_handle         = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	int result                               = 0;

	/* Initialize test
	 */
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

	result = libfsntfs_mft_attribute_initialize(
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

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_attribute_data1,
	          40,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_initialize(
	          &attribute,
	          mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_internal_attribute_read_value(
	          (libfsntfs_internal_attribute_t *) attribute,
	          io_handle,
	          NULL,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_internal_attribute_read_value(
	          (libfsntfs_internal_attribute_t *) attribute,
	          io_handle,
	          NULL,
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
	result = libfsntfs_internal_attribute_free(
	          (libfsntfs_internal_attribute_t **) &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsntfs_attribute_initialize(
	          &attribute,
	          mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_internal_attribute_read_value(
	          NULL,
	          io_handle,
	          NULL,
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
	result = libfsntfs_internal_attribute_free(
	          (libfsntfs_internal_attribute_t **) &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_free(
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_attribute",
	 mft_attribute );

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
	if( attribute != NULL )
	{
		libfsntfs_internal_attribute_free(
		 (libfsntfs_internal_attribute_t **) &attribute,
		 NULL );
	}
	if( mft_attribute != NULL )
	{
		libfsntfs_mft_attribute_free(
		 &mft_attribute,
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

/* Tests the libfsntfs_internal_attribute_get_type function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_internal_attribute_get_type(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	uint32_t type            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_internal_attribute_get_type(
	          (libfsntfs_internal_attribute_t *) attribute,
	          &type,
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
	result = libfsntfs_internal_attribute_get_type(
	          NULL,
	          &type,
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

	result = libfsntfs_internal_attribute_get_type(
	          (libfsntfs_internal_attribute_t *) attribute,
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

/* Tests the libfsntfs_attribute_get_type function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_type(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	uint32_t type            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_type(
	          attribute,
	          &type,
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
	result = libfsntfs_attribute_get_type(
	          NULL,
	          &type,
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

	result = libfsntfs_attribute_get_type(
	          attribute,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_type with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_type(
	          attribute,
	          &type,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_type with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_type(
	          attribute,
	          &type,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_data_flags function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_data_flags(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	uint16_t data_flags      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_data_flags(
	          attribute,
	          &data_flags,
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
	result = libfsntfs_attribute_get_data_flags(
	          NULL,
	          &data_flags,
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

	result = libfsntfs_attribute_get_data_flags(
	          attribute,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_data_flags with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_data_flags(
	          attribute,
	          &data_flags,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_data_flags with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_data_flags(
	          attribute,
	          &data_flags,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_internal_attribute_get_value function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_internal_attribute_get_value(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	intptr_t *value          = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_internal_attribute_get_value(
	          (libfsntfs_internal_attribute_t *) attribute,
	          &value,
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
	result = libfsntfs_internal_attribute_get_value(
	          NULL,
	          &value,
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

	result = libfsntfs_internal_attribute_get_value(
	          (libfsntfs_internal_attribute_t *) attribute,
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

/* Tests the libfsntfs_attribute_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_utf8_name_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	size_t utf8_name_size    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_utf8_name_size(
	          attribute,
	          &utf8_name_size,
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
	result = libfsntfs_attribute_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
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

	result = libfsntfs_attribute_get_utf8_name_size(
	          attribute,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_utf8_name_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_utf8_name_size(
	          attribute,
	          &utf8_name_size,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_utf8_name_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_utf8_name_size(
	          attribute,
	          &utf8_name_size,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_utf8_name(
     libfsntfs_attribute_t *attribute )
{
	uint8_t utf8_name[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_utf8_name(
	          attribute,
	          utf8_name,
	          64,
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
	result = libfsntfs_attribute_get_utf8_name(
	          NULL,
	          utf8_name,
	          64,
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

	result = libfsntfs_attribute_get_utf8_name(
	          attribute,
	          NULL,
	          64,
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

	result = libfsntfs_attribute_get_utf8_name(
	          attribute,
	          utf8_name,
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

	result = libfsntfs_attribute_get_utf8_name(
	          attribute,
	          utf8_name,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_utf8_name with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_utf8_name(
	          attribute,
	          utf8_name,
	          64,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_utf8_name with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_utf8_name(
	          attribute,
	          utf8_name,
	          64,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_utf16_name_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	size_t utf16_name_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_utf16_name_size(
	          attribute,
	          &utf16_name_size,
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
	result = libfsntfs_attribute_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
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

	result = libfsntfs_attribute_get_utf16_name_size(
	          attribute,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_utf16_name_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_utf16_name_size(
	          attribute,
	          &utf16_name_size,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_utf16_name_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_utf16_name_size(
	          attribute,
	          &utf16_name_size,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_utf16_name(
     libfsntfs_attribute_t *attribute )
{
	uint16_t utf16_name[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_utf16_name(
	          attribute,
	          utf16_name,
	          64,
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
	result = libfsntfs_attribute_get_utf16_name(
	          NULL,
	          utf16_name,
	          64,
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

	result = libfsntfs_attribute_get_utf16_name(
	          attribute,
	          NULL,
	          64,
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

	result = libfsntfs_attribute_get_utf16_name(
	          attribute,
	          utf16_name,
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

	result = libfsntfs_attribute_get_utf16_name(
	          attribute,
	          utf16_name,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_utf16_name with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_utf16_name(
	          attribute,
	          utf16_name,
	          64,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_utf16_name with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_utf16_name(
	          attribute,
	          utf16_name,
	          64,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_data_vcn_range function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_data_vcn_range(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	uint64_t data_first_vcn  = 0;
	uint64_t data_last_vcn   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_data_vcn_range(
	          attribute,
	          &data_first_vcn,
	          &data_last_vcn,
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
	result = libfsntfs_attribute_get_data_vcn_range(
	          NULL,
	          &data_first_vcn,
	          &data_last_vcn,
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

	result = libfsntfs_attribute_get_data_vcn_range(
	          attribute,
	          NULL,
	          &data_last_vcn,
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

	result = libfsntfs_attribute_get_data_vcn_range(
	          attribute,
	          &data_first_vcn,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_data_vcn_range with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_data_vcn_range(
	          attribute,
	          &data_first_vcn,
	          &data_last_vcn,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_data_vcn_range with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_data_vcn_range(
	          attribute,
	          &data_first_vcn,
	          &data_last_vcn,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_internal_attribute_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_internal_attribute_get_data_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	size64_t data_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_internal_attribute_get_data_size(
	          (libfsntfs_internal_attribute_t *) attribute,
	          &data_size,
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
	result = libfsntfs_internal_attribute_get_data_size(
	          NULL,
	          &data_size,
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

	result = libfsntfs_internal_attribute_get_data_size(
	          (libfsntfs_internal_attribute_t *) attribute,
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

/* Tests the libfsntfs_attribute_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_data_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	size64_t data_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_data_size(
	          attribute,
	          &data_size,
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
	result = libfsntfs_attribute_get_data_size(
	          NULL,
	          &data_size,
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

	result = libfsntfs_attribute_get_data_size(
	          attribute,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_data_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_data_size(
	          attribute,
	          &data_size,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_data_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_data_size(
	          attribute,
	          &data_size,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_get_valid_data_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_get_valid_data_size(
     libfsntfs_attribute_t *attribute )
{
	libcerror_error_t *error = NULL;
	size64_t valid_data_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_get_valid_data_size(
	          attribute,
	          &valid_data_size,
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
	result = libfsntfs_attribute_get_valid_data_size(
	          NULL,
	          &valid_data_size,
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

	result = libfsntfs_attribute_get_valid_data_size(
	          attribute,
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

#if defined( HAVE_FSNTFS_TEST_RWLOCK )

	/* Test libfsntfs_attribute_get_valid_data_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_valid_data_size(
	          attribute,
	          &valid_data_size,
	          &error );

	if( fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libfsntfs_attribute_get_valid_data_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libfsntfs_attribute_get_valid_data_size(
	          attribute,
	          &valid_data_size,
	          &error );

	if( fsntfs_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fsntfs_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_FSNTFS_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

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

	libcerror_error_t *error                 = NULL;
	libfsntfs_attribute_t *attribute         = NULL;
	libfsntfs_io_handle_t *io_handle         = NULL;
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	int result                               = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_initialize",
	 fsntfs_test_attribute_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_free",
	 fsntfs_test_attribute_free );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_internal_attribute_free",
	 fsntfs_test_internal_attribute_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_internal_attribute_read_value",
	 fsntfs_test_internal_attribute_read_value );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
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

	result = libfsntfs_mft_attribute_initialize(
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

	result = libfsntfs_mft_attribute_read_data(
	          mft_attribute,
	          io_handle,
	          fsntfs_test_attribute_data1,
	          40,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_initialize(
	          &attribute,
	          mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_internal_attribute_get_type",
	 fsntfs_test_internal_attribute_get_type,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_type",
	 fsntfs_test_attribute_get_type,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_data_flags",
	 fsntfs_test_attribute_get_data_flags,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_internal_attribute_get_value",
	 fsntfs_test_internal_attribute_get_value,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_utf8_name_size",
	 fsntfs_test_attribute_get_utf8_name_size,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_utf8_name",
	 fsntfs_test_attribute_get_utf8_name,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_utf16_name_size",
	 fsntfs_test_attribute_get_utf16_name_size,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_utf16_name",
	 fsntfs_test_attribute_get_utf16_name,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_data_vcn_range",
	 fsntfs_test_attribute_get_data_vcn_range,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_internal_attribute_get_data_size",
	 fsntfs_test_internal_attribute_get_data_size,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_data_size",
	 fsntfs_test_attribute_get_data_size,
	 attribute );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_get_valid_data_size",
	 fsntfs_test_attribute_get_valid_data_size,
	 attribute );

	/* Clean up
	 */
	result = libfsntfs_internal_attribute_free(
		 (libfsntfs_internal_attribute_t **) &attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_mft_attribute_free(
	          &mft_attribute,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "mft_attribute",
	 mft_attribute );

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

on_error:
#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( attribute != NULL )
	{
		libfsntfs_internal_attribute_free(
		 (libfsntfs_internal_attribute_t **) &attribute,
		 NULL );
	}
	if( mft_attribute != NULL )
	{
		libfsntfs_mft_attribute_free(
		 &mft_attribute,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsntfs_io_handle_free(
		 &io_handle,
		 NULL );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

