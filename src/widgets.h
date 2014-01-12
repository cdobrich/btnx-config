
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef WIDGETS_H_
#define WIDGETS_H_

#include <gtk/gtk.h>
#include <glade/glade.h>

#define GLADE_NAME		"btnx-config.glade"

#define WIN_CONFIG		"win_config"
#define WIN_MOUSED		"win_moused"
#define WIN_BTND		"win_btnd"
#define WIN_DIALOG_BTND	"win_dialog_btnd"
#define DIALOG_ABOUT	"dialog_about"
#define DIALOG_BTND		"dialog_btnd"

/* ---------------Main window global widgets---------------- */
#define LABEL_CONFIG_TAB_CONFIG		"label_config_tab_config"
#define LABEL_BUTTONS_TAB_CONFIG	"label_buttons_tab_config"
#define LABEL_REVOCO_TAB_CONFIG		"label_revoco_tab_config"
#define LABEL_ADVANCED_TAB_CONFIG	"label_advanced_tab_config"
#define BTN_CLOSE_CONFIG			"btn_close_config"
#define BTN_HELP_CONFIG				"btn_help_config"
#define BTN_ABOUT_CONFIG			"btn_about_config"
//#define STATUS_CONFIG				"status_config"

/* ---------------Configurations tab widgets---------------- */
#define TREE_CONFIGS				"tree_configs"
#define BTN_NEW_CONFIG				"btn_new_config"
#define BTN_DELETE_CONFIG			"btn_delete_config"
#define BTN_UP_CONFIG				"btn_up_config"
#define BTN_DOWN_CONFIG				"btn_down_config"
#define BTN_BACKUP_CONFIG			"btn_backup_config"
#define BTN_RESTORE_CONFIG			"btn_restore_config"
#define BTN_RESTART_BTNX_CONFIG		"btn_restart_btnx_config"
#define BTN_DETECT_CONFIG			"btn_detect_config"
#define ENTRY_CONFIG_NAME_CONFIG	"entry_config_name_config"
#define LABEL_FRAME_MANAGE_CONFIG	"label_frame_manage_config"
#define LABEL_FRAME_CURRENT_CONFIG	"label_frame_current_config"
#define LABEL_CONFIG_NAME_CONFIG	"label_config_name_config"
#define LABEL_DEVICES_CONFIG		"label_devices_config"
#define LABEL_BUTTONS_CONFIG		"label_buttons_config"
#define LABEL_VENDORIDVAL_CONFIG	"label_vendoridval_config"
#define LABEL_PRODUCTIDVAL_CONFIG	"label_productidval_config"
#define LABEL_FRAME_DETECT_CONFIG	"label_frame_detect_config"
#define LABEL_FRAME_MOUSE_CONFIG	"label_frame_mouse_config"
#define LABEL_VENDOR_CONFIG			"label_vendor_config"
#define LABEL_PRODUCT_CONFIG		"label_product_config"
#define LABEL_VENDORID_CONFIG		"label_vendorid_config"
#define LABEL_PRODUCTID_CONFIG		"label_productid_config"

/* ---------------Buttons tab widgets---------------- */
//#define BTN_SAVE_CONFIG				"btn_save_config"
#define LABEL_FRAME_GENERAL_CONFIG	"label_frame_general_config"
#define LABEL_FRAME_KEYCOMBO_CONFIG	"label_frame_keycombo_config"
#define LABEL_FRAME_CONFIG_SWITCH_CONFIG "label_frame_config_switch_config"
#define LABEL_FRAME_EXEC_CONFIG		"label_frame_exec_config"
#define BTN_DELETE_BUTTON_CONFIG	"btn_delete_button_config"
//#define BTN_CLEAR_CONFIG			"btn_clear_config"
#define CB_ENABLED_CONFIG			"cb_enabled_config"
#define CB_FORCE_RELEASE_CONFIG		"cb_force_release_config"
#define LABEL_FRAME_BUTTON_CONFIG	"label_frame_button_config"
#define LABEL_FRAME_FILE_CONFIG		"label_frame_file_config"
#define LABEL_BUTTON_NAME_CONFIG	"label_button_name_config"
#define LABEL_EVTYPE_CONFIG			"label_evtype_config"
//#define LABEL_TYPE_CONFIG			"label_type_config"
#define LABEL_DELAY_CONFIG			"label_delay_config"
#define LABEL_KEYCODE_CONFIG		"label_keycode_config"
#define LABEL_MOD1_CONFIG			"label_mod1_config"
#define LABEL_MOD2_CONFIG			"label_mod2_config"
#define LABEL_MOD3_CONFIG			"label_mod3_config"
#define LABEL_COMMAND_CONFIG		"label_command_config"
#define LABEL_UID_CONFIG			"label_uid_config"
#define ENTRY_VENDOR_CONFIG			"entry_vendor_config"
#define	ENTRY_PRODUCT_CONFIG		"entry_product_config"
#define ENTRY_BUTTON_NAME_CONFIG	"entry_button_name_config"
//#define ENTRY_TYPE_CONFIG			"entry_type_config"
#define ENTRY_DELAY_CONFIG			"entry_delay_config"
#define ENTRY_COMMAND_CONFIG		"entry_command_config"
#define ENTRY_UID_CONFIG			"entry_uid_config"
#define COMBOBOX_EVTYPE_CONFIG		"combobox_evtype_config"
#define COMBOBOX_MOD1_CONFIG		"combobox_mod1_config"
#define COMBOBOX_MOD2_CONFIG		"combobox_mod2_config"
#define COMBOBOX_MOD3_CONFIG		"combobox_mod3_config"
#define COMBOBOX_KEYCODE_CONFIG		"combobox_keycode_config"
#define COMBOBOX_ENTRY_KEYCODE_CONFIG	"comboboxentry_keycode_config"
#define COMBOBOX_SWITCH_TO_CONFIG	"combobox_switch_to_config"
#define RADIO_SWITCH_NEXT_CONFIG	"radio_switch_next_config"
#define RADIO_SWITCH_PREV_CONFIG	"radio_switch_prev_config"
#define RADIO_SWITCH_TO_CONFIG		"radio_switch_to_config"
#define TREEVIEW_BUTTONS_CONFIG		"tree_config"

/* ---------------revoco tab widgets---------------- */
#define LABEL_REVOCO_STATUS_CONFIG	"label_revoco_status_config"
#define LABEL_FRAME_REVOCO_CONFIG	"label_frame_revoco_config"
#define LABEL_REVOCO_INFO_CONFIG	"label_revoco_info_config"
#define LABEL_REVOCO_BUTTON_CONFIG	"label_revoco_button_config"
#define LABEL_REVOCO_UP_CONFIG		"label_revoco_up_config"
#define LABEL_REVOCO_DOWN_CONFIG	"label_revoco_down_config"
#define CB_ENABLE_REVOCO_CONFIG		"cb_enable_revoco_config"
#define RADIO_REVOCO_FREE_CONFIG	"radio_revoco_free_config"
#define RADIO_REVOCO_CLICK_CONFIG	"radio_revoco_click_config"
#define RADIO_REVOCO_MANUAL_CONFIG	"radio_revoco_manual_config"
#define RADIO_REVOCO_AUTO_CONFIG	"radio_revoco_auto_config"
#define SPIN_REVOCO_BUTTON_CONFIG	"spin_revoco_button_config"
#define SPIN_REVOCO_UP_CONFIG		"spin_revoco_up_config"
#define SPIN_REVOCO_DOWN_CONFIG		"spin_revoco_down_config"

/* ---------------Advanced tab widgets---------------- */
#define CB_UDEV_ENABLE_CONFIG		"cb_udev_enable_config"
#define CB_SYSFS_CONFIG				"cb_sysfs_config"
#define LABEL_FRAME_ADVANCED_CONFIG	"label_frame_advanced_config"
#define LABEL_FRAME_HOTPLUGGING_CONFIG "label_frame_hotplugging_config"

/* ---------------Mouse detection window widgets---------------- */
#define LABEL_FRAME_INSTRUCTIONS_MOUSED "label_frame_instructions_moused"
#define LABEL_FRAME_STATUS_MOUSED	"label_frame_status_moused"
#define LABEL_VENDORID_MOUSED		"label_vendorid_moused"
#define LABEL_PRODUCTID_MOUSED		"label_productid_moused"
#define LABEL_HELP_MOUSED			"label_help_moused"
#define LABEL_HANDLERS_MOUSED		"label_handlers_moused"
#define LABEL_PRODUCTIDVAL_MOUSED	"label_productidval_moused"
#define LABEL_VENDORIDVAL_MOUSED	"label_vendoridval_moused"
#define BTN_CONTROL_MOUSED			"btn_control_moused"
#define BTN_CANCEL_MOUSED			"btn_cancel_moused"
#define BTN_FORWARD_MOUSED			"btn_forward_moused"
#define BTN_UNDO_MOUSED				"btn_undo_moused"
#define BTN_HELP_MOUSED				"btn_help_moused"

/* ---------------Button detection window widgets---------------- */
#define PROGRESSBAR_BTND			"progressbar_btnd"
#define LABEL_FRAME_INSTRUCTIONS_BTND "label_frame_instructions_btnd"
#define LABEL_FRAME_STATUS_BTND		"label_frame_status_btnd"
#define LABEL_STATUS_BTND			"label_status_btnd"
#define LABEL_HELP_BTND				"label_help_btnd"
#define LABEL_STATUS_BTND			"label_status_btnd"
#define LABEL_BTN_NAME_BTND			"label_btn_name_btnd"
#define ENTRY_NAME_BTND				"entry_name_btnd"
#define BTN_CONTROL_BTND			"btn_control_btnd"
#define BTN_ADD_BTND				"btn_add_btnd"
#define BTN_OK_BTND					"btn_ok_btnd"
#define BTN_HELP_BTND				"btn_help_btnd"
#define BTN_CANCEL_BTND				"btn_cancel_btnd"

#define TREEVIEW_DIALOG_BTND		"treeview_dialog_btnd"
#define LABEL_TITLE_DIALOG_BTND		"label_title_dialog_btnd"
#define LABEL_BODY_DIALOG_BTND		"label_body_dialog_btnd"


/* ---------------Widget macros---------------- */
#define GET_WIDGET(widget) glade_xml_get_widget((get_glade_xml()), (widget))
#define GET_LABEL(widget) GTK_LABEL(GET_WIDGET(widget))
#define GET_KEYCODE_ENTRY() keycodes_entry_get_entry()
#define SET_LABEL_TEXT(widget, text) gtk_label_set_text(GET_LABEL(widget), text)
#define SET_LABEL_MARKUP(widget, text) \
gtk_label_set_markup(GET_LABEL(widget), text)
#define SET_BUTTON_TEXT(widget, text) \
gtk_button_set_label(GTK_BUTTON(GET_WIDGET(widget)), text)
#define SET_TOOLTIP(tips, widget, text, detail) \
gtk_tooltips_set_tip(tips, GET_WIDGET(widget), text, detail)
#define SET_WIDGET_STATE(widget, enabled) \
gtk_widget_set_sensitive(GET_WIDGET(widget), enabled)

#define SHOW_WARNING_DIALOG(dialog, parent, title, body) \
{ \
	dialog = gtk_message_dialog_new(GTK_WINDOW(parent), \
									GTK_DIALOG_DESTROY_WITH_PARENT, \
									GTK_MESSAGE_WARNING, \
									GTK_BUTTONS_CLOSE, \
									title); \
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), \
										body); \
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent)); \
	gtk_dialog_run(GTK_DIALOG(dialog)); \
	gtk_widget_destroy(GTK_WIDGET(dialog)); \
}

#define SHOW_ERROR_DIALOG(dialog, parent, title, body) \
{ \
	dialog = gtk_message_dialog_new(GTK_WINDOW(parent), \
									GTK_DIALOG_DESTROY_WITH_PARENT, \
									GTK_MESSAGE_ERROR, \
									GTK_BUTTONS_CLOSE, \
									title); \
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), \
										body); \
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent)); \
	gtk_dialog_run(GTK_DIALOG(dialog)); \
	gtk_widget_destroy(GTK_WIDGET(dialog)); \
}

#define SHOW_CONFIRM_DIALOG(dialog, parent, confirm, title, body) \
{ \
	dialog = gtk_message_dialog_new(GTK_WINDOW(parent), \
									GTK_DIALOG_DESTROY_WITH_PARENT, \
									GTK_MESSAGE_QUESTION, \
									GTK_BUTTONS_YES_NO, \
									title); \
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), \
										body); \
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent)); \
	confirm = gtk_dialog_run(GTK_DIALOG(dialog)); \
	gtk_widget_destroy(GTK_WIDGET(dialog)); \
}

GladeXML *init_glade_xml(void);
GladeXML *get_glade_xml(void);
void free_glade_xml(void);

void widgets_center_on_parent(GtkWidget *win, GtkWidget *parent);

void init_main(void);

void init_config_button_field(void);
void init_config(void);
void init_evtype(void);

void init_moused(void);

void init_btnd(void);



#endif /*WIDGETS_H_*/
