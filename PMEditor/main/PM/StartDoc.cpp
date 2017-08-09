// StartPageDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "StartDoc.h"

// CStartDoc
using namespace MVC;
using namespace Start;
IMPLEMENT_DYNCREATE(CStartDoc, CDocument)

CStartDoc::CStartDoc()
{
	int i=1;
}

BOOL CStartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	SetTitle("起始页");
	return TRUE;
}

CStartDoc::~CStartDoc()
{
}


BEGIN_MESSAGE_MAP(CStartDoc, CDocument)
END_MESSAGE_MAP()


// CStartDoc 诊断

#ifdef _DEBUG
void CStartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStartDoc 序列化

// CStartDoc 命令

BOOL CStartDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(nID==59392 || nID==2 || nID==3 || nID==133 || nID==134 || nID==59137 || nID==59138 || nID==59139 || 
		nID==32833 || nID==32834 || nID==103 || nID==104 || nID==57664 || nID==59396 || nID==59648 || nID==32886)
		return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	else
		return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
