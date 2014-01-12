
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "handlers.h"

#define NUM_HANDLER_LOCATIONS	3
const char handler_locations[][15] =
{
	{"/dev"},
	{"/dev/input"},
	{"/dev/misc"}
};

static handler **handlers=NULL;
static int num_handlers=0;

int handlers_add(char *path)
{
	int fd;
	unsigned short id[6];
	
	if ((fd = open(path, O_RDONLY)) < 0)
	{
		fprintf(stderr, "Error: handler_add event handler open failed: %s: %s\n", path, strerror(errno));
		return -1;
	}
	
	if (num_handlers == 0)
	{
		handlers = (struct handler **) malloc(sizeof(handler*));
	}
	else
	{
		handlers = (handler **) realloc(handlers, (num_handlers+1)*sizeof(handler*));
	}
	
	handlers[num_handlers] = (handler *) malloc(sizeof(handler));
	
	if (handlers[num_handlers] == NULL || handlers == NULL)
	{
		fprintf(stderr, "Error, handler_add handler allocation failed: %s\n", strerror(errno));
		close(fd);
		return -1;
	}
	
	ioctl(fd, EVIOCGID, id);
	handlers[num_handlers]->vendor = id[ID_VENDOR];
	handlers[num_handlers]->product = id[ID_PRODUCT];
	handlers[num_handlers]->fd = -1;
	handlers[num_handlers]->open = 0;
	handlers[num_handlers]->path = (char *) malloc((strlen(path)+1) * sizeof(char));
	handlers[num_handlers]->vendor_name = NULL;
	handlers[num_handlers]->product_name = NULL;
	
	if (handlers[num_handlers]->path == NULL)
	{
		fprintf(stderr, "Error: handler_add path allocation failed: %s\n", strerror(errno));
		close(fd);
		free(handlers[num_handlers]);
		return -1;
	}
	
	strcpy(handlers[num_handlers]->path, path);
	
	close(fd);
	num_handlers++;
	return num_handlers-1;
}

const char *handlers_get_location(int index)
{
	if (index < 0 || index > NUM_HANDLER_LOCATIONS - 1)
		return NULL;
	
	return handler_locations[index];
}

int handlers_add_by_ids(int vendor, int product)
{
	const char *loc;
	int x=0, i=0, fd;
	char loc_buffer[128];
	unsigned short id[6];
	
	while ((loc = handlers_get_location(x++)) != NULL)
	{
		for (i=0; i<20; i++)
		{
			sprintf(loc_buffer, "%s/%s%d", loc, "event", i);
			if ((fd = open(loc_buffer, O_RDONLY)) >= 0)
			{
				//printf("Opened handler: %s\n", loc_buffer);
				ioctl(fd, EVIOCGID, id);
				if (vendor == id[ID_VENDOR] && product == id[ID_PRODUCT])
					handlers_add(loc_buffer);
				close(fd);
			}
		}
	}
	return num_handlers;
}

void handlers_free(void)
{
	int i;
	
	if (handlers != NULL && num_handlers > 0)
	{
		for (i=0; i<num_handlers; i++)
		{
			if (handlers[i] != NULL)
			{
				if (handlers[i]->path != NULL)
				{
					free(handlers[i]->path);
				}
				if (handlers[i]->vendor_name != NULL)
				{
					free(handlers[i]->vendor_name);
				}
				if (handlers[i]->product_name != NULL)
				{
					free(handlers[i]->product_name);
				}
				if (handlers[i]->open == 1 && handlers[i]->fd >=0)
				{
					close(handlers[i]->fd);
				}
				free(handlers[i]);
			}
		}
		free(handlers);
	}
	num_handlers = 0;
	handlers = NULL;
}


int handlers_count(void)
{
	return num_handlers;
}

int handlers_check_index(int index)
{
	if (index < num_handlers && index >= 0)
		return index;
	//fprintf(stderr, "Error: invalid index(%d) passed to handlers_check_index.\n", index);
	return -1;
}

int handlers_get_product(int index)
{
	if (handlers_check_index(index) >= 0)
		return handlers[index]->product;
	return 0;
}

int handlers_get_vendor(int index)
{
	if (handlers_check_index(index) >= 0)
		return handlers[index]->vendor;
	return 0;
}

int handlers_get_fd(int index)
{
	if (handlers_check_index(index) >= 0)
		return handlers[index]->fd;
	return 0;
}

char *handlers_get_vendor_name(int index)
{
	if (handlers_check_index(index) >= 0)
		return handlers[index]->vendor_name;
	return 0;
}

char *handlers_get_product_name(int index)
{
	if (handlers_check_index(index) >= 0)
		return handlers[index]->product_name;
	return 0;
}

int handlers_open(int index)
{
	if (handlers[index]->open == 1)
		return handlers[index]->fd;
	if ((handlers[index]->fd = open(handlers[index]->path, O_RDONLY)) < 0)
	{
		fprintf(stderr, "Error: handler_open event handler open failed: %s\n", strerror(errno));
		return -1;
	}
	handlers[index]->open = 1;
	return handlers[index]->fd;
}


int handlers_open_all(void)
{
	int i;
	for (i=0; i<num_handlers; i++)
	{
		if (handlers_open(i) < 0)
			return -1;
	}
	return 0;
}

void handlers_close(int index)
{
	if (handlers_check_index(index) >= 0)
	{
		if (handlers[index]->open == 1)
			close(handlers[index]->fd);
		handlers[index]->open = 0;
		handlers[index]->fd = -1;
	}
}

void handlers_close_all(void)
{
	int i;
	for (i=0; i<num_handlers; i++)
	{
		handlers_close(i);
	}
}

int handlers_check_id_conflicts(void)
{
	int product, vendor, i;
	
	if (num_handlers < 1)
		return 0;
	product = handlers[0]->product;
	vendor = handlers[0]->vendor;
	
	for (i=0; i<num_handlers; i++)
	{
		if (handlers[i]->vendor != vendor || handlers[i]->product != product)
			return -1;
	}
	return 0;
}

int handlers_zero_and_fill_fds(fd_set *fds)
{
	int i, max_fd=-1;
	FD_ZERO(fds);
	for (i=0; i<num_handlers; i++)
	{
		if (handlers[i]->open == 1)
		{
			FD_SET(handlers[i]->fd, fds);
			if (handlers[i]->fd > max_fd)
				max_fd = handlers[i]->fd;
		}
	}
	return max_fd;
}

int handlers_isset(fd_set *fds, int i)
{
	if (handlers_check_index(i) == -1)
		return -1;
	if (handlers[i]->fd < 0 || handlers[i]->open == 0)
		return 0;
	
	return FD_ISSET(handlers[i]->fd, fds);
}


