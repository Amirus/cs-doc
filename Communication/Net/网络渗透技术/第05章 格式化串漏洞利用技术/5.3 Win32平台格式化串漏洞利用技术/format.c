/* format.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ���ڸ�ʽ����©������ʾ����
*/

#include <stdio.h>
#define IOSIZE 1024

int main(int argc, char **argv )
{
    FILE * binFileH;
    char binFile[] = "binfile";
    char buf[IOSIZE];

    if ( (binFileH = fopen(binFile, "rb")) == NULL )
    {
        printf("can't open file %s!\n", binFile);
        exit();
    }

    memset(buf, 0, sizeof(buf));
    fread(buf, sizeof(char), IOSIZE, binFileH);

    printf("%d\n", strlen(buf));
    printf(buf);

    fclose(binFileH);
}
