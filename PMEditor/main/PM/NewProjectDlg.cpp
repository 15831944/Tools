// NewProjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PMApp.h"
#include "NewProjectDlg.h"

#include <afxdialogex.h>
#include <io.h>

#include "Gbl.h"
#include "MyHelp.h"


// CNewProjectDlg �Ի���

IMPLEMENT_DYNAMIC(CNewProjectDlg, CDialogEx)

CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewProjectDlg::IDD, pParent)
{
	m_strProjectName = _T("�½�����");
	m_strProjectPath = _T("");
	m_strProjectAuthor = _T("");
	m_strProjectVersion = _T("1.0");
	m_strProjectDescription = _T("");

	//��ȡ��ǰϵͳ��¼���û���
	if (true)
	{
		BOOL bRet(TRUE);
		DWORD dwSize = MAX_PATH;
		TCHAR * pszName = new TCHAR[dwSize];
		if (!GetUserName(pszName, &dwSize))
		{
			delete[] pszName;
			pszName = new TCHAR[dwSize];
			bRet = GetUserName(pszName, &dwSize);
		}
		m_strProjectAuthor = pszName;
		delete[] pszName;
	}

	//�����ڹ��������б�����ʾ�õ�ͼ���б�
	m_imgList.Create(48, 48, ILC_COLOR32 | ILC_MASK, 2, 1);
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_PROJECT_NATIVE));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_PROJECT_WEB));
}

CNewProjectDlg::~CNewProjectDlg()
{
}

CNewProjectDlg& CNewProjectDlg::GetMe()
{
	static CNewProjectDlg one;
	return one;
}

void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROJECT_NAME, m_strProjectName);
	DDX_Text(pDX, IDC_COMBO_PROJECT_PATH, m_strProjectPath);
	DDX_Control(pDX, IDC_LIST_PROJECT_TYPE, m_ctrlProjectType);
}


BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BROWSE_PATH, &CNewProjectDlg::OnBnClickedBtnBrowsePath)
	ON_BN_CLICKED(IDOK, &CNewProjectDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROJECT_TYPE, &CNewProjectDlg::OnLvnItemchangedListProjectType)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROJECT_TYPE, &CNewProjectDlg::OnNMDblclkListProjectType)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CNewProjectDlg ��Ϣ�������
BOOL CNewProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ʼ�����������б�ؼ�
	m_ctrlProjectType.SetImageList(&m_imgList, LVSIL_NORMAL);

	m_ctrlProjectType.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, 0, _T("����/Web����"), LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, NULL);
	m_ctrlProjectType.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, 1, _T("��ƽ̨����"), 0, 0, 1, NULL);

	POINT pt1 = { 50, 20 };
	m_ctrlProjectType.SetItemPosition(0, pt1);

	POINT pt2 = { 150, 20 };
	m_ctrlProjectType.SetItemPosition(1, pt2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CNewProjectDlg::OnBnClickedBtnBrowsePath()
{
	CXTBrowseDialog dlg(this);
	dlg.SetTitle(_T("��ѡ���ļ���"));
	TCHAR path[_MAX_PATH];
	STRCPY_S(path, _MAX_PATH, m_strProjectPath);
	dlg.SetSelPath(path);
	if (dlg.DoModal() == IDOK)
	{
		m_strProjectPath = dlg.GetSelPath();
		if (m_strProjectPath.GetAt(m_strProjectPath.GetLength() - 1) != '\\')
			m_strProjectPath = m_strProjectPath + _T("\\");
		UpdateData(FALSE);
	}
}

void CNewProjectDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_strProjectName = m_strProjectName.Trim();
	m_strProjectPath = m_strProjectPath.Trim();
	UpdateData(FALSE);

	//�������Ʋ�����Ϊ��
	if (m_strProjectName.IsEmpty())
	{
		AfxMessageBox(_T("������������Ϊ�գ�"));
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_PROJECT_NAME)->SetFocus());
		return;
	}

	//����·��������Ϊ��
	if (m_strProjectPath.IsEmpty())
	{
		AfxMessageBox(_T("����·��������Ϊ�գ�"));
		//GotoDlgCtrl(GetDlgItem(IDC_COMBO_PROJECT_PATH)->SetFocus());
		GetDlgItem(IDC_COMBO_PROJECT_PATH)->SetFocus();
		return;
	}

	//��⹤�������Ƿ�Ϸ�
	if (!CGbl::GetCorrectFileName(m_strProjectName, true)){
		GotoDlgCtrl(GetDlgItem(IDC_PROJECT_NAME));
		return;
	}

	//��⹤��·���Ƿ���Ч
	if (_access((LPCTSTR)(m_strProjectPath + m_strProjectName + _T("\\")), 06) == 0)	//!< ��ͻ
	{
		if (AfxMessageBox("���̳�ͻ���Ƿ񸲸�?\n\r���ǻᶪʧԭ�й��̣�", MB_YESNO) == IDNO)
			return;
	}

	CDialogEx::OnOK();
}

void CNewProjectDlg::OnLvnItemchangedListProjectType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if ((pNMLV->uNewState & LVIS_SELECTED) != 0)
	{
		m_nProjectType = pNMLV->iItem;	//���õ�ǰѡ�еĹ�������
		switch (pNMLV->iItem)
		{
		case 0:
			SetDlgItemText(IDC_TXT_PROJECT_TYPE_DESC, _T("����һ��ͬʱ֧�ֱ��غ�Web��ʽ���е�DView����"));
			break;
		case 1:
			SetDlgItemText(IDC_TXT_PROJECT_TYPE_DESC, _T("����һ��֧�ֿ�ƽ̨���е�DView����"));
			break;
		default:
			break;
		}
	}

	*pResult = 0;
}

void CNewProjectDlg::OnNMDblclkListProjectType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem >= 0)
	{
		this->OnBnClickedOk();
	}

	*pResult = 0;
}

BOOL CNewProjectDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("��������"));

	return CDialog::OnHelpInfo(pHelpInfo);
}
