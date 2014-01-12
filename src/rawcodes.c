
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "rawcodes.h"

static s_rawcode **rcs=NULL;
static int num_rcs=0;

int rawcodes_report(int rawcode)
{
	int index=0;
	if (rcs == NULL || num_rcs == 0)
	{
		rcs = (s_rawcode **) malloc(sizeof(s_rawcode *));
		rcs[0] = (s_rawcode *) malloc(sizeof(s_rawcode));
		if (rcs == NULL)
		{
			fprintf(stderr, "Error: rawcodes_report failed s_rawcode allocation: %s\n", strerror(errno));
			return -1;
		}
		num_rcs = 0;
	}
	else
	{
		if ((index = rawcodes_find(rawcode)) < 0)
		{
			rcs = (s_rawcode **) realloc(rcs, (num_rcs+1)*sizeof(s_rawcode **));
			rcs[num_rcs] = (s_rawcode *) malloc(sizeof(s_rawcode));
			if (rcs == NULL)
			{
				fprintf(stderr, "Error: rawcodes_report failed s_rawcode reallocation: %s\n", strerror(errno));
				return -1;
			}
		}
		else
		{
			(rcs[index]->pressed)++;
			return rcs[index]->pressed;
		}
	}
	
	rcs[num_rcs]->rawcode = rawcode;
	rcs[num_rcs]->pressed = 1;
	rcs[num_rcs]->got_highest = 0;
	num_rcs++;
	
	return rcs[num_rcs-1]->pressed;
}

void rawcodes_free(void)
{
	int i;
	if (rcs != NULL)
	{
		for (i=0; i<num_rcs; i++)
		{
			if (rcs[i] != NULL)
				free(rcs[i]);
		}
		free(rcs);
	}
	num_rcs = 0;
	rcs = NULL;
}

int rawcodes_find(int rawcode)
{
	int i;
	for (i=0; i<num_rcs; i++)
	{
		if (rcs[i]->rawcode == rawcode)
			return i;
	}
	return -1;
}

void rawcodes_reset_highest(void)
{
	int i;
	for (i=0; i<num_rcs; i++)
	{
		rcs[i]->got_highest = 0;
	}
}

int rawcodes_get_pressed(int rawcode)
{
	int index = rawcodes_find(rawcode);
	if (index < 0) return -1;
	return rcs[index]->pressed;
}

int rawcodes_get_most_pressed(void)
{
	int highest=0, i, rawcode_highest=0;
	int index_highest=-1;

	for (i=0; i<num_rcs; i++)
	{
		if (rcs[i]->pressed > highest && rcs[i]->got_highest == 0)
		{
			highest = rcs[i]->pressed;
			rawcode_highest = rcs[i]->rawcode;
			index_highest = i;
		}
	}
	
	if (index_highest >= 0)
		rcs[index_highest]->got_highest = 1;

	return rawcode_highest;
}
