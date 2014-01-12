
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef CONFIG_H_
#define CONFIG_H_

#define EVTYPE_KEY			0
#define EVTYPE_CONFIG		1
#define EVTYPE_COMMAND		2
#define KEYCODE_NONE		"NONE"

enum
{
	CONFIG_SWITCH_NONE,
	CONFIG_SWITCH_NEXT,
	CONFIG_SWITCH_PREV,
	CONFIG_SWITCH_TO
};

enum
{
	SHOW_ERROR_UID=1,
	SHOW_ERROR_KEYCODE,
	SHOW_ERROR_DELAY,
	SHOW_ERROR_EVKEY,
	SHOW_ERROR_EVCOMMAND,
	SHOW_ERROR_ALL
};

int config_edit_rawcode(int rawcode);
int config_get_rawcode(void);
void config_refresh_mouse_properties(void);
void config_refresh_button_properties(void);
void config_refresh_all_fields(void);
int config_refresh_config_combobox(const gchar *name);
void config_update_config_combobox(void);
void config_update_mouse_properties(void);
void config_update_button_properties(int show_error);
void config_update_evtype(int evtype);
void config_refresh_advanced_settings(void);
void config_clear_all(void);
void config_delete_selected_button(void);
int config_save(void);

#endif /*CONFIG_H_*/
