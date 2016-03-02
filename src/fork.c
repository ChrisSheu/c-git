#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

static int *inputnum;

void forkChildren (int nChildren)
{
    pid_t pid;
        pid = fork();
        if (pid == -1) {
            /* error handling here, if needed */
            return;
        }
        if (pid == 0) {
            printf("I am a child: PID: %d\n", getpid());
            while(1)
            {
              if( *(inputnum+1) == 999)
                break;
              else
              {
                printf("[Sub] your inputnum:%d \n", *(inputnum+1));
                printf("=========[Sub]==size:%d ========\n", sizeof *inputnum);
              }

              sleep(1);
            }
            exit( EXIT_SUCCESS);
        }
}

int main (int argc, char *argv[])
{
    inputnum = mmap(NULL, sizeof(int)*2 , PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if( inputnum == MAP_FAILED)
    {
        printf(" Get nmap error...return! \n");
        return -1;
    }

    *(inputnum+1) = -1;

    if (argc < 2) {
        forkChildren (2);
    } else {
        forkChildren (atoi (argv[1]));
    }
    while(1)
    {
      printf("input number:\n");
      scanf("%d", inputnum+1);
      if( *(inputnum+1) == 999)
          break;
      else
          printf(" [Main] your inputnum:%d \n", *(inputnum+1));
    }

    printf("===================\n");
    return 0;
}
