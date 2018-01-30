/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "snowflake_lib.h"

struct globle g_info;
#define  sequenceMask (-1L ^ (-1L << 12L))

void set_workid(int workid)
{
 g_info.workid = workid;
}

pid_t gettid( void )
{
	return syscall( __NR_gettid );
}

uint64_t get_curr_ms()
{
	struct timeval time_now;
	gettimeofday(&time_now,NULL);
	ms_time1 = (uint64_t)time_now.tv_sec*1000 + (uint64_t)time_now.tv_usec/1000;
	return ms_time;
}

uint64_t wait_next_ms(uint64_t lastStamp)
{
	uint64_t cur = 0;
	do {
		cur = get_curr_ms();
	} while (cur <= lastStamp);
	return cur;
}

int atomic_incr(int id)
{
	__sync_add_and_fetch( &id, 1 );
	return id;
}

int64_t get_unique_id()
{
	int64_t  uniqueId = 0;
	uint64_t nowtime = get_curr_ms();
	uniqueId = nowtime<<22;
	uniqueId |=(g_info.workid&0x3ff)<<12;

	if (nowtime <g_info.last_stamp)
	{
		perror("error");
		exit(-1);
	}
	if (nowtime == g_info.last_stamp)
	{
		g_info.seqid = atomic_incr(g_info.seqid)& 0xfff;
		if (g_info.seqid ==0)
		{
			nowtime = wait_next_ms(g_info.last_stamp);
		}
	}
	else
	{
		g_info.seqid  = 0;
	}
	g_info.last_stamp = nowtime;
	uniqueId |=g_info.seqid;
	return uniqueId;
}
