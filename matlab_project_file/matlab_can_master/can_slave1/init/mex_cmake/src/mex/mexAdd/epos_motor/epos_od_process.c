/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : epos_od_process.c
   Author : tao.jing
   Date   : 19-8-10
   Brief  : 
**************************************************************************/

#include "epos_env.h"
#include <stdio.h>

unsigned char get_status_word_bit(unsigned char node_id, unsigned char bit_idx)
{
    //printf("Current Status Word %x\n", g_epos_status_word);
    unsigned short mask = (unsigned short)1 << bit_idx;
    //printf("Current Mask Word %x\n", mask);
    unsigned short bit_value = (unsigned short)(g_epos_status_word[node_id] & mask);
    //printf("Bit value @ bit index %d is : %d\n", bit_idx, bit_value);
    return bit_value != 0;
}