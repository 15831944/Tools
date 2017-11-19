#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "PropertyItems.h"
#include "ProjectMgr.h"
#include "Project.h"

#include "Item.h"
#include "SourceProperty.h"
#include "ItemMgr.h"
#include "ItemGroup.h"
#include "ScriptEditDlg.h"

const CString ITEM_TITLE = _T("��������ͳһ�޸�");

const CString ITEM_BASE_INFO = _T("����������Ϣ");
const CString ITEM_BASE_INFO_TOOLTIP = _T("����������Ϣ");
const CString ITEM_VALTYPE = _T("����ֵ����");
const CString ITEM_VALTYPE_TOOLTIP = _T("����ֵ����");
const CString ITEM_SRCTYPE = _T("����Դ����");
const CString ITEM_SRCTYPE_TOOLTIP = _T("����Դ����");
const CString ITEM_GROUP = _T("����������");
const CString ITEM_GROUP_TOOLTIP = _T("����������");
const CString ITEM_ACCESSRIGHT = _T("����Ȩ��");
const CString ITEM_ACCESSRIGHT_TOOLTIP = _T("����Ȩ��");
const CString ITEM_DEFAULTVALUE = _T("Ĭ��ֵ");
const CString ITEM_DEFAULTVALUE_TOOLTIP = _T("Ĭ��ֵ");
const CString ITEM_RESERVE = _T("�Ƿ���ֵ");
const CString ITEM_RESERVE_TOOLTIP = _T("�Ƿ���ֵ");
const CString ITEM_RESFDB = _T("�Ƿ񱣴���ʷ����");
const CString ITEM_RESFDB_TOOLTIP = _T("�Ƿ񱣴���ʷ����");
const CString ITEM_MODBUS485 = _T("�Ƿ񵼳�Modbus����");
const CString ITEM_MODBUS485_TOOLTIP = _T("���⴮��485�����Ե����ı����ṩModbusЭ����ʷ���");

const CString SRC_INFO = _T("����Դ��Ϣ");
const CString SRC_INFO_TOOLTIP = _T("����Դ��Ϣ");
const CString SRC_FRESHTIME = _T("����ˢ��ʱ��,��λ1ms");
const CString SRC_FRESHTIME_TOOLTIP = _T("����ˢ��ʱ��,��λ1ms");
const CString SRC_CONVERT = _T("ת������");
const CString SRC_CONVERT_TOOLTIP = _T("ת�����ͣ�����IO������Ч");
const CString SRC_MINPROJ = _T("��С����ֵ");
const CString SRC_MINPROJ_TOOLTIP = _T("��С����ֵ������IO������Ч");
const CString SRC_MAXPROJ = _T("��󹤳�ֵ");
const CString SRC_MAXPROJ_TOOLTIP = _T("��󹤳�ֵ������IO������Ч");
const CString SRC_MINIO = _T("��Сԭʼֵ");
const CString SRC_MINIO_TOOLTIP = _T("��Сԭʼֵ������IO������Ч");
const CString SRC_MAXIO = _T("���ԭʼֵ");
const CString SRC_MAXIO_TOOLTIP = _T("���ԭʼֵ������IO������Ч");

const UINT ITEM_VALTYPE_ID = 1;
const UINT ITEM_SRCTYPE_ID = ITEM_VALTYPE_ID + 1;
const UINT ITEM_GROUP_ID = ITEM_SRCTYPE_ID + 1;
const UINT ITEM_ACCESSRIGHT_ID = ITEM_GROUP_ID + 1;
const UINT ITEM_DEFAULTVALUE_ID = ITEM_ACCESSRIGHT_ID + 1;
const UINT ITEM_RESERVE_ID = ITEM_DEFAULTVALUE_ID + 1;
const UINT ITEM_RESFDB_ID = ITEM_RESERVE_ID + 1;
const UINT ITEM_MODBUS485_ID = ITEM_RESFDB_ID + 1;

const UINT SRC_FRESHTIME_ID = 101;
const UINT SRC_CONVERT_ID = SRC_FRESHTIME_ID + 1;
const UINT SRC_MINPROJ_ID = SRC_CONVERT_ID + 1;
const UINT SRC_MAXPROJ_ID = SRC_MINPROJ_ID + 1;
const UINT SRC_MINIO_ID = SRC_MAXPROJ_ID + 1;
const UINT SRC_MAXIO_ID = SRC_MINIO_ID + 1;

using namespace MVC;
using namespace Item;
CPropertyItems::CPropertyItems(void)
{
	m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));
}

CPropertyItems::~CPropertyItems(void)
{
}

// ��ñ����������
CString CPropertyItems::GetTitle()
{
	return ITEM_TITLE;
}

// ��ʾ���ĺ���
void CPropertyItems::ShowInfo(CXTPPropertyGrid& grid)
{
	CreateEdit();

	CXTPPropertyGridItem* pGroup;					// ������
	std::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CPMApp *)AfxGetApp())->m_XmlMgr;		// ������Ϣ
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::list<CString> strList, boolList;			// �����б������
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));
	CComVariant cvr = _T("");
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();

	// ����������Ϣ
	pGroup = grid.AddCategory(ITEM_BASE_INFO);
	pGroup->SetTooltip(ITEM_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	// ����ֵ����
	strList.clear();
	strList.push_back(_T("λ"));
	strList.push_back(_T("�з����ֽ�"));
	strList.push_back(_T("�޷����ֽ�"));
	strList.push_back(_T("�з�����"));
	strList.push_back(_T("�޷�����"));
	strList.push_back(_T("�з���˫��"));
	strList.push_back(_T("�޷���˫��"));
	strList.push_back(_T("�����ȸ���"));
//	strList.push_back(_T("˫���ȸ���"));
	if(m_bValType)
		AddItemList(*pGroup, ITEM_VALTYPE, ITEM_VALTYPE_TOOLTIP, strList, m_ShowItem->getValType(), ITEM_VALTYPE_ID);
	else
		AddItemList(*pGroup, ITEM_VALTYPE, ITEM_VALTYPE_TOOLTIP, strList, -1, ITEM_VALTYPE_ID);

	// ����Դ����
	strList.clear();
	strList.push_back(_T("�ڴ����"));
	strList.push_back(_T("I/O����"));
	if(m_bSrcType)
		AddItemList(*pGroup, ITEM_SRCTYPE, ITEM_SRCTYPE_TOOLTIP, strList, m_ShowItem->getSrcType(), ITEM_SRCTYPE_ID);
	else
		AddItemList(*pGroup, ITEM_SRCTYPE, ITEM_SRCTYPE_TOOLTIP, strList, -1, ITEM_SRCTYPE_ID);

	// ����������
	strList.clear();
	CString myGroupName = m_ShowItem->GetGroupName();
	std::shared_ptr<CItemGroup> group;
	int dftGroup = 0;
	for (auto group : itemMgr->m_vtItemGroup)
	{
		if(!group)			continue;
		if(group->getName() == myGroupName)		dftGroup = strList.size();
		strList.push_back(group->getName());
	}
	if(m_bGroup)
		AddItemList(*pGroup, ITEM_GROUP, ITEM_GROUP_TOOLTIP, strList, dftGroup, ITEM_GROUP_ID);
	else
		AddItemList(*pGroup, ITEM_GROUP, ITEM_GROUP_TOOLTIP, strList, -1, ITEM_GROUP_ID);

	// ����Ȩ��
	strList.clear();
	strList.push_back(_T("�ɶ���д"));
	strList.push_back(_T("ֻ��"));
	strList.push_back(_T("ֻд"));
	if(m_bAccessRight)
		AddItemList(*pGroup, ITEM_ACCESSRIGHT, ITEM_ACCESSRIGHT_TOOLTIP, strList, m_ShowItem->getAccessRight(), ITEM_ACCESSRIGHT_ID);
	else
		AddItemList(*pGroup, ITEM_ACCESSRIGHT, ITEM_ACCESSRIGHT_TOOLTIP, strList, -1, ITEM_ACCESSRIGHT_ID);

	// Ĭ��ֵ����
	AddItemVariant(*pGroup, ITEM_DEFAULTVALUE, ITEM_DEFAULTVALUE_TOOLTIP, m_ShowItem->getDefault(), ITEM_DEFAULTVALUE_ID, !m_bDefValue);

	// �Ƿ��Ǳ���ֵ
	if(m_bReservFlag)
		AddItemList(*pGroup, ITEM_RESERVE, ITEM_RESERVE_TOOLTIP, boolList, m_ShowItem->getReservFlag()?1:0, ITEM_RESERVE_ID);
	else
		AddItemList(*pGroup, ITEM_RESERVE, ITEM_RESERVE_TOOLTIP, boolList, -1, ITEM_RESERVE_ID);

	// �Ƿ��Ǳ������ݿ�
	if(m_bReservDB)
		AddItemList(*pGroup, ITEM_RESFDB, ITEM_RESFDB_TOOLTIP, boolList, m_ShowItem->getReservDB()?1:0, ITEM_RESFDB_ID);
	else
		AddItemList(*pGroup, ITEM_RESFDB, ITEM_RESFDB_TOOLTIP, boolList, -1, ITEM_RESFDB_ID);

	// ����Դ��Ϣ
	pGroup = grid.AddCategory(SRC_INFO);
	pGroup->SetTooltip(SRC_INFO_TOOLTIP);
	pGroup->Expand();

	// ˢ��ʱ��
	AddItemNumber(*pGroup, SRC_FRESHTIME, SRC_FRESHTIME_TOOLTIP, showSrc->getFreshTime(), SRC_FRESHTIME_ID, !m_bFreshTime);

	// ת������
	strList.clear();
	strList.push_back(_T("��ת��"));
	strList.push_back(_T("����ת��"));
	if(m_bConvertFlag)
		AddItemList(*pGroup, SRC_CONVERT, SRC_CONVERT_TOOLTIP, strList, showSrc->getConvertType(), SRC_CONVERT_ID);
	else
		AddItemList(*pGroup, SRC_CONVERT, SRC_CONVERT_TOOLTIP, strList, -1, SRC_CONVERT_ID);

	// ��С����ֵ
	AddItemVariant(*pGroup, SRC_MINPROJ, SRC_MINPROJ_TOOLTIP, showSrc->getProjMin(), SRC_MINPROJ_ID, !m_bMinProj);

	// ��󹤳�ֵ
	AddItemVariant(*pGroup, SRC_MAXPROJ, SRC_MAXPROJ_TOOLTIP, showSrc->getProjMax(), SRC_MAXPROJ_ID, !m_bMaxProj);

	// ��Сԭʼֵ
	AddItemVariant(*pGroup, SRC_MINIO, SRC_MINIO_TOOLTIP, showSrc->getIOMin(), SRC_MINIO_ID, !m_bMinIO);

	// ���ԭʼֵ
	AddItemVariant(*pGroup, SRC_MAXIO, SRC_MAXIO_TOOLTIP, showSrc->getIOMax(), SRC_MAXIO_ID, !m_bMaxIO);
}

// �༭ǰ������׼������
void CPropertyItems::CreateEdit()
{
	// ����Ĭ�ϣ�����ͬ
	m_bValType = true;			// ����ֵ�����Ƿ���ͬ
	m_bSrcType = true;			// ����Դ�����Ƿ���ͬ
	m_bGroup = true;			// �����������Ƿ���ͬ
	m_bDefValue = true;			// Ĭ��ֵ�����Ƿ���ͬ
	m_bAccessRight = true;		// ����Ȩ���Ƿ���ͬ
	m_bReservFlag = true;		// �Ƿ��Ǳ���ֵ�Ƿ���ͬ
	m_bReservDB = true;			// �Ƿ��Ǳ������ݿ��Ƿ���ͬ
	m_bFreshTime = true;		// ˢ��ʱ���Ƿ���ͬ
	m_bConvertFlag = true;		// ת�������Ƿ���ͬ
	m_bMinProj = true;			// ��С����ֵ�Ƿ���ͬ
	m_bMaxProj = true;			// ��󹤳�ֵ�Ƿ���ͬ
	m_bMinIO = true;			// ��Сԭʼֵ�Ƿ���ͬ
	m_bMaxIO = true;			// ���ԭʼֵ�Ƿ���ͬ

	// �Ƚ�ÿһ�����ҵ���Щ����һ������Щ���Բ�һ��
	*m_ShowItem = *m_ltEditItem.front();
	std::shared_ptr<CItem> item, front;
	front = m_ltEditItem.front();
	std::shared_ptr<CPropertySource> showSrc, itemSrc;
	showSrc = m_ShowItem->getSrcInfo();
	ASSERT(showSrc);
	for (auto item : m_ltEditItem)
	{
		if(item == front)		continue;
		itemSrc = item->getSrcInfo();
		ASSERT(itemSrc);
		// ��ʼ����ƥ��
		if(m_bValType)			m_bValType = (m_ShowItem->getValType() == item->getValType());
		if(m_bSrcType)			m_bSrcType = (m_ShowItem->getSrcType() == item->getSrcType());
		if(m_bGroup)			m_bGroup = (m_ShowItem->getMyGroupID() == item->getMyGroupID());
		if(m_bDefValue)			m_bDefValue = (m_ShowItem->getDefault() == item->getDefault());
		if(m_bAccessRight)		m_bAccessRight = (m_ShowItem->getAccessRight() == item->getAccessRight());
		if(m_bReservFlag)		m_bReservFlag = (m_ShowItem->getReservFlag() == item->getReservFlag());
		if(m_bReservDB)			m_bReservDB = (m_ShowItem->getReservDB() == item->getReservDB());

		if(m_bFreshTime)		m_bFreshTime = (showSrc->getFreshTime() == itemSrc->getFreshTime());
		if(m_bConvertFlag)		m_bConvertFlag = (showSrc->getConvertType() == itemSrc->getConvertType());
		if(m_bMinProj)			m_bMinProj = (showSrc->getProjMin() == itemSrc->getProjMin());
		if(m_bMaxProj)			m_bMaxProj = (showSrc->getProjMax() == itemSrc->getProjMax());
		if(m_bMinIO)			m_bMinIO = (showSrc->getIOMin() == itemSrc->getIOMin());
		if(m_bMaxIO)			m_bMaxIO = (showSrc->getIOMax() == itemSrc->getIOMax());
	}
}

// Ҫ���汻�ı������
bool CPropertyItems::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	CString itemValue;
	UINT itemID;
	CComVariant cvr;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<CItem> pItem = m_ltEditItem.front();
	std::shared_ptr<CPropertySource> itemSrc = pItem->getSrcInfo();
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();
	ASSERT(pItem);
	ASSERT(itemSrc);
	ASSERT(showSrc);

	m_bValType = true;			// ����ֵ�����Ƿ���ͬ
	m_bSrcType = true;			// ����Դ�����Ƿ���ͬ
	m_bGroup = true;			// �����������Ƿ���ͬ
	m_bDefValue = true;			// Ĭ��ֵ�����Ƿ���ͬ
	m_bAccessRight = true;		// ����Ȩ���Ƿ���ͬ
	m_bReservFlag = true;		// �Ƿ��Ǳ���ֵ�Ƿ���ͬ
	m_bReservDB = true;			// �Ƿ��Ǳ������ݿ��Ƿ���ͬ
	m_bFreshTime = true;		// ˢ��ʱ���Ƿ���ͬ
	m_bConvertFlag = true;		// ת�������Ƿ���ͬ
	m_bMinProj = true;			// ��С����ֵ�Ƿ���ͬ
	m_bMaxProj = true;			// ��󹤳�ֵ�Ƿ���ͬ
	m_bMinIO = true;			// ��Сԭʼֵ�Ƿ���ͬ
	m_bMaxIO = true;			// ���ԭʼֵ�Ƿ���ͬ

	for(int i = 0; i < num; ++i){
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			// ֻ�б��޸ĵĲŻ���б��棬�Ż���һ��
		itemID = item->GetID();
		itemValue = item->GetValue();

		if(ITEM_VALTYPE_ID == itemID){						// ����ֵ����
			m_ShowItem->setValType(item->GetConstraints()->GetCurrent());
//			m_bValType = (m_ShowItem->getValType() == pItem->getValType());
			m_bValType = false;
		}
		else if(ITEM_SRCTYPE_ID == itemID){					// ����Դ����
			m_ShowItem->setSrcType(item->GetConstraints()->GetCurrent());
//			m_bSrcType = (m_ShowItem->getSrcType() == pItem->getSrcType());
			m_bSrcType = false;
		}
		else if(ITEM_GROUP_ID == itemID){					// �����������Ƿ���ͬ
			std::shared_ptr<CItemGroup> itemGroup = itemMgr->GetGroup(itemValue.Trim());
			m_ShowItem->setMyGroupID(itemGroup ? itemGroup->getID() : 0);
			m_bGroup = false;
		}
		else if(ITEM_ACCESSRIGHT_ID == itemID){				// ����Ȩ��
			m_ShowItem->setAccessRight(item->GetConstraints()->GetCurrent());
			m_bAccessRight = false;
		}
		else if(ITEM_DEFAULTVALUE_ID == itemID){			// Ĭ��ֵ
			CComVariant cvrDef = item->GetValue();
			cvrDef.ChangeType(m_ShowItem->getDefault().vt);
			m_ShowItem->setDefault(cvrDef);
			m_bDefValue = false;
		}
		else if(ITEM_RESERVE_ID == itemID){					// �Ƿ���
			m_ShowItem->setReservFlag(item->GetConstraints()->GetCurrent());
			m_bReservFlag = false;
		}
		else if(ITEM_RESFDB_ID == itemID){					// �Ƿ�����ݿ�
			m_ShowItem->setReservDB(item->GetConstraints()->GetCurrent());
			m_bReservDB = false;
		}
		else if(SRC_FRESHTIME_ID == itemID){				// ˢ��ʱ��
			showSrc->setFreshTime(((CXTPPropertyGridItemNumber *)item)->GetNumber());
			m_bFreshTime = false;
		}
		else if(SRC_CONVERT_ID == itemID){					// ת������
			showSrc->setConvertType(item->GetConstraints()->GetCurrent());
			m_bConvertFlag = false;
		}
		else if(SRC_MINPROJ_ID == itemID){					// ��С����ֵ
			CComVariant cvrMinProj = item->GetValue();
			cvrMinProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMin(cvrMinProj);
			m_bMinProj = false;
		}
		else if(SRC_MAXPROJ_ID == itemID){					// ��󹤳�ֵ
			CComVariant cvrMaxProj = item->GetValue();
			cvrMaxProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMax(cvrMaxProj);
			m_bMaxProj = false;
		}
		else if(SRC_MINIO_ID == itemID){					// ��Сԭʼֵ
			CComVariant cvrMinIO = item->GetValue();
			cvrMinIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMin(cvrMinIO);
			m_bMinIO = false;
		}
		else if(SRC_MAXIO_ID == itemID){					// ���ԭʼֵ
			CComVariant cvrMaxIO = item->GetValue();
			cvrMaxIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMax(cvrMaxIO);
			m_bMaxIO = false;
		}
	}

	// ��ʼ�޸����б���������
	for (auto pItem : m_ltEditItem)
	{
		ASSERT(pItem);
		itemSrc = pItem->getSrcInfo();
		ASSERT(itemSrc);

		if(!m_bValType)				pItem->setValType(m_ShowItem->getValType());
		if(!m_bSrcType)				pItem->setSrcType(m_ShowItem->getSrcType());
		if(!m_bGroup)				pItem->setMyGroupID(m_ShowItem->getMyGroupID());
		if(!m_bDefValue)			pItem->setDefault(m_ShowItem->getDefault());
		if(!m_bAccessRight)			pItem->setAccessRight(m_ShowItem->getAccessRight());
		if(!m_bReservFlag)			pItem->setReservFlag(m_ShowItem->getReservFlag());
		if(!m_bReservDB)			pItem->setReservDB(m_ShowItem->getReservDB());
		if(!m_bFreshTime)			itemSrc->setFreshTime(showSrc->getFreshTime());
		if(!m_bConvertFlag)			itemSrc->setConvertType(showSrc->getConvertType());
		if(!m_bMinProj)				itemSrc->setProjMin(showSrc->getProjMin());
		if(!m_bMaxProj)				itemSrc->setProjMax(showSrc->getProjMax());
		if(!m_bMinIO)				itemSrc->setIOMin(showSrc->getIOMin());
		if(!m_bMaxIO)				itemSrc->setIOMax(showSrc->getIOMax());

		if(!m_bReservFlag || !m_bReservDB)
			CProjectMgr::GetMe().GetProj()->SetModify(true);
	}
	return true;
}

// ��ĳһ��޸�ʱ
void CPropertyItems::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)			return;
	CString itemValue = item->GetValue();
}

// ��ʾ������Ϣ
void CPropertyItems::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("ͳһ�޸�"));
}