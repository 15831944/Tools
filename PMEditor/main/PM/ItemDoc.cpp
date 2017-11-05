// ItemDoc.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "PMApp.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "ItemDoc.h"
#include "ItemMgr.h"
#include "ItemGrid.h"
#include "ItemView.h"
#include "ItemGroup.h"
#include "Item.h"
#include "SourceProperty.h"

#include "ItemInConfigDlg.h"

// CItemDoc
using namespace MVC;
using namespace Item;

tagItemUndo::tagItemUndo()
{
	m_uiEditType = 0;
	m_uiEditInfo = 0;
	m_bEnd = false;
}

tagItemUndo::tagItemUndo(UINT type, std::shared_ptr<CItem> item, UINT info)
{
	if(type == CGbl::UNDO_TYPE_UPD){
		m_Item = std::shared_ptr<CItem>(new CItem(_T("")));
		*m_Item = *item;
	}
	else{m_Item = item;}
	m_uiEditType = type;
	m_uiEditInfo = info;
	m_bEnd = false;
}

IMPLEMENT_DYNCREATE(CItemDoc, CDocument)

CItemDoc::CItemDoc()
{
	m_uiGroupID = UINT(-1);
}

CItemDoc::~CItemDoc()
{
	if(m_uiGroupID == -1){
		if(CItemMgr::GetMe().m_pItemDoc == this)
			CItemMgr::GetMe().m_pItemDoc = NULL;
	}
	else{
		std::shared_ptr<CItemGroup> group = CItemMgr::GetMe().GetGroup(m_uiGroupID);
		if(group)		group->m_ItemDoc = NULL;
	}
}

BEGIN_MESSAGE_MAP(CItemDoc, CDocument)
END_MESSAGE_MAP()

// CItemDoc ���
#ifdef _DEBUG
void CItemDoc::AssertValid() const{CDocument::AssertValid();}
#ifndef _WIN32_WCE
void CItemDoc::Dump(CDumpContext& dc) const{CDocument::Dump(dc);}
#endif
#endif //_DEBUG
#ifndef _WIN32_WCE
// CItemDoc ���л�
void CItemDoc::Serialize(CArchive& ar){if (ar.IsStoring()){}else{}}
#endif

// CItemDoc ����
BOOL CItemDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())	return FALSE;
	return TRUE;
}

BOOL CItemDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))	return FALSE;
	return TRUE;
}

CItemView* CItemDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	CView* view = GetNextView(pos);
	ASSERT(view);
	return (CItemView*)view;
}

void CItemDoc::ShowAllItem()
{
	CItemView* pView = GetView();
	pView->m_ItemGrid.m_uiCulGroupID = UINT(-1);
	pView->m_ItemGrid.m_uiShowType = 0;
	m_uiGroupID = UINT(-1);
	pView->m_ItemGrid.RedrawGrid();
}

void CItemDoc::ShowGroup(UINT groupid)
{
	CItemView* pView = GetView();
	pView->m_ItemGrid.m_uiCulGroupID = groupid;
	pView->m_ItemGrid.m_uiShowType = 3;
	m_uiGroupID = groupid;
	pView->m_ItemGrid.RedrawGrid();
}

void CItemDoc::AddUndoMember(std::shared_ptr<SItemUndo> itemUndo)
{
	if(!itemUndo)				return;
	CItemMgr::GetMe().SetModify();
	m_stItemUndo.push(itemUndo);
	ClearRedo();
	CItemMgr::GetMe().AddEditItem(itemUndo->m_Item->getID());
	if(itemUndo->m_bEnd)	GetGrid()->OnEditEnd();//CItemMgr::GetMe().SetEditEnd();
}

void CItemDoc::SetUndoEnd()
{
	if(!m_stItemUndo.empty())	m_stItemUndo.top()->m_bEnd = true;
	CItemMgr::GetMe().SetEditEnd();
}

void CItemDoc::OnUndo()
{
	if(m_stItemUndo.empty())	return;
	CItemGrid* itemGrid = GetGrid();
	if(!itemGrid)				return;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<SItemUndo> undo;
	itemMgr->SetModify();
	itemGrid->EnableUpdate(FALSE);
	do{
		undo = m_stItemUndo.top();
		if(undo->m_uiEditType == CGbl::UNDO_TYPE_ADD)		UndoAdd();
		if(undo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		UndoDel();
		if(undo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		UndoUpd();
		itemMgr->AddEditItem(undo->m_Item->getID());
	}
	while(!m_stItemUndo.empty() && !m_stItemUndo.top()->m_bEnd);

	itemGrid->EnableUpdate(TRUE);
	itemGrid->RedrawAll();
	itemGrid->OnEditEnd();
}

void CItemDoc::UndoAdd()
{
	std::shared_ptr<SItemUndo> undo = m_stItemUndo.top();
	m_stItemUndo.pop();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	CItemGrid* itemGrid = GetGrid();
	long row = itemGrid->GetItemRow(undo->m_Item->getID());

	itemMgr->DeleteItem(undo->m_Item->getID());
	m_stItemRedo.push(undo);
}

void CItemDoc::UndoDel()
{
	std::shared_ptr<SItemUndo> undo = m_stItemUndo.top();
	m_stItemUndo.pop();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	itemMgr->m_vtItem[undo->m_Item->getID()] = undo->m_Item;
	itemMgr->m_mpItem[undo->m_Item->getName()] = undo->m_Item;
//	std::shared_ptr<CItemGroup> group;
// 	foreach(UINT id, undo->m_Item->m_ltGroupID)
// 	{
// 		group = itemMgr->GetGroup(id);
// 		ASSERT(group);
// 		group->AddItem(undo->m_Item->getID());
// 	}
	m_stItemRedo.push(undo);
}

// void CItemDoc::UndoGup()
// {
// 	std::shared_ptr<SItemUndo> undo = m_stItemUndo.top();
// 	m_stItemUndo.pop();
// 	CItemMgr* itemMgr = &CItemMgr::GetMe();
// 	std::shared_ptr<CItemGroup> group = itemMgr->GetGroup(undo->m_uiEditInfo);
// 	ASSERT(group);
// 
// 	if(undo->m_uiEditType == CGbl::UNDO_TYPE_GUP_DEL){
// //		group->AddItem(undo->m_Item->getID());
// 		undo->m_Item->setMyGroupID(undo->m_uiEditInfo);
// 	}
// 	else{
// //		group->RemoveItem(undo->m_Item->getID());
// 	}
// 	m_stItemRedo.push(undo);
// }

void CItemDoc::UndoUpd()
{
	std::shared_ptr<SItemUndo> undo = m_stItemUndo.top();
	m_stItemUndo.pop();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
//	std::shared_ptr<CItemGroup> group;

	std::shared_ptr<CItem> item = itemMgr->GetItem(undo->m_Item->getID());
	std::shared_ptr<SItemUndo> redo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item, 0));
	redo->m_bEnd = undo->m_bEnd;
	m_stItemRedo.push(redo);

//	foreach(group, itemMgr->m_vtItemGroup){
//		if(!group)		continue;
//		group->RemoveItem(item->getID());
//	}

	itemMgr->m_mpItem[item->getName()].reset();
	itemMgr->m_mpItem[undo->m_Item->getName()] = item;
	*item = *undo->m_Item;
//	foreach(UINT id, item->m_ltGroupID){
//		group = itemMgr->GetGroup(id);
//		ASSERT(group);
//		group->AddItem(item->getID());
//	}
}

void CItemDoc::OnRedo()
{
	if(m_stItemRedo.empty())	return;
	CItemGrid* itemGrid = GetGrid();
	if(!itemGrid)				return;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<SItemUndo> redo;
	itemMgr->SetModify();
	itemGrid->EnableUpdate(FALSE);
	while(!m_stItemRedo.empty() && !m_stItemRedo.top()->m_bEnd){
		redo = m_stItemRedo.top();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_ADD)		RedoAdd();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		RedoDel();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		RedoUpd();
		itemMgr->AddEditItem(redo->m_Item->getID());
	}
	if(!m_stItemRedo.empty() && m_stItemRedo.top()->m_bEnd){
		redo = m_stItemRedo.top();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_ADD)		RedoAdd();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		RedoDel();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		RedoUpd();
		itemMgr->AddEditItem(redo->m_Item->getID());
	}
	itemGrid->EnableUpdate(TRUE);
	itemGrid->OnEditEnd();
}

void CItemDoc::RedoAdd()
{
	std::shared_ptr<SItemUndo> redo = m_stItemRedo.top();
	m_stItemRedo.pop();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	itemMgr->m_vtItem[redo->m_Item->getID()] = redo->m_Item;
	itemMgr->m_mpItem[redo->m_Item->getName()] = redo->m_Item;
//	std::shared_ptr<CItemGroup> group;
//	foreach(UINT id, redo->m_Item->m_ltGroupID)
//	{
//		group = itemMgr->GetGroup(id);
//		ASSERT(group);
//		group->AddItem(redo->m_Item->getID());
//	}
	m_stItemUndo.push(redo);
}

void CItemDoc::RedoDel()
{
	std::shared_ptr<SItemUndo> redo = m_stItemRedo.top();
	m_stItemRedo.pop();
	CItemMgr::GetMe().DeleteItem(redo->m_Item->getID());
	m_stItemUndo.push(redo);
}

// void CItemDoc::RedoGup()
// {
// 	std::shared_ptr<SItemUndo> redo = m_stItemRedo.top();
// 	m_stItemRedo.pop();
// 	CItemMgr* itemMgr = &CItemMgr::GetMe();
// 	std::shared_ptr<CItemGroup> group = itemMgr->GetGroup(redo->m_uiEditInfo);
// 	ASSERT(group);
// 
// 	if(redo->m_uiEditType == CGbl::UNDO_TYPE_GUP_DEL){
// //		group->RemoveItem(redo->m_Item->getID());
// 	}
// 	else{
// 		redo->m_Item->setMyGroupID(redo->m_uiEditInfo);
// //		group->AddItem(redo->m_Item->getID());
// 	}
// 	m_stItemUndo.push(redo);
// }

void CItemDoc::RedoUpd()
{
	std::shared_ptr<SItemUndo> redo = m_stItemRedo.top();
	m_stItemRedo.pop();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
//	std::shared_ptr<CItemGroup> group;

	std::shared_ptr<CItem> item = itemMgr->GetItem(redo->m_Item->getID());
	std::shared_ptr<SItemUndo> undo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item, 0));
	undo->m_bEnd = redo->m_bEnd;
	m_stItemUndo.push(undo);

//	foreach(group, itemMgr->m_vtItemGroup){
//		if(!group)		continue;
//		group->RemoveItem(item->getID());
//	}

	itemMgr->m_mpItem[item->getName()].reset();
	itemMgr->m_mpItem[redo->m_Item->getName()] = item;
	*item = *redo->m_Item;
//	foreach(UINT id, item->m_ltGroupID){
//		group = itemMgr->GetGroup(id);
//		ASSERT(group);
//		group->AddItem(item->getID());
//	}
}

void CItemDoc::RedoOne(std::shared_ptr<SItemUndo> redo)
{
}

void CItemDoc::ClearUndo()
{
	while(!m_stItemUndo.empty())		m_stItemUndo.pop();
}

void CItemDoc::ClearRedo()
{
	while(!m_stItemRedo.empty())		m_stItemRedo.pop();
}

CItemGrid* CItemDoc::GetGrid()
{
	CView* pView = (CView *)m_viewList.GetHead();
	if(!pView)											return NULL;
	if(!pView->IsKindOf(RUNTIME_CLASS(CItemView)))		return NULL;
	return &((CItemView *)pView)->m_ItemGrid;
}

void CItemDoc::SerializeIn(TiXmlElement* pNode)
{
	// ������Ե���var��vxpt�����ļ�,�������ļ����ļ�ͷ���в�ͬ,��Ҫ�������ж�һ��
	if(!pNode->FirstAttribute() || (_T("ItemOutFile") != (CString)pNode->FirstAttribute()->Value() &&
		_T("EditItemFile") != (CString)pNode->FirstAttribute()->Value()))		return;
	Dialog::CItemInConfigDlg* dlg = &Dialog::CItemInConfigDlg::GetMe();
	if(IDOK != dlg->DoModal(Dialog::CItemInConfigDlg::ItemIn, GetView()->m_ItemGrid.GetGroupID()))	return;

	CString name;
	TiXmlElement* pChild = pNode->FirstChildElement();
	std::shared_ptr<CItem> item;
	std::list<std::shared_ptr<CItem> > ltItem;
	while(pChild)
	{
		CString text = pChild->ValueStr().c_str();
		if(text == _T("Item"))
		{
			item = std::shared_ptr<CItem>(new CItem(_T("")));
			if(!item->SerializeXml(pChild, true, true))		return;
			ltItem.push_back(item);
		}
		pChild = pChild->NextSiblingElement();
	}
	// ������ϣ����ڿ�ʼ���뵽����
	InItem(ltItem, dlg);
}

// ��ȡConfig�����ı������ļ�
void CItemDoc::SerializeConfig(CString strPath, CString strTitle, CString strExt)
{
}

// ��Ӷ������
void CItemDoc::InItem(std::list<std::shared_ptr<CItem> > ltItem, Dialog::CItemInConfigDlg* dlg)
{
	ItemInAdvanceOptions(ltItem);
	if(dlg->m_nSameNameType == 0)			ItemInRenameItem(ltItem, dlg->m_uiGroupID);
	else if(dlg->m_nSameNameType == 1)		ItemInDelFileItem(ltItem, dlg->m_uiGroupID);
	else if(dlg->m_nSameNameType == 2)		ItemInDelMgrItem(ltItem, dlg->m_uiGroupID);
	else if(dlg->m_nSameNameType == 3)		ItemInRenameStrItem(ltItem, dlg->m_strItemSameName, dlg->m_uiGroupID);
	SetUndoEnd();
}

// ���������ճ���ĸ߼�ѡ���
bool CItemDoc::ItemInAdvanceOptions(std::list<std::shared_ptr<CItem> > ltItem)
{
	Dialog::CItemInConfigDlg* itemInInfo = &Dialog::CItemInConfigDlg::GetMe();
	std::shared_ptr<CItem> item;
	if(!itemInInfo->m_bUseAdvanceOptions)		return true;
	// ָ�����
	if(itemInInfo->m_bUseBaseID)
	{
		int nBaseID = itemInInfo->m_nBaseID;
		int id = 0;
		for (auto item : ltItem)
		{
			id = item->getID() + nBaseID;
			if(id < 0)
			{
				CString str;
				str.Format("%d�ű��� ", item->getID());
				str = str + item->getName() + _T(" ָ�����ʧ��!");
				MessageBox(NULL, str, "����", MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			item->setID(id);
		}
	}
	// ���Ʋ����ַ���
	if(itemInInfo->m_bNameName)
	{
		for (auto item : ltItem){
			if(itemInInfo->m_nNameAddr == 0)	item->setItemName(itemInInfo->m_strNameName + item->getName());
			else								item->setItemName(item->getName() + itemInInfo->m_strNameName);
		}
	}
	// ��ǩ�����ַ���
	if(itemInInfo->m_bTagName)
	{
		for (auto item : ltItem){
			if(itemInInfo->m_nTagAddr == 0)		item->setTag(itemInInfo->m_strTagName + item->getTag());
			else								item->setTag(item->getTag() + itemInInfo->m_strTagName);
		}
	}
	// ��ע�����ַ���
	if(itemInInfo->m_bDespName)
	{
		for (auto item : ltItem){
			if(itemInInfo->m_nDespAddr == 0)	item->setDescription(itemInInfo->m_strDespName + item->getDescription());
			else								item->setDescription(item->getDescription() + itemInInfo->m_strDespName);
		}
	}
	return true;
}

// ����������Զ������������ı���
void CItemDoc::ItemInRenameItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID/* = 0 */)
{
	UINT i = 1;
	CString name, strFill;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<SItemUndo> itemUndo;
	std::shared_ptr<CItem> item;
	int maxID = MAX_ITEM_COUNT;

	for (auto item : ltItem)
	{
		name = item->getName();
		while(itemMgr->GetItemFast(item->getName())){
			strFill.Format("%d",i++);
			item->setItemName(name + strFill);
		}
		i = 1;
		if(Dialog::CItemInConfigDlg::GetMe().m_bUseAdvanceOptions && Dialog::CItemInConfigDlg::GetMe().m_bUseBaseID)
		{	// ָ��ID��
			if(!AddItem(item->getID(), item, maxID, groupID, Dialog::CItemInConfigDlg::GetMe().m_nBaseIDSame))
				return;
		}
		else
		{
			if(!itemMgr->AddItem(item, maxID, groupID))
				return;		// ��������ش�֤���ǳ��磬�Ժ��Ҳ��ͬ�ټ���
		}
		itemUndo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_ADD, item, 0));
		AddUndoMember(itemUndo);
	}
}

// ��������������Ĳ����ַ���
void CItemDoc::ItemInRenameStrItem(std::list<std::shared_ptr<CItem> > ltItem, CString strName, UINT groupID/* = 0 */)
{
	UINT i = 1;
	CString name;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<SItemUndo> itemUndo;
	int maxID = MAX_ITEM_COUNT;
	for (std::shared_ptr<CItem> item : ltItem)
	{
		name = item->getName();
		if(itemMgr->GetItemFast(item->getName()))
		{
			if(Dialog::CItemInConfigDlg::GetMe().m_nSameNameAddr == 0)		item->setItemName(strName + name);
			else															item->setItemName(name + strName);
		}
		if(itemMgr->GetItemFast(item->getName()))
		{
			int nResult = MessageBox(NULL, "������Ϊ \"" + item->getName() + " \"�ı����Ѵ��ڣ�����������ʧ��!\r\n�Ƿ��������?",
				_T("��ʾ"), MB_YESNO|MB_ICONINFORMATION);
			if(nResult == IDYES)	continue;
			else					return;
		}
		if(Dialog::CItemInConfigDlg::GetMe().m_bUseAdvanceOptions && Dialog::CItemInConfigDlg::GetMe().m_bUseBaseID)
		{	// ָ��ID��
			if(!AddItem(item->getID(), item, maxID, groupID, Dialog::CItemInConfigDlg::GetMe().m_nBaseIDSame))
				return;
		}
		else
		{
			if(!itemMgr->AddItem(item, maxID, groupID)){
				return;		// ��������ش�֤���ǳ��磬�Ժ��Ҳ��ͬ�ټ���
			}
		}
		itemUndo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_ADD, item, 0));
		AddUndoMember(itemUndo);
	}
}

// ��������������������ı���
void CItemDoc::ItemInDelFileItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID/* = 0 */)
{
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<SItemUndo> itemUndo;
	int maxID = MAX_ITEM_COUNT;
	for (std::shared_ptr<CItem> item : ltItem)
	{
		if(itemMgr->GetItemFast(item->getName()))		continue;
		if(Dialog::CItemInConfigDlg::GetMe().m_bUseAdvanceOptions && Dialog::CItemInConfigDlg::GetMe().m_bUseBaseID)
		{	// ָ��ID��
			if(!AddItem(item->getID(), item, maxID, groupID, Dialog::CItemInConfigDlg::GetMe().m_nBaseIDSame))
				return;
		}
		else if(!itemMgr->AddItem(item, maxID, groupID))
			return;
		itemUndo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_ADD, item, 0));
		AddUndoMember(itemUndo);
	}
}

// �������������ʱ�õ���ı������Ǳ������еı���
void CItemDoc::ItemInDelMgrItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID/* = 0 */)
{
	std::shared_ptr<CItem> oldItem, newItem;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<SItemUndo> itemUndo;
	int maxID = MAX_ITEM_COUNT;
	for (auto newItem : ltItem)
	{
		oldItem = itemMgr->GetItemFast(newItem->getName());
		if(oldItem)
		{
			itemUndo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_DEL, oldItem, 0));
			AddUndoMember(itemUndo);
			itemMgr->DeleteItem(oldItem->getID());

			if(Dialog::CItemInConfigDlg::GetMe().m_bUseAdvanceOptions && Dialog::CItemInConfigDlg::GetMe().m_bUseBaseID)
			{	// ָ��ID��
				if(!AddItem(newItem->getID(), newItem, maxID, groupID, Dialog::CItemInConfigDlg::GetMe().m_nBaseIDSame))
					return;
			}
			else{
				itemMgr->AddItem(newItem, maxID, groupID);
			}
			itemUndo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_ADD, newItem, 0));
			AddUndoMember(itemUndo);
		}
		else
		{
			if(Dialog::CItemInConfigDlg::GetMe().m_bUseAdvanceOptions && Dialog::CItemInConfigDlg::GetMe().m_bUseBaseID)
			{	// ָ��ID��
				if(!AddItem(newItem->getID(), newItem, maxID, groupID, Dialog::CItemInConfigDlg::GetMe().m_nBaseIDSame))
					return;
			}
			else if(!itemMgr->AddItem(newItem, maxID, groupID))
				continue;
			itemUndo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_ADD, newItem, 0));
			AddUndoMember(itemUndo);
		}
	}
}

void MVC::Item::CItemDoc::SetTitle(LPCTSTR lpszTitle)
{
	CString strTitle;
	if(m_uiGroupID == UINT(-1))
		strTitle = _T("������һ��");
	else{
		std::shared_ptr<CItemGroup> group = CItemMgr::GetMe().GetGroup(m_uiGroupID);
		ASSERT(group);
		strTitle = _T("�����顪") + group->getName();
	}
	CString strWatch = _T("�����״̬��");
	if(CProjectMgr::GetMe().IsWatch())		strTitle += strWatch;
	CDocument::SetTitle(strTitle);
}

// ��ӱ�����ָ��ID����������Ƿ�ѯ�ʣ�0�ʣ�1���ʸ��ǣ�2������
// ��Ϊ���Ҫ���Ǳ���ʱ����Ҫ��һ�β�������ջ������CItemMgr��Ͳ��У������Ұᵽ���������
bool MVC::Item::CItemDoc::AddItem(UINT id, std::shared_ptr<CItem> item, int maxID, UINT groupid/*=0*/, UINT uiAsk/*=0*/)
{
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	if(!item)												return false;
	if(!itemMgr->FindGroup(groupid))						return false;
	item->setID(id);
	if(itemMgr->FindItem(id))								// �����Ѿ�����
	{
		if(uiAsk == 0)		// ����ѯ����δ���
		{
			CString str;
			str.Format("���Ϊ %d �ı����Ѿ����ڣ���θ��ǣ�\r\n�ǣ�����\t������", id);
			LRESULT lr = MessageBox(NULL, str, _T("��ʾ"), MB_YESNO);
			if(lr == IDYES)									goto COVER;
			else											goto JUMPOUT;
		}
		else if(uiAsk == 1)	// ����
		{
COVER:		std::shared_ptr<SItemUndo> itemUndo;
			std::shared_ptr<CItem> oldItem = itemMgr->GetItem(id);
			itemUndo = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_DEL, oldItem, 0));
			AddUndoMember(itemUndo);
			itemMgr->DeleteItem(id);
//			itemMgr->m_vtItemGroup[groupid]->AddItem(id);
//			item->AddGroup(groupid);
			item->setMyGroupID(groupid);
			itemMgr->m_vtItem[id] = item;
			itemMgr->m_mpItem[item->getName()] = item;
			return true;
		}
		else				// ����
		{
JUMPOUT:	return true;
		}
	}
	else if(itemMgr->GetItemSize() >= maxID)				// ������������
	{
		CString strError;
		strError.Format("�������������� %d �����޷�����ӱ�����", maxID);
		MessageBox(NULL, strError, _T("����"), MB_OK);
		return false;
	}
	else if(id >= itemMgr->m_vtItem.size())					
	{
// 		if(id >= maxID)
// 		{
// 			CString strError;
// 			strError.Format("�������������� %d �����޷�����ӱ�����", maxID);
// 			MessageBox(NULL, strError, _T("����"), MB_OK);
// 			return false;
// 		}
//		itemMgr->m_vtItemGroup[groupid]->AddItem(id);
//		item->AddGroup(groupid);
		item->setMyGroupID(groupid);
		itemMgr->m_vtItem.resize(id + 1);
		itemMgr->m_vtItem[id] = item;
		itemMgr->m_mpItem[item->getName()] = item;
		return true;
	}
	else													// �����ڣ�ֱ�ӷž�����
	{
//		itemMgr->m_vtItemGroup[groupid]->AddItem(id);
//		item->AddGroup(groupid);
		item->setMyGroupID(groupid);
		itemMgr->m_vtItem[id] = item;
		itemMgr->m_mpItem[item->getName()] = item;
		return true;
	}
	return false;
}
