// NewProjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "NewProjectDlg.h"

#include <afxdialogex.h>
#include <io.h>

#include "Gbl.h"
#include "MyHelp.h"


// CNewProjectDlg 对话框

IMPLEMENT_DYNAMIC(CNewProjectDlg, CDialogEx)

CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewProjectDlg::IDD, pParent)
{
	m_strProjectName = _T("新建工程");
	m_strProjectPath = _T("");
	m_strProjectAuthor = _T("");
	m_strProjectVersion = _T("1.0");
	m_strProjectDescription = _T("");

	//获取当前系统登录的用户名
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

	//创建在工程类型列表中显示用的图像列表
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


// CNewProjectDlg 消息处理程序
BOOL CNewProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//初始化工程类型列表控件
	m_ctrlProjectType.SetImageList(&m_imgList, LVSIL_NORMAL);

	m_ctrlProjectType.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, 0, _T("本地/Web工程"), LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, NULL);
	m_ctrlProjectType.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, 1, _T("跨平台工程"), 0, 0, 1, NULL);

	POINT pt1 = { 50, 20 };
	m_ctrlProjectType.SetItemPosition(0, pt1);

	POINT pt2 = { 150, 20 };
	m_ctrlProjectType.SetItemPosition(1, pt2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CNewProjectDlg::OnBnClickedBtnBrowsePath()
{
	CXTBrowseDialog dlg(this);
	dlg.SetTitle(_T("请选择文件夹"));
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

	//工程名称不可以为空
	if (m_strProjectName.IsEmpty())
	{
		AfxMessageBox(_T("工程名不可以为空！"));
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_PROJECT_NAME)->SetFocus());
		return;
	}

	//工程路径不可以为空
	if (m_strProjectPath.IsEmpty())
	{
		AfxMessageBox(_T("工程路径不可以为空！"));
		//GotoDlgCtrl(GetDlgItem(IDC_COMBO_PROJECT_PATH)->SetFocus());
		GetDlgItem(IDC_COMBO_PROJECT_PATH)->SetFocus();
		return;
	}

	//检测工程名称是否合法
	if (!CGbl::GetCorrectFileName(m_strProjectName, true)){
		GotoDlgCtrl(GetDlgItem(IDC_PROJECT_NAME));
		return;
	}

	//检测工程路径是否有效
	if (_access((LPCTSTR)(m_strProjectPath + m_strProjectName + _T("\\")), 06) == 0)	//!< 冲突
	{
		if (AfxMessageBox("工程冲突，是否覆盖?\n\r覆盖会丢失原有工程！", MB_YESNO) == IDNO)
			return;
	}

	CDialogEx::OnOK();
}

void CNewProjectDlg::OnLvnItemchangedListProjectType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if ((pNMLV->uNewState & LVIS_SELECTED) != 0)
	{
		m_nProjectType = pNMLV->iItem;	//设置当前选中的工程类型
		switch (pNMLV->iItem)
		{
		case 0:
			SetDlgItemText(IDC_TXT_PROJECT_TYPE_DESC, _T("创建一个同时支持本地和Web方式运行的DView工程"));
			break;
		case 1:
			SetDlgItemText(IDC_TXT_PROJECT_TYPE_DESC, _T("创建一个支持跨平台运行的DView工程"));
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
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("创建工程"));

	return CDialog::OnHelpInfo(pHelpInfo);
}
