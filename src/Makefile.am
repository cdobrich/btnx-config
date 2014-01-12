## Makefile.am

sbin_PROGRAMS = btnx-config

btnx_configdir = $(pkgdatadir)
localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
LIBS = @LIBINTL@ @LIBS@

AM_CFLAGS = -Wall -O2 `pkg-config --cflags gtk+-2.0 libglade-2.0` -I../intl -I$(top_srcdir)/intl
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I../intl -I$(top_srcdir)/intl
btnx_config_LDFLAGS = `pkg-config --libs gtk+-2.0 libglade-2.0` -Wl -export-dynamic -DPREFIX=$(pkgdatadir)
btnx_config_LDADD = @LIBINTL@

##btnx_config_HEADERS = \
	

btnx_config_SOURCES = \
	btnx_config.c \
	widgets.c \
	callbacks_config.c \
	callbacks_moused.c \
	timers.c \
	moused.c \
	ip_pipe.c \
	handlers.c \
	btnd.c \
	rawcodes.c \
	callbacks_btnd.c \
	buttons.c \
	threads.c \
	config.c \
	tree_buttons.c \
	keycodes.c \
	config_file.c \
	mouse.c \
	dialog_btnd.c \
	keycodes_entry.c \
	udev.c \
	help.c \
	revoco.c \
	config_manager.c \
	tree_config.c \
	backup.c \
	status_bar.c \
## START HEADERS
	widgets.h \
	callbacks.h \
	timers.h \
	moused.h \
	ip_pipe.h \
	handlers.h \
	btnd.h \
	rawcodes.h \
	buttons.h \
	threads.h \
	config.h \
	tree_buttons.h \
	keycodes.h \
	config_file.h \
	mouse.h \
	dialog_btnd.h \
	common.h \
	text.h \
	keycodes_entry.h \
	udev.h \
	help.h \
	revoco.h \
	config_manager.h \
	tree_config.h \
	backup.h \
	status_bar.h

