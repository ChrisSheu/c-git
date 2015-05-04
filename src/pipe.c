#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

/* [pipe] function.
 * 1.one-way pipe , fd[0]:read, fd[1]:write.
 * 2.success 0, else -1 and check errno.
 * 3.Buffer is 4096 bytes before linux 2.6.11, and then 65535 bytes.
 * 4.I/O FIFO method.
 * 5.when end of read have been closed according TCP protocol RST response packet, a write will cause a SIGPIPE signal to be generated.
 */

void help()
{
    printf("\n It's a pipe example, and how to use pipe feature to write and read.\n");
    printf("./pipe \n");
    printf("<key in something>\n\n\n");
}

void readpipe(void *r_fd)
{
    int ret = 0;
    int ret2 = 0;
    char in;

    printf("\n(%s:%d) Enter readpipe thread pid:%d!!\n",__func__,__LINE__, getpid());

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
int open_read_thread(int *readfd)
{
    int ret = 0;
    pthread_t id;

    ret = pthread_create(&id,NULL,(void *) readpipe, readfd);
    if(ret!=0){
        printf ("Create pthread[readpipe] error!\n");
        return -1;
    }

    return ret;
}

int init_pipe(int *p)
{
    int ret = -1;

    ret = pipe(p);
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
                printf("\n(%s:%d)\033[0;33m pipe return unknown value.. %d\033[m\n",__func__,__LINE__, ret);
                break;
        }
        /* init pipe error. */
        return ret;
    }

    return ret;
}

int goto_write(int wp)
{
    int readc;
    printf("\n(%s:%d)\033[0;33m key something into pipe\033[m\n",__func__,__LINE__);
    //write into pipe[1]
    while(1)
    {
        if((readc = fgetc(stdin)) != EOF)
        {
            write(wp, &readc, 1);
        }
    }

    printf("\n(%s:%d)\033[0;33m exit that write into pipe\033[m\n",__func__,__LINE__);

    return -1;
}

int start_pipe(int *p)
{
    int ret = -1;

    /* Init pipe. */
    ret = init_pipe(p);
    if(ret < 0)
    {
        printf ("init_pipe error!...%d\n", ret);
        return ret;
    }

    //open a thread to prepare for read pipe[0] content.
    ret = open_read_thread(&p[0]);
    if(ret < 0)
    {
        printf ("open_read_thread error!...%d\n", ret);
        return ret;
    }


    ret = goto_write(p[1]);

    return ret;

}

int main(int argc, char *argv[])
{
    int mypipe[2];
    int ret;
    int c;

    printf("\n\n argc: %d \n", argc);

    while((c = getopt(argc, argv, "h::")) != -1)
    {
        switch(c)
        {
            case 'h':
                help();
                exit(0);
            default:
                break;
        }
    }

    ret = start_pipe(mypipe);

    return 0;

}
