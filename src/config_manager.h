
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef CONFIG_MANAGER_H_
#define CONFIG_MANAGER_H_

#include <gtk/gtk.h>

#define MAX_CONFIGS		100

typedef struct t_configs
{
	gint count;		/* Number of configs */
	gint current;	/* Currently open config */
	gint changed;	/* Flag whether current config has changed since saving */
	gchar **name;	/* Names of configs */
} t_configs;

int config_manager_init(void);
void config_manager_free(void);
int config_manager_new(char *name, int copy);
int config_manager_delete(char *name);
int config_manager_priority_higher(char *name);
int config_manager_priority_lower(char *name);
int config_manager_current(void);
int config_manager_save_file(void);
int config_manager_changed(void);
void config_manager_set_changed(int changed);
int config_manager_count(void);
const char *config_manager_get_current_name(void);
int config_manager_set_current(const char *name);
int config_manager_set_current_name(const char *new_name);
void config_manager_open_config(const char *name);
void config_manager_open_selected_config(void);
const char *config_manager_get_name(int index);
void config_manager_set_widget_states(int enabled);

#endif /*CONFIG_MANAGER_H_*/
