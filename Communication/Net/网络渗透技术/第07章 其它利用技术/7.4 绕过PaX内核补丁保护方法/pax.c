#include <stdlib.h>
#include <string.h>
char spare[1024+200];//����200��bytes,��Ȼ����ʵ��Ļ�����FRAMESINDATAΪ0x8049a00(����\0)
char bigbuf[1024];

int
main(int argc, char ** argv)
{
	char buf[16];
	char * ptr=getenv("STR");
	if (ptr) {
		bigbuf[0]=0;
		strncat(bigbuf, ptr, sizeof(bigbuf)-1);
	}
	ptr=getenv("LNG");
	if (ptr)
		 strcpy(buf, ptr);	
}
