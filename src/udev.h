
 /*
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#ifndef UDEV_H_
#define UDEV_H_

//#define UDEV_PATH	"/etc/udev/rules.d/99-btnx.rules"

void udev_enabled_set(int enabled);
void udev_sysfs_enabled_set(int enabled);
int udev_enabled_get(void);
int udev_sysfs_enabled_get(void);
int udev_create(void);
void udev_init(void);

#endif /*UDEV_H_*/
