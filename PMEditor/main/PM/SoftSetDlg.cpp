// SoftSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "SoftInfo.h"
#include "SoftSetDlg.h"

#include "SoftSetItemGridDlg.h"
#include "SoftSetTuoPuDlg.h"
#include "SoftSetCompileDlg.h"
#include "SoftSetServerDlg.h"
#include "SoftSetDeviceXmlDlg.h"


// CSoftSetDlg �Ի���
using namespace SoftInfo;
IMPLEMENT_DYNAMIC(CSoftSetDlg, CDialog)

CSoftSetDlg::CSoftSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftSetDlg::IDD, pParent)
{

}

CSoftSetDlg::~CSoftSetDlg()
{
}

void CSoftSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PAGE, m_PageTree);
}

BOOL CSoftSetDlg::DestroyWindow()
{
	for(int i = 0; i < (int)m_vtPage.size(); ++i)
	{
		ASSERT(m_vtPage[i]);
		ASSERT(m_vtPage[i]->m_Dialog);
		if(m_vtPage[i]->m_Dialog->GetSafeHwnd()){
			m_vtPage[i]->m_Dialog->DestroyWindow();
		}
	}
	m_vtPage.clear();

	return CDialog::DestroyWindow();
}


BEGIN_MESSAGE_MAP(CSoftSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSoftSetDlg::OnBnClickedOk)
	ON_MESSAGE(MESSAGE_OK, &CSoftSetDlg::OnRevMsgOK)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CSoftSetDlg ��Ϣ�������
BOOL CSoftSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_PageTree.InitImg();
	m_PageTree.SetOwner(this);

	//!< �����ҳ
	CSoftSetItemGridDlg* page1 = NULL;
	AddPage(page1, _T("������"), 4);
	CSoftSetTuoPuDlg* page2 = NULL;
	AddPage(page2, _T("����ͼ"), 17);
	CSoftSetCompileDlg* page3 = NULL;
	AddPage(page3, _T("������ɨ��"), 18);
	CSoftSetServerDlg* page4 = NULL;
	AddPage(page4, _T("��������"), 23);
	CSoftSetDeviceXmlDlg* page5 = NULL;
	AddPage(page5, _T("֧�ֵ��豸"), 13);

	//!< ��ʾ��һ��
	if(!m_vtPage.empty())
		OnTreeLClick(&m_PageTree, m_PageTree.GetRootItem());
		//ShowPage(m_vtPage[0]);
	return TRUE;
}

template<class Page>
void CSoftSetDlg::AddPage(Page* page, CString name, int nImageID)
{
	boost::shared_ptr<SPageOne> pageOne = boost::shared_ptr<SPageOne>(new SPageOne);
	boost::shared_ptr<Page> p = boost::shared_ptr<Page>(new Page);
	pageOne->m_Dialog = p;
	pageOne->m_strName = name;

	CRect rect(190,12,628,320);			//!< �������һ��һ���������ģ��Ժ�ɲ��ø�
	p->Create(Page::IDD, this);
	p->ModifyStyle(0, WS_BORDER);
	p->UpdateWindow();
	p->MoveWindow(&rect);
	m_vtPage.push_back(pageOne);

	m_PageTree.InsertItem(name, nImageID, nImageID, TVI_ROOT, TVI_LAST);
}

void CSoftSetDlg::OnTreeLClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
	CString itemText = pTreeCtrl->GetItemText(hItem);
	pTreeCtrl->SelectDropTarget(hItem);
	pTreeCtrl->SetFocus();
	foreach(boost::shared_ptr<SPageOne> page, m_vtPage)
	{
		if(page->m_strName != itemText)		continue;
		ShowPage(page);
		return;
	}
}

void CSoftSetDlg::OnTreeKeyDown(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, UINT nChar)
{
	if(nChar == VK_RETURN)
	{
		pTreeCtrl->SelectDropTarget(hItem);
		OnTreeLClick(pTreeCtrl, hItem);
	}
}

void CSoftSetDlg::ShowPage(boost::shared_ptr<SPageOne> page)
{
	ASSERT(page);
	ASSERT(page->m_Dialog);
	ASSERT(page->m_Dialog->GetSafeHwnd());
	if(m_culPage)	m_culPage->m_Dialog->ShowWindow(SW_HIDE);
	page->m_Dialog->ShowWindow(SW_SHOW);
	m_culPage = page;
}

void CSoftSetDlg::OnBnClickedOk()
{
	OnOK();
}

LRESULT CSoftSetDlg::OnRevMsgOK(WPARAM wParam, LPARAM lParam)
{
	OnOK();
	return 0;
}

BOOL SoftInfo::CSoftSetDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HTREEITEM hItem = m_PageTree.GetSelectedItem();
	if(!hItem)		return CDialog::OnHelpInfo(pHelpInfo);
	CString itemText = m_PageTree.GetItemText(hItem);
	SoftInfo::CMyHelp* pHelp = &SoftInfo::CMyHelp::GetMe();

	if(itemText == _T("������"))			pHelp->ShowHelp(_T("��������ʾ����"));
	else if(itemText == _T("����ͼ"))		pHelp->ShowHelp(_T("����ͼ��ʾ����"));
	else if(itemText == _T("������ɨ��"))	pHelp->ShowHelp(_T("��������"));
	else if(itemText == _T("��������"))	pHelp->ShowHelp(_T("�����������"));
	else if(itemText == _T("֧�ֵ��豸"))	pHelp->ShowHelp(_T("֧�ֵ��豸"));

	return CDialog::OnHelpInfo(pHelpInfo);
}
