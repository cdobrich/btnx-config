
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <gtk/gtk.h>
#include <glade/glade.h>


/* Function code in callbacks_config.c */
/* btn_close_config handled separately (init_config()) */
gboolean win_config_delete_event(	GtkWidget	*widget, 
									GdkEvent	*event, 
									gpointer	user_data);
void win_config_destroy(GtkWidget	*widget, 
						gpointer	user_data);	
void btn_new_config_clicked(GtkButton *button, gpointer user_data);
void btn_delete_config_clicked(GtkButton *button, gpointer user_data);
void btn_up_config_clicked(GtkButton *button, gpointer user_data);
void btn_down_config_clicked(GtkButton *button, gpointer user_data);
void btn_backup_config_clicked(GtkButton *button, gpointer user_data);
void btn_restore_config_clicked(GtkButton *button, gpointer user_data);
void btn_detect_config_clicked(GtkButton *button, gpointer user_data);
void btn_save_config_clicked(GtkButton *button, gpointer user_data);
void btn_restart_btnx_config_clicked(GtkButton *button, gpointer user_data);
void tree_configs_row_activated(GtkTreeView *tree_view, 
								GtkTreePath *path, 
								GtkTreeViewColumn *column, 
								gpointer user_data);
gboolean entry_config_name_config_activate(GtkEntry *entry, gpointer user_data);
gboolean config_name_changed(GtkWidget *widget, gpointer user_data);
gboolean mouse_property_changed(GtkWidget *widget, gpointer user_data);

gboolean button_property_changed(GtkWidget *widget, gpointer user_data);
void cb_enabled_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void cb_force_release_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void tree_config_cursor_changed(GtkTreeView *tree_view, gpointer user_data);
void tree_config_row_activated(	GtkTreeView *tree_view, 
								GtkTreePath *path, 
								GtkTreeViewColumn *column, 
								gpointer user_data);
void btn_clear_config_clicked(GtkButton *button, gpointer user_data);
void btn_about_config_clicked(GtkButton *button, gpointer user_data);
void btn_delete_button_config_clicked(GtkButton *button, gpointer user_data);
void combobox_keycode_entry_config_changed(GtkComboBox *widget, gpointer user_data);
void radio_switch_next_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void radio_switch_prev_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void radio_switch_to_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void set_block_update(int block);

void cb_enable_revoco_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void radio_revoco_free_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void radio_revoco_click_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void radio_revoco_manual_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void radio_revoco_auto_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void spin_revoco_button_config_value_changed(GtkSpinButton *spin, gpointer user_data);
void spin_revoco_up_config_value_changed(GtkSpinButton *spin, gpointer user_data);
void spin_revoco_down_config_value_changed(GtkSpinButton *spin, gpointer user_data);

void cb_udev_enable_config_toggled(GtkToggleButton *toggle, gpointer user_data);
void cb_sysfs_config_toggled(GtkToggleButton *toggle, gpointer user_data);


/* Function code in callbacks_moused.c */
gboolean win_moused_delete_event(	GtkWidget	*widget, 
									GdkEvent	*event, 
									gpointer	user_data);
void win_moused_destroy(GtkWidget	*widget, 
						gpointer	user_data);	
void btn_control_moused_clicked(GtkButton *button, gpointer user_data);
void btn_cancel_moused_clicked(GtkButton *button, gpointer user_data);
void btn_undo_moused_clicked(GtkButton *button, gpointer user_data);



/* Function code in callbacks_btnd.c */
gboolean win_btnd_delete_event(	GtkWidget	*widget, 
								GdkEvent	*event, 
								gpointer	user_data);
void win_btnd_destroy(	GtkWidget	*widget, 
						gpointer	user_data);	
void btn_control_btnd_clicked(GtkButton *button, gpointer user_data);
void btn_add_btnd_clicked(GtkButton *button, gpointer user_data);
void btn_ok_btnd_clicked(GtkButton *button, gpointer user_data);
void btn_cancel_btnd_clicked(GtkButton *button, gpointer user_data);
void btn_ok_dialog_btnd_clicked(GtkButton *button, gpointer user_data);
void btn_cancel_dialog_btnd_clicked(GtkButton *button, gpointer user_data);

/* Help buttons, defined in help.c */
void btn_help_config_clicked(GtkButton *button, gpointer user_data);
void btn_help_moused_clicked(GtkButton *button, gpointer user_data);
void btn_help_btnd_clicked(GtkButton *button, gpointer user_data);

#endif /*CALLBACKS_H_*/
