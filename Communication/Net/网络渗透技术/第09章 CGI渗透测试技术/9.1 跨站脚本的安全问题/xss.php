<?php
/* xss.php
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ���ڿ�վ�ű�©������ʾ����
*/

setcookie ("TestCookie", "xss");

$input = stripslashes($input);
echo "test string: ".$input;
?>
