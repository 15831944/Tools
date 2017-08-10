// SoftSetTuoPuDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "SoftInfo.h"
#include "DeviceMapDoc.h"
#include "DeviceMapView.h"
#include "SoftSetTuoPuDlg.h"


// CSoftSetTuoPuDlg �Ի���
using namespace SoftInfo;
IMPLEMENT_DYNAMIC(CSoftSetTuoPuDlg, CDialog)

CSoftSetTuoPuDlg::CSoftSetTuoPuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftSetTuoPuDlg::IDD, pParent)
{
	m_bTooltip = TRUE;
	m_bReadBehaviorFirst = TRUE;
	m_bShowAddr = TRUE;
}

CSoftSetTuoPuDlg::~CSoftSetTuoPuDlg()
{
}

void CSoftSetTuoPuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_HV, m_comHV);
	DDX_Check(pDX, IDC_CHECK_TOOLTIP, m_bTooltip);
	DDX_Check(pDX, IDC_CHECK_UPBEHAVIORFIRST, m_bReadBehaviorFirst);
	DDX_Check(pDX, IDC_CHECK_SHOWADDR, m_bShowAddr);
	DDX_Control(pDX, IDC_SPIN_ZOOM_CTRL, m_ZoomCtrl);
}

BEGIN_MESSAGE_MAP(CSoftSetTuoPuDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_FONT, &CSoftSetTuoPuDlg::OnBnClickedBtFont)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CSoftSetTuoPuDlg ��Ϣ�������

BOOL CSoftSetTuoPuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	m_comHV.SetCurSel(csi->getHDevMap()?0:1);
	m_bTooltip = csi->getTooltip();
	m_bReadBehaviorFirst = csi->getUploadBFirst();
	m_bShowAddr = csi->getShowAddr();
	m_ZoomCtrl.SetRange32(50, 500);
	m_ZoomCtrl.SetPos32(csi->getZoomSize());

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CSoftSetTuoPuDlg::DestroyWindow()
{
	UpdateData(TRUE);
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	csi->setHDevMap(m_comHV.GetCurSel() == 0?true:false);
	csi->setTooltip(!!m_bTooltip);
	csi->setUploadBFrist(!!m_bReadBehaviorFirst);
	csi->setShowAddr(!!m_bShowAddr);
	UINT zoomValue = m_ZoomCtrl.GetPos32();
	if(zoomValue < 50)		zoomValue = 50;
	if(zoomValue > 500)		zoomValue = 500;
	csi->setZoomSize(zoomValue);
	POSITION pos = g_App.m_pDeviceDocMgr->GetFirstDocPosition();
	while(pos)
	{
		MVC::Device::CDeviceMapDoc* pDoc = (MVC::Device::CDeviceMapDoc*)g_App.m_pDeviceDocMgr->GetNextDoc(pos);
		MVC::Device::CDeviceMapView* pView = (MVC::Device::CDeviceMapView*)pDoc->GetView();
		if(pView)	pView->FreshDeviceMap();
	}
	return CDialog::DestroyWindow();
}

void SoftInfo::CSoftSetTuoPuDlg::OnBnClickedBtFont()
{
	LOGFONT logFont;
	SoftInfo::CSoftInfo::GetMe().getTuopuFontInfo(logFont);
	CFontDialog dlg(&logFont);
	dlg.m_cf.Flags &= ~CF_EFFECTS;	
	if(IDOK == dlg.DoModal())
	{
		SoftInfo::CSoftInfo::GetMe().setTuopuFont(*dlg.m_cf.lpLogFont);
	}
}

void SoftInfo::CSoftSetTuoPuDlg::OnOK()
{
	CWnd* pWnd = GetParent();
	if(!pWnd)										return;
	pWnd->PostMessage(MESSAGE_OK, 0, 0);
}

BOOL SoftInfo::CSoftSetTuoPuDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("����ͼ��ʾ����"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
