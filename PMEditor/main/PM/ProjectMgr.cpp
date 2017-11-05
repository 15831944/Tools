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

//!< ���������Ƿ񱻸���
bool CProjectMgr::IsModify()
{
	if(!GetProj())										return false;
	if(GetProj()->IsModify())							return true;
	if(MVC::Item::CItemMgr::GetMe().IsModify())			return true;
	return false;
}

//!< �½����̵ĺ������
bool CProjectMgr::NewProject()
{
	//!< �ȹر����й���
	if (!((CMainFrame *)g_App.GetMainWnd())->AskAndProjClose())
		return false;	// �û�����ȡ�����Ͳ��½���

	//!< �����¹��̣��û�����
	CString path = CGbl::GetMe().getProjDefPath();
	CNewProjectDlg* dlg = &CNewProjectDlg::GetMe();
	dlg->SetDefaultPath(path);
	if(dlg->DoModal()!=IDOK)	return false;
	CGbl::MakeDir(dlg->GetPath());

	//!< ��ʼ��������µĹ��̶��󣬴������
	m_CulProject=std::shared_ptr<CProject>(new CProject(dlg->GetType(),dlg->GetName(),dlg->GetPath(),dlg->GetAuthor(),dlg->GetDescription(),dlg->GetVersion()));
	m_CulProject->CreateProject();
	g_App.AddToRecentFileList(dlg->GetPath() + dlg->GetName() + _T(".dsl"));

	//!< ���ˢ�¹�����
	UpdateTreeView();
	return true;
}

void CProjectMgr::EditProjInfo()
{
	if(GetProj())	GetProj()->ShowInfo();
}

//!< �򿪹��̵ĺ�����ڣ�������Ҫ���̾���·����
bool CProjectMgr::OpenProject(CString pathName/* = _T("") */)
{
	//!< �رյ�ǰ������
	if (!((CMainFrame *)g_App.GetMainWnd())->AskAndProjClose())
		return false;	// �û�����ȡ�����Ͳ�����

	CString path, name ,title;
	if(pathName == _T(""))
	{
		//!< ��һ�����̣��û�����
		CString extendName = _T("*.dsl");
		CString extendNames = _T("�����ļ� (") + extendName + _T(")|") + extendName + _T("||");
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

//!< ���湤�̵ĺ������
bool CProjectMgr::SaveProject()
{
	if(m_CulProject){
		//if(!IsModify())		return true;
		return m_CulProject->SaveProject();
	}
	return false;
}

//!< ���ݹ��̵ĺ������
bool CProjectMgr::BackUpProject()
{
	if(!m_CulProject)		return false;
	CString defFileName = m_CulProject->GetProjFileName();
	defFileName = defFileName;//+ _T(" (") + m_CulProject->GetVersion() + _T(")");
	CString extendName = _T("*.dsl");
	CString extendNames = _T("�����ļ� (") + extendName + _T(")|") + extendName + _T("||");
	CFileDialog saveDlg(FALSE, (LPCTSTR)extendName, (LPCTSTR)defFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	if(IDOK == saveDlg.DoModal()){
		CString name = saveDlg.GetFileTitle();
		CGbl::GetCorrectFileName(name);
		CString path = CGbl::GetPathFromFilePathName(saveDlg.GetPathName());
		return m_CulProject->BackUpProject(name, path + name + _T("\\"));
	}
	return false;
}

//!< �رչ��̵ĺ������
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

//!< ˢ�¹������ĺ������
void CProjectMgr::UpdateTreeView()
{
	((CMainFrame *)g_App.GetMainWnd())->UpdateTreeView();
}

//!< ����/ȡ�����
void CProjectMgr::SetWatch(const bool b)
{
	if(!GetProj())		return;		//!< û�й��̲��ܼ��
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

//!< ������״̬���ܲ�������ʾ
bool CProjectMgr::SayWatch()
{
	if(!IsWatch())		return false;
	MessageBox(g_App.GetMainWnd()->GetSafeHwnd(), _T("���״̬���޷����б༭������"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
	return true;
}