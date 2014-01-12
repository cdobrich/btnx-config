
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#ifndef KEYCODES_ENTRY_H_
#define KEYCODES_ENTRY_H_

GtkWidget *keycodes_entry_get_entry(void);
void keycodes_entry_init(void);
void keycodes_entry_add(const gchar *keycode);

#endif /*KEYCODES_ENTRY_H_*/
