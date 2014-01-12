
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/* Functions for parsing and saving configuration files */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "buttons.h"
#include "mouse.h"
#include "config.h"
#include "config_file.h"
#include "common.h"
#include "udev.h"

#define CONFIG_MOUSE_BEGIN		"Mouse"
#define CONFIG_MOUSE_END		"EndMouse"
#define CONFIG_BUTTON_BEGIN		"Button"
#define CONFIG_BUTTON_END		"EndButton"

#define CONFIG_SAVE_ERROR		"Error: config_file_save failed to write to the configuration file: %s\n"

#define CONFIG_PARSE_BUFFER_SIZE			512
#define CONFIG_PARSE_OPTION_SIZE			64
#define CONFIG_PARSE_VALUE_SIZE				512
#define SETTINGS_PARSE_BUFFER_SIZE			CONFIG_PARSE_BUFFER_SIZE

#define IS_ENCLOSING(c) ((c) == '\'' || (c) == '"' || (c) == '`')

enum
{
	BLOCK_NONE,
	BLOCK_MOUSE,
	BLOCK_BUTTON
};

int config_file_parse(const char *name)
{
	FILE *fp;
	char buffer[CONFIG_PARSE_BUFFER_SIZE];
	char option[CONFIG_PARSE_OPTION_SIZE];
	char value[CONFIG_PARSE_VALUE_SIZE];
	char *loc_eq, *loc_com, *loc_beg, *loc_end;
	int block_begin = 0, block_end = 1;
	int i=-1;
	int block_type=BLOCK_NONE;
	
	if (name == NULL)
		sprintf(buffer,"%s/%s", CONFIG_PATH, CONFIG_NAME);
	else
		sprintf(buffer,"%s%s", CONFIG_FILE_PREPEND, name);
	
	if (!(fp = fopen(buffer,"r")))
	{
		fprintf(stderr, "No previous config file at %s. No configuration loaded.\n", buffer);
		return -1;
	}
	
	while (fgets(buffer, 511, fp) != NULL)
	{
		if (buffer[0] != '#')
		{
			loc_eq = strchr(buffer, '=');
			loc_com = strchr(buffer,'#');
			loc_beg = buffer;
			while (isspace(*loc_beg)) loc_beg++;
			
			if (loc_eq && (loc_com == NULL || loc_com > loc_eq) && block_type != BLOCK_NONE)
			{
				if (loc_com != NULL && loc_com != loc_eq + 1)
					*loc_com = '\0';
				
				loc_beg = buffer;
				while (isspace(*loc_beg)) loc_beg++;
				loc_end = loc_eq;
				while (isspace(*(loc_end-1))) loc_end--;
				*loc_end = '\0';
				strcpy(option, loc_beg);
				
				loc_beg = loc_eq;
				while (isspace(*(loc_beg+1))) loc_beg++;
				loc_beg++;
				loc_end = loc_beg + strlen(loc_beg);
				while (isspace(*(loc_end-1))) loc_end--;
				*loc_end = '\0';		
				strcpy(value, loc_beg);
				
				if (!config_file_add_value(i, block_type, option, value))
					perror("Warning: parse error");
				
				memset(value, '\0', CONFIG_PARSE_VALUE_SIZE * sizeof(char));
				memset(option, '\0', CONFIG_PARSE_OPTION_SIZE * sizeof(char));
				memset(buffer, '\0', CONFIG_PARSE_BUFFER_SIZE * sizeof(char));
			}
			else if (*loc_beg == '\0')
				continue;
			else
			{
				loc_end = loc_beg;
				
				if (loc_com == NULL)
					loc_com = &buffer[strlen(buffer)-1];
				
				while (!isspace(*loc_end) && loc_end < loc_com && *loc_end != '\0') loc_end++;
				*loc_end = '\0';
				
				if (strcmp(loc_beg, CONFIG_BUTTON_BEGIN) == 0)
				{
					if (block_end == 0)
					{
						fprintf(stderr, "Warning: config file parse error\n");
						continue;
					}
					block_begin = 1;
					block_type = BLOCK_BUTTON;
					i++;
					buttons_add(i);
				}
				else if (strcmp(loc_beg, CONFIG_BUTTON_END) == 0)
				{
					if (block_begin == 0)
					{
						fprintf(stderr, "Warning: config file parse error\n");
						continue;
					}
					block_end = 1;
					block_type = BLOCK_NONE;
				}
				if (strcmp(loc_beg, CONFIG_MOUSE_BEGIN) == 0)
				{
					if (block_end == 0)
					{
						fprintf(stderr, "Warning: config file parse error\n");
						continue;
					}
					block_begin = 1;
					block_type = BLOCK_MOUSE;
				}
				else if (strcmp(loc_beg, CONFIG_MOUSE_END) == 0)
				{
					if (block_begin == 0)
					{
						fprintf(stderr, "Warning: config file parse error\n");
						continue;
					}
					block_end = 1;
					block_type = BLOCK_NONE;
				}
			}
		}
	}
	
	fclose(fp);
	
	return 1;
}

char *config_file_add_value(int button, int block, char *option, char *value)
{
	if (block == BLOCK_BUTTON)
	{
		if (button < 0)
		{
			fprintf(stderr, "Error: config_file_add_value incorrect button index %d.\n", button);
			return NULL;
		}
		if (!strcasecmp(option, "rawcode"))
		{
			buttons_set_rawcode(button, strtol(value, NULL, 16));
			return option;
		}
		if (!strcasecmp(option, "enabled"))
		{
			buttons_set_enabled(button, strtol(value, NULL, 10));
			return option;
		}
		if (!strcasecmp(option, "type"))
		{
			buttons_set_type(button, strtol(value, NULL, 10));
			return option;
		}
		if (!strcasecmp(option, "delay"))
		{
			buttons_set_delay(button, strtol(value, NULL, 10));
			return option;
		}
		if (!strcasecmp(option, "keycode"))
		{
			buttons_set_evtype(button, EVTYPE_KEY);
			buttons_set_keycode(button, value);
			return option;
		}
		if (!strcasecmp(option, "mod1"))
		{
			buttons_set_mod1(button, value);
			return option;
		}
		if (!strcasecmp(option, "mod2"))
		{
			buttons_set_mod2(button, value);
			return option;
		}
		if (!strcasecmp(option, "mod3"))
		{
			buttons_set_mod3(button, value);
			return option;
		}
		if (!strcasecmp(option, "command"))
		{
			buttons_set_evtype(button, EVTYPE_COMMAND);
			buttons_set_command(button, value);
			return option;
		}
		if (!strcasecmp(option, "uid"))
		{
			buttons_set_uid(button, strtol(value, NULL, 10));
			return option;
		}
		if (!strcasecmp(option, "name"))
		{
			buttons_set_name(button, value);
			return option;
		}
		if (!strcasecmp(option, "switch_type"))
		{
			buttons_set_switch_type(button, strtol(value, NULL, 10));
			return option;
		}
		if (!strcasecmp(option, "switch_name"))
		{
			buttons_set_evtype(button, EVTYPE_CONFIG);
			buttons_set_switch_name(button, value);
			return option;
		}
		if (!strcasecmp(option, "force_release"))
		{
			buttons_set_force_release(button, strtol(value, NULL, 10));
			return option;
		}
	}
	else if (block == BLOCK_MOUSE)
	{
		if (!strcasecmp(option, "vendor_name"))
		{
			mouse_set_vendor_name(value);
			return option;
		}
		if (!strcasecmp(option, "product_name"))
		{
			mouse_set_product_name(value);
			return option;
		}
		if (!strcasecmp(option, "vendor_id"))
		{
			mouse_set_vendor_id(strtol(value, NULL, 16));
			return option;
		}
		if (!strcasecmp(option, "product_id"))
		{
			mouse_set_product_id(strtol(value, NULL, 16));
			return option;
		}
		/* revoco settings */
		if (!strcasecmp(option, "revoco_mode"))
		{
			mouse_set_revoco_status(strtol(value, NULL, 10));
			return option;
		}
		if (!strcasecmp(option, "revoco_btn"))
		{
			mouse_set_revoco_btn(strtol(value, NULL, 10));
			return option;
		}
		if (!strcasecmp(option, "revoco_up_scroll"))
		{
			mouse_set_revoco_up_scroll(strtol(value, NULL, 10));
			return option;
		}
		if (!strcasecmp(option, "revoco_down_scroll"))
		{
			mouse_set_revoco_down_scroll(strtol(value, NULL, 10));
			return option;	
		}
	}
	else
		fprintf(stderr, "Warning: config_file_add_value invalid block type.\n");
	
	return NULL;
}

int config_file_save(const char *name)
{
	FILE *fp;
	char buffer[CONFIG_PARSE_BUFFER_SIZE];
	int i;
	
	if (name == NULL)
		sprintf(buffer,"%s/%s", CONFIG_PATH, CONFIG_SAVE_NAME);
	else
		sprintf(buffer,"%s%s", CONFIG_FILE_PREPEND, name);
	
	if (!(fp = fopen(buffer,"w")))
	{
		fprintf(stderr, "Error: config_file_save could not open the "
				"configuration file %s for saving: %s\n.\n", 
				buffer, strerror(errno));
		return -1;
	}
	
	if (g_fprintf(fp, "\n# This is a btnx configuration file generated by btnx-config\n\n") <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	if (g_fprintf(fp, "Mouse\n") <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	if (mouse_get_vendor_name() != NULL)
	{
		if (g_fprintf(fp, "vendor_name = %s\n", mouse_get_vendor_name()) <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	}
	if (mouse_get_product_name() != NULL)
	{
		if (g_fprintf(fp, "product_name = %s\n", mouse_get_product_name()) <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	}
	if (g_fprintf(fp, "vendor_id = 0x%04x\n", mouse_get_vendor_id()) <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	if (g_fprintf(fp, "product_id = 0x%04x\n", mouse_get_product_id()) <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	if (g_fprintf(fp, "revoco_mode = %d\n", mouse_get_revoco_status()) <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	if (g_fprintf(fp, "revoco_btn = %d\n", mouse_get_revoco_btn()) <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	if (g_fprintf(fp, "revoco_up_scroll = %d\n", mouse_get_revoco_up_scroll()) <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	if (g_fprintf(fp, "revoco_down_scroll = %d\n", mouse_get_revoco_down_scroll()) <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	if (g_fprintf(fp, "EndMouse\n\n") <= 0)
		fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	
	for (i=0; i<buttons_count(); i++)
	{
		// check for NULLs??
		
		if (g_fprintf(fp, "Button\n") <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		if (g_fprintf(fp, "name = %s\n", buttons_get_name(i)) <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		if (g_fprintf(fp, "rawcode = 0x%08x\n", buttons_get_rawcode(i)) <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		if (g_fprintf(fp, "enabled = %d\n", buttons_get_enabled(i)) <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		if (g_fprintf(fp, "type = %d\n", buttons_get_type(i)) <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		if (g_fprintf(fp, "delay = %d\n", buttons_get_delay(i)) <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		if (g_fprintf(fp, "force_release = %d\n", buttons_get_force_release(i)) <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		
		if (buttons_get_evtype(i) == EVTYPE_KEY)
		{
			if (g_fprintf(fp, "keycode = %s\n", buttons_get_keycode(i)) <= 0)
				fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
			if (g_fprintf(fp, "mod1 = %s\n", buttons_get_mod1(i)) <= 0)
				fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
			if (g_fprintf(fp, "mod2 = %s\n", buttons_get_mod2(i)) <= 0)
				fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
			if (g_fprintf(fp, "mod3 = %s\n", buttons_get_mod3(i)) <= 0)
				fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		}
		else if (buttons_get_evtype(i) == EVTYPE_COMMAND)
		{
			if (g_fprintf(fp, "command = %s\n", buttons_get_command(i)) <= 0)
				fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
			if (g_fprintf(fp, "uid = %d\n", buttons_get_uid(i)) <= 0)
				fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		}
		else if (buttons_get_evtype(i) == EVTYPE_CONFIG)
		{
			if (g_fprintf(fp, "switch_type = %d\n", buttons_get_switch_type(i)) <= 0)
				fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
			if (g_fprintf(fp, "switch_name = %s\n", buttons_get_switch_name(i)) <= 0)
				fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
		}
		else
		{
			fprintf(stderr, "Error: config_file_save encountered button with "
					"invalid event type: %d\n", buttons_get_evtype(i));
		}
		if (g_fprintf(fp, "EndButton\n\n") <= 0)
			fprintf(stderr, CONFIG_SAVE_ERROR, strerror(errno));
	}
	
	fclose(fp);
	return 1;
}

int config_file_delete(char *name)
{
	char *tmp_path;
	
	if (name == NULL)
		tmp_path = g_strdup(CONFIG_PATH "/" CONFIG_NAME);
	else
		tmp_path = g_strdup_printf("%s%s", CONFIG_FILE_PREPEND, name);
	
	if (unlink(tmp_path) < 0)
	{
		fprintf(stderr, "Warning: config_file_delete could not delete file %s: %s\n",
				tmp_path, strerror(errno));
		g_free(tmp_path);
		return -1;
	}
	
	g_free(tmp_path);
	return 0;
}

static int settings_add_value(const char *option, const char *value)
{
	if (strcmp(option, "udev_enable") == 0)
	{
		udev_enabled_set(strtol(value, NULL, 10));
		return 0;
	}
	if (strcmp(option, "wait_for_sysfs") == 0)
	{
		udev_sysfs_enabled_set(strtol(value, NULL, 10));
		return 0;
	}
	return -1;
}

int settings_file_parse(void)
{
	FILE *fp;
	char buffer[SETTINGS_PARSE_BUFFER_SIZE];
	gchar **split;
	
	if (!(fp = fopen(SETTINGS_FILE,"r")))
	{
		fprintf(stderr, "No settings file at " SETTINGS_FILE "\n");
		return -1;
	}
	
	while (fgets(buffer, SETTINGS_PARSE_BUFFER_SIZE - 1, fp) != NULL)
	{
		split = g_strsplit_set(buffer, "=\n", -1);
		
		if (g_strv_length(split) < 2)
			fprintf(stderr, "Warning: settings_file_parse() parse error.\n");
		else
		{
			split[0] = g_strstrip(split[0]);
			split[1] = g_strstrip(split[1]);
			if (settings_add_value(split[0], split[1]) < 0)
				fprintf(stderr, "Warning: settings_file_parse() parse error: Invalid option\n");
		}
		
		if (split != NULL)
			g_strfreev(split);
	}
	return 0;
}

static inline void settings_file_output_line(FILE *fp, char *buffer, const char *option, int value)
{
	g_sprintf(buffer, "%s = %d\n", option, value);
	if (g_fprintf(fp, "%s", buffer) <= 0)
	{
		fprintf(stderr, "Error: could not write to settings file: %s\n",
				strerror(errno));
	}
}

int settings_file_save(void)
{
	FILE *fp;
	char buffer[SETTINGS_PARSE_BUFFER_SIZE];
	
	if (!(fp = fopen(SETTINGS_FILE ,"w")))
	{
		fprintf(stderr, "Error: settings_file_save could not open the "
				"settings file " SETTINGS_FILE " for saving: %s\n.\n", 
				strerror(errno));
		return -1;
	}
	
	settings_file_output_line(fp, buffer, "udev_enable", udev_enabled_get());
	settings_file_output_line(fp, buffer, "wait_for_sysfs", udev_sysfs_enabled_get());
	
	fclose(fp);
	return 0;
}


