#pragma once

/*!
	全局的基础数据服务类，单件
*/
class CGbl
{
public:
	enum EnumUndo{
		UNDO_TYPE_ADD = 1,					//!< 添加
		UNDO_TYPE_DEL = 2,					//!< 删除
		UNDO_TYPE_UPD = 3,					//!< 修改

		UNDO_INFO_EXCHANGE = 1,				//!< 交换
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
	CString m_strLanguagePath;		//!< 语言包路径
	CString m_strProjDefPath;		//!< 工程默认路径

	CString m_strComplierName;		//!< 编译器的文件名，包括扩展名，但不包括路径
	CString m_strServerName;		//!< 服务器的文件名，包括扩展名，但不包括路径
	CString m_strHmiName;			//!< HMI的文件名，包括扩展名，但不包括路径
	CString m_strConfigName;		//!< Config的文件名，包括扩展名，但不包括路径

	//!< 接口通用参数
	std::list<CString> m_ltBaudStr;
	std::list<CString> m_ltCheckStyleStr;
	std::list<CString> m_ltDataBitNumStr;
	std::list<CString> m_ltStopBitNumStr;

public:
	static CGbl& GetMe();
	const CString getExePath(){return m_strExePath;}				//!< 获得可执行程序路径
	const CString getInstallPath(){return m_strInstallPath;}		//!< 获得安装根目录
	const CString getSolutionPath(){return m_strSlnPath;}			//!< 获得解决方案目录
	const CString getDataPath(){return m_strDataPath;}				//!< 获得data文件夹的目录
	const CString getDocPath(){return m_strDocPath;}				//!< 获得文档文件的目录
	const CString getLanguagePath(){return m_strLanguagePath;}		//!< 获得语言包的目录
	const CString getProjDefPath(){return m_strProjDefPath;}		//!< 获得工程默认路径
	const CString getCompilerName(){return m_strComplierName;}		//!< 获得编译器的名字
	const CString getServerName(){return m_strServerName;}			//!< 获得底层服务的名字
	const CString getHmiName(){return m_strHmiName;}				//!< 获得HMI程序的名字
	const CString getConfigName(){return m_strConfigName;}			//!< 获得Config程序的名字

	std::list<CString> getBaudList(){return m_ltBaudStr;}				//!< 获得波特率列表
	std::list<CString> getCheckStyleList(){return m_ltCheckStyleStr;}	//!< 获得校验类型列表
	std::list<CString> getDataBitNumList(){return m_ltDataBitNumStr;}	//!< 获得数据位列表
	std::list<CString> getStopBitNumList(){return m_ltStopBitNumStr;}	//!< 获得停止位列表

public:
	const CString getVersionStr(UINT type = 0);						//!< 获得软件版本号,type表示版本类型，0全部，1高版本号，2中版本号，3低版本号
	const CString getSVNStr();										//!< 获得SVN号,这个需要自己算一下
	const CString getLanguageText(UINT id);
	const CString getExpText(UINT id);

	static void SetProgressRange(int nEnd = 1000);
	static void SetProgressStep(int step);
	static void SetProgressEnd();

	CString getBaudStr(int index);									//!< 获得波特率
	CString getCheckStyleStr(int index);							//!< 获得校验类型
	CString getDataBitNumStr(int index);							//!< 获得数据位
	CString getStopBitNumStr(int index);							//!< 获得停止位
	UINT getBaudIndex(CString str);									//!< 获得波特率索引
	UINT getCheckIndex(CString str);								//!< 获得校验类型索引
	UINT getDataIndex(CString str);									//!< 获得数据位索引
	UINT getStopIndex(CString str);									//!< 获得停止位索引

	static void PrintClear();												//!< 清空输出栏
	static void PrintOut(CString text,bool fresh=false,bool twice=true);	//!< 输出文本，最后一个参数表示是否允许出现2次相同的提示，true表示允许
	static void PrintLock(bool bLock);									//!< 锁住不让输出框刷新

	//!< 下面是全局服务的函数
	static CString GetMssageFHResult(HRESULT hr);							//!< 通过HRESULT获得结果的文本说明
	static bool SetSystemTimeFromStr(SYSTEMTIME& st,CString text);			//!< 解析文本，获得系统时间
	static CString GetSystemTime2Str(const SYSTEMTIME& st);				//!< 解析系统时间，获得文本
	//!< 将字符串text以rule字符串中任意一个字符为分隔条件分离，并将结果赋到strList中
	static void SpliteBy(CString text,CString rule,std::vector<CString>& vtStr);
	//!< 将数字从字符串中分离出去，参数type = 0 表示从左提取； = 1 表示从右提取
	static bool GetNumberFromString(CString& str, int& number, int type=0);
	//!< 将浮点数从字符串中分离出去，参数type = 0 表示从左提取； = 1 表示从右提取
	static void GetFloatFromString(CString& str, CString& integer, CString& decimal, UINT type=0);
	static bool GetCorrectFileName(CString& str, bool showError=false);	//!< 判断文件名是否合法，返回合法的文件名
	static CString StringFormat(CString str,int n);						//!< 超出部分显示省略号,区分汉字跟字母
	static CString GetPathFromFilePathName(CString pathName);				//!< 从文件的全路径名中，获得文件的路径
	static CString GetNameFromFilePathName(CString pathName);				//!< 从文件的全路径名中，获得文件的名称
	static void GetSerialCount(std::list<UINT>& ltComID);					//!< 获得串口信息
	static CString GetIPFromNumber(UINT number);							//!< 由数字类型的IP地址获得字符串式的IP地址
	static UINT GetNumberFromIP(CString ip);								//!< 有字符串式的IP地址获得数据类型的IP地址
	static UINT IPAddNumber(UINT ip, UINT number);							//!< 给IP加上一个数字，在显示循序上加，比如1.1.1.1 + 1 = 1.1.1.2
	static CString GetHostIPString();										//!< 获得本机的IP地址
	static bool MakeDir(CString path);										//!< 创建多层目录
	static bool RemoveFiles(CString path, CString name);					//!< 删除path文件夹下所有名称为name的文件
	static bool CopyDirectoryAll(CString csSrcFolder, CString csNewFolder);//!< 复制整个目录(及其子目录和所有内部文件)到指定目录下,目录最后都有'\'
	static CString GetCulTime(bool bMillSec = true);						//!< 获得当前时间的字符串，参数表示是否需要显示毫秒
	static bool ChangeVariantType(CComVariant& cvr, UINT vt);				//!< 强行将一个CComVariant的变量转换成vt
	static LONGLONG VariantCompare(CComVariant cvr1, CComVariant cvr2);	//!< 比较大小，如果大返回整数，小返回负数，等于返回0
	static bool SearchT(CComVariant cvr, CString strMatch, bool bWhole, bool bCase, bool bRegex, CString strType, CString& strOut);	//!< 进行匹配的模板函数
	std::wstring Change2WString(std::string str);
	std::string Change2String(std::wstring wstr);
	bool RegexMatch(const std::string& strRegex, std::string& strMatch, std::vector<std::string>& vt);	//!< 匹配正则表达式函数，返回匹配后的分组数据
	bool RegexMatch(const std::string& strRegex, std::string& strMatch);		//!< 匹配正则表达式函数
	static CString GetCPUID();												//!< 获得CPU号
	static CString GetSpecialPath(int id);									//!< 获得操作系统关键路径, 带\, 参数都是系统的以 CSIDL_ 开头, 如CSIDL_PERSONAL 我的文档
	static bool ReadFile2Binary(CString pathName, char** data, int& len);	//!< 从这个文件中获取二进制数据
	static bool WriteBinary2File(CString pathName, char* data, int len);	//!< 将二进制数据写入文件中
	static CString UintToCString(UINT numbe);			//!<uint转换成cstring
	static UINT DomainToIp(CString domain,int &ip);				//!<域名转换成IP
	static bool Regexmatch(CString strRegex,CString str);  //!<字符串是否符合正则表达式 
	static bool RegexDomain(CString domain);			//!<字符串是否是域名
	static bool RegexIP(CString ip);								//!<字符串是否是IP
	static bool StartHighTime();						//!< 启动高精度定时器,用于统计程序效率
	static double GetHighTime(bool bReStart = false);	//!< 获得定时器差值,并设置是否重新计时
	bool IsNumber(CString str);					//!<是否是数字字符串
//static void DNSError(int i_error);							//!<域名解析失败错误分析
};

