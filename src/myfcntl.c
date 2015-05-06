#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>

/* [fcntl] function.
 * 
 * this example set file descriptor non-blocking and blocking.
 * 
 * 
 * 
 */
void readpipe(void *r_fd)
{
    int ret = 0;
    int ret2 = 0;
    char in;

    static int count = 0;

    printf("\n(%s:%d)\033[0;33m Enter readpipe thread pid:%d!!\033[m\n",__func__,__LINE__, getpid());

    while(1)
    {   //read char from pipe until get '\n'.
        ret = read(*((int *)r_fd), &in, 1);
        if(in == '\n')
        {
            continue;
        }
        else if(ret > 0)
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

        printf("\n(%s:%d)\033[0;33m Thread count=%d ret=%d \033[m\n",__func__,__LINE__, count++, ret);
        memset(&in, '\0', sizeof(in));
        usleep(500000);
    }

    printf("\n(%s:%d)\033[0;33m readpipe thread broken...\033[m\n",__func__,__LINE__);

}

//open thread.
int open_read_thread(int *readfd)
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

int set_fd_nonblocking(int fd)
{
    int ret = -1;

    int old_option = fcntl(fd, F_GETFL);
    if(old_option < 0)
    {
        printf("F_GETFL error...%d, %s \n", old_option, (char *)strerror(errno));
        return -1;
    }

    int new_option = old_option | O_NONBLOCK;
    ret = fcntl( fd, F_SETFL, new_option);
    if(ret < 0)
    {
        printf("F_SETFL error...%d, %s \n", ret, (char *)strerror(errno));
        return -2;
    }

    return 0;
}

int set_fd_blocking(int fd)
{
    int ret = -1;

    int old_option = fcntl(fd, F_GETFL);
    if(old_option < 0)
    {
        printf("F_GETFL error...%d, %s \n", old_option, (char *)strerror(errno));
        return -1;
    }

    int new_option = (old_option & (~O_NONBLOCK));
    ret = fcntl( fd, F_SETFL, new_option);
    if(ret < 0)
    {
        printf("F_SETFL error...%d, %s \n", ret, (char *)strerror(errno));
        return -2;
    }

    return 0;
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

    //open_a thread read pipe.
    ret = open_read_thread(&mypipe[0]);
    if(ret < 0)
    {
        printf ("open_read_thread error!...%d\n", ret);
        return -1;
    }

    printf("write into pipe [$ = nonblocking, * = blocking]\n");

    int readc;

    //write pipe
    while(1)
    {
        if((readc = fgetc(stdin)) != EOF)
        {
            //check specified notation which determine file descriptor blocking or non-blocking.
            if(readc == '$')
            {
                if(ret = set_fd_nonblocking(mypipe[0]) == 0)
                {
                    printf("\n(%s:%d)\033[0;33m Set non-blocking\033[m\n",__func__,__LINE__);  
                }
                else
                    printf("\n(%s:%d)\033[0;33m non-blocking fail...%d \033[m\n",__func__,__LINE__, ret);  
            }
            else if(readc == '*')
            {
                if(ret = set_fd_blocking(mypipe[0]) == 0)
                {
                    printf("\n(%s:%d)\033[0;33m Set blocking\033[m\n",__func__,__LINE__);  
                }
                else
                    printf("\n(%s:%d)\033[0;33m blocking fail...%d \033[m\n",__func__,__LINE__, ret);  
            }

            write(mypipe[1], &readc, 1);
        }
    }

    return 0;

}
