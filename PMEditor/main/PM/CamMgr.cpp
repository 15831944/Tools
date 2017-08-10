#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "MainFrm.h"
#include "ProjectMgr.h"
#include "Project.h"

#include "CamMgr.h"
#include "CamDoc.h"
#include "Camera.h"
#include "CamView.h"
#include "CamFrame.h"

const CString CAM_EXPAND_NAME = _T("cam");			//!< ��չ��

const CString INFO_HEAD = _T("Infomation");
const CString VERSION = _T("Version");
const CString ITEM = _T("Camera");

using namespace MVC;
using namespace Camera;
MVC::Camera::CCamMgr::CCamMgr(void)
:m_bModify(false)					//!< �Ƿ��޸���
,m_strName(_T("Camera"))
{
}

MVC::Camera::CCamMgr::~CCamMgr(void)
{
}

bool MVC::Camera::CCamMgr::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		//!< ����汾���Ȳ�����
		//!< �ӽڵ�
		CString text, strValue;
		TiXmlElement* pChild = pNode->FirstChildElement();
		std::list<std::shared_ptr<CCamera> > ltItem;
		std::shared_ptr<CCamera> item;
		while(pChild)
		{
			text = pChild->ValueStr().c_str();
			if(ITEM == text)
			{
				item = std::shared_ptr<CCamera>(new CCamera());
				if(item->SerializeXml(pChild, bRead))		ltItem.push_back(item);
			}
			pChild = pChild->NextSiblingElement();
		}
		int itemcount = 0;
		for (auto item : ltItem)		itemcount = max(itemcount, item->GetID());
		m_vtCam.clear();
		m_vtCam.resize(itemcount + 1);
		for (auto item : ltItem)		m_vtCam[item->GetID()] = item;
	}
	else
	{
		pNode->SetAttribute(_T("FileType"), _T("EditItemFile"));
		pNode->SetAttribute(_T("Version"), _T("1.0"));

		TiXmlElement* pXmlInfo = new TiXmlElement(INFO_HEAD);
		pNode->LinkEndChild(pXmlInfo);

		TiXmlElement* pChild;
		for (std::shared_ptr<CCamera> item : m_vtCam)
		{
			if(!item)		continue;
			pChild = pNode->AddTiXmlChild((LPCTSTR)ITEM);
			item->SerializeXml(pChild, false);
			CGbl::SetProgressStep(1);
		}
	}
	return true;
}

void MVC::Camera::CCamMgr::OnCreate()
{
	m_vtCam.clear();
	if(m_pItemDoc)		m_pItemDoc->OnCloseDocument();
	SetModify(true);	//!< �½��Ĺ���Ĭ��Ϊ���޸�״̬

	//!< ��ʼ��·����Ϣ
	m_strName = _T("Camera");
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	m_strFileName = m_strName + _T(".") + CAM_EXPAND_NAME;						//!< ·����"p_������.var"
}

void MVC::Camera::CCamMgr::OnClose()
{
	m_vtCam.clear();
	m_pItemDoc = NULL;
}

void MVC::Camera::CCamMgr::OpenDoc()
{
	if(!m_pItemDoc){
		m_pItemDoc=(MVC::Camera::CCamDoc*)((CPMApp *)AfxGetApp())->m_pCamDocMgr->OpenDocumentFile(NULL);
		m_pItemDoc->ShowAllItem();
	}
	else{
		CView* view = (CView*)m_pItemDoc->GetView();
		CFrameWnd* frm = view->GetParentFrame();
		ASSERT(frm);
		if(frm->IsKindOf(RUNTIME_CLASS(CCamFrame)))
			frm->ActivateFrame();
	}
	m_pItemDoc->SetTitle(_T(""));
}

//!< �����ļ�
void MVC::Camera::CCamMgr::SaveFile()
{
	if(IsModify())
	{
		std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
		CString pathAll = proj->GetPath() + m_strName + _T(".") + CAM_EXPAND_NAME;
		TiXmlDocument pTiXml(pathAll);

		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< ��ʼ����
		pTiXml.LinkEndChild(decl);
		TiXmlComment * comment = new TiXmlComment();							//!< ע����Ϣ
		comment->SetValue(_T("�����ֹ��༭���ĵ�"));
		pTiXml.LinkEndChild(comment);
		TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					//!< д����ڵ�

		SerializeXml(pRoot, false);												//!< ��ӹ��̽ڵ�,д����
		pTiXml.LinkEndChild(pRoot);
		if(!pTiXml.SaveFile()){
			MessageBox(NULL, _T("����ʧ�ܣ�"), _T("����"), MB_ICONEXCLAMATION);
			return ;
		}
	}
	CGbl::SetProgressStep(1);
	m_bModify = false;
}

//!< ����Ƶ�����ļ������� �� ���� + ·�� + �汾��
bool MVC::Camera::CCamMgr::OpenFile(CString name, CString pathall, CString ver)
{
	m_strName = name;
	m_strFileName = m_strName + _T(".") + CAM_EXPAND_NAME;
	m_strVersion = ver;
	TiXmlDocument pTiXml((LPCTSTR)pathall);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("�������ļ���ʽ����"), _T("����"), MB_ICONEXCLAMATION);
		return false;
	}
	//!< ��ʼ����
	SerializeXml(pTiXml.RootElement(), true);
	return true;
}

//!< ���id�ű���
std::shared_ptr<CCamera> MVC::Camera::CCamMgr::GetCam(ULONG id)
{
	std::shared_ptr<CCamera> _item;
	if(id >= m_vtCam.size())
		return _item;
	return m_vtCam[id];
}

//!< ��õ�һ������
std::shared_ptr<CCamera> MVC::Camera::CCamMgr::GetFirstItem()
{
	std::vector<std::shared_ptr<CCamera> >::iterator iter = m_vtCam.begin();
	for(;iter !=  m_vtCam.end(); ++iter){
		if(*iter)		return *iter;
	}
	std::shared_ptr<CCamera> empty;
	return empty;
}

//!< ������һ������
std::shared_ptr<CCamera> MVC::Camera::CCamMgr::GetLastItem()
{
	std::vector<std::shared_ptr<CCamera> >::reverse_iterator iter = m_vtCam.rbegin();
	for(;iter !=  m_vtCam.rend(); ++iter){
		if(*iter)		return *iter;
	}
	std::shared_ptr<CCamera> empty;
	return empty;
}

//!< ���ұ��Ϊid�ı���
bool MVC::Camera::CCamMgr::FindItem(ULONG id)
{
	if(id >= m_vtCam.size())			return false;
	return !!m_vtCam[id];
}

//!< ��ӱ���,��ʱ������û��ID�����ڱ�����������
bool MVC::Camera::CCamMgr::AddItem(std::shared_ptr<CCamera> item)
{
	if(!item)												return false;
	for(UINT i = 0; i < m_vtCam.size(); ++i)
	{
		if(m_vtCam[i])										continue;
		item->SetID(i);										//!< �ҵ���һ���յ�λ�ã���item�ı�Ÿ���
		m_vtCam[i] = item;
		return true;
	}
	//!< ���ǰ��û�пյ�λ�ã����������
	item->SetID((UINT)m_vtCam.size());						//!< ���ñ���ID��
	m_vtCam.push_back(item);
	return true;
}

//!< ɾ�����Ϊid�ı���
void MVC::Camera::CCamMgr::DeleteItem(ULONG id)
{
	if(GetCam(id))						m_vtCam[id].reset();
}

//!< ��ñ�������ʵ����
UINT MVC::Camera::CCamMgr::GetItemSize()
{
	UINT count = 0;
	for (std::shared_ptr<CCamera> item : m_vtCam){
		if(item)		++count;
	}
	return count;
}

//!< ��������������
void MVC::Camera::CCamMgr::ExChangeItem(UINT id1, UINT id2)
{
	UINT size = (UINT)m_vtCam.size();
	ASSERT(id1 < size && id2 < size);
	std::shared_ptr<CCamera> item1, item2;
	item1 = GetCam(id1);
	item2 = GetCam(id2);
	if(item1){	item1->SetID(id2);	m_vtCam[id2] = item1;	}
	if(item2){	item2->SetID(id1);	m_vtCam[id1] = item2;	}
}

//!< ��ʾ�����Ϊid�ı���
void MVC::Camera::CCamMgr::ShowItem(UINT id)
{
	std::shared_ptr<CCamera> item = GetCam(id);
	if(!item)		return;

	if(!CProjectMgr::GetMe().GetProj())	return;
	//!< ���������û�򿪣���ô�򿪱�����һ��
	BOOL isMax;
	CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
	CFrameWnd* pFrame = mf->MDIGetActive(&isMax);		//!< �ҵ���ǰ��������Ӵ���
	if(pFrame)
	{
		CDocument* pDoc = pFrame->GetActiveDocument();
		if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CCamDoc)))
		{
			if(((CCamDoc *)pDoc)->GetView()->ShowItem(id))
				return;
		}
	}
	OpenDoc();
	m_pItemDoc->GetView()->ShowItem(id);
}

//!< ���ҷ��ϸ��ַ����ı���������ƥ�������
int MVC::Camera::CCamMgr::SearchItem(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex /* = false */)
{
	int nMatchCount = 0;
	//std::shared_ptr<CCamera> item;
	if(!bMatchCase && !bRegex)		str = str.MakeUpper();

	for (auto item : m_vtCam){
		if(item)
			if(item->DoSearch(str, bMatchWhole, bMatchCase, bRegex))
				++nMatchCount;
	}
	return nMatchCount;
}

//!< ���һ����ͬ�Ķ˿ں�
int MVC::Camera::CCamMgr::GetDifferentPort(int nDefPort)
{
	//std::shared_ptr<CCamera> item;
	for (auto item : m_vtCam)
	{
		if(!item)						continue;	// �����ڵĲ���
		if(!item->IsBroadCast())		continue;	// ���㲥�Ĳ���
		if(item->GetPort() != nDefPort)	continue;	// �˿ڲ�ͬ�Ĳ���
		return GetDifferentPort(++nDefPort);		//!< �ݹ�,���ڷ�������ͬ�˿�,������һ���ز��ظ�	
	}
	return nDefPort;
}

//!< ˢ�����б����ʾ
void MVC::Camera::CCamMgr::FreshAllGrid()
{
	POSITION pos = g_App.m_pCamDocMgr->GetFirstDocPosition();
	CCamDoc* pDoc;
	CCamView* pView;
	while(pos)
	{
		pDoc = (CCamDoc *)g_App.m_pCamDocMgr->GetNextDoc(pos);
		pView = pDoc->GetView();
		pView->m_ItemGrid.DeleteRowAll();
		pView->m_ItemGrid.DeleteColAll();
		pView->m_ItemGrid.ResetAll();
		pView->m_ItemGrid.InitAll();
		pView->m_ItemGrid.RedrawGrid();
		pView->m_ItemGrid.RedrawAll();
		pDoc->UpdateAllViews(NULL);
	}
}

//!< ��Ӹոձ��޸ĵı���
void MVC::Camera::CCamMgr::AddEditItem(UINT id)
{
	m_ltEditItemID.push_back(id);
}

//!< �����������������
void MVC::Camera::CCamMgr::SetEditEnd()
{
	if(m_ltEditItemID.empty())		return;
	POSITION pos = g_App.m_pCamDocMgr->GetFirstDocPosition();
	CCamDoc* pDoc;
	CCamView* pView;
	while(pos)
	{
		pDoc = (CCamDoc *)g_App.m_pCamDocMgr->GetNextDoc(pos);
		pView = pDoc->GetView();
		pView->m_ItemGrid.FreshItemRows(m_ltEditItemID);
	}
	m_ltEditItemID.clear();
}

