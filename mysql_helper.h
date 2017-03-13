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
    *@brief ���ݿ��쳣�ṹ��
    **********************/
    struct MysqlException
    {
        MysqlHelper(const string &szBuffer) : m_errInfo(szBuffer) {};
        ~MysqlHelper() throw() {};

        string m_errInfo;
    };
    
    /***********************
    * @brief ���ݿ����ýӿ�
    ***********************/
    struct MysqlConf
    {
        string _szHost;             // ������ַ
        string _szUser;             // �û���
        string _szPasswd;           // ����
        string _szDBName;           // ���ݿ�
        string _szCharSet;          // �����ַ���
        int _iPort;                 // �˿�
        int _iFlag;                 // ���

        MysqlConf() : _iPort(0), _iFlag(0) {};
        
        /**********************************
        * @brief ��ȡ���ݿ�����
        * @param mpParam ������ݿ����õ�map 
        *        dbhost: ������ַ
        *        dbuser:�û���
        *        dbpass:����
        *        dbname:���ݿ�����
        *        dbport:�˿�
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
    * @brief:MySQL���ݿ������ 
    * @feature:���̰߳�ȫ��ͨ��һ���߳�һ��MysqlHelper����
    *          ����insert/update�����и��õĺ�����װ����֤SQLע�룻
    *          MysqlHelper::DB_INT��ʾ��װsql���ʱ�����ӡ�����ת�壻
    *          MysqlHelper::DB_STR��ʾ��װsql���ʱ���ӡ�����ת�壻
    **************************************************************/
    class MysqlHelper
    {
    public:
        /**
        * @brief ���캯��
        */
        MysqlHelper();

		/**
		* @brief ���캯��
		* @param szHost			����IP
		* @param szUser			�û�
		* @param szPasswd		����
		* @param szDBName		���ݿ�
		* @param szCharSet		�ַ����뼯
		* @param iPort			�˿�
		* @param iFlag			�ͻ��˱�ʶ
		*/
		MysqlHelper(const string &szHost = "", const string &szUser = "", const string &szPasswd = "", const string &szDBName = "", const string &szCharSet = "", int iPort = 0, int iFlag = 0);

		/**
		* @brief ���캯��.
		* @param stMysqlConf ���ݿ�����
		*/
		MysqlHelper(const MysqlConf& stMysqlConf);

		/**
		* @brief ��������.
		*/
        ~MysqlHelper();

		/**
		* @brief ��ʼ��.
		*
		* @param szHost			����IP
		* @param szUser			�û�
		* @param szPasswd		����
		* @param szDBName		���ݿ�
		* @param szCharSet		�ַ����뼯
		* @param iPort			�˿�
		* @param iFlag			�ͻ��˱�ʶ
		* @return ��
		*/
		void Init(const string &szHost = "", const string &szUser = "", const string &szPasswd = "", const string &szDBName = "", const string &szCharSet = "", int iPort = 0, int iFlag = 0);

		/**
		* @brief ��ʼ��.
		* @param stMysqlConf ���ݿ�����
		*/
		void Init(const MysqlConf& stMysqlConf);

		/**
		* @brief �������ݿ�.
		*
		* @throws MysqlException
		* @return ��
		*/
		void MysqlConnect();

		/**
		* @brief �Ͽ����ݿ�����.
		* @return ��
		*/
		void MysqlDisconnect();

		/**
		* @brief ��ȡ���ݿ����.
		* @return ���ݿ����
		*/
		void GetVariables(const string &szDBName, string &szResult);

		/**
		*  @brief ֱ�ӻ�ȡ���ݿ�ָ��.
		*
		* @return MYSQL* ���ݿ�ָ��
		*/
		MYSQL *GetMysql();

		/**
		*  @brief �ַ�ת��.
		*
		* @param szFrom  Դ�ַ���
		* @param szTo    ����ַ���
		* @return       ����ַ���
		*/
		void EscapeString(const string &szFrom, string &szTo);

		/**
		* @brief ���»��߲�������.
		*
		* @param szSQL  sql���
		* @throws      MysqlException
		* @return
		*/
		void ExecuteSQL(const string &szSQL);

		/**
		*  @brief mysql��һ����¼
		*/
		class MysqlRecord
		{
		public:
			/**
			* @brief ���캯��.
			* @param record
			*/
			MysqlRecord(const map<string, string> &mpRecord);

			/**
			* @brief ��ȡ����
			* @param szFieldName Ҫ��ȡ���ֶ�
			* @return  ���ϲ�ѯ�����ļ�¼��ֵ
			*/
			const string& operator[](const string &szFieldName);

		protected:
			map<string, string> m_mpRecord;
		};

		/**
		* @brief ��ѯ������mysql����
		*/
		class MysqlData
		{
		public:
			/**
			* @brief ��������.
			*
			* @return vector<map<string,string>>&
			*/
			vector<map<string, string>> &Data();

			/**
			* ���ݵļ�¼����
			*
			* @return size_t
			*/
			size_t Size();

			/**
			* @brief ��ȡĳһ����¼.
			*
			* @param index  Ҫ��ȡ�ڼ�����¼
			* @return   MysqlRecord���͵����ݣ����Ը����ֶλ�ȡ�����Ϣ��
			*/
			MysqlRecord operator[](size_t index);

		protected:
			vector<map<string, string>> m_vecData;
		};

		/**
		* @brief Query Record.
		*
		* @param szSQL sql���
		* @throws     MysqlException
		* @return     MysqlData���͵����ݣ����Ը����ֶλ�ȡ�����Ϣ
		*/
		MysqlData QueryRecord(const string &szSQL);

		/**
		* @brief �����ֶ����ͣ�
		*  DB_INT:��������
		*  DB_STR:�ַ�������
		*/
		enum EFIELDT
		{
			DB_INT,
			DB_STR,
		};

		/**
		* ���ݼ�¼
		*/
		typedef map<string, pair<EFIELDT, string>> RECORD_DATA;

		/**
		* @brief ���¼�¼.
		*
		* @param szTableName ����
		* @param mpColumns  ����/ֵ��
		* @param szCondition where�����,����:where A = B
		* @throws           MysqlException
		* @return           size_t Ӱ�������
		*/
		size_t UpdateRecord(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns, const string &szCondition);

		/**
		* @brief �����¼.
		*
		* @param szTableName  ����
		* @param mpColumns  ����/ֵ��
		* @throws           MysqlException
		* @return           size_t Ӱ�������
		*/
		size_t InsertRecord(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns);

		/**
		* @brief �滻��¼.
		*
		* @param szTableName  ����
		* @param mpColumns   ����/ֵ��
		* @throws            MysqlException
		* @return            size_t Ӱ�������
		*/
		size_t ReplaceRecord(const string &sTableName, const map<string, pair<EFIELDT, string> > &mpColumns);

		/**
		* @brief ɾ����¼.
		*
		* @param szTableName   ����
		* @param szCondition   where�����,����:where A = B
		* @throws             MysqlException
		* @return             size_t Ӱ�������
		*/
		size_t DeleteRecord(const string &szTableName, const string &szCondition = "");

		/**
		* @brief ��ȡTable��ѯ�������Ŀ.
		*
		* @param szTableName ���ڲ�ѯ�ı���
		* @param szCondition where�����,����:where A = B
		* @throws           MysqlException
		* @return           size_t ��ѯ�ļ�¼��Ŀ
		*/
		size_t GetRecordCount(const string& szTableName, const string &szCondition = "");

		/**
		* @brief ��ȡSql���ؽ�����ĸ���.
		*
		* @param szCondition where�����,����:where A = B
		* @throws           MysqlException
		* @return           ��ѯ�ļ�¼��Ŀ
		*/
		size_t GetSqlCount(const string &szCondition = "");

		/**
		* @brief ���ڼ�¼.
		*
		* @param szSQL  sql���
		* @throws     MysqlException
		* @return     �����Ƿ�ɹ�
		*/
		bool ExistRecord(const string& szSQL);

		/**
		* @brief ��ȡ�ֶ����ֵ.
		*
		* @param szTableName ���ڲ�ѯ�ı���
		* @param szFieldName ���ڲ�ѯ���ֶ�
		* @param szCondition where�����,����:where A = B
		* @throws           MysqlException
		* @return           ��ѯ�ļ�¼��Ŀ
		*/
		int GetMaxValue(const string& szTableName, const string& szFieldName, const string &szCondition = "");

		/**
		* @brief ��ȡauto_increment�������ID.
		*
		* @return IDֵ
		*/
		long LastInsertID();

		/**
		* @brief ����Insert-SQL���.
		*
		* @param szTableName  ����
		* @param mpColumns  ����/ֵ��
		* @return           string insert-SQL���
		*/
		string BuildInsertSQL(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns);

		/**
		* @brief ����Replace-SQL���.
		*
		* @param sZTableName  ����
		* @param mpColumns ����/ֵ��
		* @return           string insert-SQL���
		*/
		string BuildReplaceSQL(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns);

		/**
		* @brief ����Update-SQL���.
		*
		* @param szTableName  ����
		* @param mpColumns   ����/ֵ��
		* @param szCondition  where�����
		* @return            string Update-SQL���
		*/
		string BuildUpdateSQL(const string &szTableName, const map<string, pair<EFIELDT, string> > &mpColumns, const string &szCondition);

		/**
		* @brief ��ȡ���ִ�е�SQL���.
		*
		* @return SQL���
		*/
		string GetLastSQL() { return m_szLastSQL; }

		/**
		* @brief ��ȡ��ѯӰ����
		* @return int
		*/
		size_t GetAffectedRows();
    
    protected:
		/**
		* @brief copy contructor��ֻ����,������,��֤����ʹ��
		*/
		MysqlHelper(const MysqlHelper &objMysql);

		/**
		*
		* @brief ֻ����,������,��֤����ʹ��
		*/
		MysqlHelper &operator=(const MysqlHelper &objMysql);

	private:
		/**
		* ���ݿ�ָ��
		*/
		MYSQL *m_ptrMysql;

		/**
		* ���ݿ�����
		*/
		MysqlConf m_stDBConf;

		/**
		* �Ƿ��Ѿ�����
		*/
		bool m_bConnected;

		/**
		* ���ִ�е�sql
		*/
		string m_szLastSQL;

    };

}

#endif // _MYSQL_HELPER_H_
