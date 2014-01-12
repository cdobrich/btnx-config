
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef BACKUP_H_
#define BACKUP_H_

/* Backup error codes returned from backup_config() or restore_config() */
enum
{
	NO_BACKUP_ERROR=0,
	ERROR_BACKUP_OPEN_VERSION,
	ERROR_BACKUP_WRITE_VERSION,
	ERROR_BACKUP_TAR_CREATE,
	ERROR_RESTORE_EXTRACT_VERSION,
	ERROR_RESTORE_VERSION_INCOMPATIBLE,
	ERROR_RESTORE_TAR_EXTRACT
};

int backup_config(const char *path);
int restore_config(const char *path, int ignore_version);

#endif /*BACKUP_H_*/
