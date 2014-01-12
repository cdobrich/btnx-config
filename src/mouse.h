
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#ifndef MOUSE_H_
#define MOUSE_H_

typedef struct mouse_data
{
	int vendor_id;
	int product_id;
	char *vendor_name;
	char *product_name;
	
	int revoco_status;
	int revoco_btn;
	int revoco_up_scroll;
	int revoco_down_scroll;
} mouse_data;

void mouse_init(void);
void mouse_free(void);
void mouse_set_product_id(int id);
void mouse_set_vendor_id(int id);
void mouse_set_vendor_name(const char *name);
void mouse_set_product_name(const char *name);
void mouse_set_revoco_status(int status);
void mouse_set_revoco_btn(int button);
void mouse_set_revoco_up_scroll(int speed);
void mouse_set_revoco_down_scroll(int speed);

int mouse_get_vendor_id(void);
int mouse_get_product_id(void);
const char *mouse_get_vendor_name(void);
const char *mouse_get_product_name(void);
int mouse_get_revoco_status(void);
int mouse_get_revoco_btn(void);
int mouse_get_revoco_up_scroll(void);
int mouse_get_revoco_down_scroll(void);

#endif /*MOUSE_H_*/
