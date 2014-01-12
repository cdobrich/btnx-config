
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef MOUSED_H_
#define MOUSED_H_

typedef struct ev_handler
{
	int fd;
	int set_fd;
	int	index;
	char *path;
} ev_handler;

int moused_detecting(void);
void moused_start(void);
void moused_stop(void);
int moused_check_stop(void);
int moused_check_start(void);

int moused_check_handler_report(void);
int moused_report_handler(ev_handler *handler);

int moused_set_mouse(void);

#endif /*MOUSED_H_*/
