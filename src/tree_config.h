
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef TREE_CONFIG_H_
#define TREE_CONFIG_H_


int tree_config_move_selected_up(void);
int tree_config_move_selected_down(void);
int tree_config_get_selected_data(int *current, int *priority, gchar **name);
void tree_config_init(void);
int tree_config_update_config(const gchar *old_name, int current, int priority, const gchar *new_name);
int tree_config_remove_selected(void);
void tree_config_add_config(char *name, int current, int priority);
int tree_config_set_current(const gchar *name);
void tree_config_empty_store(void);

#endif /*TREE_CONFIG_H_*/
