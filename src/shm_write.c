#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <ipcshm.h>
#include <unistd.h>

static int insertShm(PACKAGE *packObj, int *index, time_t *t, char *msg)
{

    memcpy(&(packObj->id), index, sizeof(int));
    memcpy(&(packObj->ticks), t, sizeof(time_t));
    memcpy(&(packObj->msg), msg, MSG_LEN);

    return 0;
}

static int shm_write()
{
    void *shared;
    SHARED_MEMORY_CONTENT *gShm;
    int shm_id;
    int id;
    int ret;
    char line[MSG_LEN];
    time_t tt;

    if((shm_id = shmget((key_t)SHM_KEY, sizeof(SHARED_MEMORY_CONTENT), 0666 | IPC_CREAT)) == -1)
    {
        printf("%s(%d) Get share memory fail \n",__func__, __LINE__);
        return -3;
    }

    shared = (void *)shmat(shm_id,(void *)0,0);
    gShm = (SHARED_MEMORY_CONTENT *)shared;

    while(1)
    {
        //get id.
        printf("Select ID(0~%d), exit(key: -1)", PACKAGE_LEN-1 );
        fgets(line, sizeof(line), stdin);
        ret = sscanf(line, "%d", &id);
        if(!ret)
        {
            printf("Not Integer.\n");
        }
        else{
            if(id == -1)
                break;
            //check ID range is valid.
            else if((id < 0) || (id > (PACKAGE_LEN-1)))
                printf("ID out of range..[%d]\n", id);
            else
            {
                printf("ID[%d] input msg( length: 1~%d)", id, MSG_LEN);
                scanf("%s", line);
                //check msg length is valid.
                if((strlen(line) < 0) || (strlen(line) > MSG_LEN))
                    printf("msg len out of range..[%d]\n", strlen(line));
                else
                {
                    tt     = time(NULL);
                    ret = insertShm(&(gShm->p[id]), &id, &tt, line);
                    if(ret < 0)
                    {
                        printf("\n(%s:%d)\033[0;33m insertShm error...%d\033[m\n",__func__,__LINE__, ret);
                        return -2;
                    }
                    else
                    {
                        printf("\n(%s:%d)\033[0;33m insertShm OK!\033[m\n",__func__,__LINE__);
                        printf("\n(%s:%d)\033[0;33m time:%.24s\033[m\n",__func__,__LINE__, (char *)ctime(&tt));
                        printf("\n(%s:%d)\033[0;33m id:%d\033[m\n",__func__,__LINE__, id);
                        printf("\n(%s:%d)\033[0;33m msg:%s\033[m\n",__func__,__LINE__, line);
                    }
                }
            }
        }
        //clear stdin buff.
        setbuf(stdin, NULL);
    }

    /* detach shared memory */
    if (shmdt(gShm) == -1)
    {
        printf("%s(%d) detach share memory fail \n",__func__, __LINE__);
        return -1;
    }
/*
    //[Warning]
    // destroy shared memory
    if (shmctl(shm_id, IPC_RMID, 0) == -1) {
        printf("%s(%d) destroy share memory fail \n",__func__, __LINE__);
        return -2;
    }
*/
    return 0;
}

int main(int argc, char **argv)
{
    shm_write();

    return 0;
}

