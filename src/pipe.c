#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>


void readpipe(void *r_fd)
{
    int ret = 0;
    int ret2 = 0;
    char in;

    printf("\n(%s:%d)\033[0;33m Enter readpipe thread pid:%d!!\033[m\n",__func__,__LINE__, getpid());

    while(1)
    {   //read char from pipe until get '\n'.
        ret = read(*((int *)r_fd), &in, 1);
        if((ret > 0) && (in != '\n'))
        {
            printf("\033[0;32m readpipe: \033[m");

            do
            {
                if(in != '\n')
                {
                    
                    printf("%c", in);
                }
                else
                    break;
            }
            while((ret2 = read(*((int *)r_fd), &in, 1)) >=0);

            printf("\n");

        }
    }

    printf("\n(%s:%d)\033[0;33m readpipe thread broken...\033[m\n",__func__,__LINE__);

}

//open thread.
int init(int *readfd)
{
    int ret = 0;
    pthread_t id;

    ret = pthread_create(&id,NULL,(void *) readpipe, readfd);
    if(ret!=0){
        printf ("Create pthread error!\n");
        return -1;
    }

    return ret;
}


int main(int argc, char *argv[])
{
    int mypipe[2];

    int ret;

    ret = pipe(mypipe);
    /* Init pipe. */
    if(ret != 0)
    {
        printf ("pipe error!...%d, %s\n", ret, (char *)strerror(errno));
        switch(ret)
        {
            case EMFILE:
                printf("Too many files descriptors are in use by the process....\n");
                break;
            case ENFILE:
                printf("The system limit on the total number of open files has been reached....\n");
                break;
            case EFAULT:
                printf("pipefd is not valid....\n");
                break;
            case EINVAL:
                printf("(pipe2()) Invalid value in flags....\n");
                break;
            default:
                printf("\n(%s:%d)\033[0;33m pipe unknown return %d\033[m\n",__func__,__LINE__, ret);
                break;
        }

        return -1;
    }

    ret = init(&mypipe[0]);
    if(ret < 0)
    {
        printf ("init error!...%d\n", ret);
        return -1;
    }

    printf("write into pipe\n");

    int readc;

    //write pipe
    while(1)
    {
        if((readc = fgetc(stdin)) != EOF)
        {
            write(mypipe[1], &readc, 1);
        }
    }

    return 0;

}
