#include <stdio.h>
#include <syslog.h>

int main()
{
    while(1)
    {
        syslog(LOG_INFO,"hello world my friend",30);
        sleep(300);
        syslog(LOG_INFO,"hello world liuaiping",30);
    }/*ÿ��3�����ļ�/var/log/messages��д����2�仰*/
    return 0;
}