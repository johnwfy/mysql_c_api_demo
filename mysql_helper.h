/*
 * @Author: john 
 * @Date: 2017-03-11 12:23:25 
 * @Last Modified by: john
 * @Last Modified time: 2017-03-11 12:30:37
 * @Description: use mysql c api operation db...
 */
// mysql_helper.h

#ifndef _MYSQL_HELPER_H_
#define _MYSQL_HELPER_H_

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <mysql/mysql.h>
#include <map>

using namespace std;

namespace MysqlHelper
{
    /*********************
    *@brief 数据库异常结构体
    **********************/
    struct MysqlException
    {
        MysqlHelper(const string &szBuffer) : m_errInfo(szBuffer) {};
        ~MysqlHelper() throw() {};

        string m_errInfo;
    };
    
    /***********************
    * @brief 数据库配置接口
    ***********************/
    struct MysqlConf
    {
        string _szHost;             // 主机地址
        string _szUser;             // 用户名
        string _szPasswd;           // 密码
        string _szDBName;           // 数据库
        string _szCharSet;          // 编码字符集
        int _iPort;                 // 端口
        int _iFlag;                 // 标记

        MysqlConf() : _iPort(0), _iFlag(0) {};
        
        /**********************************
        * @brief 读取数据库配置
        * @param mpParam 存放数据库配置的map 
        *        dbhost: 主机地址
        *        dbuser:用户名
        *        dbpass:密码
        *        dbname:数据库名称
        *        dbport:端口
        **********************************/
        void LoadDataFromMap(const map<string, string> &mpParam)
        {
            map<string, string> mpTempParam = mpParam;

            _szHost = mpTempParam["dbhost"];
            _szUser = mpTempParam["dbuser"];
            _szPasswd = mpTempParam["dbpass"];
            _szDBName = mpTempParam["dbname"];
            _szCharSet = mpTempParam["charset"];
            _iPort = atoi(mpTempParam["dbport"].c_str());
            _iFlag = 0;

            if(mpTempParam["dbport"] == "")
            {
                _iPort = 3306;
            }

        }

    };

    /**************************************************************
    * @brief:MySQL数据库操作类 
    * @feature:非线程安全，通常一个线程一个MysqlHelper对象；
    *          对于insert/update可以有更好的函数封装，保证SQL注入；
    *          MysqlHelper::DB_INT表示组装sql语句时，不加””和转义；
    *          MysqlHelper::DB_STR表示组装sql语句时，加””并转义；
    **************************************************************/
    class MysqlHelper
    {
    public:
        /**
        * @brief 构造函数
        */
        MysqlHelper();

		/**
		* @brief 构造函数
		* @param szHost			主机IP
		* @param szUser			用户
		* @param szPasswd		密码
		* @param szDBName		数据库
		* @param szCharSet		字符编码集
		* @param iPort			端口
		* @param iFlag			客户端标识
		*/
		MysqlHelper(const string &szHost = "", const string &szUser = "", const string &szPasswd = "", const string &szDBName = "", const string &szCharSet = "", int iPort = 0, int iFlag = 0);

		/**
		* @brief 构造函数.
		* @param stMysqlConf 数据库配置
		*/
		MysqlHelper(const MysqlConf& stMysqlConf);

		/**
		* @brief 析构函数.
		*/
        ~MysqlHelper();

		/**
		* @brief 初始化.
		*
		* @param szHost			主机IP
		* @param szUser			用户
		* @param szPasswd		密码
		* @param szDBName		数据库
		* @param szCharSet		字符编码集
		* @param iPort			端口
		* @param iFlag			客户端标识
		* @return 无
		*/
		void Init(const string &szHost = "", const string &szUser = "", const string &szPasswd = "", const string &szDBName = "", const string &szCharSet = "", int iPort = 0, int iFlag = 0);

		/**
		* @brief 初始化.
		* @param stMysqlConf 数据库配置
		*/
		void Init(const MysqlConf& stMysqlConf);

		/**
		* @brief 连接数据库.
		*
		* @throws MysqlException
		* @return 无
		*/
		void MysqlConnect();

		/**
		* @brief 断开数据库连接.
		* @return 无
		*/
		void MysqlDisconnect();

		/**
		* @brief 获取数据库变量.
		* @return 数据库变量
		*/
		void GetVariables(const string &szDBName, string &szResult);

		/**
		*  @brief 直接获取数据库指针.
		*
		* @return MYSQL* 数据库指针
		*/
		MYSQL *GetMysql();

		/**
		*  @brief 字符转义.
		*
		* @param szFrom  源字符串
		* @param szTo    输出字符串
		* @return       输出字符串
		*/
		void EscapeString(const string &szFrom, string &szTo);

		/**
		* @brief 更新或者插入数据.
		*
		* @param szSQL  sql语句
		* @throws      MysqlException
		* @return
		*/
		void ExecuteSQL(const string &szSQL);

		/**
		*  @brief mysql的一条记录
		*/
		class MysqlRecord
		{
		public:
			/**
			* @brief 构造函数.
			* @param record
			*/
			MysqlRecord(const map<string, string> &mpRecord);

			/**
			* @brief 获取数据
			* @param szFieldName 要获取的字段
			* @return  符合查询条件的记录的值
			*/
			const string& operator[](const string &szFieldName);

		protected:
			map<string, string> m_mpRecord;
		};

		/**
		* @brief 查询出来的mysql数据
		*/
		class MysqlData
		{
		public:
			/**
			* @brief 所有数据.
			*
			* @return vector<map<string,string>>&
			*/
			vector<map<string, string>> &Data();

			/**
			* 数据的记录条数
			*
			* @return size_t
			*/
			size_t Size();

			/**
			* @brief 获取某一条记录.
			*
			* @param index  要获取第几条记录
			* @return   MysqlRecord类型的数据，可以根据字段获取相关信息，
			*/
			MysqlRecord operator[](size_t index);

		protected:
			vector<map<string, string>> m_vecData;
		};

		/**
		* @brief Query Record.
		*
		* @param szSQL sql语句
		* @throws     MysqlException
		* @return     MysqlData类型的数据，可以根据字段获取相关信息
		*/
		MysqlData QueryRecord(const string &szSQL);

		/**
		* @brief 定义字段类型，
		*  DB_INT:数字类型
		*  DB_STR:字符串类型
		*/
		enum EFIELDT
		{
			DB_INT,
			DB_STR,
		};

		/**
		* 数据记录
		*/
		typedef map<string, pair<EFIELDT, string>> RECORD_DATA;

		/**
		* @brief 更新记录.
		*
		* @param szTableName 表名
		* @param mpColumns  列名/值对
		* @param szCondition where子语句,例如:where A = B
		* @throws           MysqlException
		* @return           size_t 影响的行数
		*/
		size_t UpdateRecord(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns, const string &szCondition);

		/**
		* @brief 插入记录.
		*
		* @param szTableName  表名
		* @param mpColumns  列名/值对
		* @throws           MysqlException
		* @return           size_t 影响的行数
		*/
		size_t InsertRecord(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns);

		/**
		* @brief 替换记录.
		*
		* @param szTableName  表名
		* @param mpColumns   列名/值对
		* @throws            MysqlException
		* @return            size_t 影响的行数
		*/
		size_t ReplaceRecord(const string &sTableName, const map<string, pair<EFIELDT, string> > &mpColumns);

		/**
		* @brief 删除记录.
		*
		* @param szTableName   表名
		* @param szCondition   where子语句,例如:where A = B
		* @throws             MysqlException
		* @return             size_t 影响的行数
		*/
		size_t DeleteRecord(const string &szTableName, const string &szCondition = "");

		/**
		* @brief 获取Table查询结果的数目.
		*
		* @param szTableName 用于查询的表名
		* @param szCondition where子语句,例如:where A = B
		* @throws           MysqlException
		* @return           size_t 查询的记录数目
		*/
		size_t GetRecordCount(const string& szTableName, const string &szCondition = "");

		/**
		* @brief 获取Sql返回结果集的个数.
		*
		* @param szCondition where子语句,例如:where A = B
		* @throws           MysqlException
		* @return           查询的记录数目
		*/
		size_t GetSqlCount(const string &szCondition = "");

		/**
		* @brief 存在记录.
		*
		* @param szSQL  sql语句
		* @throws     MysqlException
		* @return     操作是否成功
		*/
		bool ExistRecord(const string& szSQL);

		/**
		* @brief 获取字段最大值.
		*
		* @param szTableName 用于查询的表名
		* @param szFieldName 用于查询的字段
		* @param szCondition where子语句,例如:where A = B
		* @throws           MysqlException
		* @return           查询的记录数目
		*/
		int GetMaxValue(const string& szTableName, const string& szFieldName, const string &szCondition = "");

		/**
		* @brief 获取auto_increment最后插入得ID.
		*
		* @return ID值
		*/
		long LastInsertID();

		/**
		* @brief 构造Insert-SQL语句.
		*
		* @param szTableName  表名
		* @param mpColumns  列名/值对
		* @return           string insert-SQL语句
		*/
		string BuildInsertSQL(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns);

		/**
		* @brief 构造Replace-SQL语句.
		*
		* @param sZTableName  表名
		* @param mpColumns 列名/值对
		* @return           string insert-SQL语句
		*/
		string BuildReplaceSQL(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns);

		/**
		* @brief 构造Update-SQL语句.
		*
		* @param szTableName  表名
		* @param mpColumns   列名/值对
		* @param szCondition  where子语句
		* @return            string Update-SQL语句
		*/
		string BuildUpdateSQL(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns, const string &szCondition);

		/**
		* @brief 获取最后执行的SQL语句.
		*
		* @return SQL语句
		*/
		string GetLastSQL() { return m_szLastSQL; }

		/**
		* @brief 获取查询影响数
		* @return int
		*/
		size_t GetAffectedRows();
    
    protected:
		/**
		* @brief copy contructor，只申明,不定义,保证不被使用
		*/
		MysqlHelper(const MysqlHelper &objMysql);

		/**
		*
		* @brief 只申明,不定义,保证不被使用
		*/
		MysqlHelper &operator=(const MysqlHelper &objMysql);

	private:
		/**
		* 数据库指针
		*/
		MYSQL *m_ptrMysql;

		/**
		* 数据库配置
		*/
		MysqlConf m_stDBConf;

		/**
		* 是否已经连接
		*/
		bool m_bConnected;

		/**
		* 最后执行的sql
		*/
		string m_szLastSQL;

    };

}

#endif // _MYSQL_HELPER_H_
