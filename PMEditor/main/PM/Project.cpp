#include "StdAfx.h"
#include "PMApp.h"
#include "MainFrm.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "ServerCtrl.h"
#include "Project.h"
#include "DTreeCtrl.h"
#include "ItemMgr.h"
#include "DevMgr.h"
#include "DBMgr.h"
#include "Compiler.h"
#include "CreatProjectDlg.h"
#include "ReNameDlg.h"
#include "Hmi.h"

//!< �򿪹���ʱʹ���������

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
const CString DEVICE = _T("DeviceMap");
const CString DEVICE_NAME = _T("Name");
const CString DEVICE_PATH = _T("Path");
const CString DEVICE_VERSION = _T("Version");
const CString DBSET = _T("DBSet");

//!< �½�����ʱʹ���������
CProject::CProject()
:m_bCompiled(false)			//!< �Ƿ񱻱����
{
	m_nProjectType = 0;
	m_bModify = true;
	m_bHmiModify = false;
	GetLocalTime(&m_ctBuildTime);
	m_ctUpdateTime = m_ctBuildTime;
	m_strFileName = m_strProjName = m_strPath = m_strDescription = m_strAuthor = m_strVersion = _T("");
}

CProject::CProject(CString name,CString path,CString author,CString description,CString version)
:m_bCompiled(false)			//!< �Ƿ񱻱����
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
	:m_bCompiled(false)			//!< �Ƿ񱻱����
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
	if(Servers::DB::CDBMgr::GetMe().IsModify())			return true;
	return false;
}

//!< ����ļ���ȫ·��·��
CString CProject::GetWholePathName()
{
	return GetPath() + GetProjFileName() + _T(".") + PROJ_EXPAND_NAME;
}

//!< �������̣�����������һЩ������������ļ�
bool CProject::CreateProject()
{
	MVC::Item::CItemMgr::GetMe().OnCreate();
	MVC::Device::CDevMgr::GetMe().OnCreate();
	Servers::DB::CDBMgr::GetMe().setBackupPath(m_strPath);
	SaveProject();	//�½��Ĺ�����Ҫ��������һ�²������ǳɹ�
	SetModify(true);
	MVC::Device::CDevMgr::GetMe().OpenDoc();		// �򿪹���֮��,ֱ�Ӵ�����ͼ
	return true;
}

//!< �رչ���ʱҪ���������
bool CProject::OnClose()
{
	MVC::Item::CItemMgr::GetMe().OnClose();
	MVC::Device::CDevMgr::GetMe().OnClose();
	Servers::DB::CDBMgr::GetMe().OnClose();
	SetModify(false);
	return true;
}

//!< ��һ������
bool CProject::OpenProject(CString title, CString name,CString pathname)
{
	m_strFileName = title;
	m_strProjName = title;
	m_strPath = CGbl::GetPathFromFilePathName(pathname);
	TiXmlDocument pTiXml((LPCTSTR)pathname);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("�����ļ�δ�ҵ����ʽ����"), _T("����"), MB_ICONEXCLAMATION);
		return false;
	}
	if(!SerializeXml(pTiXml.RootElement(), true)){
		MessageBox(NULL, _T("�����ļ���ʽ����"), _T("����"), MB_ICONEXCLAMATION);
		return false;
	}
	SetModify(false);
	MVC::Device::CDevMgr::GetMe().OpenDoc();		// �򿪹���֮��,ֱ�Ӵ�����ͼ
	return true;
}

//!< ���汾����
bool CProject::SaveProject()
{
	//!< ���湤��ǰ,��������ݱ��޸���,��Ҫ����ϵͳΪδ����״̬
	if(CProjectMgr::GetMe().IsModify())			SetCompiled(false);

	//!< ����һ�¹�����
	UINT processlen = 3 + MVC::Device::CDevMgr::GetMe().GetDeviceSize();
	processlen += MVC::Item::CItemMgr::GetMe().GetItemSize();
	CGbl::SetProgressRange(processlen);

	//!< ����ǰ����һ���޸�ʱ�䣬���ʱ��Ҳ����ͨ����������㣬��Ϊ����ʱҲ��û���޸�
	if (true)//IsModify())
	{
		CString path = m_strPath + m_strFileName + _T(".") + PROJ_EXPAND_NAME;
		TiXmlDocument pTiXml(path);
		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< ��ʼ����
		pTiXml.LinkEndChild(decl);
		TiXmlComment * comment = new TiXmlComment();							//!< ע����Ϣ
		comment->SetValue(_T("�����ֹ��༭���ĵ�"));
		pTiXml.LinkEndChild(comment);
		TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					//!< д����ڵ�
		SerializeXml(pRoot, false);												//!< ��ӹ��̽ڵ�,д����
		pTiXml.LinkEndChild(pRoot);
		if(!pTiXml.SaveFile()){
			int id = pTiXml.ErrorId();
			CGbl::SetProgressEnd();
			MessageBox(NULL, pTiXml.ErrorDesc(), _T("����"), MB_ICONEXCLAMATION);
			return false;
		}
	}
	CGbl::SetProgressStep(1);

	MVC::Device::CDevMgr::GetMe().SaveFile();
	MVC::Item::CItemMgr::GetMe().SaveItemFile();
	CGbl::SetProgressEnd();
	SetModify(false);
	return true;
}

//!< ��д�����ļ��ڵ�
bool CProject::SerializeXml(TiXmlElement* pNode, bool bRead/* = true*/)
{
	if(bRead){
		//!< ����汾
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		CString text, strValue;
		CString devPath, devName, devVer, deviceTime;
		CString itemPath, itemName, itemVer, itemTime;
		CString camPath, camName, camVer, camActive;
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
			else if(DEVICE == text)
			{
				pAttr = pChild->FirstAttribute();
				CString path,name,ver;
				while(pAttr)
				{
					text = pAttr->NameTStr().c_str();
					strValue = pAttr->Value();
					if(DEVICE_NAME == text)					devName = strValue;
					else if(DEVICE_PATH == text)			devPath = strValue;
					else if(DEVICE_VERSION == text)			devVer = strValue;
					else if(EDIT_TIME == text)				deviceTime =strValue;
					pAttr = pAttr->Next();
				}
			}
			else if(DBSET == text)
			{
				Servers::DB::CDBMgr::GetMe().SerializeXml(pChild, bRead);
			}
			pChild = pChild->NextSiblingElement();
		}
		if(!MVC::Device::CDevMgr::GetMe().OpenDevMgrFile(devName, m_strPath + devPath, devVer, deviceTime))
			return false;
		if(!MVC::Item::CItemMgr::GetMe().OpenItemFile(itemName, m_strPath + itemPath, itemVer, itemTime))
			return false;
	}
	else{
		pNode->SetAttribute(_T("FileType"), _T("EditorProjectFile"));
		pNode->SetAttribute(PROJ_TYPE, m_nProjectType);
		pNode->SetAttribute(PROJ_VERSION, GetVersion());
		pNode->SetAttribute(_T("SoftVersion"), _T("1.0"));

		TiXmlElement* pInfo = pNode->AddTiXmlChild((LPCTSTR)PROJ);
//		pInfo->SetAttribute(PROJ_NAME, GetProjName());
		pInfo->SetAttribute(PROJ_COMPILED, IsCompiled() ? _T("1") : _T("0"));
		pInfo->SetAttribute(PROJ_AUTHOR, GetAuthor());
		pInfo->SetAttribute(PROJ_DECSRIP, GetDescription());

		TiXmlElement* pDevice = pNode->AddTiXmlChild((LPCTSTR)DEVICE);
		MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
		pDevice->SetAttribute(DEVICE_NAME, devMgr->getName());
		pDevice->SetAttribute(DEVICE_PATH, devMgr->getFileName());
		pDevice->SetAttribute(DEVICE_VERSION, devMgr->getVersion());
		pDevice->SetAttribute(EDIT_TIME, devMgr->GetEditTime());

		TiXmlElement* pItem = pNode->AddTiXmlChild((LPCTSTR)ITEM);
		MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
		pItem->SetAttribute(ITEM_NAME, itemMgr->getName());
		pItem->SetAttribute(ITEM_PATH, itemMgr->getFileName());
		pItem->SetAttribute(ITEM_VERSION, itemMgr->getVersion());
		pItem->SetAttribute(EDIT_TIME, itemMgr->GetEditTime());

		TiXmlElement* pDBSet = pNode->AddTiXmlChild((LPCTSTR)DBSET);
		Servers::DB::CDBMgr::GetMe().SerializeXml(pDBSet, bRead);

		Servers::Compile::CCompiler::GetMe().SerializeXml(pNode, bRead);
	}
	return true;
}

//!< ���ݹ��̣����ݾ��Ǹ��˵�ַ�ı��棬�������ٸĻ���
bool CProject::BackUpProject(CString name,CString path)
{
	CString strOldName = m_strFileName;
	ReName(name);
	CGbl::MakeDir(path);
	if(!SaveProject()){
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}
	if(!CGbl::CopyDirectoryAll(m_strPath, path))	return false;
	ReName(strOldName);
	SaveProject();
	//rename(path + m_strFileName + _T(".") + PROJ_EXPAND_NAME, path + name + _T(".") + PROJ_EXPAND_NAME);
	return true;
}

//!< ���ջس���Ϣ
void CProject::OnTreeEnter(CTreeCtrl* treeCtrl, HTREEITEM item)
{
	OnTreeDblClick(treeCtrl, item);
}

//!< ����˫����Ϣ
void CProject::OnTreeDblClick(CTreeCtrl* treeCtrl, HTREEITEM item)
{
	if(item == m_hItemItem)				MVC::Item::CItemMgr::GetMe().OpenDoc();
	else if(item == m_hDeviceItem)		MVC::Device::CDevMgr::GetMe().OpenDoc();
	else if(item == m_hDBItem)			Servers::DB::CDBMgr::GetMe().OnSetDB();
}

//!< �����Ҽ���Ϣ
void CProject::OnTreeRClick(CTreeCtrl* treeCtrl, HTREEITEM item, CPoint point)
{
	if(item == m_hProjectItem){
		CMenu popMenu;
		popMenu.LoadMenu(IDR_RBUTTON_MENU);
		CMenu* menu =popMenu.GetSubMenu(4);
		menu->EnableMenuItem(ID_PROJ_IN, MF_DISABLED | MF_BYCOMMAND);
		CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_NONOTIFY, point.x, point.y, g_App.GetMainWnd());
	}
	else if(item == m_hDeviceItem){
		CMenu popMenu;
		popMenu.LoadMenu(IDR_RBUTTON_MENU);
		CMenu* menu =popMenu.GetSubMenu(8);
		if(CProjectMgr::GetMe().IsWatch())
			menu->EnableMenuItem(ID_SCANANDSHOW, MF_DISABLED | MF_BYCOMMAND);
		CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_NONOTIFY, point.x, point.y, g_App.GetMainWnd());
	}
}

//!< ˢ�¹����������ü�¼��û�۵���ȫ��չ��
void CProject::UpdateProjView(Tool::CDTreeCtrl& pTreeCtrl)
{
	switch (m_nProjectType)
	{
	case 0:
		m_hProjectItem = pTreeCtrl.InsertItem(_T("����/Web����(") + m_strProjName + _T(")"), 1, 1);
		break;
	case 1:
		m_hProjectItem = pTreeCtrl.InsertItem(_T("��ƽ̨����(") + m_strProjName + _T(")"), 1, 1);
		break;
	default:
		m_hProjectItem = pTreeCtrl.InsertItem(_T("����(") + m_strProjName + _T(")"), 1, 1);
		break;
	}
	m_hDeviceItem = pTreeCtrl.InsertItem(_T("�豸���˽ṹ"), 17, 17, m_hProjectItem);
	m_hItemItem = pTreeCtrl.InsertItem(_T("������"), 4, 4, m_hProjectItem);
	m_hDBItem = pTreeCtrl.InsertItem(_T("��ʷ���ݿ�����"), 16, 16, m_hProjectItem);

	pTreeCtrl.Expand(m_hProjectItem, TVM_EXPAND);
	pTreeCtrl.Expand(m_hItemItem, TVM_EXPAND);
}

//!< ����Ҫ��������
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

//!< �����̶���Ϊ�������
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

//!< ���F1����
void CProject::OnHelpInfo(HTREEITEM hItem)
{
	if(hItem == NULL)	return;

	SoftInfo::CMyHelp* pHelp = &SoftInfo::CMyHelp::GetMe();
	if(hItem == m_hProjectItem)			pHelp->ShowHelp(_T("����"));				//!< ���������еĽڵ�
	else if(hItem == m_hDeviceItem)		pHelp->ShowHelp(_T("�豸����"));			//!< �豸���������еĽڵ�
	else if(hItem == m_hItemItem)		pHelp->ShowHelp(_T("����"));				//!< ���������еĽڵ�
	else if(hItem == m_hDBItem)			pHelp->ShowHelp(_T("��ʷ���ݿ�"));			//!< ���ݿ����ýڵ�
}
