#include <stdio.h>
#include <syslog.h>

int main()
{
    while(1)
    {
        syslog(LOG_INFO,"hello world my friend",30);
        sleep(300);
        syslog(LOG_INFO,"hello world liuaiping",30);
    }/*每隔3秒向文件/var/log/messages中写入这2句话*/
    return 0;
}