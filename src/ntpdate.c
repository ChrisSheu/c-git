/*
 * This code will query a ntp server for the local time and display
 * it.  it is intended to show how to use a NTP server as a time
 * source for a simple network connected device.
 * This is the C version.  The orignal was in Perl
 *
 * For better clock management see the offical NTP info at:
 * http://www.eecis.udel.edu/~ntp/
 *
 * written by Tim Hogard (thogard@abnormal.com)
 * Thu Sep 26 13:35:41 EAST 2002
 * Converted to C Fri Feb 21 21:42:49 EAST 2003
 * this code is in the public domain.
 * it can be found here http://www.abnormal.com/~thogard/ntp/
 *
 * written by Chris Sheu (cs1987717@gmail.com)
 * Wed Apr 2 2:34:35 Taiwan 2015
 * Add a argument to get time from NTP server.
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//this function check [ip_str] is ipv4 format.
//return 0, yes; otherwise, return -1;
static int is_valid_ipv4(char *ip_str)
{
    unsigned ip_s1, ip_s2, ip_s3, ip_s4;

    //sscanf return 4, success; otherwise, fail.
    if(sscanf( ip_str, "%u.%u.%u.%u", &ip_s1, &ip_s2, &ip_s3, &ip_s4) != 4)
        return -1;

    //check ip range 0-255.
    if((ip_s1 | ip_s2 | ip_s3 | ip_s4) > 255)
        return -2;


    return 0;
}

/* get host by name, it only catch first ip, because a symbol [break] in for loop.
 *
 * hostname: input.
 * addr    : output.
 * wformat : only catch specified format ip str. AF_INET, AF_INET6, AF_UNSPEC.
 */
int net_getHostByName2(char *hostname, char *dst_ipstr, int wformat)
{
    struct addrinfo hints, *res, *p;
    int status;
    int IPV4_MAX_LEN = 15;
    int IPV6_MAX_LEN = 45;

    char ipstr[48];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6.
    hints.ai_socktype = SOCK_STREAM;

    dst_ipstr[0] = '\0';

    //get host info.
    if((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
    {
        printf("getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    void *addr;
    char *ipver;

    //Search all nodes.
    for(p = res;p != NULL; p = p->ai_next)
    {
        if(p->ai_family == AF_INET)
        {
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
            // convert the IP to a string and print it:
            inet_ntop(p->ai_family, addr, ipstr, IPV4_MAX_LEN);
        }
        else
        {
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
            // convert the IP to a string and print it:
            inet_ntop(p->ai_family, addr, ipstr, IPV6_MAX_LEN);
        }


        //only catch first specified format ip str, then break;
        if((wformat == AF_INET) && (p->ai_family == AF_INET)) //Only IPv4
        {
            //copy to dst.
            strncpy( dst_ipstr, ipstr, IPV4_MAX_LEN);
            break;
        }
        else if((wformat == AF_INET6) && (p->ai_family == AF_INET6)) //Only IPv6
        {
            //copy to dst.
            strncpy( dst_ipstr, ipstr, IPV6_MAX_LEN);
            break;
        }
        else if(wformat == AF_UNSPEC) //Both IPv4 or IPv6
        {
            //copy to dst.
            strncpy( dst_ipstr, ipstr, strlen(ipstr));
            printf("\n%s: unknown ai_family...",__func__, p->ai_family);
            break;
        }
    }

    //put ip string into sockaddr_in.sin_addr.s_addr, if you want.
    //inet_aton(ipstr, &sockaddr_in.sin_addr.s_addr);

    freeaddrinfo(res); //free resource.

    if(strlen(dst_ipstr) == 0)
        return -2;

    return 0;
}

int ntpdate(char *host)
{
    char hostname[50];
    int	portno=123;		//NTP is port 123
    int	maxlen=2048;		//check our buffers
    int	i;			// misc var i
    unsigned char msg[48]={010,0,0,0,0,0,0,0,0};	// the packet we send
    unsigned long  buf[maxlen];	// the buffer we get back
    //struct in_addr ipaddr;		//
    struct protoent *proto;		//
    struct sockaddr_in server_addr;
    int	s;	// socket
    int	tmit;	// the time -- This is a time_t sort of
    int ret, ret2;
    char ip_addr[48];

    if((host != NULL) && (strlen(host) > 0) )
    {
        memcpy( hostname, host, strlen(host));
    }
    else
        return -1;


    //use Socket;
    //
    //#we use the system call to open a UDP socket
    //socket(SOCKET, PF_INET, SOCK_DGRAM, getprotobyname("udp")) or die "socket: $!";
    proto=getprotobyname("udp");
    s=socket(PF_INET, SOCK_DGRAM, proto->p_proto);
    if(s) {
        //perror("asd");
        printf("socket=%d\n",s);
    }

    memset( &server_addr, 0, sizeof( server_addr ));
    server_addr.sin_family=AF_INET;
    //check ipv4 format, [0-255].[0-255].[0-255].[0-255] or xxx.xxx.xx...
    ret = is_valid_ipv4(hostname);
    if(ret == 0)
        server_addr.sin_addr.s_addr = inet_addr(hostname);
    else if(ret < 0)
    {
        //check hostname and get ip address.
        //success, return 0; otherwise , < 0.
        ret2 = net_getHostByName2(hostname, ip_addr, AF_INET);
        if(ret2 < 0)
        {
            printf("Hostname error...%d", ret2);
            return -2;
        }

    }

    inet_aton(ip_addr, &server_addr.sin_addr.s_addr);
    server_addr.sin_port=htons(portno);
    //printf("ipaddr (in hex): %x\n",server_addr.sin_addr);

    /*
     * build a message.  Our message is all zeros except for a one in the
     * protocol version field
     * msg[] in binary is 00 001 000 00000000
     * it should be a total of 48 bytes long
     */

    // send the data
    i=sendto(s,msg,sizeof(msg),0,(struct sockaddr *)&server_addr,sizeof(server_addr));

    // get the data back
    i=recv(s,buf,sizeof(buf),0);
    //printf("recvfr: %d\n",i);
    //perror("recvfr:");

    //We get 12 long words back in Network order
    /*
       for(i=0;i<12;i++)
       printf("%d\t%-8x\n",i,ntohl(buf[i]));
     */

    /*
     * The high word of transmit time is the 10th word we get back
     * tmit is the time in seconds not accounting for network delays which
     * should be way less than a second if this is a local NTP server
     */

    tmit=ntohl((time_t)buf[10]);	//# get transmit time
    //printf("tmit=%d\n",tmit);

    /*
     * Convert time to unix standard time NTP is number of seconds since 0000
     * UT on 1 January 1900 unix time is seconds since 0000 UT on 1 January
     * 1970 There has been a trend to add a 2 leap seconds every 3 years.
     * Leap seconds are only an issue the last second of the month in June and
     * December if you don't try to set the clock then it can be ignored but
     * this is importaint to people who coordinate times with GPS clock sources.
     */
    //1970.1.1 00:00:00;
    tmit-= 2208988800U;
    //printf("tmit=%d\n",tmit);
    /* use unix library function to show me the local time (it takes care
     * of timezone issues for both north and south of the equator and places
     * that do Summer time/ Daylight savings time.
     */


    //#compare to system time
    printf("Time: %s",ctime(&tmit));
    i=time(0);
    //printf("%d-%d=%d\n",i,tmit,i-tmit);
    printf("System time is %d seconds off\n",i-tmit);

    return 0;
}

void help()
{
    printf("It's a get ntp time tool from ntp-server.\n");
    printf("\n");
    printf("-n,    a name or a ipaddr.\n");
    printf("\n");
    printf("IP   example: #./ntp -n ip1.ip2.ip3.ip4\n");
    printf("Name example: #./ntp -n ntp0.nl.net\n");
}

int main(int argc, char *argv[])
{
    int c;
    printf("argc: %d\n", argc);
    while((c = getopt ( argc, argv, "h::n:")) != -1)
    {
        switch(c)
        {
            case 'h':
                help();
                break;
            case 'n':
                ntpdate(optarg);
                break;
            default:
                help();
                break;
        }
    }

    if(argc < 2)
    {
        help();
        exit(0);
    }

    return 0;
}
