#pragma once

/*!
	ȫ�ֵĻ������ݷ����࣬����
*/
class CGbl
{
public:
	enum EnumUndo{
		UNDO_TYPE_ADD = 1,					//!< ���
		UNDO_TYPE_DEL = 2,					//!< ɾ��
		UNDO_TYPE_UPD = 3,					//!< �޸�

		UNDO_INFO_EXCHANGE = 1,				//!< ����
	};
private:
	CGbl(void);
	~CGbl(void);

	//!< ·���ĺ�߶����С�\��
	CString m_strInstallPath;		//!< ��װĿ¼
	CString m_strExePath;			//!< ��ִ�г���·��
	CString m_strSlnPath;			//!< �������Ĭ��·��
	CString m_strDataPath;			//!< �������ļ�·��
	CString m_strDocPath;			//!< �ĵ��ļ�·��
	CString m_strLanguagePath;		//!< ���԰�·��
	CString m_strProjDefPath;		//!< ����Ĭ��·��

	CString m_strComplierName;		//!< ���������ļ�����������չ������������·��
	CString m_strServerName;		//!< ���������ļ�����������չ������������·��
	CString m_strHmiName;			//!< HMI���ļ�����������չ������������·��
	CString m_strConfigName;		//!< Config���ļ�����������չ������������·��

	//!< �ӿ�ͨ�ò���
	std::list<CString> m_ltBaudStr;
	std::list<CString> m_ltCheckStyleStr;
	std::list<CString> m_ltDataBitNumStr;
	std::list<CString> m_ltStopBitNumStr;

public:
	static CGbl& GetMe();
	const CString getExePath(){return m_strExePath;}				//!< ��ÿ�ִ�г���·��
	const CString getInstallPath(){return m_strInstallPath;}		//!< ��ð�װ��Ŀ¼
	const CString getSolutionPath(){return m_strSlnPath;}			//!< ��ý������Ŀ¼
	const CString getDataPath(){return m_strDataPath;}				//!< ���data�ļ��е�Ŀ¼
	const CString getDocPath(){return m_strDocPath;}				//!< ����ĵ��ļ���Ŀ¼
	const CString getLanguagePath(){return m_strLanguagePath;}		//!< ������԰���Ŀ¼
	const CString getProjDefPath(){return m_strProjDefPath;}		//!< ��ù���Ĭ��·��
	const CString getCompilerName(){return m_strComplierName;}		//!< ��ñ�����������
	const CString getServerName(){return m_strServerName;}			//!< ��õײ���������
	const CString getHmiName(){return m_strHmiName;}				//!< ���HMI���������
	const CString getConfigName(){return m_strConfigName;}			//!< ���Config���������

	std::list<CString> getBaudList(){return m_ltBaudStr;}				//!< ��ò������б�
	std::list<CString> getCheckStyleList(){return m_ltCheckStyleStr;}	//!< ���У�������б�
	std::list<CString> getDataBitNumList(){return m_ltDataBitNumStr;}	//!< �������λ�б�
	std::list<CString> getStopBitNumList(){return m_ltStopBitNumStr;}	//!< ���ֹͣλ�б�

public:
	const CString getVersionStr(UINT type = 0);						//!< �������汾��,type��ʾ�汾���ͣ�0ȫ����1�߰汾�ţ�2�а汾�ţ�3�Ͱ汾��
	const CString getSVNStr();										//!< ���SVN��,�����Ҫ�Լ���һ��
	const CString getLanguageText(UINT id);
	const CString getExpText(UINT id);

	static void SetProgressRange(int nEnd = 1000);
	static void SetProgressStep(int step);
	static void SetProgressEnd();

	CString getBaudStr(int index);									//!< ��ò�����
	CString getCheckStyleStr(int index);							//!< ���У������
	CString getDataBitNumStr(int index);							//!< �������λ
	CString getStopBitNumStr(int index);							//!< ���ֹͣλ
	UINT getBaudIndex(CString str);									//!< ��ò���������
	UINT getCheckIndex(CString str);								//!< ���У����������
	UINT getDataIndex(CString str);									//!< �������λ����
	UINT getStopIndex(CString str);									//!< ���ֹͣλ����

	static void PrintClear();												//!< ��������
	static void PrintOut(CString text,bool fresh=false,bool twice=true);	//!< ����ı������һ��������ʾ�Ƿ��������2����ͬ����ʾ��true��ʾ����
	static void PrintLock(bool bLock);									//!< ��ס���������ˢ��

	//!< ������ȫ�ַ���ĺ���
	static CString GetMssageFHResult(HRESULT hr);							//!< ͨ��HRESULT��ý�����ı�˵��
	static bool SetSystemTimeFromStr(SYSTEMTIME& st,CString text);			//!< �����ı������ϵͳʱ��
	static CString GetSystemTime2Str(const SYSTEMTIME& st);				//!< ����ϵͳʱ�䣬����ı�
	//!< ���ַ���text��rule�ַ���������һ���ַ�Ϊ�ָ��������룬�����������strList��
	static void SpliteBy(CString text,CString rule,std::vector<CString>& vtStr);
	//!< �����ִ��ַ����з����ȥ������type = 0 ��ʾ������ȡ�� = 1 ��ʾ������ȡ
	static bool GetNumberFromString(CString& str, int& number, int type=0);
	//!< �����������ַ����з����ȥ������type = 0 ��ʾ������ȡ�� = 1 ��ʾ������ȡ
	static void GetFloatFromString(CString& str, CString& integer, CString& decimal, UINT type=0);
	static bool GetCorrectFileName(CString& str, bool showError=false);	//!< �ж��ļ����Ƿ�Ϸ������غϷ����ļ���
	static CString StringFormat(CString str,int n);						//!< ����������ʾʡ�Ժ�,���ֺ��ָ���ĸ
	static CString GetPathFromFilePathName(CString pathName);				//!< ���ļ���ȫ·�����У�����ļ���·��
	static CString GetNameFromFilePathName(CString pathName);				//!< ���ļ���ȫ·�����У�����ļ�������
	static void GetSerialCount(std::list<UINT>& ltComID);					//!< ��ô�����Ϣ
	static CString GetIPFromNumber(UINT number);							//!< ���������͵�IP��ַ����ַ���ʽ��IP��ַ
	static UINT GetNumberFromIP(CString ip);								//!< ���ַ���ʽ��IP��ַ����������͵�IP��ַ
	static UINT IPAddNumber(UINT ip, UINT number);							//!< ��IP����һ�����֣�����ʾѭ���ϼӣ�����1.1.1.1 + 1 = 1.1.1.2
	static CString GetHostIPString();										//!< ��ñ�����IP��ַ
	static bool MakeDir(CString path);										//!< �������Ŀ¼
	static bool RemoveFiles(CString path, CString name);					//!< ɾ��path�ļ�������������Ϊname���ļ�
	static bool CopyDirectoryAll(CString csSrcFolder, CString csNewFolder);//!< ��������Ŀ¼(������Ŀ¼�������ڲ��ļ�)��ָ��Ŀ¼��,Ŀ¼�����'\'
	static CString GetCulTime(bool bMillSec = true);						//!< ��õ�ǰʱ����ַ�����������ʾ�Ƿ���Ҫ��ʾ����
	static bool ChangeVariantType(CComVariant& cvr, UINT vt);				//!< ǿ�н�һ��CComVariant�ı���ת����vt
	static LONGLONG VariantCompare(CComVariant cvr1, CComVariant cvr2);	//!< �Ƚϴ�С������󷵻�������С���ظ��������ڷ���0
	static bool SearchT(CComVariant cvr, CString strMatch, bool bWhole, bool bCase, bool bRegex, CString strType, CString& strOut);	//!< ����ƥ���ģ�庯��
	std::wstring Change2WString(std::string str);
	std::string Change2String(std::wstring wstr);
	bool RegexMatch(const std::string& strRegex, std::string& strMatch, std::vector<std::string>& vt);	//!< ƥ��������ʽ����������ƥ���ķ�������
	bool RegexMatch(const std::string& strRegex, std::string& strMatch);		//!< ƥ��������ʽ����
	static CString GetCPUID();												//!< ���CPU��
	static CString GetSpecialPath(int id);									//!< ��ò���ϵͳ�ؼ�·��, ��\, ��������ϵͳ���� CSIDL_ ��ͷ, ��CSIDL_PERSONAL �ҵ��ĵ�
	static bool ReadFile2Binary(CString pathName, char** data, int& len);	//!< ������ļ��л�ȡ����������
	static bool WriteBinary2File(CString pathName, char* data, int len);	//!< ������������д���ļ���
	static CString UintToCString(UINT numbe);			//!<uintת����cstring
	static UINT DomainToIp(CString domain,int &ip);				//!<����ת����IP
	static bool Regexmatch(CString strRegex,CString str);  //!<�ַ����Ƿ����������ʽ 
	static bool RegexDomain(CString domain);			//!<�ַ����Ƿ�������
	static bool RegexIP(CString ip);								//!<�ַ����Ƿ���IP
	static bool StartHighTime();						//!< �����߾��ȶ�ʱ��,����ͳ�Ƴ���Ч��
	static double GetHighTime(bool bReStart = false);	//!< ��ö�ʱ����ֵ,�������Ƿ����¼�ʱ
	bool IsNumber(CString str);					//!<�Ƿ��������ַ���
//static void DNSError(int i_error);							//!<��������ʧ�ܴ������
};

