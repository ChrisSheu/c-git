#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * dup            : return new descriptor.
 * dup2(des, src) : src refer to des pointer.
 */
 
int main(int argc, char *argv[])
{
    int fd, save_fd;
    char msg[] = "HelloWorld\n";

    fd = open("somefile", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if(fd<0) {
        perror("open");
        exit(1);
    }
/*
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

