#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    /* l_type   l_whence  l_start  l_len  l_pid   */
    struct flock fl = { F_WRLCK,SEEK_SET,0,      0,    0 };
    int fd;
    int ret = -1;
    char msg[] = "123456\n";

    fl.l_pid = getpid();

    if (argc > 1)
        fl.l_type = F_RDLCK;

    if ((fd = open("testfile",O_RDWR | O_CREAT, 0644)) == -1) {
        perror("open");
        exit(1);
    }

    if (fcntl(fd, F_SETFD, 0) == -1) {
        perror("fcntl");
        exit(1);
    }

    printf("Press  to try to get lock: \n");
    getchar();
    printf("Trying to get lock...\n");

    if (fcntl(fd, F_SETLK, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }

    printf("got lock\n");


    if((ret = write(fd, msg, strlen(msg))) == strlen(msg))
    {
        printf("\n(%s:%d)\033[0;34m write success! %d\033[m\n",__func__,__LINE__, ret);  
    }

    printf("Press  to release lock: \n");
    getchar();

    fl.l_type = F_UNLCK;  /* set to unlock same region */

    if (fcntl(fd, F_SETLK, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }

    printf("Unlocked.\n");

    close(fd);
}
