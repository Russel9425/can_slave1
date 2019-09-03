/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : epos_motor_api.h
   Author : tao.jing
   Date   : 19-8-6
   Brief  : 
**************************************************************************/

#ifndef CAN_MASTER_EPOS_MOTOR_API_H
#define CAN_MASTER_EPOS_MOTOR_API_H

#include "canfestival.h"

enum MOTOR_MODE
{
    PPM = 0,
    PVM = 1,
};

/************/
/*   State  */
/************/
UNS8 read_device_type(UNS8 node_id);

UNS8 read_status_word(UNS8 node_id);

UNS8 read_analog_encoder_value(UNS8 node_id);

UNS8 read_digital_encoder1_value(UNS8 node_id);

UNS8 read_vel_actual_value(UNS8 node_id);

UNS8 write_control_word(UNS8 node_id, UNS16 control_word);

UNS8 reset_by_control_word(UNS8 node_id);

/************/
/*   PVM    */
/************/
UNS8 avtivate_mode(UNS8 node_id, UNS8 mode);

UNS8 set_pvm_velocity(UNS8 node_id, INTEGER32 vel);

UNS8 start_pvm(UNS8 node_id);

UNS8 halt_move(UNS8 node_id);

/**********************/
/*   SDO Framewrok    */
/**********************/

UNS8 sync_read_od_by_index(UNS8 node_id, UNS16 index, UNS8 sub_index);

#endif //CAN_MASTER_EPOS_MOTOR_API_H
