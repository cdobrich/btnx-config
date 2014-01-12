
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include "common.h"
#include "help.h"
#include "callbacks.h"
#include "widgets.h"
#include "text.h"

#if HAVE_HELP
static void help_launch(GtkWidget *parent)
{
	GError *error=NULL;
	gchar *arg[3] = {HELP_VIEWER_FULL_PATH, HELP_FULL_PATH, NULL};
	
	GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(parent));
	gdk_spawn_on_screen(screen, NULL, arg, NULL, G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, NULL, &error);
	
	if (error != NULL)
	{
		printf(error->message);
		printf("\n" HELP_VIEWER_FULL_PATH);
	}
}
#else
static void help_launch(GtkWidget *parent)
{
	GtkWidget *dialog;
	
	SHOW_WARNING_DIALOG(dialog, parent,
_("Help support not compiled."),
_("Local help is unavailable. This program was not compiled with help support. \
Online help documentation is available at \
<http://www.ollisalonen.com/btnx/man>. Either compile this \
program with help support or contact the maintaner of this package."));
}
#endif

void btn_help_config_clicked(GtkButton *button, gpointer user_data)
{
	help_launch(GET_WIDGET(WIN_CONFIG));
}

void btn_help_moused_clicked(GtkButton *button, gpointer user_data)
{
	help_launch(GET_WIDGET(WIN_MOUSED));
}

void btn_help_btnd_clicked(GtkButton *button, gpointer user_data)
{
	help_launch(GET_WIDGET(WIN_BTND));
}
