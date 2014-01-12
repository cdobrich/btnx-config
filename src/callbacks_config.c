
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/* Callbacks for the main configuration window */

#include <stdlib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk/gdk.h>

#include "widgets.h"
#include "callbacks.h"
#include "threads.h"
#include "config.h"
#include "tree_buttons.h"
#include "buttons.h"
#include "handlers.h"
#include "mouse.h"
#include "config_file.h"
#include "text.h"
#include "keycodes_entry.h"
#include "udev.h"
#include "revoco.h"
#include "config_manager.h"
#include "tree_config.h"
#include "backup.h"
#include "common.h"

static int block_update=0;
static int block_config_name_check=0;

void set_block_update(int block)
{
	block_update = block;
}

gboolean win_config_delete_event(	GtkWidget	*widget, 
									GdkEvent	*event, 
									gpointer	user_data)
{
	return FALSE;
}

void win_config_destroy(GtkWidget	*widget, 
						gpointer	user_data)
{
	config_manager_save_file();
	config_save();
	udev_create();
	settings_file_save();
	threads_sigterm(30);	
}

void btn_detect_config_clicked(GtkButton *button, gpointer user_data)
{
	//g_print("product: %04x vendor: %04x\n", mouse_get_product_id(), mouse_get_vendor_id());
	if (mouse_get_product_id() != 0 && mouse_get_vendor_id() != 0)
	{
		if (handlers_add_by_ids(mouse_get_vendor_id(), mouse_get_product_id()) != 0)
		{
			init_btnd();
			widgets_center_on_parent(GET_WIDGET(WIN_BTND), GET_WIDGET(WIN_CONFIG));			
			gtk_widget_hide(GET_WIDGET(WIN_CONFIG));
			gtk_widget_show(GET_WIDGET(WIN_BTND));
			return;
		}
		else
		{
			fprintf(stderr, "Error: mouse handler IDs set but not found.\n");
		}
	}
	init_moused();
	widgets_center_on_parent(GET_WIDGET(WIN_MOUSED), GET_WIDGET(WIN_CONFIG));			
	gtk_widget_hide(GET_WIDGET(WIN_CONFIG));
	gtk_widget_show(GET_WIDGET(WIN_MOUSED));
}

void btn_save_config_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog;
	gint confirm;
	
	SHOW_CONFIRM_DIALOG(dialog, GET_WIDGET(WIN_CONFIG), confirm,
	/* TRANSLATORS: This is a dialog title. It asks the user whether they
	 * want to save the current configuration to a file. (This is a specific
	 * file, it's name cannot be specified by the user) */
						_("Save configuration to file?"),
						" ");
	if (confirm != GTK_RESPONSE_YES)
		return;
	
	config_save();
	//udev_create();
}

void btn_restart_btnx_config_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog;
	gchar *cmd, *err;
	int ret;

	config_save();
	//udev_create();
	/* MAINTAINERS: the directory of INIT_SCRIPT_FILE (default=/etc/init.d)
	 * is controlled through "./configure init_scripts_path=/path/to" 
	 * see common.h for declaration of INIT_SCRIPT_FILE */
	cmd = g_strconcat(	INIT_SCRIPT_FILE " restart \"", 
						config_manager_get_current_name(), 
						"\"", 
						NULL);
	if ((ret = system(cmd)) != 0)
	{
		switch (ret)
		{
		case BTNX_ERROR_NO_CONFIG:
			err = g_strdup(TEXT_BTNX_NO_CONFIG_ERROR);
			break;
		case BTNX_ERROR_BAD_CONFIG:
			err = g_strdup(TEXT_BTNX_BAD_CONFIG_ERROR);
			break;
		case BTNX_ERROR_OPEN_HANDLER:
			err = g_strdup(TEXT_BTNX_OPEN_HANDLER_ERROR);
			break;
		case BTNX_ERROR_OPEN_UINPUT:
			err = g_strdup(TEXT_BTNX_OPEN_UINPUT_ERROR);
			break;
		default:
			err = g_strdup(TEXT_BTNX_UNSPECIFIED_ERROR);
		}
		SHOW_ERROR_DIALOG(	dialog, GET_WIDGET(WIN_CONFIG),
							TEXT_BTNX_RESTART_ERROR,
							err);
		g_free(err);
	}
	g_free(cmd);
}

void tree_configs_row_activated(GtkTreeView *tree_view, 
								GtkTreePath *path, 
								GtkTreeViewColumn *column, 
								gpointer user_data)
{
	config_save();
	config_manager_open_selected_config();
}

gboolean entry_config_name_config_activate(GtkEntry *entry, gpointer user_data)
{
	config_name_changed(GTK_WIDGET(entry), user_data);
	return FALSE;
}

gboolean config_name_changed(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *dialog=NULL;
	int ret;
	
	if (block_config_name_check == 1)
		return FALSE;
	block_config_name_check = 1;
	
	ret = config_manager_set_current_name(gtk_entry_get_text(GTK_ENTRY(widget)));
	if (ret == -1)
	{
		block_config_name_check = 0;
		return FALSE;
	}
	if (ret == -2)
	{
		SHOW_ERROR_DIALOG(dialog, GET_WIDGET(WIN_CONFIG),
			_("Error: invalid configuration name"),
			_("The configuration name you chose contains invalid characters."));
	}
	else if (ret == -3)
	{
		SHOW_ERROR_DIALOG(dialog, GET_WIDGET(WIN_CONFIG),
			_("Error: invalid configuration name"),
			_("The configuration name you chose cannot be used."));
	}
	else if (ret == -4)
	{
		SHOW_ERROR_DIALOG(dialog, GET_WIDGET(WIN_CONFIG),
			_("Error: configuration name exists"),
			_("The configuration name is already used by another configuration."
			" Choose another name"));
	}
	block_config_name_check = 0;
	return FALSE;
}

gboolean button_property_changed(GtkWidget *widget, gpointer user_data)
{
	if (block_update == 1)
	{
		//g_print("update blocked\n");
		return FALSE;
	}
		
	if (widget == GET_KEYCODE_ENTRY())
		config_update_button_properties(SHOW_ERROR_KEYCODE);
	else if (widget == GET_WIDGET(ENTRY_UID_CONFIG))
		config_update_button_properties(SHOW_ERROR_UID);
	else if (widget == GET_WIDGET(ENTRY_DELAY_CONFIG))
		config_update_button_properties(SHOW_ERROR_DELAY);
	else
		config_update_button_properties(0);

	if (widget == GET_WIDGET(COMBOBOX_EVTYPE_CONFIG))
		config_update_evtype(buttons_get_evtype(buttons_exists(config_get_rawcode())));
	
	return FALSE;
}

void cb_enabled_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	button_property_changed(GTK_WIDGET(toggle), NULL);
}

void cb_force_release_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	button_property_changed(GTK_WIDGET(toggle), NULL);
}

gboolean mouse_property_changed(GtkWidget *widget, gpointer user_data)
{
	config_update_mouse_properties();
	return FALSE;
}

void tree_config_cursor_changed(GtkTreeView *tree_view, gpointer user_data)
{
	int rawcode;
	block_update = 1;
	
	GdkCursor* cursor;
	GdkWindow *gdkwindow;
	
	gdkwindow = gtk_widget_get_parent_window(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG));
	cursor = gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(gdkwindow, cursor);
	gdk_cursor_destroy(cursor);
	
	//g_print("getting selected rawcode\n");
	rawcode = tree_buttons_get_selected_button_rawcode();
	if (config_edit_rawcode(rawcode) == 0)
		goto done;
		
	//g_print("refreshing properties\n");
	config_refresh_button_properties();
	
done:
	cursor = gdk_cursor_new(GDK_LEFT_PTR);
	gdk_window_set_cursor(gdkwindow, cursor);
	gdk_cursor_destroy(cursor);
	block_update = 0;
}

void tree_config_row_activated(	GtkTreeView *tree_view, 
								GtkTreePath *path, 
								GtkTreeViewColumn *column, 
								gpointer user_data)
{
	if (gtk_tree_view_row_expanded(GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)), path))
	{
		gtk_tree_view_collapse_row(
			GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)),
			path);
	}
	else
	{
		gtk_tree_view_expand_row(
			GTK_TREE_VIEW(GET_WIDGET(TREEVIEW_BUTTONS_CONFIG)),
			path, 
			FALSE);
	}
}

void btn_clear_config_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog;
	gint confirm;
	
	SHOW_CONFIRM_DIALOG(dialog, GET_WIDGET(WIN_CONFIG), confirm,
	/* TRANSLATORS: This is a dialog title. It asks whether the user wants to
	 * delete all the configuration data of the program (this change is not
	 * permanent unless the user saves the cleared configuration) */
	_("Are you sure you want to clear all configuration values?"),
	_("These changes are not permanent until you save the configuration file."));
	
	if (confirm == GTK_RESPONSE_YES)
		config_clear_all();
}

void btn_about_config_clicked(GtkButton *button, gpointer user_data)
{
	gtk_dialog_run(GTK_DIALOG(GET_WIDGET(DIALOG_ABOUT)));
	gtk_widget_hide(GET_WIDGET(DIALOG_ABOUT));
}

void btn_delete_button_config_clicked(GtkButton *button, gpointer user_data)
{
	config_delete_selected_button();
}

void combobox_keycode_config_changed(GtkComboBox *widget, gpointer user_data)
{
	GtkWidget *entry = GET_KEYCODE_ENTRY();
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *text;
	
	if (gtk_combo_box_get_active_iter(widget, &iter) == FALSE)
		return;
	if ((model = gtk_combo_box_get_model(widget)) == FALSE)
		return;
	gtk_tree_model_get(	model, &iter, 
						0, &text,
						-1);
	
	if (text == NULL)
		return;
	
	gtk_entry_set_text(GTK_ENTRY(entry), text);
	button_property_changed(GTK_WIDGET(widget), NULL);
	
	g_free(text);
}

void radio_switch_next_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	gtk_widget_set_sensitive(GET_WIDGET(COMBOBOX_SWITCH_TO_CONFIG), FALSE);
	button_property_changed(GTK_WIDGET(toggle), user_data);
}

void radio_switch_prev_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	gtk_widget_set_sensitive(GET_WIDGET(COMBOBOX_SWITCH_TO_CONFIG), FALSE);
	button_property_changed(GTK_WIDGET(toggle), user_data);
}

void radio_switch_to_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	gtk_widget_set_sensitive(GET_WIDGET(COMBOBOX_SWITCH_TO_CONFIG), TRUE);
	button_property_changed(GTK_WIDGET(toggle), user_data);
}


/* -------------------------------------------------------------------------- */
/* Callbacks for revoco tab */

void cb_enable_revoco_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	int enabled = gtk_toggle_button_get_active(
					GTK_TOGGLE_BUTTON(
					GET_WIDGET(CB_ENABLE_REVOCO_CONFIG)));
	revoco_enabled_changed(enabled);
}

void radio_revoco_free_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	revoco_free_selected();
}

void radio_revoco_click_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	revoco_click_selected();
}

void radio_revoco_manual_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	revoco_manual_selected();
}

void radio_revoco_auto_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	revoco_auto_selected();
}

void spin_revoco_button_config_value_changed(GtkSpinButton *spin, gpointer user_data)
{
	revoco_btn_changed(gtk_spin_button_get_value(spin));
}

void spin_revoco_up_config_value_changed(GtkSpinButton *spin, gpointer user_data)
{
	revoco_up_changed(gtk_spin_button_get_value(spin));
}

void spin_revoco_down_config_value_changed(GtkSpinButton *spin, gpointer user_data)
{
	revoco_down_changed(gtk_spin_button_get_value(spin));
}


/* -------------------------------------------------------------------------- */
/* Callbacks for configuration manager tab */

void btn_new_config_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog;
	int copy; //, save;
	
	if (config_manager_current() < 0)
	{
		config_manager_new("New config", 0);
		return;
	}
	/*else if (config_manager_changed() == 1)
	{
		SHOW_CONFIRM_DIALOG(dialog, GET_WIDGET(WIN_CONFIG), save,
			_("Save configuration?"),
			_("You have made changes to the previous configuration. You will "
			"lose your changes if you do not save them before creating a new "
			"configuration."));
		if (save == GTK_RESPONSE_YES)
			config_save();
	}*/
		
	SHOW_CONFIRM_DIALOG(dialog, GET_WIDGET(WIN_CONFIG), copy,
		_("Copy configuration?"),
		_("Pressing yes will copy your currently open configuration "
		"values into the new one."));
	
	if (copy == GTK_RESPONSE_YES)
		config_manager_new("New config", 1);
	else 
		config_manager_new("New config", 0);
}

void btn_delete_config_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog;
	int confirm=-1;
	gchar *name=NULL;
	
	if (tree_config_get_selected_data(NULL, NULL, &name) < 0)
		return;
	
	SHOW_CONFIRM_DIALOG(dialog, GET_WIDGET(WIN_CONFIG), confirm,
			_("Delete configuration?"),
			_("Pressing yes will delete the selected configuration and its "
			"file."));
	
	if (confirm == GTK_RESPONSE_YES)
		config_manager_delete(name);
		
	g_free(name);
}

void btn_up_config_clicked(GtkButton *button, gpointer user_data)
{
	gchar *name=NULL;
	if (tree_config_get_selected_data(NULL, NULL, &name) < 0)
		return;
	config_manager_priority_higher(name);
	g_free(name);
}

void btn_down_config_clicked(GtkButton *button, gpointer user_data)
{
	gchar *name=NULL;
	
	if (tree_config_get_selected_data(NULL, NULL, &name) < 0)
		return;
	config_manager_priority_lower(name);
	g_free(name);
}

void btn_backup_config_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog, *err_dialog;
	char *file_name;
	int ret;
	
	dialog = gtk_file_chooser_dialog_new(	"Backup to file",
											GTK_WINDOW(GET_WIDGET(WIN_CONFIG)),
											GTK_FILE_CHOOSER_ACTION_SAVE,
											GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
											GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
											NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), "/home");
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "btnx_conf_backup.tar.gz");
	
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		settings_file_save();
		file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		ret = backup_config(file_name);
		g_free(file_name);
		
		switch (ret)
		{
		case ERROR_BACKUP_OPEN_VERSION:
		case ERROR_BACKUP_WRITE_VERSION:
			SHOW_ERROR_DIALOG(err_dialog, dialog,
				TEXT_BACKUP_ERROR,
				_("Could not create a version stamp file."));
			break;
		case ERROR_BACKUP_TAR_CREATE:
			SHOW_ERROR_DIALOG(err_dialog, dialog,
				TEXT_BACKUP_ERROR,
				_("Unspecified tar execution failure."));
			break;
		default:
			break;
		}
	}
	
	gtk_widget_destroy(dialog);
}

void btn_restore_config_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog, *err_dialog;
	char *file_name=NULL;
	int ret;
	
	dialog = gtk_file_chooser_dialog_new(	"Restore from file",
											GTK_WINDOW(GET_WIDGET(WIN_CONFIG)),
											GTK_FILE_CHOOSER_ACTION_OPEN,
											GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
											GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
											NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), "/home");
	//gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "btnx_conf_backup.tar.gz");
	
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		ret = restore_config(file_name, 0);
		
		switch (ret)
		{
		case ERROR_RESTORE_EXTRACT_VERSION:
			SHOW_ERROR_DIALOG(err_dialog, GET_WIDGET(WIN_CONFIG),
				TEXT_RESTORE_ERROR,
				_("Possibly broken or invalid backup file."));
			break;
		case ERROR_RESTORE_VERSION_INCOMPATIBLE:
			SHOW_ERROR_DIALOG(err_dialog, GET_WIDGET(WIN_CONFIG),
				TEXT_RESTORE_ERROR,
				_("The backup configuration is from an incompatible version of "
				"btnx. Restore operation cancelled."));
			break;
		case ERROR_RESTORE_TAR_EXTRACT:
			SHOW_ERROR_DIALOG(err_dialog, GET_WIDGET(WIN_CONFIG),
				TEXT_RESTORE_ERROR,
				_("Unspecified tar execution failure."));
			break;
		default:
			config_clear_all();
			config_manager_init();
			udev_init();
			settings_file_parse();
			config_refresh_advanced_settings();
			break;
		}
	}
	
	if (file_name != NULL)
		g_free(file_name);
	gtk_widget_destroy(dialog);
}

void cb_udev_enable_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	udev_enabled_set(gtk_toggle_button_get_active(toggle));
	if (udev_enabled_get() == 0)
		gtk_widget_set_sensitive(GET_WIDGET(CB_SYSFS_CONFIG), FALSE);
	else
		gtk_widget_set_sensitive(GET_WIDGET(CB_SYSFS_CONFIG), TRUE);
}

void cb_sysfs_config_toggled(GtkToggleButton *toggle, gpointer user_data)
{
	udev_sysfs_enabled_set(gtk_toggle_button_get_active(toggle));
}

