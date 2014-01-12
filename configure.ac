
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(btnx-config, 0.4.9, oasalonen@gmail.com)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/common.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_GNU_SOURCE

# Check command line arguments
AC_ARG_VAR(builddocs, Set builddocs=no if you do not want to generate or install help documentation)
AC_ARG_VAR(init_scripts_path, The init script directory for your system. Default is "/etc/init.d". Do not append a final '/'.)
AC_ARG_VAR(udev_rules_path, The udev rules directory for your system. Default is "/etc/udev/rules.d". Do not append a final '/'.)
test "$prefix" = NONE && prefix=/usr
test "$exec_prefix" = NONE && exec_prefix=$prefix
test "$datadir" = NONE && datadir=$prefix/share
test "$builddocs" != no && builddocs=yes
test -z "$init_scripts_path" && init_scripts_path=/etc/init.d
test -z "$udev_rules_path" && udev_rules_path=/etc/udev/rules.d

PREFIX=$prefix
AC_SUBST(PREFIX)
AC_DEFINE_UNQUOTED(PREFIX, ["$PREFIX"], [Installation path prefix])
AC_DEFINE_UNQUOTED(INIT_SCRIPTS_PATH, ["$init_scripts_path"], [Init scripts path])
AC_DEFINE_UNQUOTED(UDEV_RULES_PATH, ["$udev_rules_path"], [udev rules path])


# Documentation installation
#------------------------------------------------------------------
docrootdir=$prefix/share/btnx-config/doc

AH_TEMPLATE(HAVE_SCROLLKEEPER, [Check for scrollkeeper])
AH_TEMPLATE(HAVE_HELP, [Check for a help viewer])
AC_CHECK_PROG(SK_CONFIG, scrollkeeper-config, yes, no)
AC_CHECK_PROGS(rootexec, gksu kdesu, gksu, $PATH:/sbin:/usr/sbin:/usr/local/sbin)
#rootexec=gksu
#AC_MSG_WARN(rootexec=$rootexec)
if test x$SK_CONFIG != xno; then
	AC_DEFINE(HAVE_SCROLLKEEPER, 1)
	AC_CHECK_PROGS(TEST_HELP_VIEWER, [yelp khelpcenter], [no])
	if test x$TEST_HELP_VIEWER != xno; then
		AC_DEFINE_UNQUOTED(HELP_VIEWER, ["$TEST_HELP_VIEWER"], ["The program used to view help files."])
		SCROLLKEEPER_BUILD_REQUIRED=0.3.14
		AC_SUBST(SCROLLKEEPER_BUILD_REQUIRED)
		BUILD_HELP=1
	else
		BUILD_HELP=0
	fi
	#if test x$TEST_HELP_VIEWER = xkhelpcenter; then
	#	rootexec=kdesu
	#fi
else
	AC_DEFINE(HAVE_SCROLLKEEPER, 0)
	BUILD_HELP=0
fi

if test x$builddocs != xyes; then
	BUILD_HELP=0
fi
AC_SUBST(BUILD_HELP)
AM_CONDITIONAL(BUILD_HELP_TEST, test x$BUILD_HELP = x1)

BUILD_HELP_HTML=0
helpname=
if test x$BUILD_HELP = x1; then
	AC_DEFINE(HAVE_HELP, 1)
	AC_PATH_PROG(HELP_VIEWER_FULL_PATH, $TEST_HELP_VIEWER, "none")
	if test x$TEST_HELP_VIEWER = xkhelpcenter; then
		BUILD_HELP_HTML=1
		helpname=index.html
	elif test x$TEST_HELP_VIEWER = xyelp; then
		helpname=btnx-manual.xml
	fi
else
	HELP_VIEWER_FULL_PATH=
	AC_DEFINE(HAVE_HELP, 0)
fi

AC_DEFINE_UNQUOTED(HELP_VIEWER_FULL_PATH, ["$HELP_VIEWER_FULL_PATH"], [Full path to help viewer])
AC_SUBST(helpname)
AC_DEFINE_UNQUOTED(DOC_NAME, ["$helpname"], [Help filename])
AC_SUBST(BUILD_HELP_HTML)
AM_CONDITIONAL(BUILD_HELP_HTML_TEST, test x$BUILD_HELP_HTML = x1)

#echo "Documentation to $docrootdir"
AC_SUBST(docrootdir)
AC_DEFINE_UNQUOTED(DOC_PATH, ["$docrootdir"], [Help documentation location])

AC_DEFINE_UNQUOTED(DOC_FULL_PATH, ["$docrootdir/btnx-manual/C/$helpname"], [Help document path])

# Root exec prog for .desktop entry
AC_SUBST(rootexec)

# End documentation installation
#-------------------------------------------------------------------

# Find tar path
AC_PATH_PROG(TAR_PATH, tar, "/bin/tar")
AC_DEFINE_UNQUOTED(TAR_PATH, ["$TAR_PATH"], [Tar path])

# Checks for libraries.
PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.10.4)
PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.12.3)
PKG_CHECK_MODULES(GLADE, libglade-2.0 >= 2.6.0)

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h libintl.h locale.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([memset select setlocale strcasecmp strchr strdup strerror strncasecmp strtol])

# I18n
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION(0.16.1)

AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile doc/btnx-manual/Makefile doc/btnx-manual/C/Makefile po/Makefile.in intl/Makefile])

if test x$SK_CONFIG = xno; then
	AC_MSG_WARN(Scrollkeeper not found. No documentation will be installed.)
fi
if test x$TEST_HELP_VIEWER = xno; then
	AC_MSG_WARN(Checked for yelp and kdehelpcenter. No help viewer found. Documentation will not be installed.)
fi
if test x$builddocs != xyes; then
	AC_MSG_NOTICE(No help documentation has been generated or installed.)
fi

AC_OUTPUT

