#include "StdAfx.h"
#include "PMApp.h"
#include "MainFrm.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "Compiler.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "ServerCtrl.h"

const UINT COMPILE_START = 2001;
const UINT COMPILE_MESSAGE = 2002;
const UINT COMPILE_END = 2003;
const UINT COMPILE_SCANEND = 2004;

const CString COMPILE = _T("Compile");
const CString COMPILE_NAME = _T("Name");
const CString COMPILE_PATH = _T("Path");
const CString COMPILE_VERSION = _T("Version");

using namespace Servers;
using namespace Compile;
CCompiler::CCompiler(void)
:m_bCompiling(false)
{
}

CCompiler::~CCompiler(void)
{
}

CCompiler& CCompiler::GetMe()
{
	static CCompiler one;
	return one;
}

//!< ���빤��
void CCompiler::CompileProj()
{
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)					return;
	if(IsCompiling())			return;

	CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
	UINT uiH = (UINT)mf->GetSafeHwnd();
	CString strHandle;
	strHandle.Format("%u", uiH);
	CString ext = _T(".cpl");
	if(!SoftInfo::CSoftInfo::GetMe().IsCplXml())	ext += _T("x");

	CString projPathName = proj->GetWholePathName();
	CString xmlMgrPathName = CGbl::GetMe().getDataPath() + _T("��������.ddm");
	CString cplPathName = projPathName.Left(projPathName.ReverseFind('.')) + ext;
	CString strCmd = _T("c:\\CommCompl.exe");
	CString strMax;
	strMax.Format(_T("%d"), MAX_ITEM_COUNT);
	strCmd = strCmd + _T(" -l ") + strMax;
	strCmd = strCmd + _T(" -b /H") + strHandle + _T("/") + projPathName + _T("/") + xmlMgrPathName + _T("/")+ cplPathName + _T("/");

	CompileInfo(strCmd);
	SetCompiling(true);
}

//!< ��������,�������ݸ���cmdLine�Ĳ�ͬ����ͬ
void CCompiler::CompileInfo(CString cmdLine)
{
	CString exePath = CGbl::GetMe().getExePath();							//!< ����װ·��
	CString exePathName = exePath + CGbl::GetMe().getCompilerName();		//!< ������·������
	ShellExecute(NULL, "open", exePathName, cmdLine, exePath, SW_HIDE);		//!< �����������
}

//!< ���õ�ǰ������״̬
void CCompiler::SetCompiling(bool b)
{
	if(!m_bCompiling && b)
	{	//!< ����ǴӲ����뵽������ת,����Ҫ����һ����ʱ
		g_App.GetMainWnd()->SetTimer(COMPILE_OVERTIME_ID, SoftInfo::CSoftInfo::GetMe().getCompileOverTime(), NULL);
	}
	else if(!b)
	{	//!< ���������Ϊ������״̬,��ȡ�������ʱ
		g_App.GetMainWnd()->KillTimer(COMPILE_OVERTIME_ID);
	}
	m_bCompiling = b;
}

bool CCompiler::IsINeed(UINT dwData)
{
	if(dwData == COMPILE_START)		return true;
	if(dwData == COMPILE_MESSAGE)	return true;
	if(dwData == COMPILE_END)		return true;
	if(dwData == COMPILE_SCANEND)	return true;
	return false;
}

void CCompiler::OnReceive(COPYDATASTRUCT* pCopyDataStruct)
{
	char *data = NULL;
	CString msg;
	bool bCompiled = false;
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	switch(pCopyDataStruct->dwData)
	{
	case COMPILE_MESSAGE:			//!< ��������ӡ������
		data = new char[pCopyDataStruct->cbData + 1];
		memcpy(data, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		data[pCopyDataStruct->cbData] = 0;
		msg = data;
		CGbl::PrintOut(msg, 0);
		delete[] data;
		break;

	case COMPILE_END:				//!< ���������������־
		data = new char[pCopyDataStruct->cbData];
		memcpy(data, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		bCompiled = data[0] == '0' ? true : false;
		//!< �����ǰ������û�������������з���
		if(proj)					proj->SetCompiled(bCompiled);
		OnRunObj();
		m_ltNeedRunObj.clear();
		SetCompiling(false);
		delete[] data;
		break;
	}
}

//!< ��Ӵ�������
void CCompiler::AddRunObj(SNeedRunObj* obj)
{
	if(!obj)										return;
	m_ltNeedRunObj.remove(obj);
	m_ltNeedRunObj.push_back(obj);
}

//!< ���������,�������������е��ⲿģ����
void CCompiler::OnRunObj()
{
	if(m_ltNeedRunObj.empty())						return;
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)										return;
	if(!proj->IsCompiled())							//!< �������ûͨ��,����Ҫ��ʾ,��������������
	{
		CString strOut;
		for (SNeedRunObj* nro : m_ltNeedRunObj)
		{
			if(strOut == _T(""))					strOut = _T(" '") + nro->GetTitle() + _T("'");
			else									strOut = strOut + _T(" �� '") + nro->GetTitle() + _T("'");
		}
		strOut = _T("����δͨ��,�޷�������������") + strOut + _T(".");
		// DXP��HMIӦ�ò�ͬ�Դ�,����Ҫ�������⴦��,����ô�����е����
		MessageBox(g_App.GetMainWnd()->GetSafeHwnd(), strOut, _T("��ʾ"), MB_OK | MB_ICONWARNING);
		return;
	}

	//!< �������д����е��ⲿģ��
	//SNeedRunObj* obj;
	for (auto obj : m_ltNeedRunObj)
		obj->OnRun();
}

bool CCompiler::SerializeXml(TiXmlElement* pNode, bool bRead)		//!< ����xml��ʽ�ı༭��Ϣ
{
	if(!bRead)
	{
		TiXmlElement* pChild = pNode->AddTiXmlChild((LPCTSTR)COMPILE);
		CString ext = _T(".cpl");
		if(!SoftInfo::CSoftInfo::GetMe().IsCplXml())	ext += _T("x");
		CString pathName = CProjectMgr::GetMe().GetProj()->GetProjName() + ext;
		pChild->SetAttribute(COMPILE_NAME, _T("�����ļ�"));
		pChild->SetAttribute(COMPILE_PATH, pathName);
		pChild->SetAttribute(COMPILE_VERSION, _T("1.0"));
	}
	return true;
}

//!< �����볬ʱʱ
void CCompiler::OnCompileOverTime()
{
	if(!IsCompiling())		return;		//!< ����Ѿ��������,�˳�
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(proj)				proj->SetCompiled(false);
	SetCompiling(false);
	CGbl::PrintOut(_T("���볬ʱ"));
	m_ltNeedRunObj.clear();
	MessageBox(g_App.GetMainWnd()->GetSafeHwnd(), _T("���볬ʱ"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
}
