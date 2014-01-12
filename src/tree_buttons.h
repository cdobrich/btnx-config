
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef TREE_BUTTONS_H_
#define TREE_BUTTONS_H_

#define tree_buttons_populate() tree_buttons_replace_and_populate(0)
#define tree_buttons_replace() tree_buttons_replace_and_populate(1)

void tree_buttons_init(void);
void tree_buttons_replace_and_populate(int replace);
void tree_buttons_remove_button(void);
gchar *tree_buttons_get_selected_button_name(gchar **name);
int tree_buttons_update_button(int rawcode);
int tree_buttons_get_selected_button_rawcode(void);

#endif /*TREE_BUTTONS_H_*/
