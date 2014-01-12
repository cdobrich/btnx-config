
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  * 
  * Actual revoco code, distributed with btnx (not this program),
  * is not under the same copyright.
  */

#ifndef REVOCO_H_
#define REVOCO_H_

#include <gtk/gtk.h>

#define REVOCO_MIN_SCROLL		1
#define REVOCO_MAX_SCROLL		50

#define REVOCO_DEFAULT_SCROLL	5
#define REVOCO_DEFAULT_BTN		3

#define REV_VENDOR				0x046D
#define REV_PRODUCT1			0xC51A
#define REV_PRODUCT2			0xC525

enum
{
	REVOCO_DISABLED=0,
	REVOCO_FREE_MODE,
	REVOCO_CLICK_MODE,
	REVOCO_MANUAL_MODE,
	REVOCO_AUTO_MODE,
	REVOCO_INVALID_MODE
};

void revoco_init(void);
void revoco_refresh_properties(void);
void revoco_enabled_changed(gboolean enabled);
void revoco_free_selected(void);
void revoco_click_selected(void);
void revoco_manual_selected(void);
void revoco_auto_selected(void);
void revoco_btn_changed(int value);
void revoco_up_changed(int value);
void revoco_down_changed(int value);

#endif /*REVOCO_H_*/
