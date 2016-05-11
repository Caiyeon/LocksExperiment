#include "main.h"


void *fnC()
{
    int i;
    for(i=0;i<1000000;i++)
    {   
        c++;
    }
}


pthread_mutex_t count_mutex;
void *pthreadMutexTest()
{
    int i;
    for(i=0;i<numItterations;i++)
    { 	
		pthread_mutex_lock(&count_mutex);
			c++;
		pthread_mutex_unlock(&count_mutex);
    }
}

pthread_spinlock_t pthread_spinlock;
void *pthreadSpinlockTest()
{
	int i;
	for(i=0; i<numItterations; i++) {
		pthread_spin_lock(&pthread_spinlock);
			c++;
		pthread_spin_unlock(&pthread_spinlock);
	}
}

my_spinlock_t count_spinlock;
void *mySpinlockTAStest()
{
	int i;
	for(i=0; i<numItterations; i++) {
		my_spinlock_lockTAS(&count_spinlock);
			c++;
			//printf( "C: %d\n", c );
		my_spinlock_unlock(&count_spinlock);
	}
}

//using the same lock as above: count_spinlock
void* mySpinlockTTAStest()
{
	int i;
	for(i=0; i<numItterations; i++) {
		my_spinlock_lockTTAS(&count_spinlock);
			c++;
		my_spinlock_unlock(&count_spinlock);
	}
}

my_mutex_t count_mymutex;
void* myMutexTAStest()
{
	int i=0;
	for(i=0; i<numItterations; i++) {
		my_mutex_lockTAS(&count_mymutex);
			c++;
		my_mutex_unlock(&count_mymutex);
	}
}

//using the same mutex lock as above: count_mymutex
void* myMutexTTAStest()
{
	int i=0;
	for(i=0; i<numItterations; i++) {
		my_mutex_lockTTAS(&count_mymutex);
			c++;
		my_mutex_unlock(&count_mymutex);
	}
}



int runTest(int testID)
{
	if (testID == 0 || testID == 1 ) /*Pthread Mutex*/
	{
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++)
		{
			if( (rt=(pthread_create( threads+i, NULL, &pthreadMutexTest, NULL))) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("\n\n********************************************\n");
		printf("Threaded Run Pthread (Mutex) Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread Mutex time(ms): %llu\n",result/1000000);
		printf("********************************************\n");
		free (threads);
	}

	if(testID == 0 || testID == 2) /*Pthread Spinlock*/
	{
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result;
		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		pthread_spin_init(&pthread_spinlock, 0);
		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++)
		{
			if( (rt=(pthread_create( threads+i, NULL, &pthreadSpinlockTest, NULL))) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("\n********************************************\n");
		printf("Threaded Run Pthread (Spinlock) Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread Spinlock time(ms): %llu\n",result/1000000);
		printf("********************************************\n");
		free (threads);
	}

	if(testID == 0 || testID == 3) /*MySpinlockTAS*/
	{
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result;
		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		my_spinlock_init(&count_spinlock);
		for(i=0;i<numThreads;i++)
		{
			if( (rt=(pthread_create( threads+i, NULL, &mySpinlockTAStest, NULL))) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("\n********************************************\n");
		printf("Threaded Run MySpinlock (TAS) Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread MySpinlockTAS time(ms): %llu\n",result/1000000);
		printf("********************************************\n");
		free (threads);
	}

	if(testID == 0 || testID == 4) /*MySpinlockTTAS*/
	{
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result;
		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		my_spinlock_init(&count_spinlock);
		for(i=0;i<numThreads;i++)
		{
			if( (rt=(pthread_create( threads+i, NULL, &mySpinlockTTAStest, NULL))) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("\n********************************************\n");
		printf("Threaded Run mySpinlock (TTAS) Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread mySpinlockTTAS time(ms): %llu\n",result/1000000);
		printf("********************************************\n");
		free (threads);
	}

	if(testID == 0 || testID == 5) /*myMutexTAS*/
	{
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result;
		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		my_mutex_init(&count_mymutex);
		time_t t;
		srand((unsigned) time(&t));
		for(i=0;i<numThreads;i++)
		{
			if( (rt=(pthread_create( threads+i, NULL, &myMutexTAStest, NULL))) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("\n********************************************\n");
		printf("Threaded Run myMutex (TAS) Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread myMutexTAS time(ms): %llu\n",result/1000000);
		printf("********************************************\n");
		free (threads);
	}

	if(testID == 0 || testID == 6) /*myMutexTTAS*/
	{
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result;
		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		my_mutex_init(&count_mymutex);
		time_t t;
		srand((unsigned) time(&t));
		for(i=0;i<numThreads;i++)
		{
			if( (rt=(pthread_create( threads+i, NULL, &myMutexTTAStest, NULL))) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("\n********************************************\n");
		printf("Threaded Run myMutex (TTAS) Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread myMutexTTAS time(ms): %llu\n",result/1000000);
		printf("********************************************\n");
		free (threads);
	}

	return 0;
}

int testAndSetExample()
{
	volatile unsigned long test = 0; //Test is set to 0
	printf("Test before atomic OP:%lo\n",test);
		tas(&test);
	printf("Test after atomic OP:%lo\n",test);
}

int processInput(int argc, char *argv[])
{
	/*testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myMutexTTAS*/
	numThreads=4;
	numItterations=1000000;
	testID = 0; // default
	// if the last parameter is within range, assume that is the testid instead
	int i;
	for( i=0; i<argc; i++ ) {
		if( !strncmp( argv[i], "-t", 2*sizeof(char) ) ) {
			if( sscanf( argv[i+1], "%d", &numThreads ) != 1 ) {
				perror( "scanf" );
				numThreads = 4; // default due to error reading
			}
			if( numThreads < 0 || numThreads > 10000 ) {
				printf( "Out of bounds numThreads, resetting to default numThreads=4\n" );
				numThreads = 4;
			}
			i++;
		}
		else if( !strncmp( argv[i], "-i", 2*sizeof(char) ) ) {
			if( sscanf( argv[i+1], "%d", &numItterations ) != 1 ) {
				perror( "scanf" );
				numItterations=1000000; // default due to error reading
			}
			if( numItterations < 0 ) {
				printf( "Out of bounds numItterations, resetting to default numItterations=1,000,000\n" );
				numItterations=1000000;
			}
			i++;
		}
		else if( !strncmp( argv[i], "-d",2*sizeof(char) ) ) {
			if( sscanf( argv[i+1], "%d", &testID ) != 1 ) {
				perror( "scanf" );
				testID = 0; // default due to error reading
			}
			if( testID < 0 || testID > 6 ) {
				printf( "Out of bounds testID, resetting to default testID=0\n" );
				testID = 0;
			}
			i++;
		}
	}
	return 0;
}


int main(int argc, char *argv[])
{
	printf("Usage of: %s -t #threads -i #itterations -d testid\n", argv[0]);
	printf("testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myMutexTTAS\n");	
	
	processInput(argc,argv);
	runTest(testID);
	return 0;
}
