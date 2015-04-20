#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include "ipcshm.h"
#include <unistd.h>

static int showShm(SHARED_MEMORY_CONTENT *shmObj)
{
    int i;

    for( i = 0; i < PACKAGE_LEN; i++)
        printf("\n(%s:%d)\033[0;33m==%d==\ntime:%.24s\nid:%d\nmsg:%s\n\033[m",__func__,__LINE__, i, (char *)ctime(&(shmObj->p[i].ticks)), shmObj->p[i].id, shmObj->p[i].msg);

    return 0;
}

static int clearShm(SHARED_MEMORY_CONTENT *shmObj)
{
    int i;

    for( i = 0; i < PACKAGE_LEN; i++)
        memset(&(shmObj->p[i]), 0, sizeof(PACKAGE));

    return 0;
}

void help()
{
    printf("It's a shared memory init and read example.\n");
    printf("\n");
    printf("-t,    interval, print shared memory content every [t] seconds.\n");
    printf("\n");
    printf("example(every 2 seconds): #./shm_read -t 2\n");
}

int shm_start(char *inter_str)
{
    void *shared;
    SHARED_MEMORY_CONTENT *gShm;
    int shm_id;
    int interval;

    interval = atoi(inter_str);
    //default every 3 seconds
    if(interval == 0)
        interval = DEFAULT_SEC;

    if((shm_id = shmget((key_t)SHM_KEY, sizeof(SHARED_MEMORY_CONTENT), 0666 | IPC_CREAT)) == -1)
    {
        printf("%s(%d) Get share memory fail \n",__func__, __LINE__);
        return -1;
    }

    shared = (void *)shmat(shm_id,(void *)0,0);
    gShm = (SHARED_MEMORY_CONTENT *)shared;

    clearShm(gShm);

    while(1)
    {
        shared = (void *)shmat(shm_id,(void *)0,0);
        gShm = (SHARED_MEMORY_CONTENT *)shared;
        showShm(gShm);
        sleep(interval);
    }

    return 0;
}

int main(int argc, char **argv)
{
    int c;
    printf("argc: %d\n", argc);
    while((c = getopt ( argc, argv, "h::t:")) != -1)
    {
        switch(c)
        {
            case 'h':
                help();
                break;
            case 't':
                shm_start(optarg);
                break;
            default:
                help();
                break;
        }
    }

    if(argc < 2)
    {
        help();
        exit(0);
    }

    return 0;
}

