// DeviceMapDoc.cpp : 实现文件
#include "stdafx.h"
#include "PMApp.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "DeviceMapDoc.h"
#include "DeviceMapView.h"
#include "DeviceOne.h"
#include "DevMgr.h"

using namespace MVC;
using namespace Device;
// CDeviceMapDoc
IMPLEMENT_DYNCREATE(CDeviceMapDoc, CDocument)

// CDeviceMapDoc 诊断
#ifdef _DEBUG
void CDeviceMapDoc::AssertValid() const{CDocument::AssertValid();}
#ifndef _WIN32_WCE
void CDeviceMapDoc::Dump(CDumpContext& dc) const{CDocument::Dump(dc);}
#endif
#endif //_DEBUG
#ifndef _WIN32_WCE
// CDeviceMapDoc 序列化
void CDeviceMapDoc::Serialize(CArchive& ar){if (ar.IsStoring()){}/*在此添加存储代码*/else{}/*在此添加加载代码*/}
#endif

CDeviceMapDoc::CDeviceMapDoc()
{
}

BOOL CDeviceMapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDeviceMapDoc::~CDeviceMapDoc()
{
	if(CDevMgr::GetMe().m_DevMapDoc == this)
		CDevMgr::GetMe().m_DevMapDoc = NULL;
}

BEGIN_MESSAGE_MAP(CDeviceMapDoc, CDocument)
END_MESSAGE_MAP()

// CDeviceMapDoc 命令
CDeviceMapView* CDeviceMapDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	CView* view = GetNextView(pos);
	ASSERT(view);
	return (CDeviceMapView*)view;
}

CSize CDeviceMapDoc::GetViewSize()
{
	CSize size(110,110);
	CDevMgr* mgr = &CDevMgr::GetMe();
	//std::shared_ptr<CDeviceOne> device;
	for (auto device : mgr->m_vtDevice){
		if(!device)		continue;
		if(size.cx < (device->GetLTPt().x + DEV_WIDTH + 20))
			size.cx = device->GetLTPt().x + DEV_WIDTH + 20;
		if(size.cy < (device->GetLTPt().y + DEV_HEIGH + 40))
			size.cy = device->GetLTPt().y + DEV_HEIGH + 40;
	}
	return size;
}

void CDeviceMapDoc::OnCountChildPoint()
{
	CDevMgr* mgr = &CDevMgr::GetMe();
	//std::shared_ptr<CDeviceOne> pDevice;
	std::list<std::shared_ptr<CDeviceOne> > ltMainDev;
	std::list<std::shared_ptr<CDeviceOne> > ltMainScanDev;	//!< 为了把扫描上来的设备画在后边
	for (auto pDevice : mgr->m_vtDevice){
		if(!pDevice)		continue;
		if(pDevice->getParentID() == UINT(-1))
		{
			if(pDevice->IsProj())	ltMainDev.push_back(pDevice);
			else					ltMainScanDev.push_back(pDevice);
		}
		pDevice->SetLTPt(0,0);
	}
	for(int i = 0; i < DEV_MAXNUM; ++i)		m_MaxX[i] = m_MaxY[i] = DEV_EDGE;
	for(auto pDevice: ltMainDev)				pDevice->OnSetLevel(0);
	for(auto pDevice: ltMainScanDev)			pDevice->OnSetLevel(0);
	for(auto pDevice: ltMainDev)				pDevice->OnCountLevel();
	for(auto pDevice: ltMainScanDev)			pDevice->OnCountLevel();
	for(auto pDevice: ltMainDev)				pDevice->OnCountPoint();
	for(auto pDevice: ltMainScanDev)			pDevice->OnCountPoint();
	UpdateAllViews(NULL);
}

void CDeviceMapDoc::OnMoveMaxX(BYTE level,UINT x)
{
	for(int i = level; i < DEV_MAXNUM; ++i){	//!< 如果左右侧不比x大是不行的
		if(SoftInfo::CSoftInfo::GetMe().getHDevMap()){
			if(m_MaxX[i] < (x+DEV_DIS_X))		m_MaxX[i] = x + DEV_DIS_X;
		}
		else{
			if(m_MaxY[i] < (x+DEV_DIS_Y))		m_MaxY[i] = x + DEV_DIS_Y;
		}
	}
}

void MVC::Device::CDeviceMapDoc::SetTitle(LPCTSTR lpszTitle)
{
	CString strTitle = _T("设备拓扑图");
	if(CProjectMgr::GetMe().IsWatch())	strTitle += _T("（监控状态）");
	CDocument::SetTitle(strTitle);
}

void MVC::Device::CDeviceMapDoc::ClearUndo()
{
	while(!m_stDevUndo.empty())		m_stDevUndo.pop();
}

void MVC::Device::CDeviceMapDoc::ClearRedo()
{
	while(!m_stDevRedo.empty())		m_stDevRedo.pop();
}

void MVC::Device::CDeviceMapDoc::UndoAdd()
{
	std::shared_ptr<SDevUndo> undo = m_stDevUndo.top();
	m_stDevUndo.pop();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	devMgr->DeleteDevice(undo->m_Device->getID());
	m_stDevRedo.push(undo);
}

void MVC::Device::CDeviceMapDoc::UndoDel()
{
	std::shared_ptr<SDevUndo> undo = m_stDevUndo.top();
	m_stDevUndo.pop();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	devMgr->m_vtDevice[undo->m_Device->getID()] = undo->m_Device;
	m_stDevRedo.push(undo);
}

void MVC::Device::CDeviceMapDoc::UndoUpd()
{
	std::shared_ptr<SDevUndo> undo = m_stDevUndo.top();
	m_stDevUndo.pop();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device = std::shared_ptr<CDeviceOne>(new CDeviceOne);
	device->CopyFrom(*devMgr->GetDevice(undo->m_Device->getID()));
	std::shared_ptr<SDevUndo> redo = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_UPD, device));
	redo->SetEnd(undo->m_bEnd);
	m_stDevRedo.push(redo);
	devMgr->m_vtDevice[undo->m_Device->getID()]->CopyFrom(*undo->m_Device);
}

void MVC::Device::CDeviceMapDoc::RedoAdd()
{
	std::shared_ptr<SDevUndo> redo = m_stDevRedo.top();
	m_stDevRedo.pop();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	devMgr->m_vtDevice[redo->m_Device->getID()] = redo->m_Device;
	m_stDevUndo.push(redo);
}

void MVC::Device::CDeviceMapDoc::RedoDel()
{
	std::shared_ptr<SDevUndo> redo = m_stDevRedo.top();
	m_stDevRedo.pop();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	devMgr->DeleteDevice(redo->m_Device->getID());
	m_stDevUndo.push(redo);
}

void MVC::Device::CDeviceMapDoc::RedoUpd()
{
	std::shared_ptr<SDevUndo> redo = m_stDevRedo.top();
	m_stDevRedo.pop();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device = std::shared_ptr<CDeviceOne>(new CDeviceOne);
	device->CopyFrom(*devMgr->GetDevice(redo->m_Device->getID()));
	std::shared_ptr<SDevUndo> undo = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_UPD, device));
	undo->SetEnd(redo->m_bEnd);
	m_stDevUndo.push(undo);
	devMgr->m_vtDevice[redo->m_Device->getID()]->CopyFrom(*redo->m_Device);
}

void MVC::Device::CDeviceMapDoc::OnUndo()
{
	if(m_stDevUndo.empty())	return;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<SDevUndo> undo;
	bool bNeedFresh = false;
	CDeviceMapView* pView = GetView();
	devMgr->SetModify();
	do{
		undo = m_stDevUndo.top();
		undo->m_Device->SetShow(true);
		undo->m_Device->SetExpand(true);
		if(undo->m_uiEditType == CGbl::UNDO_TYPE_ADD)			{UndoAdd();	bNeedFresh = true;}
		else if(undo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		{UndoDel();	bNeedFresh = true;}
		else if(undo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		{UndoUpd();}
	}
	while(!m_stDevUndo.empty() && !m_stDevUndo.top()->m_bEnd);
	if(bNeedFresh)												devMgr->CheckAndConnect();
	if(undo && undo->m_Device)									pView->ShowDeviceAt(undo->m_Device->getID());
	FreshMap();
}

void MVC::Device::CDeviceMapDoc::OnRedo()
{
	if(m_stDevRedo.empty())	return;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<SDevUndo> redo;
	bool bNeedFresh = false;
	CDeviceMapView* pView = GetView();
	devMgr->SetModify();
	while(!m_stDevRedo.empty() && !m_stDevRedo.top()->m_bEnd){
		redo = m_stDevRedo.top();
		redo->m_Device->SetShow(true);
		redo->m_Device->SetExpand(true);
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_ADD)			{RedoAdd();	bNeedFresh = true;}
		else if(redo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		{RedoDel();	bNeedFresh = true;}
		else if(redo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		{RedoUpd();}
	}
	if(!m_stDevRedo.empty() && m_stDevRedo.top()->m_bEnd){
		redo = m_stDevRedo.top();
		redo->m_Device->SetShow(true);
		redo->m_Device->SetExpand(true);
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_ADD)			{RedoAdd();	bNeedFresh = true;}
		else if(redo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		{RedoDel();	bNeedFresh = true;}
		else if(redo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		{RedoUpd();}
	}
	if(bNeedFresh)												devMgr->CheckAndConnect();
	if(redo && redo->m_Device)									pView->ShowDeviceAt(redo->m_Device->getID());
	FreshMap();
}

void MVC::Device::CDeviceMapDoc::AddUndoMember(std::shared_ptr<SDevUndo> devUndo)
{
	if(!devUndo)				return;
	CDevMgr::GetMe().SetModify();
	m_stDevUndo.push(devUndo);
	devUndo->m_Device->SetShow(true);
	ClearRedo();
}

void MVC::Device::CDeviceMapDoc::SetUndoEnd()
{
	if(!m_stDevUndo.empty())	m_stDevUndo.top()->SetEnd();
}

void MVC::Device::CDeviceMapDoc::FreshMap()
{
	OnCountChildPoint();
	UpdateAllViews(NULL);
}