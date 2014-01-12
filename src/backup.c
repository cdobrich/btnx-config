
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/* Contains functions for backing up and restoring btnx configurations */

#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "backup.h"
#include "common.h"
#include "config_file.h"
#include "udev.h"
#include "threads.h"

/* Version information. Restored configuration must have been backed up with 
 * a version greater than or equal to the compatibility version. */
#define VERSION_COMPATIBILITY	"0.4.7"
#define VERSION_FILE			"btnx_version"
#define VERSION_PATH			CONFIG_PATH "/" VERSION_FILE

/* Converts version string to an integer for version comparison */
static unsigned int version_from_str(const char *str)
{
	gchar **split;
	unsigned int ret;
	unsigned int v0, v1, v2;
	
	split = g_strsplit_set(str, ".-", 4);
	
	v2 = strtol(split[2], NULL, 10);
	v1 = strtol(split[1], NULL, 10);
	v0 = strtol(split[0], NULL, 10);
	
	ret = v2;
	ret += (v1 << 12);
	ret += (v0 << 24);
	
	g_strfreev(split);
	
	return ret;
}

/* Backs up btnx configurations */
int backup_config(const char *path)
{
	int fd;
	gchar **args, *tmp_config_path, *tmp_udev_path;
	gchar *config_path, *udev_path;
	
	if ((fd = open(	VERSION_PATH, O_WRONLY | O_CREAT | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) < 0)
	{
		fprintf(stderr, "Error: backup_config could not open version file: %s\n",
				strerror(errno));
		return ERROR_BACKUP_OPEN_VERSION;
	}
	
	if (write(fd, VERSION, strlen(VERSION)) < 0)
	{
		fprintf(stderr, "Error: backup_config could not write version file: %s\n",
				strerror(errno));
		close(fd);
		return ERROR_BACKUP_WRITE_VERSION;
	}
	
	fsync(fd);
	close(fd);
	
	tmp_config_path = g_strdup(CONFIG_PATH);
	tmp_udev_path = g_strdup(UDEV_RULES_FILE);
	if (tmp_config_path[0] == '/')
	{
		config_path = g_strdup(tmp_config_path+1);
		g_free(tmp_config_path);
	}
	else
		config_path = tmp_config_path;
	
	if (udev_enabled_get() == 1)
	{
		if (tmp_udev_path[0] == '/')
		{
			udev_path = g_strdup(tmp_udev_path+1);
			g_free(tmp_udev_path);
		}
		else
			udev_path = tmp_udev_path;
	}
	else
		udev_path = NULL;
	
	args = (gchar **) g_malloc(8 * sizeof(gchar *));
	args[0] = g_strdup(TAR_PATH);
	args[1] = g_strdup("czf");
	args[2] = g_strdup(path);
	args[3] = g_strdup("-C");
	args[4] = g_strdup("/");
	args[5] = config_path;
	args[6] = udev_path;
	args[7] = NULL;
	
	if (threads_fork_execv(TAR_PATH, args) < 0)
	{
		fprintf(stderr, "Error: backup_config tar exec failed.\n");
		g_strfreev(args);
		return ERROR_BACKUP_TAR_CREATE;
	}
	
	g_strfreev(args);

	return 0;
}

/* Restores btnx configurations */
int restore_config(const char *path, int ignore_version)
{
	gchar **args;
	char buffer[64];
	FILE *fp;
	
	if (!ignore_version)
	{
		args = (gchar **) g_malloc(7 * sizeof(gchar *));
		args[0] = g_strdup(TAR_PATH);
		args[1] = g_strdup("xzf");
		args[2] = g_strdup(path);
		args[3] = g_strdup("-C");
		args[4] = g_strdup("/tmp");
		args[5] = g_strdup("etc/btnx/" VERSION_FILE);
		args[6] = NULL;
		
		if (threads_fork_execv(TAR_PATH, args) < 0)
		{
			fprintf(stderr, "Error: could not extract file " VERSION_FILE 
					" from backup archive.\n");
			g_strfreev(args);
			return ERROR_RESTORE_EXTRACT_VERSION;
		}
		g_strfreev(args);
		
		if ((fp = fopen("/tmp" VERSION_PATH, "r")) == NULL)
		{
			fprintf(stderr, "Error: restore_config could not open version file: %s\n",
					strerror(errno));
			return ERROR_RESTORE_EXTRACT_VERSION;
		}
		memset(buffer, '\0', 64);
		fgets(buffer, 64, fp);
		fclose(fp);
		
		if (version_from_str(VERSION_COMPATIBILITY) > version_from_str(buffer))
		{
			fprintf(stderr, "Warning: restore_config version incompatibility. "
					"Attempted to restore configuration from version %s but this "
					"version of btnx-config is only compatible with versions >=%s\n",
					buffer, VERSION_COMPATIBILITY);
			
			return ERROR_RESTORE_VERSION_INCOMPATIBLE;
		}
	}
	
	args = (gchar **) g_malloc(7 * sizeof(gchar *));
	args[0] = g_strdup(TAR_PATH);				// tar path to exec
	args[1] = g_strdup("xzf");					// extract, gunzip, file
	args[2] = g_strdup(path);					// path to archive
	args[3] = g_strdup("-C");					// change working directory->
	args[4] = g_strdup("/");					// to root
	args[5] = g_strdup("--no-overwrite-dir");	// don't overwrite dir metadata
	args[6] = NULL;								// NULL terminated string vector
	
	if (threads_fork_execv(TAR_PATH, args) < 0)
	{
		fprintf(stderr, "Error: restore_config tar exec failed.\n");
		g_strfreev(args);
		return ERROR_RESTORE_TAR_EXTRACT;
	}
	g_strfreev(args);
	
	return 0;
}


