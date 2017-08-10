// DevCompanyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "DevCompanyDlg.h"

#include "XmlMgr.h"
#include "XmlDevice.h"

// CDevCompanyDlg �Ի���
using namespace Dialog;
IMPLEMENT_DYNAMIC(CDevCompanyDlg, CDialog)

CDevCompanyDlg::CDevCompanyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDevCompanyDlg::IDD, pParent)
	, m_strCompany(_T(""))
	, m_strDevType(_T(""))
	, m_strDevDescription(_T(""))
{
	m_uiDevID = UINT(-1);
}

CDevCompanyDlg::~CDevCompanyDlg()
{
}

void CDevCompanyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DEV_COMPANY, m_strCompany);
	DDX_Text(pDX, IDC_DEV_TYPENAME, m_strDevType);
	DDX_Text(pDX, IDC_DEV_DESCRIPTION, m_strDevDescription);
	DDX_Control(pDX, IDC_DEV_COMPANY_TREE, m_TreeCtrl);
}

BEGIN_MESSAGE_MAP(CDevCompanyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDevCompanyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDevCompanyDlg::OnBnClickedCancel)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CDevCompanyDlg ��Ϣ�������
BOOL CDevCompanyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//!< ��ʼ�����οؼ�
	m_TreeCtrl.InitImg();
	m_TreeCtrl.SetOwner(this);
	//HTREEITEM ;
	std::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CPMApp *)AfxGetApp())->m_XmlMgr;
	//std::shared_ptr<XmlInfo::CXmlDevice> device;
	std::map<CString, HTREEITEM> m_mpCompanyItem;

	HTREEITEM hCompany = NULL, hDevice = NULL, hFirstDev = NULL;
	for (auto device : xmlMgr->m_vtDevice)
	{
		if(!device)			continue;
		hCompany = m_mpCompanyItem[device->getCompany()];
		if(!hCompany)
		{
			hCompany = m_TreeCtrl.InsertItem(device->getCompany(), 12, 12, TVI_ROOT, TVI_LAST);
			m_mpCompanyItem[device->getCompany()] = hCompany;
		}
		hDevice = m_TreeCtrl.InsertItem(device->getName(), 13, 13, hCompany, TVI_LAST);
		m_TreeCtrl.Expand(hCompany, TVE_EXPAND);
		if(hFirstDev == 0)	hFirstDev = hDevice;
	}
	UpdateData(FALSE);
	if(hFirstDev)			OnTreeLClick(&m_TreeCtrl, hFirstDev);

	return TRUE;
}

void CDevCompanyDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_strDevType == _T(""))
	{
		MessageBox(_T("��ѡ��һ���豸"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}
	OnOK();
}

void CDevCompanyDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CDevCompanyDlg::OnTreeLClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
	HTREEITEM hParent = pTreeCtrl->GetParentItem(hItem);
	if(!hParent)		return;
	m_strCompany = pTreeCtrl->GetItemText(hParent);
	m_strDevType = pTreeCtrl->GetItemText(hItem);

	std::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CPMApp *)AfxGetApp())->m_XmlMgr;
	std::shared_ptr<XmlInfo::CXmlDevice> device = xmlMgr->GetDevice(m_strDevType, m_strCompany);
	ASSERT(device);
	m_strDevDescription = device->getName();
	m_uiDevID = device->getID();
	pTreeCtrl->SelectDropTarget(hItem);
	UpdateData(FALSE);
}

void CDevCompanyDlg::OnTreeRClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, CPoint point)
{
	OnTreeLClick(pTreeCtrl, hItem);
}

void CDevCompanyDlg::OnTreeKeyDown(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, UINT nChar)
{
	if(nChar == VK_RETURN){
		pTreeCtrl->SelectDropTarget(hItem);
		OnTreeLClick(pTreeCtrl, hItem);
	}
}

BOOL Dialog::CDevCompanyDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("���豸"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
