/* function [mmap]
 * 1.MAP_SHARED     : shared memory. (sync file.)
 * 2.MAP_PRIVATE    : private memory.(async file.)
 * 3.MAP_ANONYMOUS  : init memory.( all zero.)
 * 4.MAP_FIXED      : first argument(required), and size must be 4096 bytes.
 * 5.MAP_HUGETLB    : get [Hugepagesize: 2048 kB] memory, cat /proc/memoryinfo.
 */
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
    int i = 0;
    int fd = -1;
    int ret = -1;
    int *map;  /* mmapped array of int's */

    //init file.
    fd = open(FILEPATH, O_RDWR | O_CREAT, 0755); // O_RDWR & 755
    if (fd == -1) {
        printf("Error opening file for reading \n");
        exit(EXIT_FAILURE);
    }

    //init file content.(required)
    if((ret = write(fd, "", 1)) != 1)
    {
        printf("init file content fail...%d \n", ret);
        return -1;
    }

    //mmap config.
    //it's get error when executing virtualbox-environment.
    map = mmap_config( FILESIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (map < 0)
    {
        close(fd);
	    printf("mmap_config fail...%d\n", map);
        exit(EXIT_FAILURE);
    }

    /*
     * 1.write the file int-by-int from the mmap
     * 2.sync file descriptor.(when writing)
     */
    for (i = 0; i <NUMINTS; ++i) {
        map[i] = i + 1;
    }

//TODO lock here, and execute PROT_READ, it can read data successfully.
//getchar();

    //free map.
    if (munmap(map, FILESIZE) == -1) {
        printf("Error un-mmapping the file \n");
    }

    close(fd);
    return 0;
}
