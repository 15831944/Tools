// ScanSetDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "MainFrm.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "ServerCtrl.h"
#include "DevMgr.h"
#include "DSerial.h"
#include "DevMgr.h"
#include "ScanSetDlg.h"
#include "ScanSetInfo.h"

using namespace Dialog;
// CScanSetDlg 对话框
IMPLEMENT_DYNAMIC(CScanSetDlg, CDialog)

CScanSetDlg::CScanSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanSetDlg::IDD, pParent)
	, m_bUseEthernet(FALSE)
	, m_bUseSerial(FALSE)
	, m_bSync(TRUE)		//!< 是否将扫描配置同步到工程师站配置中
	, m_nSerialStart(0)
	, m_nSerialEnd(0)
	, m_uiIPStart(0)
	, m_uiIPEnd(-1)
{
}

CScanSetDlg::~CScanSetDlg()
{
}

void CScanSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ETHERNET, m_bUseEthernet);
	DDX_Check(pDX, IDC_CHECK_SERIAL, m_bUseSerial);
	DDX_Check(pDX, IDC_CHECK_SYNC, m_bSync);
	DDX_Text(pDX, IDC_EDIT_ADDR_START, m_nSerialStart);
	DDX_Text(pDX, IDC_EDIT_ADDR_END, m_nSerialEnd);
	DDX_Control(pDX, IDC_COMBO_COM, m_cbCom);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_cbBaud);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_cbParity);
	DDX_Control(pDX, IDC_COMBO_DATABITS, m_cbDataBits);
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_cbStopBits);
	DDX_Control(pDX, IDC_COMBO_SCANLEVEL, m_cbScanLevel);
	DDX_IPAddress(pDX, IDC_IPSTART, m_uiIPStart);
	DDX_IPAddress(pDX, IDC_IPEND, m_uiIPEnd);
}

BEGIN_MESSAGE_MAP(CScanSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CScanSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_SCAN, &CScanSetDlg::OnBnClickedScan)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_COM, &CScanSetDlg::OnCbnSelchangeComboCom)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CScanSetDlg 消息处理程序
BOOL CScanSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//!< 初始化各个控件
	std::shared_ptr<MVC::Device::CScanSetInfo> info = MVC::Device::CDevMgr::GetMe().GetScanInfo();
	m_bSync = info->IsSync();
	m_bUseEthernet = info->IsUseEthernet();
	m_bUseSerial = info->IsUseSerial();
	m_uiIPStart = GetIPTurnOver(info->GetIPStart());
	m_uiIPEnd = GetIPTurnOver(info->GetIPEnd());
	m_nSerialStart = info->GetSerialStart();
	m_nSerialEnd = info->GetSerialEnd();
	m_cbScanLevel.SetCurSel(info->GetScanLevel() - 1);

	CGbl* gbl = &CGbl::GetMe();
	CString str;

	std::list<CString> ltStr = gbl->getBaudList();
	for (auto str : ltStr)		m_cbBaud.AddString(str);
	m_cbBaud.SetCurSel(gbl->getBaudIndex(info->GetBaud()));

	ltStr = gbl->getCheckStyleList();
	for (auto str : ltStr)		m_cbParity.AddString(str);
	m_cbParity.SetCurSel(gbl->getCheckIndex(info->GetParity()));

	ltStr = gbl->getDataBitNumList();
	for (auto str : ltStr)		m_cbDataBits.AddString(str);
	m_cbDataBits.SetCurSel(gbl->getDataIndex(info->GetDataBit()));

	ltStr = gbl->getStopBitNumList();
	for (auto str : ltStr)		m_cbStopBits.AddString(str);
	m_cbStopBits.SetCurSel(gbl->getStopIndex(info->GetStopBit()));

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial;
	str = info->GetCom();
	int index = 0, i = 0;
	for (auto serial : devMgr->m_ltSerial)
	{
		m_cbCom.AddString(serial->getName());
		if(str == serial->getName())	index = i;
		++i;
	}
	if(!devMgr->m_ltSerial.empty())		m_cbCom.SetCurSel(index);
	else{								GetDlgItem(IDC_CHECK_SERIAL)->EnableWindow(FALSE); info->SetUseSerial(false); m_bUseSerial = false;}
	UpdateData(FALSE);
	OnCbnSelchangeComboCom();
	return TRUE;
}

void CScanSetDlg::OnBnClickedOk()
{
	if(OnApplay())				OnOK();
}

//!< 检验
bool CScanSetDlg::OnCheck()
{
	UpdateData(TRUE);
	if(GetIPTurnOver(m_uiIPEnd) < GetIPTurnOver(m_uiIPStart))
	{
		MessageBox(_T("以太网扫描范围错误"), _T("错误"), MB_OK | MB_ICONWARNING);
		GotoDlgCtrl(GetDlgItem(IDC_IPSTART));
		return false;
	}
	if(m_nSerialStart > 255)
	{
		MessageBox(_T("串口扫描范围错误"), _T("错误"), MB_OK | MB_ICONWARNING);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_ADDR_START));
		return false;
	}
	if(m_nSerialEnd > 255 || m_nSerialEnd < m_nSerialStart)
	{
		MessageBox(_T("串口扫描范围错误"), _T("错误"), MB_OK | MB_ICONWARNING);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_ADDR_END));
		return false;
	}
	return true;
}

//!< 应用
bool CScanSetDlg::OnApplay()
{
	if(!OnCheck())				return false;
	std::shared_ptr<MVC::Device::CScanSetInfo> info = MVC::Device::CDevMgr::GetMe().GetScanInfo();
	info->SetSync(m_bSync);
	info->SetUseEthernet(m_bUseEthernet);
	info->SetUseSerial(m_bUseSerial);
	info->SetIPStart(GetIPTurnOver(m_uiIPStart));
	info->SetIPEnd(GetIPTurnOver(m_uiIPEnd));
	info->SetSerialStart(m_nSerialStart);
	info->SetSerialEnd(m_nSerialEnd);
	info->SetScanLevel(m_cbScanLevel.GetCurSel() + 1);

	CGbl* gbl = &CGbl::GetMe();
	info->SetBaud(gbl->getBaudStr(m_cbBaud.GetCurSel()));
	info->SetParity(gbl->getCheckStyleStr(m_cbParity.GetCurSel()));
	info->SetDataBit(gbl->getDataBitNumStr(m_cbDataBits.GetCurSel()));
	info->SetStopBit(gbl->getStopBitNumStr(m_cbStopBits.GetCurSel()));

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial;
	int index = m_cbCom.GetCurSel(), i = 0;
	for (auto serial : devMgr->m_ltSerial)
	{
		if(index != i++)		continue;
		info->SetCom(serial->getName());
		break;
	}

	if(m_bSync)		info->OnSync();
	return true;
}

//!< 返回IP的逆端值
UINT CScanSetDlg::GetIPTurnOver(UINT ip)
{
	byte *pIP = (byte*)&ip;
	swap(pIP[0], pIP[3]);
	swap(pIP[1], pIP[2]);
	return ip;
}

void CScanSetDlg::OnBnClickedScan()
{
	if(CProjectMgr::GetMe().IsWatch())
	{
		MessageBox(_T("监控状态下不能进行扫描!"), _T("提示"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	if(!OnApplay())											return;
	if(Servers::DXP::CServerCtrl::GetMe().GetScanState() == 1)
	{
		((CMainFrame*)g_App.GetMainWnd())->OnScanStop();
	}
	SetTimer(1, 1000, NULL);
	OnTimer(1);
}

void Dialog::CScanSetDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		if(Servers::DXP::CServerCtrl::GetMe().GetScanState() == 0)
		{
			((CMainFrame*)g_App.GetMainWnd())->OnScanStart();
			KillTimer(1);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void Dialog::CScanSetDlg::OnCbnSelchangeComboCom()
{
	if(!m_bSync)							return;
	UpdateData(TRUE);
	CGbl* gbl = &CGbl::GetMe();
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial;
	CString strCom;
	m_cbCom.GetWindowText(strCom);
	for (auto serial : devMgr->m_ltSerial)
	{
		if(serial->getName() != strCom)		continue;
		m_cbBaud.SetCurSel(serial->getBaud());
		m_cbParity.SetCurSel(serial->getCheckStyle());
		m_cbDataBits.SetCurSel(serial->getDataBitNum());
		m_cbStopBits.SetCurSel(serial->getStopBitNum());
	}

	UpdateData(FALSE);
}

BOOL Dialog::CScanSetDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("扫描配置"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
