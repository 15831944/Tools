#pragma once

namespace MVC{
namespace Item{
class CScriptItem
{
private:
	CString m_strScriptText;	//!< ��ֵ�ű������ݣ�û�к���ͷ��β
	UINT m_uiFreshTime;			//!< ��ֵ�ű���ˢ��ʱ��
	UINT m_uiID;				//!< �ű��ı��

public:
	virtual UINT getFreshTime(){return m_uiFreshTime;}
	UINT getID(){return m_uiID;}

public:
	CScriptItem(UINT id,UINT freshtime);
	~CScriptItem(void);

	bool Serialize(CArchive& ar);						//!< �����������Ϣ

	virtual void AddItemScriptTextVB(CString text);		//!< ׷��һ�������ĸ�ֵ�ű�
	virtual CString GetScriptContentVB();				//!< ��������Ľű���������������ͷ��β
	virtual CString GetScriptHeadVB();					//!< ��������Ľű������ĺ�����
};
}
}