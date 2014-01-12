
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef THREADS_H_
#define THREADS_H_

typedef void sighandler_t(int);

void threads_sigsegv(int num);
void threads_sigterm(int num);
void threads_child_sigint(int num);
void threads_sigchld(int num);
pid_t threads_fork_with_pipe(pid_t *parent);
int threads_fork_execv(const char *path, char *const args[]);

#endif /*THREADS_H_*/
