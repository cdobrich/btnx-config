
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "tree_buttons.h"
#include "widgets.h"
#include "buttons.h"
#include "config.h"
#include "text.h"

enum
{
	COL_NAME,
	COL_PROPERTY,
	NUM_COL
};

static int empty=1;

static void tree_buttons_make_store(void)
{	
	GtkTreeStore *store;
	store = gtk_tree_store_new(	NUM_COL,
								G_TYPE_STRING,
								G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)),
							GTK_TREE_MODEL(store));
	g_object_unref(G_OBJECT(store));
}


static void tree_buttons_make_columns(void)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeView *view = GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG));
	
	//	Property column
	renderer = gtk_cell_renderer_text_new();
	/* TRANSLATORS: Property is the title of a table column. Make it as short
	 * as possible. Abbreviate if necessary. It refers to a property that
	 * a certain group can have. */
	column = gtk_tree_view_column_new_with_attributes(	_("Property"),
														renderer,
														"text", COL_NAME,
														NULL);
	gtk_tree_view_column_set_min_width (column, 80);
	gtk_tree_view_append_column(view, column);
	
	// Value column
	renderer = gtk_cell_renderer_text_new();
	/* TRANSLATORS: Value is the title of a table column. Make it as short
	 * as possible. Abbreviate if necessary. It refers to the value that a 
	 * certain property can have. For example, color can be a property and
	 * blue is one possible value. */
	column = gtk_tree_view_column_new_with_attributes(	_("Value"),
														renderer,
														"text", COL_PROPERTY,
														NULL);
	gtk_tree_view_column_set_min_width (column, 80);
	gtk_tree_view_append_column(view, column);
}


static GtkTreeStore *tree_buttons_get_store(void)
{
	return GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG))));
}

static void tree_buttons_empty_store(void)
{
	GtkTreeIter iter;
	GtkTreeStore *store = tree_buttons_get_store();
	
	if (!gtk_tree_model_get_iter_first(	GTK_TREE_MODEL(store), &iter))
	{
		//fprintf(stderr, "Warning: tree_buttons_empty_store could not get an iter.\n");
		return;
	}
	while (gtk_tree_store_remove(store, &iter));
}

static void tree_buttons_add_button(int button, GtkTreeIter *iter_after, GtkTreeStore *store)
{
	GtkTreeIter iter, iter_parent;
	int evtype_val, switch_type_val;
	gchar 	*rawcode, 
			*delay, 
			*enabled,
			*force_release,
			*evtype,
			*switch_type,
			*switch_name,
			*keycode, 
			*mod1,
			*mod2,
			*mod3,
			*uid,
			*name,
			*command;
	
	rawcode = g_strdup_printf("0x%08x", buttons_get_rawcode(button));
	delay = g_strdup_printf("%d", buttons_get_delay(button));
	enabled = g_strdup_printf("%d", buttons_get_enabled(button));
	force_release = g_strdup_printf("%d", buttons_get_force_release(button));
	evtype_val = buttons_get_evtype(button);
	switch_type_val = buttons_get_switch_type(button);
	//evtype = g_strdup_printf("%d", buttons_get_evtype(button));
	uid = g_strdup_printf("%d", buttons_get_uid(button));
	
	if (buttons_get_keycode(button) != NULL)
		keycode = g_strndup(buttons_get_keycode(button), 16);
	else
		keycode = g_strdup("NONE");
	if (buttons_get_mod1(button) != NULL)
		mod1 = g_strndup(buttons_get_mod1(button), 16);
	else
		mod1 = g_strdup("NONE");
	if (buttons_get_mod2(button) != NULL)
		mod2 = g_strndup(buttons_get_mod2(button), 16);
	else
		mod2 = g_strdup("NONE");
	if (buttons_get_mod3(button) != NULL)
		mod3 = g_strndup(buttons_get_mod3(button), 16);
	else
		mod3 = g_strdup("NONE");
	if (buttons_get_switch_name(button) != NULL)
		switch_name = g_strndup(buttons_get_switch_name(button), 16);
	else
		switch_name = g_strdup("NONE");
	if (buttons_get_name(button) != NULL)
		name = g_strndup(buttons_get_name(button), 16);
	else
		name = g_strdup(" ");
	if (buttons_get_command(button) != NULL)
		command = g_strndup(buttons_get_command(button), 16);
	else
		command = g_strdup(" ");
	if (evtype_val == EVTYPE_KEY)
		evtype = strdup(EVTYPE_KEY_STR);
	else if (evtype_val == EVTYPE_CONFIG)
		evtype = strdup(EVTYPE_CONFIG_STR);
	else if (evtype_val == EVTYPE_COMMAND)
		evtype = strdup(EVTYPE_COMMAND_STR);
	else
		evtype = strdup(TEXT_ERROR);
	
	if (switch_type_val == CONFIG_SWITCH_NONE)
		switch_type = strdup("NONE");
	else if (switch_type_val == CONFIG_SWITCH_NEXT)
		switch_type = strdup(CONFIG_SWITCH_NEXT_STR);
	else if (switch_type_val == CONFIG_SWITCH_PREV)
		switch_type = strdup(CONFIG_SWITCH_PREV_STR);
	else if (switch_type_val == CONFIG_SWITCH_TO)
		switch_type = strdup(CONFIG_SWITCH_TO_STR);
	else
		switch_type = strdup(TEXT_ERROR);
			
	//gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter_parent);
	if (iter_after == NULL)
	{
		gtk_tree_store_append(store, &iter_parent, NULL);	
	}
	else
	{
		gtk_tree_store_insert_after(store, &iter_parent, NULL, iter_after);
	}
	
	gtk_tree_store_set(	store, &iter_parent,
						COL_NAME, 		name,
						COL_PROPERTY,	" ",
						-1);
	
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
						COL_NAME,		"RC",
						COL_PROPERTY,	rawcode);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. Enabled can be
	 * either true or false. In this case, it means whether the program should
	 * ignore a certain mouse button or do something when it is being pressed.*/
						COL_NAME,		_("Enabled"),
						COL_PROPERTY,	enabled);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. Evtype is already
	 * abbreviated from the words "Event type". This refers to the type of
	 * event that should be sent by the program when it recognizes it must do
	 * something. */
						COL_NAME,		_("Evtype"),
						COL_PROPERTY,	evtype);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. Delay refers to a
	 * certain time interval during which the program should wait. */
						COL_NAME,		_("Delay"),
						COL_PROPERTY,	delay);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
						COL_NAME,		_("Release"),
						COL_PROPERTY,	force_release);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. This word is used
	 * elsewhere in the program. The keycode is a keyboard or button
	 * name. Example: KEY_PAGEUP for a "Page Up" key. */
						COL_NAME,		_("Keycode"),
						COL_PROPERTY,	keycode);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. Modifier refers
	 * to a keyboard modifier key. Examples: Shift, Ctrl, Alt. Make sure to
	 * include the number at the end.*/
						COL_NAME,		_("Modifier1"),
						COL_PROPERTY,	mod1);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. Modifier refers
	 * to a keyboard modifier key. Examples: Shift, Ctrl, Alt. Make sure to
	 * include the number at the end.*/
						COL_NAME,		_("Modifier2"),
						COL_PROPERTY,	mod2);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. Modifier refers
	 * to a keyboard modifier key. Examples: Shift, Ctrl, Alt. Make sure to
	 * include the number at the end.*/
						COL_NAME,		_("Modifier3"),
						COL_PROPERTY,	mod3);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
						COL_NAME,		_("Switch type"),
						COL_PROPERTY,	switch_type);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
						COL_NAME,		CONFIG_SWITCH_TO_STR,
						COL_PROPERTY,	switch_name);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. The UID refers
	 * to User Identification, a number referring to a user in a 
	 * Linux system. Do not try to translate UID unless it is explicitly
	 * used differently by Linux in your language. */
						COL_NAME,		_("UID"),
						COL_PROPERTY,	uid);
	gtk_tree_store_append(store, &iter, &iter_parent);
	gtk_tree_store_set(	store, &iter,
	/* TRANSLATORS: Make this short. Abbreviate if necessary. Command refers
	 * to a shell command that can be executed. */
						COL_NAME,		_("Command"),
						COL_PROPERTY,	command);
	
	
	g_free(rawcode);
	g_free(delay);
	g_free(enabled);
	g_free(force_release);
	g_free(evtype);
	g_free(keycode);
	g_free(mod1);
	g_free(mod2);
	g_free(mod3);
	g_free(uid);
	g_free(name);
	g_free(command);
	g_free(switch_type);
	g_free(switch_name);
}

static void tree_buttons_replace_button(int button, GtkTreeIter *iter, GtkTreeStore *store)
{
	GtkTreePath *path_old, *path_new;
	GtkTreeIter tmp;
	GtkTreeSelection *sel;
	gboolean expanded;
	
	tmp = *iter;
	tree_buttons_add_button(button, iter, store);
	path_old = gtk_tree_model_get_path(GTK_TREE_MODEL(store), &tmp);
	expanded = gtk_tree_view_row_expanded(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)), path_old);
	
	*iter = tmp;
	gtk_tree_model_iter_next(GTK_TREE_MODEL(store), iter);
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)));
	gtk_tree_selection_select_iter(sel, iter);
	
	if (expanded)
	{
		path_new = gtk_tree_model_get_path(GTK_TREE_MODEL(store), iter);
		if (gtk_tree_view_expand_row(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)),
			path_new, FALSE) == FALSE)
		{
			fprintf(stderr, "Warning: tree_buttons_replace_button could not find newly added button.\n");
		}
		gtk_tree_path_free(path_new);
	}
	gtk_tree_path_free(path_old);
	
	gtk_tree_store_remove(store, &tmp);
}

void tree_buttons_remove_button(void)
{
	GtkTreeModel *model;
	GtkTreeSelection *sel;
	GtkTreeIter iter, iter_parent, iter_next;
	gint sel_new=1;
	
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)));
	if (gtk_tree_selection_get_selected(sel, &model, &iter) == FALSE)
	{
		fprintf(stderr,
		"Warning: tree_buttons_remove_button error, no selected button.\n");
		return;
	}
	if (gtk_tree_model_iter_parent(model, &iter_parent, &iter) == FALSE)
	{
		iter_parent = iter;
	}
	iter_next = iter_parent;
	if (gtk_tree_model_iter_next(model, &iter_next) == FALSE)
	{
		if (buttons_count() <= 1)
			sel_new = 0;
		else
			gtk_tree_model_get_iter_first(model, &iter_next);
	}
	gtk_tree_store_remove(GTK_TREE_STORE(model), &iter_parent);
	if (sel_new == 1)
		gtk_tree_selection_select_iter(sel, &iter_next);
}

void tree_buttons_replace_and_populate(int replace)
{
	int i;
	GtkTreeStore *store = NULL;
	int count = buttons_count();
	if (replace)
		tree_buttons_empty_store();
	
	store = tree_buttons_get_store();
	
	if (store == NULL)
	{
		fprintf(stderr, "Error: tree_buttons_populate could not get store.\n");
		return;
	}
	for (i=0; i<count; i++)
	{
		if (replace || empty)
			tree_buttons_add_button(i, NULL, store);
		//else
			//tree_buttons_replace_button(i, store);
	}
}


gchar *tree_buttons_get_selected_button_name(gchar **name)
{
	GtkTreeModel *model;
	GtkTreeSelection *sel;
	GtkTreeIter iter, iter_parent;
	
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)));
	if (gtk_tree_selection_get_selected(sel, &model, &iter) == FALSE)
	{
		*name = NULL;
		return NULL;
	}
	if (gtk_tree_model_iter_parent(model, &iter_parent, &iter) == TRUE)
		gtk_tree_model_get(model, &iter_parent, COL_NAME, name, -1);
	else
		gtk_tree_model_get(model, &iter, COL_NAME, name, -1);
	
	return *name;
}

int tree_buttons_update_button(int rawcode)
{
	int button, rawcode_val, found=0;
	GtkTreeModel *model;
	GtkTreeIter iter, iter_parent;
	gchar *name, *rawcode_str;
	
	if ((button = buttons_exists(rawcode)) < 0)
		return 0;
	
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)));
	if (gtk_tree_model_get_iter_first(model, &iter_parent) == FALSE)
	{
		fprintf(stderr, "Warning: cannot update button. Tree is empty.\n");
		return 0;
	}
	while (!found)
	{
		if (gtk_tree_model_iter_children(model, &iter, &iter_parent) == FALSE)
		{
			fprintf(stderr, "Error: button has no children, or another iter level error occurred.\n");
			return 0;
		}
		while (1)
		{
			gtk_tree_model_get(model, &iter, COL_NAME, &name, COL_PROPERTY, &rawcode_str, -1);
			if (strcasecmp("RC", name) == 0)
			{
				rawcode_val = strtol(rawcode_str, NULL, 16);
				g_free(name);
				g_free(rawcode_str);
				if (rawcode_val == rawcode)
					found = 1;
				break;
			}
			//	break;
			g_free(name);
			g_free(rawcode_str);
			if (gtk_tree_model_iter_next(model, &iter) == FALSE)
			{
				fprintf(stderr, "Warning: tree_buttons_update_button did not find a rawcode row.\n");
				break;
			}
		}
		if (found == 1)
			break;
		if (gtk_tree_model_iter_next(model, &iter_parent) == FALSE)
		{
			fprintf(stderr, "Error: tree_buttons_update_button did not find the updated button.\n");
			return -1;
		}
	}
	
	tree_buttons_replace_button(button, &iter_parent, GTK_TREE_STORE(model));
	
	return 1;
}

int tree_buttons_get_selected_button_rawcode(void)
{
	GtkTreeModel *model;
	GtkTreeSelection *sel;
	GtkTreeIter iter, iter_parent;
	gchar *rawcode, *name=NULL;
	int rawcode_val;
	
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)));
	if (gtk_tree_selection_get_selected(sel, &model, &iter) == FALSE)
		return 0;
	if (gtk_tree_model_iter_parent(model, &iter_parent, &iter) == TRUE)
		iter = iter_parent;
	else
		iter_parent = iter;
	
	if (gtk_tree_model_iter_children(model, &iter, &iter_parent) == FALSE)
	{
		fprintf(stderr, "Error: button has no children, or another iter level error occurred.\n");
		return 0;
	}
	
	while (1)
	{
		gtk_tree_model_get(model, &iter, COL_NAME, &name, COL_PROPERTY, &rawcode, -1);
		if (strcasecmp("RC", name) == 0)
			break;
		g_free(name);
		g_free(rawcode);
		if (gtk_tree_model_iter_next(model, &iter) == FALSE)
		{
			fprintf(stderr, "Error: tree_buttons_get_selected_button_rawcode did not find a rawcode row.\n");
			return 0;
		}
	}
	
	rawcode_val = strtol(rawcode, NULL, 16);
	g_free(name);
	g_free(rawcode);
	
	return rawcode_val;
}

void tree_buttons_init(void)
{
	empty = 1;
	tree_buttons_make_store();
	tree_buttons_make_columns();
}

