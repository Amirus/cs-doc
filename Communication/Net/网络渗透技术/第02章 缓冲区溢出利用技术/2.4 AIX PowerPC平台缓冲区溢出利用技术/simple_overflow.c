/* simple_overflow.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  Simple program to demonstrate buffer overflows
*  on the PowerPC architecture.
*/
#include <stdio.h>
#include <string.h>
char largebuff[] =
"123451234512345123451234=PRESERVEDSPACE=ABCD";
int main (void)
{
    char smallbuff[16];
    strcpy (smallbuff, largebuff);
}
