/* simple_overflow.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  Simple program to demonstrate buffer overflows
*  on the SPARC architecture.
*/

#include <stdio.h>
#include <string.h>

char largebuff[] =
"1234512345123451234512345123451234512345"
"1234512345123451234512345123451234512345"
"123451234512"
"ABCD";

int overflow(char * str) {
    char smallbuff[16];
    strcpy (smallbuff, str);
}

int main (void)
{
    overflow(largebuff);
}
