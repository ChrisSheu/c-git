/*
 * 主、子 線程互相等待 互斥鎖 a, b 的解除，導致兩方都在等待對方將互斥鎖解除，形成死鎖。
 *
 * TODO 如果將sleep函數拿掉，也許可往下繼續執行，但潛伏著一個BUG的存在。
 *
 */
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int a = 0;
int b = 0;

pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;

void * another( void* arg)
{
    pthread_mutex_lock( &mutex_b);   //子線程佔有互斥鎖b
    printf( "in child thread, got mutex b, waiting for mutex a\n");
    sleep(5);                        //等5秒
    ++b;
    pthread_mutex_lock( &mutex_a);   //子線程佔有互斥鎖a
    b += a++;
    pthread_mutex_unlock( &mutex_a);
    pthread_mutex_unlock( &mutex_b);

    pthread_exit( NULL);
}

int main()
{
    pthread_t id;

    pthread_mutex_init( &mutex_a, NULL);
    pthread_mutex_init( &mutex_b, NULL);

    pthread_create( &id, NULL, another, NULL);

    pthread_mutex_lock( &mutex_a);    //主線程佔有互斥鎖a

    printf(" in parent thread, got mutex a, waiting for mutexb \n");

    sleep(5);                         //等5秒
    ++a;
    pthread_mutex_lock( &mutex_b);    //主線程佔有互斥鎖b
    a+= b++;

    pthread_mutex_unlock( &mutex_b);
    pthread_mutex_unlock( &mutex_a);

    pthread_join( id, NULL);
    pthread_mutex_destroy( &mutex_a);
    pthread_mutex_destroy( &mutex_b);


    return 0;
}
