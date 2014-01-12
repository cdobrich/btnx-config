
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>

#include "ip_pipe.h"

static ip_pipe **ip_pipes=NULL;
static int num_pipes=0;
static int rw_on=0;
static int freeing=0;
static int *pids_unhandled=NULL;
static int num_unhandled=0;

int ip_pipe_init(pid_t pid)
{
	int pfd[2];
	
	if (pipe(pfd) == -1)
	{
		fprintf(stderr, "Error: ip_pipe_init failed: %s\n", strerror(errno));
		return -1;
	}
	
	if (num_pipes == 0)
	{
		ip_pipes = (ip_pipe **) malloc(sizeof(ip_pipe*));
		if (ip_pipes == NULL)
		{
			fprintf(stderr, "Error: ip_pipe_init malloc failed: %s\n",
					strerror(errno));
		}
		ip_pipes[0] = NULL;
	}
	else
	{
		ip_pipes = (ip_pipe **) realloc(ip_pipes, 
										(num_pipes+1)*sizeof(ip_pipe*));
		if (ip_pipes == NULL)
		{
			fprintf(stderr, "Error: ip_pipe_init realloc failed: %s\n",
					strerror(errno));
		}
		ip_pipes[num_pipes] = NULL;
	}
	
	ip_pipes[num_pipes] = (ip_pipe *) malloc(sizeof(ip_pipe));
	if (ip_pipes[num_pipes] == NULL)
	{
		fprintf(stderr, 
				"Error: ip_pipes_init could not allocate a new pipe: %s\n", 
				strerror(errno));
		return -1;
	}
	
	ip_pipes[num_pipes]->pid_to = pid;
	ip_pipes[num_pipes]->pid_from = -1;
	ip_pipes[num_pipes]->rpipe = pfd[0];
	ip_pipes[num_pipes]->wpipe = pfd[1];
	ip_pipes[num_pipes]->pipe_max = ip_pipes[num_pipes]->rpipe;
	if (ip_pipes[num_pipes]->wpipe > ip_pipes[num_pipes]->pipe_max) 
		ip_pipes[num_pipes]->pipe_max = ip_pipes[num_pipes]->wpipe;
	
	num_pipes++;
	
	return num_pipes-1;
}

void ip_pipe_free(void)
{
	int i;
	
	if (ip_pipes != NULL)
	{
		for (i=0; i<num_pipes; i++)
		{
			if (ip_pipes[i] != NULL)
			{
				close(ip_pipes[i]->rpipe);
				close(ip_pipes[i]->wpipe);
				free(ip_pipes[i]);
			}
			ip_pipes[i] = NULL;
		}
		free(ip_pipes);
	}
	ip_pipes = NULL;
	num_pipes = 0;
}

int ip_pipe_free_bypid(pid_t pid_child)
{
	int index_to, index_from, i, j;
	
	freeing = 1;
	
	if (pid_child == -1)
	{
		fprintf(stderr, "Error: ip_pipe_free_bypid invalid pid: -1\n");
		freeing = 0;
		return -1;
	}
	
	index_to = ip_pipe_get_bypid_both(pid_child, getpid());
	index_from = ip_pipe_get_bypid_both(getpid(), pid_child);
	
	if (rw_on == 1) // more pid specific checking
		return -1;
	
	if (index_to == -1 || index_from == -1)
	{
		/*fprintf(stderr, "Error: ip_pipe_free_bypid failed, could not get \
ip_pipe indeces.\n");*/
		freeing = 0;
		return -1; // better handling, when called from handle_old_pids
	}
	
	for (j=0; j<2; j++)
	{
		if (j==0) i=index_to;
		else i=index_from;
		if (ip_pipes[i] != NULL)
		{
			if (close(ip_pipes[i]->rpipe) < 0)
			{
				fprintf(stderr,
						"Error: ip_pipe_free_bypid closing1 failed: %s\n",
						strerror(errno));
			}
			if (close(ip_pipes[i]->wpipe)  < 0)
			{
				fprintf(stderr,
						"Error: ip_pipe_free_bypid closing2 failed: %s\n",
						strerror(errno));
			}
			free(ip_pipes[i]);
		}
		ip_pipes[i] = NULL;
		
		for (i+=1; i<num_pipes; i++)
		{
			//printf("ip_pipes switching %d to %d, num:%d\n", i, i-1, num_pipes);
			ip_pipes[i-1] = ip_pipes[i];
		}
		ip_pipes[num_pipes-1] = NULL;
	}
	
	num_pipes -= 2;
	if (num_pipes == 0)
	{
		if (ip_pipes != NULL)
			free(ip_pipes);
		ip_pipes = NULL;
	}
	else
	{
		ip_pipes = (ip_pipe **) realloc(ip_pipes, 
										num_pipes * sizeof(ip_pipe *));
		if (ip_pipes == NULL)
		{
			fprintf(stderr,
					"Error: ip_pipes_free_bypid end realloc failed: %s\n",
					strerror(errno));					
		}
	}
	
	freeing = 0;
	return 1;
}

int ip_pipe_flag_pid_old(pid_t pid_child)
{
	if (num_unhandled == 0)
	{
		pids_unhandled = (int *) malloc(sizeof(int));
		if (pids_unhandled == NULL)
		{
			fprintf(stderr, "Error: could not alloc space for pids_unhandled.\n");
			return -1;
		}
	}
	else
	{
		if ((pids_unhandled = (int *) realloc(pids_unhandled,
			(num_unhandled+1)*sizeof(int))) == NULL)
		{
			fprintf(stderr, "Error: could not realloc space for pids_unhandled.\n");
			return -1;
		}
	}
	pids_unhandled[num_unhandled] = pid_child;
	num_unhandled++;
	return 1;
}

int ip_pipe_handle_old_pids(void)
{
	int i, j, ret=1;
	
	if (freeing == 1)
		return 0;
	
	//printf("ip_pipe_handle_old_pids called\n");
	
	for (i=0; i<num_unhandled; i++)
	{
		if (ip_pipe_free_bypid(pids_unhandled[i]) > 0)
		{
			for (j=i+1; j<num_unhandled; j++)
			{
				pids_unhandled[j-1] = pids_unhandled[j];
			}
			pids_unhandled[num_unhandled-1] = 0;
			pids_unhandled = (int *) realloc(pids_unhandled,
											(num_unhandled-1)*sizeof(int));
			num_unhandled --;
		}
		else
			ret = 0;
	}
	return ret;
}

int ip_pipe_get_bypid(	pid_t	pid_to,
						pid_t	pid_from,
						int		bypid)
{
	int i;
	
	for (i=0; i<num_pipes; i++)
	{
		if (bypid == IP_PIPE_PID_FROM)
		{
			if (ip_pipes[i]->pid_from == pid_from)
				return i;
		}
		else if (bypid == IP_PIPE_PID_TO)
		{
			if (ip_pipes[i]->pid_to == pid_to)
				return i;
		}
		else if (bypid == IP_PIPE_PID_BOTH)
		{
			if (ip_pipes[i]->pid_to == pid_to && 
				ip_pipes[i]->pid_from == pid_from)
				return i;
		}
	}
	return -1;
}

int ip_pipe_link_parent(pid_t child)
{
	int pp_i, pc_i;
	
	pp_i = ip_pipe_get_bypid_both(getpid(), -1);
	pc_i = ip_pipe_get_bypid_both(-1, -1);
	if (pp_i < 0 || pc_i < 0)
	{
		fprintf(stderr, "Error: no pipe bypid found.\n");
		return -1;
	}
	
	ip_pipes[pp_i]->pid_from = child;
	ip_pipes[pc_i]->pid_to = child;
	ip_pipes[pc_i]->pid_from = getpid();
	
	//while (ip_pipe_wait_header(IP_PIPE_BLOCK, IP_ACK, child) < 1);
	if (ip_pipe_wait_header(2000, IP_ACK, child) < 1)
	{
		fprintf(stderr,
				"Error: ip_pipe_link_parent IP_ACK wait timeout.\n");
		return -1;
	}
	
	//printf("parent link done\n");
	return 1;
}

int ip_pipe_link_child(pid_t parent)
{
	int pp_i, pc_i;
	
	pp_i = ip_pipe_get_bypid_both(parent, -1);
	pc_i = ip_pipe_get_bypid_both(-1, -1);
	if (pp_i < 0 || pc_i < 0)
	{
		fprintf(stderr, "Error: no pipe bypid found.\n");
		return -1;
	}
	
	ip_pipes[pp_i]->pid_from = getpid();
	ip_pipes[pc_i]->pid_to = getpid();
	ip_pipes[pc_i]->pid_from = parent;
	
	ip_pipe_send_header(IP_ACK, parent);
	
	//printf("child link done\n");
	return 1;
}

ip_pipe_buffer *ip_pipe_buffer_init(void)
{
	ip_pipe_buffer *buffer = (ip_pipe_buffer *) malloc(sizeof(ip_pipe_buffer));
	buffer->data = NULL;
	buffer->len = 0;
	return buffer;
}

int ip_pipe_buffer_set(ip_pipe_buffer *buffer, int size)
{
	if (buffer == NULL)
	{
		buffer = ip_pipe_buffer_init();
	}
	if (size > 0)
	{
		if (buffer->data != NULL)
			free(buffer->data);
		buffer->data = (char *) malloc(size * sizeof(char));
		if (!(buffer->data))
		{
			fprintf(stderr,
					"Error: ip_pipe_buffer_set malloc failed: %s\n",
					strerror(errno));
			return IP_PIPE_ERROR;
		}
	}
	return size;
}

void ip_pipe_buffer_unset(ip_pipe_buffer *buffer)
{
	if (buffer->data != NULL)
		free(buffer->data);
	buffer->data = NULL;
}

void ip_pipe_buffer_free(ip_pipe_buffer *buffer)
{
	ip_pipe_buffer_unset(buffer);
	if (buffer != NULL)
		free(buffer);
}

int ip_pipe_wait(	int		timeout,
					int 	rw,
					ip_pipe	*sel_pipe)
{
	static fd_set fds[2];
	struct timeval *tv=NULL;
	int ret;
	
	if (timeout != IP_PIPE_NONBLOCK)
	{
		tv = (struct timeval *) malloc(sizeof(struct timeval));
		if (!tv)
		{
			fprintf(stderr,
					"Error: ip_pipe_wait malloc failed: %s\n",
					strerror(errno));
			exit(1);
		}
		if (timeout == IP_PIPE_BLOCK)
		{
			tv->tv_sec = 0;
			tv->tv_usec = 0;
		}
		else
		{
			tv->tv_sec = timeout/1000;
			timeout = timeout - tv->tv_sec*1000;
			tv->tv_usec = timeout*1000;
		}
	}
	
	FD_ZERO(&(fds[0])); FD_ZERO(&(fds[1]));
	if ((rw & IP_PIPE_R))
		FD_SET(sel_pipe->rpipe, &(fds[0]));
	if ((rw & IP_PIPE_W))
		FD_SET(sel_pipe->wpipe, &(fds[1]));
		
	if (rw == IP_PIPE_R)
		ret = select(sel_pipe->rpipe+1, &(fds[0]), NULL, NULL, tv);
	else if (rw == IP_PIPE_W)
		ret = select(sel_pipe->wpipe+1, NULL, &(fds[1]), NULL, tv);
	else
		ret = select(sel_pipe->pipe_max, &(fds[0]), &(fds[1]), NULL, tv);
		
	if (tv != NULL)
		free(tv);

	//printf("wait ret: %d\n", ret);

	if (ret == 0)
		return IP_PIPE_NULL_SET;
	else if (ret == 1 && FD_ISSET(sel_pipe->rpipe, &(fds[0])))
		return IP_PIPE_R_SET;
	else if (ret == 1 && FD_ISSET(sel_pipe->wpipe, &(fds[1])))
		return IP_PIPE_W_SET;
	else if (ret == 2)
		return IP_PIPE_RW_SET;
	else
	{
		fprintf(stderr,
				"Error: ip_pipe_wait select error: %s\n",
				strerror(errno));
		fprintf(stderr,
				"rpipe %d wpipe %d\n",
				sel_pipe->rpipe,
				sel_pipe->wpipe);
		return IP_PIPE_ERROR;
	}
}

ip_pipe_buffer *ip_pipe_read(	int				timeout,
								ip_pipe_buffer	*buffer,
								pid_t			pid)
{
	int ret;
	char data[IP_PIPE_DATA_BUFFER_MAX];
	int pipe_i = ip_pipe_get_bypid_both(getpid(), pid);
	
	rw_on = 1;
	
	if (pipe_i == -1)
	{
		fprintf(stderr,
				"Warning: no valid pipes from pid: %d\n",
				pid);
		rw_on = 0;
		return NULL;
	}
	
	//printf("reading from %d\n", ip_pipes[pipe_i].rpipe);
	
	if (ip_pipe_wait(timeout, IP_PIPE_R, ip_pipes[pipe_i]) != IP_PIPE_R_SET)
	{
		rw_on = 0;
		return NULL;
	}
	//printf("trying to read\n");
	
	ret = read(ip_pipes[pipe_i]->rpipe, data, IP_PIPE_DATA_BUFFER_MAX);
	if (ret <= 0)
	{
		fprintf(stderr,
				"Error: ip_pipe_read read error: %s\n",
				strerror(errno));
		rw_on = 0;
		return NULL;
	}
	
	if (buffer->data != NULL)
		free(buffer->data);
	
	//printf("read %d\n", ret);
	buffer->data = (char *) malloc(ret*sizeof(char));
	memcpy(buffer->data, data, ret);
	buffer->len = ret;
	
	rw_on = 0;
	return buffer;
}


int ip_pipe_write(	int				timeout,
					ip_pipe_buffer	*buffer,
					pid_t			pid)
{
	int len_wrote=0;
	int ret;
	int pipe_i = ip_pipe_get_bypid_both(pid, getpid());
	
	rw_on = 1;
	
	if (pipe_i == -1)
	{
		fprintf(stderr,
				"Warning: no valid pipes to pid: %d\n",
				pid);
		rw_on = 0;
		return IP_PIPE_ERROR;
	}

	if (buffer->data == NULL || buffer->len <= 0)
	{
		rw_on = 0;
		return IP_PIPE_ERROR;
	}
	
	while (len_wrote < buffer->len)
	{
		if (len_wrote != 0)
			fprintf(stderr, "ip_pipe_write trying to write again. wrote: %d, len:%d\n", len_wrote, buffer->len);
		if (ip_pipe_wait(timeout, IP_PIPE_W, ip_pipes[pipe_i]) != IP_PIPE_W_SET)
		{
			rw_on = 0;
			return IP_PIPE_NULL_SET;
		}
		if ((ret = write(	ip_pipes[pipe_i]->wpipe,
							buffer->data,
							buffer->len)) < 0)
		{
			rw_on = 0;
			return IP_PIPE_ERROR;
		}
		len_wrote += ret;
	}
	
	//printf("wrote %d\n", len_wrote);
	rw_on = 0;
	return len_wrote;
}

int ip_pipe_send_header(int header, pid_t pid)
{
	ip_pipe_buffer buffer;
	buffer.data = NULL;
	ip_pipe_buffer_set(&buffer, 1);
	buffer.data[0] = header;
	buffer.len = 1;
	//ip_pipe_write(IP_PIPE_BLOCK, &buffer, pid);
	ip_pipe_write(5000, &buffer, pid);
	
	ip_pipe_buffer_unset(&buffer);
	
	return 1;
}


int ip_pipe_wait_header_data(	int				timeout,
								int				header,
								ip_pipe_buffer	*buffer,
								pid_t			pid)
{
	if (ip_pipe_read(timeout, buffer, pid) == NULL)
		return -1;
	if (buffer->len == 0)
		return -1;
	
	//printf("wait_header read %02x\n", buffer->data[0]);
	
	if (buffer->data[0] == header)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int ip_pipe_wait_header(int		timeout,
						int		header,
						pid_t	pid)
{
	int ret;
	ip_pipe_buffer *buffer;
	buffer = ip_pipe_buffer_init();
	
	ret = ip_pipe_wait_header_data(timeout, header, buffer, pid);
	ip_pipe_buffer_free(buffer);
	return ret;
}


