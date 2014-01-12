
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef COMMON_H_
#define COMMON_H_

#if HAVE_CONFIG_H
# include "../config.h"
# if HAVE_HELP
#   define HELP_PROG		HELP_VIEWER
#   define HELP_PATH		DOC_PATH
#	define HELP_NAME		DOC_NAME
#	define HELP_FULL_PATH 	DOC_FULL_PATH
//#	define HELP_FULL_PATH HELP_PATH "/" HELP_NAME "/C/" HELP_NAME ".xml"
# endif
#else
# define PACKAGE		"btnx-config"
# define VERSION		"Unknown version. Built without autotools. Contact the \
package maintainer."
# define TAR_PATH		"/bin/tar"
# define HAVE_HELP		0
#endif

#ifndef PREFIX
# define PREFIX			"/usr"
#endif

/* Init scripts directory path. Use ./configure init_scripts_path=/path/to to
 * control its value */
#ifndef INIT_SCRIPTS_PATH
# define INIT_SCRIPTS_PATH	"/etc/init.d"
#endif
#define INIT_SCRIPT_FILE	INIT_SCRIPTS_PATH "/btnx"

#ifndef UDEV_RULES_PATH
# define UDEV_RULES_PATH	"/etc/udev/rules.d"
#endif
#define UDEV_RULES_FILE		UDEV_RULES_PATH "/99-btnx.rules"

/* Not necessary to check for DEBUG anymore */
#ifdef DEBUG
#define DATA_PATH		"data/"
#else
#define DATA_PATH		PREFIX "/share/" PACKAGE
#endif

enum
{
	BTNX_EXIT_NORMAL=0,
	BTNX_ERROR_FATAL,
	BTNX_ERROR_NO_BIN_RESERVED,
	BTNX_ERROR_NO_CONFIG=150,
	BTNX_ERROR_BAD_CONFIG,
	BTNX_ERROR_OPEN_HANDLER,
	BTNX_ERROR_OPEN_UINPUT,
};

#endif /*COMMON_H_*/
