
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */


/*
 * ip_pipe is an interprocess (inter-thread) communication module.
 * It uses two pipes to achieve two-way communication between a parent
 * and child thread. This provides a simple framework, and does not enforce
 * a protocol.
 * 
 * This can be used in just about any application that needs parent-child
 * communication. The application specific protocol must be developed
 * separately, however.
 */
#ifndef IP_PIPE_H_
#define IP_PIPE_H_

#define IP_PIPE_R			1
#define IP_PIPE_W			2
#define IP_PIPE_ERROR		-1
#define IP_PIPE_EMPTY		0

#define IP_PIPE_NULL_SET	0
#define IP_PIPE_R_SET		1
#define IP_PIPE_W_SET		2
#define IP_PIPE_RW_SET		IP_PIPE_R_SET + IP_PIPE_W_SET

#define IP_PIPE_BLOCK		-1
#define IP_PIPE_NONBLOCK	0

#define IP_PIPE_DATA_BUFFER_MAX		512

#define ip_pipe_wait_read(to) ip_pipe_wait((to), IP_PIPE_R)
#define ip_pipe_wait_write(to) ip_pipe_wait((to), IP_PIPE_W)


#define IP_PIPE_PID_TO		0
#define IP_PIPE_PID_FROM	1
#define IP_PIPE_PID_BOTH	2
#define ip_pipe_get_bypid_to(to) ip_pipe_get_bypid(to, 0, IP_PIPE_PID_TO)
#define ip_pipe_get_bypid_from(from) ip_pipe_get_bypid(0, from, IP_PIPE_PID_FROM)
#define ip_pipe_get_bypid_both(to, from) ip_pipe_get_bypid(to, from, IP_PIPE_PID_BOTH)


/*
 * This enum shouldn't actually be here. It is specific to btnx-config.
 * They are headers used to indicate the type of data in the stream.
 * These must be changed and handled according to an application's needs.
 * 
 * HOWEVER: IP_ACK=1 and IP_ERROR=2 must be defined. They are the only two
 * headers used internally by ip_pipe.
 */
enum ip_pipe_header // is '\n' handled??? others???
{
	IP_ACK=1,
	IP_ERROR,
	IP_CHILD_LINK,
	IP_START_MOUSED,
	IP_STOP_MOUSED,
	IP_REPORT_HANDLER,
	IP_REPORT_BUTTON,
	IP_STOP_BTND,
	IP_START_BTND
};


/*
 * struct ip_pipe
 * -----------------------------------------------------------------------------
 * This structure is for ip_pipe internal use only. Each ip_pipe contains the
 * necessary information to communicate on a single pipe. Each process needs
 * two ip_pipes to achieve two-way communication.
 * 
 * Members:
 * 	pid_to:		The PID of the receiving process of the pipe.
 * 	pid_from:	The PID of the sending process of the pipe.
 * 	rpipe:		File descriptor for reading the pipe.
 * 	wpipe:		File descriptor for writing to the pipe.
 * 	pipe_max:	The value of the higher fd (a helper used for select()).
 */
typedef struct ip_pipe
{
	pid_t	pid_to;
	pid_t	pid_from;
	int		rpipe;
	int		wpipe;
	int		pipe_max;
} ip_pipe;


/*
 * struct ip_pipe_buffer
 * -----------------------------------------------------------------------------
 * This structure is used to hold the data being read from or written to an
 * ip_pipe. A process must allocate and initialize the structure when writing
 * to an ip_pipe. The process must initialize it to NULL when reading from
 * a pipe.
 * 
 * Members:
 * 	data:	The stream of data.
 * 	len:	The number of bytes in data.
 */
typedef struct ip_pipe_buffer
{
	char *data;
	int len;
} ip_pipe_buffer;

int ip_pipe_init(pid_t pid);

void ip_pipe_free(void);

int ip_pipe_free_bypid(pid_t pid_child);

int ip_pipe_flag_pid_old(pid_t pid_child);

int ip_pipe_handle_old_pids(void);

int ip_pipe_get_bypid(	pid_t 	pid_to,
						pid_t 	pid_from,
						int		bypid);

int ip_pipe_link_parent(pid_t child);

int ip_pipe_link_child(pid_t parent);

int ip_pipe_buffer_set(	ip_pipe_buffer	*buffer,
						int				size);

ip_pipe_buffer *ip_pipe_buffer_init(void);

void ip_pipe_buffer_unset(ip_pipe_buffer *buffer);

void ip_pipe_buffer_free(ip_pipe_buffer *buffer);

ip_pipe_buffer *ip_pipe_read(	int 			timeout,
								ip_pipe_buffer 	*buffer,
								pid_t			pid);

int ip_pipe_write(int timeout, ip_pipe_buffer *buffer, pid_t pid);

int ip_pipe_wait(int timeout, int rw, ip_pipe *sel_pipe);

int ip_pipe_send_header(int header, pid_t pid);

int ip_pipe_wait_header_data(int timeout, int header, ip_pipe_buffer *buffer, pid_t pid);

int ip_pipe_wait_header(int timeout, int header, pid_t pid);


#endif /*IP_PIPE_H_*/
