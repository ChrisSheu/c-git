#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

pthread_mutex_t mutex;

void* another(void* arg)
{
    printf( " in child thread, lock the mutex \n");

    pthread_mutex_lock( &mutex);

    sleep( 5 );

    pthread_mutex_unlock( &mutex);
    
}

int main()
{
    pthread_mutex_init( &mutex, NULL);

    pthread_t id;

    pthread_create( &id, NULL, another, NULL);

    /* 父進程暫停1s，確保在fork之前，子線程已開始運行並獲得互斥變量 */
    sleep(1);

    int pid = fork();

    if(pid < 0)
    {
        pthread_join( id, NULL);
        pthread_mutex_destroy( &mutex);
        return 1;

    }
    else if(pid == 0) /* 子進程 */
    {
        /* 子進程從父進程繼承了互斥鎖mutex的狀態，該互斥鎖鎖住的狀態，這是由父進程中的子線程執行pthread_mutex_lock引起的，因此
           下面會一直阻塞，盡管從邏輯上來說它是不應該阻塞的。*/
        printf( "I am in the child, want to get the lock \n"); 
        pthread_mutex_lock( &mutex);
        printf( " I can not run to here, opp.... \n");
        exit(0);
    }
    else             /* 父進程 */
    {
        wait( NULL);
    }

    pthread_join( id, NULL);
    pthread_mutex_destroy( &mutex);

    return 0;
}
