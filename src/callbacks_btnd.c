
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
 
/* Callback functions for the button detection window */

#include <gtk/gtk.h>

#include "callbacks.h"
#include "btnd.h"
#include "widgets.h"
#include "buttons.h"
#include "threads.h"
#include "config.h"
#include "handlers.h"
#include "dialog_btnd.h"
#include "text.h"

gboolean win_btnd_delete_event(	GtkWidget	*widget, 
								GdkEvent	*event, 
								gpointer	user_data)
{
	return FALSE;
}

void win_btnd_destroy(	GtkWidget	*widget, 
						gpointer	user_data)
{
	threads_sigterm(30);	
}

void btn_control_btnd_clicked(GtkButton *button, gpointer user_data)
{
	if (btnd_detecting() == 1)
	{
		btnd_stop();
		init_btnd();
	}
	else
	{
		btnd_start();
		gtk_button_set_label(GTK_BUTTON(GET_WIDGET(BTN_CONTROL_BTND)),
							TEXT_STOP_BUTTON_DETECTION);
	}
}

void btn_add_btnd_clicked(GtkButton *button, gpointer user_data)
{
	btnd_save_button();
}

void btn_ok_btnd_clicked(GtkButton *button, gpointer user_data)
{
	buttons_set_old_all();
	handlers_free();
	config_refresh_all_fields();
	widgets_center_on_parent(GET_WIDGET(WIN_CONFIG), GET_WIDGET(WIN_BTND));
	gtk_widget_hide(GET_WIDGET(WIN_BTND));
	gtk_widget_show(GET_WIDGET(WIN_CONFIG));
}

void btn_cancel_btnd_clicked(GtkButton *button, gpointer user_data)
{
	if (btnd_detecting() == 1)
	{
		btnd_stop();
	}
	handlers_free();
	
	buttons_remove_new();
	
	widgets_center_on_parent(GET_WIDGET(WIN_CONFIG), GET_WIDGET(WIN_BTND));
	gtk_widget_show(GET_WIDGET(WIN_CONFIG));
	gtk_widget_hide(GET_WIDGET(WIN_BTND));
}

void btn_ok_dialog_btnd_clicked(GtkButton *button, gpointer user_data)
{
	int rawcode;
	
	rawcode = dialog_btnd_get_rawcode();
	if (rawcode == 0)
	{
		fprintf(stderr, "Error: dialog btnd, no valid rawcode selected.\n");
		return;
	}
	dialog_btnd_stop();
	btnd_set_rawcode(rawcode);
	btnd_enable_save();
}

void btn_cancel_dialog_btnd_clicked(GtkButton *button, gpointer user_data)
{
	dialog_btnd_stop();
	init_btnd();
}

