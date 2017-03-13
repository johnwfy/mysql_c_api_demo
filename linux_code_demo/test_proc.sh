#!/bin/bash
# test_proc
#this script is used for start or stop
#
#the test_proc Daemon
#chkconfig:345 83 14      /*这个必不可少，意思是设置默认开启级别为345，后面的84其实是启动运行顺序*/
#description:test_proc Daemon   /*这个描述也不可少*/
#Author: JOHN
#Writem: 2017.3.12
#
#

start() {
 echo "###################start the  test_proc##############################"
 /usr/sbin/test_proc &       
 #/*test_proc是一个我自己由.c文件得到的可执行文件，其实就是执行一段程序，后面会将如何得到,其中&表示后台运行*/
 exit 0; 
}    
#/*上面start部分可参照我们系统/etc/init.d/下面的文件格式*/


#/*格式也是参照系统中的*/

case "$1" in
  start)
    start
        ;;
  stop)
    stop
        ;;
esac
exit 0