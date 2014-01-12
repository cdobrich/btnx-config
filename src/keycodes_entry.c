
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#include <gtk/gtk.h>
#include "widgets.h"
#include "callbacks.h"

GtkEntry *keycodes_entry=NULL;
GtkEntryCompletion *keycodes_completion=NULL;

GtkWidget *keycodes_entry_get_entry(void)
{
	return GTK_WIDGET(keycodes_entry);
}

gboolean keycodes_entry_selected(	GtkEntryCompletion *widget,
									GtkTreeModel *model,
									GtkTreeIter *iter,
									gpointer user_data)
{
	button_property_changed(GTK_WIDGET(keycodes_entry), NULL);
	return FALSE;
}

void keycodes_entry_activate(GtkEntry *entry, gpointer user_data)
{
	
}

gboolean keycodes_entry_focus_out(	GtkWidget *widget,
									GdkEventFocus *event,
									gpointer user_data)
{
	button_property_changed(widget, NULL);
	return FALSE;
}

void keycodes_entry_init(void)
{
	GtkTreeStore *store = gtk_tree_store_new(1, G_TYPE_STRING);
	gtk_combo_box_set_model(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG)), GTK_TREE_MODEL(store));
	gtk_container_remove(GTK_CONTAINER(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG)), GET_WIDGET(COMBOBOX_ENTRY_KEYCODE_CONFIG));
	
	keycodes_completion = gtk_entry_completion_new();
	gtk_entry_completion_set_model(keycodes_completion, GTK_TREE_MODEL(store));
	gtk_entry_completion_set_text_column(keycodes_completion, 0);
	gtk_entry_completion_set_inline_completion(keycodes_completion, TRUE);
	
	keycodes_entry = GTK_ENTRY(gtk_entry_new());
	gtk_entry_set_completion(keycodes_entry, keycodes_completion);
	gtk_container_add(GTK_CONTAINER(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG)), GTK_WIDGET(keycodes_entry));
	gtk_widget_show(GTK_WIDGET(keycodes_entry));
	
	g_signal_connect(G_OBJECT(keycodes_completion), "match-selected",
					G_CALLBACK(keycodes_entry_selected), NULL);
	g_signal_connect(G_OBJECT(keycodes_entry), "activate",
					G_CALLBACK(keycodes_entry_activate), NULL);
	g_signal_connect(G_OBJECT(keycodes_entry), "focus-out-event",
					G_CALLBACK(keycodes_entry_focus_out), NULL);
}

void keycodes_entry_add(const gchar *keycode)
{
	GtkTreeIter iter;
	GtkTreeStore *store;
	store = GTK_TREE_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG))));
	gtk_tree_store_append(store, &iter, NULL);
	gtk_tree_store_set(store, &iter, 0, keycode, -1);
}

