#include <stdlib.h>
#include <string.h>
int main(int argc, char ** argv)
{
        char buf[16];
	char buf1[32];//��Ҫ�����������Ȼָ��add    xxx,%esp��������mmap�����Ĳ����ܹ���С��
		      //����˵��������� char pad3[8 + POPNUM - sizeof(struct mmap_args)];
		      //�ͻ����Ҳ���ǻ����8 + POPNUM - sizeof(struct mmap_args) <0�����
		
        if (argc==2)
                strcpy(buf,argv[1]);
}
