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
	-rm -f libfsntfs.h
	-rm -f libfsntfs/definitions.h
	-rm -f libfsntfs/features.h
	-rm -f libfsntfs/types.h
	-rm -f Makefile

