/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : Master.h
   Author : tao.jing
   Date   : 19-7-30
   Brief  :
**************************************************************************/
#ifndef MASTER_API_H
#define MASTER_API_H

#include "MasterOD.h"

void Master_heartbeatError(CO_Data *d, UNS8 heartbeatID);

UNS8 Master_canSend(Message *);

void Master_initialisation(CO_Data *d);
void Master_preOperational(CO_Data *d);
void Master_operational(CO_Data *d);
void Master_stopped(CO_Data *d);

void Master_post_sync(CO_Data *d);
void Master_post_TPDO(CO_Data *d);
void Master_post_emcy(CO_Data *d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);
void Master_post_SlaveBootup(CO_Data *d, UNS8 nodeid);

#endif