/* t.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ��ʼ�������������ʾ����
*/

#include<stdio.h>
int x = 5;
void main(void)
{
        char *p="Hello World";  /* pָ�����ѳ�ʼ�������� */
        int i=0;
        char buff[1024];
        for(i=0;i<1024;buff[i++]='A');
        buff[1023]=0;
        strcpy(p,buff); /*�������������PLT�д��strcpy������ַ���ڴ浥Ԫ */
        strcpy(p,buff); /* ��ε��ý���ת��0x41414140�� (ָ���ַ��4�ֽڶ��룬*/
                        /* ���û�ж�����Զ�������������) */
}
