
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <gtk/gtk.h>

#include "udev.h"
#include "mouse.h"
#include "common.h"

#define UDEV_RULE \
"BUS==\"usb\", SYSFS{idVendor}==\"%04x\", SYSFS{idProduct}==\"%04x\", \
ACTION==\"add\", RUN+=\"" INIT_SCRIPT_FILE " start\", WAIT_FOR_SYSFS=\"bus\""
/* "\n\
BUS==\"usb\", SYSFS{idVendor}==\"%04x\", SYSFS{idProduct}==\"%04x\", \
ACTION==\"remove\", RUN+=\"" INIT_SCRIPT_FILE " stop\""*/

#define UDEV_RULE_NO_SYSFS \
"BUS==\"usb\", SYSFS{idVendor}==\"%04x\", SYSFS{idProduct}==\"%04x\", \
ACTION==\"add\", RUN+=\"" INIT_SCRIPT_FILE " start\""
/* "\n\
BUS==\"usb\", SYSFS{idVendor}==\"%04x\", SYSFS{idProduct}==\"%04x\", \
ACTION==\"remove\", RUN+=\"" INIT_SCRIPT_FILE " stop\""*/

static int udev_enabled=1;
static int sysfs_enabled=0;

void udev_enabled_set(int enabled)
{
	udev_enabled = enabled;
}

void udev_sysfs_enabled_set(int enabled)
{
	sysfs_enabled = enabled;
}

int udev_enabled_get(void)
{
	return udev_enabled;
}

int udev_sysfs_enabled_get(void)
{
	return sysfs_enabled;
}

int udev_create(void)
{
	FILE *fp;
	gchar *udev_rule=NULL;
	
	if (udev_enabled == 0)
	{
		unlink(UDEV_RULES_FILE);
		return 0;
	}
		
	fp = fopen(UDEV_RULES_FILE, "w");
	if (fp == NULL)
	{
		fprintf(stderr, "Error: could not make a udev rule: %s\n", strerror(errno));
		return -1;
	}
	
	if (sysfs_enabled == 0)
	{
		udev_rule = g_strdup_printf(UDEV_RULE_NO_SYSFS, 
			mouse_get_vendor_id(), mouse_get_product_id(),
			mouse_get_vendor_id(), mouse_get_product_id());
	}
	else
	{
		udev_rule = g_strdup_printf(UDEV_RULE, 
			mouse_get_vendor_id(), mouse_get_product_id(),
			mouse_get_vendor_id(), mouse_get_product_id());
	}
	
	if (fputs(udev_rule, fp) == EOF)
	{
		fprintf(stderr, "Error: could not write to udev file: %s\n", strerror(errno));
		g_free(udev_rule);
		fclose(fp);
		return -1;
	}
	
	fclose(fp);
	
	if (udev_rule != NULL)
		g_free(udev_rule);
	return 0;
}

void udev_init(void)
{
	udev_enabled = 1;
	sysfs_enabled = 0;
}

