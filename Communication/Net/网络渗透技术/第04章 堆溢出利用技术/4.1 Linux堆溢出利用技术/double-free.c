/* double-free.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ���������ͷ�©������ʾ����
*/

#include <stdio.h>

int main(int argc, char *argv[])
{
    char *p0 = (char *) malloc(8);
    char *p1 = (char *) malloc(8);
    char *p2, *p3;

    free(p0);
    free(p0);

    p2 = (char *) malloc(8);
    if (argc > 1)
        strcpy(p2, argv[1]);
    p3 = (char *) malloc(8);
}
