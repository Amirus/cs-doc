<?php
/* test.php
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  MySQLע����ʾ����
*/

$link = mysql_connect("localhost", "root") or die("Could not connect: " . mysql_error() . "\n");
mysql_select_db("test") or die("Could not select database.\n");

$query = "SELECT * FROM member WHERE id={$_REQUEST['id']}";
$result = mysql_query($query) or die("Query error: " . mysql_error() . "\nQuery string: {$query}\n");

while($row = mysql_fetch_assoc($result)) {
    echo $row['id']." ".$row['name']."\n";
}

mysql_free_result($result);
mysql_close($link);
?>
