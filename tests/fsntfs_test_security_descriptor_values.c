/*
 * Library security_descriptor_values type test program
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

#include "fsntfs_test_functions.h"
#include "fsntfs_test_libbfio.h"
#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfdata.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_io_handle.h"
#include "../libfsntfs/libfsntfs_mft_attribute.h"
#include "../libfsntfs/libfsntfs_security_descriptor_values.h"

uint8_t fsntfs_test_security_descriptor_values_data1[ 128 ] = {
	0x50, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x03, 0x00,
	0x64, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x80, 0x48, 0x00, 0x00, 0x00,
	0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x02, 0x00, 0x34, 0x00,
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x89, 0x00, 0x12, 0x00, 0x01, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x05, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x89, 0x00, 0x12, 0x00,
	0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x20, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00,
	0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x12, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x05, 0x20, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Reads data from the current offset into a buffer
 * Callback for the compressed block descriptor data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t fsntfs_test_security_descriptor_values_read_segment_data(
         intptr_t *data_handle FSNTFS_TEST_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index FSNTFS_TEST_ATTRIBUTE_UNUSED,
         int segment_file_index FSNTFS_TEST_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags FSNTFS_TEST_ATTRIBUTE_UNUSED,
         uint8_t read_flags FSNTFS_TEST_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	static char *function = "fsntfs_test_security_descriptor_values_read_segment_data";
	ssize_t read_count    = 0;

	FSNTFS_TEST_UNREFERENCED_PARAMETER( data_handle )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( segment_index )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( segment_file_index )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( segment_flags )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( read_flags )

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              segment_data,
	              segment_data_size,
	              error );

	if( read_count != (ssize_t) segment_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment data.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain offset of the data
 * Callback for the cluster block stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t fsntfs_test_security_descriptor_values_seek_segment_offset(
         intptr_t *data_handle FSNTFS_TEST_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index FSNTFS_TEST_ATTRIBUTE_UNUSED,
         int segment_file_index FSNTFS_TEST_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "fsntfs_test_security_descriptor_values_seek_segment_offset";

	FSNTFS_TEST_UNREFERENCED_PARAMETER( data_handle )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( segment_index )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( segment_file_index )

	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     segment_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek segment offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 segment_offset,
		 segment_offset );

		return( -1 );
	}
	return( segment_offset );
}

/* Tests the libfsntfs_security_descriptor_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_values_initialize(
     void )
{
	libcerror_error_t *error                                           = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values = NULL;
	int result                                                         = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                                    = 1;
	int number_of_memset_fail_tests                                    = 1;
	int test_number                                                    = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_security_descriptor_values_free(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_security_descriptor_values_initialize(
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

	security_descriptor_values = (libfsntfs_security_descriptor_values_t *) 0x12345678UL;

	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	security_descriptor_values = NULL;

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
		/* Test libfsntfs_security_descriptor_values_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_security_descriptor_values_initialize(
		          &security_descriptor_values,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( security_descriptor_values != NULL )
			{
				libfsntfs_security_descriptor_values_free(
				 &security_descriptor_values,
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
			 "security_descriptor_values",
			 security_descriptor_values );

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
		/* Test libfsntfs_security_descriptor_values_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_security_descriptor_values_initialize(
		          &security_descriptor_values,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( security_descriptor_values != NULL )
			{
				libfsntfs_security_descriptor_values_free(
				 &security_descriptor_values,
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
			 "security_descriptor_values",
			 security_descriptor_values );

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
	if( security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &security_descriptor_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_security_descriptor_values_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_values_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_security_descriptor_values_free(
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

/* Tests the libfsntfs_security_descriptor_values_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_values_read_data(
     void )
{
	libcerror_error_t *error                                           = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values = NULL;
	int result                                                         = 0;

	/* Initialize test
	 */
	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_values_read_data(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
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
	result = libfsntfs_security_descriptor_values_read_data(
	          NULL,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
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

	result = libfsntfs_security_descriptor_values_read_data(
	          security_descriptor_values,
	          NULL,
	          100,
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

	result = libfsntfs_security_descriptor_values_read_data(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
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

	/* Clean up
	 */
	result = libfsntfs_security_descriptor_values_free(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

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
	if( security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &security_descriptor_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_security_descriptor_values_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_values_read_buffer(
     void )
{
	libcerror_error_t *error                                           = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values = NULL;
	int result                                                         = 0;

	/* Initialize test
	 */
	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_values_read_buffer(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
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
	result = libfsntfs_security_descriptor_values_read_buffer(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
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
	result = libfsntfs_security_descriptor_values_free(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_security_descriptor_values_read_buffer(
	          NULL,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
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

	result = libfsntfs_security_descriptor_values_read_buffer(
	          security_descriptor_values,
	          NULL,
	          100,
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

	result = libfsntfs_security_descriptor_values_read_buffer(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
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

	result = libfsntfs_security_descriptor_values_read_buffer(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
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

	/* Test libfsntfs_security_descriptor_values_read_buffer with malloc failing
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_security_descriptor_values_read_buffer(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
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
#if defined( OPTIMIZATION_DISABLED )
	/* Test libfsntfs_security_descriptor_values_read_buffer with memcpy failing
	 */
	fsntfs_test_memcpy_attempts_before_fail = 0;

	result = libfsntfs_security_descriptor_values_read_buffer(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
	          &error );

	if( fsntfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsntfs_test_memcpy_attempts_before_fail = -1;
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
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Test libfsntfs_security_descriptor_values_read_buffer with libfsntfs_security_descriptor_values_read_data failing
	 */
/* TODO implement */

	/* Clean up
	 */
	result = libfsntfs_security_descriptor_values_free(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

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
	if( security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &security_descriptor_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_security_descriptor_values_read_stream function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_values_read_stream(
     void )
{
	libbfio_handle_t *file_io_handle                                   = NULL;
	libcerror_error_t *error                                           = NULL;
	libfdata_stream_t *data_stream                                     = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values = NULL;
	int element_index                                                  = 0;
	int result                                                         = 0;

	/* Initialize test
	 */
	result = fsntfs_test_open_file_io_handle(
	          &file_io_handle,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
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

	result = libfdata_stream_initialize(
	          &data_stream,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &fsntfs_test_security_descriptor_values_read_segment_data,
	          NULL,
	          (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &fsntfs_test_security_descriptor_values_seek_segment_offset,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_stream",
	 data_stream );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_stream_append_segment(
	          data_stream,
	          &element_index,
	          0,
	          0,
	          100,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_values_read_stream(
	          security_descriptor_values,
	          file_io_handle,
	          data_stream,
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
	result = libfsntfs_security_descriptor_values_read_stream(
	          security_descriptor_values,
	          file_io_handle,
	          data_stream,
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
	result = libfsntfs_security_descriptor_values_free(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_security_descriptor_values_read_stream(
	          NULL,
	          file_io_handle,
	          data_stream,
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

/* TODO implement */

	/* Clean up
	 */
	result = libfsntfs_security_descriptor_values_free(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_stream_free(
	          &data_stream,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "data_stream",
	 data_stream );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "file_io_handle",
	 file_io_handle );

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
	if( security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &security_descriptor_values,
		 NULL );
	}
	if( data_stream != NULL )
	{
		libfdata_stream_free(
		 &data_stream,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_security_descriptor_values_read_from_mft_attribute function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_values_read_from_mft_attribute(
     void )
{
	libcerror_error_t *error                                           = NULL;
	libfsntfs_io_handle_t *io_handle                                   = NULL;
	libfsntfs_mft_attribute_t *mft_attribute                           = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values = NULL;
	int result                                                         = 0;

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
	          fsntfs_test_security_descriptor_values_data1,
	          128,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_values_read_from_mft_attribute(
	          security_descriptor_values,
	          mft_attribute,
	          io_handle,
	          NULL,
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
	result = libfsntfs_security_descriptor_values_read_from_mft_attribute(
	          NULL,
	          mft_attribute,
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

	result = libfsntfs_security_descriptor_values_read_from_mft_attribute(
	          security_descriptor_values,
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
	result = libfsntfs_security_descriptor_values_free(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

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
	if( security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &security_descriptor_values,
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

/* Tests the libfsntfs_security_descriptor_values_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_values_get_data_size(
     libfsntfs_security_descriptor_values_t *security_descriptor_values )
{
	libcerror_error_t *error = NULL;
	size_t data_size         = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_values_get_data_size(
	          security_descriptor_values,
	          &data_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "data_size",
	 data_size,
	 (size_t) 100 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_security_descriptor_values_get_data_size(
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

	result = libfsntfs_security_descriptor_values_get_data_size(
	          security_descriptor_values,
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

/* Tests the libfsntfs_security_descriptor_values_get_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_security_descriptor_values_get_data(
     libfsntfs_security_descriptor_values_t *security_descriptor_values )
{
	uint8_t data[ 128 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_security_descriptor_values_get_data(
	          security_descriptor_values,
	          data,
	          128,
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
	result = libfsntfs_security_descriptor_values_get_data(
	          NULL,
	          data,
	          128,
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

	result = libfsntfs_security_descriptor_values_get_data(
	          security_descriptor_values,
	          NULL,
	          128,
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

	result = libfsntfs_security_descriptor_values_get_data(
	          security_descriptor_values,
	          data,
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

	result = libfsntfs_security_descriptor_values_get_data(
	          security_descriptor_values,
	          data,
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

#if defined( HAVE_FSNTFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libfsntfs_security_descriptor_values_get_data with memcpy failing
	 */
	fsntfs_test_memcpy_attempts_before_fail = 0;

	result = libfsntfs_security_descriptor_values_get_data(
	          security_descriptor_values,
	          data,
	          128,
	          &error );

	if( fsntfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsntfs_test_memcpy_attempts_before_fail = -1;
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
#endif /* defined( OPTIMIZATION_DISABLED ) && defined( HAVE_FSNTFS_TEST_MEMORY ) */

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
	libcerror_error_t *error                                           = NULL;
	libfsntfs_security_descriptor_values_t *security_descriptor_values = NULL;
	int result                                                         = 0;
#endif

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_values_initialize",
	 fsntfs_test_security_descriptor_values_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_values_free",
	 fsntfs_test_security_descriptor_values_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_values_read_data",
	 fsntfs_test_security_descriptor_values_read_data );

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_values_read_buffer",
	 fsntfs_test_security_descriptor_values_read_buffer );

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_values_read_stream",
	 fsntfs_test_security_descriptor_values_read_stream );

	FSNTFS_TEST_RUN(
	 "libfsntfs_security_descriptor_values_read_from_mft_attribute",
	 fsntfs_test_security_descriptor_values_read_from_mft_attribute );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize security_descriptor_values for tests
	 */
	result = libfsntfs_security_descriptor_values_initialize(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_security_descriptor_values_read_buffer(
	          security_descriptor_values,
	          &( fsntfs_test_security_descriptor_values_data1[ 24 ] ),
	          100,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_security_descriptor_values_get_data_size",
	 fsntfs_test_security_descriptor_values_get_data_size,
	 security_descriptor_values );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_security_descriptor_values_get_data",
	 fsntfs_test_security_descriptor_values_get_data,
	 security_descriptor_values );

	/* Clean up
	 */
	result = libfsntfs_security_descriptor_values_free(
	          &security_descriptor_values,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "security_descriptor_values",
	 security_descriptor_values );

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
	if( security_descriptor_values != NULL )
	{
		libfsntfs_security_descriptor_values_free(
		 &security_descriptor_values,
		 NULL );
	}
#endif
	return( EXIT_FAILURE );
}

