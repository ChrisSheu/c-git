#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "readv_writev.h"

int main(int argc, char *argv[])
{
    struct iovec iov[IO_VECTOR_LEN];
    char *buf[IO_VECTOR_LEN];
    char *str;
    ssize_t nread, nwrite;
    int i = 0, j = 0;

//char **buf;
//    buf = malloc(sizeof(char *) * IO_VECTOR_LEN);

    //init buffer.
    for(i = 0; i < IO_VECTOR_LEN; i++)
    {
        buf[i] = malloc(sizeof(char) * BUF_LEN);
        if(buf[i] == NULL)
        {
            for(j = 0; j <= i; j++)
            {
                free(buf[j]);
            }

            return -1;
        }
    }

    //clear buffer.
    for(i = 0; i < IO_VECTOR_LEN; i++)
        memset(buf[i], '\0', BUF_LEN);


    //config iovec array.
    for(i = 0; i < IO_VECTOR_LEN; i++)
    {
        memset(buf[i], '\0', BUF_LEN);
        iov[i].iov_base = buf[i];
        iov[i].iov_len  = BUF_LEN;
    }

    //read stdin.
    nread = readv(STDIN_FILENO, iov, IO_VECTOR_LEN);
    if(nread == -1)
    {
        printf("readv error....%s\n", strerror(errno));
        goto fail;
    }
    else
    {
        printf("readv:\n");
        for(i = 0; i < IO_VECTOR_LEN; i++)
            printf("buf%d is: %s\t length is: %d\n",i, buf[i], strlen(buf[i]));
    }

    //write stdout.
    printf("writev:\n");
    nwrite = writev(STDOUT_FILENO, iov, IO_VECTOR_LEN);
    if(nwrite == -1)
    {
        printf("writev error....%s\n", strerror(errno));
        goto fail;
    }
    else
        printf("\n");

    //free buffer.
    for(j = 0; j < IO_VECTOR_LEN; j++)
    {
        printf(" free buffer...%d \n", j);
        free(buf[j]);
    }
    return 0;

fail:
    for(j = 0; j < IO_VECTOR_LEN; j++)
    {
        printf(" free buffer...%d \n", j);
        free(buf[j]);
    }
    return -1;

}

