/* server.c -  overflow demo
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ���ڻ���������ķ���˳���
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

char Buff[1024];
void overflow(char * s,int size)
{
    char s1[50];
    printf("receive %d bytes",size);
    s[size]=0;
    //strcpy(s1,s);
    memcpy(s1, s, size);
    sync(); // ����������һЩ��������ϵͳ���ã�����ֱ�ӷ��غ��ֻ�����I-cache�����⣬shellcode���޷�ִ�е�
}

int main(int argc, char *argv[]) 
{ 
    int s, c, ret, lBytesRead; 
    struct sockaddr_in srv; 

    s = socket(AF_INET, SOCK_STREAM, 0); 
    srv.sin_addr.s_addr = INADDR_ANY; 
    srv.sin_port = htons(4444); 
    srv.sin_family = AF_INET; 

    bind(s, &srv, sizeof(srv)); 
    listen(s, 3);

    c = accept(s,NULL,NULL);

    while(1)
    { 
        lBytesRead = recv(c, Buff, 1024, 0);
        if(lBytesRead<=0)    break;

        printf("fd = %x recv %d bytes\n", c, lBytesRead);
        overflow(Buff, lBytesRead);  

        ret=send(c,Buff,lBytesRead,0);
        if(ret<=0)    break;
    }

    close(s);
    close(c);
}
