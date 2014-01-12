
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/* Functions that control button detection */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <linux/input.h>

#include "btnd.h"
#include "ip_pipe.h"
#include "widgets.h"
#include "handlers.h"
#include "timers.h"
#include "rawcodes.h"
#include "buttons.h"
#include "threads.h"
#include "dialog_btnd.h"
#include "text.h"

#define CLEAR_CODE_TRAIL(i) ((i) & 0xFFFFFF00)
#define IS_MOUSE_X(i) (CLEAR_CODE_TRAIL(i) == 0x02000000)
#define IS_MOUSE_Y(i) (CLEAR_CODE_TRAIL(i) == 0x02000100)
#define IS_MOUSE_MOVE(i) (IS_MOUSE_X(i) || IS_MOUSE_Y(i))

#define CHAR2INT(c, x) (((int)(c)) << ((x) * 8))
#define INPUT_BUFFER_SIZE	512
#define MAX_RAWCODES	10
#define HEXDUMP_SIZE	8

#define MAX_NUM_PRESSES	12

static pid_t pid_parent=-1;
static pid_t pid_btnd=-1;

static int detecting = 0;
static int cur_rawcode = 0;
static int num_pressed = 0;
static int total_num_pressed = 0;
static int rawcode_conflict = 0;


int btnd_detecting(void)
{
	if (pid_btnd == -1)
		return 0;
	return detecting;
}

int btnd_get_rawcode(void)
{
	return cur_rawcode;
}

void btnd_set_rawcode(int rawcode)
{
	cur_rawcode = rawcode;
}

void btnd_detect(void)
{
	int rawcode;
	fd_set fds;
	int ret, i;
	struct timeval timeout = {0, 100000};
	int max_fd;
	struct input_event ev;
	
	handlers_open_all();
	ip_pipe_send_header(IP_START_BTND, pid_parent);
	
	for (;;)
	{
		if (btnd_check_stop() > 0)
			break;
		max_fd = handlers_zero_and_fill_fds(&fds);
		if (max_fd < 0)
		{
			fprintf(stderr, "Error: handlers FD_SET failed in btnd_detect.\n");
			_exit(1);
		}
		select(max_fd+1, &fds, NULL, NULL, &timeout);
					
		i=0;
		while ((ret = handlers_isset(&fds, i)) >= 0)
		{
			if (ret == 0)
			{
				i++;
				continue;
			}
			
			if ((ret = read(handlers_get_fd(i), &ev, sizeof(ev)) < sizeof(ev)))
			{
				i++;
				continue;
			}
			
			
			if (((ev.code == REL_X || ev.code == REL_Y) && ev.type == EV_REL) || 
				(ev.code == SYN_REPORT && ev.type == EV_SYN)) {
				//detected mouse movement, ignore
			}
			else if (ev.value == 1 || ev.type == EV_REL) {
				//g_print("evcode = %04x, type=%04x, value=%08x\n", ev.code, ev.type, ev.value);
				rawcode = ev.code & 0xFFFF;
				if (ev.type == EV_REL)
					rawcode += (ev.value & 0xFF) << 16;
				rawcode += (ev.type & 0xFF) << 24;
				btnd_report_button(rawcode);
			}		
			i++;
		}
		
	}
	
	handlers_close_all();
}

void btnd_start(void)
{
	GtkWidget *dialog;
	pid_t parent = getpid();
	
	rawcodes_free();
	pid_btnd = -1;
	detecting = 0;
	cur_rawcode = 0;
	num_pressed = 0;
	total_num_pressed = 0;
	rawcode_conflict = 0;
	gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_STATUS_BTND)),
						TEXT_DETECTING);
	
	if ((pid_btnd = threads_fork_with_pipe(&parent)) == 0)
	{
		pid_parent = parent;
		usleep(1000);
		buttons_free();
		rawcodes_free();
		btnd_detect();
		_exit(0);
	}
	else if (pid_btnd == -1)
	{
		fprintf(stderr, "Error: btnd_start fork failed: %s\n", strerror(errno));
		SHOW_ERROR_DIALOG(	dialog, GET_WIDGET(WIN_BTND),
							TEXT_TITLE_BTND_FORK_FAIL,
							TEXT_BODY_BTND_FORK_FAIL);
		init_btnd();
		return;
	}
	
	detecting = 1;
	gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_HELP_BTND)),
						TEXT_HELP_BTND_RUN);
	g_timeout_add_full(	G_PRIORITY_HIGH_IDLE,
					BTND_CHECK_START_INTERVAL,
					(GSourceFunc)btnd_check_start_timer,
					NULL, NULL);
}

static void btnd_handle_button_report(int rawcode)
{
	float prc;
	//g_print("prev: %08x now: %08x\n", cur_rawcode, rawcode);
	rawcodes_report(rawcode);
	total_num_pressed++;
	if (total_num_pressed >= MAX_NUM_PRESSES)
	{
		//g_print("Max num presses reached\n");
		rawcode_conflict = 1;
		btnd_stop();
		dialog_btnd_run();
		gtk_progress_bar_update(GTK_PROGRESS_BAR(GET_WIDGET(PROGRESSBAR_BTND)), 1);
		return;
	}
	if (buttons_exists(rawcode)>= 0)
	{
		gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_STATUS_BTND)),
							TEXT_ALREADY_DETECTED);
		return;
	}
	else
	{
		gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_STATUS_BTND)),
							TEXT_DETECTING);
	}
	if (cur_rawcode == 0 || num_pressed <= 0)
	{
		cur_rawcode = rawcode;
		num_pressed = 0;
	}
	if (rawcode == cur_rawcode)
	{
		num_pressed++;
	}
	else
	{
		if (num_pressed > 0)
			num_pressed--;
	}
	prc = ((float)num_pressed)/((float)BTND_DETECTION_PRESSES);
	gtk_progress_bar_update(GTK_PROGRESS_BAR(GET_WIDGET(PROGRESSBAR_BTND)),
								prc);
	if (num_pressed == BTND_DETECTION_PRESSES)
	{
		btnd_enable_save();
		btnd_stop();
	}
}

void btnd_stop(void)
{
	if (detecting == 1 && pid_btnd != -1)
		ip_pipe_send_header(IP_STOP_BTND, pid_btnd);
	detecting = 0;
	pid_btnd = -1;
}

int btnd_check_stop(void)
{
	return ip_pipe_wait_header(10, IP_STOP_BTND, pid_parent);
}

int btnd_check_start(void)
{
	if (pid_btnd == -1)
		return -1;
	return ip_pipe_wait_header(10, IP_START_BTND, pid_btnd);
}

int btnd_report_button(int rawcode)
{
	ip_pipe_buffer *buffer;
	int len;
	int *pint;
	
	buffer = ip_pipe_buffer_init();
	
	len = 1 + sizeof(int);
	
	if (ip_pipe_buffer_set(buffer, len) != len)
	{
		fprintf(stderr, 
		"Error: btnd_report_button failed buffer set: %s\n", strerror(errno));
		return -1;
	}
	
	buffer->data[0] = IP_REPORT_BUTTON;
	pint = (int*) &(buffer->data[1]);
	*pint = rawcode;
	buffer->len = len;
	if (ip_pipe_write(1000, buffer, pid_parent) < 1)
		fprintf(stderr, "Error: button report write error\n");
	ip_pipe_buffer_free(buffer);
	
	return 1;
}

int btnd_check_button_report(void)
{
	int rawcode=0, *pint;
	ip_pipe_buffer *buffer;
	
	if (pid_btnd == -1)
		return -1;
	
	buffer = ip_pipe_buffer_init();

	if (ip_pipe_wait_header_data(1, IP_REPORT_BUTTON, buffer, pid_btnd) == 1)
	{
		pint = (int *) &(buffer->data[1]);
		rawcode = *pint;
		btnd_handle_button_report(rawcode);
		ip_pipe_buffer_free(buffer);
		return 1;
	}
	ip_pipe_buffer_free(buffer);
	return 0;
}

void btnd_enable_save(void)
{
	gchar *default_name=NULL;
	GtkWidget *name_entry = GET_WIDGET(ENTRY_NAME_BTND);
	
	default_name = g_strdup_printf("%s%d", TEXT_BUTTON, buttons_count()+1);
	gtk_entry_set_text(GTK_ENTRY(name_entry), default_name);
	gtk_editable_set_editable(	GTK_EDITABLE(GET_WIDGET(ENTRY_NAME_BTND)),
								TRUE);
	gtk_widget_set_sensitive(	GET_WIDGET(ENTRY_NAME_BTND),
								TRUE);
	gtk_widget_set_sensitive(	GET_WIDGET(BTN_ADD_BTND),
								TRUE);
	gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_STATUS_BTND)),
						TEXT_DETECTED);
	gtk_label_set_text(	GTK_LABEL(GET_WIDGET(LABEL_HELP_BTND)),
						TEXT_HELP_BTND_ADD);
	gtk_button_set_label(GTK_BUTTON(GET_WIDGET(BTN_CONTROL_BTND)),
						TEXT_START_BUTTON_DETECTION);
	gtk_editable_select_region(GTK_EDITABLE(name_entry), 0, -1);
	gtk_widget_grab_focus(name_entry);
	if (default_name != NULL)
		g_free(default_name);
}

void btnd_save_button(void)
{
	const gchar *btn_name;
	GtkWidget *dialog;
	int ret;
	
	btn_name = gtk_entry_get_text(	GTK_ENTRY(GET_WIDGET(ENTRY_NAME_BTND)));
	if (btn_name == NULL || g_ascii_strcasecmp("", btn_name) == 0)
	{
		SHOW_WARNING_DIALOG(dialog, 
							GET_WIDGET(WIN_BTND),
		/* TRANSLATORS: This is a dialog title. It informs the user they did
		 * something wrong, followed by a specific description.*/
							_("Invalid operation"),
							_("You must specify a button name to add it."));
		return;
	}
	if (buttons_name_exists(btn_name) != -1)
	{
		SHOW_WARNING_DIALOG(dialog,
							GET_WIDGET(WIN_BTND),
		/* TRANSLATORS: This is a dialog title. The user tried to add a button
		 * with a name that has already been used. */
							_("Button name already exists"),
							_("You must specify a different button name."));
		return;
	}
	if ((ret = buttons_exists(btnd_get_rawcode())) == -1)
		ret = buttons_add(btnd_get_rawcode());
	if (ret < 0)
	{
		fprintf(stderr, "Error: button_add failed.\n");
		return;
	}
	buttons_set_name(ret, btn_name);
	buttons_set_new(ret, 1);
	init_btnd();
}
