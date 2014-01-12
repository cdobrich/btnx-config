
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */
  
#ifndef BTND_H_
#define BTND_H_

#define BTND_DETECTION_PRESSES	5

int btnd_get_rawcode(void);
void btnd_set_rawcode(int rawcode);
int btnd_detecting(void);
void btnd_start(void);
void btnd_stop(void);
int btnd_check_stop(void);
int btnd_check_start(void);
int btnd_check_button_report(void);
int btnd_report_button(int rawcode);
void btnd_enable_save(void);
void btnd_save_button(void);

#endif /*BTND_H_*/
