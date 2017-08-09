#pragma once

/*!
	全局的基础数据服务类，单件
*/
class CGbl
{
public:
	enum EnumUndo{
		UNDO_TYPE_ADD = 1,			//!< 添加
		UNDO_TYPE_DEL = 2,			//!< 删除
		UNDO_TYPE_UPD = 3,			//!< 修改
		UNDO_INFO_EXCHANGE = 1,		//!< 交换
	};
	enum ReportColor{
		REPORT_RED = 1,				//!< 红色
		REPORT_GREEN = 2,			//!< 绿色
		REPORT_BLUE = 3,			//!< 蓝色
		REPORT_YELLOW = 4,			//!< 黄色
		REPORT_PURPLE = 5,			//!< 紫色
		REPORT_WATERBLUE = 6,		//!< 水蓝色/青色
		REPORT_WHITE = 7,			//!< 白色
	};
private:
	CGbl(void);
	~CGbl(void);

	//!< 路径的后边都带有‘\’
	CString m_strInstallPath;		//!< 安装目录
	CString m_strExePath;			//!< 可执行程序路径
	CString m_strSlnPath;			//!< 解决方案默认路径
	CString m_strDataPath;			//!< 主描述文件路径
	CString m_strDocPath;			//!< 文档文件路径
	CString m_strProjDefPath;		//!< 工程默认路径

	//!< 软件权限相关

public:
	static CGbl& GetMe();
	const CString getExePath(){return m_strExePath;}				//!< 获得可执行程序路径
	const CString getInstallPath(){return m_strInstallPath;}		//!< 获得安装根目录
	const CString getSolutionPath(){return m_strSlnPath;}			//!< 获得解决方案目录
	const CString getDataPath(){return m_strDataPath;}				//!< 获得data文件夹的目录
	const CString getDocPath(){return m_strDocPath;}				//!< 获得文档文件的目录
	const CString getProjDefPath(){return m_strProjDefPath;}		//!< 获得工程默认路径

public:
	static const CString getVersionStr(UINT type = 0);						//!< 获得软件版本号,type表示版本类型，0全部，1高版本号，2中版本号，3低版本号

	//!< 下面是全局服务的函数
	static CString GetMssageFHResult(HRESULT hr);							//!< 通过HRESULT获得结果的文本说明
	static bool SetSystemTimeFromStr(SYSTEMTIME& st,CString text);			//!< 解析文本，获得系统时间
	static CString GetSystemTime2Str(const SYSTEMTIME& st);					//!< 解析系统时间，获得文本
	//!< 将字符串text以rule字符串中任意一个字符为分隔条件分离，并将结果赋到strList中
	static void SpliteBy(CString text,CString rule,std::vector<CString>& vtStr);
	static bool GetCorrectFileName(CString& str, bool showError=false);		//!< 判断文件名是否合法，返回合法的文件名
	static CString StringFormat(CString str,int n);							//!< 超出部分显示省略号,区分汉字跟字母
	static CString GetPathFromFilePathName(CString pathName);				//!< 从文件的全路径名中，获得文件的路径
	static CString GetNameFromFilePathName(CString pathName);				//!< 从文件的全路径名中，获得文件的名称
// 	static void GetSerialCount(std::list<UINT>& ltComID);					//!< 获得串口信息
	static CString GetIPFromNumber(UINT number);							//!< 由数字类型的IP地址获得字符串式的IP地址
	static bool GetNumberFromIP(CString ip, UINT& uiIP);					//!< 有字符串式的IP地址获得数据类型的IP地址
	static UINT IPAddNumber(UINT ip, UINT number);							//!< 给IP加上一个数字，在显示循序上加，比如1.1.1.1 + 1 = 1.1.1.2
	static CString GetHostIPString();										//!< 获得本机的IP地址
	static bool MakeDir(CString path);										//!< 创建多层目录
	static bool CopyDirectoryAll(CString csSrcFolder, CString csNewFolder);//!< 复制整个目录(及其子目录和所有内部文件)到指定目录下,目录最后都有'\'
	static CString GetCulTime(bool bMillSec = true);						//!< 获得当前时间的字符串，参数表示是否需要显示毫秒
	static bool ChangeVariantType(CComVariant& cvr, UINT vt);				//!< 强行将一个CComVariant的变量转换成vt
	static LONGLONG VariantCompare(CComVariant cvr1, CComVariant cvr2);	//!< 比较大小，如果大返回整数，小返回负数，等于返回0
	static void ShowHelp(int id = -1);										//!< 打开帮助文档
	static void InitReport();												//!< 显示输出栏
	static void DebugMsg(CString strMsg, WORD color, bool bIntensity);		//!< 打印信息
	static void DebugMsg(CString strMsg, byte* data, int len, WORD color, bool bIntensity);	//!< 打印信息
	static short CRC16(unsigned char* puchMsg, unsigned short usDataLen);	//!< 为数据做CRC校验
};
