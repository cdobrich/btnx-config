
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>

#include "widgets.h"
#include "keycodes_entry.h"

#define KEYCODE_FILE_PATH		"/etc/btnx/events"

static gchar **keycodes=NULL;
static gchar **mods=NULL;
static int num_keycodes=0;
static int num_mods=0;

/* 
 * MUST BE CALLED AFTER WIDGETS ARE INITIALIZED!!
 * Fills keycodes into comboboxes
 */
int keycodes_parse(void)
{
	FILE *fp;
	gchar buffer[512];
	gchar *code;
	int mod;
	
	if ((fp = fopen(KEYCODE_FILE_PATH, "r")) == 0)
	{
		fprintf(stderr, "Error: keycodes_parse could not open %s: %s\n",
				KEYCODE_FILE_PATH, g_strerror(errno));
		return -1;
	}
	
	mods = (gchar **) malloc(sizeof(gchar *));
	keycodes = (gchar **) malloc(sizeof(gchar *));
	mods[0] = g_strdup("NONE");
	keycodes[0] = g_strdup("NONE");
	keycodes_entry_add(keycodes[0]);
	
	num_mods++;
	num_keycodes++;
	
	while (fgets(buffer, 511, fp) != NULL)
	{
		mod = 0;
		g_strchug(buffer);
		if (strlen(buffer) == 0)
			continue;
		g_strdelimit(buffer, " \t", '\0');
		code = g_strndup(buffer, 60);
		
		/* -------- Add the keycodes straight to the comboboxes --------- */
		if (strcasecmp("KEY_LEFTCTRL", code) == 0) mod=1;
		else if (strcasecmp("KEY_RIGHTCTRL", code) == 0) mod=1;
		else if (strcasecmp("KEY_LEFTSHIFT", code) == 0) mod=1;
		else if (strcasecmp("KEY_RIGHTSHIFT", code) == 0) mod=1;
		else if (strcasecmp("KEY_LEFTALT", code) == 0) mod=1;
		else if (strcasecmp("KEY_RIGHTALT", code) == 0) mod=1;
		else if (strcasecmp("KEY_RIGHTCTRL", code) == 0) mod=1;
		else if (strcasecmp("KEY_LEFTMETA", code) == 0) mod=1;
		else if (strcasecmp("KEY_RIGHTMETA", code) == 0) mod=1;
		
		if (mod == 1)
		{
			gtk_combo_box_append_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD1_CONFIG)), code);
			gtk_combo_box_append_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD2_CONFIG)), code);
			gtk_combo_box_append_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD3_CONFIG)), code);
			mods = (gchar **) realloc(mods, (num_mods+1) * sizeof(gchar *));
			mods[num_mods++] = code;
		}
		else
		{
			keycodes_entry_add(code);
			//gtk_combo_box_append_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG)), code);
			keycodes = (gchar **) realloc(keycodes, (num_keycodes+1) * sizeof(gchar *));
			keycodes[num_keycodes++] = code;
		}
		/* ---------------------------------------------------------------*/
	}
	
	fclose(fp);
	return 1;
}

void keycodes_free(void)
{
	int i;
	if (keycodes != NULL)
	{
		for (i=0; i<num_keycodes; i++)
		{
			if (keycodes[i] != NULL)
				free(keycodes[i]);
		}
		free(keycodes);
	}
	keycodes = NULL;
	num_keycodes = 0;
	if (mods != NULL)
	{
		for (i=0; i<num_mods; i++)
		{
			if (mods[i] != NULL)
				free(mods[i]);
		}
		free(mods);
	}
	mods = NULL;
	num_mods = 0;
}

int keycodes_get_index(const gchar *keycode)
{
	int i;
	for (i=0; i<num_keycodes; i++)
	{
		if (strcasecmp(keycode, keycodes[i]) == 0)
			return i;
	}
	fprintf(stderr, "Warning: keycodes_get_index could not find keycode: %s\n", keycode);
	return -1;
}

int mods_get_index(const gchar *mod)
{
	int i;
	for (i=0; i<num_mods; i++)
	{
		if (strcasecmp(mod, mods[i]) == 0)
			return i;
	}
	fprintf(stderr, "Warning: mods_get_index could not find mod: %s\n", mod);
	return -1;
}

gchar *keycodes_get_keycode(int index)
{
	if (index < num_keycodes && index >= 0)
		return g_strdup(keycodes[index]);
	return NULL;
}

gchar *mods_get_keycode(int index)
{
	if (index < num_mods && index >= 0)
		return g_strdup(mods[index]);
	return NULL;
}
