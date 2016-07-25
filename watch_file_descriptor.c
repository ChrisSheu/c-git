#include <stdio.h>
#include <stdlib.h>

void Usage()
{
    printf(" ./%s <pid> <timer> \n",__FILE__);  
    printf(" <pid>  : process id. \n",__FILE__);  
    printf(" <timer>: specifies delay time for updating \n",__FILE__);  
}

int main(int argc, char* argv[])
{
    char cmd[256] = {0};
    int i = 0;
    int timer = 1;      //default 1 second.

    if(argc < 3){
        Usage();
        return -1;
    }

    sprintf( cmd, "ls /proc/%s/fd -l", argv[1]);
    timer = atoi(argv[2]);

    while(1){

        printf("==times:[%d]== \n", i++);
        system(cmd);
        printf("===========================\n");

        sleep( timer);
    }

    return 0;

}

