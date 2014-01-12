
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */


#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

#include "widgets.h"
#include "timers.h"
#include "moused.h"
#include "btnd.h"
#include "ip_pipe.h"
#include "text.h"


guint cleanup_unhandled_pids(gpointer data)
{
	ip_pipe_handle_old_pids();
	return TRUE;
}

guint moused_check_handler_report_timer(gpointer data)
{
	if (moused_detecting() == 0)
		return FALSE;
	moused_check_handler_report();
	return TRUE;
}

guint moused_check_start_timer(gpointer data)
{
	if (moused_detecting() == 0)
		return FALSE;
	if (moused_check_start() > 0)
	{
		//g_print("moused check started\n");
		gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_HELP_MOUSED)),
							TEXT_HELP_MOUSED_START);
		g_timeout_add_full(	G_PRIORITY_HIGH_IDLE,
					MOUSED_CHECK_REPORT_INTERVAL,
					(GSourceFunc)moused_check_handler_report_timer,
					NULL, NULL);
		return FALSE;
	}
	return TRUE;
}


guint btnd_check_button_report_timer(gpointer data)
{
	if (btnd_detecting() == 0)
		return FALSE;
	btnd_check_button_report();
	return TRUE;
}

guint btnd_check_start_timer(gpointer data)
{
	if (btnd_detecting() == 0)
	{
		fprintf(stderr, "Warning: btnd_check_start_timer stopped. Not detecting.\n");
		init_btnd();
		return FALSE;
	}
	if (btnd_check_start() > 0)
	{
		//g_print("btnd check started\n");
		g_timeout_add_full(	G_PRIORITY_HIGH_IDLE,
					BTND_CHECK_REPORT_INTERVAL,
					(GSourceFunc)btnd_check_button_report_timer,
					NULL, NULL);
		return FALSE;
	}
	return TRUE;
}

