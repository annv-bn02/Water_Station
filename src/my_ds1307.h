#ifndef _MY_DS1307_
#define _MY_DS1307_

#include "all_header.h"
extern uint8_t md_time[8];
extern String ds1307;
void MD_Config(void);
void MD_Run(void);

#endif
