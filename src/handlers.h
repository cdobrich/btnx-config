
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#ifndef HANDLERS_H_
#define HANDLERS_H_

#include <sys/select.h>

typedef struct handler
{
	char *path;
	char *vendor_name;
	char *product_name;
	int vendor;
	int product;
	int open;
	int fd;
} handler;

int handlers_add(char *path);
int handlers_add_by_ids(int vendor, int product);
void handlers_free(void);
int handlers_count(void);
int handlers_get_product(int index);
int handlers_get_vendor(int index);
int handlers_get_fd(int index);
char *handlers_get_vendor_name(int index);
char *handlers_get_product_name(int index);
int handlers_open(int index);
int handlers_open_all(void);
void handlers_close(int index);
void handlers_close_all(void);
int handlers_check_id_conflicts(void);
int handlers_zero_and_fill_fds(fd_set *fds);
int handlers_isset(fd_set *fds, int i);

#endif /*HANDLERS_H_*/
