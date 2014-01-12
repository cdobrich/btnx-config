
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#ifndef TIMERS_H_
#define TIMERS_H_

#define PIDS_CLEANUP_INTERVAL			5000
#define ZOMBIE_CLEANUP_INTERVAL			5000
#define MOUSED_START_INTERVAL			1000
#define MOUSED_CHECK_START_INTERVAL		10
#define MOUSED_CHECK_REPORT_INTERVAL	100
#define BTND_CHECK_START_INTERVAL		10
#define BTND_CHECK_REPORT_INTERVAL		50

#define MOUSED_START_DELAY			5

guint moused_check_handler_report_timer(gpointer data);
guint moused_check_start_timer(gpointer data);

guint btnd_check_button_report_timer(gpointer data);
guint btnd_check_start_timer(gpointer data);

guint cleanup_unhandled_pids(gpointer data);

#endif /*TIMERS_H_*/
