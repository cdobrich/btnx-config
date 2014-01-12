
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <stdlib.h>
#include <string.h>

#include "mouse.h"
#include "revoco.h"

static mouse_data mouse;

void mouse_init(void)
{
	mouse.product_id = 0;
	mouse.vendor_id = 0;
	mouse.vendor_name = NULL;
	mouse.product_name = NULL;
	
	mouse.revoco_status = REVOCO_DISABLED;
	mouse.revoco_btn = REVOCO_DEFAULT_BTN;
	mouse.revoco_up_scroll = REVOCO_DEFAULT_SCROLL;
	mouse.revoco_down_scroll = REVOCO_DEFAULT_SCROLL;
}

void mouse_free(void)
{
	mouse.product_id = 0;
	mouse.vendor_id = 0;
	mouse_set_vendor_name(NULL);
	mouse_set_product_name(NULL);
	
	mouse.revoco_status = REVOCO_DISABLED;
	mouse.revoco_btn = REVOCO_DEFAULT_BTN;
	mouse.revoco_up_scroll = REVOCO_DEFAULT_SCROLL;
	mouse.revoco_down_scroll = REVOCO_DEFAULT_SCROLL;
}

void mouse_set_product_id(int id)
{
	mouse.product_id = id;
}

void mouse_set_vendor_id(int id)
{
	mouse.vendor_id = id;
}

void mouse_set_vendor_name(const char *name)
{
	if (mouse.vendor_name != NULL)
		free(mouse.vendor_name);
	if (name != NULL)
	{
		mouse.vendor_name = (char *) malloc((strlen(name)+1) *sizeof(char));
		strcpy(mouse.vendor_name, name);
	}
	else
		mouse.vendor_name = NULL;
}

void mouse_set_product_name(const char *name)
{
	if (mouse.product_name != NULL)
		free(mouse.product_name);
	if (name != NULL)
	{
		mouse.product_name = (char *) malloc((strlen(name)+1) *sizeof(char));
		strcpy(mouse.product_name, name);
	}
	else
		mouse.product_name = NULL;
}

void mouse_set_revoco_status(int status)
{
	if (status < 0 || status >= REVOCO_INVALID_MODE)
		return;
	mouse.revoco_status = status;
}

void mouse_set_revoco_btn(int btn)
{
	if (btn < 3 || btn > 13)
		return;
	if (btn == 10 || btn == 12)
		return;
	mouse.revoco_btn = btn;
}

void mouse_set_revoco_up_scroll(int speed)
{
	if (speed < 1 || speed > 50)
		return;
	mouse.revoco_up_scroll = speed;	
}

void mouse_set_revoco_down_scroll(int speed)
{
	if (speed < 1 || speed > 50)
		return;
	mouse.revoco_down_scroll = speed;	
}

int mouse_get_vendor_id(void)
{
	return mouse.vendor_id;
}

int mouse_get_product_id(void)
{
	return mouse.product_id;
}

const char *mouse_get_vendor_name(void)
{
	return mouse.vendor_name;
}

const char *mouse_get_product_name(void)
{
	return mouse.product_name;
}

int mouse_get_revoco_status(void)
{
	return mouse.revoco_status;	
}

int mouse_get_revoco_btn(void)
{
	return mouse.revoco_btn;	
}

int mouse_get_revoco_up_scroll(void)
{
	return mouse.revoco_up_scroll;	
}

int mouse_get_revoco_down_scroll(void)
{
	return mouse.revoco_down_scroll;	
}
