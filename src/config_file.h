
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef CONFIG_FILE_H_
#define CONFIG_FILE_H_

#ifdef DEBUG
#define CONFIG_SAVE_NAME		"btnx_config_test"
#else
#define CONFIG_SAVE_NAME		"btnx_config"
#endif
#ifdef DEBUG
#define CONFIG_NAME				CONFIG_SAVE_NAME
#else
#define CONFIG_NAME				CONFIG_SAVE_NAME
#endif

#define CONFIG_PATH				"/etc/btnx"
#define CONFIG_FILE_PREPEND		CONFIG_PATH "/" CONFIG_NAME "_"
#define EVENTS_NAME				"events"
#define SETTINGS_FILE			CONFIG_PATH "/btnx_settings"

int config_file_parse(const char *name);
char *config_file_add_value(int button, int block, char *option, char *value);
int config_file_save(const char *name);
int config_file_delete(char *name);
int settings_file_parse(void);
int settings_file_save(void);

#endif /*CONFIG_FILE_H_*/
