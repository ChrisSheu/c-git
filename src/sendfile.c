/* Function : sendfile(int out_fd, int in_fd, off_t offset, size_t count)
 *
 * out_fd : must be a socket.
 * in_fd  : a real file descriptor.
 * offset : start reading data.
 * count  : total bytes to copy from in_fd to out_fd.
 *
 * [warning]:
 *
 * in_fd Buffer size fail =  0x10000000[268435456 bytes](success...) ~ 0x20000000[536870912 bytes](fail...)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int sendfile_init_config(int fd, struct stat *st, off_t *offset)
{
    int ret = -1;

    /* sendfile config. */
    memset(st, 0, sizeof(struct stat));
    *offset = 0;                         //start position.
    ret = fstat(fd, st);                 //get file stat.
    if(ret < 0)
    {
        printf("get fd fstat fail...%d", ret);
        return ret;
    }

    return ret;
}

int main(int argc, char**argv)
{
    int sockfd = -1;
    struct sockaddr_in servaddr;
    int ret = -1;

    /* sendfile init */
    int filefd = -1;
    struct stat stat_buf;
    off_t offset = 0;

    if (argc <= 2)
    {
        printf("usage:  client <IP address> <Port>\n");
        exit(1);
    }

    /* init file */
    filefd = open("somefile", O_RDWR);  //init file descriptor.

    /* init socket */
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        printf("init socket fail...%d, %s\n", sockfd, strerror(errno));
        return -1;
    }

    /* server config */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    servaddr.sin_port=htons(atoi(argv[2]));

    ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(ret < 0)
    {
        printf("\n(%s:%d)\033[0;33m Connect fail...%d \033[m\n",__func__,__LINE__, ret);
        return -1;
    }
    else
        printf("\n(%s:%d)\033[0;33m Connect to [%s] Port:%d successfully!\033[m\n",__func__,__LINE__, argv[1], atoi(argv[2]));

    // init sendfile config 
    ret = sendfile_init_config(filefd, &stat_buf, &offset);
    if(ret < 0)
    {
        printf("sendfile_init_config fail...%d\n", ret);
        return -1;
    }

    ret = sendfile(sockfd, filefd, &offset, stat_buf.st_size);
    if(ret < 0)
        printf("\n(%s:%d)\033[0;33m sendfile fail...%d\033[m\n",__func__,__LINE__, ret);
    else
        printf("sendfile success...%d\n", ret);

    /* fd [][][][][][][][][] */
    /*                    ↑ */
    /*                 offset*/
    /* reset offset in filefd , otherwise send nothing to  sockfd */
    /* init sendfile config */

    ret = sendfile_init_config(filefd, &stat_buf, &offset);
    if(ret < 0)
    {
        printf("sendfile_init_config fail...%d\n", ret);
        return -1;
    }
    /* after sendfile_init_config */
    /* fd [][][][][][][][][]      */
    /*    ↑                      */
    /*  offset                    */
    ret = sendfile(sockfd, filefd, &offset, stat_buf.st_size);
    if(ret < 0)
        printf("\n(%s:%d)\033[0;33m sendfile fail...%d\033[m\n",__func__,__LINE__, ret);
    else
        printf("sendfile success...%d\n", ret);

    close(filefd);
    close(sockfd);
}

