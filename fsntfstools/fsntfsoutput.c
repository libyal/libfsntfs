/*
 * Common output functions for the fsntfstools
 *
 * Copyright (C) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_LOCAL_LIBUNA )
#include <libuna_definitions.h>
#elif defined( HAVE_LIBUNA_H )
#include <libuna.h>
#endif

#if defined( HAVE_LOCAL_LIBBFIO )
#include <libbfio_definitions.h>
#elif defined( HAVE_LIBBFIO_H )
#include <libbfio.h>
#endif

#if defined( HAVE_LOCAL_LIBFDATA )
#include <libfdata_definitions.h>
#elif defined( HAVE_LIBFDATA )
#include <libfdata.h>
#endif

#if defined( HAVE_LOCAL_LIBFDATETIME )
#include <libfdatetime_definitions.h>
#elif defined( HAVE_LIBFDATETIME_H )
#include <libfdatetime.h>
#endif

#if defined( HAVE_LOCAL_LIBFGUID )
#include <libfguid_definitions.h>
#elif defined( HAVE_LIBFGUID )
#include <libfguid.h>
#endif

#if defined( HAVE_LOCAL_LIBFWNT )
#include <libfwnt_definitions.h>
#elif defined( HAVE_LIBFWNT )
#include <libfwnt.h>
#endif

#include "fsntfsoutput.h"
#include "fsntfstools_libcerror.h"
#include "fsntfstools_libclocale.h"
#include "fsntfstools_libcnotify.h"
#include "fsntfstools_libcstring.h"
#include "fsntfstools_libcsystem.h"
#include "fsntfstools_libfsntfs.h"

/* Prints the copyright information
 */
void fsntfsoutput_copyright_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf(
	 stream,
	 "Copyright (c) 2010-2014, Joachim Metz <%s>.\n"
	 "This is free software; see the source for copying conditions. There is NO\n"
	 "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n",
	 PACKAGE_BUGREPORT );
}

/* Prints the version information
 */
void fsntfsoutput_version_fprint(
      FILE *stream,
      const char *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
        fprintf(
	 stream,
	 "%s %s\n\n",
         program,
	 LIBFSNTFS_VERSION_STRING );
}

/* Prints the detailed version information
 */
void fsntfsoutput_version_detailed_fprint(
      FILE *stream,
      const char *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
        fprintf(
	 stream,
	 "%s %s (libfsntfs %s",
         program,
	 LIBFSNTFS_VERSION_STRING,
	 LIBFSNTFS_VERSION_STRING );

	fprintf(
	 stream,
	 ", libuna %s",
	 LIBUNA_VERSION_STRING );

	fprintf(
	 stream,
	 ", libbfio %s",
	 LIBBFIO_VERSION_STRING );

	fprintf(
	 stream,
	 ", libfdata %s",
	 LIBFDATA_VERSION_STRING );

	fprintf(
	 stream,
	 ", libfdatetime %s",
	 LIBFDATETIME_VERSION_STRING );

	fprintf(
	 stream,
	 ", libfguid %s",
	 LIBFGUID_VERSION_STRING );

	fprintf(
	 stream,
	 ", libfwnt %s",
	 LIBFWNT_VERSION_STRING );

        fprintf(
	 stream,
	 ")\n\n" );
}

