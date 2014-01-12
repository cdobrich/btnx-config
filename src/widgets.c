
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "widgets.h"
#include "callbacks.h"
#include "tree_buttons.h"
#include "config.h"
#include "dialog_btnd.h"
#include "common.h"
#include "text.h"
#include "keycodes_entry.h"
#include "revoco.h"
#include "tree_config.h"

#ifndef DEBUG
#define PIXMAP_PATH		"/usr/share/pixmaps"
#define ICON_PATH		PIXMAP_PATH "/btnx.png"
#else
#define PIXMAP_PATH		"data/btnx.png"
#define ICON_PATH		"data/icons/btnx-72x72.png"
#endif

static GladeXML *glade_xml = NULL;

GladeXML *init_glade_xml(void)
{
	printf("%s\n", DATA_PATH "/" GLADE_NAME);
	glade_xml = glade_xml_new(DATA_PATH "/" GLADE_NAME, NULL, NULL);
	if (!glade_xml) return NULL;
	glade_xml_signal_autoconnect(glade_xml);
	
	return glade_xml;
}

GladeXML *get_glade_xml(void)
{
	return glade_xml;
}

void free_glade_xml(void)
{
	g_object_unref(glade_xml);
}

/* Used to center dialogs on top of their parent window */
void widgets_center_on_parent(GtkWidget *win, GtkWidget *parent)
{
	gint x1=0, x2=0, y1=0, y2=0;
	gint w1=0, w2=0, h1=0, h2=0;
	
	gtk_window_get_position(GTK_WINDOW(parent), &x2, &y2);
	gtk_window_get_size(GTK_WINDOW(parent), &w2, &h2);
	gtk_window_get_size(GTK_WINDOW(win), &w1, &h1);

	x1 = x2 + w2/2 - w1/2;
	y1 = y2 + h2/2 - h1/2;
	
	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;
	
	gtk_window_move(GTK_WINDOW(win), x1, y1);
	
}

static void init_static_texts(void)
{
	gchar *mod_text;
	
	/* -------------Main window global widgets-----------------*/
	SET_LABEL_TEXT(LABEL_CONFIG_TAB_CONFIG,		TEXT_CONFIGURATIONS);
	SET_LABEL_TEXT(LABEL_BUTTONS_TAB_CONFIG,	TEXT_BUTTONS);
	SET_LABEL_TEXT(LABEL_REVOCO_TAB_CONFIG,		"revoco");
	SET_LABEL_TEXT(LABEL_ADVANCED_TAB_CONFIG,	TEXT_ADVANCED);
	
	/* -------------Configurations tab widgets-----------------*/
	SET_LABEL_MARKUP(LABEL_FRAME_MANAGE_CONFIG, _("<b>Manage configurations</b>"));
	SET_LABEL_MARKUP(LABEL_FRAME_CURRENT_CONFIG,_("<b>Current configuration</b>"));
	SET_LABEL_MARKUP(LABEL_FRAME_DETECT_CONFIG,	_("<b>Device detection</b>"));
	SET_LABEL_MARKUP(LABEL_FRAME_MOUSE_CONFIG,	_("<b>Mouse properties</b>"));
	SET_BUTTON_TEXT(BTN_BACKUP_CONFIG,			_("Backup"));
	SET_BUTTON_TEXT(BTN_RESTORE_CONFIG,			_("Restore"));
	SET_LABEL_TEXT(LABEL_CONFIG_NAME_CONFIG,	_("Configuration name:"));
	/* TRANSLATORS: keep this short. It is the label of a button. */
	SET_BUTTON_TEXT(BTN_DETECT_CONFIG,			_("Detect mouse & buttons"));
	/* TRANSLATORS: Make this short. Abbreviate if necessary. It refers to the
	 * name of the manufacturer of a device. */
	SET_LABEL_TEXT(LABEL_VENDOR_CONFIG,			_("Vendor:"));
	/* TRANSLATORS: Make this short. Abbreviate if necessary. It refers to the
	 * name of a certain product (a mouse in this case, example: MX-600). */
	SET_LABEL_TEXT(LABEL_PRODUCT_CONFIG,		_("Product:"));
	SET_LABEL_TEXT(LABEL_VENDORID_CONFIG,		TEXT_VENDOR_ID);
	SET_LABEL_TEXT(LABEL_PRODUCTID_CONFIG,		TEXT_PRODUCT_ID);
	SET_BUTTON_TEXT(BTN_RESTART_BTNX_CONFIG,	TEXT_RESTART_BTNX);
	
	/* -------------Buttons tab widgets-----------------*/
	SET_LABEL_MARKUP(LABEL_FRAME_BUTTON_CONFIG,	_("<b>Button properties</b>"));
	SET_LABEL_MARKUP(LABEL_FRAME_GENERAL_CONFIG, _("<b>General</b>"));
	SET_LABEL_MARKUP(LABEL_FRAME_KEYCOMBO_CONFIG, _("<b>Key combination</b>"));
	SET_LABEL_MARKUP(LABEL_FRAME_CONFIG_SWITCH_CONFIG, _("<b>Configuration switch</b>"));
	SET_LABEL_MARKUP(LABEL_FRAME_EXEC_CONFIG,	_("<b>Command execution</b>"));
	/* TRANSLATORS: Enabled can be
	 * either true or false. In this case, it means whether the program should
	 * ignore a certain mouse button or do something when it is being pressed.
	 * It is used elsewhere in the program. Make sure you use the same
	 * translation here.*/
	SET_BUTTON_TEXT(CB_ENABLED_CONFIG,			_("Enabled"));
	/* TRANSLATORS: name refers to the name of button given by the user. It is
	 * _not_ the name of a person. */
	SET_LABEL_TEXT(LABEL_BUTTON_NAME_CONFIG,	_("Name:"));
	/* TRANSLATORS: This refers to the type of
	 * event that should be sent by the program when it recognizes it must do
	 * something. */
	SET_LABEL_TEXT(LABEL_EVTYPE_CONFIG,			_("Event type:"));
	/* The repeat delay refers to an interval of time during which the program
	 * should ignore any identical events. For example, in this case it
	 * means that if a person presses a button, the program does what the user
	 * wants. If they press the same button again before the repeat delay has
	 * expired, the program will ignore that button. */
	SET_LABEL_TEXT(LABEL_DELAY_CONFIG,			_("Repeat delay:"));
	SET_BUTTON_TEXT(CB_FORCE_RELEASE_CONFIG,	TEXT_FORCE_RELEASE);
	/* TRANSLATORS: This word is used
	 * elsewhere in the program. Make sure to use the same translation here.
	 * The keycode is a keyboard or button
	 * name. Example: KEY_PAGEUP for a "Page Up" key. */
	SET_LABEL_TEXT(LABEL_KEYCODE_CONFIG,		_("Keycode:"));
	mod_text = g_strdup_printf("%s %d:", TEXT_MODIFIER, 1);
	SET_LABEL_TEXT(LABEL_MOD1_CONFIG,			mod_text);
	g_free(mod_text);
	mod_text = g_strdup_printf("%s %d:", TEXT_MODIFIER, 2);
	SET_LABEL_TEXT(LABEL_MOD2_CONFIG,			mod_text);
	g_free(mod_text);
	mod_text = g_strdup_printf("%s %d:", TEXT_MODIFIER, 3);
	SET_LABEL_TEXT(LABEL_MOD3_CONFIG,			mod_text);
	g_free(mod_text);
	SET_BUTTON_TEXT(RADIO_SWITCH_NEXT_CONFIG,	_("Next config."));
	SET_BUTTON_TEXT(RADIO_SWITCH_PREV_CONFIG,	_("Previous config."));
	SET_BUTTON_TEXT(RADIO_SWITCH_TO_CONFIG,		_("Switch to:"));
	/* TRANSLATORS: this refers to the shell command that should be
	 * executed by the program. */
	SET_LABEL_TEXT(LABEL_COMMAND_CONFIG,		_("Execute command:"));
	/* TRANSLATORS: this refers to the UID that a certain launched process
	 * should use. It is used in conjuction with "Execute command".
	 * The UID refers
	 * to User Identification, a number referring to a user in a 
	 * Linux system. Do not try to translate UID unless it is explicitly used
	 * differently in your language. */
	SET_LABEL_TEXT(LABEL_UID_CONFIG,			_("Process UID:"));
	//SET_LABEL_TEXT(LABEL_FRAME_FILE_CONFIG, 	_("Configuration file"));
	
	
	
	/* -------------revoco tab widgets-----------------*/
	SET_LABEL_MARKUP(LABEL_REVOCO_STATUS_CONFIG,TEXT_REVOCO_STATUS_DISABLED);
	SET_LABEL_MARKUP(LABEL_FRAME_REVOCO_CONFIG,	TEXT_REVOCO_SETTINGS);
	SET_LABEL_TEXT(LABEL_REVOCO_INFO_CONFIG,	TEXT_REVOCO_INFO);
	SET_BUTTON_TEXT(CB_ENABLE_REVOCO_CONFIG,	TEXT_REVOCO_ENABLE);
	SET_BUTTON_TEXT(RADIO_REVOCO_FREE_CONFIG,	TEXT_REVOCO_FREE_MODE);
	SET_BUTTON_TEXT(RADIO_REVOCO_CLICK_CONFIG,	TEXT_REVOCO_CLICK_MODE);
	SET_BUTTON_TEXT(RADIO_REVOCO_MANUAL_CONFIG,	TEXT_REVOCO_MANUAL_MODE);
	SET_BUTTON_TEXT(RADIO_REVOCO_AUTO_CONFIG,	TEXT_REVOCO_AUTO_MODE);
	SET_LABEL_TEXT(LABEL_REVOCO_BUTTON_CONFIG,	TEXT_REVOCO_BUTTON_NUMBER);
	SET_LABEL_TEXT(LABEL_REVOCO_UP_CONFIG,		TEXT_REVOCO_UP_SCROLL);
	SET_LABEL_TEXT(LABEL_REVOCO_DOWN_CONFIG,	TEXT_REVOCO_DOWN_SCROLL);
	
	/* -------------Advanced tab widgets-----------------*/
	SET_LABEL_MARKUP(LABEL_FRAME_ADVANCED_CONFIG, TEXT_ADVANCED_SETTINGS);
	SET_LABEL_MARKUP(LABEL_FRAME_HOTPLUGGING_CONFIG, TEXT_HOTPLUGGING);
	SET_BUTTON_TEXT(CB_UDEV_ENABLE_CONFIG,		TEXT_ENABLE_HOTPLUGGING);
	SET_BUTTON_TEXT(CB_SYSFS_CONFIG,			TEXT_WAIT_FOR_SYSFS);
	
	/* -------------Mouse detection window widgets-----------------*/
	SET_LABEL_MARKUP(LABEL_FRAME_INSTRUCTIONS_MOUSED, TEXT_INSTRUCTIONS);
	SET_LABEL_MARKUP(LABEL_FRAME_STATUS_MOUSED,	_("<b>Detection status</b>"));
	SET_LABEL_TEXT(LABEL_VENDORID_MOUSED,		TEXT_VENDOR_ID);
	SET_LABEL_TEXT(LABEL_PRODUCTID_MOUSED,		TEXT_PRODUCT_ID);
	
	/* -------------Button detection window widgets-----------------*/
	SET_LABEL_MARKUP(LABEL_FRAME_INSTRUCTIONS_BTND, TEXT_INSTRUCTIONS);
	SET_LABEL_MARKUP(LABEL_FRAME_STATUS_BTND,	_("<b>Button detection</b>"));
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(GET_WIDGET(PROGRESSBAR_BTND)),
												_("Detection progress"));
	SET_LABEL_TEXT(LABEL_STATUS_BTND,			_("Status:"));
	/* TRANSLATORS: the button refers to a mouse button. */
	SET_LABEL_TEXT(LABEL_BTN_NAME_BTND,			_("Button name:"));
	
	/* -------------Button detection error dialog widgets-----------------*/
	SET_LABEL_TEXT(LABEL_TITLE_DIALOG_BTND,		TEXT_TITLE_DIALOG_BTND);
	SET_LABEL_TEXT(LABEL_BODY_DIALOG_BTND,		TEXT_BODY_DIALOG_BTND);
	
}

static void init_tooltips(void)
{
	GtkTooltips *tips = gtk_tooltips_new();
	
	/* -------------Configurations tab tooltips-----------------*/
	SET_TOOLTIP(tips, ENTRY_CONFIG_NAME_CONFIG,
				_("Enter the name of this configuration."),
				NULL);
	SET_TOOLTIP(tips, BTN_NEW_CONFIG,
				_("Create a new configuration."),
				NULL);
	SET_TOOLTIP(tips, BTN_DELETE_CONFIG,
				_("Delete the selected configuration."),
				NULL);
	SET_TOOLTIP(tips, BTN_UP_CONFIG,
				_("Increase the selected configuration's priority."),
				NULL);
	SET_TOOLTIP(tips, BTN_DOWN_CONFIG,
				_("Decrease the selected configuration's priority."),
				NULL);
	SET_TOOLTIP(tips, BTN_BACKUP_CONFIG,
				_("Backup all configurations and settings to a file."),
				NULL);
	SET_TOOLTIP(tips, BTN_RESTORE_CONFIG,
				_("Restore all configurations and settings from a backup file."),
				NULL);
	
	/* -------------Buttons tab tooltips-----------------*/
	SET_TOOLTIP(tips, BTN_DELETE_BUTTON_CONFIG,
				_("Delete the currently selected button"),
				NULL);
	SET_TOOLTIP(tips, TREEVIEW_BUTTONS_CONFIG,
				_("Select a button from the list to edit its properties"),
				NULL);
	SET_TOOLTIP(tips, BTN_DETECT_CONFIG,
				_("Go to the mouse and button detection utility"),
				NULL);
	SET_TOOLTIP(tips, ENTRY_VENDOR_CONFIG,
				_("Enter the name of your mouse manufacturer"),
				NULL);
	SET_TOOLTIP(tips, ENTRY_PRODUCT_CONFIG,
				_("Enter the model name of your mouse"),
				NULL);
	SET_TOOLTIP(tips, CB_ENABLED_CONFIG,
	/* TRANSLATORS: do not translate btnx. It is the name of a program. */
				_("Enable|Disable this button for use with btnx"),
				NULL);
	SET_TOOLTIP(tips, ENTRY_BUTTON_NAME_CONFIG,
				_("The name of this button"),
				NULL);
	SET_TOOLTIP(tips, LABEL_EVTYPE_CONFIG,
				TEXT_EVTYPE_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, COMBOBOX_EVTYPE_CONFIG,
				TEXT_EVTYPE_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, ENTRY_DELAY_CONFIG,
/* TRANSLATORS: this describes to the user what "Repeat delay" means. Do not
 * translate btnx, it is the name of a program. */
_("The delay in milliseconds before btnx will allow this button to send \
another event after being pressed. This is useful if your button is very \
sensitive or sends many signals at once."),
				NULL);
	SET_TOOLTIP(tips, CB_FORCE_RELEASE_CONFIG,
_("Force btnx to send a button release signal right after the button is \
pressed. Helps with buttons that get stuck. The button cannot be held down."),
				NULL);
	gtk_tooltips_set_tip(tips, GET_KEYCODE_ENTRY(),
/* TRANSLATORS: btnx is the name of a program. Do not translate it. The modifer
 * key is a keyboard key that can be, for example, Shift, Ctrl, or Alt. */
_("The key or mouse button sent by btnx when pressing this button. It can be \
accompanied by up to three modifier keys."),
				NULL);
	SET_TOOLTIP(tips, LABEL_MOD1_CONFIG,
				TEXT_MODIFIER_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, COMBOBOX_MOD1_CONFIG,
				TEXT_MODIFIER_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, LABEL_MOD2_CONFIG,
				TEXT_MODIFIER_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, COMBOBOX_MOD2_CONFIG,
				TEXT_MODIFIER_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, LABEL_MOD3_CONFIG,
				TEXT_MODIFIER_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, COMBOBOX_MOD3_CONFIG,
				TEXT_MODIFIER_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, RADIO_SWITCH_NEXT_CONFIG,
				_("Switch to the next configuration with a lower priority."),
				NULL);
	SET_TOOLTIP(tips, RADIO_SWITCH_PREV_CONFIG,
				_("Switch to the previous configuration with a higher priority."),
				NULL);
	SET_TOOLTIP(tips, RADIO_SWITCH_TO_CONFIG,
_("Switch to the configuration selected in the drop-down list to the right."),
				NULL);
	SET_TOOLTIP(tips, ENTRY_COMMAND_CONFIG,
/* TRANSLATORS: do not translate btnx. It is the name of a program. */
_("The shell command sent by btnx when pressing this button. The first \
word must be the full path of the program (Example: /usr/bin/gedit) followed \
by optional arguments."),
				NULL);
	SET_TOOLTIP(tips, ENTRY_UID_CONFIG,
/* TRANSLATORS: The UID refers
 * to User Identification, a number referring to a user in a 
 * Linux system. Do not try to translate UID unless it is explicitly. Try
 * to make this sound serious, because the user might be doing something that
 * compromises their system security! */
_("The UID of the owner of the process started by the command. Change this to \
your UID or the UID defaults to 0. This means the PROCESS WILL BE RUN AS ROOT \
(not recommended, unless necessary)."),
				NULL);
	/*SET_TOOLTIP(tips, BTN_CLEAR_CONFIG,
				_("Clear all configuration values"),
				NULL);*/
	/*SET_TOOLTIP(tips, BTN_SAVE_CONFIG,
				_("Save the configuration file"),
				NULL);*/
	SET_TOOLTIP(tips, BTN_RESTART_BTNX_CONFIG,
	/* TRANSLATORS: do not translate btnx. It is the name of a program. */
				_("Restart btnx with the current settings"),
				NULL);
	SET_TOOLTIP(tips, BTN_HELP_CONFIG,
				TEXT_HELP_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, BTN_ABOUT_CONFIG,
				_("View information about this program"),
				NULL);
	SET_TOOLTIP(tips, BTN_CLOSE_CONFIG,
	/* TRANSLATORS: do not translate btnx-config. It is the name of a program. */
				_("Close btnx-config"),
				NULL);
	
	/* -------------revoco tab tooltips-----------------*/
	SET_TOOLTIP(tips, CB_ENABLE_REVOCO_CONFIG,
				_("If enabled, btnx will launch revoco with the settings below."),
				NULL);
	SET_TOOLTIP(tips, RADIO_REVOCO_FREE_CONFIG,
				_("Set the mouse wheel to free scroll where it spins without friction."),
				NULL);
	SET_TOOLTIP(tips, RADIO_REVOCO_CLICK_CONFIG,
_("Set the mouse wheel to click-to-click scroll where it spins like a normal "
"mouse wheel"),
				NULL);
	SET_TOOLTIP(tips, RADIO_REVOCO_MANUAL_CONFIG,
_("Select a mouse button to switch between free-scroll and click-to-click mode."),
				NULL);
	SET_TOOLTIP(tips, RADIO_REVOCO_AUTO_CONFIG,
_("Set the mouse wheel to automatic which switches the mode to free-scroll "
"automatically when you scroll it up or down faster than the values selected "
"on the right."),
				NULL);
	
	/* -------------Advanced tab tooltips-----------------*/
	SET_TOOLTIP(tips, CB_UDEV_ENABLE_CONFIG,
_("This setting starts btnx automatically when you plug in your mouse. Also "
"starts btnx after standby. Useful for laptops."),
				NULL);
	SET_TOOLTIP(tips, CB_SYSFS_CONFIG,
_("Set the WAIT_FOR_SYSFS udev option in btnx's udev rule. Some systems have "
"trouble with hot-plugging with this enabled, others have trouble with it disabled."),
				NULL);

				
	/* -------------Mouse detection window tooltips-----------------*/
	SET_TOOLTIP(tips, BTN_CONTROL_MOUSED,
				_("Start|Stop mouse detection"),
				NULL);
	SET_TOOLTIP(tips, BTN_HELP_MOUSED,
				TEXT_HELP_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, BTN_CANCEL_MOUSED,
				TEXT_CANCEL_DETECTION,
				NULL);
	SET_TOOLTIP(tips, BTN_UNDO_MOUSED,
				_("Remove detection information"),
				NULL);
	SET_TOOLTIP(tips, BTN_FORWARD_MOUSED,
				_("Continue to button detection"),
				NULL);
	
	/* -------------Button detection window tooltips-----------------*/
	SET_TOOLTIP(tips, BTN_CONTROL_BTND,
				_("Start|Stop button detection"),
				NULL);
	SET_TOOLTIP(tips, ENTRY_NAME_BTND,
				_("Give this button a unique name"),
				NULL);
	SET_TOOLTIP(tips, BTN_HELP_BTND,
				TEXT_HELP_TOOLTIP,
				NULL);
	SET_TOOLTIP(tips, BTN_CANCEL_BTND,
				TEXT_CANCEL_DETECTION,
				NULL);
	SET_TOOLTIP(tips, BTN_ADD_BTND,
	/* TRANSLATORS: this is a tooltip that appears when a user hovers over an
	 * add button. Its function is to add a mouse button to a list that the
	 * user has detected with the program. */
				_("Add the button"),
				NULL);
	SET_TOOLTIP(tips, BTN_OK_BTND,
	/* TRANSLATORS: buttons refers to mouse buttons. This saves information
	 * about buttons that the user has detected with the program. */
				_("Save the buttons and return to the main window"),
				NULL);
}


void init_main(void)
{
	GtkWidget *win_config, *win_moused, *win_btnd;
	GtkWidget *dialog_about;
	GdkPixbuf *logo;
	GError *error = NULL;
	
	init_static_texts();
	
	keycodes_entry_init();
	
	init_config();
	
	g_signal_connect_swapped(	G_OBJECT(GET_WIDGET(BTN_CLOSE_CONFIG)),
								"clicked",
								G_CALLBACK(gtk_widget_destroy),
								G_OBJECT(GET_WIDGET(WIN_CONFIG)));
	
	tree_buttons_init();
	dialog_btnd_init();
	
	dialog_about = GET_WIDGET(DIALOG_ABOUT);
	win_config = GET_WIDGET(WIN_CONFIG);
	win_moused = GET_WIDGET(WIN_MOUSED);
	win_btnd = GET_WIDGET(WIN_BTND);
	
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog_about), VERSION);
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog_about),
	/* TRANSLATORS: do not translate btnx or Button Extension. They are
	 * names of a program. You may add additional words to provide a translation
	 * for "Button Extension" in your language. */
		_("A configuration utility for btnx (Button Extension)"));
	
	logo = gdk_pixbuf_new_from_file(ICON_PATH, &error);
	if (logo != NULL)
	{
		gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog_about), logo);
		g_object_unref(logo);
	}
	else
		fprintf(stderr, "Error: could not load program logo.\n");
	
	gtk_window_set_icon_from_file(GTK_WINDOW(win_config), ICON_PATH, &error);
	gtk_window_set_icon_from_file(GTK_WINDOW(win_moused), ICON_PATH, &error);
	gtk_window_set_icon_from_file(GTK_WINDOW(win_btnd), ICON_PATH, &error);
	
	init_tooltips();
	
	gtk_widget_hide(win_moused);
	gtk_widget_hide(win_btnd);
	gtk_widget_hide(dialog_about);
}

void init_moused(void)
{
	gchar *handlers = g_strdup_printf(TEXT_DETECTION_STATUS, 0);
	gtk_widget_set_sensitive(	GET_WIDGET(BTN_FORWARD_MOUSED),
								FALSE);
	gtk_widget_set_sensitive(	GET_WIDGET(BTN_UNDO_MOUSED),
								FALSE);
	gtk_button_set_label(GTK_BUTTON(GET_WIDGET(BTN_CONTROL_MOUSED)),
						TEXT_START_MOUSE_DETECTION);
	gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_HELP_MOUSED)),
						TEXT_HELP_MOUSED_INIT);
	gtk_label_set_text( GTK_LABEL(GET_WIDGET(LABEL_HANDLERS_MOUSED)),
						handlers);
	gtk_label_set_text( GTK_LABEL(GET_WIDGET(LABEL_VENDORIDVAL_MOUSED)),
						"0x0000");
	gtk_label_set_text( GTK_LABEL(GET_WIDGET(LABEL_PRODUCTIDVAL_MOUSED)),
						"0x0000");
	gtk_window_set_focus(GTK_WINDOW(GET_WIDGET(WIN_MOUSED)), GET_WIDGET(BTN_CONTROL_MOUSED));
	g_free(handlers);
}

void init_config_button_field(void)
{
	gtk_widget_set_sensitive(GET_WIDGET(BTN_DELETE_BUTTON_CONFIG), FALSE);
	gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_EVTYPE_CONFIG)), 0);
	//gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_KEYCODE_CONFIG)), 0);
	gtk_entry_set_text(GTK_ENTRY(GET_KEYCODE_ENTRY()), "NONE");
	gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD1_CONFIG)), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD2_CONFIG)), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_MOD3_CONFIG)), 0);
	gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_BUTTON_NAME_CONFIG)), "");
	gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_DELAY_CONFIG)), "");
	gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_COMMAND_CONFIG)), "");
	gtk_entry_set_text(GTK_ENTRY(GET_WIDGET(ENTRY_UID_CONFIG)), "");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(CB_ENABLED_CONFIG)), FALSE);
	
	config_update_evtype(-1);
}

void init_config(void)
{
	tree_config_init();
	init_config_button_field();
	config_refresh_mouse_properties();
	revoco_init();
}

void init_btnd(void)
{
	gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_HELP_BTND)),
						TEXT_HELP_BTND_INIT);
	gtk_widget_set_sensitive(	GET_WIDGET(BTN_ADD_BTND),
								FALSE);
	gtk_widget_set_sensitive(	GET_WIDGET(ENTRY_NAME_BTND),
								FALSE);
	gtk_button_set_label(GTK_BUTTON(GET_WIDGET(BTN_CONTROL_BTND)),
						TEXT_START_BUTTON_DETECTION);
	gtk_label_set_text(GTK_LABEL(GET_WIDGET(LABEL_STATUS_BTND)),
						TEXT_WAIT_DETECTION);
	gtk_label_set_text(GTK_LABEL(GET_WIDGET(LABEL_HELP_BTND)),
						TEXT_HELP_BTND_INIT);
	gtk_progress_bar_set_fraction(	GTK_PROGRESS_BAR(GET_WIDGET(PROGRESSBAR_BTND)), 0);
	
	gtk_window_set_focus(GTK_WINDOW(GET_WIDGET(WIN_BTND)), GET_WIDGET(BTN_CONTROL_BTND));
}

void init_evtype(void)
{
	gtk_combo_box_remove_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_EVTYPE_CONFIG)), 1);
	gtk_combo_box_remove_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_EVTYPE_CONFIG)), 0);
	gtk_combo_box_append_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_EVTYPE_CONFIG)), EVTYPE_KEY_STR);
	gtk_combo_box_append_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_EVTYPE_CONFIG)), EVTYPE_CONFIG_STR);
	gtk_combo_box_append_text(GTK_COMBO_BOX(GET_WIDGET(COMBOBOX_EVTYPE_CONFIG)), EVTYPE_COMMAND_STR);
}


