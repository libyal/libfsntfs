dnl Function to detect if libfsntfs dependencies are available
AC_DEFUN([AX_LIBFSNTFS_CHECK_LOCAL],
 [dnl Check for internationalization functions in libfsntfs/libfsntfs_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

