/*
 * Library name functions test program
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
#include "fsntfs_test_libuna.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_name.h"

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_name_compare function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_name_compare(
     void )
{
        uint8_t utf16_stream_equal[ 12 ]       = { 'e', 0, 'q', 0, 'u', 0, 'a', 0, 'l', 0, 0 };
        uint8_t utf16_stream_equal_upper[ 12 ] = { 'E', 0, 'Q', 0, 'U', 0, 'A', 0, 'L', 0, 0 };
        uint8_t utf16_stream_great[ 12 ]       = { 'g', 0, 'r', 0, 'e', 0, 'a', 0, 't', 0, 0, 0 };
        uint8_t utf16_stream_greater[ 16 ]     = { 'g', 0, 'r', 0, 'e', 0, 'a', 0, 't', 0, 'e', 0, 'r', 0, 0, 0 };
        uint8_t utf16_stream_less[ 10 ]        = { 'l', 0, 'e', 0, 's', 0, 's', 0, 0, 0 };
        uint8_t utf16_stream_lesser[ 14 ]      = { 'l', 0, 'e', 0, 's', 0, 's', 0, 'e', 0, 'r', 0, 0, 0 };
        uint8_t utf16_stream_more[ 10 ]        = { 'm', 0, 'o', 0, 'r', 0, 'e', 0, 0, 0 };
	libcerror_error_t *error               = NULL;
	int result                             = 0;

	/* Test regular cases
	 */
	result = libfsntfs_name_compare(
	          utf16_stream_equal,
	          12,
	          utf16_stream_equal,
	          12,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare(
	          utf16_stream_equal,
	          12,
	          utf16_stream_equal,
	          12,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare(
	          utf16_stream_equal,
	          12,
	          utf16_stream_equal_upper,
	          12,
	          1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare(
	          utf16_stream_great,
	          12,
	          utf16_stream_greater,
	          16,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare(
	          utf16_stream_less,
	          10,
	          utf16_stream_more,
	          10,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare(
	          utf16_stream_more,
	          10,
	          utf16_stream_less,
	          10,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare(
	          utf16_stream_lesser,
	          14,
	          utf16_stream_less,
	          10,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_name_compare(
	          NULL,
	          12,
	          utf16_stream_equal,
	          12,
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

	result = libfsntfs_name_compare(
	          utf16_stream_equal,
	          (size_t) SSIZE_MAX + 1,
	          utf16_stream_equal,
	          12,
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

	result = libfsntfs_name_compare(
	          utf16_stream_equal,
	          12,
	          NULL,
	          12,
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

	result = libfsntfs_name_compare(
	          utf16_stream_equal,
	          12,
	          utf16_stream_equal,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_name_compare_short function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_name_compare_short(
     void )
{
        uint8_t utf16_stream_long1[ 46 ] = {
		'D', 0, 'o', 0, 'c', 0, 'u', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0,
		's', 0, ' ', 0, 'a', 0, 'n', 0, 'd', 0, ' ', 0, 'S', 0, 'e', 0,
		't', 0, 't', 0, 'i', 0, 'n', 0, 'g', 0, 's', 0, 0, 0 };

        uint8_t utf16_stream_long2[ 98 ] = {
		'B', 0, 'C', 0, 'D', 0, '{', 0, '3', 0, 'e', 0, '8', 0, '0', 0,
		'4', 0, '9', 0, '5', 0, '9', 0, '-', 0, 'e', 0, 'b', 0, '0', 0,
		'4', 0, '-', 0, '1', 0, '1', 0, 'e', 0, '4', 0, '-', 0, '8', 0,
		'0', 0, 'c', 0, '4', 0, '-', 0, '0', 0, '0', 0, '2', 0, '6', 0,
		'b', 0, '9', 0, '4', 0, 'a', 0, '1', 0, '7', 0, '7', 0, '3', 0,
		'}', 0, '.', 0, 'T', 0, 'M', 0, '.', 0, 'b', 0, 'l', 0, 'f', 0,
		0, 0 };

        uint8_t utf16_stream_long3[ 8 ] = {
		'B', 0, 'C', 0, 'D', 0, 0, 0 };

        uint8_t utf16_stream_long4[ 18 ] = {
		'B', 0, 'C', 0, 'D', 0, '.', 0, 'L', 0, 'O', 0, 'G', 0, '2', 0,
		 0, 0 };

        uint8_t utf16_stream_long5[ 14 ] = {
		'O', 0, 'l', 0, 'e', 0, ' ', 0, 'D', 0, 'B', 0, 0, 0 };

        uint8_t utf16_stream_long6[ 18 ] = {
		'f', 0, '[', 0, '1', 0, ']', 0, '.', 0, 't', 0, 'x', 0, 't', 0,
		0, 0 };

        uint8_t utf16_stream_long7[ 52 ] = {
		'6', 0, '.', 0, '1', 0, '.', 0, '0', 0, '.', 0, '0', 0, '_', 0,
		'_', 0, '3', 0, '1', 0, 'b', 0, 'f', 0, '3', 0, '8', 0, '5', 0,
		'6', 0, 'a', 0, 'd', 0, '3', 0, '6', 0, '4', 0, 'e', 0, '3', 0,
		'5', 0, 0, 0 };

        uint8_t utf16_stream_long8[ 14 ] = {
		'v', 0, '2', 0, '.', 0, '0', 0, '.', 0, '6', 0, 0, 0 };

        uint8_t utf16_stream_long9[ 28 ] = {
		'.', 0, 'N', 0, 'E', 0, 'T', 0, ' ', 0, 'C', 0, 'L', 0, 'R', 0,
		' ', 0, 'D', 0, 'a', 0, 't', 0, 'a', 0, 0, 0 };

        uint8_t utf16_stream_long10[ 36 ] = {
		'a', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0, 'e', 0, 'n', 0, 'd', 0,
		'p', 0, 'o', 0, 'i', 0, 'n', 0, 't', 0, '.', 0, 'P', 0, 'N', 0,
		'F', 0, 0, 0 };

        uint8_t utf16_stream_short1[ 18 ] = {
		'D', 0, 'O', 0, 'C', 0, 'U', 0, 'M', 0, 'E', 0, '~', 0, '1', 0,
		0, 0 };

        uint8_t utf16_stream_short2[ 26 ] = {
		'B', 0, 'C', 0, 'D', 0, '{', 0, '3', 0, 'E', 0, '~', 0, '1', 0,
		'.', 0, 'B', 0, 'L', 0, 'F', 0, 0, 0 };

        uint8_t utf16_stream_short4[ 20 ] = {
		'B', 0, 'C', 0, 'D', 0, '~', 0, '2', 0, '.', 0, 'L', 0, 'O', 0,
		'G', 0, 0, 0 };

        uint8_t utf16_stream_short5[ 16 ] = {
		'O', 0, 'L', 0, 'E', 0, 'D', 0, 'B', 0, '~', 0, '1', 0, 0, 0 };

        uint8_t utf16_stream_short6[ 22 ] = {
		'F', 0, '_', 0, '1', 0, '_', 0, '~', 0, '1', 0, '.', 0, 'T', 0,
		'X', 0, 'T', 0, 0, 0 };

        uint8_t utf16_stream_short7[ 20 ] = {
		'6', 0, '1', 0, '0', 0, '~', 0, '1', 0, '.', 0, '0', 0, '_', 0,
		'_', 0, 0, 0 };

        uint8_t utf16_stream_short8[ 16 ] = {
		'V', 0, '2', 0, '0', 0, '~', 0, '1', 0, '.', 0, '6', 0, 0, 0 };

        uint8_t utf16_stream_short9[ 18 ] = {
		'N', 0, 'E', 0, 'T', 0, 'C', 0, 'L', 0, 'R', 0, '~', 0, '1', 0,
		0, 0 };

        uint8_t utf16_stream_short10[ 26 ] = {
		'A', 0, 'U', 0, 'D', 0, 'I', 0, 'O', 0, 'E', 0, '~', 0, '1', 0,
		'.', 0, 'P', 0, 'N', 0, 'F', 0, 0, 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_name_compare_short(
	          utf16_stream_long1,
	          26,
	          utf16_stream_short1,
	          18,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long2,
	          96,
	          utf16_stream_short2,
	          26,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long3,
	          8,
	          utf16_stream_short2,
	          26,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long4,
	          18,
	          utf16_stream_short2,
	          26,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long4,
	          18,
	          utf16_stream_short4,
	          20,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long5,
	          14,
	          utf16_stream_short5,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long6,
	          18,
	          utf16_stream_short6,
	          22,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long7,
	          52,
	          utf16_stream_short7,
	          20,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long8,
	          14,
	          utf16_stream_short8,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long9,
	          28,
	          utf16_stream_short9,
	          18,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_short(
	          utf16_stream_long10,
	          36,
	          utf16_stream_short10,
	          26,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_name_compare_short(
	          NULL,
	          26,
	          utf16_stream_short1,
	          18,
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

	result = libfsntfs_name_compare_short(
	          utf16_stream_long1,
	          (size_t) SSIZE_MAX + 1,
	          utf16_stream_short1,
	          18,
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

	result = libfsntfs_name_compare_short(
	          utf16_stream_long1,
	          26,
	          NULL,
	          18,
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

	result = libfsntfs_name_compare_short(
	          utf16_stream_long1,
	          26,
	          utf16_stream_short1,
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

/* Tests the libfsntfs_name_compare_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_name_compare_with_utf8_string(
     void )
{
        uint8_t utf16_stream_equal[ 12 ]     = { 'e', 0, 'q', 0, 'u', 0, 'a', 0, 'l', 0, 0 };
        uint8_t utf16_stream_great[ 12 ]     = { 'g', 0, 'r', 0, 'e', 0, 'a', 0, 't', 0, 0, 0 };
        uint8_t utf16_stream_less[ 10 ]      = { 'l', 0, 'e', 0, 's', 0, 's', 0, 0, 0 };
        uint8_t utf16_stream_lesser[ 14 ]    = { 'l', 0, 'e', 0, 's', 0, 's', 0, 'e', 0, 'r', 0, 0, 0 };
        uint8_t utf16_stream_more[ 10 ]      = { 'm', 0, 'o', 0, 'r', 0, 'e', 0, 0, 0 };
        uint8_t utf8_string_equal[ 6 ]       = { 'e', 'q', 'u', 'a', 'l', 0 };
        uint8_t utf8_string_equal_upper[ 6 ] = { 'E', 'Q', 'U', 'A', 'L', 0 };
        uint8_t utf8_string_greater[ 7 ]     = { 'g', 'r', 'e', 'a', 't', 'e', 'r' };
        uint8_t utf8_string_less[ 4 ]        = { 'l', 'e', 's', 's' };
        uint8_t utf8_string_more[ 4 ]        = { 'm', 'o', 'r', 'e' };
	libcerror_error_t *error             = NULL;
	int result                           = 0;

	/* Test regular cases
	 */
	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          utf8_string_equal,
	          5,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          utf8_string_equal,
	          6,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          utf8_string_equal_upper,
	          5,
	          1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_great,
	          12,
	          utf8_string_greater,
	          7,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_less,
	          10,
	          utf8_string_more,
	          4,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_more,
	          10,
	          utf8_string_less,
	          4,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_lesser,
	          14,
	          utf8_string_less,
	          4,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_name_compare_with_utf8_string(
	          NULL,
	          12,
	          utf8_string_equal,
	          5,
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

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          (size_t) SSIZE_MAX + 1,
	          utf8_string_equal,
	          5,
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

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          NULL,
	          5,
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

	result = libfsntfs_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          utf8_string_equal,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_name_compare_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_name_compare_with_utf16_string(
     void )
{
        uint8_t utf16_stream_equal[ 12 ]       = { 'e', 0, 'q', 0, 'u', 0, 'a', 0, 'l', 0, 0 };
        uint8_t utf16_stream_great[ 12 ]       = { 'g', 0, 'r', 0, 'e', 0, 'a', 0, 't', 0, 0, 0 };
        uint8_t utf16_stream_less[ 10 ]        = { 'l', 0, 'e', 0, 's', 0, 's', 0, 0, 0 };
        uint8_t utf16_stream_lesser[ 14 ]      = { 'l', 0, 'e', 0, 's', 0, 's', 0, 'e', 0, 'r', 0, 0, 0 };
        uint8_t utf16_stream_more[ 10 ]        = { 'm', 0, 'o', 0, 'r', 0, 'e', 0, 0, 0 };
        uint16_t utf16_string_equal[ 6 ]       = { 'e', 'q', 'u', 'a', 'l', 0 };
        uint16_t utf16_string_equal_upper[ 6 ] = { 'E', 'Q', 'U', 'A', 'L', 0 };
        uint16_t utf16_string_greater[ 7 ]     = { 'g', 'r', 'e', 'a', 't', 'e', 'r' };
        uint16_t utf16_string_less[ 4 ]        = { 'l', 'e', 's', 's' };
        uint16_t utf16_string_more[ 4 ]        = { 'm', 'o', 'r', 'e' };
	libcerror_error_t *error               = NULL;
	int result                             = 0;

	/* Test regular cases
	 */
	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          utf16_string_equal,
	          5,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          utf16_string_equal,
	          6,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          utf16_string_equal_upper,
	          5,
	          1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_great,
	          12,
	          utf16_string_greater,
	          7,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_less,
	          10,
	          utf16_string_more,
	          4,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_more,
	          10,
	          utf16_string_less,
	          4,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_lesser,
	          14,
	          utf16_string_less,
	          4,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_name_compare_with_utf16_string(
	          NULL,
	          12,
	          utf16_string_equal,
	          5,
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

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          (size_t) SSIZE_MAX + 1,
	          utf16_string_equal,
	          5,
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

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          NULL,
	          5,
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

	result = libfsntfs_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          utf16_string_equal,
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
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_name_compare",
	 fsntfs_test_name_compare );

	FSNTFS_TEST_RUN(
	 "libfsntfs_name_compare_short",
	 fsntfs_test_name_compare_short );

	FSNTFS_TEST_RUN(
	 "libfsntfs_name_compare_with_utf8_string",
	 fsntfs_test_name_compare_with_utf8_string );

	FSNTFS_TEST_RUN(
	 "libfsntfs_name_compare_with_utf16_string",
	 fsntfs_test_name_compare_with_utf16_string );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */
}

