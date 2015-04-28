/* function [splice]  ssize_t splice(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);
 *
 * fd_in  : src fd.    if it's fd of pipe, off_in = NULL; off_in = offset.
 * fd_out : target fd. if it's fd of pipe, off_in = NULL; off_in = offset.
 * len    : copy length.
 * flags  :
 *         [SPLICE_F_NONBLOCK] :
 *         [SPLICE_F_MORE]     : more data.
 *         [SPLICE_F_MOVE]     : The initial implementation of  this  flag  was  buggy:  therefore starting  in  Linux  2.6.21  it  is  a  no-op.
 *         [SPLICE_F_GIFT]     : unused.
 *
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

int readpipe(int rfd)
{
    int ret = 0;
    int ret2 = -1;

    char in = 0;
    static int filefd = -1;

    if(filefd < 0)
    {
        filefd = open("inputfile", O_CREAT | O_RDWR, 0755);
        if(filefd < 0)
        {
            printf("file open error...%d, %s \n", filefd, strerror(errno));
            return -1;
        }
    }

    //splice return 0-written length; error -1, check errno.
    ret = splice(rfd, NULL, filefd, NULL, 15, SPLICE_F_MORE | SPLICE_F_MOVE);
    if(ret < 0)
    {
        printf("splice error...%d, %s\n", ret, strerror(errno));
        return -2;
    }
    else
    {
        printf("\n(%s:%d)\033[0;34m write file success!, splice=%d\033[m\n",__func__,__LINE__, ret);  
    }

    return 0;

}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char c = 0;
    int ret  = -1;
    int ret2  = -1;

    int pipefd[2];


    if(argc < 2)
    {
        printf("\n(%s:%d)\033[0;34m exec : ./server <Port>\033[m\n",__func__,__LINE__);
        return -1;
    }

    ret = pipe(pipefd);
    if(ret < 0)
    {
        printf ("pipe error!...%d\n", ret);
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
            while((ret = splice(connfd, NULL, pipefd[1], NULL, 15, SPLICE_F_MORE | SPLICE_F_MOVE)) > 0)
            {
                printf("\n(%s:%d)\033[0;34m write pipe success!, splice=%d\033[m\n",__func__,__LINE__, ret);
                ret2 = readpipe(pipefd[0]);
                if(ret2 < 0)
                {
                    printf("readpipe error...%d\n", ret2);
                }
            }
            //error return -1;
            if(ret < 0)
            {
                printf("splice error...%d, %s\n", ret, strerror(errno));
            }
            else //Success, ret == 0, A return value of 0 means that there was no data to transfer.
            {}
        }

        close(connfd);
    }

}

