
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/* Functionality for the configuration manager, which manages multiple
 * configurations and their properties */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config_manager.h"
#include "config.h"
#include "widgets.h"
#include "tree_config.h"
#include "config_file.h"

#define CONFIG_MANAGER_FILE			"/etc/btnx/btnx_manager"
#define CONFIG_MANAGER_FILE_DELIM	"\n"

enum {
	CM_ERROR_FILE_EMPTY=1,
	CM_ERROR_FILE_EXISTS,
	CM_ERROR_FILE_OTHER,
} CONFIG_MANAGER_ERRORS;

static t_configs configs = {
	.count = 0,
	.current = -1,
	.changed = 0,
	.name = NULL
};

static int config_manager_get_index(const char *name)
{
	int x=0;
	
	if (name == NULL)
	{
		fprintf(stderr, "Error: attempted to get index of a null config name.\n");
		return -1;
	}
	if (configs.count <= 0 || configs.name == NULL) 
		return -1;
	while (x < configs.count && configs.name[x] != NULL)
	{
		if (strcmp(name, configs.name[x]) == 0)
			return x;
		x++;
	}
	return -1;
}

static void config_manager_check_files(void)
{
	struct stat st;
	gchar *path;
	int x=0;
	
	if (configs.count < 0 || configs.name == NULL)
		return;
	
	while (x < configs.count)
	{
		if (configs.name == NULL) break;
		if (configs.name[x] == NULL) break;
		path=g_strdup_printf("/etc/btnx/btnx_config_%s",configs.name[x]);
		if (stat(path, &st) < 0)
		{
			fprintf(stderr, "Warning: configuration file for configuration \"%s\" "
					"does not exist. Deleting configuration.\n", configs.name[x]);
			config_manager_delete(configs.name[x]);
		}
		else
			x++;
		g_free(path);
	}
}

void config_manager_free(void)
{
	if (configs.name != NULL)
	{
		g_strfreev(configs.name);
		configs.name = NULL;
	}
	configs.count = 0;
	configs.current = -1;
	configs.changed = 0;
	
	config_manager_set_widget_states(FALSE);
}

int config_manager_new(char *name, int copy)
{
	char *tmp_name;
	int x=1;
	int ret=0;
	
	if (name == NULL)
		return -1;
	
	tmp_name = g_strdup(name);
	
	if (config_manager_get_index(tmp_name) >= 0)
	{
		do
		{
			g_free(tmp_name);
			tmp_name = g_strdup_printf("%s%d", name, x);
			ret = x;
			x++;
		} while (x<MAX_CONFIGS && config_manager_get_index(tmp_name) >= 0);
		if (x >= MAX_CONFIGS)
		{
			fprintf(stderr, "Warning: config_manager_new max number of similar "
					"config names reached. New config not created.\n");
			g_free(tmp_name);
			return -1;
		}
	}
	
	configs.name = g_renew(gchar*, configs.name, configs.count+2);
	configs.name[configs.count] = tmp_name;
	configs.name[configs.count+1] = NULL;
	configs.count++;
	
	if (copy == 0)
	{
		config_clear_all();
	}
	else
	{
		
	}
	config_manager_set_current(configs.name[configs.count-1]);
	config_manager_set_changed(1);
	gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_CONFIG_NAME_CONFIG)), tmp_name);
	/* update all fields */
	
	tree_config_add_config(tmp_name, 1, configs.count);
	config_update_config_combobox();
	
	return ret;
}

int config_manager_delete(char *name)
{
	int x, index, current=0;
	
	if (configs.count < 1 || configs.name == NULL)
	{
		fprintf(stderr, "Warning: config_manager_delete attempted to delete "
						"a configuration but none exist.\n");
		return -1;
	}
	if (name == NULL)
	{
		fprintf(stderr, "Error: null config name passed to config_manager_delete.\n");
		return -1;
	}
	index = config_manager_get_index(name);
	if (index < 0)
	{
		fprintf(stderr, "Warning: config_manager_delete attempted to delete "
				"an unexisting configuration.\n");
		return -1;
	}
	if (strcmp(configs.name[configs.current], name) == 0)
		current = 1;
	tree_config_remove_selected();	// should remove according to name
	
	config_file_delete(name);
	
	if (configs.count == 1 && index == 0)
	{
		config_manager_free();
		return 0;
	}
	g_free(configs.name[index]);
	for (x=index+1; x<configs.count; x++)
	{
		configs.name[x-1] = configs.name[x];
		if (configs.name[x] == NULL)
			break;
	}
	configs.name = g_renew(gchar*, configs.name, configs.count);
	configs.count--;
	configs.name[configs.count] = NULL;
	
	config_update_config_combobox();
	
	if (configs.count > 0 && current == 1)
	{
		if (index >= configs.count)
			index = configs.count - 1;
		config_manager_set_current(configs.name[index]);
		config_manager_open_config(configs.name[index]);
	}
	
	return 0;
}

int config_manager_priority_higher(char *name)
{
	int index;
	gchar *tmp=NULL;
	
	index = config_manager_get_index(name);
	if (index <= 0)
		return -1;
	if (tree_config_move_selected_up() < 0)
		return -1;
	tmp = configs.name[index-1];
	configs.name[index-1] = configs.name[index];
	configs.name[index] = tmp;
	if (configs.current == index) configs.current--;
	return 0;
}

int config_manager_priority_lower(char *name)
{
	int index;
	gchar *tmp=NULL;
	
	index = config_manager_get_index(name);
	if (index == configs.count-1)
		return -1;
	
	if (tree_config_move_selected_down() < 0)
		return -1;
	
	tmp = configs.name[index+1];
	configs.name[index+1] = configs.name[index];
	configs.name[index] = tmp;
	
	if (configs.current == index) configs.current++;
	return 0;
}

const char *config_manager_get_current_name(void)
{
	if (configs.current < 0 || configs.count <=0 || configs.name == NULL ||
		configs.current >= configs.count)
		return NULL;
	return configs.name[configs.current];
}

const char *config_manager_get_name(int index)
{
	if (index < configs.count && index >= 0)
		return configs.name[index];
	else
		return NULL;
}

int config_manager_current(void)
{
	return configs.current;
}

int config_manager_changed(void)
{
	return configs.changed;
}

int config_manager_count(void)
{
	return configs.count;
}

void config_manager_set_changed(int changed)
{
	if (configs.current < 0)
	{
		/*fprintf(stderr, "Warning: config_manager_config_changed called "
				"but no configuration is currently open.\n");*/
		return;
	}
	configs.changed = changed;
}

int config_manager_set_current(const char *name)
{
	int x;
	
	if (configs.count < 1 || configs.name == NULL)
	{
		fprintf(stderr, "Warning: config_manager_set current called but no "
				"configurations exist.\n");
		return -1;
	}
	x = config_manager_get_index(name);
	
	if (x < 0)
	{
		fprintf(stderr, "Warning: config_manager_set current tried to set "
				"an unexisting configuration.\n");
		return -1;
	}
	configs.current = x;
	tree_config_set_current(name);
	
	/* read file and set config values */
	//config_manager_open_config(name);
	
	gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_CONFIG_NAME_CONFIG)), name);
	config_manager_set_widget_states(TRUE);
	
	return 0;
}

int config_manager_set_current_name(const char *new_name)
{
	char *tmp_name, *config_path, *config_path_new;
	int x, only_space=1;
	struct stat st;
	
	if (configs.count <= 0 || configs.current < 0 || configs.name == NULL)
		return -1;
	if (g_strstr_len(new_name, strlen(new_name), "/") != NULL)
		return -2;
	if (g_strstr_len(new_name, strlen(new_name), "\"") != NULL)
		return -2;
	if (g_strstr_len(new_name, strlen(new_name), "\\") != NULL)
		return -2;
	if (strcmp(new_name, "NONE") == 0)
		return -3;
	if (strlen(new_name) == 0)
		return -3;
	
	for (x=0; x<strlen(new_name); x++)
	{
		if (new_name[x] != ' ') 
		{
			only_space = 0;
			break;
		}
	}
	if (only_space==1)
		return -3;
	
	for (x=0; x<configs.count; x++)
	{
		if (strcmp(new_name, configs.name[x]) == 0 && configs.current != x)
			return -4;
	}
	
	tree_config_update_config(configs.name[configs.current], 1, configs.current+1, new_name);
	
	config_path = g_strconcat("/etc/btnx/btnx_config_", configs.name[configs.current], NULL);
	if (stat(config_path, &st) == 0)
	{
		config_path_new = g_strconcat("/etc/btnx/btnx_config_", new_name, NULL);
		rename(config_path, config_path_new);
		g_free(config_path_new);
	}
	g_free(config_path);
	
	tmp_name = g_strdup(new_name);
	g_free(configs.name[configs.current]);
	configs.name[configs.current] = tmp_name;
	
	return 0;
}

void config_manager_open_config(const char *name)
{
	if (config_manager_set_current(name) < 0)
	{
		fprintf(stderr, "Warning: config_manager_open_config attempted to open "
				"an unexisting configuration.\n");
		return;
	}
	config_clear_all();
	config_file_parse(name);
	config_refresh_all_fields();
	config_manager_set_changed(0);
}

void config_manager_open_selected_config(void)
{
	gchar *name;
	int current=0;
	
	tree_config_get_selected_data(&current, NULL, &name);
	if (current != 0)
		return;
	config_manager_open_config(name);
	/*
	config_manager_set_current(name);
	config_clear_all();
	config_file_parse(name);
	config_refresh_all_fields();
	config_manager_set_changed(0);
	*/
	g_free(name);
}

static int config_manager_read_file(void)
{
	gchar *contents = NULL;
	gsize length = 0;
	GError *error = NULL;
	int x=0;
	
	if (configs.name != NULL)
		config_manager_free();
	
	if (g_file_test(CONFIG_MANAGER_FILE, G_FILE_TEST_EXISTS) == FALSE)
		return CM_ERROR_FILE_EXISTS;
	g_file_get_contents(CONFIG_MANAGER_FILE, &contents, &length, &error);
	if (error != NULL)
	{
		fprintf(stderr, "Error: could not read file %s: %s\n",
				CONFIG_MANAGER_FILE, error->message);
		g_error_free(error);
		return CM_ERROR_FILE_OTHER;
	}
	if (length == 0)
		return CM_ERROR_FILE_EMPTY;
	
	configs.name = g_strsplit(contents, CONFIG_MANAGER_FILE_DELIM, MAX_CONFIGS);
	
	x=0;
	while (configs.name[x] != NULL) x++;
	configs.count = x;
	config_manager_check_files();
	if (configs.count > 0)
		config_manager_set_current(configs.name[0]);
	
	g_free(contents);
	
	tree_config_empty_store();
	x=0;
	if (configs.name != NULL)
	{
		while (configs.name[x] != NULL)
		{
			tree_config_add_config(configs.name[x], 0, x+1);
			x++;
		}
	}
		
	return 0;
}

static void config_manager_delete_file(void)
{
	unlink(CONFIG_MANAGER_FILE);
}

int config_manager_save_file(void)
{
	gchar *contents=NULL;
	GError *error=NULL;
	
	if (configs.count <= 0 || configs.name == NULL)
	{
		fprintf(stderr, "Note: config_manager_save_file failed because no "
				"configurations exist. Old config manager file will be deleted.\n");
		config_manager_delete_file();
		return -1;
	}
	
	contents = g_strjoinv(CONFIG_MANAGER_FILE_DELIM, configs.name);
	g_file_set_contents(CONFIG_MANAGER_FILE, contents, -1, &error);
	
	if (error != NULL)
	{
		fprintf(stderr, "Error: config_manager_save_file failed to write "
				"to file %s: %s\n", CONFIG_MANAGER_FILE, error->message);
		g_error_free(error);
		g_free(contents);
		return -1;
	}
	
	g_free(contents);
	
	return 0;
}

void config_manager_set_widget_states(int enabled)
{
	gboolean en = (gboolean) enabled;
	
	SET_WIDGET_STATE(BTN_DELETE_CONFIG, en);
	SET_WIDGET_STATE(BTN_UP_CONFIG, 	en);
	SET_WIDGET_STATE(BTN_DOWN_CONFIG, 	en);
	SET_WIDGET_STATE(BTN_BACKUP_CONFIG, en);
	//SET_WIDGET_STATE(BTN_CLEAR_CONFIG, 	en);
	SET_WIDGET_STATE(BTN_DETECT_CONFIG, en);
	SET_WIDGET_STATE(ENTRY_CONFIG_NAME_CONFIG, en);
	SET_WIDGET_STATE(ENTRY_VENDOR_CONFIG, en);
	SET_WIDGET_STATE(ENTRY_PRODUCT_CONFIG, en);
}

/* Allow transition from old versions of btnx-config */
static int config_manager_open_old(void)
{
	struct stat st;
	char *name, *new_path;
	int ret;
	
	if (stat("/etc/btnx/btnx_config", &st) < 0)
		return -1;
		
	if ((ret = config_manager_new("old_config", 0)) < 0)
		return -1;
	else if (ret > 0)
	{
		name = g_strdup_printf("old_config%d", ret);
		new_path = g_strdup_printf("/etc/btnx/btnx_config_%s", name);
		if (stat(new_path, &st) == 0)
		{
			fprintf(stderr, "Error: the programmer was too lazy to take into "
					"account this problem. Tough luck.\n");
			g_free(name);
			g_free(new_path);
			return -1;
		}
	}
	else /* ret == 0 */
	{
		name = g_strdup("old_config");
		new_path = g_strdup_printf("/etc/btnx/btnx_config_%s", name);
		if (stat(new_path, &st) == 0)
		{
			fprintf(stderr, "Error: the programmer was too lazy to take into "
					"account this problem. Tough luck.\n");
			g_free(name);
			g_free(new_path);
			return -1;
		}
	}
	if (rename("/etc/btnx/btnx_config", new_path) < 0)
	{
		config_manager_delete(name);
		g_free(name);
		g_free(new_path);
		return -1;
	}
	/*if (config_file_parse(name) < 0)
	{
		rename(new_path, "/etc/btnx/btnx_config");
		config_manager_delete(name);
		g_free(name);
		g_free(new_path);
		return -1;
	}*/
	
	g_free(name);
	g_free(new_path);
		
	return 0;
}

int config_manager_init(void)
{
	int ret;
	ret = config_manager_read_file();
	if (config_manager_open_old() == 0)
		config_manager_save_file();
	
	if (configs.count > 0 && configs.current >= 0)
	{
		config_manager_open_config(configs.name[configs.current]);
		//config_file_parse(configs.name[configs.current]);
		tree_config_set_current(configs.name[configs.current]);
	}
	else if (configs.count <= 0)
		config_manager_new("Default", 0);
	if (configs.count >= 0 && configs.current >= 0)
		config_manager_set_widget_states(TRUE);
	else
		config_manager_set_widget_states(FALSE);
	
	config_update_config_combobox();
	
	switch (ret)
	{
	case CM_ERROR_FILE_EXISTS:
	case CM_ERROR_FILE_EMPTY:
		return -1;
	case CM_ERROR_FILE_OTHER:
		return -1;
	default:
		break;
	}
	
	return 0;
}
