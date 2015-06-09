/*
 * This file is used for receive/send device command via select.
 *
 * TODO select    : 如果/dev/mytest 是一個檔案，select always return > 0. refer to http://stackoverflow.com/questions/7022810/select-doesnt-wait-any-changes.
 * TODO POSIX say : File descriptors associated with regular files shall always select true for ready to read, ready to write, and error conditions.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/termios.h>

#define CMD_LEN           256
#define MCU_DEV_STR_LEN   20
#define SELECT_TIMEOUT    2

//input want handle devices.
static char MCU_DEV[][MCU_DEV_STR_LEN] = { "/dev/mytest", "/dev/mytest2"};

/*
 * this function will return fd of device.
 *
 * @src_mcu_dev_str : the string array of device path.
 *
 */
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
    fd = open( src_mcu_dev_str, O_RDONLY | O_NONBLOCK);

    return fd;
}

/*
 * Append fd in fd_set.
 *
 * @src_arr_fd     : all device fds.
 * @src_arr_fd_len : the length of fds.
 * @dst_fds        : target fd_set.
 *
 */
int append_fdset(int *src_arr_fd, int src_arr_fd_len, fd_set *dst_fds)
{
    int i = -1;

    if((src_arr_fd_len <= 0) || (src_arr_fd == NULL) || (dst_fds == NULL))
    {
        printf(" argument is invalid... \n");
        return -1;
    }

    for( i = 0; i < src_arr_fd_len; i++)
        FD_SET( src_arr_fd[i], dst_fds);

    return 0;
}

/*
 * this function will return the maximum in this fd array.
 *
 * @dst_max_fd     : target maximum.
 * @src_arr_fd     : the fd array.
 * @src_arr_fd_len : the length of fd array.
 *
 */
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
    int select_ret = -1;

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
        switch( (select_ret = select( max_fd +1, &rfds, NULL, NULL, &tv)))
        {
            case -1:
                printf(" select fail...errno = %d, %s \n", errno, strerror(errno));
                break;
            case 0:
                printf(" No mcu data within [%d] seconds and continue\n", SELECT_TIMEOUT);
                break;
            default:
                {
                    int tmpfd = -1;
                    //TODO get data.
                    for( i = 0; i < mcu_fd_num; i++)
                    {
                        if(FD_ISSET( mcu_fd[i], &rfds))
                        {
                            tmpfd = mcu_fd[i];
                            if(tmpfd < 0)
                            {
                                printf(" get ready mcu-fd fail...%d \n", tmpfd);
                                break;
                            }

                            retval = read( tmpfd, cmd_str, sizeof(cmd_str));
                            if(retval < 0)
                                printf(" read ready mcu-fd fail...ret:%d, errno:%d, errstr:%s \n", retval, errno, strerror(errno));
                            else if( retval == 0)
                            {
                                //printf(" finish errno:%d str:%s \n", errno, strerror(errno));
                                continue;
                            }
                            else
                                printf("\n(%s:%d)\033[0;34m select_ret:%d fd:%d capture string:%s \033[m\n",__func__,__LINE__, select_ret, tmpfd, cmd_str);
                        }
                    }
                }
                break;
        }

        sleep(1);
    }

    return 0;
}
