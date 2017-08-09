// SoftSetDeviceXmlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "MainFrm.h"
#include "SpaceDevice.h"
#include "SoftInfo.h"
#include "SoftSetDeviceXmlDlg.h"

#include "XmlMgr.h"
#include "XmlDevice.h"

#include "DevMgr.h"
#include "DeviceOne.h"

// CSoftSetDeviceXmlDlg 对话框
using namespace SoftInfo;
IMPLEMENT_DYNAMIC(CSoftSetDeviceXmlDlg, CDialog)

CSoftSetDeviceXmlDlg::CSoftSetDeviceXmlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftSetDeviceXmlDlg::IDD, pParent)
{
	m_strParent = m_strDeviceName = m_strDevicePath = _T("");
}

CSoftSetDeviceXmlDlg::~CSoftSetDeviceXmlDlg()
{
}

void CSoftSetDeviceXmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEVICE, m_TreeCtrl);
	DDX_Text(pDX, IDC_EDIT_PARENT, m_strParent);
	DDX_Text(pDX, IDC_EDIT_DEVICE, m_strDeviceName);
}


BEGIN_MESSAGE_MAP(CSoftSetDeviceXmlDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_DEVIN, &CSoftSetDeviceXmlDlg::OnBnClickedBtDevin)
	ON_BN_CLICKED(IDC_BT_DEVINFO, &CSoftSetDeviceXmlDlg::OnBnClickedBtDevinfo)
	ON_BN_CLICKED(IDC_BT_ADDDEV, &CSoftSetDeviceXmlDlg::OnBnClickedBtAdddev)
	ON_BN_CLICKED(IDC_BT_DELDEV, &CSoftSetDeviceXmlDlg::OnBnClickedBtDeldev)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CSoftSetDeviceXmlDlg 消息处理程序
BOOL CSoftSetDeviceXmlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//!< 初始化公司设备树
	m_TreeCtrl.InitImg();
	FreshTree();
	UpdateData(FALSE);
	return TRUE;
}

void CSoftSetDeviceXmlDlg::OnOK()
{
	CWnd* pWnd = GetParent();
	if(!pWnd)										return;
	pWnd->PostMessage(MESSAGE_OK, 0, 0);
}

BOOL CSoftSetDeviceXmlDlg::DestroyWindow()
{
	UpdateData(TRUE);
	return CDialog::DestroyWindow();
}

void CSoftSetDeviceXmlDlg::FreshTree()
{
	m_TreeCtrl.DeleteAllItems();
	boost::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CDXPEditorApp *)AfxGetApp())->m_XmlMgr;
	boost::shared_ptr<XmlInfo::CXmlDevice> device;

	std::map<CString, HTREEITEM> m_mpCompanyItem;

	HTREEITEM hCompany = NULL, hDevice = NULL;
	foreach(device, xmlMgr->m_vtDevice)
	{
		if(!device)			continue;
		hCompany = m_mpCompanyItem[device->getCompany()];
		if(!hCompany)
		{
			hCompany = m_TreeCtrl.InsertItem(device->getCompany(), 12, 12, TVI_ROOT, TVI_LAST);
			m_mpCompanyItem[device->getCompany()] = hCompany;
		}
		hDevice = m_TreeCtrl.InsertItem(device->getName(), 13, 13, hCompany, TVI_LAST);
		m_TreeCtrl.SetItemData(hDevice, device->getID());
		m_TreeCtrl.Expand(hCompany, TVE_EXPAND);
	}
}

void CSoftSetDeviceXmlDlg::OnBnClickedBtDevin()
{
	CString extendName = _T("*.dd");
	CString extendNames = _T("设备描述文件 (") + extendName + _T(")|") + extendName + _T("||");
	CFileDialog openDlg(TRUE, (LPCTSTR)extendName, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);

	if(IDOK == openDlg.DoModal())
	{
		CString title = openDlg.GetFileTitle();
		m_strDeviceName = openDlg.GetFileName();
		m_strDevicePath = openDlg.GetPathName();
		m_XmlDevice = boost::shared_ptr<XmlInfo::CXmlDevice>(new XmlInfo::CXmlDevice);
		m_XmlDevice->m_strPathName = m_strDevicePath;
		if(!m_XmlDevice->OpenXml())
		{
			m_strDeviceName = m_strDevicePath = _T("");
			m_XmlDevice.reset();
			return;
		}
		m_strParent = m_XmlDevice->getCompany();
	}
	UpdateData(FALSE);
}

void CSoftSetDeviceXmlDlg::OnBnClickedBtDevinfo()
{
}

void CSoftSetDeviceXmlDlg::OnBnClickedBtAdddev()
{
	if(!m_XmlDevice)						return;
	boost::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CDXPEditorApp *)AfxGetApp())->m_XmlMgr;
	if(!xmlMgr->AddDevice(m_XmlDevice))		return;
	FreshTree();
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();
	mf->GetSpaceDevice()->FreshCompanyDevice();
}

void CSoftSetDeviceXmlDlg::OnBnClickedBtDeldev()
{
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
	if(m_TreeCtrl.GetParentItem(hItem) == NULL)		return;
	UINT id = m_TreeCtrl.GetItemData(hItem);
	boost::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CDXPEditorApp *)AfxGetApp())->m_XmlMgr;
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = xmlMgr->GetDevice(id);
	ASSERT(xmlDev);

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	boost::shared_ptr<MVC::Device::CDeviceOne> projDev;
	foreach(projDev, devMgr->m_vtDevice)
	{
		if(!projDev)								continue;
		if(xmlDev != projDev->GetXmlInfo())			continue;
		MessageBox(_T("该类型设备已经在工程中被使用，无法移除！"), _T("提示"), MB_OK|MB_ICONINFORMATION);
		return;
	}
	xmlMgr->DeleteDevice(id);
	FreshTree();
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();
	mf->GetSpaceDevice()->FreshCompanyDevice();
}

BOOL SoftInfo::CSoftSetDeviceXmlDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("支持的设备"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
