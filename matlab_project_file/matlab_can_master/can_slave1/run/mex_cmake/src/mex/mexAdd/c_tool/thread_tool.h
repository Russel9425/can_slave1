/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : thread_tool.h
   Author : tao.jing
   Date   : 19-8-31
   Brief  : 
**************************************************************************/

#ifndef CAN_MASTER_THREAD_TOOL_H
#define CAN_MASTER_THREAD_TOOL_H

#include <sys/time.h>

void init_thread_tool();

void make_cond_timeout_ms(struct timespec *p_tsp, unsigned int ms);

#endif //CAN_MASTER_THREAD_TOOL_H
