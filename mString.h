/**	
* my::string ����������� acl::string, my::locker �������
* acl::locker��
* ��������ʹ��ACL���������������� mString.h �ļ��ж�����
* #define USING_ACL 1����ʾ�Ƿ�ʹ��ACL�ַ�����ͬ��������ESU��
* �� USING_ACL ����Ϊ1����ʹ��ACL���ַ�����������������ʹ��
* ���ļ����ַ�������������
* ��������ʹ��ACL�������CSqliteManager������й����ݿ���޸ġ�
*/

#pragma once

// �Ƿ�ʹ��ACL
#define USING_ACL 0

// �Ƿ�ʹ��ACL��Sqlite���ݿ�
#define USING_ACL_SQLITE 0

// my string��ʼʱ��Ĭ��ֵ��bytes��
#define DEFAULT_SIZE 1024


#if USING_ACL

//acl���ļ�
#include "acl_cpp/lib_acl.hpp"
#include "acl_cpp/stdlib/locker.hpp"
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/db/db_sqlite.hpp"

#define my acl
#define NAMES acl

#else

#define MY_API

#include <stdlib.h>
#include <string>
#include <stdarg.h>
#include <windows.h>
#include <process.h>
#include "cppsqlite3/CppSQLite3.h"
#include "StrTransfer.h"
#include <vector>

namespace my
{
	/**
	* @class locker
	* @brief �����������acl::locker
	*/
	class locker
	{
	public:
		locker() { ::InitializeCriticalSection(&m_csLock); }
		~locker(){ ::DeleteCriticalSection(&m_csLock); }

		void lock() { EnterCriticalSection(&m_csLock); }
		void unlock() { LeaveCriticalSection(&m_csLock); }

	private:
		CRITICAL_SECTION	m_csLock;
	};

	/**
	* @class string
	* @brief �Զ����ַ��������acl::string
	* @details ����������� acl::string��
	* ��������ʹ��ACL���������������� mString.h �ļ��ж�����
	* #define USING_ACL 1����ʾ�Ƿ�ʹ��ACL�ַ�����ͬ��������ESU��
	* �� USING_ACL ����Ϊ1����ʹ��ACL���ַ�����������������ʹ��
	* ���ļ����ַ�������������
	* ��������ʹ��ACL�������CSqliteManager������й����ݿ���޸ġ�
	*/
	class string
	{
	private:
		int *m_nRef;		// ���ü���
		void AddRef() const { ++ *m_nRef; }
		int RemoveRef() const { return -- *m_nRef; }

	protected:
		int m_nSize;			// ����������
		char *m_pBuf;			// �ַ���ָ��
		void resize(int nSize, int nLen) // ��С����
		{
			if (nSize > m_nSize)
			{
				m_nSize = nSize;
				const char *t = m_pBuf;
				m_pBuf = new char[m_nSize];
				memcpy(m_pBuf, t, nLen + 1);
				delete [] t;
			}
		}
		char* addr(int i) const { return m_pBuf + i; }
		char& at(int i) const { return m_pBuf[i]; }

	public:
		void print(const char *method = NULL) const
		{
			printf("%s %s (Ref=%d).\n", method ? method : "", 
				*m_pBuf ? m_pBuf : "null", *m_nRef);
		}
		int GetRef() const { return *m_nRef; }
		char* c_str() const { return m_pBuf; }
		bool empty() const { return 0 == m_pBuf[0]; }

		explicit string(int nLen = DEFAULT_SIZE)
		{
			m_nRef = new int(1);
			m_nSize = max(nLen, 1);
			m_pBuf = new char[m_nSize];
			m_pBuf[0] = 0;
#if PRINT_REF
			print("Ĭ�Ϲ���");
#endif
		}

		string(const char *pSrc)
		{
			m_nRef = new int(1);
			m_nSize = pSrc ? strlen(pSrc) + 1 : DEFAULT_SIZE;
			m_pBuf = new char[m_nSize];
			strcpy(m_pBuf, pSrc ? pSrc : "");
#if PRINT_REF
			print("����");
#endif
		}

		string(const string &other)
		{
			m_nSize = other.m_nSize;
			m_pBuf = other.m_pBuf;
			m_nRef = other.m_nRef;
			AddRef();
#if PRINT_REF
			print("��������");
#endif
		}

		string& operator = (const string &other)
		{
			int n = other.length();
			resize(n + 1, n);
			memcpy(m_pBuf, other.c_str(), n);
			m_pBuf[n] = 0;
			return *this;
		}

		~string()
		{
#if PRINT_REF
			print("����");
#endif
			if (0 == RemoveRef())
			{
				delete [] m_pBuf;
				delete m_nRef;
			}
		}

		operator std::string()
		{
			return std::string(m_pBuf);
		}

		string& operator += (const string &other)
		{
			int n = length(), n0 = other.length();
			resize(n + n0 + 1, n);
			memcpy(m_pBuf + n, other.c_str(), n0);
			m_pBuf[n + n0] = 0;
			return *this;
		}

		/**
		* ��յ�ǰ��������ݻ�����
		* @return {string&} ��ǰ���������
		*/
		string& clear()
		{
			memset(m_pBuf, 0, m_nSize);
			return *this;
		}

		/**
		* ��ָ���ַ�������ڵ�ǰ�������ݻ��������ݵ�β��
		* @param s {const string&} Դ���ݶ���ָ��
		* @return {string&} ��ǰ���������
		*/
		string& append(const char* s)
		{
			const char *p = s ? s : "";
			int n = length(), n0 = strlen(p);
			resize(n + n0 + 1, n);
			memcpy(m_pBuf + n, p, n0);
			m_pBuf[n + n0] = 0;
			return *this;
		}

		/**
		* ��ָ���ַ�����������ڵ�ǰ�������ݻ��������ݵ��ײ�
		* @param s {const char*} Դ���ݵ�ַ
		* @return {string&} ��ǰ���������
		*/
		string& prepend(const char* s)
		{
			const char *p = s ? s : "";
			int n = length(), n0 = strlen(p);
			resize(n + n0 + 1, n);
			memcpy(m_pBuf+n0, m_pBuf, n+1);// �Ƿ�ȫ
			memcpy(m_pBuf, p, n0);
			return *this;
		}

		friend 	bool operator == (const string &s1, const string &s2)
		{
			return 0 == strcmp(s1.c_str(), s2.c_str());
		}

		friend 	bool operator != (const string &s1, const string &s2)
		{
			return 0 != strcmp(s1.c_str(), s2.c_str());
		}

		/**
		* ����ָ���ַ��ڵ�ǰ���󻺳�����λ�ã��±�� 0 ��ʼ��
		* @param n {char} Ҫ���ҵ��з����ַ�
		* @return {int} �ַ��ڻ������е�λ�ã�������ֵ < 0 ���ʾ������
		*/
		int find(char n) const
		{
			int i = 0;
			for (const char *p = m_pBuf; (n != *p && i < m_nSize); ++i)
				++p;
			return i == m_nSize ? -1 : i;
		}

		/**
		* ��β����ǰ����ָ���ַ����ڵ�ǰ���󻺳�������ʼλ�ã��±�� 0 ��ʼ��
		* @param needle {const char*} Ҫ���ҵ��з����ַ���
		* @param case_sensitive {bool} Ϊ true ��ʾ���ִ�Сд
		* @return {char*} �ַ����ڻ������е���ʼλ�ã�������ֵΪ��ָ�����ʾ������
		*/
		char* rfind(const char* needle, bool case_sensitive=true) const
		{
			int n = length(), n0 = needle ? strlen(needle) : 0;
			char *p = m_pBuf + n - n0, *p0 = needle ? needle : "";
			do{
				if (0 == strncmp(p, p0, n0))
					return p;
			}while (--p >= m_pBuf);
			return NULL;
		}

		/**
		* ���شӵ�ǰ�ַ��������л�����ָ��λ�����������
		* @param npos {size_t} �±�λ�ã�����ֵ���ڵ��ڵ�ǰ�ַ��������ݳ���ʱ��
		*  �򷵻������ַ������󣻷���ֵ��������ֵָ��λ�õ��ַ�����
		* @return {string} ����ֵΪһ�����Ķ��󣬲���Ҫ�����ͷţ��ú�����Ч��
		*  ���ܲ���̫��
		*/
		string left(int npos)
		{
			string ret(npos + 1);
			memcpy(ret.c_str(), m_pBuf, npos);
			ret.at(npos) = 0;
			return ret;
		}

		/**
		* ���شӵ�ǰ�ַ��������л�����ָ��λ�����ҵ�����
		* @param npos {size_t} �±�λ�ã�����ֵ���ڵ��ڵ�ǰ�ַ��������ݳ���ʱ��
		*  �򷵻ص��ַ�����������Ϊ�գ�����ֵ��������ֵָ��λ�õ��ַ�����
		* @return {const string} ����ֵΪһ�����Ķ��󣬲���Ҫ�����ͷţ���
		*  ������Ч�ʿ��ܲ���̫��
		*/
		string right(int npos)
		{
			int len = length() - npos;
			len = len < 0 ? 0 : len;
			string ret(len + 1);
			memcpy(ret.c_str(), addr(npos), len);
			ret.at(len) = 0;
			return ret;
		}

		/**
		* ���ص�ǰ�����ַ����ĳ��ȣ�����\0��
		* @return {size_t} ����ֵ >= 0
		*/
		int length() const
		{
			const char *p = m_pBuf;
			while (*p)
				++p;
			return p - m_pBuf;
		}

		/**
		* ����ʽ��ʽ��������ݣ������� sprintf �ӿڷ�ʽ��
		* @param fmt {const char*} ��ʽ�ַ���
		* @param ... �������
		* @return {string&} ��ǰ���������
		*/
		string& format(const char* fmt, ...)
		{
			va_list ap;
			va_start(ap, fmt);
			vsprintf(m_pBuf, fmt, ap);
			va_end(ap);
			return *this;
		}
	};

	/**
	* ���麯�����߳������࣬����ʵ���� run �����������߳��б�ִ�е�
	* ����ο���ACL
	*/
	class MY_API thread_job
	{
	public:
		thread_job() {}
		virtual ~thread_job() {}

		/**
		* ���麯�����������ʵ�ִ˺������ú��������߳���ִ��
		* @return {void*} �߳��˳�ǰ���صĲ���
		*/
		virtual void* run() = 0;
	};

	/**
	* �̴߳����࣬����Ľӿڶ��������� Java �Ľӿڶ��壬������Ҫʵ��
	* ����Ĵ��麯����ʹ����ͨ������ thread::start() �����̹߳���
	* ����ο���ACL
	*/
	class MY_API thread : public thread_job
	{
	public:
		thread() : is_running(false), detachable(false) { }
		virtual ~thread() { }

		/**
		* ��ʼ�����̹߳��̣�һ���ú��������ã������������һ���µ�
		* ���̣߳������߳���ִ�л��� thread_job::run ����
		* @return {bool} �Ƿ�ɹ������߳�
		*/
		bool start() { is_running = true; _beginthread(thread_run, 0, this);  return is_running; }

		/**
		* �������߳�ʱΪ�� detachable ״̬���������ô˺����ȴ��߳̽�����
		* �������߳�ʱΪ detachable ״̬ʱ����ֹ���ñ�����
		* @param out {void**} ���ò����ǿ�ָ��ʱ���ò����������
		*  �߳��˳�ǰ���صĲ���
		* @return {bool} �Ƿ�ɹ�
		*/
		void wait(void** out = NULL) { if (false == detachable) while(is_running) Sleep(1); }

		/**
		* �ڵ��� start ǰ���ô˺������������������߳��Ƿ�Ϊ
		* ���� (detachable) ״̬�����δ���ô˺�������������
		* ���߳�Ĭ��Ϊ����״̬
		* @param yes {bool} �Ƿ�Ϊ����״̬
		* @return {thread&}
		*/
		thread& set_detachable(bool yes) { detachable = yes; return *this; }

	private:
		bool is_running;
		bool detachable;
		void end() { is_running = false; }
		static void thread_run(void* arg) { thread *pThis = (thread*)arg; if (pThis){ pThis->run(); pThis->end(); } }
	};

#if USING_ACL_SQLITE

#define NAMES acl

#else

#define NAMES my

	/**
	* 32�ֽ��ַ���
	*/
	typedef struct str32 
	{
		char data[64];
		str32(const char *pStr) { strcpy_s(data, pStr ? pStr : ""); };
		str32(const str32 &other) { memcpy(data, other.data, sizeof(data)); }
		const char* c_str() const { return data; }
	}str32;

	/**
	* ���ݿ��ѯ��������м�¼���Ͷ���
	*/
	class MY_API db_row
	{
	public:
		/**
		* ���캯��
		* @param names {const std::vector<const char*>&} ���ݿ�����ֶ����б�
		*/
		db_row(const std::vector<str32>& n, const std::vector<str32>& v)
		{
			names.assign(n.begin(), n.end());
			values.assign(v.begin(), v.end());
		}
		~db_row() { }

		/**
		* �Ӳ�ѯ����ļ�¼���и����ֶ���ȡ����Ӧ���ֶ�ֵ��
		* ������ field_value ��ͬ
		* @param name {const char*} ���ݱ���ֶ���
		* @return {const char*} ��Ӧ���ֶ�ֵ��Ϊ�����ʾ�ֶ�ֵ�����ڻ�
		*  �ֶ����Ƿ�
		*/
		const char* operator[](const char* n) const
		{
			std::vector<str32>::const_iterator pos = names.begin();
			for (int i = 0; pos != names.end(); ++i, ++pos)
				if (0 == strcmp(pos->c_str(), n))
					return values.at(i).c_str();
			return "";
		}

		// ��ȡ��ѯ����еĵ�n���ֶ�ֵ
		inline const char* operator[](int n) const{ return values.at(n).c_str(); }

	private:
		// ���ݱ���ֶ�������
		std::vector<str32> names;

		// ���ݽ���е��ֶμ���
		std::vector<str32> values;
	};

	/**
	* ���ݿ��ѯ������м�¼�������Ͷ���
	*/
	class MY_API db_rows
	{
	public:
		db_rows() { }
		virtual ~db_rows()
		{ 
			for (std::vector<db_row*>::const_iterator p = rows.begin(); p != rows.end(); ++p)
				if (*p) delete *p;
		}

		/**
		* ȡ�����еĲ�ѯ�����
		* @return {const std::vector<db_row*>&} �����м�¼�������Ͷ���
		*  ����ͨ������ db_rows.empty() ���жϽ���Ƿ�Ϊ��
		*/
		const std::vector<db_row*>& get_rows() const { return rows; }

		/**
		* �Ӳ�ѯ���м�¼�����и��������±�ȡ�ö�Ӧ��ĳ�м�¼
		* @param idx {size_t} �����±꣬��ֵӦ�� < �������С
		* @return {const db_row*} ���ؿձ�ʾ�����±�ֵ�Ƿ����ֶ�ֵ����
		*  Ϊ��
		*/
		const db_row* operator[](size_t idx) const { return rows.at(idx); }

		/**
		* �жϽ�����Ƿ�Ϊ��
		* @return {bool} �Ƿ�Ϊ��
		*/
		bool empty() const { return rows.empty(); }

		/**
		* ��������м�¼����
		* @return {size_t} �м�¼����
		*/
		size_t length() const { return rows.size(); }

		/**
		* ��������push_backһ�����
		*/
		void push(db_row* row) { rows.push_back(row); }

	protected:
		// ��ѯ����м��ϣ����е�Ԫ�� db_row �����Ƕ�̬��ӽ�ȥ�ģ�
		// ��Ϊ�ڱ����������ʱ���Զ� delete rows �е�����Ԫ�ض���
		std::vector<db_row*> rows;
	};

	/**
	* @class db_handle
	* @brief �����滻acl::db_handle
	*/
	class db_handle
	{
	protected:
		CppSQLite3DB *m_pDatabase;

	public:
		db_handle() : m_pDatabase(NULL) { }
		virtual ~db_handle() { if (m_pDatabase) { m_pDatabase->close(); delete m_pDatabase; } }

		/**
		* �����ö�̬���ط�ʽ���ض�̬��ʱ������ʹ�ô˺������ö�̬��ļ���ȫ·��
		*/
		static void set_loadpath(const char* path) { printf("Load \"%s\". \n", path); }

		/**
		* ���� connect_client �麯����ʵ��
		* @return {bool} �����ݿ������Ƿ�ɹ�
		*/
		virtual bool open() = 0;

		/**
		* ����ӿڣ��������ʵ�ִ˽ӿ�����ִ�� SELECT SQL ���
		* @param sql {const char*} ��׼�� SQL ��䣬�ǿգ�����һ����Ҫע���
		*  SQL �����뾭��ת�崦���Է�ֹ SQL ע�빥��
		* @return {bool} ִ���Ƿ�ɹ�
		*/
		virtual bool sql_select(const char* sql) = 0;

		/**
		* @brief �жϱ�����Ƿ�����ֶ�
		*/
		virtual bool IsColumnExist(const char *tbl_name, const char *col_name) { return false; }

		/**
		* ���ִ�� SQL ����Ľ��
		* @return {const db_rows*}�����ؽ�����ǿգ����������Ҫ����
		*  free_result() ���ͷŽ������
		*/
		virtual const db_rows* get_result() const = 0;

		/**
		* �ͷ��ϴβ�ѯ�Ľ��������ѯ��ɺ󣬵��øú������ͷ��ϴβ�ѯ�Ľ�����ú�����
		* ��ε��ò��޺�������Ϊ����һ�ε���ʱ���Զ����ڲ����� result_ �ÿ�,
		* ���⣬Ҫ�����������ÿ��ִ�� SQL ��ѯǰ�ȵ��ô˷����������û�����
		* ���ö�����ڴ�й¶�����⣬�������������ʱ���Զ��ٵ��ñ������ͷſ���
		* δ�ͷŵ��ڴ�
		*/
		virtual void free_result() = 0;

		/**
		* ����ӿڣ��������ʵ�ִ˽ӿ�����ִ�� INSERT/UPDATE/DELETE SQL ���
		* @param sql {const char*} ��׼�� SQL ��䣬�ǿգ�����һ����Ҫע���
		*  SQL �����뾭��ת�崦���Է�ֹ SQL ע�빥��
		* @return {bool} ִ���Ƿ�ɹ�
		*/
		virtual bool sql_update(const char* sql) = 0;

		// ��������
		inline void begin() { if (m_pDatabase) m_pDatabase->beginTransaction(); }

		// �ύ����
		inline void commit() { if (m_pDatabase) m_pDatabase->commitTransaction(); }

		// �ر�ͬ��д
		inline void sync_off() { if (m_pDatabase) m_pDatabase->sync_off(); }

		// ��ȡsqlite3���
		sqlite3* getHandle() { return m_pDatabase ? m_pDatabase->getHandle() : NULL; }
	};

	class db_sqlite : public db_handle
	{
	private:
		string m_sDbFile;
		string m_sCharset;
		db_rows *m_rows;

	public:
		db_sqlite(const char* dbfile, const char* charset = "utf-8") 
			: m_sDbFile(dbfile), m_sCharset(charset), m_rows(NULL) { }
		~db_sqlite(void) { if(m_rows) delete m_rows; }

		/**
		* ���� connect_client �麯����ʵ��
		* @return {bool} �����ݿ������Ƿ�ɹ�
		*/
		virtual bool open()
		{
			m_pDatabase	= m_pDatabase ? m_pDatabase : new CppSQLite3DB;

			//�򿪻��½�һ�����ݿ�
			m_pDatabase->open( m_sDbFile.c_str() );

			//�жϱ����Ƿ����
			return m_pDatabase->tableExists("faceInfo");
		}

		/**
		* ����ӿڣ��������ʵ�ִ˽ӿ�����ִ�� SELECT SQL ���
		* @param sql {const char*} ��׼�� SQL ��䣬�ǿգ�����һ����Ҫע���
		*  SQL �����뾭��ת�崦���Է�ֹ SQL ע�빥��
		* @return {bool} ִ���Ƿ�ɹ�
		*/
		virtual bool sql_select(const char* sql)
		{
			if (NULL == m_pDatabase)
				return false;
			// ��ѯ���
			CppSQLite3Query	querySQLite3 = m_pDatabase->execQuery(sql);
			if(m_rows) delete m_rows;
			m_rows = new db_rows;
			while ( !querySQLite3.eof() )
			{
				int cols = querySQLite3.numFields();
				std::vector<str32> names, values;
				for (int nIdx = 0; nIdx < cols; ++nIdx)
				{
					const char *n = querySQLite3.fieldName(nIdx);
					const char *v = querySQLite3.getStringField(nIdx);
					if (!n && !v) break;
					names.push_back(str32(n));
					values.push_back(str32(v));
				}
				m_rows->push(new db_row(names, values));
				querySQLite3.nextRow();
			}
			querySQLite3.finalize();
			return true;
		}

		/**
		* @brief �жϱ�����Ƿ�����ֶ�
		*/
		virtual bool IsColumnExist(const char *tbl_name, const char *col_name)
		{
			bool ret = false;
			if (m_pDatabase)
			{
				string sql;
				sql.format("SELECT sql from sqlite_master WHERE tbl_name=\"%s\" and type=\"table\";", tbl_name);
				CppSQLite3Query	querySQLite3 = m_pDatabase->execQuery(sql.c_str());
				if (!querySQLite3.eof())
				{
					const char* sess = querySQLite3.getStringField(0);
					ret = strstr(sess, col_name);
				}
				querySQLite3.finalize();
			}
			return ret;
		}

		/**
		* ���ִ�� SQL ����Ľ��
		* @return {const db_rows*}�����ؽ�����ǿգ����������Ҫ����
		*  free_result() ���ͷŽ������
		*/
		virtual const db_rows* get_result() const
		{
			return m_rows ? (m_rows->empty() ? NULL : m_rows) : NULL;
		}

		/**
		* �ͷ��ϴβ�ѯ�Ľ��������ѯ��ɺ󣬵��øú������ͷ��ϴβ�ѯ�Ľ�����ú�����
		* ��ε��ò��޺�������Ϊ����һ�ε���ʱ���Զ����ڲ����� result_ �ÿ�,
		* ���⣬Ҫ�����������ÿ��ִ�� SQL ��ѯǰ�ȵ��ô˷����������û�����
		* ���ö�����ڴ�й¶�����⣬�������������ʱ���Զ��ٵ��ñ������ͷſ���
		* δ�ͷŵ��ڴ�
		*/
		virtual void free_result()
		{
			if(m_rows)
			{
				delete m_rows;
				m_rows = NULL;
			}
		}

		/**
		* ����ӿڣ��������ʵ�ִ˽ӿ�����ִ�� INSERT/UPDATE/DELETE SQL ���
		* @param sql {const char*} ��׼�� SQL ��䣬�ǿգ�����һ����Ҫע���
		*  SQL �����뾭��ת�崦���Է�ֹ SQL ע�빥��
		* @return {bool} ִ���Ƿ�ɹ�
		*/
		virtual bool sql_update(const char* sql)
		{
			if (NULL == m_pDatabase)
				return false;
			const char *pUtf8 = NULL;
			try
			{
				pUtf8 = MByteToUtf8(sql);
				int ret = m_pDatabase->execDML(pUtf8);
				if (pUtf8) delete [] pUtf8;
				return ret > 0;
			}
			catch(CppSQLite3Exception ex)
			{
				TRACE("CppSQLite3Exception: %s \n", ex.errorMessage());
				if (pUtf8) delete [] pUtf8;
				return false;
			}
		}
	};
#endif
}

#endif
