#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define CMD_LEN           256
#define MCU_DEV_STR_LEN   20
#define SELECT_TIMEOUT    2
static char MCU_DEV[][MCU_DEV_STR_LEN] = { "test.file", "test.file2"};

int mcu_dev_init( char *src_mcu_dev_str)
{
    int fd = -1;

    if(src_mcu_dev_str == NULL)
    {
        printf("argument is invalid...\n");
        return -1;
    }

    //TODO for Debug.
    printf("init: %s \n",  src_mcu_dev_str);
    fd = open( src_mcu_dev_str, O_RDONLY);

    return fd;
}

int append_fdset(int *src_arr_fd, int src_arr_fd_len, fd_set *dst_fds)
{
    int i = -1;

    if((src_arr_fd_len <= 0) || (src_arr_fd == NULL) || (dst_fds == NULL))
    {
        printf(" argument is invalid... \n");
        return -1;
    }

    FD_ZERO( dst_fds);
 printf("\n(%s:%d)\033[0;34m======= ========\033[m\n",__func__,__LINE__);
    for( i = 0; i < src_arr_fd_len; i++)
    {
        printf("\n(%s:%d)\033[0;34m append %d \033[m\n",__func__,__LINE__, src_arr_fd[i]);
        FD_SET( src_arr_fd[i], dst_fds);
    }

    return 0;
}

int get_maxfd(int *dst_max_fd, int *src_arr_fd, int src_arr_fd_len)
{
    int i = -1;
    if((src_arr_fd_len <= 0) || (src_arr_fd == NULL) || (dst_max_fd == NULL))
    {
        printf(" argument is invalid... \n");
        return -1;
    }

    *dst_max_fd = -1;

    for( i = 0; i < src_arr_fd_len; i++)
        *dst_max_fd = (*dst_max_fd > src_arr_fd[i])? *dst_max_fd: src_arr_fd[i];

    return 0;
}

int get_ready_fd(int *arr_fd, int arr_fd_len, fd_set *fds)
{
    int fd = -1;
    int i  = -1;

    if((arr_fd_len <= 0) || (arr_fd == NULL) || (fds == NULL))
    {
        printf(" argument is invalid... \n");
        return -1;
    }

    for( i = 0; i < arr_fd_len; i++)
    {
        if(FD_ISSET(arr_fd[i], fds))
        {
            fd = arr_fd[i];
            printf("\n(%s:%d)\033[0;34m %d fd:%d \033[m\n",__func__,__LINE__, i, fd);
        }
    }

    return fd;
}

int main(void)
{
    int mcu_fd[sizeof(MCU_DEV) / MCU_DEV_STR_LEN];
    int mcu_fd_num = sizeof(MCU_DEV) / MCU_DEV_STR_LEN;
    fd_set rfds;
    int max_fd = -1;
    struct timeval tv;
    int retval = -1;
    int i = -1;
    char cmd_str[CMD_LEN] = {0};


    if( mcu_fd_num <= 0)
    {
        printf( "MCU Device not found \n");
        return -1;
    }

    //init MCU device.
    for( i = 0; i < mcu_fd_num; i++)
    {
        mcu_fd[i] = mcu_dev_init( MCU_DEV[i]);
        if(mcu_fd[i] < 0)
        {
            printf(" mcu dev init fail...%s \n", MCU_DEV[i]);
            return -2;
        }

    }

    //start to listening...
    while(1)
    {
        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        retval = append_fdset( mcu_fd, mcu_fd_num, &rfds);
        if(retval < 0)
        {
            printf(" append fdset fail...%d \n", retval);
        }

        retval = get_maxfd( &max_fd, mcu_fd, mcu_fd_num);
        if(retval < 0)
        {
            printf(" get maxfd fail...%d \n", retval);
            return -3;
        }

        /* Wait up to five seconds. */
        tv.tv_sec  = SELECT_TIMEOUT;
        tv.tv_usec = 0;
        switch( select( max_fd +1, &rfds, NULL, NULL, &tv))
        {
            case -1:
                printf(" select fail...errno = %d, %s \n", errno, strerror(errno));
                break;
            case 0:
                printf(" No mcu data within [%d] seconds and continue\n", SELECT_TIMEOUT);
                break;
            default:
                {
                    //TODO get data. 
                    int tmpfd = -1;
                    tmpfd = get_ready_fd( mcu_fd, mcu_fd_num, &rfds);
                    if(tmpfd < 0)
                    {
                        printf(" get ready mcu-fd fail...%d \n", tmpfd);
                        break;
                    }

 printf("\n(%s:%d)\033[0;34m tmpfd:%d \033[m\n",__func__,__LINE__, tmpfd);
                    retval = read( tmpfd, cmd_str, CMD_LEN);
                    if(retval < 0)
                    {
                        printf(" read ready mcu-fd fail...ret:%d, errno:%d, errstr:%s \n", retval, errno, strerror(errno));
                        break;
                    }
                    else if(retval == 0)
                            break;
                    else
                    {
                        printf("\n(%s:%d)\033[0;34m capture string:%s \033[m\n",__func__,__LINE__, cmd_str);
                    }
                }
                break;
        }

        sleep(1);
    }

    exit(EXIT_SUCCESS);

}
