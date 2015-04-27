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

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    char readBuff[1025];
    time_t ticks;

    char c;
    int ret =-1;

    if(argc < 2)
    {
        printf("\n(%s:%d)\033[0;34m exec : ./server <Port>\033[m\n",__func__,__LINE__);
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

        while((ret = read(connfd, &c, 1)) > 0)
        {
            printf("%c", c);
        }

        close(connfd);
    }
}

