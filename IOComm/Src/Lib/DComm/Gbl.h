#pragma once

/*!
	ȫ�ֵĻ������ݷ����࣬����
*/
class CGbl
{
public:
	enum EnumUndo{
		UNDO_TYPE_ADD = 1,			//!< ���
		UNDO_TYPE_DEL = 2,			//!< ɾ��
		UNDO_TYPE_UPD = 3,			//!< �޸�
		UNDO_INFO_EXCHANGE = 1,		//!< ����
	};
	enum ReportColor{
		REPORT_RED = 1,				//!< ��ɫ
		REPORT_GREEN = 2,			//!< ��ɫ
		REPORT_BLUE = 3,			//!< ��ɫ
		REPORT_YELLOW = 4,			//!< ��ɫ
		REPORT_PURPLE = 5,			//!< ��ɫ
		REPORT_WATERBLUE = 6,		//!< ˮ��ɫ/��ɫ
		REPORT_WHITE = 7,			//!< ��ɫ
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
	CString m_strProjDefPath;		//!< ����Ĭ��·��

	//!< ���Ȩ�����

public:
	static CGbl& GetMe();
	const CString getExePath(){return m_strExePath;}				//!< ��ÿ�ִ�г���·��
	const CString getInstallPath(){return m_strInstallPath;}		//!< ��ð�װ��Ŀ¼
	const CString getSolutionPath(){return m_strSlnPath;}			//!< ��ý������Ŀ¼
	const CString getDataPath(){return m_strDataPath;}				//!< ���data�ļ��е�Ŀ¼
	const CString getDocPath(){return m_strDocPath;}				//!< ����ĵ��ļ���Ŀ¼
	const CString getProjDefPath(){return m_strProjDefPath;}		//!< ��ù���Ĭ��·��

public:
	static const CString getVersionStr(UINT type = 0);						//!< �������汾��,type��ʾ�汾���ͣ�0ȫ����1�߰汾�ţ�2�а汾�ţ�3�Ͱ汾��

	//!< ������ȫ�ַ���ĺ���
	static CString GetMssageFHResult(HRESULT hr);							//!< ͨ��HRESULT��ý�����ı�˵��
	static bool SetSystemTimeFromStr(SYSTEMTIME& st,CString text);			//!< �����ı������ϵͳʱ��
	static CString GetSystemTime2Str(const SYSTEMTIME& st);					//!< ����ϵͳʱ�䣬����ı�
	//!< ���ַ���text��rule�ַ���������һ���ַ�Ϊ�ָ��������룬�����������strList��
	static void SpliteBy(CString text,CString rule,std::vector<CString>& vtStr);
	static bool GetCorrectFileName(CString& str, bool showError=false);		//!< �ж��ļ����Ƿ�Ϸ������غϷ����ļ���
	static CString StringFormat(CString str,int n);							//!< ����������ʾʡ�Ժ�,���ֺ��ָ���ĸ
	static CString GetPathFromFilePathName(CString pathName);				//!< ���ļ���ȫ·�����У�����ļ���·��
	static CString GetNameFromFilePathName(CString pathName);				//!< ���ļ���ȫ·�����У�����ļ�������
// 	static void GetSerialCount(std::list<UINT>& ltComID);					//!< ��ô�����Ϣ
	static CString GetIPFromNumber(UINT number);							//!< ���������͵�IP��ַ����ַ���ʽ��IP��ַ
	static bool GetNumberFromIP(CString ip, UINT& uiIP);					//!< ���ַ���ʽ��IP��ַ����������͵�IP��ַ
	static UINT IPAddNumber(UINT ip, UINT number);							//!< ��IP����һ�����֣�����ʾѭ���ϼӣ�����1.1.1.1 + 1 = 1.1.1.2
	static CString GetHostIPString();										//!< ��ñ�����IP��ַ
	static bool MakeDir(CString path);										//!< �������Ŀ¼
	static bool CopyDirectoryAll(CString csSrcFolder, CString csNewFolder);//!< ��������Ŀ¼(������Ŀ¼�������ڲ��ļ�)��ָ��Ŀ¼��,Ŀ¼�����'\'
	static CString GetCulTime(bool bMillSec = true);						//!< ��õ�ǰʱ����ַ�����������ʾ�Ƿ���Ҫ��ʾ����
	static bool ChangeVariantType(CComVariant& cvr, UINT vt);				//!< ǿ�н�һ��CComVariant�ı���ת����vt
	static LONGLONG VariantCompare(CComVariant cvr1, CComVariant cvr2);	//!< �Ƚϴ�С������󷵻�������С���ظ��������ڷ���0
	static void ShowHelp(int id = -1);										//!< �򿪰����ĵ�
	static void InitReport();												//!< ��ʾ�����
	static void DebugMsg(CString strMsg, WORD color, bool bIntensity);		//!< ��ӡ��Ϣ
	static void DebugMsg(CString strMsg, byte* data, int len, WORD color, bool bIntensity);	//!< ��ӡ��Ϣ
	static short CRC16(unsigned char* puchMsg, unsigned short usDataLen);	//!< Ϊ������CRCУ��
};
