/*
 * Author: 			Tony Cai
 * Last modified:	February 25th 2015
 * sync.c:			The implementation of functions regarding 2 spinlocks and a mutex lock
 * Dependencies:	sync.h
 */

#define _REENTRANT

#include "sync.h"
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Spinlock routines
 */

int my_spinlock_init(my_spinlock_t *lock)
{
	lock->owner = 0;
	lock->locked = 0;
}

int my_spinlock_destroy(my_spinlock_t *lock)
{
}

int my_spinlock_unlock(my_spinlock_t *lock)
{
	//owner must be reset FIRST (before resetting lock) otherwise deadlock
	lock->owner = 0;
	lock->locked = 0;
}

int my_spinlock_lockTAS(my_spinlock_t *lock)
{
	pid_t tid = syscall(SYS_gettid);
	if( lock->owner == tid ) return;
	while( cas(&(lock->locked), 0, 1) ); // returns 1 as long as it is locked
	lock->owner = tid;
}

int my_spinlock_lockTTAS(my_spinlock_t *lock)
{
	pid_t tid = syscall(SYS_gettid);
	if( lock->owner == tid ) return;
	do {
		while ( lock->locked ) continue; // if locked, SKIP the while() check, and just keep checking lock
	} while( tas(&(lock->locked)) );
	// NOTE: the above TTAS does NOT work if it was a while() loop on top rather than do() while();
	lock->owner = tid;
}

int my_spinlock_trylock(my_spinlock_t *lock)
{
}


/*
 * Mutex routines
 */

int my_mutex_init(my_mutex_t *mutex)
{
	mutex->owner = 0;
	mutex->locked = 0;
}

int my_mutex_destroy(my_mutex_t *mutex)
{
}

int my_mutex_unlock(my_mutex_t *mutex)
{
	//owner must be reset FIRST (before resetting lock) otherwise deadlock
	mutex->owner = 0;
	mutex->locked = 0;
}

int my_mutex_lockTAS(my_mutex_t *mutex)
{	
	pid_t tid = syscall(SYS_gettid);
	if( mutex->owner == tid ) return;
	useconds_t delay = tid%20;
	while( cas( &(mutex->locked), 0, 1) ) {
		usleep(delay);
		if( delay < 100000 ) {
			delay *= 2;
		}
	}
	mutex->owner = tid;
}


int my_mutex_lockTTAS(my_mutex_t *mutex)
{
	pid_t tid = syscall(SYS_gettid);
	if( mutex->owner == tid ) return;
	useconds_t delay = tid%20;
	while( 1 ) {
		while( mutex->locked ) {
			usleep(delay);
			if( delay < 100000 ) {
				delay *= 2;
			}
		}
		if( !cas( &(mutex->locked), 0, 1) ) break;
	}
	mutex->owner = tid;
}

int my_mutex_trylock(my_mutex_t *mutex)
{
}


