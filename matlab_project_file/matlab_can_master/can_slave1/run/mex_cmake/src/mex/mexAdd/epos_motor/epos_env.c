/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : epos_env.c
   Author : tao.jing
   Date   : 19-8-10
   Brief  : 
**************************************************************************/

#include "epos_env.h"

unsigned short g_epos_status_word[5]  = {0};
unsigned short g_epos_control_word[5] = {0};

unsigned int g_epos_encoder_value[5] = {0};
unsigned int g_epos_actul_speed[5] = {0};

unsigned int g_epos_sync_read_sdo_data = 0;

/***********************/
/*   Help Variables    */
/***********************/

pthread_cond_t  g_epos_sync_read_sdo_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_epos_sync_read_sdo_mutex = PTHREAD_MUTEX_INITIALIZER;