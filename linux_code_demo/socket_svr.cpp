#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <iostream>

#include "mysqlHelper.h"
using namespace mysqlhelper;
using namespace std;

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024


int main()
{
	//初始化mysql对象并建立连接
	MysqlHelper mysqlHelper;
	mysqlHelper.init("45.78.44.136", "root", "Xx753951xx", "mysql_ip_record");
	try {
		mysqlHelper.connect();
	}
	catch (MysqlHelper_Exception& excep) {
		cout << excep.errorInfo;
		return -1;
	}

	///定义sockfd
	int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	///定义sockaddr_in
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(MYPORT);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	///设置套接字选项避免地址使用错误
	int on = 1;
	if ((setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))<0)
	{
		perror("setsockopt failed");
		exit(1);
	}

	///bind，成功返回0，出错返回-1
	if (bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	///listen，成功返回0，出错返回-1
	if (listen(server_sockfd, QUEUE) == -1)
	{
		perror("listen");
		exit(1);
	}

	///客户端套接字
	char buffer[BUFFER_SIZE];
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);

	///成功返回非负描述字，出错返回-1
	int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
	if (conn<0)
	{
		perror("connect");
		exit(1);
	}

	char ipstr[128];
	time_t tNow = time(NULL);
	printf("client ip %s\tport %d\tnow is %s\n",
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipstr, sizeof(ipstr)),
		ntohs(client_addr.sin_port), ctime(&tNow));
	char timestr[128];
	sprintf(timestr, "%ld", (long)tNow);

	MysqlHelper::RECORD_DATA record;
	record.insert(make_pair("timeStamp", make_pair(MysqlHelper::DB_INT, timestr)));
	record.insert(make_pair("ipAddr", make_pair(MysqlHelper::DB_STR, ipstr)));
	record.insert(make_pair("strTime", make_pair(MysqlHelper::DB_STR, ctime(&tNow))));
	int res = 0;
	try {
		res = mysqlHelper.insertRecord("ip_time_table", record);
	}
	catch (MysqlHelper_Exception& excep) {
		cout << excep.errorInfo;
		return -1;
	}
	cout << "res:" << res << " insert successfully " << endl;

	while (1)
	{
		memset(buffer, 0, sizeof(buffer));
		int len = recv(conn, buffer, sizeof(buffer), 0);
		if (strcmp(buffer, "exit\n") == 0)
			break;
		fputs(buffer, stdout);
		send(conn, buffer, len, 0);
	}
	close(conn);
	close(server_sockfd);
	return 0;
}