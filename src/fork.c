#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

static int *inputnum;
static int *testnum;

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
              if( *inputnum == 999)
              {
                *testnum = 100;
                break;
              }
              else
              {
                printf("[Sub] your inputnum:%d \n", *inputnum);
              }

              sleep(1);
            }
            exit(1);
        }
}

int main (int argc, char *argv[])
{
    inputnum = mmap(NULL, sizeof(int) , PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    testnum = mmap(NULL, sizeof(int) , PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if( inputnum == MAP_FAILED)
    {
        printf(" Get nmap error...return! \n");
        return -1;
    }

    if( testnum == MAP_FAILED)
    {
        printf(" Get nmap error...return! \n");
        return -1;
    }

    *inputnum = -1;
    *testnum = -1;

    forkChildren (2);
    while(1)
    {
      printf("input number:\n");
      scanf("%d", inputnum);
      if( *inputnum == 997)
      {
          printf(" [Main] get out your inputnum:%d, testnum:%d \n", *inputnum, *testnum);
          break;
      }
      else
          printf(" [Main] your inputnum:%d, testnum:%d \n", *inputnum, *testnum);
    }

    printf("===================\n");
    return 0;
}
