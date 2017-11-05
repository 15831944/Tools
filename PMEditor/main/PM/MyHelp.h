#pragma once

namespace SoftInfo{
class CMyHelp
{
private:
	CString m_strHelpPath;
	CMyHelp(void);
	~CMyHelp(void);
	CString GetHelpPath(){return m_strHelpPath;}			// 获得Help文件地址

	CString GetHelpUrl(CString strTitle);					// 根据索引名获得帮助文档的名称
	void SerializeXml(TiXmlElement* pNode);					// 加载或保存配置

public:
	static CMyHelp& GetMe(){static CMyHelp one; return one;}
	void InitInfo();										// 初始化数据
	void ShowHelp(CString strTitle);						// 显示帮助信息
};
}