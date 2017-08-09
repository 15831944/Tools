#include "stdafx.h"
#include "ScriptItem.h"

using namespace MVC;
using namespace Item;
CScriptItem::CScriptItem(UINT id, UINT freshtime)
{
	if(freshtime < 50)	freshtime = 50;
	m_uiID = id;
	m_uiFreshTime = freshtime;
}

CScriptItem::~CScriptItem(void)
{
}

void CScriptItem::AddItemScriptTextVB(CString text)
{
	m_strScriptText = m_strScriptText + text + _T("\r\n");
}

CString CScriptItem::GetScriptContentVB()
{
	return GetScriptHeadVB() + _T("\r\n") + m_strScriptText + _T("End Function\r\n");
}

CString CScriptItem::GetScriptHeadVB()
{
	CString _title;
	_title.Format("Public Function Script%d()", m_uiFreshTime);
	return _title;
}
