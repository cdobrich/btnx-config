
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include "widgets.h"
#include "threads.h"
#include "ip_pipe.h"
#include "buttons.h"
#include "handlers.h"
#include "rawcodes.h"
#include "keycodes.h"
#include "text.h"
#include "config_manager.h"


void threads_sigsegv(int num)
{
	/*
	 * Error: btnx-config has detected an incompetent programmer.
	 * On a more serious note, Segmentation fault occurred, 
	 * and a memory dump follows
	 */
	abort();
}

void threads_sigterm(int num)
{
	gtk_main_quit();
	
	buttons_free();
	handlers_free();
	ip_pipe_free();
	rawcodes_free();
	keycodes_free();
	config_manager_free();
	free_glade_xml();
	
	/* TRANSLATORS: do not translate btnx-config. It is the name of a program.*/
	printf(_("Quitting btnx-config...\n"));
	exit(0);
}

void threads_child_sigint(int num)
{
	_exit(0);
}

void threads_sigchld(int num)
{
	sigset_t set, oldset;
	pid_t pid;
	int status;
	
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, &oldset);
	
	while ((pid = waitpid((pid_t)-1, &status, WNOHANG)) > 0)
	{
		if (WIFSTOPPED(status))
			fprintf(stderr, "Warning: child was stopped, but will be handled as terminated.\n");
		if (ip_pipe_flag_pid_old(pid) < 0)
			fprintf(stderr, "Warning: Could not flag child ip_pipe.\n");
	}
	
	signal(SIGCHLD, threads_sigchld);
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_UNBLOCK, &set, &oldset);
}

pid_t threads_fork_with_pipe(pid_t *parent)
{
	pid_t child;
	ip_pipe_init(*parent);
	ip_pipe_init(-1);
	
	if ((child = fork()) == 0)
	{
		signal(SIGINT, threads_child_sigint);
		ip_pipe_link_child(*parent);
		return 0;
	}
	else if (child == -1)
	{
		fprintf(stderr, "Error: threads_fork_with_pipe fork failed: %s\n",
				strerror(errno));
		return -1;
	}
	
	if (ip_pipe_link_parent(child) < 0)
	{
		fprintf(stderr,
				"Error: parent link failed during fork.\n");
		return -1;
	}
	return child;
}

int threads_fork_execv(const char *path, char *const args[])
{
	int pid, ret=0;
	
	/* disable child signal handler */
	signal(SIGCHLD, SIG_DFL);
		
	if (!(pid = fork()))
		execv(path, args);
	else if (pid < 0)
	{
		fprintf(stderr, "Error: threads_fork_execv could not fork: %s\n",
				strerror(errno));
		return -1;
	}
	
	do {
		waitpid(pid, &ret, 0);
	} while (!WIFEXITED(ret));
	
	/* reenable child signal handler */
	signal(SIGCHLD, threads_sigchld);
	
	return WEXITSTATUS(ret);
}
