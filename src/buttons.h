
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef BUTTONS_H_
#define BUTTONS_H_

typedef struct btnx_button
{
	int rawcode;
	int type;
	int delay;
	int enabled;
	int force_release;
	int evtype;
	int switch_type;
	char *switch_name;
	char *keycode;
	char *mod1;
	char *mod2;
	char *mod3;
	int pressed;
	char *name;
	char *command;
	char **args;
	int uid;
	int new;
} btnx_button;

int buttons_add(int rawcode);
void buttons_free(void);
int buttons_remove(int index);
int buttons_remove_new(void);
int buttons_exists(int rawcode);
int buttons_name_exists(const char *name);
int buttons_check_index(int index);
int buttons_set_name(int index, const char *name);
int buttons_count(void);

int buttons_get_rawcode(int index);
int buttons_get_type(int index);
int buttons_get_delay(int index);
int buttons_get_enabled(int index);
int buttons_get_force_release(int index);
int buttons_get_evtype(int index);
int buttons_get_switch_type(int index);
const char *buttons_get_switch_name(int index);
const char *buttons_get_keycode(int index);
const char *buttons_get_mod1(int index);
const char *buttons_get_mod2(int index);
const char *buttons_get_mod3(int index);
int buttons_get_pressed(int index);
int buttons_get_uid(int index);
const char *buttons_get_name(int index);
const char *buttons_get_command(int index);
//const char **buttons_get_args(int index);

int buttons_set_rawcode(int index, int rawcode);
int buttons_set_type(int index, int type);
int buttons_set_delay(int index, int delay);
int buttons_set_enabled(int index, int enabled);
int buttons_set_force_release(int index, int release);
int buttons_set_evtype(int index, int evtype);
int buttons_set_switch_type(int index, int switch_type);
int buttons_set_switch_name(int index, const char *switch_name);
int buttons_set_keycode(int index, const char *keycode);
int buttons_set_mod1(int index, const char *mod1);
int buttons_set_mod2(int index, const char *mod2);
int buttons_set_mod3(int index, const char *mod3);
int buttons_set_uid(int index, int uid);
int buttons_set_new(int index, int new);
int buttons_set_command(int index, const char *command);
void buttons_set_old_all(void);


#endif /*BUTTONS_H_*/
