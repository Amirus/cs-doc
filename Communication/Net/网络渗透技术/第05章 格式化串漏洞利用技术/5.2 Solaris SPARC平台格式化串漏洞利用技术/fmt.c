/* fmt.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ���ڸ�ʽ����©������ʾ����
*  ��ʾƽ̨��Solaris SPARC 7
*/

int main (int argc, char **argv)
{
    char buf[512];

    strncpy(buf, argv[1], sizeof(buf)-1);
    printf(buf);
    return 0;
}
