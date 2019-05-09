dnl Checks for libfusn required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libfusn is available
dnl ac_libfusn_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFUSN_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfusn" = xno],
    [ac_cv_libfusn=no],
    [ac_cv_libfusn=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libfusn" != x && test "x$ac_cv_with_libfusn" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libfusn"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfusn}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfusn}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfusn],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfusn],
          [libfusn >= 20180611],
          [ac_cv_libfusn=yes],
          [ac_cv_libfusn=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfusn" = xyes],
        [ac_cv_libfusn_CPPFLAGS="$pkg_cv_libfusn_CFLAGS"
        ac_cv_libfusn_LIBADD="$pkg_cv_libfusn_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfusn" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfusn.h])

      AS_IF(
        [test "x$ac_cv_header_libfusn_h" = xno],
        [ac_cv_libfusn=no],
        [dnl Check for the individual functions
        ac_cv_libfusn=yes

        AC_CHECK_LIB(
          fusn,
          libfusn_get_version,
          [ac_cv_libfusn_dummy=yes],
          [ac_cv_libfusn=no])

        dnl Record functions
        AC_CHECK_LIB(
          fusn,
          libfusn_record_initialize,
          [ac_cv_libfusn_dummy=yes],
          [ac_cv_libfusn=no])
        AC_CHECK_LIB(
          fusn,
          libfusn_record_free,
          [ac_cv_libfusn_dummy=yes],
          [ac_cv_libfusn=no])

        AC_CHECK_LIB(
          fusn,
          libfusn_record_copy_from_byte_stream,
          [ac_cv_libfusn_dummy=yes],
          [ac_cv_libfusn=no])

        AC_CHECK_LIB(
          fusn,
          libfusn_record_get_size,
          [ac_cv_libfusn_dummy=yes],
          [ac_cv_libfusn=no])

        ac_cv_libfusn_LIBADD="-lfusn"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libfusn" != x && test "x$ac_cv_with_libfusn" != xauto-detect && test "x$ac_cv_libfusn" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libfusn in directory: $ac_cv_with_libfusn],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfusn" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFUSN],
      [1],
      [Define to 1 if you have the `fusn' library (-lfusn).])
    ])

  AS_IF(
    [test "x$ac_cv_libfusn" = xyes],
    [AC_SUBST(
      [HAVE_LIBFUSN],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFUSN],
      [0])
    ])
  ])

dnl Function to detect if libfusn dependencies are available
AC_DEFUN([AX_LIBFUSN_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfusn_CPPFLAGS="-I../libfusn";
  ac_cv_libfusn_LIBADD="../libfusn/libfusn.la";

  ac_cv_libfusn=local
  ])

dnl Function to detect how to enable libfusn
AC_DEFUN([AX_LIBFUSN_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfusn],
    [libfusn],
    [search for libfusn in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFUSN_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfusn" != xyes],
    [AX_LIBFUSN_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFUSN],
      [1],
      [Define to 1 if the local version of libfusn is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFUSN],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFUSN],
    [test "x$ac_cv_libfusn" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfusn_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFUSN_CPPFLAGS],
      [$ac_cv_libfusn_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfusn_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFUSN_LIBADD],
      [$ac_cv_libfusn_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfusn" = xyes],
    [AC_SUBST(
      [ax_libfusn_pc_libs_private],
      [-lfusn])
    ])

  AS_IF(
    [test "x$ac_cv_libfusn" = xyes],
    [AC_SUBST(
      [ax_libfusn_spec_requires],
      [libfusn])
    AC_SUBST(
      [ax_libfusn_spec_build_requires],
      [libfusn-devel])
    ])
  ])

