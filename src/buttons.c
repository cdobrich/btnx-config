
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/* Button data structure, contains all necessary information about a detected
 * and configured button. Contains functions to manipulate them */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "buttons.h"

static btnx_button **buttons=NULL;
static int num_buttons=0;

int buttons_add(int rawcode)
{
	if (buttons_exists(rawcode) >= 0)
	{
		fprintf(stderr, "Warning: button_add quitting: button already exists.\n");
		return -1;
	}
	
	if (num_buttons == 0)
	{
		buttons = (btnx_button **) malloc(sizeof(btnx_button*));
		buttons[0] = NULL;
	}
	else
	{
		buttons = (btnx_button **) realloc(buttons, (num_buttons+1)*sizeof(btnx_button*));
		buttons[num_buttons] = NULL;
	}
	
	buttons[num_buttons] = (btnx_button *) malloc(sizeof(btnx_button));
	
	if (buttons[num_buttons] == NULL || buttons == NULL)
	{
		fprintf(stderr, "Error: buttons_add button allocation failed: %s\n", strerror(errno));
		return -1;
	}
	
	buttons[num_buttons]->rawcode = rawcode;
	if ((rawcode & 0x02000000) == 0x02000000)
		buttons[num_buttons]->type = 1;
	else if ((rawcode & 0x01000000) == 0x01000000)
		buttons[num_buttons]->type = 0;
	else
		buttons[num_buttons]->type = -1;
	buttons[num_buttons]->delay = 0;
	buttons[num_buttons]->enabled = 0;
	buttons[num_buttons]->evtype = 0;
	buttons[num_buttons]->force_release = 0;
	buttons[num_buttons]->switch_type = 0;
	buttons[num_buttons]->switch_name = NULL;
	buttons[num_buttons]->keycode = NULL;
	buttons[num_buttons]->mod1 = NULL;
	buttons[num_buttons]->mod2 = NULL;
	buttons[num_buttons]->mod3 = NULL;
	
	buttons[num_buttons]->pressed = 0;
	buttons[num_buttons]->name = NULL;
	buttons[num_buttons]->command = NULL;
	buttons[num_buttons]->args = NULL;
	buttons[num_buttons]->uid = 0;
	buttons[num_buttons]->new = 0;
	
	num_buttons++;
	
	
	buttons_set_keycode(num_buttons-1, "NONE");
	buttons_set_mod1(num_buttons-1, "NONE");
	buttons_set_mod2(num_buttons-1, "NONE");
	buttons_set_mod3(num_buttons-1, "NONE");
	buttons_set_switch_name(num_buttons-1, "NONE");
	
	return num_buttons-1;
}

void buttons_free(void)
{
	int i, j;
	if (buttons != NULL)
	{
		for (i=0; i<num_buttons; i++)
		{
			j=0;
			if (buttons[i] != NULL)
			{
				if (buttons[i]->command != NULL)
					free(buttons[i]->command);
				if (buttons[i]->name != NULL)
					free(buttons[i]->name);
				if (buttons[i]->keycode != NULL)
					free(buttons[i]->keycode);
				if (buttons[i]->mod1 != NULL)
					free(buttons[i]->mod1);
				if (buttons[i]->mod2 != NULL)
					free(buttons[i]->mod2);
				if (buttons[i]->mod3 != NULL)
					free(buttons[i]->mod3);
				if (buttons[i]->switch_name != NULL)
					free(buttons[i]->switch_name);
				if (buttons[i]->args != NULL)
				{
					while (buttons[i]->args[j] != NULL)
						free(buttons[i]->args[j++]);
				}
				free(buttons[i]);
			}
			buttons[i] = NULL;
		}
		free(buttons);
	}
	buttons = NULL;
	num_buttons = 0;
}

int buttons_remove(int index)
{
	int i = 0;
	if (buttons_check_index(index) < 0)
		return -1;
	
	if (buttons[index] != NULL)
	{
		if (buttons[index]->command != NULL)
			free(buttons[index]->command);
		if (buttons[index]->name != NULL)
			free(buttons[index]->name);
		if (buttons[index]->keycode != NULL)
			free(buttons[index]->keycode);
		if (buttons[index]->mod1 != NULL)
			free(buttons[index]->mod1);
		if (buttons[index]->mod2 != NULL)
			free(buttons[index]->mod2);
		if (buttons[index]->mod3 != NULL)
			free(buttons[index]->mod3);
		if (buttons[index]->switch_name != NULL)
			free(buttons[index]->switch_name);
		if (buttons[index]->args != NULL)
		{
			while (buttons[index]->args[i] != NULL)
				free(buttons[index]->args[i++]);
		}
		free(buttons[index]);
	}
	buttons[index] = NULL;
	
	for (i=index+1; i<num_buttons; i++)
	{
		buttons[i-1] = buttons[i];
	}
	buttons[num_buttons-1] = NULL;
	num_buttons--;
	
	if (num_buttons == 0 && buttons != NULL)
	{
		free(buttons);
		buttons = NULL;
	}
	else
	{
		buttons = (btnx_button **) realloc(buttons, num_buttons*sizeof(btnx_button *));
	}
	
	return num_buttons;
}

int buttons_remove_new(void)
{
	int i;
	for (i=0; i<num_buttons; i++)
	{
		if (buttons[i]->new == 1)
		{
			buttons_remove(i);
			i--;
		}
	}
	return 1;
}

int buttons_check_index(int index)
{
	if (index >= 0 && index < num_buttons)
		return 1;
	//fprintf(stderr, "Warning: invalid button index reported by buttons_check_index.\n");
	return -1;
}

int buttons_exists(int rawcode)
{
	int i;
	for (i=0; i<num_buttons; i++)
	{
		if (buttons[i]->rawcode == rawcode)
			return i;
	}
	return -1;
}

int buttons_name_exists(const char *name)
{
	int i;
	for (i=0; i<num_buttons; i++)
	{
		if (buttons[i]->name == NULL)
			continue;
		if (strncasecmp(name, buttons[i]->name, 100) == 0)
			return i;
	}
	return -1;
}

int buttons_count(void)
{
	return num_buttons;
}


int buttons_get_rawcode(int index)
{	
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->rawcode;
}

int buttons_get_evtype(int index)
{	
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->evtype;
}

int buttons_get_type(int index)
{	
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->type;
}

int buttons_get_enabled(int index)
{	
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->enabled;
}

int buttons_get_force_release(int index)
{	
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->force_release;
}

int buttons_get_delay(int index)
{	
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->delay;
}

int buttons_get_switch_type(int index)
{
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->switch_type;
}

const char *buttons_get_switch_name(int index)
{	
	if (buttons_check_index(index) < 0)
		return NULL;
	return buttons[index]->switch_name;
}

const char *buttons_get_keycode(int index)
{	
	if (buttons_check_index(index) < 0)
		return NULL;
	return buttons[index]->keycode;
}

const char *buttons_get_mod1(int index)
{	
	if (buttons_check_index(index) < 0)
		return NULL;
	return buttons[index]->mod1;
}

const char *buttons_get_mod2(int index)
{	
	if (buttons_check_index(index) < 0)
		return NULL;
	return buttons[index]->mod2;
}

const char *buttons_get_mod3(int index)
{	
	if (buttons_check_index(index) < 0)
		return NULL;
	return buttons[index]->mod3;
}

int buttons_get_pressed(int index)
{	
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->pressed;
}

int buttons_get_uid(int index)
{	
	if (buttons_check_index(index) < 0)
		return -1;
	return buttons[index]->uid;
}

const char *buttons_get_name(int index)
{	
	if (buttons_check_index(index) < 0)
		return NULL;
	return buttons[index]->name;
}

const char *buttons_get_command(int index)
{	
	if (buttons_check_index(index) < 0)
		return NULL;
	return buttons[index]->command;
}

/*const char **buttons_get_args(int index)
{	
	if (buttons_check_index(index) < 0)
		return NULL;
	return buttons[index]->args;
}*/


int buttons_set_rawcode(int index, int rawcode)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->rawcode = rawcode;
	return rawcode;
}

int buttons_set_name(int index, const char *name)
{
	if (buttons_check_index(index) < 0)
		return -1;
	if (buttons[index]->name != NULL)
		free(buttons[index]->name);
	if (name != NULL)
	{
		buttons[index]->name = (char *) malloc((strlen(name)+1) * sizeof(char));
		strcpy(buttons[index]->name, name);
	}
	else
		buttons[index]->name = NULL;
	return 1;
}

int buttons_set_type(int index, int type)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->type = type;
	return type;
}

int buttons_set_delay(int index, int delay)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->delay = delay;
	return delay;
}

int buttons_set_switch_type(int index, int switch_type)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->switch_type = switch_type;
	return switch_type;
}

int buttons_set_enabled(int index, int enabled)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->enabled = enabled;
	return enabled;
}

int buttons_set_force_release(int index, int force_release)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->force_release = force_release;
	return force_release;
}

int buttons_set_evtype(int index, int evtype)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->evtype = evtype;
	return evtype;
}

int buttons_set_switch_name(int index, const char *switch_name)
{
	if (buttons_check_index(index) < 0)
		return -1;
	if (buttons[index]->switch_name != NULL)
		free(buttons[index]->switch_name);
	if (switch_name != NULL)
	{
		buttons[index]->switch_name = (char *) malloc((strlen(switch_name)+1) * sizeof(char));
		strcpy(buttons[index]->switch_name, switch_name);
	}
	else
		buttons[index]->switch_name = NULL;
	return 1;
}

int buttons_set_keycode(int index, const char *keycode)
{
	if (buttons_check_index(index) < 0)
		return -1;
	if (buttons[index]->keycode != NULL)
		free(buttons[index]->keycode);
	if (keycode != NULL)
	{
		buttons[index]->keycode = (char *) malloc((strlen(keycode)+1) * sizeof(char));
		strcpy(buttons[index]->keycode, keycode);
	}
	else
		buttons[index]->keycode = NULL;
	return 1;
}

int buttons_set_mod1(int index, const char *mod1)
{
	if (buttons_check_index(index) < 0)
		return -1;
	if (buttons[index]->mod1 != NULL)
		free(buttons[index]->mod1);
	if (mod1 != NULL)
	{
		buttons[index]->mod1 = (char *) malloc((strlen(mod1)+1) * sizeof(char));
		strcpy(buttons[index]->mod1, mod1);
	}
	else
		buttons[index]->mod1 = NULL;
	return 1;
}

int buttons_set_mod2(int index, const char *mod2)
{
	if (buttons_check_index(index) < 0)
		return -1;
	if (buttons[index]->mod2 != NULL)
		free(buttons[index]->mod2);
	if (mod2 != NULL)
	{
		buttons[index]->mod2 = (char *) malloc((strlen(mod2)+1) * sizeof(char));
		strcpy(buttons[index]->mod2, mod2);
	}
	else
		buttons[index]->mod2 = NULL;
	return 1;
}

int buttons_set_mod3(int index, const char *mod3)
{
	if (buttons_check_index(index) < 0)
		return -1;
	if (buttons[index]->mod3 != NULL)
		free(buttons[index]->mod3);
	if (mod3 != NULL)
	{
		buttons[index]->mod3 = (char *) malloc((strlen(mod3)+1) * sizeof(char));
		strcpy(buttons[index]->mod3, mod3);
	}
	else
		buttons[index]->mod3 = NULL;
	return 1;
}

int buttons_set_uid(int index, int uid)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->uid = uid;
	return uid;
}

int buttons_set_new(int index, int new)
{
	if (buttons_check_index(index) < 0)
		return -1;
	buttons[index]->new = new;
	return new;
}

int buttons_set_command(int index, const char *command)
{
	if (buttons_check_index(index) < 0)
		return -1;
	if (buttons[index]->command != NULL)
		free(buttons[index]->command);
	if (command != NULL)
	{
		buttons[index]->command = (char *) malloc((strlen(command)+1) * sizeof(char));
		strcpy(buttons[index]->command, command);
	}
	else
		buttons[index]->command = NULL;
	return 1;
}

void buttons_set_old_all(void)
{
	int i;
	for (i=0; i<num_buttons; i++)
	{
		buttons[i]->new = 0;
	}
}

