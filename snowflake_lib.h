#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <linux/types.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#define WORKID_WIDTH 1024

typedef struct globle
{
	int global_int:12;
	int64_t last_stamp;
	int workid;
	int seqid;
} globle;

void set_workid(int workid);
pid_t gettid( void );
uint64_t get_curr_ms();
uint64_t wait_next_ms(int64_t lastStamp);
int atomic_incr(int id);
int64_t get_unique_id();
