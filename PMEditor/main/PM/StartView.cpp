// StartPageView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "Gbl.h"
#include "DCCE_HtmlDialog.h"
#include "StartView.h"


// CStartView
using namespace MVC;
using namespace Start;
IMPLEMENT_DYNCREATE(CStartView, CScrollView)

CStartView::CStartView()
:m_isShowHomePage(FALSE)
{
}

CStartView::~CStartView()
{
}


BEGIN_MESSAGE_MAP(CStartView, CScrollView)
	ON_WM_SIZE()
	ON_COMMAND(ID_TB_PAGEBACK, OnTbPageback)
	ON_COMMAND(ID_TB_PAGEFRONT, OnTbPagefront)
	ON_COMMAND(ID_TB_PAGEHOME, OnTbPagehome)
	ON_UPDATE_COMMAND_UI(ID_TB_PAGEBACK, OnUpdateTbPageback)
	ON_UPDATE_COMMAND_UI(ID_TB_PAGEFRONT, OnUpdateTbPagefront)
	ON_UPDATE_COMMAND_UI(ID_TB_PAGEHOME, OnUpdateTbPagehome)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CStartView ��ͼ

void CStartView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;

	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	//��ʼ��IE����
	m_HtmlDlg = boost::shared_ptr<CDCCE_HtmlDialog>(new CDCCE_HtmlDialog(this));
	CRect rect;
	GetClientRect(&rect);
	if(!m_HtmlDlg->Create(IDD_DCCE_HTMLDIALOG,this))
	{
		CGbl::PrintOut(_T("��ʼ��IE����ʧ��"));
		return;
	}
	m_HtmlDlg->ShowWindow(SW_SHOW);
	m_HtmlDlg->UpdateWindow();
	m_HtmlDlg->SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);

	//�����ʼҳ
	m_HtmlDlg->m_HomeRUL = CGbl::GetMe().getDataPath();
	m_HtmlDlg->m_HomeRUL += _T("StartHtml\\html\\DCCE_HtmlDialog.html");
	m_HtmlDlg->Navigate(LPCTSTR(m_HtmlDlg->m_HomeRUL));
}

void CStartView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}


// CStartView ���

#ifdef _DEBUG
void CStartView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CStartView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CStartView ��Ϣ�������

void CStartView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	if(m_HtmlDlg==NULL || m_HtmlDlg->m_hWnd==NULL)
		return;
	CRect rect;
	GetClientRect(&rect);
	m_HtmlDlg->UpdateWindow();
	m_HtmlDlg->SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
	// TODO: �ڴ˴������Ϣ����������
}

void CStartView::OnTbPageback()
{
	// TODO: �ڴ���������������
	m_HtmlDlg->GoBack();
}

void CStartView::OnTbPagefront()
{
	// TODO: �ڴ���������������
	m_HtmlDlg->GoForward();
}

void CStartView::OnTbPagehome()
{
	// TODO: �ڴ���������������
	m_HtmlDlg->GoHome();
}

void CStartView::OnUpdateTbPageback(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(!m_HtmlDlg->m_BackStack.empty());
}

void CStartView::OnUpdateTbPagefront(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(!m_HtmlDlg->m_FrontStack.empty());
}

void CStartView::OnUpdateTbPagehome(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_HtmlDlg->m_LocalURL.MakeUpper()==m_HtmlDlg->m_HomeRUL.MakeUpper())
	{
		pCmdUI->Enable(FALSE);
		if(!m_isShowHomePage){
			CString filePath,fileName,htmlText,str;
			CRecentFileList *pList = g_App.GetRecentFileList();
			int count = pList->GetSize();
			if(count > 6)	count = 6;
			for(int i = 0; i < count; i++)
			{
				fileName = filePath = pList->m_arrNames[i];
				if(filePath == _T(""))			continue;
				htmlText = htmlText + _T("<tr><td><a id=\"ProjRULList\" href=\"#\" hrefs=\"") +
					filePath + _T("\">&nbsp;") + fileName + _T("</a><br></td></tr>");
			}
//			htmlText = _T("<tr><td align=\"center\" bgColor=\"#deeff0\"><asp:label id=\"LblDateHeader\"
			htmlText = _T("<tr><td align=\"center\" ><asp:label id=\"LblDateHeader\" \
					 runat=\"server\" Width=\"64px\"><FONT face=\"΢���ź�\">������</FONT></asp:label><br></td></tr>") + htmlText;
			htmlText = _T("<FONT face=\"�����п�\" size=\"5px\" color=\'#0000ff\'>") + htmlText + _T("</FONT>");	//�޸�����
			htmlText = _T("<TABLE id=\"Table1\" left=\"10px\" cellSpacing=\"1\" cellPadding=\"4\" border=\"1\" \
					 borderColor=\"#ccccff\" borderColorLight=\"#000066\" width=\"80%\"") + htmlText + _T("</table>");//���ر��
			htmlText = _T("<FONT face=\"΢���ź�\" size=\"4px\" color=\'#303030\'>����򿪹��̣�</FONT>") + htmlText;
			m_HtmlDlg->SetHomeProjItem(htmlText);
			m_isShowHomePage = TRUE;
		}
	}
	else{
		pCmdUI->Enable(TRUE);
		m_isShowHomePage = FALSE;
	}
}

BOOL CStartView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(pMsg->message){
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			return TRUE;
		default:
			return CScrollView::PreTranslateMessage(pMsg);
	}
	return CScrollView::PreTranslateMessage(pMsg);
}

void CStartView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	CXTMemDC MemDC(&dc, rect);
	OnDraw(&dc);
}
