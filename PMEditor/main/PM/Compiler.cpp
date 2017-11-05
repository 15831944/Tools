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

//!< 编译工程
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
	CString xmlMgrPathName = CGbl::GetMe().getDataPath() + _T("描述管理.ddm");
	CString cplPathName = projPathName.Left(projPathName.ReverseFind('.')) + ext;
	CString strCmd = _T("c:\\CommCompl.exe");
	CString strMax;
	strMax.Format(_T("%d"), MAX_ITEM_COUNT);
	strCmd = strCmd + _T(" -l ") + strMax;
	strCmd = strCmd + _T(" -b /H") + strHandle + _T("/") + projPathName + _T("/") + xmlMgrPathName + _T("/")+ cplPathName + _T("/");

	CompileInfo(strCmd);
	SetCompiling(true);
}

//!< 启动编译,编译内容根据cmdLine的不同而不同
void CCompiler::CompileInfo(CString cmdLine)
{
	CString exePath = CGbl::GetMe().getExePath();							//!< 程序安装路径
	CString exePathName = exePath + CGbl::GetMe().getCompilerName();		//!< 编译器路径名字
	ShellExecute(NULL, "open", exePathName, cmdLine, exePath, SW_HIDE);		//!< 启动编译程序
}

//!< 设置当前按编译状态
void CCompiler::SetCompiling(bool b)
{
	if(!m_bCompiling && b)
	{	//!< 如果是从不编译到编译跳转,则需要设置一个超时
		g_App.GetMainWnd()->SetTimer(COMPILE_OVERTIME_ID, SoftInfo::CSoftInfo::GetMe().getCompileOverTime(), NULL);
	}
	else if(!b)
	{	//!< 如果是设置为不编译状态,则取消这个超时
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
	case COMPILE_MESSAGE:			//!< 编译器打印的数据
		data = new char[pCopyDataStruct->cbData + 1];
		memcpy(data, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		data[pCopyDataStruct->cbData] = 0;
		msg = data;
		CGbl::PrintOut(msg, 0);
		delete[] data;
		break;

	case COMPILE_END:				//!< 编译器编译结束标志
		data = new char[pCopyDataStruct->cbData];
		memcpy(data, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		bCompiled = data[0] == '0' ? true : false;
		//!< 如果当前服务器没有启动，则不运行服务
		if(proj)					proj->SetCompiled(bCompiled);
		OnRunObj();
		m_ltNeedRunObj.clear();
		SetCompiling(false);
		delete[] data;
		break;
	}
}

//!< 添加待启动项
void CCompiler::AddRunObj(SNeedRunObj* obj)
{
	if(!obj)										return;
	m_ltNeedRunObj.remove(obj);
	m_ltNeedRunObj.push_back(obj);
}

//!< 编译结束了,可以启动待运行的外部模块了
void CCompiler::OnRunObj()
{
	if(m_ltNeedRunObj.empty())						return;
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)										return;
	if(!proj->IsCompiled())							//!< 如果编译没通过,则需要提示,并不再启动其它
	{
		CString strOut;
		for (SNeedRunObj* nro : m_ltNeedRunObj)
		{
			if(strOut == _T(""))					strOut = _T(" '") + nro->GetTitle() + _T("'");
			else									strOut = strOut + _T(" 和 '") + nro->GetTitle() + _T("'");
		}
		strOut = _T("编译未通过,无法启动其它程序：") + strOut + _T(".");
		// DXP和HMI应该不同对待,所以要增加特殊处理,但这么处理有点恶心
		MessageBox(g_App.GetMainWnd()->GetSafeHwnd(), strOut, _T("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	//!< 启动所有待运行的外部模块
	//SNeedRunObj* obj;
	for (auto obj : m_ltNeedRunObj)
		obj->OnRun();
}

bool CCompiler::SerializeXml(TiXmlElement* pNode, bool bRead)		//!< 保存xml格式的编辑信息
{
	if(!bRead)
	{
		TiXmlElement* pChild = pNode->AddTiXmlChild((LPCTSTR)COMPILE);
		CString ext = _T(".cpl");
		if(!SoftInfo::CSoftInfo::GetMe().IsCplXml())	ext += _T("x");
		CString pathName = CProjectMgr::GetMe().GetProj()->GetProjName() + ext;
		pChild->SetAttribute(COMPILE_NAME, _T("编译文件"));
		pChild->SetAttribute(COMPILE_PATH, pathName);
		pChild->SetAttribute(COMPILE_VERSION, _T("1.0"));
	}
	return true;
}

//!< 当编译超时时
void CCompiler::OnCompileOverTime()
{
	if(!IsCompiling())		return;		//!< 如果已经编译完毕,退出
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(proj)				proj->SetCompiled(false);
	SetCompiling(false);
	CGbl::PrintOut(_T("编译超时"));
	m_ltNeedRunObj.clear();
	MessageBox(g_App.GetMainWnd()->GetSafeHwnd(), _T("编译超时"), _T("提示"), MB_OK | MB_ICONINFORMATION);
}
