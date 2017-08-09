#pragma once


// CStartDoc ÎÄµµ
namespace MVC{namespace Start{
class CStartDoc : public CDocument
{
	DECLARE_DYNCREATE(CStartDoc)

public:
	CStartDoc();
	virtual ~CStartDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
}}