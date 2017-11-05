#pragma once

namespace MVC{
namespace Item{
class CItem;
class CPropertySource
{
private:
	CItem* m_pParent;				// �����ı���
	CString m_strScriptText;		// ��ֵ�ű��ı�

	UINT m_uiIOType;				// IO���ͣ��������ݿ������ļ�
	UINT m_uiFreshTime;				// ����ˢ��ʱ��,��λ1����
	UINT m_uiConvertType;			// ת�����ͣ�0��ת����1�Ƿ�ȡ��(λ����ʹ��)��1����ת��
	CComVariant m_MinProjectValue;	// ��С����ֵ
	CComVariant m_MaxProjectValue;	// ��󹤳�ֵ
	CComVariant m_MinIOValue;		// ��СIOֵ
	CComVariant m_MaxIOValue;		// ���IOֵ

public:
	void setScriptText(CString text){m_strScriptText = text;}
	void setFreshTime(UINT fresh_time){if(fresh_time < 10) fresh_time = 10;	m_uiFreshTime = fresh_time;}
	void setIOType(UINT type){m_uiIOType = type;}
	void setConvertType(UINT type){m_uiConvertType = type; if(m_pParent->getValType() == 0)	m_uiConvertType = 0;}
	void setProjMax(CComVariant cvr){m_MaxProjectValue = cvr;}
	void setProjMin(CComVariant cvr){m_MinProjectValue = cvr;}
	void setIOMax(CComVariant cvr){m_MaxIOValue = cvr;}
	void setIOMin(CComVariant cvr){m_MinIOValue = cvr;}

	CItem* getItem(){return m_pParent;}
	CString getScriptText(){return m_strScriptText;}
	UINT getFreshTime(){return m_uiFreshTime;}
	UINT getIOType(){return m_uiIOType;}
	UINT getConvertType(){return m_uiConvertType;}
	CComVariant getProjMax(){return m_MaxProjectValue;}
	CComVariant getProjMin(){return m_MinProjectValue;}
	CComVariant getIOMax(){return m_MaxIOValue;}
	CComVariant getIOMin(){return m_MinIOValue;}

private:
	int GetTypeFromStr(CString strType);				// �������ƻ������

public:
	CPropertySource(CItem* item);
	~CPropertySource(void);

	bool Serialize(CArchive& ar);						// �����������Ϣ
	bool SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport = false);
	void ReadFromPMExcel(std::vector<CString>& vtCell);					// ��������Դ����

	CPropertySource& operator = (CPropertySource& src);
	bool operator == (CPropertySource& src);
};
}
}