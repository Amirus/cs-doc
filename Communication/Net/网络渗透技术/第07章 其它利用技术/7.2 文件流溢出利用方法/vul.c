/* vul.c
* 
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  FSO©��ʵ��
*/

#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE * fp;
	char buf[1024];
	int i;
	fp =stdout;
	i = (int)&fp-(int)&buf;
	printf(" fp addr %p point %p\nbuf addr %p\n len %d\n",&fp,fp,buf,i);
	
	strncpy(buf,argv[1],i +4 );

	fprintf(fp,"%s\n",buf);
	exit(0);
}
