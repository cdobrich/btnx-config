
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
/* Callbacks for the mouse detection window */

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "widgets.h"
#include "callbacks.h"
#include "timers.h"
#include "moused.h"
#include "btnd.h"
#include "threads.h"
#include "handlers.h"
#include "text.h"

gboolean win_moused_delete_event(	GtkWidget	*widget, 
									GdkEvent	*event, 
									gpointer	user_data)
{
	return FALSE;
}

void win_moused_destroy(GtkWidget	*widget, 
						gpointer	user_data)
{
	threads_sigterm(30);	
}

void btn_forward_moused_clicked(GtkButton *button, gpointer user_data)
{
	moused_stop();
	if (moused_set_mouse() < 0)
	{
		fprintf(stderr, "Show an error dialog here\n"); // Necessary?
	}
	widgets_center_on_parent(GET_WIDGET(WIN_BTND), GET_WIDGET(WIN_MOUSED));
	gtk_widget_hide(GET_WIDGET(WIN_MOUSED));
	gtk_widget_show(GET_WIDGET(WIN_BTND));
	init_btnd();
}

void btn_control_moused_clicked(GtkButton *button, gpointer user_data)
{	
	if (moused_detecting() == 0)
	{
		gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_HELP_MOUSED)),
							TEXT_HELP_MOUSED_WAIT);
		gtk_button_set_label(	GTK_BUTTON(GET_WIDGET(BTN_CONTROL_MOUSED)),
								TEXT_STOP_MOUSE_DETECTION);
		moused_start();
	}
	else
	{
		gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_HELP_MOUSED)),
							TEXT_HELP_MOUSED_INIT);
		gtk_button_set_label(	GTK_BUTTON(GET_WIDGET(BTN_CONTROL_MOUSED)),
								TEXT_START_MOUSE_DETECTION);
		moused_stop();
	}
}

void btn_cancel_moused_clicked(GtkButton *button, gpointer user_data)
{
	if (moused_detecting() == 1)
	{
		//g_print("sending stop");
		moused_stop();
	}
	handlers_free();
	widgets_center_on_parent(GET_WIDGET(WIN_CONFIG), GET_WIDGET(WIN_MOUSED));
	gtk_widget_show(GET_WIDGET(WIN_CONFIG));
	gtk_widget_hide(GET_WIDGET(WIN_MOUSED));
}

void btn_undo_moused_clicked(GtkButton *button, gpointer user_data)
{
	if (moused_detecting() == 1)
		moused_stop();
	handlers_free();
	init_moused();
}

