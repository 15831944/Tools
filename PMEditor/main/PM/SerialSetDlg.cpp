// SerialSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "SerialSetDlg.h"
#include "StringAddDelDlg.h"
#include "DevMgr.h"
#include "DSerial.h"

// CSerialSetDlg 对话框
using namespace Dialog;
IMPLEMENT_DYNAMIC(CSerialSetDlg, CXTResizePropertyPage)

CSerialSetDlg::CSerialSetDlg(CWnd* pParent /*=NULL*/)
	: CXTResizePropertyPage(CSerialSetDlg::IDD)
{
//	m_strCulSerialName = _T("");
	m_bInit = false;
}

CSerialSetDlg::~CSerialSetDlg()
{
}

void CSerialSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERIAL_TREE, m_TreeCtrl);
	DDX_Control(pDX, IDC_SERIAL_HOLDER, m_GridHolder);
}

BEGIN_MESSAGE_MAP(CSerialSetDlg, CXTResizePropertyPage)
	ON_BN_CLICKED(ID_ADD_SERIAL, &CSerialSetDlg::OnBnClickedAddSerial)
	ON_BN_CLICKED(ID_DEL_SERIAL, &CSerialSetDlg::OnBnClickedDelSerial)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
	ON_WM_TIMER()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CSerialSetDlg 消息处理程序
BOOL CSerialSetDlg::OnInitDialog()
{
	CXTResizePropertyPage::OnInitDialog();

	//!< 初始化Property表格
	CRect rc;
	m_GridHolder.GetWindowRect(&rc);
	m_GridHolder.ShowWindow(SW_HIDE);
	ScreenToClient(&rc);
	if(!m_PropertyGrid.Create(rc, this, IDC_PROPERTY_GRID))		return FALSE;
	m_PropertyGrid.SetFont(GetFont());							//!< 设置一下字体，以确保每次打开的字体都一样
	m_PropertyGrid.SetTheme(xtpGridThemeNativeWinXP);			//!< 设置主题
	m_Object.m_pParent = this;									//!< 属性能找到自己，从而找到自己的串口列表

	//!< 初始化串口列表
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial, newSerial;
	m_ltSerial.clear();
	for (auto serial : devMgr->m_ltSerial)
	{
		newSerial = std::shared_ptr<MVC::Device::InterfaceSet::CDSerial>(new MVC::Device::InterfaceSet::CDSerial());
		newSerial->CopyFrom(*serial);
		m_ltSerial.push_back(newSerial);
	}
	m_bInit = true;

	//!< 初始化串口树
	m_TreeCtrl.SetOwner(this);
	m_TreeCtrl.InitImg();

	//!< 初始化串口数节点
	InitSerialList();
	return TRUE;
}

void CSerialSetDlg::InitSerialList()
{
	m_TreeCtrl.DeleteAllItems();
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serialtmp;

	for (auto serialtmp : m_ltSerial)
		m_TreeCtrl.InsertItem(serialtmp->getName(), 11, 11, TVI_ROOT, TVI_LAST);

	if(!m_ltSerial.empty()){
		OnTreeLClick(&m_TreeCtrl, m_TreeCtrl.GetRootItem());
		m_TreeCtrl.SetFocus();
	}
}

//!< 重置串口
void CSerialSetDlg::OnBnClickedAddSerial()
{
	CStringAddDelDlg dlg;
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial;
	CString str;
	for(int i = 0; i < 128; ++i)
	{
		str.Format("COM%d", i + 1);
		if(GetSerial(str))			dlg.m_ltIn.push_back(str);
		else						dlg.m_ltOut.push_back(str);
	}
	if(IDOK != dlg.DoModal())		return;

	//!< 重新选择串口结束，现在开始创建对应的串口对象
	std::list<UINT> ltNumber;
	CString text;
	int number;
	//!< 获得需要添加的串口号
	for (auto str : dlg.m_ltIn)
	{
		text = str;
		if(!CGbl::GetNumberFromString(text, number, 1)) continue;
		ltNumber.push_back(number - 1);
	}
	ltNumber.sort();

	//!< 创建要添加的所有串口
	std::list<std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> > ltSerial;
	for (auto number : ltNumber)
	{
		str.Format("COM%d", number + 1);
		serial = GetSerial(str);
		if(!serial)
		{
			serial = std::shared_ptr<MVC::Device::InterfaceSet::CDSerial>(new MVC::Device::InterfaceSet::CDSerial);
			serial->setNumber(number);
		}
		ltSerial.push_back(serial);
	}
	m_ltSerial.clear();
	for (auto serial : ltSerial)
		m_ltSerial.push_back(serial);

	//!< 重新画串口树
	InitSerialList();
}

//!< 删除串口
void CSerialSetDlg::OnBnClickedDelSerial()
{
}

void CSerialSetDlg::OnTreeLClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
	pTreeCtrl->SelectDropTarget(hItem);
	CString strName = pTreeCtrl->GetItemText(hItem);
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial = GetSerial(strName);
	if(!serial)		return;
	m_PropertyGrid.Refresh();
	m_Object.SetInfo(serial->getName());
	m_Object.ShowInfo(m_PropertyGrid);
	UpdateData(FALSE);
}

void CSerialSetDlg::OnTreeRClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, CPoint point)
{
}

void CSerialSetDlg::OnTreeKeyDown(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, UINT nChar)
{
	if(nChar == VK_RETURN){
		pTreeCtrl->SelectDropTarget(hItem);
		OnTreeLClick(pTreeCtrl, hItem);
	}
}

std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> CSerialSetDlg::GetSerial(CString name)
{
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial, empty;
	for (auto serial : m_ltSerial)
		if(name == serial->getName())
			return serial;
	return empty;
}

LRESULT CSerialSetDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED){		//!< 项的值被改变
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		m_Object.OnItemModify(m_PropertyGrid, pItem->GetID());
	}
	return 0;
}

void CSerialSetDlg::SaveModify()
{
	if(!m_bInit)		return;
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial;

	devMgr->m_ltSerial.clear();
	for (auto serial : m_ltSerial){
		devMgr->m_ltSerial.push_back(serial);
	}
	m_ltSerial.clear();
}
void Dialog::CSerialSetDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnTimer(nIDEvent);
}

BOOL Dialog::CSerialSetDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	m_Object.OnShowHelp();
	return CDialog::OnHelpInfo(pHelpInfo);
}