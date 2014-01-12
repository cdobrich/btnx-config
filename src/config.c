
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/* Functionality for button configuration and the main configuration window */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "config.h"
#include "widgets.h"
#include "handlers.h"
#include "tree_buttons.h"
#include "buttons.h"
#include "keycodes.h"
#include "mouse.h"
#include "text.h"
#include "config_file.h"
#include "keycodes_entry.h"
#include "callbacks.h"
#include "revoco.h"
#include "config_manager.h"
#include "common.h"
#include "udev.h"

static int edit_rawcode=0;

int config_edit_rawcode(int rawcode)
{
	if (rawcode == edit_rawcode)
		return 0;
	if (buttons_exists(rawcode) >= 0)
		edit_rawcode = rawcode;
	else
	{
		fprintf(stderr,
		"Warning: config_edit_rawcode cannot find button with rawcode 0x%08x\n",
		 rawcode);
		return 0;
	}
	
	return rawcode;
}

int config_get_rawcode(void)
{ return edit_rawcode; }

void config_refresh_mouse_properties(void)
{
	int vendor_id, product_id;
	gchar tmp_str[16], *detected_buttons, *detected_devices;
	vendor_id = mouse_get_vendor_id();
	g_snprintf(tmp_str, 16, "0x%04x", vendor_id);
	gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_VENDORIDVAL_CONFIG)),
						tmp_str);
							
	product_id = mouse_get_product_id();
	g_snprintf(tmp_str, 16, "0x%04x", product_id);
	gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_PRODUCTIDVAL_CONFIG)),
						tmp_str);
						
	if (mouse_get_vendor_name() == NULL)
		gtk_entry_set_text(	GTK_ENTRY(GET_WIDGET(ENTRY_VENDOR_CONFIG)), "");
	else
	{
		gtk_entry_set_text(	GTK_ENTRY(GET_WIDGET(ENTRY_VENDOR_CONFIG)),
							mouse_get_vendor_name());
	}
	if (mouse_get_product_name() == NULL)
		gtk_entry_set_text(	GTK_ENTRY(GET_WIDGET(ENTRY_PRODUCT_CONFIG)), "");
	else
	{
		gtk_entry_set_text(	GTK_ENTRY(GET_WIDGET(ENTRY_PRODUCT_CONFIG)),
							mouse_get_product_name());
	}
	
	if (vendor_id != 0 && product_id != 0)
		detected_devices = g_strdup_printf("%s %d", TEXT_DETECTED_DEVICES, 1);
	else
		detected_devices = g_strdup_printf("%s %d", TEXT_DETECTED_DEVICES, 0);
	detected_buttons = g_strdup_printf("%s %d", TEXT_DETECTED_BUTTONS, buttons_count());
	
	SET_LABEL_TEXT(LABEL_DEVICES_CONFIG, detected_devices);
	SET_LABEL_TEXT(LABEL_BUTTONS_CONFIG, detected_buttons);
							
	g_free(detected_devices);
	g_free(detected_buttons);
}

void config_refresh_button_properties(void)
{
	int	button, enabled, delay, evtype, uid, mod1_index, 
		mod2_index, mod3_index, switch_type, force_release; // keycode_index,
	const gchar	*name, *keycode, *mod1, *mod2, *mod3, *command, *switch_name;
	gchar *delay_str, *uid_str;
	
	if ((button = buttons_exists(edit_rawcode)) < 0)
		return;
	
	enabled = buttons_get_enabled	(button);
	delay 	= buttons_get_delay		(button);
	evtype	= buttons_get_evtype	(button);
	uid 	= buttons_get_uid		(button);
	name	= buttons_get_name		(button);
	keycode	= buttons_get_keycode	(button);
	mod1	= buttons_get_mod1		(button);
	mod2	= buttons_get_mod2		(button);
	mod3	= buttons_get_mod3		(button);
	command = buttons_get_command	(button);
	switch_type 	= buttons_get_switch_type	(button);
	switch_name 	= buttons_get_switch_name	(button);
	force_release 	= buttons_get_force_release	(button);
	
	delay_str	= g_strdup_printf("%d", delay);
	uid_str 	= g_strdup_printf("%d", uid);
	
	config_update_evtype(evtype);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(CB_ENABLED_CONFIG)), (gboolean)enabled);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(CB_FORCE_RELEASE_CONFIG)), (gboolean)force_release);
	switch (switch_type)
	{
	case CONFIG_SWITCH_NONE:
	case CONFIG_SWITCH_NEXT:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_SWITCH_NEXT_CONFIG)), TRUE);
		break;
	case CONFIG_SWITCH_PREV:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_SWITCH_PREV_CONFIG)), TRUE);
		break;
	case CONFIG_SWITCH_TO:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_SWITCH_TO_CONFIG)), TRUE);
		break;
	}
	if (name != NULL)
		gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_BUTTON_NAME_CONFIG)), name);
	else
		gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_BUTTON_NAME_CONFIG)), "");
	if (command != NULL)
		gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_COMMAND_CONFIG)), command);
	else
		gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_COMMAND_CONFIG)), "");
	if (delay_str != NULL)
		gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_DELAY_CONFIG)), delay_str);
	else
		gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_DELAY_CONFIG)), "");
	if (uid_str != NULL)
		gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_UID_CONFIG)), uid_str);
	else
		gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_UID_CONFIG)), "");
	if (keycode == NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(GET_KEYCODE_ENTRY()), "NONE");
		//gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG)), 0);
	}
	else
	{
		gtk_entry_set_text(GTK_ENTRY(GET_KEYCODE_ENTRY()), keycode);
		//keycode_index = keycodes_get_index(keycode);
		//gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG)), keycode_index);
	}
	if (mod1 == NULL)
		gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD1_CONFIG)), 0);
	else
	{
		mod1_index = mods_get_index(mod1);
		gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD1_CONFIG)), mod1_index);
	}
	if (mod2 == NULL)
		gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD2_CONFIG)), 0);
	else
	{
		mod2_index = mods_get_index(mod2);
		gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD2_CONFIG)), mod2_index);
	}
	if (mod3 == NULL)
		gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD3_CONFIG)), 0);
	else
	{
		mod3_index = mods_get_index(mod3);
		gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD3_CONFIG)), mod3_index);
	}
	if (evtype != -1)
		gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_EVTYPE_CONFIG)), evtype);
	
	if (config_refresh_config_combobox(switch_name) < 0)
		buttons_set_switch_name(button, "NONE");
	
	gtk_widget_set_sensitive(GET_WIDGET(BTN_DELETE_BUTTON_CONFIG), TRUE);
	
	g_free(delay_str);
	g_free(uid_str);
}

void config_refresh_all_fields(void)
{
	config_refresh_mouse_properties();
	config_refresh_button_properties();
	tree_buttons_replace();
	revoco_refresh_properties();
}

static int config_check_integer(const gchar *str)
{
	int i, len;
	len = strlen(str);
	for (i=0; i<len; i++)
	{
		if (!g_ascii_isdigit(str[i]))
			return -1;
	}
	return 1;
}

void config_update_mouse_properties(void)
{
	const gchar *vendor, *product;
	
	vendor = gtk_entry_get_text(GTK_ENTRY(GET_WIDGET(ENTRY_VENDOR_CONFIG)));
	product = gtk_entry_get_text(GTK_ENTRY(GET_WIDGET(ENTRY_PRODUCT_CONFIG)));
	
	mouse_set_vendor_name(vendor);
	mouse_set_product_name(product);
}

void config_update_button_properties(int show_error)
{
	int button;
	int enabled, release, delay_val, uid_val, evtype, switch_type;
	const gchar *name, *delay, *uid, *command;
	gchar *mod1, *mod2, *mod3, *uid_str, *delay_str, *keycode, *switch_name;
	GtkWidget *dialog;
	
	if (edit_rawcode == 0)
		return;
	if ((button = buttons_exists(edit_rawcode)) < 0)
		return;
	
	enabled = 	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GET_WIDGET(CB_ENABLED_CONFIG)));
	release =	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GET_WIDGET(CB_FORCE_RELEASE_CONFIG)));
	name 	= 	gtk_entry_get_text(GTK_ENTRY(GET_WIDGET(ENTRY_BUTTON_NAME_CONFIG)));
	delay 	= 	gtk_entry_get_text(GTK_ENTRY(GET_WIDGET(ENTRY_DELAY_CONFIG)));
	command = 	gtk_entry_get_text(GTK_ENTRY(GET_WIDGET(ENTRY_COMMAND_CONFIG)));
	uid 	= 	gtk_entry_get_text(GTK_ENTRY(GET_WIDGET(ENTRY_UID_CONFIG)));
	keycode = 	g_strdup(gtk_entry_get_text(GTK_ENTRY(GET_KEYCODE_ENTRY())));
	//gtk_combo_box_get_active_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG)));
	mod1 	= 	gtk_combo_box_get_active_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD1_CONFIG)));
	mod2 	= 	gtk_combo_box_get_active_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD2_CONFIG)));
	mod3 	= 	gtk_combo_box_get_active_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD3_CONFIG)));
	switch_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_SWITCH_TO_CONFIG)));
	evtype	=	gtk_combo_box_get_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_EVTYPE_CONFIG)));
	
	uid_str		= g_strdup(uid);
	delay_str	= g_strdup(delay);
	g_strstrip(delay_str);
	g_strstrip(uid_str);
	if (config_check_integer(delay_str) < 0)
	{
		if (show_error == SHOW_ERROR_DELAY)
		{
			SHOW_ERROR_DIALOG(	dialog, GET_WIDGET(WIN_CONFIG),
								TEXT_INVALID_VALUE,
			/* TRANSLATORS: the "delay" refers to an interval of time that
			 * must pass before something else happens. The whole number refers
			 * to an integer, meaning a number that does not have decimal
			 * places. For example, 1.5 or 99,90 are invalid values */
								_("Delay must be a whole number"));
			gtk_window_set_focus(GTK_WINDOW(GET_WIDGET(WIN_CONFIG)), GET_WIDGET(ENTRY_DELAY_CONFIG));
			gtk_editable_select_region(GTK_EDITABLE(GET_WIDGET(ENTRY_DELAY_CONFIG)), 0, -1);
		}
		fprintf(stderr, "Delay is not a valid number.\n");
		goto done;
	}
	if (config_check_integer(uid_str) < 0)
	{
		if (show_error == SHOW_ERROR_UID)
		{
			SHOW_ERROR_DIALOG(	dialog, GET_WIDGET(WIN_CONFIG),
								TEXT_INVALID_VALUE,
			/* TRANSLATORS: this informs the user that they tried to set the
			 * value of UID to something other than a number. The UID refers
			 * to User Identification, a number referring to a user in a 
			 * Linux system. Do not try to translate UID unless it is explicitly
			 * used differently by Linux in your language. */
								_("UID must be a number"));
			gtk_window_set_focus(GTK_WINDOW(GET_WIDGET(WIN_CONFIG)), GET_WIDGET(ENTRY_UID_CONFIG));
			gtk_editable_select_region(GTK_EDITABLE(GET_WIDGET(ENTRY_UID_CONFIG)), 0, -1);
		}
		fprintf(stderr, "UID is not a valid number.\n");
		goto done;
	}
	delay_val	= strtol(delay_str, NULL, 10);
	uid_val 	= strtol(uid_str, NULL, 10);
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_SWITCH_NEXT_CONFIG))) == TRUE)
		switch_type = CONFIG_SWITCH_NEXT;
	else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_SWITCH_PREV_CONFIG))) == TRUE)
		switch_type = CONFIG_SWITCH_PREV;
	else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_SWITCH_TO_CONFIG))) == TRUE)
		switch_type = CONFIG_SWITCH_TO;
	else
		switch_type = CONFIG_SWITCH_NEXT;
	
	if (strcmp(keycode, "") == 0 || keycode == NULL)
	{
		if (keycode != NULL)
			g_free(keycode);
		keycode = g_strdup("NONE");
		gtk_entry_set_text(GTK_ENTRY(GET_KEYCODE_ENTRY()), keycode);
	}
	if (keycodes_get_index(keycode) < 0)
	{
		if (show_error == SHOW_ERROR_KEYCODE)
		{
			SHOW_ERROR_DIALOG(	dialog, GET_WIDGET(WIN_CONFIG),
								TEXT_INVALID_VALUE,
			/* TRANSLATORS: keycode refers to the name of a button (keyboard
			 * or mouse button) that is used internally by the Linux kernel. 
			 * Examples: KEY_PAGEUP refers to the "Page Up" key on a keyboard.
			 * BTN_LEFT refers to the left mouse button. Make sure this is
			 * translated exactly the same as other references to "keycode" in
			 * this program. */
								_("No keycode by that name exists."));
			gtk_window_set_focus(GTK_WINDOW(GET_WIDGET(WIN_CONFIG)), GET_KEYCODE_ENTRY());
			gtk_editable_select_region(GTK_EDITABLE(GET_KEYCODE_ENTRY()), 0, -1);
		}
		fprintf(stderr, "Error: no keycode by that name exists.\n");
		goto done;
	}
	else
	{
		
	}
	
	buttons_set_enabled	(button, enabled);
	buttons_set_force_release(button, release);
	buttons_set_name	(button, name);
	buttons_set_delay	(button, delay_val);
	buttons_set_evtype	(button, evtype);
	buttons_set_command	(button, command);
	buttons_set_uid		(button, uid_val);
	buttons_set_keycode	(button, keycode);
	buttons_set_mod1	(button, mod1);
	buttons_set_mod2	(button, mod2);
	buttons_set_mod3	(button, mod3);
	buttons_set_switch_type(button, switch_type);
	buttons_set_switch_name(button, switch_name);
	
	tree_buttons_update_button(edit_rawcode);
	
done:	
	g_free(keycode);
	g_free(mod1);
	g_free(mod2);
	g_free(mod3);
	g_free(uid_str);
	g_free(delay_str);
	g_free(switch_name);
}

int config_refresh_config_combobox(const gchar *name)
{
	gchar *tmp;
	GtkComboBox *cbox;
	GtkTreeModel *model;
	//GtkListStore *store;
	GtkTreeIter iter;
	
	if (name == NULL)
		return -1;
	
	cbox = GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_SWITCH_TO_CONFIG));
	model = gtk_combo_box_get_model(cbox);
	//store = GTK_LIST_STORE(gtk_combo_box_get_model(cbox));
	
	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
		return -1;
	
	do
	{
		gtk_tree_model_get(	model, &iter,
							0, &tmp,
							-1);
		if (tmp != NULL)
		{
			if (strcmp(tmp, name) == 0)
			{
				gtk_combo_box_set_active_iter(cbox, &iter);
				g_free(tmp);
				return 0;
			}
		}
		g_free(tmp);
	} while (gtk_tree_model_iter_next(model, &iter) == TRUE);

	gtk_combo_box_set_active(cbox, 0);
	return -1;
}

void config_update_config_combobox(void)
{
	GtkListStore *store;
	GtkComboBox *cbox;
	int x=0;
	const gchar *tmp=NULL;
	
	cbox = GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_SWITCH_TO_CONFIG));
	store = GTK_LIST_STORE(gtk_combo_box_get_model(cbox));
	
	gtk_list_store_clear(store);
	
	gtk_combo_box_append_text(cbox, "NONE");
	while ((tmp = config_manager_get_name(x)) != NULL)
	{
		gtk_combo_box_append_text(cbox, tmp);
		x++;
	}
}

void config_update_evtype(int evtype)
{
	gboolean evkey=TRUE, evconfig=FALSE, evcommand=FALSE, global=TRUE;
	if (evtype == EVTYPE_KEY)
	{
		evkey = TRUE;
		evconfig = FALSE;
		evcommand = FALSE;
	}
	else if (evtype == EVTYPE_CONFIG)
	{
		evkey = FALSE;
		evconfig = TRUE;
		evcommand=FALSE;
	}
	else if (evtype == EVTYPE_COMMAND)
	{
		evkey = FALSE;
		evconfig = FALSE;
		evcommand = TRUE;
	}
	else
	{
		evkey = FALSE;
		evconfig = FALSE;
		evcommand = FALSE;
		global = FALSE;
	}
	gtk_widget_set_sensitive(	GET_WIDGET(CB_ENABLED_CONFIG),
								global);
	gtk_widget_set_sensitive(	GET_WIDGET(ENTRY_BUTTON_NAME_CONFIG),
								global);
	gtk_widget_set_sensitive(	GET_WIDGET(COMBOBOX_EVTYPE_CONFIG),
								global);
	gtk_widget_set_sensitive(	GET_WIDGET(ENTRY_DELAY_CONFIG),
								global);
	gtk_widget_set_sensitive(	GET_WIDGET(CB_FORCE_RELEASE_CONFIG),
								global);
	
	gtk_widget_set_sensitive(	GET_WIDGET(COMBOBOX_KEYCODE_CONFIG),
								evkey);
	gtk_widget_set_sensitive(	GET_KEYCODE_ENTRY(),
								evkey);
	gtk_widget_set_sensitive(	GET_WIDGET(COMBOBOX_MOD1_CONFIG),
								evkey);
	gtk_widget_set_sensitive(	GET_WIDGET(COMBOBOX_MOD2_CONFIG),
								evkey);
	gtk_widget_set_sensitive(	GET_WIDGET(COMBOBOX_MOD3_CONFIG),
								evkey);
	
	gtk_widget_set_sensitive(	GET_WIDGET(RADIO_SWITCH_NEXT_CONFIG),
								evconfig);
	gtk_widget_set_sensitive(	GET_WIDGET(RADIO_SWITCH_PREV_CONFIG),
								evconfig);
	gtk_widget_set_sensitive(	GET_WIDGET(RADIO_SWITCH_TO_CONFIG),
								evconfig);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_SWITCH_TO_CONFIG))) == TRUE
		&& evconfig)
	{
		gtk_widget_set_sensitive(	GET_WIDGET(COMBOBOX_SWITCH_TO_CONFIG),
									TRUE);
	}
	else
	{
		gtk_widget_set_sensitive(	GET_WIDGET(COMBOBOX_SWITCH_TO_CONFIG),
									FALSE);
	}
	
	gtk_widget_set_sensitive(	GET_WIDGET(ENTRY_COMMAND_CONFIG),
								evcommand);
	gtk_widget_set_sensitive(	GET_WIDGET(ENTRY_UID_CONFIG),
								evcommand);
}

void config_refresh_advanced_settings(void)
{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(CB_UDEV_ENABLE_CONFIG)), udev_enabled_get());
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(CB_SYSFS_CONFIG)), udev_sysfs_enabled_get());
	if (udev_enabled_get() == 0)
		gtk_widget_set_sensitive(GET_WIDGET(CB_SYSFS_CONFIG), FALSE);
	else
		gtk_widget_set_sensitive(GET_WIDGET(CB_SYSFS_CONFIG), TRUE);
}

void config_clear_all(void)
{
	//config_manager_set_changed(1);
	
	buttons_free();
	handlers_free();
	mouse_free();
	
	revoco_init();
	config_refresh_all_fields();
}

void config_delete_selected_button(void)
{
	if (edit_rawcode == 0)
	{
		fprintf(stderr,"Error: no selected button\n");
		return;
	}
	
	set_block_update(1);
	
	config_manager_set_changed(1);
	
	buttons_remove(buttons_exists(edit_rawcode));
	tree_buttons_remove_button();
	
	if (buttons_count() == 0)
	{
		edit_rawcode = 0;
		init_config_button_field();
	}
	else
	{
		edit_rawcode = tree_buttons_get_selected_button_rawcode();
		config_refresh_button_properties();
	}
	config_refresh_mouse_properties();
	set_block_update(0);
}


int config_save(void)
{
	GtkWidget *dialog;
	const char *config_name=NULL;
	
	/*if (mouse_get_vendor_id() == 0 && mouse_get_product_id() == 0)
	{
		SHOW_WARNING_DIALOG(dialog, GET_WIDGET(WIN_CONFIG),
_("Cannot save the configuration"),*/
/* TRANSLATORS: device IDs are numbers that are unique to input devices. For
 * example, a certain mouse might have ID (Identification) values of 046d and 
 * C51A (these are hexadecimal values). The "detect" word refers to the
 * detection process that is part of the program. */
/*_("No valid device IDs are set. You must first detect your mouse before saving \
the configuration to a file."));
		return -1;
	}*/
	if ((config_name = config_manager_get_current_name()) == NULL)
	{
		fprintf(stderr, "Warning: config_save failed, no current config.\n");
		return -1;
	}
	if (config_file_save(config_name) < 0)
	{
		SHOW_ERROR_DIALOG(dialog, GET_WIDGET(WIN_CONFIG),
			_("Configuration file save error"),
			_("An error occurred during saving the configuration."));
		return -1;
	}
	config_manager_set_changed(0);
	config_manager_save_file();
	return 0;
}

