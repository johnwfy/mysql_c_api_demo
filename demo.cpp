/****************************************************
*@brief:mysqlhelper demo
*@autor:lvlv
*@date:2016.06.12
*@MySQL version:MySQL Community Server 5.6.30 (GPL)
****************************************************/

#include <string.h>
#include <iostream>
#include <string>
using namespace std;

#include "mysqlHelper.h"
using namespace mysqlhelper;


int main(int argc, char* argv[]) {

	//初始化mysql对象并建立连接
	MysqlHelper mysqlHelper;
	mysqlHelper.init("45.78.44.136", "root", "Xx753951xx", "test");
	try {
		mysqlHelper.connect();
	}
	catch (MysqlHelper_Exception& excep) {
		cout << excep.errorInfo;
		return -1;
	}

	cout << "mysql connect success!" << endl;

	return 0;
}