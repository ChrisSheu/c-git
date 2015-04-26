#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "mymmap.h"

static int mmap_config( size_t len, int prot, int flag, int fd, off_t offset)
{
    int ret = -1;
    int *m;

    m = (int *)mmap(0, len, prot, flag, fd, offset);
    if( m == MAP_FAILED)
    {
        printf("Error mmapping the file \n");
        return -1;
    }

    return m;
}


int main(int argc, char *argv[])
{
    int i;
    int fd;
    int *map;  /* mmapped array of int's */

    fd = open(FILEPATH, O_RDONLY);
    if (fd == -1) {
        printf("Error opening file for reading \n");
        exit(EXIT_FAILURE);
    }

    map = mmap_config( FILESIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (map < 0) {
        close(fd);
        printf("mmap_config fail...%d\n", map);
        exit(EXIT_FAILURE);
    }

    /* Read the file int-by-int from the mmap
     */
    for (i = 0; i <NUMINTS; ++i) {
        printf("%d: %d\n", i, map[i]);
    }

    if (munmap(map, FILESIZE) == -1) {
        printf("Error un-mmapping the file \n");
    }
    close(fd);
    return 0;
}
