/* function [tee]  ssize_t tee(int fd_in, int fd_out, size_t len, unsigned int flags);
 *
 * fd_in  : src fd.
 * fd_out : target fd.
 * len    : copy length.
 * flags  :
 *         [SPLICE_F_NONBLOCK] :
 *         [SPLICE_F_MORE]     : more data.
 *         [SPLICE_F_MOVE]     : The initial implementation of  this  flag  was  buggy:  therefore starting  in  Linux  2.6.21  it  is  a  no-op.
 *         [SPLICE_F_GIFT]     : unused.
 *
 * warning: fd_in and fd_out are refer to pipe, but not in the same.
 * [EINVAL] fd_in or fd_out does not refer to a pipe; or fd_in and fd_out refer to the same pipe.
 */
#define _GNU_SOURCE
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>


int readpipe_to_stdoutpipe(int srcfd, int desfd)
{
    int ret = 0;
    int ret2 = -1;

    char in = 0;

    //TODO [tee]   srcfd transfer to desfd. return a number of bytes are written.
    ret = tee(srcfd,  desfd,  32768, SPLICE_F_NONBLOCK);
    if(ret < 0)
    {
        printf("\n(%s:%d)\033[0;34m tee error...%d, %s \033[m\n",__func__,__LINE__, ret, strerror(errno));
        return -2;
    }

    return 0;

}

void stdoutpipe(void *r_fd)
{
    int ret = 0;
    int ret2 = 0;
    char in;
    static int idx = 0;

    printf("\n(%s:%d)\033[0;33m Enter stdoutpipe thread pid:%d!!\033[m\n",__func__,__LINE__, getpid());

    while(1)
    {
        ret = splice(*((int *)r_fd), NULL, STDOUT_FILENO,  NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        if(ret < 0)
        {
            printf("\n(%s:%d)\033[0;34m splice error...%d, %s \033[m\n",__func__,__LINE__, ret, strerror(errno));
        }
        else
        {
            printf("\n(%s:%d)\033[0;32mpipestdout\033[m -> \033[0;33mstdout\033[m success!, splice=%d\n",__func__,__LINE__, ret);
        }
    }

    printf("\n(%s:%d)\033[0;33m stdoutpipe thread broken...\033[m\n",__func__,__LINE__);

}

//open thread.
int init(int *readfd)
{
    int ret = 0;
    pthread_t id;

    ret = pthread_create(&id,NULL,(void *) stdoutpipe, readfd);
    if(ret!=0){
        printf ("Create pthread error!\n");
        return -1;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char c = 0;
    int ret  = -1;
    int ret2  = -1;

    int pipefd[2];
    int pipestdout[2];

    if(argc < 2)
    {
        printf("\n(%s:%d)\033[0;34m exec : ./mytee <Port>\033[m\n",__func__,__LINE__);
        return -1;
    }

    //init socket pipe.
    ret = pipe(pipefd);
    if(ret < 0)
    {
        printf ("pipe error!...%d\n", ret);
        return -1;
    }

    //init socket pipe.
    ret = pipe(pipestdout);
    if(ret < 0)
    {
        printf ("pipestdout error!...%d\n", ret);
        return -1;
    }

    //create thread pipestdout[0] for stdout.
    ret = init(&pipestdout[0]);
    if(ret < 0)
    {
        printf ("init pipestdout error!...%d\n", ret);
        return -1;
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
    {
        printf ("socket error!...%d\n", listenfd);
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    ret = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret < 0)
    {
        printf ("bind error!...%d\n", ret);
        return -1;
    }

    ret = listen(listenfd, 10);
    if(ret < 0)
    {
        printf ("listen error!...%d\n", ret);
        return -1;
    }


    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if(connfd < 0)
        {
            printf("connfd error...%s\n", strerror(errno));
        }
        else
        {
            //clinet send twice, server recvice twice.
            //splice return 0-written length; error -1, check errno.
            while((ret = splice(connfd, NULL, pipefd[1],  NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE)) > 0)
            {
                printf("\n\n(%s:%d)===========================",__func__,__LINE__);  
                printf("\n(%s:%d)\033[0;34m connfd\033[m -> \033[0;31mpipefd\033[m success!, splice=%d",__func__,__LINE__, ret);
                ret2 = readpipe_to_stdoutpipe(pipefd[0], pipestdout[1]);
                if(ret2 < 0)
                {
                    printf("readpipe error...%d\n", ret2);
                }
                else
                    printf("\n(%s:%d)\033[0;34m \033[0;31mpipefd\033[m -> \033[0;32mpipestdout\033[m success!, tee=%d\033[m",__func__,__LINE__, ret);
            }
            //error return -1;
            if(ret < 0)
            {
                printf("\n(%s:%d)\033[0;34m splice error...%d, %s \033[m\n",__func__,__LINE__, ret, strerror(errno));
            }
            else //Success, ret == 0, A return value of 0 means that there was no data to transfer.
            {}
        }

        close(connfd);
    }

}

