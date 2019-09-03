/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : thread_tool.c
   Author : tao.jing
   Date   : 19-8-31
   Brief  : 
**************************************************************************/
#include "thread_tool.h"
#include <stdlib.h>
#include <pthread.h>

/************/
/*   Event  */
/************/
#ifdef EVENT_NUM
#undef EVENT_NUM
#endif

#define EVENT_NUM 3

pthread_cond_t  c_cond_arry[EVENT_NUM];
pthread_mutex_t c_mutex_arry[EVENT_NUM];

void init_thread_tool()
{
    unsigned char idx = 0;
    for ( idx = 0; idx < EVENT_NUM; idx ++ )
    {
        pthread_cond_init(&c_cond_arry[idx], NULL);
        pthread_mutex_init(&c_mutex_arry[idx], NULL);
    }
}

void make_cond_timeout_ms(struct timespec *p_tsp, unsigned int ms)
{
    struct timeval now;

    //get current time
    gettimeofday(&now, NULL);

    //convert current time to timespec
    p_tsp->tv_sec  = now.tv_sec;
    p_tsp->tv_nsec = now.tv_usec * 1000;

    //add wait time
    p_tsp->tv_nsec += ms * 1000 * 1000;
    while ( p_tsp->tv_nsec > (long)1E9 )
    {
        p_tsp->tv_sec += 1;
        p_tsp->tv_nsec -= (long)1E9;
    }
}