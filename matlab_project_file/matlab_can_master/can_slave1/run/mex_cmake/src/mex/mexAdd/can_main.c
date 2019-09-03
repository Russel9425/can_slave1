/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if defined(WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include "getopt.h"
void pause(void)
{
    system("PAUSE");
}
#else

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
//#include <pthread.h>

#endif

#include "canfestival.h"
//#include <can_driver.h>
//#include <timers_driver.h>

#include "MasterAPI.h"
#include "can_main.h"
#include "epos_motor_api.h"
#include "epos_od_process.h"
#include "epos_env.h"
#include "thread_tool.h"

/* MEX header */
#include <mex.h>
#include "matrix.h"

UNS32 OnMasterMap1Update(CO_Data *d, const indextable *unsused_indextable, UNS8 unsused_bSubindex)
{
    return 0;
    //eprintf("OnMasterMap1Update:%d\n", MasterMap1);
    //return 0;
}

s_BOARD SlaveBoard = {"can0", "125K"};
s_BOARD MasterBoard = {"can0", "1M"};

UNS8 MOTOR_ID_ARRY[] = {2};

#if !defined(WIN32) || defined(__CYGWIN__)

void catch_signal(int sig)
{
    signal(SIGTERM, catch_signal);
    signal(SIGINT, catch_signal);
    eprintf("Got Signal %d\n", sig);
}

#endif

void wait_key(const char* state_str);

/***************************  INIT  *****************************************/
void InitNodes(CO_Data *d, UNS32 id)
{
    /****************************** INITIALISATION MASTER *******************************/
    //RegisterSetODentryCallBack(&MasterOD_Data, 0x2000, 0, &OnMasterMap1Update);

    /* Defining the node Id */
    setNodeId(&MasterOD_Data, 0x08);

    /* init */
    setState(&MasterOD_Data, Initialisation);
}

/***************************  EXIT  *****************************************/
void Exit(CO_Data *d, UNS32 id)
{
    if (strcmp(MasterBoard.baudrate, "none"))
    {

        //masterSendNMTstateChange(&MasterOD_Data, 0x02, NMT_Reset_Node);

        //Stop master
        setState(&MasterOD_Data, Stopped);
    }
}

/* Callback function that check the read SDO demand */
void CheckReadSDO(CO_Data *d, UNS8 nodeid)
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

void wait_key(const char* state_str)
{
    char cmd;
    printf("Press any key to %s\n", state_str);
    scanf("%c", &cmd);
}

/****************************************************************************/
/***************************  MAIN  *****************************************/
/****************************************************************************/
// int main(int argc, char **argv)
void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
    // Can Device lib path
#ifdef USE_ARM
    // On ARM
    //char *LibraryPath = "/home/tao/work/imx6/can-festival/lib/libcanfestival_can_socket.so";
    char *LibraryPath = "./libcanfestival_can_socket.so";
#else
    // On Native
    //char* LibraryPath="../../drivers/can_virtual/libcanfestival_can_virtual.so";
    char *LibraryPath = "/usr/local/lib/libcanfestival_can_socket.so";
#endif

#if !defined(WIN32) || defined(__CYGWIN__)
    /* install signal handler for manual break */
    //signal(SIGTERM, catch_signal);
    //signal(SIGINT, catch_signal);
#endif
    // Also Windows needs to initialize the Timer!
    // Moving this line into the ifndef leads to a lethal crash!
    TimerInit();

#ifndef NOT_USE_DYNAMIC_LOADING
    if (LoadCanDriver(LibraryPath) == NULL)
        printf("Unable to load library: %s\n", LibraryPath);
#endif

    double *a;
    a = mxGetPr(prhs[0]);
    plhs[0] = mxCreateDoubleMatrix(2,1,mxREAL);
    double *c;
    c = mxGetPr(plhs[0]);
    if(a[0]==1)
    {
            // Set MasterOD Callbacks
        MasterOD_Data.heartbeatError = Master_heartbeatError;
        MasterOD_Data.initialisation = Master_initialisation;
        MasterOD_Data.preOperational = Master_preOperational;
        MasterOD_Data.operational = Master_operational;
        MasterOD_Data.stopped = Master_stopped;
        MasterOD_Data.post_sync = Master_post_sync;
        MasterOD_Data.post_TPDO = Master_post_TPDO;
        MasterOD_Data.post_emcy = Master_post_emcy;
        MasterOD_Data.post_SlaveBootup = Master_post_SlaveBootup;

        // Open CAN devices
        CAN_PORT master_can_port = canOpen(&MasterBoard, &MasterOD_Data);
        if (!master_can_port)
        {
            eprintf("Cannot open Master Board (%s,%s)\n", MasterBoard.busname, MasterBoard.baudrate);
            goto fail_master;
        }

        // wait_key("Press any key to start master");

        // Start timer thread
        StartTimerLoop(&InitNodes);

        int slave_node_id = 2;
        int ret = 0;
        int slave_speed = 0;
        // if ( argc == 2 )
        // {
        //     slave_node_id = atoi(argv[1]);
        // }

        // unsigned char motor_count = sizeof( MOTOR_ID_ARRY ) / sizeof (MOTOR_ID_ARRY[0]);
        // printf(" ====== Slave Motor Count : %d ====== \n", motor_count);

        for (unsigned char slave_idx = 2; slave_idx <= 2 ; slave_idx ++)
        {
            // // slave_node_id = MOTOR_ID_ARRY[slave_idx];
            slave_node_id = slave_idx;
            printf(" ====== Slave Node ID : %d ====== \n", slave_node_id);

            //wait_key("1.Press any key to read device type");
            printf("Read device type %d\n", slave_node_id);
            ret = read_device_type(slave_node_id);

            //wait_key("2.Press any key to check status word");
            printf("Read status word %d\n", slave_node_id);
            ret = read_status_word(slave_node_id);
            sleep(1);

            if ( 1 == get_status_word_bit(slave_node_id, SWITCH_ON_DISABLED) || 0 == get_status_word_bit(slave_node_id, SWITCHED_ON))
            {
                printf("Switch on disable Or Not Switch On!\n");
                reset_by_control_word(slave_node_id);
            }

            //wait to switch on
            const unsigned char wait_time = 5;
            unsigned char wait_idx = 0;
            while ( wait_idx < wait_time )
            {
                read_status_word(slave_node_id);
                sleep(1);
                if ( 0 == get_status_word_bit(slave_node_id, SWITCHED_ON) )
                {
                    sleep(1);
                }
                else
                {
                    break;
                }
                wait_idx++;
            }

            if ( 0 == get_status_word_bit(slave_node_id, SWITCHED_ON) )
            {
                printf("Switch on failed!\n");
            }

            //wait_key("3.Press any key to activate velocity mode");
            printf("Activate velocity mode %d\n", slave_node_id);
            ret = avtivate_mode(slave_node_id, PVM);
            sleep(1);

            // if(slave_speed != a[1])
            // {
            //wait_key("4.Press any key to set velocity speed");
            slave_speed = a[1];
            printf("Set velocity speed %d\n", slave_node_id);
            ret = set_pvm_velocity(slave_node_id, slave_speed);
            sleep(1);
            
            //wait_key("5.Press any key to start pvm movement");
            printf("Start pvm movement %d\n", slave_node_id);
            ret = start_pvm(slave_node_id);
            sleep(1);

                // slave_node_id = MOTOR_ID_ARRY[slave_idx];
            slave_node_id = slave_idx;
            ret = read_digital_encoder1_value(slave_node_id);
            sleep(1);
            c[0] = g_epos_encoder_value[slave_node_id];
            printf("Cheng Lu %d\n", g_epos_encoder_value[slave_node_id]);

            ret = read_vel_actual_value(slave_node_id);
            sleep(1);
            c[1] = g_epos_actul_speed[slave_node_id];
            printf("Cheng Lu %d\n", g_epos_actul_speed[slave_node_id]);
            // ret = sync_read_od_by_index(slave_node_id, 0x3010, 0x0);
            // struct timespec tsp;
            // make_cond_timeout_ms(&tsp, 2000);
            // //block wait
            // //int wait_ret = pthread_cond_wait(&g_epos_sync_read_sdo_cond, &g_epos_sync_read_sdo_mutex);
            // //timed wait
            // int wait_ret = pthread_cond_timedwait(&g_epos_sync_read_sdo_cond, &g_epos_sync_read_sdo_mutex, &tsp);
            // if ( wait_ret != 0 )
            // {
            //     printf("Wait device type timeout! Ret : %d...\n", wait_ret);
            // }
            // else
            // {
            //     printf("Node %d device type : 0x%x\n", slave_node_id, g_epos_encoder_value[slave_node_id]);
            //     *c = g_epos_encoder_value[slave_node_id];
            // }
        }

        // Check Encoder Value
        // const unsigned char read_count = 10;
        // for (unsigned char read_idx = 0; read_idx < read_count; read_idx++)
        // {
        //     for (unsigned char slave_idx = 0; slave_idx < motor_count ; slave_idx ++)
        //     {
        //         slave_node_id = MOTOR_ID_ARRY[slave_idx];
        //         ret = read_digital_encoder1_value(slave_node_id);
        //         c = g_digital_encoder_value[slave_node_id];
        //         printf("Cheng Lu %d\n",c);
        //         usleep(1000 * 50);
        //     }
        //     sleep(1);
        // }


        // wait_key("Press any key to halt movement\n");
        // for (unsigned char slave_idx = 0; slave_idx < motor_count ; slave_idx ++)
        // {
        //     slave_node_id = MOTOR_ID_ARRY[slave_idx];
        //     printf("Halt slave %d movement...\n", slave_node_id);
        //     ret = halt_move(slave_node_id);
        // }

        // wait Ctrl-C
        // pause();

        printf("Cheng Lu 11111111111\n");
        // Stop timer thread
        StopTimerLoop(&Exit);

        // Close CAN devices (and can threads)
        canClose(&MasterOD_Data);

    fail_master:
        TimerCleanup();
        // return 0;
    }
}
