#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/termios.h>
#include <string.h>
#include <unistd.h>

#define NODEPATH_LEN    (256)
typedef struct _Serial_Config_
{
    char nodepath[NODEPATH_LEN];
    unsigned long baudrate;
    int databits;
    char parity;
    int stopbits;
    int fd;
}Serial_Config;

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

/*
 *
 * brief  : capture stdin into a string buffer orderly.
 * return : return a string buffer, and free it after it is finished.
 *
 */
char *getln()
{
    char *line = NULL, *tmp = NULL;
    size_t size = 0, index = 0;
    int ch = EOF;

    while (ch) {
        ch = getc(stdin);

        /* Check if we need to stop. */
        if (ch == EOF || ch == '\n')
            ch = 0;

        /* Check if we need to expand. */
        if (size <= index) {
            size += 1;
            tmp = realloc(line, size);
            if (!tmp) {
                free(line);
                line = NULL;
                break;
            }
            line = tmp;
        }

        /* Actually store the thing. */
        line[index++] = ch;
    }

    return line;
}

//*********************************************************************************************
// Select serial format: 7o1, 7e1, 8n1, 8e1.... for communicate a device.(ex. /dev/ttyAMA1)
//*********************************************************************************************
int initSerial_port(int *fd, unsigned long baudrate, int databits, char parity, int stopbits)
{
    struct termios com;
    
    if(*fd < 0) {
        printf("invalid *fd : %d\n", *fd);
        return -1;
    }
    
    memset(&com, 0, sizeof(struct termios));
    
    com.c_cflag |= (CLOCAL|CREAD);
    com.c_cflag &= ~CSIZE;
    //select Data bits[7/8]
    switch(databits)
    {   
        case 7:
            com.c_cflag |= CS7;
            break;
        case 8:
            com.c_cflag |= CS8;
            break;
        default:
            printf("\n(%s:%d) databits error(%d). use default 8 bits\n",__func__,__LINE__,databits);
            com.c_cflag |= CS8;
            break;
    }
    //select [none/odd/even] parity
    switch(parity)
    {
        //odd parity
        case 'o':
        case 'O':
            com.c_cflag |= (PARENB | PARODD);
            /** If this bit is set, input parity checking is enabled. **/
            //com.c_cflag |= INPCK;
            break;

        //even parity
        case 'e':
        case 'E':
            com.c_cflag |= PARENB;
            com.c_cflag &= ~PARODD;
            break;

        //none parity
        case 'n':
        case 'N':
        default:
            com.c_cflag &= ~(PARENB | PARODD);
            break;
    }       

    //select stopbits
    switch(stopbits)
    {
        case 2:
            com.c_cflag |= CSTOPB;
            break;
        case 1:
        default:
            com.c_cflag &= ~CSTOPB;
            break;
    }       
    //select baudrate
    switch(baudrate){
        case 1200:
            com.c_cflag |= B1200;
            break;
        case 2400:
            com.c_cflag |= B2400;
            break;
        case 4800:
            com.c_cflag |= B4800;
            break;
        case 9600:
            com.c_cflag |= B9600;
            break;
        case 19200:
            com.c_cflag |= B19200;
            break;
        case 38400:
            com.c_cflag |= B38400;
            break;
        case 57600:
            com.c_cflag |= B57600;
            break;
        case 115200:
            com.c_cflag |= B115200;
            break;
        case 230400:
            com.c_cflag |= B230400;
            break;
        default:
            printf("\n(%s:%d) baud rate error.(%lu) use default 9600\n",__func__,__LINE__, baudrate);
            com.c_cflag |= B9600;
            break;
    }       
    
    tcsetattr(*fd, TCSANOW, &com);
    
    return 0;
}   

void Usage()
{
    printf(" Usage: ./%s [options] \n", __FILE__);
    printf(" -n, node path:\n \
        Specify the node path to be opened.(Required)\n");
    printf(" -b, baudrate:\n \
        Specify the baud rate to set the serial-port.(Default: 115200)\n");
    printf(" -d, databits:\n\
        Defines the number of data bits in every character. Must be one of(Default: 8 ):\n \
        5, 6, 7, 8 \n");
    printf(" -p, parity:\n \
        Defines the flow-control mode to set the serial-port to. Must be one of(Default: n ):\n \
        ´o' for odd parity mode.\n \
        ´e' for even parity mode.\n \
        ´n' for no parity, mode. \n");
    printf(" -s, stop bits:\n \
        Defines the stop-bits to set the serial-port to. Must be one of(Default: 1 ):\n \
        1, 2  \n");
}


void replaceSubstring(char *s,const char *toremove, char c)
{
  while( s=strstr(s,toremove) )
  {
    s[strlen(toremove)] = c;
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
  }
}

void removeSubstring(char *s,const char *toremove, char c)
{
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

void *read_thrd(void *arg)
{
    int *fd = (int *)arg;

    fd_set rdSet;
    int select_ret = -1;
    int read_ret = 0;
    char buf[1024] = {0};

    printf("%s starts...\n", __func__);
    while(1)
    {
        struct timeval time_out = {0, 1000};
        FD_ZERO(&rdSet);
        FD_SET( *fd, &rdSet);
        select_ret = select(*fd + 1, &rdSet, NULL, NULL, &time_out);

        if(select_ret > 0)
        {
            if ((*fd > 0) && (FD_ISSET(*fd, &rdSet)))
            {
                memset( buf, 0, 1024);
                read_ret = read(*fd, buf, sizeof(buf));
                if (read_ret < 0) {                                                                                                                    
                    printf("read error, read_ret=%d, %s(%d)\n", read_ret, strerror(errno), errno);
                    continue;
                }
                else if(read_ret == 0)
                {
                    continue;
                }
                printf("%s",buf);
            }
        }

    }

    pthread_exit(NULL);

}

int main( int argc, char *argv[])
{
    int devicefd = 0;
    int opt = 0;
    int ret = -1;

    Serial_Config config;
    memset( &config, 0, sizeof(Serial_Config));
    //define default value.
    config.baudrate = 115200;
    config.databits = 8;
    config.parity   = 'n';
    config.stopbits = 1;
    

    if( argc < 2)
    {
        Usage();
        return 0;
    }

    //Check options.
    /*
     * h : help.
     * b : baudrate.
     * d : databits
     * p : parity.
     * s : stop-bits.
     * n : node path.
     */
    while ((opt = getopt(argc, argv, "b:p:d:s:h:n:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                {
                    Usage();
                }
                return 0;
            case 'b':
                {
                    //printf ("baudrate: \"%s\"\n", optarg);
                    ret = sscanf( optarg, "%ld", &config.baudrate);
                    if(ret != 1)
                        config.baudrate = 9600;
                }
                break;
            case 'd':
                {
                    //printf ("data-bits: \"%s\"\n", optarg);
                    ret = sscanf( optarg, "%d", &config.databits);
                    if(ret != 1)
                        config.databits = 8;
                }
                break;
            case 'p':
                {
                    //printf ("parity: \"%s\"\n", optarg);
                    ret = sscanf( optarg, "%c", &config.parity);
                    if(ret != 1)
                        config.parity = 'n';
                }
                break;
            case 's':
                {
                    //printf ("stop-bits: \"%s\"\n", optarg);
                    ret = sscanf( optarg, "%d", &config.stopbits);
                    if(ret != 1)
                        config.stopbits = 1;
                }
                break;
            case 'n':
                {
                    //printf ("node: \"%s\", len:%d \n", optarg, strlen( optarg));
                    if( strlen(optarg) > sizeof(config.nodepath))
                    {
                        printf(" nodepath is out of range...%d \n", sizeof(config.nodepath));
                        return -1;
                    }

                    if( strlen(optarg) <= 0)
                    {
                        printf(" nodepath is required. \n");
                        return -1;
                    }

                    memset( config.nodepath, 0, sizeof(config.nodepath));
                    strncpy( config.nodepath, optarg, strlen(optarg));
                }
                break;
            default:
                {
                    Usage();
                }
                return 0;
        }
    }

    printf("\033[0;33m ==================== \033[m\n"); 
    printf("\033[0;33m Path: [%s] \033[m\n", config.nodepath); 
    printf("\033[0;33m Baudrate: [%lu] \033[m\n", config.baudrate); 
    printf("\033[0;33m Databits: [%d] \033[m\n", config.databits); 
    printf("\033[0;33m Parity: [%c] \033[m\n", config.parity); 
    printf("\033[0;33m Stopbits: [%d] \033[m\n", config.stopbits); 
    printf("\033[0;33m ==================== \033[m\n"); 

    //Open Device Node.
    config.fd = open( config.nodepath, O_RDWR);
    if( config.fd < 0)
    {
        printf("opening dev:[%s] is fail...%d, %s\n", config.nodepath, errno, strerror(errno));
        return -1;
    }

    //Init Device config.
    ret = initSerial_port( &config.fd, config.baudrate, config.databits, config.parity, config.stopbits);
    if(ret < 0)
    {
        printf("init serial:[%d] is fail...%d, %s\n", ret, errno, strerror(errno));
        return -1;
    }

    pthread_t thread_id;
    if(pthread_create(&thread_id, NULL, read_thrd, (void*)&config.fd) != 0)
    {
        printf("create_norm_thread() thread create error: %s\n", strerror(errno));
        return -1;
    }
    pthread_detach(thread_id);


    char *tmpBuf = NULL;
    char *delim="0x";
    long ch;
    char str[6];
    char *curstr = NULL;
    char *chkstr = NULL;

    //Start talk.
    while(1)
    {
        printf("#");
        tmpBuf = getln();
        curstr = strstr( tmpBuf, delim);
        int offset = 0;
        while( curstr != NULL)
        {
            memset( &str, 0, sizeof(char)*6);
            ret = sscanf( curstr,"%6[^h]h", str);
            if(ret)
            {
                chkstr = strstr(curstr, str);
                if(!chkstr)
                {
                    offset = 1;
                }
                else if(chkstr[strlen(str)] != 'h')
                {
                    offset = 1;
                }
                else
                {
                    ch = strtol(str, NULL, 16);
                    if( ch == 0)
                        offset = 1;
                    else
                    {
                        replaceSubstring( tmpBuf, str, (char) ch);
                        offset = 0;
                    }
                }
            }
            else
            {
                printf(" symbol[%s] not found, ignore \n", delim);
                offset = 1;
            }

            //go next delim.
            curstr = strstr( curstr+offset, delim);
        }

        if(strlen(tmpBuf)>0)
        {
            //write to node path.
            ret = write( config.fd, tmpBuf, strlen(tmpBuf));
            if(ret <= 0)
            {
                printf("write fd:%d, fail:%d, %d, %s\n", config.fd, ret, errno, strerror(errno));
            }

            ret = write( config.fd, "\n", 1);
            if(ret <= 0)
            {
                printf("write fd:%d, fail:%d, %d, %s\n", config.fd, ret, errno, strerror(errno));
            }
        }

        free(tmpBuf);
        tmpBuf = NULL;
    }

    close(config.fd);

    return 0;
}
