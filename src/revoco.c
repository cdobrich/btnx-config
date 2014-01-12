
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  * 
  * Actual revoco code, distributed with btnx (not this program),
  * is not under the same copyright.
  */
  
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "common.h"
#include "text.h"
#include "revoco.h"
#include "mouse.h"
#include "widgets.h"

static int revoco_prev_mode = REVOCO_FREE_MODE;
//static int revoco_prev_scroll_up = REVOCO_DEFAULT_SCROLL;
//static int revoco_prev_scroll_down = REVOCO_DEFAULT_SCROLL;
static int revoco_prev_btn = REVOCO_DEFAULT_BTN;

void revoco_set_widget_status(gboolean status)
{
	gtk_widget_set_sensitive(	GET_WIDGET(RADIO_REVOCO_FREE_CONFIG),
								status);
	gtk_widget_set_sensitive(	GET_WIDGET(RADIO_REVOCO_CLICK_CONFIG),
								status);
	gtk_widget_set_sensitive(	GET_WIDGET(RADIO_REVOCO_MANUAL_CONFIG),
								status);
	gtk_widget_set_sensitive(	GET_WIDGET(RADIO_REVOCO_AUTO_CONFIG),
								status);
	gtk_widget_set_sensitive(	GET_WIDGET(SPIN_REVOCO_BUTTON_CONFIG),
								status);
	gtk_widget_set_sensitive(	GET_WIDGET(SPIN_REVOCO_UP_CONFIG),
								status);
	gtk_widget_set_sensitive(	GET_WIDGET(SPIN_REVOCO_DOWN_CONFIG),
								status);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(CB_ENABLE_REVOCO_CONFIG)), status);
}

void revoco_force_disable(void)
{
	SET_LABEL_MARKUP(LABEL_REVOCO_STATUS_CONFIG, TEXT_REVOCO_STATUS_DISABLED);
	gtk_widget_set_sensitive(	GET_WIDGET(CB_ENABLE_REVOCO_CONFIG),
								FALSE);
	revoco_set_widget_status(FALSE);
}

void revoco_set_manual_mode(gboolean status)
{
	gtk_widget_set_sensitive(	GET_WIDGET(SPIN_REVOCO_BUTTON_CONFIG), 
								status);
}

void revoco_set_auto_mode(gboolean status)
{
	gtk_widget_set_sensitive(	GET_WIDGET(SPIN_REVOCO_UP_CONFIG),
								status);
	gtk_widget_set_sensitive(	GET_WIDGET(SPIN_REVOCO_DOWN_CONFIG),
								status);
}

void revoco_refresh_properties(void)
{
	/* Check if an MX Revolution has not been detected */
	if ((mouse_get_vendor_id() == REV_VENDOR && 
		(mouse_get_product_id() == REV_PRODUCT1 || 
		mouse_get_product_id() == REV_PRODUCT2))
		== 0)
	{
		revoco_force_disable();
		mouse_set_revoco_status(0);
		return;	
	}
	
	SET_LABEL_MARKUP(LABEL_REVOCO_STATUS_CONFIG, TEXT_REVOCO_STATUS_ENABLED);
	gtk_widget_set_sensitive(	GET_WIDGET(CB_ENABLE_REVOCO_CONFIG),
								TRUE);
								
	if (mouse_get_revoco_status() == REVOCO_DISABLED)
	{
		revoco_set_widget_status(FALSE);
	}
	else
	{
		revoco_set_widget_status(TRUE);	
	}
	
	switch (mouse_get_revoco_status())
	{
	case REVOCO_FREE_MODE:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_REVOCO_FREE_CONFIG)), TRUE);
		revoco_set_manual_mode(FALSE);
		revoco_set_auto_mode(FALSE);
		break;
	case REVOCO_CLICK_MODE:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_REVOCO_CLICK_CONFIG)), TRUE);
		revoco_set_manual_mode(FALSE);
		revoco_set_auto_mode(FALSE);
		break;
	case REVOCO_MANUAL_MODE:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_REVOCO_MANUAL_CONFIG)), TRUE);
		revoco_set_manual_mode(TRUE);
		revoco_set_auto_mode(FALSE);
		break;
	case REVOCO_AUTO_MODE:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_REVOCO_AUTO_CONFIG)), TRUE);
		revoco_set_manual_mode(FALSE);
		revoco_set_auto_mode(TRUE);
		break;
	}
	
	gtk_spin_button_set_value(	GTK_SPIN_BUTTON(GET_WIDGET(SPIN_REVOCO_BUTTON_CONFIG)),
								mouse_get_revoco_btn());
	gtk_spin_button_set_value(	GTK_SPIN_BUTTON(GET_WIDGET(SPIN_REVOCO_UP_CONFIG)),
								mouse_get_revoco_up_scroll());
	gtk_spin_button_set_value(	GTK_SPIN_BUTTON(GET_WIDGET(SPIN_REVOCO_DOWN_CONFIG)),
								mouse_get_revoco_down_scroll());
}

void revoco_init(void)
{
	revoco_force_disable();
	revoco_prev_mode = mouse_get_revoco_status();
	//gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_REVOCO_FREE_CONFIG)), TRUE);
}

void revoco_enabled_changed(gboolean enabled)
{
	if (enabled == TRUE)
	{
		if (revoco_prev_mode == REVOCO_DISABLED)
		{
			if (mouse_get_revoco_status() == REVOCO_DISABLED)
			{	
				mouse_set_revoco_status(REVOCO_FREE_MODE);
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GET_WIDGET(RADIO_REVOCO_FREE_CONFIG)), TRUE);
			}
		}
		else
			mouse_set_revoco_status(revoco_prev_mode);
	}
	else
	{
		mouse_set_revoco_status(REVOCO_DISABLED);	
	}
	revoco_refresh_properties();
}

void revoco_free_selected(void)
{
	revoco_prev_mode = REVOCO_FREE_MODE;
	mouse_set_revoco_status(REVOCO_FREE_MODE);
	revoco_set_manual_mode(FALSE);
	revoco_set_auto_mode(FALSE);
}

void revoco_click_selected(void)
{
	revoco_prev_mode = REVOCO_CLICK_MODE;
	mouse_set_revoco_status(REVOCO_CLICK_MODE);
	revoco_set_manual_mode(FALSE);
	revoco_set_auto_mode(FALSE);
}

void revoco_manual_selected(void)
{
	revoco_prev_mode = REVOCO_MANUAL_MODE;
	mouse_set_revoco_status(REVOCO_MANUAL_MODE);
	revoco_set_manual_mode(TRUE);
	revoco_set_auto_mode(FALSE);
}

void revoco_auto_selected(void)
{
	revoco_prev_mode = REVOCO_AUTO_MODE;
	mouse_set_revoco_status(REVOCO_AUTO_MODE);
	revoco_set_manual_mode(FALSE);
	revoco_set_auto_mode(TRUE);
}

void revoco_btn_changed(int value)
{
	int value_changed=0;
	if (value < 3)
	{
		value_changed = 1;
		value = 3;
	}
	else if (value > 13)
	{
		value_changed = 1;
		value = 13;	
	}
	else if (value == 10)
	{
		value_changed = 1;
		if (revoco_prev_btn < 10)
			value = 11;
		else
			value = 9;
	}
	else if (value == 12)
	{
		value_changed = 1;
		if (revoco_prev_btn < 12)
			value = 13;
		else
			value = 11;	
	}
	
	if (value_changed)
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(GET_WIDGET(SPIN_REVOCO_BUTTON_CONFIG)), value);
	mouse_set_revoco_btn(value);
	revoco_prev_btn = value;
}

void revoco_up_changed(int value)
{
	int value_changed=0;
	
	if (value < REVOCO_MIN_SCROLL)
	{
		value = REVOCO_MIN_SCROLL;
		value_changed = 1;	
	}
	else if (value > REVOCO_MAX_SCROLL)
	{
		value = REVOCO_MAX_SCROLL;
		value_changed = 1;
	}
	
	if (value_changed)
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(GET_WIDGET(SPIN_REVOCO_UP_CONFIG)), value);
	mouse_set_revoco_up_scroll(value);
}

void revoco_down_changed(int value)
{
	int value_changed=0;
	
	if (value < REVOCO_MIN_SCROLL)
	{
		value = REVOCO_MIN_SCROLL;
		value_changed = 1;	
	}
	else if (value > REVOCO_MAX_SCROLL)
	{
		value = REVOCO_MAX_SCROLL;
		value_changed = 1;
	}
	
	if (value_changed)
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(GET_WIDGET(SPIN_REVOCO_DOWN_CONFIG)), value);
	mouse_set_revoco_down_scroll(value);
}


