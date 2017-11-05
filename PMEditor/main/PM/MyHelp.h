#pragma once

namespace SoftInfo{
class CMyHelp
{
private:
	CString m_strHelpPath;
	CMyHelp(void);
	~CMyHelp(void);
	CString GetHelpPath(){return m_strHelpPath;}			// ���Help�ļ���ַ

	CString GetHelpUrl(CString strTitle);					// ������������ð����ĵ�������
	void SerializeXml(TiXmlElement* pNode);					// ���ػ򱣴�����

public:
	static CMyHelp& GetMe(){static CMyHelp one; return one;}
	void InitInfo();										// ��ʼ������
	void ShowHelp(CString strTitle);						// ��ʾ������Ϣ
};
}