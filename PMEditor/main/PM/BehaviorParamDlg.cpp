// BehaviorParamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "ProjectMgr.h"
#include "BehaviorParamDlg.h"
#include "ObjectInProperty.h"

#include "DevMgr.h"
#include "DeviceOne.h"
#include "PropertyParam.h"
#include "XmlDevice.h"
#include "XmlBehavior.h"
#include "XmlProtocol.h"
#include "XmlInterface.h"

// CBehaviorParamDlg 对话框
using namespace Dialog;
IMPLEMENT_DYNAMIC(CBehaviorParamDlg, CPropertyGridDlgBase)

CBehaviorParamDlg::CBehaviorParamDlg(CWnd* pParent /*=NULL*/)
	: CPropertyGridDlgBase(CBehaviorParamDlg::IDD, pParent)
	, m_nWaitToCount(0)							//!< 需要等待回复的数量
	, m_CulItem(NULL)
{
}

CBehaviorParamDlg::~CBehaviorParamDlg()
{
}

void CBehaviorParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyGridDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLACEHOLDER, m_GridHolder);
	DDX_Control(pDX, IDC_BEHAVIOR_TREE, m_TreeCtrl);
	DDX_Control(pDX, IDC_GROUP_LIST, m_GroupList);
	DDX_Text(pDX, IDC_STATIC_GROUPNAME, m_strGroupName);
	DDX_Text(pDX, IDC_EDIT_OUT, m_strOut);
}

BEGIN_MESSAGE_MAP(CBehaviorParamDlg, CPropertyGridDlgBase)
	ON_BN_CLICKED(IDOK, &CBehaviorParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBehaviorParamDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BEHAVIOR_IN, &CBehaviorParamDlg::OnBnClickedBehaviorIn)
	ON_BN_CLICKED(ID_BEHAVIOR_OUT, &CBehaviorParamDlg::OnBnClickedBehaviorOut)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
	ON_CBN_SELCHANGE(IDC_GROUP_LIST, &CBehaviorParamDlg::OnGroupChange)
	ON_BN_CLICKED(ID_BEHAVIOR_UPLOAD, &CBehaviorParamDlg::OnBnClickedBehaviorUpload)
	ON_BN_CLICKED(ID_BEHAVIOR_DOWNLOAD, &CBehaviorParamDlg::OnBnClickedBehaviorDownload)
	ON_WM_TIMER()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CBehaviorParamDlg 消息处理程序
INT_PTR CBehaviorParamDlg::DoModal(UINT devID)
{
	m_uiDevID = devID;
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	m_ShowDev = devMgr->GetDevice(devID);
	ASSERT(m_ShowDev);
	return CPropertyGridDlgBase::DoModal();
}

BOOL CBehaviorParamDlg::DestroyWindow()
{
	m_PropertyGrid.ResetContent();
	MVC::Device::CDevMgr::GetMe().UnRegisteBRequest(this);
	return CPropertyGridDlgBase::DestroyWindow();
}

BOOL CBehaviorParamDlg::OnInitDialog()
{
	CPropertyGridDlgBase::OnInitDialog();
	MVC::Device::CDevMgr::GetMe().RegisteBRequest(this);

	m_TreeCtrl.InitImg();
	m_TreeCtrl.SetOwner(this);

	//!< 初始化Property表格
	CRect rc;
	m_GridHolder.GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(!m_PropertyGrid.Create(rc, this, IDC_PROPERTY_GRID))		return FALSE;
	SetWindowText(m_Object.GetTitle());					//!< 设置标题
	m_PropertyGrid.SetFont(GetFont());					//!< 设置一下字体，以确保每次打开的字体都一样
// 	m_PropertyGrid.HighlightChangedItems(TRUE);			//!< 高亮被修改的部分
	m_PropertyGrid.SetTheme(xtpGridThemeNativeWinXP);	//!< 设置主题

	//!< 初始化树中的行为列表
	ASSERT(m_ShowDev);
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	boost::shared_ptr<XmlInfo::CXmlBehavior> xmlBehavior;
	//std::list<CString> ltBehavior;
	std::map<CString, boost::shared_ptr<XmlInfo::CXmlBehavior> > mpBev;
	foreach(xmlBehavior, xmlDev->m_vtBehavior)
	{
		if(!xmlBehavior)				continue;
		if(!xmlBehavior->m_bShowUser)	continue;
		mpBev[xmlBehavior->m_strName] = xmlBehavior;
	}

	std::map<CString, boost::shared_ptr<XmlInfo::CXmlBehavior> >::iterator iter_bev;
	//foreach(xmlBehavior, xmlDev->m_vtBehavior)
	for(iter_bev = mpBev.begin(); iter_bev != mpBev.end(); iter_bev++)
	{
		xmlBehavior = iter_bev->second;
		m_TreeCtrl.InsertItem(iter_bev->first, 14, 14, TVI_ROOT, TVI_LAST);
//		bool bExist = false;
		//!< 看看这个行为是不是已经插到树中了
//		foreach(CString name, ltBehavior)
//		{
//			if(name == xmlBehavior->m_strName){
//				bExist = true;	break;
//			}
//		}
//		if(!bExist && xmlBehavior->m_bShowUser){
//		m_TreeCtrl.InsertItem(xmlBehavior->m_strName, 14, 14, TVI_ROOT, TVI_LAST);
//			ltBehavior.push_back(xmlBehavior->m_strName);
//		}
	}
	int count = m_TreeCtrl.GetCount();
	if(m_TreeCtrl.GetCount() > 0)
		OnTreeLClick(&m_TreeCtrl, m_TreeCtrl.GetRootItem());
	else{
		GetDlgItem(IDC_STATIC_GROUPNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GROUP_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_FRESH)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_BEHAVIOR_UPLOAD)->EnableWindow(FALSE);
		GetDlgItem(ID_BEHAVIOR_DOWNLOAD)->EnableWindow(FALSE);
		GetDlgItem(ID_BEHAVIOR_OUT)->EnableWindow(FALSE);
		GetDlgItem(ID_BEHAVIOR_IN)->EnableWindow(FALSE);
	}
	return TRUE;
}

void CBehaviorParamDlg::OnBnClickedOk()
{
	CPropertyGridDlgBase::OnOK();
}

void CBehaviorParamDlg::OnBnClickedCancel()
{
	CPropertyGridDlgBase::OnCancel();
}

void CBehaviorParamDlg::OnBnClickedBehaviorIn()
{
	if(!m_ShowDev)		return;

	CString extendName = _T("*.dcfg");
	CString extendNames = _T("设备配置文件 (") + extendName + _T(")|") + extendName + _T("||");
	CFileDialog openDlg(TRUE, (LPCTSTR)extendName, _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	if(IDOK == openDlg.DoModal()){
		if(m_ShowDev->DevIn(openDlg.GetPathName(), openDlg.GetFileTitle()))
			MessageBox(_T("导入成功"), _T("提示"), MB_ICONINFORMATION);
		else
			MessageBox(_T("导入失败"), _T("提示"), MB_ICONEXCLAMATION);
		OnTreeLClick(&m_TreeCtrl, m_CulItem);
	}
}

void CBehaviorParamDlg::OnBnClickedBehaviorOut()
{
	if(!m_ShowDev)		return;

	CString extendName = _T("*.dcfg");
	CString extendNames = _T("设备配置文件 (") + extendName + _T(")|") + extendName + _T("||");
	CFileDialog openDlg(FALSE, (LPCTSTR)extendName, _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	if(IDOK == openDlg.DoModal()){
		if(m_ShowDev->DevOut(openDlg.GetPathName(), openDlg.GetFileTitle()))
			MessageBox(_T("导出成功"), _T("提示"), MB_ICONINFORMATION);
		else
			MessageBox(_T("导出失败"), _T("提示"), MB_ICONEXCLAMATION);
	}
}

void CBehaviorParamDlg::OnTreeDblClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
	m_CulItem = hItem;
}

void CBehaviorParamDlg::OnTreeLClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
	pTreeCtrl->SelectDropTarget(hItem);
	pTreeCtrl->SetFocus();
	m_CulItem = hItem;

	//!< 找到当前被选中的行为列表
	if(!m_ShowDev)		return;
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	std::list< boost::shared_ptr<XmlInfo::CXmlBehavior> > ltXmlBehavior = xmlDev->getBehavior(pTreeCtrl->GetItemText(hItem));
	ASSERT(!ltXmlBehavior.empty());

	//!< 显示或隐藏上下载按钮
	bool bSlave = false;
	boost::shared_ptr<XmlInfo::CXmlBehavior> xmlBehavior;
	bool bUp = false;
	bool bDown = false;
	foreach(xmlBehavior, ltXmlBehavior)
	{
		if(xmlBehavior->m_uiType == 0)		bUp = true;
		if(xmlBehavior->m_uiType == 1)		bDown = true;
	}
	//!< 如果只有一类行为，则显示成“执行”
//	if((bUp && !bDown) || (!bUp && bDown))
	if(!bUp && bDown)
	{
		GetDlgItem(ID_BEHAVIOR_UPLOAD)->SetWindowText(_T("执行"));
		GetDlgItem(ID_BEHAVIOR_DOWNLOAD)->ShowWindow(SW_HIDE);
	}
	else if(bUp && !bDown)
	{
		GetDlgItem(ID_BEHAVIOR_UPLOAD)->SetWindowText(_T("上载"));
		GetDlgItem(ID_BEHAVIOR_DOWNLOAD)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(ID_BEHAVIOR_UPLOAD)->SetWindowText(_T("上载"));
		GetDlgItem(ID_BEHAVIOR_DOWNLOAD)->ShowWindow(SW_SHOW);
	}

	//!< 监控时或扫描时才能上下载
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->IsWatch() && !projMgr->IsScan())
	{
		GetDlgItem(ID_BEHAVIOR_UPLOAD)->EnableWindow(FALSE);
		GetDlgItem(ID_BEHAVIOR_DOWNLOAD)->EnableWindow(FALSE);
	}

	//!< 如果是组的，需要显示出组的下拉列表
	UINT arrayNum = 1;
	foreach(xmlBehavior, ltXmlBehavior){
		arrayNum = max(arrayNum, xmlBehavior->m_uiArrayNum);
		if(xmlBehavior->m_uiArrayNum > 1)
			m_strGroupName = xmlBehavior->m_strArrayName;
	}

	CRect bigRect, smallRect;
	bigRect = CRect(248,29,643,465);
	smallRect = CRect(248,60,643,465);
	if(arrayNum > 1)
	{
		GetDlgItem(IDC_STATIC_GROUPNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_GROUPNAME)->SetWindowText(m_strGroupName);
		GetDlgItem(IDC_GROUP_LIST)->ShowWindow(SW_SHOW);
//		GetDlgItem(ID_FRESH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROPERTY_GRID)->MoveWindow(&smallRect, TRUE);
		m_GroupList.ResetContent();
		CString strCount;
		for(int i = 0; i < arrayNum; ++i){
			strCount.Format("%d", i);
			m_GroupList.AddString(strCount);
		}
		m_GroupList.SetCurSel(0);
	}
	else
	{
		GetDlgItem(IDC_STATIC_GROUPNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GROUP_LIST)->ShowWindow(SW_HIDE);
//		GetDlgItem(ID_FRESH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PROPERTY_GRID)->MoveWindow(&bigRect, TRUE);
	}

	//!< 显示参数
	if(bUp && !bDown)		OnParaFresh(true);		//!< 如果是有上载，那么所有参数都是只读的
	else					OnParaFresh(false);

	//!< 自动线上载一次
	if(SoftInfo::CSoftInfo::GetMe().getUploadBFirst())
		SetTimer(1, 200, NULL);
}

void CBehaviorParamDlg::OnTreeRClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, CPoint point)
{
}

void CBehaviorParamDlg::OnTreeKeyDown(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, UINT nChar)
{
	if(nChar == VK_RETURN){
		pTreeCtrl->SelectDropTarget(hItem);
		OnTreeLClick(pTreeCtrl, hItem);
	}
}

//!< 刷新数据
void CBehaviorParamDlg::FreshData()
{
	UpdateData();
	m_Object.m_uiGroupIndex = m_GroupList.GetCurSel();
	if(m_Object.m_uiGroupIndex == UINT(-1))
	m_Object.m_uiGroupIndex = 0;
	m_Object.FreshAllData(m_PropertyGrid);
}

void CBehaviorParamDlg::OnGroupChange()
{
	FreshData();

	//!< 自动线上载一次
	if(SoftInfo::CSoftInfo::GetMe().getUploadBFirst())
		SetTimer(1, 200, NULL);
}

void CBehaviorParamDlg::OnParaFresh(bool bReadOnly)
{
	if(!m_ShowDev)		return;
	if(IsDevDwon())		return;
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	CString strBehavior = m_TreeCtrl.GetItemText(m_CulItem);
	std::list< boost::shared_ptr<XmlInfo::CXmlBehavior> > ltXmlBehavior = xmlDev->getBehavior(strBehavior);
	ASSERT(!ltXmlBehavior.empty());
	m_Object.SetInfo(m_ShowDev->getID(), ltXmlBehavior.front()->m_strName, m_GroupList.GetCurSel(), bReadOnly);
	m_Object.ShowInfo(m_PropertyGrid);
	m_PropertyGrid.SetFont(GetFont());

	OnGroupChange();
}

LRESULT CBehaviorParamDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_INPLACEBUTTONDOWN)		//!< 如果按下了按钮
	{
		CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
		m_Object.OnButtonClick(m_PropertyGrid, pButton->GetID());
	}
	if (wParam == XTP_PGN_SORTORDER_CHANGED)		//!< 排序方式改变
	{
	}
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)		//!< 项的值被改变
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		m_Object.OnItemModify(m_PropertyGrid, pItem->GetID());		//!< 修改了菜的那些色香味都得写在菜单上
	}
	if (wParam == XTP_PGN_SELECTION_CHANGED)		//!< 选中项改变
	{
	}
	if (wParam == XTP_PGN_EDIT_CHANGED)				//!< Edit内容改变
	{
	}
	return 0;
}

void Dialog::CBehaviorParamDlg::OnBnClickedBehaviorUpload()
{
	if(!m_ShowDev)		return;
	if(IsDevDwon())		return;
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	CString strBehavior = m_TreeCtrl.GetItemText(m_CulItem);
	std::list< boost::shared_ptr<XmlInfo::CXmlBehavior> > ltXmlBehavior = xmlDev->getBehavior(strBehavior);
	boost::shared_ptr<XmlInfo::CXmlBehavior> behavior;

	CString str;
	m_strOperate = _T("");
	m_strOut = _T("");
	UpdateData(FALSE);
	GetDlgItem(ID_BEHAVIOR_UPLOAD)->GetWindowText(str);
	if(str.Trim() == _T("执行"))
	{
		bool bUp = false;
		bool bDown = false;
		foreach(behavior, ltXmlBehavior)
		{
			if(behavior->m_uiType == 0)			bUp = true;
			if(behavior->m_uiType == 1)			bDown = true;
		}
		if(!bUp && bDown)		//!< 如果只有下载的情况
		{
			OnBnClickedBehaviorDownload();
			m_strOperate = _T("执行");
			return;
		}
	}
	//!< 要上载的行为
	CComVariant cvr;
	m_nWaitToCount = 0;
	foreach(behavior, ltXmlBehavior)
	{
		if(behavior->m_uiType == 0){
			if(behavior->m_uiArrayNum > 1)		cvr = int(m_GroupList.GetCurSel());
			else								cvr = int(0);
			if(!CProjectMgr::GetMe().IsScan())
				m_ShowDev->UpLoadBehavior(behavior->m_uiID, cvr);
			else
			{
				std::list<UINT> ltAddr;
				m_ShowDev->GetWholeAddr(ltAddr, true);
				if(ltAddr.empty())				return;
				else							m_ShowDev->UpLoadBehavior(behavior->m_uiID, cvr);
			}
			++m_nWaitToCount;
		}
	}
	if(m_nWaitToCount > 0){
		m_strOperate = _T("读");
		EnabledEdit(FALSE);
		SetCommTimer();
	}
}

void Dialog::CBehaviorParamDlg::OnBnClickedBehaviorDownload()
{
	if(!m_ShowDev)		return;
	if(IsDevDwon())		return;
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	CString strBehavior = m_TreeCtrl.GetItemText(m_CulItem);
	std::list< boost::shared_ptr<XmlInfo::CXmlBehavior> > ltXmlBehavior = xmlDev->getBehavior(strBehavior);
	boost::shared_ptr<XmlInfo::CXmlBehavior> behavior;

	//!< 收集所有要下载的行为
	CComVariant cvr;
	m_nWaitToCount = 0;
	foreach(behavior, ltXmlBehavior)
	{
		if(behavior->m_uiType == 1){
			if(behavior->m_uiArrayNum > 1)		cvr = int(m_GroupList.GetCurSel());
			else								cvr = int(0);
			if(!CProjectMgr::GetMe().IsScan())
				m_ShowDev->DownLoadBehavior(behavior->m_uiID, cvr);
			else
			{
				std::list<UINT> ltAddr;
				m_ShowDev->GetWholeAddr(ltAddr, true);
				if(ltAddr.empty())				return;
				else							m_ShowDev->DownLoadBehavior(behavior->m_uiID, cvr);
			}
			++m_nWaitToCount;
		}
	}
	if(m_nWaitToCount > 0){
		if(m_strOperate != _T("执行"))			m_strOperate = _T("写");
		EnabledEdit(FALSE);
		SetCommTimer();
	}
}

//!< 数据已经刷新好了
void Dialog::CBehaviorParamDlg::OnDataReady()
{
	--m_nWaitToCount;
	if(m_nWaitToCount <= 0)		//!< 所有都返回了,如果是整数证明还没会完,如果是负数证明有超时
	{
		KillTimer(2);
		EnabledEdit(TRUE);
		FreshData();			//!< 直接刷数据，如果要调OnGroupChange()函数，就是导致再上载一遍，会出现死循环
		CString text = m_TreeCtrl.GetItemText(m_TreeCtrl.GetSelectedItem());
		text = m_strOperate + _T(" \"") + text + _T("\" 成功!");
		//MessageBox(text, _T("提示"), MB_OK | MB_ICONINFORMATION);
		m_strOut = text;
		UpdateData(FALSE);
	}
// 	else
// 		SetCommTimer();
}

//!< 设备拓扑改变了
void Dialog::CBehaviorParamDlg::OnDeviceChange()
{
	IsDevDwon();
}

//!< 设备突然没有了
bool Dialog::CBehaviorParamDlg::IsDevDwon()
{
	if(!m_ShowDev)															return true;
	if(MVC::Device::CDevMgr::GetMe().GetDevice(m_ShowDev->getID()))			return false;
	MessageBox(_T("设备已不存在，配置环境将自动关闭。"), _T("提示"), MB_OK|MB_ICONEXCLAMATION);
	OnOK();
	return true;
}

void Dialog::CBehaviorParamDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString text, str = _T("上载");
	if(nIDEvent == 1)
	{
		CWnd* pWnd = GetDlgItem(ID_BEHAVIOR_UPLOAD);
		if(!pWnd)						goto RET;
		if(!pWnd->IsWindowVisible())	goto RET;
		if(!pWnd->IsWindowEnabled())	goto RET;
		pWnd->GetWindowText(text);
		if(text != str)					goto RET;
		OnBnClickedBehaviorUpload();
RET:
		KillTimer(1);
	}
	else if(nIDEvent == 2)
	{
		KillTimer(2);
		m_nWaitToCount = 0;
		EnabledEdit(true);
		CString text = m_TreeCtrl.GetItemText(m_TreeCtrl.GetSelectedItem());
		text = m_strOperate + _T(" \"") + text + _T("\" 超时!");
		MessageBox(text, _T("提示"), MB_OK | MB_ICONEXCLAMATION);
		m_strOut = text;
		UpdateData(FALSE);
	}
	CWnd::OnTimer(nIDEvent);
}

//!< 禁止操作
void Dialog::CBehaviorParamDlg::EnabledEdit(BOOL enabl /* = FALSE */)
{
	m_bLock = (bool)enabl;
	m_TreeCtrl.EnableWindow(enabl);
	GetDlgItem(ID_BEHAVIOR_UPLOAD)->EnableWindow(enabl);
	GetDlgItem(ID_BEHAVIOR_DOWNLOAD)->EnableWindow(enabl);
}

//!< 通信前设置超时时间
void Dialog::CBehaviorParamDlg::SetCommTimer()
{
	KillTimer(2);
	if(m_nWaitToCount <= 0)		return;
	int level = m_ShowDev->getLevel();
	int over_time = MVC::Device::CDevMgr::GetMe().GetMaxOverTime(m_ShowDev);
	SetTimer(2, (level + 1) * over_time, NULL);
}


BOOL Dialog::CBehaviorParamDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	m_Object.OnShowHelp();
	return CDialog::OnHelpInfo(pHelpInfo);
}
