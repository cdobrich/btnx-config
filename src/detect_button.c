
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/* This file is a leftover from initial testing. Confirm and remove! */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include <linux/input.h>
#include <errno.h>

#define EVENT_PATH			"/dev/input/event"
#define MAX_EVENTS			20

#define CLEAR_CODE_TRAIL(i) ((i) & 0xFFFFFF00)
#define IS_MOUSE_X(i) (CLEAR_CODE_TRAIL(i) == 0x02000000)
#define IS_MOUSE_Y(i) (CLEAR_CODE_TRAIL(i) == 0x02000100)
#define IS_MOUSE_MOVE(i) (IS_MOUSE_X(i) || IS_MOUSE_Y(i))

#define CHAR2INT(c, x) (((int)(c)) << ((x) * 8))
#define INPUT_BUFFER_SIZE	512
#define MAX_RAWCODES	10
#define HEXDUMP_SIZE	16

typedef struct ev_handler
{
	int fd;
	int set_fd;
	int	index;
} ev_handler;

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

void clear_buffer(ev_handler *handlers)
{
	int i=0;
	unsigned char buffer[32];
	
	while (handlers[i].fd > 0)
	{
		while (read(handlers[i].fd, buffer, 32) > 0);
		i++;
	}
}

void get_rawcodes(ev_handler *handlers)
{
	int codes[128];
	fd_set fds;
	int ret, i, j;
	unsigned char buffer[INPUT_BUFFER_SIZE];
	int max_fd;
	
	memset(codes, 0, 128*sizeof(int));
	
	for (;;)
	{
		memset(buffer, '\0', INPUT_BUFFER_SIZE);
		FD_ZERO(&fds);
		max_fd = set_handlers(&fds, handlers, 0);
		select(max_fd+1, &fds, NULL, NULL, NULL);
			
		i=0;
		while (handlers[i].fd > 0)
		{
			if ((FD_ISSET(handlers[i].fd, &fds)) == 0)
			{
				i++;
				continue;
			}
			
			if ((ret = read(handlers[i].fd, buffer, INPUT_BUFFER_SIZE)) < 1)
			{
				i++;
				continue;
			}
			
			/*for (j=0; j<ret; j++)
			{
				printf("%02x", buffer[j]);
				if (j % 2 == 0)
					printf(" ");
			}*/
			
			//putchar('\n');
			
			for (j=0; (j < (ret / HEXDUMP_SIZE)) && (j < MAX_RAWCODES - 1); j++)
			{
				codes[j] = 	CHAR2INT(buffer[8 + j*HEXDUMP_SIZE], 3) | 
							CHAR2INT(buffer[11 + j*HEXDUMP_SIZE], 2) | 
							CHAR2INT(buffer[10 + j*HEXDUMP_SIZE], 1) | 
							CHAR2INT(buffer[13 + j*HEXDUMP_SIZE], 0);
				if (!IS_MOUSE_MOVE(codes[j]) && codes[j] != 0)
					printf("Caught %016x\n", codes[j]);
			}
			i++;
		}
		
	}
}

int detect(int argc, char *argv[])
{
	ev_handler handlers[MAX_EVENTS+1];
	char ev_path[32];
	int max_fd=0, fd, ready;
	fd_set fds;
	int i, handlers_size=0;
	struct timeval timeout = {1, 0};
	time_t start;
	int ignore=1;
	
	for (i=0; i <= MAX_EVENTS; i++)
	{
		sprintf(ev_path, "%s%d", EVENT_PATH, i);
		if ((fd = open(ev_path, O_RDONLY | O_NONBLOCK)) > 0)
		{
			printf("Found handler: %s\n", ev_path);
			handlers[handlers_size].fd = fd;
			handlers[handlers_size].index = i;
			handlers[handlers_size++].set_fd = 1;
		}
	}
	
	handlers[handlers_size].fd = -1;
	
	if (handlers_size == 0)
	{
		fprintf(stderr, "Error: could not find any event handlers.\n");
		exit(1);
	}
	
	// DEBUG
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	handlers[3].set_fd = 0;
	goto done;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	printf("Do not move your mouse for the next 5 seconds\n");
	start = time(NULL);
	sleep(3);
	clear_buffer(handlers);
	for (;;)
	{		
		FD_ZERO(&fds);
		max_fd = set_handlers(&fds, handlers, 1);
		ready = select(max_fd+1, &fds, NULL, NULL, &timeout);
		
		//printf("time: %d\n", time(NULL) - start);
		if ((time(NULL) - start) > 5 && ignore == 1)
		{
			printf("OK, move your mouse now.\n");
			ignore = 0;
		}
		
		if (ready < 0)
		{
			fprintf(stderr, "Error: select() failed: %s\n", strerror(errno));
			exit(1);
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
				printf("Using handler %s%d\n", EVENT_PATH, handlers[i].index);
				
				goto done;
			}
		}
	}
	
	done:
	get_rawcodes(handlers);
	
	return 0;
}




