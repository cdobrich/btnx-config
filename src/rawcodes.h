
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#ifndef RAWCODES_H_
#define RAWCODES_H_

typedef struct s_rawcode
{
	int rawcode;
	int pressed;
	int got_highest;
} s_rawcode;


int rawcodes_report(int rawcode);
void rawcodes_free(void);
int rawcodes_find(int rawcode);
void rawcodes_reset_highest(void);
int rawcodes_get_pressed(int rawcode);
int rawcodes_get_most_pressed(void);

#endif /*RAWCODES_H_*/
