
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef KEYCODES_H_
#define KEYCODES_H_

int keycodes_parse(void);
void keycodes_free(void);
int keycodes_get_index(const gchar *keycode);
int mods_get_index(const gchar *mod);

// Use g_free() on the return values when done
// Returns NULL on error
gchar *keycodes_get_keycode(int index);
gchar *mods_get_keycode(int index);

#endif /*KEYCODES_H_*/
