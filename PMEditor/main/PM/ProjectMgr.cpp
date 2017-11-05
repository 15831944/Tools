#include "StdAfx.h"
#include "PMApp.h"
#include "MainFrm.h"
#include "SoftInfo.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "CreatProjectDlg.h"
#include "NewProjectDlg.h"
#include "SpaceProject.h"
#include "Project.h"
#include "ItemMgr.h"

CProjectMgr::CProjectMgr(void)
{
}

CProjectMgr::~CProjectMgr(void)
{
}

CProjectMgr& CProjectMgr::GetMe()
{
	static CProjectMgr one;
	return one;
}

//!< 看看工程是否被改了
bool CProjectMgr::IsModify()
{
	if(!GetProj())										return false;
	if(GetProj()->IsModify())							return true;
	if(MVC::Item::CItemMgr::GetMe().IsModify())			return true;
	return false;
}

//!< 新建工程的函数入口
bool CProjectMgr::NewProject()
{
	//!< 先关闭现有工程
	if (!((CMainFrame *)g_App.GetMainWnd())->AskAndProjClose())
		return false;	// 用户点了取消，就不新建了

	//!< 创建新工程，用户操作
	CString path = CGbl::GetMe().getProjDefPath();
	CNewProjectDlg* dlg = &CNewProjectDlg::GetMe();
	dlg->SetDefaultPath(path);
	if(dlg->DoModal()!=IDOK)	return false;
	CGbl::MakeDir(dlg->GetPath());

	//!< 开始创建这个新的工程对象，代码操作
	m_CulProject=std::shared_ptr<CProject>(new CProject(dlg->GetType(),dlg->GetName(),dlg->GetPath(),dlg->GetAuthor(),dlg->GetDescription(),dlg->GetVersion()));
	m_CulProject->CreateProject();
	g_App.AddToRecentFileList(dlg->GetPath() + dlg->GetName() + _T(".dsl"));

	//!< 最后刷新工程树
	UpdateTreeView();
	return true;
}

void CProjectMgr::EditProjInfo()
{
	if(GetProj())	GetProj()->ShowInfo();
}

//!< 打开工程的函数入口，这里需要工程绝对路径名
bool CProjectMgr::OpenProject(CString pathName/* = _T("") */)
{
	//!< 关闭当前工程了
	if (!((CMainFrame *)g_App.GetMainWnd())->AskAndProjClose())
		return false;	// 用户点了取消，就不打开了

	CString path, name ,title;
	if(pathName == _T(""))
	{
		//!< 打开一个工程，用户操作
		CString extendName = _T("*.dsl");
		CString extendNames = _T("工程文件 (") + extendName + _T(")|") + extendName + _T("||");
		CFileDialog openDlg(TRUE,(LPCTSTR)extendName,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,(LPCTSTR)extendNames);

		if(IDOK == openDlg.DoModal()){
			title = openDlg.GetFileTitle();
			name = openDlg.GetFileName();
			path = openDlg.GetPathName();
		}
		else		return false;
	}
	else
	{
		path = CGbl::GetPathFromFilePathName(pathName);
		name = pathName.Right(pathName.GetLength() - path.GetLength());
		title = name.Left(name.ReverseFind('.'));
		path = path + name;
	}

	m_CulProject = std::shared_ptr<CProject>(new CProject());
	if(!m_CulProject->OpenProject(title, name, path)){
		m_CulProject.reset();
		return false;
	}
	else{
		g_App.AddToRecentFileList(path);
	}

	UpdateTreeView();
	return true;
}

//!< 保存工程的函数入口
bool CProjectMgr::SaveProject()
{
	if(m_CulProject){
		//if(!IsModify())		return true;
		return m_CulProject->SaveProject();
	}
	return false;
}

//!< 备份工程的函数入口
bool CProjectMgr::BackUpProject()
{
	if(!m_CulProject)		return false;
	CString defFileName = m_CulProject->GetProjFileName();
	defFileName = defFileName;//+ _T(" (") + m_CulProject->GetVersion() + _T(")");
	CString extendName = _T("*.dsl");
	CString extendNames = _T("工程文件 (") + extendName + _T(")|") + extendName + _T("||");
	CFileDialog saveDlg(FALSE, (LPCTSTR)extendName, (LPCTSTR)defFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	if(IDOK == saveDlg.DoModal()){
		CString name = saveDlg.GetFileTitle();
		CGbl::GetCorrectFileName(name);
		CString path = CGbl::GetPathFromFilePathName(saveDlg.GetPathName());
		return m_CulProject->BackUpProject(name, path + name + _T("\\"));
	}
	return false;
}

//!< 关闭工程的函数入口
bool CProjectMgr::CloseProject()
{
	CPMApp* pApp = (CPMApp *)AfxGetApp();
	pApp->m_pItemDocMgr->CloseAllDocuments(FALSE);
	SetWatch(false);
	if(m_CulProject)
	{
		m_CulProject->OnClose();
		m_CulProject.reset();
	}
	UpdateTreeView();
	CGbl::PrintClear();
	return true;
}

//!< 刷新工程树的函数入口
void CProjectMgr::UpdateTreeView()
{
	((CMainFrame *)g_App.GetMainWnd())->UpdateTreeView();
}

//!< 设置/取消监控
void CProjectMgr::SetWatch(const bool b)
{
	if(!GetProj())		return;		//!< 没有工程不能监控
	if(m_bWatch == b)	return;
	m_bWatch = b;
	MVC::Item::CItemMgr::GetMe().SetItemWatch(b);
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();
	CComVariant cvr = 0;
	if(b)
	{
		mf->InitCommerTime();
		mf->SetTimer(CMainFrame::TIME_WATCH, SoftInfo::CSoftInfo::GetMe().getFreshDataTime(), NULL);
	}
	else
	{
		mf->KillTimer(CMainFrame::TIME_WATCH);
	}

	POSITION pos = g_App.m_pItemDocMgr->GetFirstDocPosition();
	while(pos)		g_App.m_pItemDocMgr->GetNextDoc(pos)->SetTitle(_T(""));
}

//!< 填出监控状态不能操作的提示
bool CProjectMgr::SayWatch()
{
	if(!IsWatch())		return false;
	MessageBox(g_App.GetMainWnd()->GetSafeHwnd(), _T("监控状态下无法进行编辑操作！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
	return true;
}