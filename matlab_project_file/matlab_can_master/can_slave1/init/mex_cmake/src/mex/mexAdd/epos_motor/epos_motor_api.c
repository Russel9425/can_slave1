/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : epos_motor_api.c
   Author : tao.jing
   Date   : 19-8-6
   Brief  : 
**************************************************************************/
#include <unistd.h>
#include "epos_motor_api.h"
#include "canfestival.h"
#include "MasterAPI.h"
#include "can.h"
#include "epos_env.h"
#include "epos_od_process.h"

/* Callback function that check the read SDO demand */
void check_read_sdo(CO_Data *d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data = 0;
    UNS32 size = 64;

    if (getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    else
        printf("\nResult : %x\n", data);

    /* Finalize last SDO transfer with this node */
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

void check_status_word(CO_Data *d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data = 0;
    UNS32 size = 64;

    if (getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
    {
        printf("\nRead Status word Result : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    }
    else
    {
        printf("\nStatusword : %x\n", data);
        g_epos_status_word[nodeid] = (UNS16) data;

    }
    /* Finalize last SDO transfer with this node */
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

void check_analog_encoder(CO_Data *d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data = 0;
    UNS32 size = 32;

    if (getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
    {
        printf("\nRead Analog Encoder Result : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    }
    else
    {
        printf("\nNode ID: %d Analog Encoder Value : %x\n", nodeid, data);
    }
    /* Finalize last SDO transfer with this node */
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

void check_digital_encoder(CO_Data *d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data = 0;
    UNS32 size = 32;

    if (getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
    {
        printf("\nRead Analog Encoder Result : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    }
    else
    {
        printf("\nNode ID: %d Digital Encoder Value : %d\n", nodeid, data);
        g_digital_encoder_value[nodeid] = (UNS32) data;
    }
    /* Finalize last SDO transfer with this node */
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}


void check_write_sdo(CO_Data *d, UNS8 nodeid)
{
    UNS32 abortCode;

	if(getWriteResultNetworkDict(d, nodeid, &abortCode) != SDO_FINISHED)
		printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
	else
		printf("\nSend data OK\n");

	/* Finalize last SDO transfer with this node */
	closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

UNS8 read_device_type(UNS8 node_id)
{
    UNS16 index  = 0x1000;
    UNS8  subindex = 0x00;
    UNS8  datatype = 0;
    UNS8  read_ret = readNetworkDictCallback(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex,
                                            (UNS8) datatype, check_read_sdo, 0);
    if (read_ret == 0)
    {
        printf("Send read sdo successfully.\n");
    } else if (read_ret == 0xFE)
    {
        printf("No sdo client to communicate with node!\n");
    } else if (read_ret == 0xFF)
    {
        printf("Error occur...\n");
    }
    return read_ret;
}

UNS8 read_status_word(UNS8 node_id)
{
    UNS16 index  = 0x6041;
    UNS8  subindex = 0x00;
    UNS8  datatype = 0;
    UNS8  read_ret = readNetworkDictCallback(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex,
                                            (UNS8) datatype, check_status_word, 0);
    if (read_ret == 0)
    {
        printf("Send read sdo successfully.\n");
    } else if (read_ret == 0xFE)
    {
        printf("No sdo client to communicate with node!\n");
    } else if (read_ret == 0xFF)
    {
        printf("Error occur...\n");
    }
    return read_ret;
}

UNS8 read_analog_encoder_value(UNS8 node_id)
{
    UNS16 index  = 0x3011;
    UNS8  subindex = 0x03;
    UNS8  datatype = 0;
    UNS8  read_ret = readNetworkDictCallback(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex,
                                            (UNS8) datatype, check_analog_encoder, 0);
    if (read_ret == 0)
    {
        printf("Send read sdo successfully.\n");
    } else if (read_ret == 0xFE)
    {
        printf("No sdo client to communicate with node!\n");
    } else if (read_ret == 0xFF)
    {
        printf("Error occur...\n");
    }
    return read_ret;
}

UNS8 read_digital_encoder1_value(UNS8 node_id)
{
    UNS16 index  = 0x3010;
    UNS8  subindex = 0x04;
    UNS8  datatype = 0;
    UNS8  read_ret = readNetworkDictCallback(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex,
                                            (UNS8) datatype, check_digital_encoder, 0);
    if (read_ret == 0)
    {
        printf("Send read sdo successfully.\n");
    } else if (read_ret == 0xFE)
    {
        printf("No sdo client to communicate with node!\n");
    } else if (read_ret == 0xFF)
    {
        printf("Error occur...\n");
    }
    return read_ret;
}

UNS8 write_control_word(UNS8 node_id, UNS16 control_word)
{
    UNS16 index  = 0x6040;
    UNS8  subindex = 0x00;
    UNS8  datatype = 0;
    UNS8  data_count = 4;
    UNS8  data[4] = {0x00, 0x00, 0x00, 0x00};
    UNS8  use_block = 0;

    data[0] = (UNS8) control_word;            //low byte of UNS16
    data[1] = (UNS8) ( control_word >> 8 );   //low byte of UNS16

    UNS8  sdo_ret  = writeNetworkDictCallBackWithCsS(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex, data_count,
                                            (UNS8) datatype, data, check_write_sdo, use_block, 0);
    if (sdo_ret == 0)
    {
        printf("Send read sdo successfully.\n");
    }
    else if (sdo_ret == 0xFE)
    {
        printf("No sdo client to communicate with node!\n");
    }
    else if (sdo_ret == 0xFF)
    {
        printf("Error occur...\n");
    }
    return sdo_ret;
}

UNS8 reset_by_control_word(UNS8 node_id)
{
    printf("Re switch on by control word...\n");
    //Set SwitchOn = 0
    UNS8 ret = write_control_word(node_id, 0x06);
    if ( ret != 0 )
    {
        return ret;
    }

    //Check if ready to switch on
    const unsigned char wait_time = 5;
    unsigned char wait_idx = 0;
    while ( wait_idx < wait_time )
    {
        read_status_word(node_id);
        sleep(1);
        if ( 0 == get_status_word_bit(node_id, READY_SWITCH_ON) )
        {
            printf("Not ready to switch on... Continue to wait...\n");
            sleep(1);
        }
        else
        {
            break;
        }
        wait_idx++;
    }

    ret = read_status_word(node_id);
    sleep(1);
    if ( 0 == get_status_word_bit(node_id, READY_SWITCH_ON) )
    {
        printf("Not ready to switch on... Quit...\n");
        return ret;
    }

    //Set SwitchOn = 1
    ret = write_control_word(node_id, 0x0F);
    if ( ret != 0 )
    {
        return ret;
    }

    return ret;
}

UNS8 avtivate_mode(UNS8 node_id, UNS8 mode)
{
    UNS16 index  = 0x6060;
    UNS8  subindex = 0x00;
    UNS8  datatype = 0;
    UNS8  sdo_ret  = 0;
    UNS8  data_count = 4;
    UNS8  data[4] = {0x03, 0x00, 0x00, 0x00};
    UNS8  use_block = 0;
    switch (mode)
    {
        case PPM:
            break;
        case PVM:

            data[0] = 0x03;
            sdo_ret = writeNetworkDictCallBackWithCsS(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex, data_count,
                                            (UNS8) datatype, data, check_write_sdo, use_block, 0);
            /*
            data[4] = 0x03;
            m.cob_id = 0x601;
            m.rtr = NOT_A_REQUEST;
            m.len = 8;
            for (int i = 0; i < 8; i++)
            {
                m.data[i] = data[i];
            }
            read_ret  = canSend(can_port, &m);
            */
            break;
        default:
            printf("Unknown MOTOR_MODE %d...\n", mode);
            break;
    }
    return 0;
}

UNS8 set_pvm_velocity(UNS8 node_id, UNS16 vel)
{
    UNS16 index  = 0x60FF;
    UNS8  subindex = 0x00;
    UNS8  datatype = 0;
    UNS8  data_count = 4;
    UNS8  data[4] = {0x00, 0x00, 0x00, 0x00};
    UNS8  use_block = 0;

    data[0] = (UNS8) vel;   //low byte of UNS16
    data[1] = (UNS8) ( vel >> 8 );   //low byte of UNS16

    UNS8  sdo_ret  = writeNetworkDictCallBackWithCsS(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex, data_count,
                                            (UNS8) datatype, data, check_write_sdo, use_block, 0);
    if (sdo_ret == 0)
    {
        printf("Send read sdo successfully.\n");
    }
    else if (sdo_ret == 0xFE)
    {
        printf("No sdo client to communicate with node!\n");
    }
    else if (sdo_ret == 0xFF)
    {
        printf("Error occur...\n");
    }
    return sdo_ret;
}

UNS8 start_pvm(UNS8 node_id)
{
    UNS16 index  = 0x6040;
    UNS8  subindex = 0x00;
    UNS8  datatype = 0;
    UNS8  data_count = 4;
    UNS8  data[4] = {0x0F, 0x00, 0x00, 0x00};
    UNS8  use_block = 0;

    UNS8  sdo_ret  = writeNetworkDictCallBackWithCsS(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex, data_count,
                                            (UNS8) datatype, data, check_write_sdo, use_block, 0);
    if (sdo_ret == 0)
    {
        printf("Send read sdo successfully.\n");
    }
    else if (sdo_ret == 0xFE)
    {
        printf("No sdo client to communicate with node!\n");
    }
    else if (sdo_ret == 0xFF)
    {
        printf("Error occur...\n");
    }
    return sdo_ret;
}

UNS8 halt_move(UNS8 node_id)
{
    UNS16 index  = 0x6040;
    UNS8  subindex = 0x00;
    UNS8  datatype = 0;
    UNS8  data_count = 4;
    UNS8  data[4] = {0x0F, 0x01, 0x00, 0x00};
    UNS8  use_block = 0;

    UNS8  sdo_ret  = writeNetworkDictCallBackWithCsS(&MasterOD_Data, (UNS8) node_id, (UNS16) index, (UNS8) subindex, data_count,
                                            (UNS8) datatype, data, check_write_sdo, use_block, 0);
    if (sdo_ret == 0)
    {
        printf("Send read sdo successfully.\n");
    }
    else if (sdo_ret == 0xFE)
    {
        printf("No sdo client to communicate with node!\n");
    }
    else if (sdo_ret == 0xFF)
    {
        printf("Error occur...\n");
    }
    return sdo_ret;
}