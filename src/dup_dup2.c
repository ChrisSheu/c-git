/*
 * dup            : return new descriptor.
 * dup2(des, src) : src refer to des pointer.
 *
 * warning:
 */
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int write_lock(int tmpfd)
{
    struct flock lock;
    int ret = -1;

    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = 0;
    lock.l_len    = 0;
    lock.l_pid    = getpid();
    ret = fcntl(tmpfd, F_SETLK, &lock);
    if(ret < 0)
    {
        printf("\n(%s:%d)\033[0;33m fail. %s\033[m\n",__func__,__LINE__, strerror(errno));
        return ret;
    }

    return tmpfd;
}

int main(int argc, char *argv[])
{
    int fd, save_fd;
    char msg[] = "HelloWorld\n";

    int ret = -1;

    fd = open("somefile", O_RDWR | O_CREAT);
    if(fd<0) {
        perror("open");
        exit(1);
    }

#ifdef FCNTL_LOCK
    fd = write_lock(fd);
    if(fd < 0)
    {
        printf("\n(%s:%d)\033[0;33m write lock fail...%d\033[m\n",__func__,__LINE__, ret);
        return 0;
    }
struct flock lock;
fcntl(fd, F_GETLK, lock);
if(lock.l_type==F_RDLCK)
 printf("\n(%s:%d)\033[0;33m=========RDLCK==================\033[m\n",__func__,__LINE__);
else if(lock.l_type==F_WRLCK)
 printf("\n(%s:%d)\033[0;33m=========WRLCK==================\033[m\n",__func__,__LINE__);
else
 printf("\n(%s:%d)\033[0;33m============unknown===============\033[m\n",__func__,__LINE__);

    ret = write(fd, "I write again\n", strlen("I write again\n"));
    printf("\n(%s:%d)\033[0;33m===========ret=%d===============\033[m\n",__func__,__LINE__, ret);
#endif

/*  in book.....
    close(STDOUT_FILENO);
    dup(save_fd);                                     //save_fd refer stdout[1].
*/

    save_fd = dup(STDOUT_FILENO);                     //save_fd refer stdout[1].
    write(save_fd, "Hello\n", strlen("Hello\n"));     //write "Hello\n" into   save_fd -> stdout.

    close(save_fd);
    dup2(fd, save_fd);                                //save_fd refer fd[somefile]
    write(save_fd, "save_fd\n", strlen("save_fd\n")); //write "save_fd\n" into save_fd.
    write(fd, "fd\n", strlen("fd\n"));                //write "fd\n"      into      fd.

    dup2(STDOUT_FILENO, fd);                          //fd refer stdout[1].
    write(fd, msg, strlen(msg));                      //write  msg        into      fd -> stdout.


    printf("Press Any Key to Stop");
    getchar();


    return 0;

}

