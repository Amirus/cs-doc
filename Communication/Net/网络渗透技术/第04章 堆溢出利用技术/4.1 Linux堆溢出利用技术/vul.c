/* vul.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ���ڶ����©������ʾ����
*/

#include <stdio.h>

int main(int argc, char *argv[]) {
    char *p0 = (char *) malloc(16);
    char *p1 = (char *) malloc(16);

    if (argc > 1)
        strcpy(p0, argv[1]);

    printf("Before free p0.\n");
    free(p0);
    printf("Before free p1.\n");
    free(p1);
}
