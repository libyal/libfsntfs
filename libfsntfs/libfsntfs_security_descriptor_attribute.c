/*
 * Security descriptor attribute ($SECURITY_DESCRIPTOR) functions
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_security_descriptor_attribute.h"
#include "libfsntfs_security_descriptor_values.h"
#include "libfsntfs_types.h"

/* Retrieves the security descriptor (data) size
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_security_descriptor_attribute_get_security_descriptor_size(
     libfsntfs_attribute_t *attribute,
     size_t *data_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_security_descriptor_attribute_get_security_descriptor_size";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	if( internal_attribute->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_security_descriptor_values_get_data_size(
	     (libfsntfs_security_descriptor_values_t *) internal_attribute->value,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size from security descriptor values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the security descriptor (data)
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_security_descriptor_attribute_get_security_descriptor(
     libfsntfs_attribute_t *attribute,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_security_descriptor_attribute_get_security_descriptor";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	if( internal_attribute->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_security_descriptor_values_get_data(
	     (libfsntfs_security_descriptor_values_t *) internal_attribute->value,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from security descriptor values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

