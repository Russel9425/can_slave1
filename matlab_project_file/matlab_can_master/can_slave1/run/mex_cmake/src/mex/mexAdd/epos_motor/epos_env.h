/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : epos_env.h
   Author : tao.jing
   Date   : 19-8-10
   Brief  : 
**************************************************************************/

#ifndef CAN_MASTER_EPOS_ENV_H
#define CAN_MASTER_EPOS_ENV_H

#include <pthread.h>

extern unsigned short g_epos_status_word[5];
extern unsigned short g_epos_control_word[5];

extern unsigned int g_epos_encoder_value[5];
extern unsigned int g_epos_actul_speed[5];

extern unsigned int g_epos_sync_read_sdo_data;

extern pthread_cond_t  g_epos_sync_read_sdo_cond;
extern pthread_mutex_t g_epos_sync_read_sdo_mutex;

#endif //CAN_MASTER_EPOS_ENV_H
