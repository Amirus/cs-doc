/* shellcode_asm.c
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  shellcode��ʾ
*/

int main ()
{
    __asm__
    ("
        sethi   %hi(0x2f626800), %l6
        or      %l6, 0x16e, %l6         ! 0x2f62696e
        sethi   %hi(0x2f736800), %l7
        std     %l6, [ %sp + -16 ]      ! std��ָ����double word��Ҳ���ǲ��� %l6 + %l7 �Ĵ�����
        add     %sp, -16, %o0           ! $o0�ǵ�һ����������/bin/sh�ִ��ĵ�ַ
        st      %o0, [ %sp + -8 ]       ! name�������ָ��
        clr     [ %sp + -4 ]            ! ������name�������
        add     %sp, -8, %o1            ! $o1�ǵڶ�����������name�������ָ��
        xor     %o2, %o2, %o2           ! $o2�ǵ���������0
        mov     59, %g1                 ! execve��ϵͳ���ú���59
        ta      8
    ");
}
