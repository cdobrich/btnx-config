
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <stdlib.h>
#include <gtk/gtk.h>

#include "widgets.h"
#include "dialog_btnd.h"
#include "callbacks.h"
#include "buttons.h"
#include "rawcodes.h"
#include "text.h"

enum
{
	COL_RAWCODE,
	COL_CLICKS,
	COL_DETECTED_STR,
	COL_DETECTED,
	NUM_COL
};

static void dialog_btnd_make_store(void)
{	
	GtkTreeStore *store;
	store = gtk_tree_store_new(	NUM_COL,
								G_TYPE_STRING,
								G_TYPE_INT,
								G_TYPE_STRING,
								G_TYPE_BOOLEAN);
	gtk_tree_view_set_model(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_DIALOG_BTND)),
							GTK_TREE_MODEL(store));
	g_object_unref(G_OBJECT(store));
}

static void dialog_btnd_make_columns(void)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeView *view = GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_DIALOG_BTND));
	
	//	Rawcode column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(	TEXT_RAWCODE,
														renderer,
														"text", COL_RAWCODE,
														NULL);
	gtk_tree_view_append_column(view, column);
	
	// Clicks column
	renderer = gtk_cell_renderer_text_new();
	/* TRANSLATORS: "Clicks" refers to a number of presses that a user has
	 * performed on a mouse button. This should be short. Abbreviate if
	 * necessary. It is a table column title.*/
	column = gtk_tree_view_column_new_with_attributes(	_("Clicks"),
														renderer,
														"text", COL_CLICKS,
														NULL);
	gtk_tree_view_append_column(view, column);
	
	// Detected column
	renderer = gtk_cell_renderer_text_new();
	/* TRANSLATORS: "This should be short. Abbreviate if
	 * necessary. It is a table column title. A fuller title in English would
	 * be something along the lines of: "Has this button already been assigned
	 * (or associated) with a certain signal?" */
	column = gtk_tree_view_column_new_with_attributes(	_("Already assigned?"),
														renderer,
														"text", COL_DETECTED_STR,
														NULL);
	gtk_tree_view_append_column(view, column);
}

static GtkTreeStore *dialog_btnd_get_store(void)
{
	return GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_DIALOG_BTND))));
}

static void dialog_btnd_empty_store(void)
{
	GtkTreeIter iter;
	GtkTreeStore *store = dialog_btnd_get_store();
	
	if (!gtk_tree_model_get_iter_first(	GTK_TREE_MODEL(store), &iter))
	{
		fprintf(stderr, "Warning: dialog_btnd_empty_store could not get an iter.\n");
		return;
	}
	while (gtk_tree_store_remove(store, &iter));
}

static void dialog_btnd_add_rawcode(int rawcode)
{
	gchar *rawcode_str, *detected_str;
	gint clicks;
	gboolean detected;
	GtkTreeIter iter;
	GtkTreeStore *store = dialog_btnd_get_store();
	
	if (buttons_exists(rawcode) >= 0)
		detected=TRUE;
	else
		detected=FALSE;
	
	clicks = rawcodes_get_pressed(rawcode);
	
	rawcode_str = g_strdup_printf("0x%08x", rawcode);
	if (detected == TRUE)
		detected_str = g_strdup(TEXT_YES);
	else
		detected_str = g_strdup(TEXT_NO);
	
	//g_print("btnd dialog adding: rawcode_str=%s clicks=%d det=%d\n", rawcode_str, clicks, (gint)detected);
	
	gtk_tree_store_append(store, &iter, NULL);
	gtk_tree_store_set(	store, &iter,
						COL_RAWCODE,	rawcode_str,
						COL_CLICKS,		clicks,
						COL_DETECTED_STR,	detected_str,
						COL_DETECTED,	detected,
						-1);
	g_free(rawcode_str);
	g_free(detected_str);
	
	//g_print("btnd dialog done\n");
}

void dialog_btnd_populate(void)
{
	int rawcode=0;
	
	dialog_btnd_empty_store();
	
	while ((rawcode = rawcodes_get_most_pressed()) != 0)
	{
		dialog_btnd_add_rawcode(rawcode);
	}
	rawcodes_reset_highest();
}

void dialog_btnd_init(void)
{
	dialog_btnd_make_store();
	dialog_btnd_make_columns();
}

void dialog_btnd_run(void)
{
	gtk_window_set_modal(GTK_WINDOW(GET_WIDGET(WIN_DIALOG_BTND)), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW(GET_WIDGET(WIN_DIALOG_BTND)),
								GTK_WINDOW(GET_WIDGET(WIN_BTND)));
	dialog_btnd_populate();
	widgets_center_on_parent(GET_WIDGET(WIN_DIALOG_BTND), GET_WIDGET(WIN_BTND));
	gtk_widget_show(GET_WIDGET(WIN_DIALOG_BTND));
}

void dialog_btnd_stop(void)
{
	gtk_window_set_modal(GTK_WINDOW(GET_WIDGET(WIN_DIALOG_BTND)), FALSE);
	gtk_widget_hide(GET_WIDGET(WIN_DIALOG_BTND));
}

int dialog_btnd_get_rawcode(void)
{
	GtkTreeSelection *selection;
	GtkWidget *dialog;
	GtkTreeModel *model;
	GtkTreeIter iter;
	int detected, rawcode;
	gchar *rawcode_str, *msg;
	
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_DIALOG_BTND)));
	
	if (gtk_tree_selection_get_selected(selection, &model, &iter) == FALSE)
	{
		return 0;
	}
	gtk_tree_model_get(	model, &iter, 
						COL_RAWCODE, &rawcode_str,
						COL_DETECTED, &detected,
						-1);
	rawcode = strtol(rawcode_str, NULL, 16);
	if (detected == TRUE)
	{
		// show dialog
		msg = g_strdup_printf(
			_("That rawcode has already been assigned to the button \"%s\". Try another one."),
			buttons_get_name(buttons_exists(rawcode)));
		SHOW_WARNING_DIALOG(dialog, GET_WIDGET(WIN_BTND),
							_("Invalid choice"),
							msg);
		g_free(msg);
		g_free(rawcode_str);
		return 0;
	}
	
	
	g_free(rawcode_str);
	return rawcode;
}

