#include "StdAfx.h"
#include "PMApp.h"
#include "MainFrm.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "DTreeCtrl.h"
#include "ItemMgr.h"
#include "CreatProjectDlg.h"
#include "ReNameDlg.h"

// 打开工程时使用这个构造

const CString PROJ_EXPAND_NAME = _T("dsl");

const CString PROJ = _T("Infomation");
const CString PROJ_NAME = _T("Name");
const CString PROJ_COMPILED = _T("Compiled");
const CString PROJ_AUTHOR = _T("Author");
const CString PROJ_DECSRIP = _T("Description");
const CString PROJ_BUILDTIME = _T("BuildTime");
const CString PROJ_UPDATETIME = _T("UpdateTime");
const CString PROJ_VERSION = _T("Version");
const CString PROJ_TYPE = _T("Type");
const CString ITEM = _T("Item");
const CString ITEM_NAME = _T("Name");
const CString ITEM_PATH = _T("Path");
const CString ITEM_VERSION = _T("Version");
const CString EDIT_TIME = _T("EditTime");

// 新建工程时使用这个构造
CProject::CProject()
:m_bCompiled(false)			// 是否被编译过
{
	m_nProjectType = 0;
	m_bModify = true;
	GetLocalTime(&m_ctBuildTime);
	m_ctUpdateTime = m_ctBuildTime;
	m_strFileName = m_strProjName = m_strPath = m_strDescription = m_strAuthor = m_strVersion = _T("");
}

CProject::CProject(CString name,CString path,CString author,CString description,CString version)
:m_bCompiled(false)			// 是否被编译过
{
	m_nProjectType = 0;
	m_bModify = true;
	GetLocalTime(&m_ctBuildTime);
	m_ctUpdateTime = m_ctBuildTime;
	m_strFileName = m_strProjName = name;
	m_strPath = path;
	m_strDescription = description;
	m_strAuthor = author;
	m_strVersion = version;
}

CProject::CProject(int type, CString name, CString path, CString author, CString description, CString version)
	:m_bCompiled(false)			// 是否被编译过
{
	m_nProjectType = type;
	m_bModify = true;
	GetLocalTime(&m_ctBuildTime);
	m_ctUpdateTime = m_ctBuildTime;
	m_strFileName = m_strProjName = name;
	m_strPath = path;
	m_strDescription = description;
	m_strAuthor = author;
	m_strVersion = version;
}

CProject::~CProject(void)
{
}

bool CProject::IsModify()
{
	if(m_bModify)										return true;
	return false;
}

// 获得文件的全路径路径
CString CProject::GetWholePathName()
{
	return GetPath() + GetProjFileName() + _T(".") + PROJ_EXPAND_NAME;
}

// 创建工程，在这里生成一些工程所必须的文件
bool CProject::CreateProject()
{
	MVC::Item::CItemMgr::GetMe().OnCreate();
	SaveProject();	//新建的工程需要立即保存一下才能算是成功
	SetModify(true);
	return true;
}

// 关闭工程时要处理的事情
bool CProject::OnClose()
{
	MVC::Item::CItemMgr::GetMe().OnClose();
	SetModify(false);
	return true;
}

// 打开一个工程
bool CProject::OpenProject(CString title, CString name,CString pathname)
{
	m_strFileName = title;
	m_strProjName = title;
	m_strPath = CGbl::GetPathFromFilePathName(pathname);
	TiXmlDocument pTiXml((LPCTSTR)pathname);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("工程文件未找到或格式错误！"), _T("错误"), MB_ICONEXCLAMATION);
		return false;
	}
	if(!SerializeXml(pTiXml.RootElement(), true)){
		MessageBox(NULL, _T("工程文件格式错误！"), _T("错误"), MB_ICONEXCLAMATION);
		return false;
	}
	SetModify(false);
	return true;
}

// 保存本工程
bool CProject::SaveProject()
{
	// 保存工程前,如果有内容被修改了,则要设置系统为未编译状态
	if(CProjectMgr::GetMe().IsModify())			SetCompiled(false);

	// 设置一下滚动条
	UINT processlen = 3;
	processlen += MVC::Item::CItemMgr::GetMe().GetItemSize();
	CGbl::SetProgressRange(processlen);

	// 保存前更改一下修改时间，这个时间也可以通过别的来计算，因为保存时也许并没有修改
	if (true)//IsModify())
	{
		CString path = m_strPath + m_strFileName + _T(".") + PROJ_EXPAND_NAME;
		TiXmlDocument pTiXml(path);
		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	// 起始声明
		pTiXml.LinkEndChild(decl);
		TiXmlComment * comment = new TiXmlComment();							// 注释信息
		comment->SetValue(_T("请勿手工编辑此文档"));
		pTiXml.LinkEndChild(comment);
		TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					// 写入根节点
		SerializeXml(pRoot, false);												// 添加工程节点,写操作
		pTiXml.LinkEndChild(pRoot);
		if(!pTiXml.SaveFile()){
			int id = pTiXml.ErrorId();
			CGbl::SetProgressEnd();
			MessageBox(NULL, pTiXml.ErrorDesc(), _T("错误"), MB_ICONEXCLAMATION);
			return false;
		}
	}
	CGbl::SetProgressStep(1);

	MVC::Item::CItemMgr::GetMe().SaveItemFile();
	CGbl::SetProgressEnd();
	SetModify(false);
	return true;
}

// 读写工程文件节点
bool CProject::SerializeXml(TiXmlElement* pNode, bool bRead/* = true*/)
{
	if(bRead){
		// 软件版本
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		CString text, strValue;
		CString itemPath, itemName, itemVer, itemTime;
		while(pAttr)
		{
			text = pAttr->NameTStr().c_str();
			if (PROJ_VERSION == text) m_strVersion = pAttr->Value();
			if (PROJ_TYPE == text) m_nProjectType = atoi(pAttr->Value());
			pAttr = pAttr->Next();
		}
		TiXmlElement* pChild = pNode->FirstChildElement();
		while(pChild)
		{
			text = pChild->ValueStr().c_str();
			if(PROJ == text)
			{
				pAttr = pChild->FirstAttribute();
				while(pAttr)
				{
					text = pAttr->NameTStr().c_str();
					strValue = pAttr->Value();
					if(text == PROJ_COMPILED)				SetCompiled(strValue == _T("1") ? true : false);
					else if(text == PROJ_AUTHOR)			m_strAuthor = strValue;
					else if(text == PROJ_DECSRIP)			m_strDescription = strValue;
					pAttr = pAttr->Next();
				}
			}
			else if(ITEM == text)
			{
				pAttr = pChild->FirstAttribute();
				while(pAttr)
				{
					text = pAttr->NameTStr().c_str();
					strValue = pAttr->Value();
					if(ITEM_NAME == text)					itemName = strValue;
					else if(ITEM_PATH == text)				itemPath = strValue;
					else if(ITEM_VERSION == text)			itemVer = strValue;
					else if(EDIT_TIME == text)				itemTime = strValue;
					pAttr = pAttr->Next();
				}
			}
			pChild = pChild->NextSiblingElement();
		}
		if(!MVC::Item::CItemMgr::GetMe().OpenItemFile(itemName, m_strPath + itemPath, itemVer, itemTime))
			return false;
	}
	else{
		pNode->SetAttribute(_T("FileType"), _T("EditorProjectFile"));
		pNode->SetAttribute(PROJ_TYPE, m_nProjectType);
		pNode->SetAttribute(PROJ_VERSION, GetVersion());
		pNode->SetAttribute(_T("SoftVersion"), _T("1.0"));

		TiXmlElement* pInfo = pNode->AddTiXmlChild((LPCTSTR)PROJ);
		pInfo->SetAttribute(PROJ_COMPILED, IsCompiled() ? _T("1") : _T("0"));
		pInfo->SetAttribute(PROJ_AUTHOR, GetAuthor());
		pInfo->SetAttribute(PROJ_DECSRIP, GetDescription());


		TiXmlElement* pItem = pNode->AddTiXmlChild((LPCTSTR)ITEM);
		MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
		pItem->SetAttribute(ITEM_NAME, itemMgr->getName());
		pItem->SetAttribute(ITEM_PATH, itemMgr->getFileName());
		pItem->SetAttribute(ITEM_VERSION, itemMgr->getVersion());
		pItem->SetAttribute(EDIT_TIME, itemMgr->GetEditTime());
	}
	return true;
}

// 备份工程，备份就是改了地址的保存，保存完再改回来
bool CProject::BackUpProject(CString name,CString path)
{
	CString strOldName = m_strFileName;
	ReName(name);
	CGbl::MakeDir(path);
	if(!SaveProject()){
		AfxMessageBox(_T("备份失败"));
		return false;
	}
	if(!CGbl::CopyDirectoryAll(m_strPath, path))	return false;
	ReName(strOldName);
	SaveProject();
	return true;
}

// 接收回车消息
void CProject::OnTreeEnter(CTreeCtrl* treeCtrl, HTREEITEM item)
{
	OnTreeDblClick(treeCtrl, item);
}

// 接收双击消息
void CProject::OnTreeDblClick(CTreeCtrl* treeCtrl, HTREEITEM item)
{
	if(item == m_hItemItem)				MVC::Item::CItemMgr::GetMe().OpenDoc();
}

// 接收右键消息
void CProject::OnTreeRClick(CTreeCtrl* treeCtrl, HTREEITEM item, CPoint point)
{
	if(item == m_hProjectItem){
		CMenu popMenu;
		popMenu.LoadMenu(IDR_RBUTTON_MENU);
		CMenu* menu =popMenu.GetSubMenu(4);
		menu->EnableMenuItem(ID_PROJ_IN, MF_DISABLED | MF_BYCOMMAND);
		CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_NONOTIFY, point.x, point.y, g_App.GetMainWnd());
	}
}

// 刷新工程树，不用记录折没折叠，全部展开
void CProject::UpdateProjView(Tool::CDTreeCtrl& pTreeCtrl)
{
	switch (m_nProjectType)
	{
	case 0:
		m_hProjectItem = pTreeCtrl.InsertItem(_T("本地/Web工程(") + m_strProjName + _T(")"), 1, 1);
		break;
	case 1:
		m_hProjectItem = pTreeCtrl.InsertItem(_T("跨平台工程(") + m_strProjName + _T(")"), 1, 1);
		break;
	default:
		m_hProjectItem = pTreeCtrl.InsertItem(_T("工程(") + m_strProjName + _T(")"), 1, 1);
		break;
	}
	m_hItemItem = pTreeCtrl.InsertItem(_T("变量表"), 4, 4, m_hProjectItem);

	pTreeCtrl.Expand(m_hProjectItem, TVM_EXPAND);
	pTreeCtrl.Expand(m_hItemItem, TVM_EXPAND);
}

// 工程要重命名了
bool CProject::OnReName()
{
	Dialog::CReNameDlg dlg;
	dlg.SetOldName(GetProjName());
	if(IDOK == dlg.DoModal())
	{
		if(dlg.GetNewName() == GetProjName() || dlg.GetNewName().Trim() == _T(""))
			return false;
		ReName(dlg.GetNewName());
		return true;
	}
	return false;
}

// 将工程定义为这个名字
void CProject::ReName(CString nameNew)
{
	CString oldfilename = m_strPath + m_strFileName + _T(".") + PROJ_EXPAND_NAME;
	CString newfilename = m_strPath + nameNew.Trim() + _T(".") + PROJ_EXPAND_NAME;
	::rename(oldfilename, newfilename);
	m_strFileName = m_strProjName = nameNew.Trim();
	SetModify(true);
}

void CProject::ShowInfo()
{
	CString pathonly = m_strPath;
	Dialog::CCreatProjectDlg *dlg = &Dialog::CCreatProjectDlg::GetMe(false);
	dlg->SetType(m_nProjectType);
	dlg->SetName(m_strProjName);
	dlg->SetPathOnly(pathonly);
	dlg->SetAuthor(m_strAuthor);
	dlg->SetDescription(m_strDescription);
	dlg->SetVersion(m_strVersion);
	if(dlg->DoModal()){
		if (m_strAuthor != dlg->GetAuthor() ||
			m_strDescription != dlg->GetDescription() ||
			m_strVersion != dlg->GetVersion())
			SetModify(true);
		m_strAuthor = dlg->GetAuthor();
		m_strDescription = dlg->GetDescription();
		m_strVersion = dlg->GetVersion();
	}
}

// 点击F1帮助
void CProject::OnHelpInfo(HTREEITEM hItem)
{
	if(hItem == NULL)	return;

	SoftInfo::CMyHelp* pHelp = &SoftInfo::CMyHelp::GetMe();
	if(hItem == m_hProjectItem)			pHelp->ShowHelp(_T("工程"));				// 工程在树中的节点
	else if(hItem == m_hItemItem)		pHelp->ShowHelp(_T("变量"));				// 变量在树中的节点
}
