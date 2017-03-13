/*
* @Author: john
* @Date: 2017-03-11 12:23:25
* @Last Modified by: john
* @Last Modified time: 2017-03-11 12:30:37
* @Description: use mysql c api operation db...
*/
// mysql_helper.cpp

#include "mysql_helper.h"

namespace MysqlHelper
{
	MysqlHelper::MysqlHelper()
		:m_bConnected(false)
	{
		m_ptrMysql = mysql_init(nullptr);
	}

	MysqlHelper::MysqlHelper(const string &szHost = "", const string &szUser = "", const string &szPasswd = "", const string &szDBName = "", const string &szCharSet = "", int iPort = 0, int iFlag = 0)
		: m_bConnected(false)
	{
		Init(szHost, szUser, szPasswd, szDBName, szCharSet, iPort, iFlag);

		m_ptrMysql = mysql_init(nullptr);
	}

	MysqlHelper::MysqlHelper(const MysqlConf& stMysqlConf)
		:m_bConnected(false)
	{
		Init(stMysqlConf);

		m_ptrMysql = mysql_init(nullptr);
	}

	MysqlHelper::~MysqlHelper()
	{
		if (nullptr != m_ptrMysql)
		{
			mysql_close(m_ptrMysql);
			m_ptrMysql = nullptr;
		}
	}

	void MysqlHelper::Init(const string &szHost = "", const string &szUser = "", const string &szPasswd = "", const string &szDBName = "", const string &szCharSet = "", int iPort = 0, int iFlag = 0)
	{
		m_stDBConf._szHost = szHost;
		m_stDBConf._szUser = szUser;
		m_stDBConf._szPasswd = szPasswd;
		m_stDBConf._szDBName = szDBName;
		m_stDBConf._szCharSet = szCharSet;
		m_stDBConf._iPort = iPort;
		m_stDBConf._iFlag = iFlag;
	}

	void MysqlHelper::Init(const MysqlConf& stMysqlConf)
	{
		m_stDBConf = stMysqlConf;
	}

	void MysqlHelper::MysqlConnect()
	{
		MysqlDisconnect();
		
		if (nullptr == m_ptrMysql)
		{
			m_ptrMysql = mysql_init(nullptr);
		}

		//建立连接后, 自动调用设置字符集语句
		if (!m_stDBConf._szCharSet.empty()) {
			if (mysql_options(m_ptrMysql, MYSQL_SET_CHARSET_NAME, m_stDBConf._szCharSet.c_str())) {
				throw MysqlException(string("MysqlHelper::connect: mysql_options MYSQL_SET_CHARSET_NAME ") + m_stDBConf._szCharSet + ":" + string(mysql_error(m_ptrMysql)));
			}
		}

		if (mysql_real_connect(m_ptrMysql, m_stDBConf._szHost.c_str(), m_stDBConf._szUser.c_str(), m_stDBConf._szPasswd.c_str(), m_stDBConf._szDBName.c_str(), m_stDBConf._iPort, nullptr, m_stDBConf._iFlag) == nullptr)
		{
			throw MysqlException("[MysqlHelper::connect]: mysql_real_connect: " + string(mysql_error(m_ptrMysql)));
		}

		m_bConnected = true;
	}

	void MysqlHelper::MysqlDisconnect()
	{
		if (nullptr != m_ptrMysql)
		{
			mysql_close(m_ptrMysql);
			m_ptrMysql = mysql_init(nullptr);
		}

		m_bConnected = false;
	}


}
