
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

#include "ip_pipe.h"
#include "widgets.h"
#include "moused.h"
#include "timers.h"
#include "handlers.h"
#include "threads.h"
#include "mouse.h"
#include "text.h"

#define EVENT_PATH			"/dev/input/event"
#define MAX_EVENTS			20

static pid_t pid_moused=-1;
static pid_t pid_parent=-1;

static int detecting=0;

int moused_detecting(void)
{
	return detecting;
}

int set_handlers(fd_set *fds, ev_handler *handlers, int on)
{
	int max_fd=0;
	int i=0;
	
	while (handlers[i].fd > 0)
	{
		if (handlers[i].set_fd == on)
		{
			FD_SET(handlers[i].fd, fds);
			if (handlers[i].fd > max_fd)
				max_fd = handlers[i].fd;
		}
		i++;
	}
	
	return max_fd;
}

static void moused_clear_handler_buffers(ev_handler *handlers)
{
	int i=0;
	unsigned char buffer[32];
	
	while (handlers[i].fd > 0)
	{
		while (read(handlers[i].fd, buffer, 32) > 0);
		i++;
	}
}

static void moused_close_child_handlers(ev_handler *handlers)
{
	int i=0;
	
	while (handlers[i].fd > 0)
	{
		close(handlers[i].fd);
		if (handlers[i].path != NULL)
			free(handlers[i].path);
		i++;
	}
}

static int moused_detect(void)
{
	ev_handler handlers[MAX_EVENTS+1];
	char ev_path[32];
	int max_fd=0, fd, ready;
	fd_set fds;
	int i, handlers_size=0;
	struct timeval timeout = {0, 100000};
	time_t start;
	int ignore=1;
	unsigned short id[6];
	//ip_pipe_buffer pipe_buffer;
	
	for (i=0; i <= MAX_EVENTS; i++)
	{
		sprintf(ev_path, "%s%d", EVENT_PATH, i);
		handlers[handlers_size].path = NULL;
		if ((fd = open(ev_path, O_RDONLY | O_NONBLOCK)) > 0)
		{
			ioctl(fd, EVIOCGID, id);
			if (id[ID_VENDOR] == 0xB216)
			{
				fprintf(stderr, "Found a btnx handler. Ignoring it.\n");
				close(fd);
				continue;
			}
			//fprintf("Found handler: %s\n", ev_path);
			handlers[handlers_size].fd = fd;
			handlers[handlers_size].index = i;
			handlers[handlers_size].set_fd = 1;
			handlers[handlers_size].path = (char *) malloc((strlen(ev_path) + 1)*sizeof(char));
			strcpy(handlers[handlers_size].path, ev_path);
			handlers_size++;
		}
	}
	
	handlers[handlers_size].fd = -1;
	
	if (handlers_size == 0)
	{
		fprintf(stderr, "Error: could not find any event handlers.\n");
		_exit(-1);
	}
	
	//g_print("Do not move your mouse for the next 5 seconds\n");
	start = time(NULL);
	while ((time(NULL) - start) < 3)
	{
		if (moused_check_stop() > 0)
			goto done;
		usleep(1000);
	}
	moused_clear_handler_buffers(handlers);
	
	for (;;)
	{
		if (moused_check_stop() > 0)
			break;
			
		FD_ZERO(&fds);
		max_fd = set_handlers(&fds, handlers, 1);
		ready = select(max_fd+1, &fds, NULL, NULL, &timeout);
		
		//printf("time: %d\n", time(NULL) - start);
		if ((time(NULL) - start) > 5 && ignore == 1)
		{
			//g_print("OK, move your mouse now.\n");
			ignore = 0;
			ip_pipe_send_header(IP_START_MOUSED, pid_parent);
		}
		
		if (ready < 0)
		{
			fprintf(stderr, "Error: select() failed: %s\n", strerror(errno));
			_exit(-1);
		}
		else if (ready == 0)
		{
			//printf("Timeout reached with no events. Quitting\n");
			//exit(0);
		}
		for (i=0; i<handlers_size; i++)
		{
			if (FD_ISSET(handlers[i].fd, &fds) && handlers[i].set_fd != 0)
			{
				handlers[i].set_fd = 0;
				//g_print("Using handler %s%d\n", EVENT_PATH, handlers[i].index);
				moused_report_handler(&(handlers[i]));
				//goto done;
			}
		}
	}
	
done:
	//g_print("Detection done.\n");
	
	moused_close_child_handlers(handlers);
	
	return 0;
}

void moused_start(void)
{
	pid_t parent = getpid();
	
	handlers_free();
	
	if ((pid_moused = threads_fork_with_pipe(&parent)) == 0)
	{
		pid_parent = parent;
		moused_detect();
		_exit(0);
	}
	else if (pid_moused == -1)
	{
		fprintf(stderr, "moused_start fork failed: %s.\n", strerror(errno));
		fprintf(stderr, "No sense in continuing. Quitting.\n");
		exit(1);
	}
	
	detecting = 1;
	g_timeout_add_full(	G_PRIORITY_HIGH_IDLE,
					MOUSED_CHECK_START_INTERVAL,
					(GSourceFunc)moused_check_start_timer,
					NULL, NULL);
}

void moused_stop(void)
{
	if (detecting == 1)
		ip_pipe_send_header(IP_STOP_MOUSED, pid_moused);
	detecting = 0;
}

int moused_check_stop(void)
{
	return ip_pipe_wait_header(10, IP_STOP_MOUSED, pid_parent);
}

int moused_check_start(void)
{
	return ip_pipe_wait_header(10, IP_START_MOUSED, pid_moused);
}


void moused_add_handler(char *path)
{
	int ret=0;
	char handler_text[32];
	char product_text[16], vendor_text[16];
	GtkWidget *dialog;
	if ((ret = handlers_add(path)) < 0)
	{
		fprintf(stderr, "Error: moused_add_handler failed at handler addition.\n");
		return;
	}
	if (handlers_check_id_conflicts() < 0)
	{
		fprintf(stderr, "Error: moused_add_handler id conflict detected.\n");
		moused_stop();
		SHOW_WARNING_DIALOG(dialog, GET_WIDGET(WIN_MOUSED),
							TEXT_MOUSED_ID_CONFLICT_TITLE,
							TEXT_MOUSED_ID_CONFLICT_BODY);
		init_moused();
		return;
	}
	sprintf(handler_text, TEXT_DETECTION_STATUS, handlers_count());
	sprintf(product_text, "0x%04x", handlers_get_product(ret));
	sprintf(vendor_text, "0x%04x", handlers_get_vendor(ret));
	SET_LABEL_TEXT(LABEL_HANDLERS_MOUSED, handler_text);
	SET_LABEL_TEXT(LABEL_PRODUCTIDVAL_MOUSED, product_text);
	SET_LABEL_TEXT(LABEL_VENDORIDVAL_MOUSED, vendor_text);
	gtk_widget_set_sensitive(GET_WIDGET(BTN_FORWARD_MOUSED), TRUE);
	gtk_widget_set_sensitive(GET_WIDGET(BTN_UNDO_MOUSED), TRUE);
}

int moused_report_handler(ev_handler *handler)
{
	ip_pipe_buffer *buffer;
	int len;
	
	buffer = ip_pipe_buffer_init();
	
	len = 3 + strlen(handler->path);
	
	if (ip_pipe_buffer_set(buffer, len) != len)
	{
		fprintf(stderr, "Error: moused_report_handler failed buffer set: %s\n", strerror(errno));
		return -1;
	}
	
	buffer->data[0] = IP_REPORT_HANDLER;
	buffer->data[1] = handler->index;
	buffer->data[2] = '\0';
	strcat(buffer->data, handler->path);
	buffer->len = len;
	if (ip_pipe_write(IP_PIPE_NONBLOCK, buffer, pid_parent) < 1)
		fprintf(stderr, "Error: handler report write error\n");
	ip_pipe_buffer_free(buffer);
	
	return 1;
}

int moused_check_handler_report(void)
{
	ip_pipe_buffer *buffer;
	buffer = ip_pipe_buffer_init();
	
	if (ip_pipe_wait_header_data(10, IP_REPORT_HANDLER, buffer, pid_moused) == 1)
	{
		//g_print("got handler: %s\n", &(buffer->data[2]));
		moused_add_handler(&(buffer->data[2]));
		ip_pipe_buffer_free(buffer);
		return 1;
	}
	ip_pipe_buffer_free(buffer);
	return 0;
}

int moused_set_mouse(void)
{
	int vendor_id, product_id;
	if ((vendor_id = handlers_get_vendor(0)) == 0 || (product_id = handlers_get_product(0)) == 0)
	{
		fprintf(stderr, "Error: moused_set_mouse could not get valid IDs. Cannot continue.\n");
		return -1;
	}
	mouse_set_vendor_id(vendor_id);
	mouse_set_product_id(product_id);
	return 1;
}

