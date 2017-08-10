// SoftSetCompileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "SoftInfo.h"
#include "SoftSetCompileDlg.h"


// CSoftSetCompileDlg �Ի���
using namespace SoftInfo;
IMPLEMENT_DYNAMIC(CSoftSetCompileDlg, CDialog)

CSoftSetCompileDlg::CSoftSetCompileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftSetCompileDlg::IDD, pParent)
	, m_bOutPut(FALSE)
	, m_bOutPutWarning(FALSE)
	, m_nCplType(0)
	, m_nScanType(0)
{

}

CSoftSetCompileDlg::~CSoftSetCompileDlg()
{
}

void CSoftSetCompileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ISOUTPUT, m_bOutPut);
	DDX_Check(pDX, IDC_CHECK_ISWARNING, m_bOutPutWarning);
	DDX_Radio(pDX, IDC_RADIO_CPL_BIT, m_nCplType);
	DDX_Radio(pDX, IDC_RADIO_SCAN_BIT, m_nScanType);
}


BEGIN_MESSAGE_MAP(CSoftSetCompileDlg, CDialog)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CSoftSetCompileDlg ��Ϣ��������
BOOL CSoftSetCompileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	m_bOutPut = csi->isOutPut()?1:0;
	m_bOutPutWarning = csi->isOutWarning()?1:0;
	m_nCplType = csi->IsCplXml()?1:0;
	m_nScanType = csi->IsScanXml()?1:0;
	UpdateData(FALSE);
	return TRUE;
}

BOOL CSoftSetCompileDlg::DestroyWindow()
{
	UpdateData(TRUE);
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	csi->setOutPut(m_bOutPut==1?true:false);
	csi->setOutWarning(m_bOutPutWarning==1?true:false);
	csi->setCmpXml(m_nCplType==1?true:false);
	csi->setScanXml(m_nScanType==1?true:false);
	return CDialog::DestroyWindow();
}

void CSoftSetCompileDlg::OnOK()
{
	CWnd* pWnd = GetParent();
	if(!pWnd)										return;
	pWnd->PostMessage(MESSAGE_OK, 0, 0);
}

BOOL SoftInfo::CSoftSetCompileDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("��������"));
	return CDialog::OnHelpInfo(pHelpInfo);
}