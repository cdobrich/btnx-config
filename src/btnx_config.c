
/*------------------------------------------------------------------------*
 * btnx-config: An app for configuring btnx                               *
 * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>                 *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 2         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,                     *
 * Boston, MA  02110-1301, USA.                                           *
 *------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include <linux/input.h>
#include <errno.h>
#include <signal.h>
#include <glade/glade.h>
#include <gtk/gtk.h>

#include "widgets.h"
#include "timers.h"
#include "ip_pipe.h"
#include "threads.h"
#include "keycodes.h"
#include "config_file.h"
#include "tree_buttons.h"
#include "mouse.h"
#include "config.h"
#include "text.h"
#include "common.h"
#include "revoco.h"
#include "config_manager.h"

int main(int argc, char *argv[])
{
	GladeXML *xml;
	
	/* Init i18n */
	if (setlocale(LC_ALL, "") == NULL)
		fprintf(stderr, "Warning: setlocale error\n");
	if (bindtextdomain(PACKAGE, LOCALEDIR) == NULL)
		fprintf(stderr, "Warning: bindtext error\n");
	if (textdomain(PACKAGE) == NULL)
		fprintf(stderr, "Warning: textdomain error\n");
	
	gtk_init(&argc, &argv);
	
	/* Init glade XML file */
	if ((xml = init_glade_xml()) == NULL)
	{
		GtkWidget *dialog;
		SHOW_ERROR_DIALOG(	dialog, NULL,
							TEXT_TITLE_GLADE_FILE_ERROR,
							TEXT_BODY_GLADE_FILE_ERROR);
		fprintf(stderr, "Error: cannot continue without glade file. Quitting.\n");
		exit(1);
	}
	
	/* Init widgets */
	init_main();
	
	/* Connect signals */
	signal(SIGCHLD, threads_sigchld);
	signal(SIGTERM, threads_sigterm);
	signal(SIGQUIT, threads_sigterm);
	signal(SIGINT, threads_sigterm);
	signal(SIGSEGV, threads_sigsegv);
	
	/* Parse keycodes from event file */
	if (keycodes_parse() < 0)
	{
		GtkWidget *dialog;
		SHOW_ERROR_DIALOG(	dialog, GET_WIDGET(WIN_CONFIG),
							TEXT_TITLE_EVENT_FILE_ERROR,
							TEXT_BODY_EVENT_FILE_ERROR);
		fprintf(stderr, "Error: cannot continue without keycodes. Quitting.\n");
		exit(1);
	}
	
	/* Init rest of data structures and widgets */
	mouse_init();
	config_manager_init();
	settings_file_parse();
	config_refresh_advanced_settings();
	tree_buttons_replace();
	config_refresh_mouse_properties();
	init_evtype();
	revoco_refresh_properties();
	
	g_timeout_add(	PIDS_CLEANUP_INTERVAL,
					(GSourceFunc)cleanup_unhandled_pids,
					NULL);
	
	gtk_widget_show(GET_WIDGET(WIN_CONFIG));
	
#ifdef DEBUG
	GtkWidget *dialog;
	SHOW_WARNING_DIALOG(dialog, GET_WIDGET(WIN_CONFIG),
"Debugging version",
"This is a reminder that this is a debugging version of btnx-config. \
Not all functionality is present. \n\nNote, that saving a configuration \
file places it in /etc/btnx/btnx_config_test to avoid conflicts with \
the release version of btnx. \n\nThe configuration file outputted by this \
version will not work with the current version of btnx." );
#endif

	gtk_main();
	
	/* Program never gets here. Quitting is handled through threads_sigterm() */
	return 0;
}

