// CloneItemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "SoftInfo.h"
#include "CloneItemDlg.h"
#include "ProcessDlg.h"

#include "ItemMgr.h"
#include "Item.h"
#include "ItemGroup.h"

// CCloneItemDlg �Ի���
using namespace Dialog;
IMPLEMENT_DYNAMIC(CCloneItemDlg, CDialog)

CCloneItemDlg::CCloneItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCloneItemDlg::IDD, pParent)
	, m_uiCloneID(0)
	, m_strCloneName(_T(""))
	, m_uiCloneNum(0)
	, m_uiBaseNum(0)
	, m_uiBaseFlt(16)
	, m_uiAddrUnit(1)		//!< IO������ַ���
	, m_nRadioType(1)
	, m_pProcessDlg(NULL)	// ������ʾ���ȵĶԻ���
{
	m_bShowHelp = false;
	m_bIsSeries = FALSE;
}

CCloneItemDlg::~CCloneItemDlg()
{
}

CCloneItemDlg& CCloneItemDlg::GetMe()
{
	static CCloneItemDlg one;
	return one;
}

void CCloneItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLONE_ID, m_uiCloneID);
	DDX_Text(pDX, IDC_CLONE_NAME, m_strCloneName);
	DDX_Text(pDX, IDC_CLONE_NUM, m_uiCloneNum);
	DDX_Text(pDX, IDC_CLONE_NUMBER, m_uiBaseNum);
	DDX_Text(pDX, IDC_CLONE_FLOAT, m_uiBaseFlt);
	DDX_Text(pDX, IDC_ADDR_UNIT, m_uiAddrUnit);
	DDX_Radio(pDX, IDC_RADIO_NUMBER, m_nRadioType);
}

BEGIN_MESSAGE_MAP(CCloneItemDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CCloneItemDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCloneItemDlg::OnBnClickedOk)
//	ON_BN_CLICKED(ID_SHOW_CLONEINFO, &CCloneItemDlg::OnBnClickedShowCloneinfo)
	ON_BN_CLICKED(IDC_RADIO_NUMBER, &CCloneItemDlg::OnBnClickedRadioNumber)
	ON_BN_CLICKED(IDC_RADIO_FLOAT, &CCloneItemDlg::OnBnClickedRadioFloat)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CCloneItemDlg ��Ϣ�������
INT_PTR CCloneItemDlg::DoModal(UINT id)
{
	m_uiCloneID = id;
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<MVC::Item::CItem> item = mgr->GetItem(id);
	ASSERT(item);
	m_strCloneName = item->getName();
	return CDialog::DoModal();
}

BOOL CCloneItemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ltItemID.clear();
	m_bShowHelp = !m_bShowHelp;
	m_uiBaseNum = SoftInfo::CSoftInfo::GetMe().getCloneNumber();
	m_uiBaseFlt = SoftInfo::CSoftInfo::GetMe().getCloneFloat();
	UpdateData(FALSE);
	if(m_nRadioType == 0)		OnBnClickedRadioNumber();
	else						OnBnClickedRadioFloat();

	GotoDlgCtrl(GetDlgItem(IDC_CLONE_NUM)->SetFocus());
	CString text = _T("���ظ�����ʾ��\r\n1.������������������Ϊa0�������2������ظ��Ƴ����ı�����Ϊa2��a4��a6������\r\n2.�����λ��\
���������Ϊb1.13����λ��16������ظ��Ƴ����ı�����Ϊb1.14��b1.15��b2.0����");
	GetDlgItem(IDC_CLONE_HELP)->SetWindowText(text);

	UpdateData(FALSE);
	return TRUE;
}

void CCloneItemDlg::OnBnClickedCancel()
{
	OnCancel();
}

UINT CloneThread(LPVOID pParam)
{
	CCloneItemDlg *dlg = (CCloneItemDlg *)pParam;
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<MVC::Item::CItem> item = mgr->GetItem(dlg->m_uiCloneID);
	ASSERT(item);
	while (dlg->m_pProcessDlg == NULL)		Sleep(1);	// �ȵ��Ի�������
	while (!dlg->m_pProcessDlg->m_bReady)	Sleep(1);	// �ȵ��Ի���׼����
	Sleep(100);
	::SendMessage(dlg->m_pProcessDlg->GetSafeHwnd(), WM_PROCESS_RANGE, (WPARAM)0, (LPARAM)(dlg->m_uiCloneNum - 1));
	int maxID = MAX_ITEM_COUNT;
	std::shared_ptr<MVC::Item::CItem> newItem;
	for (int i = 0; i < dlg->m_uiCloneNum; ++i)
	{
		if (dlg->m_pProcessDlg->m_bStop)	break;
		newItem = std::shared_ptr<MVC::Item::CItem>(new MVC::Item::CItem(_T("")));
		if (!item->OnCloneMe(*newItem, i + 1, dlg->m_uiAddrUnit, dlg->m_nRadioType))
			continue;			//!< �������û��¡�ɹ����Ϳ�¡��һ��
		if (!mgr->AddItemBack(newItem, maxID, item->getMyGroupID()))	// AddItemBack��ӵ��ٶȱȽϿ�
		{
			i = dlg->m_uiCloneNum;	//!< �������Ͳ��ټ���������
		}
		else
		{
			dlg->m_ltItemID.push_back(newItem->getID());
		}
		::SendMessage(dlg->m_pProcessDlg->GetSafeHwnd(), WM_PROCESS_POS, (WPARAM)i, (LPARAM)0);
	}
	::SendMessage(dlg->m_pProcessDlg->GetSafeHwnd(), WM_PROCESS_DONE, (WPARAM)0, (LPARAM)0);
	return 0;
}

void CCloneItemDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_uiBaseNum < 1)		m_uiBaseNum = 1;
	if(m_uiBaseNum > 64)	m_uiBaseNum = 64;
	if(m_uiBaseFlt < 1)		m_uiBaseFlt = 1;
	if(m_uiBaseFlt > 64)	m_uiBaseFlt = 64;
	SoftInfo::CSoftInfo::GetMe().setCloneNumber(m_uiBaseNum);
	SoftInfo::CSoftInfo::GetMe().setCloneFloat(m_uiBaseFlt);
	//int maxCloneNum = 512;
	int maxCloneNum = 204800;
	if(m_uiCloneNum > maxCloneNum)
	{
		CString strError;
		strError.Format(_T("���ظ����������ܶ���%d��"), maxCloneNum);
		MessageBox(strError, _T("����"), MB_ICONEXCLAMATION);
		m_uiCloneNum = maxCloneNum;
		UpdateData(FALSE);
		GotoDlgCtrl(GetDlgItem(IDC_CLONE_NUM)->SetFocus());
		return;
	}
	else if(m_uiAddrUnit > 512)
	{
		MessageBox(_T("��ַ��������ܶ���512��"), _T("����"), MB_ICONEXCLAMATION);
		m_uiAddrUnit = 1;
		UpdateData(FALSE);
		GotoDlgCtrl(GetDlgItem(IDC_ADDR_UNIT)->SetFocus());
		return;
	}
	else if(m_uiCloneNum == 0 || m_uiAddrUnit == 0)
	{
		OnCancel();
		return;
	}
	MVC::Item::CItemMgr::GetMe().RemoveItemBackEmpty();
	m_pProcessDlg = NULL;
	CWinThread* pCloneThread = ::AfxBeginThread(CloneThread, this, THREAD_PRIORITY_HIGHEST, 0, CREATE_SUSPENDED);
	//pCloneThread->m_bAutoDelete = FALSE;
	pCloneThread->ResumeThread();

	m_pProcessDlg = new CProcessDlg(this);	// ������ʾ���ȵĶԻ���
	m_pProcessDlg->DoModal();
	Sleep(10);
	delete m_pProcessDlg;
	m_pProcessDlg = NULL;
	OnOK();
}

void CCloneItemDlg::OnBnClickedRadioNumber()
{
	UpdateData(TRUE);
	if(m_nRadioType == 0)
	{
		GetDlgItem(IDC_CLONE_NUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLONE_FLOAT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CLONE_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLONE_FLOAT)->EnableWindow(TRUE);
	}
}

void CCloneItemDlg::OnBnClickedRadioFloat()
{
	UpdateData(TRUE);
	if(m_nRadioType == 0)
	{
		GetDlgItem(IDC_CLONE_NUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLONE_FLOAT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CLONE_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLONE_FLOAT)->EnableWindow(TRUE);
	}
}


BOOL Dialog::CCloneItemDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("���ظ���"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
