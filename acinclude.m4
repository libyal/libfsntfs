dnl Checks for required headers and functions
dnl
dnl Version: 20230206

dnl Function to detect if libfsntfs dependencies are available
AC_DEFUN([AX_LIBFSNTFS_CHECK_LOCAL],
  [dnl Headers included in libfsntfs/libfsntfs_name.c
  AC_CHECK_HEADERS([wctype.h])

  dnl Functions used in libfsntfs/libfsntfs_name.c
  AC_CHECK_FUNCS([towupper])

  AS_IF(
    [test "x$ac_cv_func_towupper" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: towupper],
      [1])
  ])

  dnl Check for internationalization functions in libfsntfs/libfsntfs_i18n.c
  AC_CHECK_FUNCS([bindtextdomain])
])

dnl Function to detect if fsntfstools dependencies are available
AC_DEFUN([AX_FSNTFSTOOLS_CHECK_LOCAL],
  [AC_CHECK_HEADERS([signal.h sys/signal.h unistd.h])

  AC_CHECK_FUNCS([close getopt setvbuf])

  AS_IF(
   [test "x$ac_cv_func_close" != xyes],
   [AC_MSG_FAILURE(
     [Missing function: close],
     [1])
  ])

  dnl Headers included in fsntfstools/fsntfsmount.c
  AC_CHECK_HEADERS([errno.h sys/time.h])

  dnl Functions included in fsntfstools/mount_file_system.c and fsntfstools/mount_file_entry.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_FUNCS([clock_gettime getegid geteuid time])
  ])
])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBFSNTFS_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBFSNTFS_DLL_EXPORT],
        ["-DLIBFSNTFS_DLL_EXPORT"])

      AC_SUBST(
        [LIBFSNTFS_DLL_IMPORT],
        ["-DLIBFSNTFS_DLL_IMPORT"])
      ])
    ])
  ])

