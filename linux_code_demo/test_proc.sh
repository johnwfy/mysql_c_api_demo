#!/bin/bash
# test_proc
#this script is used for start or stop
#
#the test_proc Daemon
#chkconfig:345 83 14      /*����ز����٣���˼������Ĭ�Ͽ�������Ϊ345�������84��ʵ����������˳��*/
#description:test_proc Daemon   /*�������Ҳ������*/
#Author: JOHN
#Writem: 2017.3.12
#
#

start() {
 echo "###################start the  test_proc##############################"
 /usr/sbin/test_proc &       
 #/*test_proc��һ�����Լ���.c�ļ��õ��Ŀ�ִ���ļ�����ʵ����ִ��һ�γ��򣬺���Ὣ��εõ�,����&��ʾ��̨����*/
 exit 0; 
}    
#/*����start���ֿɲ�������ϵͳ/etc/init.d/������ļ���ʽ*/


#/*��ʽҲ�ǲ���ϵͳ�е�*/

case "$1" in
  start)
    start
        ;;
  stop)
    stop
        ;;
esac
exit 0