#pragma once

namespace MVC{
namespace Item{
class CScriptItem
{
private:
	CString m_strScriptText;	//!< 赋值脚本的内容，没有函数头和尾
	UINT m_uiFreshTime;			//!< 赋值脚本的刷新时间
	UINT m_uiID;				//!< 脚本的编号

public:
	virtual UINT getFreshTime(){return m_uiFreshTime;}
	UINT getID(){return m_uiID;}

public:
	CScriptItem(UINT id,UINT freshtime);
	~CScriptItem(void);

	bool Serialize(CArchive& ar);						//!< 保存二进制信息

	virtual void AddItemScriptTextVB(CString text);		//!< 追加一个变量的赋值脚本
	virtual CString GetScriptContentVB();				//!< 获得整理后的脚本函数，包括函数头和尾
	virtual CString GetScriptHeadVB();					//!< 获得整理后的脚本函数的函数名
};
}
}