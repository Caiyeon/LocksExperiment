/*
 * Author: 			Tony Cai
 * Last modified:	February 25th 2015
 * sync.h:			The header regarding 2 spinlocks and a mutex lock
 * Dependencies:	sync.c (optional implementation)
 */

#ifndef _my_SYNC_H_
#define _my_SYNC_H_

#include "atomic_ops.h"
#include <sys/types.h>

struct my_mutex_struct {
	volatile unsigned long locked;
	long owner;
};

typedef struct my_mutex_struct my_mutex_t;


int my_mutex_init(my_mutex_t *mutex);
int my_mutex_unlock(my_mutex_t *mutex);
int my_mutex_destroy(my_mutex_t *mutex);

int my_mutex_lockTAS(my_mutex_t *mutex);
int my_mutex_lockTTAS(my_mutex_t *mutex);
int my_mutex_trylock(my_mutex_t *mutex);



/*Spinlock Starts here*/

struct my_spinlock_struct {
	volatile unsigned long locked; // 0 for unlocked, 1 for locked
	long owner;
};

typedef struct my_spinlock_struct my_spinlock_t;

int my_spinlock_init(my_spinlock_t *lock);
int my_spinlock_destroy(my_spinlock_t *lock);
int my_spinlock_unlock(my_spinlock_t *lock);

int my_spinlock_lockTAS(my_spinlock_t *lock);
int my_spinlock_lockTTAS(my_spinlock_t *lock);
int my_spinlock_trylock(my_spinlock_t *lock);


#endif
