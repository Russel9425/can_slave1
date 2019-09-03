/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : epos_od_process.h
   Author : tao.jing
   Date   : 19-8-10
   Brief  : 
**************************************************************************/

#ifndef CAN_MASTER_EPOS_OD_PROCESS_H
#define CAN_MASTER_EPOS_OD_PROCESS_H

enum status_word_bits
{
    READY_SWITCH_ON = 0,
    SWITCHED_ON     = 1,
    OPERATION_ENABLED = 2,
    FAULT = 3,
    VOLTAGE_ENABLED = 4,
    QUICK_STOP = 5,
    SWITCH_ON_DISABLED = 6,
    WARNING = 7,
    REMOTE = 9,
};

unsigned char get_status_word_bit(unsigned char node_id, unsigned char bit_idx);

#endif //CAN_MASTER_EPOS_OD_PROCESS_H
