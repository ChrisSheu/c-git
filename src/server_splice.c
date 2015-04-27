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

int pipefd[2];

void *pipe_thread(void)
{
    printf("enter pipe_thread\n");

    while(1)
    {

    }

    pthread_exit("exit pipe_thread\n");
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    char readBuff[1025];
    time_t ticks;

    char c = 0;
    int ret =-1;


    if(argc < 2)
    {
        printf("\n(%s:%d)\033[0;34m exec : ./server <Port>\033[m\n",__func__,__LINE__);
        return -1;
    }

    ret = pipe(pipefd);
    if(ret < 0)
    {
        printf("pipe error %d, %s", ret, strerror(errno)); 
        return -1;
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    memset(readBuff, '0', sizeof(readBuff));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);


    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if(connfd < 0)
        {
            printf("connfd error...%s\n", strerror(errno));
        }
        else
        {
            //splice return 0-written length; error -1, check errno.
            ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
            if(ret < 0)
            {
                printf("splice error...%d, %s\n", ret, strerror(errno));
            }
        }
/*
        while((ret = read(connfd, &c, 1)) > 0)
        {
            printf("%c", c);
        }
*/
        close(connfd);
    }

    close(filefd);
}

