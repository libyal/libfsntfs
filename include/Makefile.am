include_HEADERS = \
	libfsntfs.h

pkginclude_HEADERS = \
	libfsntfs/codepage.h \
	libfsntfs/definitions.h \
	libfsntfs/error.h \
	libfsntfs/extern.h \
	libfsntfs/features.h \
	libfsntfs/types.h

EXTRA_DIST = \
	libfsntfs.h.in \
	libfsntfs/definitions.h.in \
	libfsntfs/features.h.in \
	libfsntfs/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfsntfs.h
	/bin/rm -f libfsntfs/definitions.h
	/bin/rm -f libfsntfs/features.h
	/bin/rm -f libfsntfs/types.h
	/bin/rm -f Makefile

