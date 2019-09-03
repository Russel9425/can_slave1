/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : SlaveAPI.h
   Author : tao.jing
   Date   : 19-7-30
   Brief  :
**************************************************************************/
#ifndef SLAVE_API_H
#define SLAVE_API_H

#include "SlaveOD.h"

void Slave_heartbeatError(CO_Data *d, UNS8 heartbeatID);

UNS8 Slave_canSend(Message *);

void Slave_initialisation(CO_Data *d);
void Slave_preOperational(CO_Data *d);
void Slave_operational(CO_Data *d);
void Slave_stopped(CO_Data *d);

void Slave_post_sync(CO_Data *d);
void Slave_post_TPDO(CO_Data *d);
void Slave_storeODSubIndex(CO_Data *d, UNS16 wIndex, UNS8 bSubindex);
void Slave_post_emcy(CO_Data *d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);

#endif