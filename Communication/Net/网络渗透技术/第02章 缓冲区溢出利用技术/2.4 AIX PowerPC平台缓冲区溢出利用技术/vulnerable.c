/* vulnerable.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  Vulnerable program on the PowerPC architecture.
*/

#include <stdio.h>
#include <string.h>
int main (int argc, char *argv[])
{
    char vulnbuff[16];
    strcpy (vulnbuff, argv[1]);
    printf ("\n%s\n", vulnbuff);
    getchar(); /* for debug */
}
