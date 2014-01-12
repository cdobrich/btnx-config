
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "tree_config.h"
#include "config_manager.h"
#include "widgets.h"
#include "config.h"
#include "text.h"

#define CURRENT_IDENTIFIER "*"

enum
{
	COL_CURRENT,
	COL_PRIORITY,
	COL_NAME,
	NUM_COL
};

static void tree_config_make_store(void)
{	
	GtkTreeStore *store;
	store = gtk_tree_store_new(	NUM_COL,
								G_TYPE_STRING,
								G_TYPE_INT,
								G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(GET_WIDGET(TREE_CONFIGS)),
							GTK_TREE_MODEL(store));
	g_object_unref(G_OBJECT(store));
}

static void tree_config_make_columns(void)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeView *view = GTK_TREE_VIEW(GET_WIDGET(TREE_CONFIGS));
	
	//	Current column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(	_("Open"),
														renderer,
														"text", COL_CURRENT,
														NULL);
	gtk_tree_view_column_set_min_width (column, 20);
	gtk_tree_view_append_column(view, column);
	
	// Priority column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(	_("Priority"),
														renderer,
														"text", COL_PRIORITY,
														NULL);
	gtk_tree_view_column_set_min_width (column, 40);
	gtk_tree_view_append_column(view, column);
	
	// Priority column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(	_("Name"),
														renderer,
														"text", COL_NAME,
														NULL);
	gtk_tree_view_column_set_min_width (column, 60);
	gtk_tree_view_append_column(view, column);
}

static GtkTreeStore *tree_config_get_store(void)
{
	return GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(GET_WIDGET(TREE_CONFIGS))));
}

static int tree_config_iter_equals(GtkTreeIter *iter1, GtkTreeIter *iter2)
{
	GtkTreeModel *model;
	gchar *name1, *name2;
	int ret=0;
	
	model = GTK_TREE_MODEL(tree_config_get_store());
	
	gtk_tree_model_get(	model, iter1,
						COL_NAME, &name1,
						-1);
	gtk_tree_model_get(	model, iter2,
						COL_NAME, &name2,
						-1);
	
	if (strcmp(name1, name2) == 0)
		ret = 1;
	
	g_free(name1);
	g_free(name2);
	return ret;
}

void tree_config_empty_store(void)
{
	GtkTreeIter iter;
	GtkTreeStore *store = tree_config_get_store();
	
	if (!gtk_tree_model_get_iter_first(	GTK_TREE_MODEL(store), &iter))
	{
		//fprintf(stderr, "Warning: tree_config_empty_store could not get an iter.\n");
		return;
	}
	while (gtk_tree_store_remove(store, &iter));
}

static void tree_config_clear_current(void)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	
	model = GTK_TREE_MODEL(tree_config_get_store());
	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
	{
		//fprintf(stderr, "Warning: cannot find iter. Config tree is empty.\n");
		return;
	}
	
	while (1)
	{
		gtk_tree_store_set(	GTK_TREE_STORE(model), &iter,
							COL_CURRENT, " ",
							-1);
		if (gtk_tree_model_iter_next(model, &iter) == FALSE)
			break;
	}
	
}

static void tree_config_update_priorities(void)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	int x=1;
	
	model = GTK_TREE_MODEL(tree_config_get_store());
	
	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
	{
		//fprintf(stderr, "Warning: tree_config_update_priorities cannot find iter. Config tree is empty.\n");
		return;
	}
	
	do
	{
		gtk_tree_store_set(	GTK_TREE_STORE(model), &iter,
							COL_PRIORITY, x++,
							-1);
	} while (gtk_tree_model_iter_next(model, &iter) != FALSE);
}

void tree_config_add_config(char *name, int current, int priority)
{
	gchar *tmp_current;
	GtkTreeIter iter;
	GtkTreeStore *store = tree_config_get_store();
	
	if (name == NULL || priority < 0)
	{
		fprintf(stderr, "Warning: tree_config_add_config attempted to add an " 
				"and invalid configuration to tree.\n");
		return;
	}
	//g_print("btnd dialog adding: rawcode_str=%s clicks=%d det=%d\n", rawcode_str, clicks, (gint)detected);
	
	if (current == 0)
		tmp_current = g_strdup(" ");
	else
	{
		tmp_current = g_strdup(CURRENT_IDENTIFIER);
		tree_config_clear_current();
	}
		
	gtk_tree_store_append(store, &iter, NULL);
	gtk_tree_store_set(	store, &iter,
						COL_CURRENT,	tmp_current,
						COL_PRIORITY,	priority,
						COL_NAME,		name,
						-1);
	
	g_free(tmp_current);
}

int tree_config_remove_selected(void)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	GtkTreeSelection *sel;
	
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREE_CONFIGS)));
	if (gtk_tree_selection_get_selected(sel, &model, &iter) == FALSE)
	{
		fprintf(stderr,
		"Warning: tree_config_remove_selected error, no selected config.\n");
		return -1;
	}
	
	gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
	
	tree_config_update_priorities();
	
	return 0;
}
	
static int tree_config_find_iter(const gchar *name, GtkTreeIter *iter, GtkTreeStore *store)
{
	gchar *tmp_name=NULL;
	GtkTreeModel *model = GTK_TREE_MODEL(store);
	
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(GET_WIDGET(TREE_CONFIGS)));
	if (gtk_tree_model_get_iter_first(model, iter) == FALSE)
	{
		//fprintf(stderr, "Warning: tree_config_find_iter cannot find iter. Config tree is empty.\n");
		return -1;
	}
	while (1)
	{
		gtk_tree_model_get(model, iter, COL_NAME, &tmp_name, -1);
		if (strcasecmp(tmp_name, name) == 0)
		{
			g_free(tmp_name);
			break;
		}
		g_free(tmp_name);
		if (gtk_tree_model_iter_next(model, iter) == FALSE)
		{
			fprintf(stderr, "Warning: tree_config_find_iter could not match name.\n");
			return -1;
		}
	}
	return 0;
}

int tree_config_update_config(const gchar *old_name, int current, int priority, const gchar *new_name)
{
	GtkTreeStore *store;
	GtkTreeIter iter;
	gchar *tmp_current=NULL;
	
	store = tree_config_get_store();
	
	if (tree_config_find_iter(old_name, &iter, store) < 0)
		return -1;
	
	if (current == 0)
		tmp_current = g_strdup(" ");
	else
	{
		tmp_current = g_strdup(CURRENT_IDENTIFIER);
		tree_config_clear_current();
	}
		
	gtk_tree_store_set(	store, &iter,
						COL_CURRENT, 	tmp_current,
						COL_PRIORITY, 	priority,
						COL_NAME,		new_name,
						-1);
						
	g_free(tmp_current);
						
	return 0;
}

int tree_config_set_current(const gchar *name)
{
	GtkTreeStore *store;
	GtkTreeIter iter;
	
	store = tree_config_get_store();
	tree_config_clear_current();	
	if (tree_config_find_iter(name, &iter, store) < 0)
		return -1;
	
	gtk_tree_store_set( store, &iter,
						COL_CURRENT, CURRENT_IDENTIFIER,
						-1);
	return 0;
}

int tree_config_move_selected_up(void)
{
	GtkTreeSelection *sel;
	GtkTreeModel *model;
	GtkTreeIter iter, iter_before, iter_tmp;
	
	//model = GTK_TREE_MODEL(tree_config_get_store());
	
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREE_CONFIGS)));
	if (gtk_tree_selection_get_selected(sel, &model, &iter) == FALSE)
		return -1;
	
	gtk_tree_model_get_iter_first(model, &iter_before);
	if (tree_config_iter_equals(&iter_before, &iter) == 1)
		return -1;
	
	iter_tmp = iter_before;
	
	while (tree_config_iter_equals(&iter_tmp, &iter) == 0)
	{
		iter_before = iter_tmp;
		gtk_tree_model_iter_next(model, &iter_tmp);
	}
	
	gtk_tree_store_move_before(GTK_TREE_STORE(model), &iter, &iter_before);
	tree_config_update_priorities();
	return 0;
}

int tree_config_move_selected_down(void)
{
	GtkTreeSelection *sel;
	GtkTreeModel *model;
	GtkTreeIter iter, iter_after;
	
	//model = GTK_TREE_MODEL(tree_config_get_store());
	
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREE_CONFIGS)));
	if (gtk_tree_selection_get_selected(sel, &model, &iter) == FALSE)
		return -1;
	
	iter_after = iter;
	
	if (gtk_tree_model_iter_next(model, &iter_after) == FALSE)
		return -1;
	
	gtk_tree_store_move_after(GTK_TREE_STORE(model), &iter, &iter_after);
	tree_config_update_priorities();
	return 0;
}

int tree_config_get_selected_data(int *current, int *priority, gchar **name)
{
	GtkTreeModel *model;
	GtkTreeSelection *sel;
	GtkTreeIter iter;
	int tmp_priority=0;
	gchar *tmp_name=NULL, *tmp_current=NULL;
	
	//model = GTK_TREE_MODEL(tree_config_get_store());
	
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(GET_WIDGET(TREE_CONFIGS)));
	if (gtk_tree_selection_get_selected(sel, &model, &iter) == FALSE)
		return -1;
	gtk_tree_model_get(	model, &iter,
						COL_CURRENT, 	&tmp_current,
						COL_PRIORITY, 	&tmp_priority,
						COL_NAME, 		&tmp_name,
						-1);
	if (current != NULL)
	{
		if (strncmp(CURRENT_IDENTIFIER, tmp_current, 1) == 0)
			*current = 1;
		else
			*current = 0;
	}
	if (priority != NULL)
		*priority = tmp_priority;
	if (name != NULL)
		*name = tmp_name;
	else
		g_free(tmp_name);
	
	g_free(tmp_current);
	
	return 0;
}

void tree_config_init(void)
{
	tree_config_make_store();
	tree_config_make_columns();
}
