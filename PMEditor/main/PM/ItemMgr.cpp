#include "stdafx.h"
#include "DXPEditor.h"
#include "Gbl.h"
//#include "Power.h"
#include "MainFrm.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "ItemInConfigDlg.h"

#include "Item.h"
#include "SourceProperty.h"
#include "AlarmProperty.h"
#include "ItemGroup.h"
#include "ItemMgr.h"
#include "ItemDoc.h"
#include "ItemView.h"
#include "ItemFrame.h"
#include "FindDlg.h"

using namespace MVC;
using namespace Item;

const CString ITEM_EXPAND_NAME = _T("var");			//!< ��չ��

const CString INFO_HEAD = _T("Infomation");
const CString VERSION = _T("Version");
const CString GROUPS = _T("Group");
const CString ITEM = _T("Item");

const CString ITEM_OUTFILE = _T("ItemOutFile");		//!< ���������ı�־

CItemMgr::CItemMgr(void)
	: m_pItemDoc(NULL)
{
	m_vtItem.reserve(50000);
}

CItemMgr::~CItemMgr(void)
{}

CItemMgr& CItemMgr::GetMe()
{
	static CItemMgr me;
	return me;
}

CString CItemMgr::GetEditTime()
{
	if(IsModify())		::GetLocalTime(&m_EditTime);
	return CGbl::GetMe().GetSystemTime2Str(m_EditTime);
}

std::list<UINT> CItemMgr::GetItemIDAll()
{
	std::list<UINT> ltItemID;
	for (std::shared_ptr<CItem> item : m_vtItem)
		if(item)
			ltItemID.push_back(item->getID());

	return ltItemID;
}

std::list<UINT> CItemMgr::GetItemIDMem()
{
	std::list<UINT> ltItemID;
	for (std::shared_ptr<CItem> item : m_vtItem)
		if(item && item->getSrcType() == CItem::SRC_TYPE_MEM)
			ltItemID.push_back(item->getID());

	return ltItemID;
}

std::list<UINT> CItemMgr::GetItemIDIO()
{
	std::list<UINT> ltItemID;
	for (std::shared_ptr<CItem> item : m_vtItem)
		if(item && item->getSrcType() == CItem::SRC_TYPE_IO)
			ltItemID.push_back(item->getID());

	return ltItemID;
}

//!< ���id�ű���
std::shared_ptr<CItem> CItemMgr::GetItem(ULONG id)
{
	std::shared_ptr<CItem> _item;
	if(id >= m_vtItem.size())
		return _item;
	return m_vtItem[id];
}

//!< �õ�����Ϊname�ı���Ԫ��
std::shared_ptr<CItem> CItemMgr::GetItem(CString name)
{
	for (std::shared_ptr<CItem> item : m_vtItem){
		if(!item)						continue;
		if(item->getName() == name)		return item;
	}
	std::shared_ptr<CItem> _empty;
	return _empty;
}

//!< ��ñ���,�ÿ��ٵĻ�÷�ʽ
std::shared_ptr<CItem> CItemMgr::GetItemFast(CString name)
{
	return m_mpItem[name];
}

//!< ��õ�һ������
std::shared_ptr<CItem> CItemMgr::GetFirstItem()
{
	std::vector<std::shared_ptr<CItem> >::iterator iter = m_vtItem.begin();
	for(;iter !=  m_vtItem.end(); ++iter){
		if(*iter)		return *iter;
	}
	std::shared_ptr<CItem> empty;
	return empty;
}

//!< ������һ������
std::shared_ptr<CItem> CItemMgr::GetLastItem()
{
	std::vector<std::shared_ptr<CItem> >::reverse_iterator iter = m_vtItem.rbegin();
	for(;iter !=  m_vtItem.rend(); ++iter){
		if(*iter)		return *iter;
	}
	std::shared_ptr<CItem> empty;
	return empty;
}

std::shared_ptr<CItemGroup> CItemMgr::GetGroup(UINT id)
{
	std::shared_ptr<CItemGroup> _group;
	if(id >= m_vtItemGroup.size())
		return _group;
	return m_vtItemGroup[id];
}

std::shared_ptr<CItemGroup> CItemMgr::GetGroup(CString name)
{
	for (std::shared_ptr<CItemGroup> _group : m_vtItemGroup){
		if(!_group)							continue;
		if(_group->getName() == name)		return _group;
	}
	std::shared_ptr<CItemGroup> _empty;
	return _empty;
}

//!< ��ĳ����������±��������ڱ�����������
//!< �û���ӵı������������������Ĳ���
bool CItemMgr::AddItem(std::shared_ptr<CItem> item,int maxID,UINT groupid)
{
	if(!item)												return false;
	if(!FindGroup(groupid))									return false;
	maxID = maxID >> 1;
	int id = -1;
	for(UINT i = 0; i < m_vtItem.size(); ++i)
	{
		if(m_vtItem[i])										continue;
		if(i >= (maxID << 1))								goto SAY_ERROR;
		id = i;
		break;
		//item->setID(i);										//!< �ҵ���һ���յ�λ�ã���item�ı�Ÿ���
		//item->setMyGroupID(groupid);
		//m_vtItem[i]=item;
		//m_mpItem[item->getName()] = item;
		//return true;
	}
	if (id == -1)	id = m_vtItem.size();
	//!< ���ǰ��û�пյ�λ�ã����������
	if (id < (maxID << 1))			//!< �������ܳ���Ȩ��ֵ
	{
		item->setID((UINT)id);		//!< ���ñ���ID��
		item->setMyGroupID(groupid);
		if (id == m_vtItem.size())	m_vtItem.push_back(item);
		else						m_vtItem[id] = item;
		m_mpItem[item->getName()] = item;
		return true;
	}
SAY_ERROR:
	CString strError;
	strError.Format("�������������� %d �����޷�����ӱ�����", maxID << 1);
	MessageBox(NULL, strError, _T("����"), MB_OK);
	return false;
}

//!< �ڱ�������������
bool CItemMgr::AddItemBack(std::shared_ptr<CItem> item, int maxID, UINT groupid)
{
	if (!item)												return false;
	if (!FindGroup(groupid))								return false; 
	maxID = maxID >> 1;
	if (m_vtItem.size() >= (maxID << 1))					goto SAY_ERROR;
	int id = m_vtItem.size();
	//!< ���ǰ��û�пյ�λ�ã����������
	if (id < (maxID << 1))			//!< �������ܳ���Ȩ��ֵ
	{
		item->setID((UINT)id);		//!< ���ñ���ID��
		item->setMyGroupID(groupid);
		m_vtItem.push_back(item);
		m_mpItem[item->getName()] = item;
		return true;
	}
SAY_ERROR:
	CString strError;
	strError.Format("�������������� %d �����޷�����ӱ�����", maxID << 1);
	MessageBox(NULL, strError, _T("����"), MB_OK);
	return false;
}

//!< ɾ����β�Ŀձ���
void CItemMgr::RemoveItemBackEmpty()
{
	int nIndex = -1;
	for (int i = m_vtItem.size() - 1; i >= 0; --i)
	{
		if (!m_vtItem[i])	continue;
		nIndex = i;
		break;
	}
	if (nIndex >= 0)
		m_vtItem.resize(nIndex + 1);
}

//!< ֻ�ʺ��û�����飬���ʺϽ��������ļ���õ���
bool CItemMgr::AddGroup(std::shared_ptr<CItemGroup> group,UINT parentid)
{
	if(!group)												return false;
	if(parentid!=-1 && !FindGroup(parentid))				return false;
	for(UINT i = 0; i < m_vtItemGroup.size(); ++i)
	{
		if(m_vtItemGroup[i])	continue;
		group->setID(i);		//!< �ҵ���һ���յ�λ�ã���group�ı�Ÿ���
		m_vtItemGroup[i] = group;
		return true;
	}
	//!< ���ǰ��û�пյ�λ�ã����������
	if(m_vtItemGroup.size() < CGbl::GetMe().getMaxGroupNum())	//!< �������ܳ���Ȩ��ֵ
	{
		group->setID((UINT)m_vtItemGroup.size());		//!< ���ñ���ID��
		m_vtItemGroup.push_back(group);
		return true;
	}
	else
	{
		MessageBox(NULL, _T("�������������ƣ��޷�����ӱ����飡"), _T("����"), MB_OK);
	}
	return false;
}

//!< ���ұ��Ϊno��Ԫ�أ��������ı�ţ�û�з���-1
bool CItemMgr::FindItem(ULONG id)
{
	if(id >= m_vtItem.size())			return false;
	return !!m_vtItem[id];
}

bool CItemMgr::FindGroup(UINT id)
{
	if(id >= m_vtItemGroup.size())		return false;
	return !!m_vtItemGroup[id];
}

//!< ɾ�����Ϊno��Ԫ�أ�����ɾ��
void CItemMgr::DeleteItem(ULONG id)
{
	std::shared_ptr<CItem> pItem = GetItem(id);
	if (!pItem)							return;
	m_vtItem[id].reset();
	m_mpItem[pItem->getName()].reset();
}

//!< ��������������
void CItemMgr::ExChangeItem(UINT id1, UINT id2)
{
	UINT size = (UINT)m_vtItem.size();
	ASSERT(id1 < size && id2 < size);
	std::shared_ptr<CItem> item1, item2;
	item1 = GetItem(id1);
	item2 = GetItem(id2);
	if(item1){	item1->setID(id2);	m_vtItem[id2] = item1;	}
	if(item2){	item2->setID(id1);	m_vtItem[id1] = item2;	}
}

void CItemMgr::RemoveGroup(UINT groupid)
{
	if(groupid == 0)	return;					//!< System�����Ƴ�
	std::shared_ptr<CItemGroup> _group = GetGroup(groupid);
	if(!_group)			return;

	if(_group->m_ItemDoc)	_group->m_ItemDoc->OnCloseDocument();		//!< �ر���ʾ�˱������������ĵ���ͼ
	std::list<UINT> ltItem = _group->getMyItem();
	for (UINT id : ltItem){
		DeleteItem(id);							//!< ɾ�������ı���
	}
	std::list<UINT>& ltGroup = _group->getGroupIDList();
	for (UINT id : ltGroup)
		RemoveGroup(id);						//!< �ݹ�ɾ����������
	if(m_vtItemGroup[groupid])		m_vtItemGroup[groupid].reset();
}

//!< ��ñ���������
UINT CItemMgr::GetItemSize()
{
	UINT count = 0;
	for (std::shared_ptr<CItem> item : m_vtItem){
		if(item)		++count;
	}
	return count;
}

//!< ��ñ����������
UINT CItemMgr::GetGroupSize()
{
	UINT count = 0;
	for (std::shared_ptr<CItemGroup> group : m_vtItemGroup){
		if(group)		++count;
	}
	return count;
}

//!< �򿪱������ļ������� �� ���� + ·�� + �汾�ţ�����޸�ʱ��
bool CItemMgr::OpenItemFile(CString name, CString pathall, CString ver, CString stime)
{
	if(!CGbl::SetSystemTimeFromStr(m_EditTime, stime))		::GetLocalTime(&m_EditTime);
	m_strName = name;
	m_strFileName = m_strName + _T(".") + ITEM_EXPAND_NAME;
	m_strVersion = ver;
//	CGbl::StartHighTime();
	TiXmlDocument pTiXml((LPCTSTR)pathall);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("�������ļ���ʽ����"), _T("����"), MB_ICONEXCLAMATION);
		return false;
	}
	//!< ��ʼ����
	SerializeXmlItem(pTiXml.RootElement(), true);
//	double t=CGbl::GetHighTime();
//	CString tm;
///	tm.Format("%.6fms",t*1000);
//	CGbl::PrintOut(tm);
	return true;
}

void CItemMgr::SaveItemFile()
{
	if(IsModify())
	{
		std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
		CString pathAll = proj->GetPath() + m_strName + _T(".") + ITEM_EXPAND_NAME;
		TiXmlDocument pTiXml(pathAll);

		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< ��ʼ����
		pTiXml.LinkEndChild(decl);
		TiXmlComment * comment = new TiXmlComment();							//!< ע����Ϣ
		comment->SetValue(_T("�����ֹ��༭���ĵ�"));
		pTiXml.LinkEndChild(comment);
		TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					//!< д����ڵ�
		
		SerializeXmlItem(pRoot, false);												//!< ��ӹ��̽ڵ�,д����
		pTiXml.LinkEndChild(pRoot);
		if(!pTiXml.SaveFile()){
			MessageBox(NULL, _T("����ʧ�ܣ�"), _T("����"), MB_ICONEXCLAMATION);
			return ;
		}
	}
	CGbl::SetProgressStep(1);
	m_bModify = false;
}

bool CItemMgr::SerializeXmlItem(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		//!< ����汾���Ȳ�����
		//!< �ӽڵ�
		CString text, strValue;
		TiXmlElement* pChild = pNode->FirstChildElement();
		std::list<std::shared_ptr<CItemGroup> > ltGroup;
		std::list<std::shared_ptr<CItem> > ltItem;
		std::shared_ptr<CItemGroup> group;
		std::shared_ptr<CItem> item;
		int maxCount = MAX_ITEM_COUNT;
		int count = 0;
		while(pChild)
		{
			text = pChild->ValueStr().c_str();
			if(GROUPS == text)
			{
				group = std::shared_ptr<CItemGroup>(new CItemGroup(_T(""), 0));
				if(group->SerializeXml(pChild, bRead))		ltGroup.push_back(group);
			}
			else if(ITEM == text)
			{
				++count;
				if(count > maxCount)
				{
					CString strError;
					strError.Format(_T("����������������%d�����������̱�����Ϣ��ֹ��"), maxCount);
					MessageBox(NULL, strError, _T("����"), MB_OK | MB_ICONWARNING);
					break;
				}
				item = std::shared_ptr<CItem>(new CItem(_T("")));
				if(item->SerializeXml(pChild, bRead))		ltItem.push_back(item);
			}
			pChild = pChild->NextSiblingElement();
		}
		UINT groupcount = 0, itemcount = 0;
		for (auto group : ltGroup)		groupcount = max(groupcount, group->getID());
		m_vtItemGroup.clear();
		m_vtItemGroup.resize(groupcount + 1);
		for (auto item : ltItem)		itemcount = max(itemcount, item->getID());
		m_mpItem.clear();
		m_vtItem.clear();
		m_vtItem.resize(itemcount + 1);
		for (auto group : ltGroup)		m_vtItemGroup[group->getID()] = group;
		for (auto item : ltItem)		{ m_vtItem[item->getID()] = item;	m_mpItem[item->getName()] = item; }
		//!< ÿ��������Ҫ��¼�Լ����ڵ���
		for (auto group : m_vtItemGroup)
		{
			if(!group)		continue;
//			group->InitItem();
		}
	}
	else
	{
		pNode->SetAttribute(_T("FileType"), _T("EditItemFile"));
		/*! ԭ����1.0���������ڴ��б������б����Դﵽ��Զ�Ĺ�ϵ�������ñ����������÷ǳ��鷳
			������1.1������ֻ��������һ���������ڣ���ϵ�������ˣ��û�ͨ���������Ҳ����
			���ڲ��������������Ĺ��̲���Ҫ�޸ģ��¾ɶ�����
		*/
		pNode->SetAttribute(_T("Version"), _T("1.1"));

		TiXmlElement* pXmlInfo = new TiXmlElement(INFO_HEAD);
		pNode->LinkEndChild(pXmlInfo);

		TiXmlElement* pChild;
		for (std::shared_ptr<CItemGroup> group : m_vtItemGroup)
		{
			if(!group)		continue;
			pChild = pNode->AddTiXmlChild((LPCTSTR)GROUPS);
			group->SerializeXml(pChild, false);
		}
		for (std::shared_ptr<CItem> item : m_vtItem)
		{
			if(!item)		continue;
			pChild = pNode->AddTiXmlChild((LPCTSTR)ITEM);
			item->SerializeXml(pChild, false);
			CGbl::SetProgressStep(1);
		}
	}
	return true;
}

//!< ��������
void CItemMgr::OutItem(std::list<UINT> ltItem)
{
	//CProjectMgr* projMgr = &CProjectMgr::GetMe();
	//std::shared_ptr<CProject> proj = projMgr->GetProj();
	//ASSERT(proj);
	//CString defFileName = _T("����������");
	//defFileName = defFileName;
	//CString extendNames = _T("���������ļ� (*.vxpt)|*.vxpt|");
	//extendNames = extendNames + _T("Excel�ļ� (*.xls)|*.xls|");
	//extendNames = extendNames + _T("Txt�ļ� (*.txt)|*.txt|");
	//extendNames = extendNames + _T("|");
	//CFileDialog saveDlg(FALSE, (LPCTSTR)extendNames, (LPCTSTR)defFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	//if(IDOK == saveDlg.DoModal())
	//{
	//	CString strExt = saveDlg.GetFileExt();
	//	if(strExt == _T("xls") || strExt == _T("txt"))
	//	{
	//		COperation excelop;
	//		std::shared_ptr<CItem> pItem;
	//		std::shared_ptr<CPropertySource> pSrc;
	//		std::shared_ptr<CPropertyAlarm> pAlarm;
	//		int nRow = 0;
	//		CString strValue;
	//		CComVariant cvrValue;

	//		//!< ����������
	//		excelop.FileGuid(FALSE, saveDlg.GetFileName(), saveDlg.GetPathName());
	//		CGbl::SetProgressRange(ltItem.size() + 1);
	//		CGbl::SetProgressStep(1);

	//		//!< ���ر���
	//		strValue = _T("���");			cvrValue = strValue;	excelop.WriteItem(nRow, 1, cvrValue);
	//		strValue = _T("����");			cvrValue = strValue;	excelop.WriteItem(nRow, 2, cvrValue);
	//		strValue = _T("��ǩ");			cvrValue = strValue;	excelop.WriteItem(nRow, 3, cvrValue);
	//		strValue = _T("��ע");			cvrValue = strValue;	excelop.WriteItem(nRow, 4, cvrValue);
	//		strValue = _T("��������");		cvrValue = strValue;	excelop.WriteItem(nRow, 5, cvrValue);
	//		strValue = _T("Ĭ��ֵ");		cvrValue = strValue;	excelop.WriteItem(nRow, 6, cvrValue);
	//		strValue = _T("����Դ����");	cvrValue = strValue;	excelop.WriteItem(nRow, 7, cvrValue);
	//		strValue = _T("����Ȩ��");		cvrValue = strValue;	excelop.WriteItem(nRow, 8, cvrValue);
	//		strValue = _T("����ֵ");		cvrValue = strValue;	excelop.WriteItem(nRow, 9, cvrValue);
	//		strValue = _T("������ʷ����");	cvrValue = strValue;	excelop.WriteItem(nRow, 10, cvrValue);
	//		strValue = _T("����ʱ��");		cvrValue = strValue;	excelop.WriteItem(nRow, 11, cvrValue);
	//		strValue = _T("�ű�");			cvrValue = strValue;	excelop.WriteItem(nRow, 12, cvrValue);
	//		strValue = _T("�豸");			cvrValue = strValue;	excelop.WriteItem(nRow, 13, cvrValue);
	//		strValue = _T("����ַ");		cvrValue = strValue;	excelop.WriteItem(nRow, 14, cvrValue);
	//		strValue = _T("IO����");		cvrValue = strValue;	excelop.WriteItem(nRow, 15, cvrValue);
	//		strValue = _T("Ѱַƫ��");		cvrValue = strValue;	excelop.WriteItem(nRow, 16, cvrValue);
	//		strValue = _T("λƫ��");		cvrValue = strValue;	excelop.WriteItem(nRow, 17, cvrValue);
	//		strValue = _T("ˢ��ʱ��");		cvrValue = strValue;	excelop.WriteItem(nRow, 18, cvrValue);
	//		strValue = _T("�����ͺ�");		cvrValue = strValue;	excelop.WriteItem(nRow, 19, cvrValue);
	//		strValue = _T("ת������");		cvrValue = strValue;	excelop.WriteItem(nRow, 20, cvrValue);
	//		strValue = _T("��С����ֵ");	cvrValue = strValue;	excelop.WriteItem(nRow, 21, cvrValue);
	//		strValue = _T("��󹤳�ֵ");	cvrValue = strValue;	excelop.WriteItem(nRow, 22, cvrValue);
	//		strValue = _T("��СIOֵ");		cvrValue = strValue;	excelop.WriteItem(nRow, 23, cvrValue);
	//		strValue = _T("���IOֵ");		cvrValue = strValue;	excelop.WriteItem(nRow, 24, cvrValue);
	//		strValue = _T("���ر�������");	cvrValue = strValue;	excelop.WriteItem(nRow, 25, cvrValue);
	//		strValue = _T("��ֵ����");		cvrValue = strValue;	excelop.WriteItem(nRow, 26, cvrValue);
	//		strValue = _T("�����ޱ�������");cvrValue = strValue;	excelop.WriteItem(nRow, 27, cvrValue);
	//		strValue = _T("�����ޱ���ֵ");	cvrValue = strValue;	excelop.WriteItem(nRow, 28, cvrValue);
	//		strValue = _T("���ޱ�������");	cvrValue = strValue;	excelop.WriteItem(nRow, 29, cvrValue);
	//		strValue = _T("���ޱ���ֵ");	cvrValue = strValue;	excelop.WriteItem(nRow, 30, cvrValue);
	//		strValue = _T("���ޱ�������");	cvrValue = strValue;	excelop.WriteItem(nRow, 31, cvrValue);
	//		strValue = _T("���ޱ���ֵ");	cvrValue = strValue;	excelop.WriteItem(nRow, 32, cvrValue);
	//		strValue = _T("�����ޱ�������");cvrValue = strValue;	excelop.WriteItem(nRow, 33, cvrValue);
	//		strValue = _T("�����ޱ���ֵ");	cvrValue = strValue;	excelop.WriteItem(nRow, 34, cvrValue);
	//		strValue = _T("Ŀ�걨������");	cvrValue = strValue;	excelop.WriteItem(nRow, 35, cvrValue);
	//		strValue = _T("Ŀ��ֵ");		cvrValue = strValue;	excelop.WriteItem(nRow, 36, cvrValue);
	//		strValue = _T("Ŀ��ƫ��ֵ");	cvrValue = strValue;	excelop.WriteItem(nRow, 37, cvrValue);
	//		strValue = _T("Ŀ������");		cvrValue = strValue;	excelop.WriteItem(nRow, 38, cvrValue);
	//		strValue = _T("�仯�ʱ�������");cvrValue = strValue;	excelop.WriteItem(nRow, 39, cvrValue);
	//		strValue = _T("���仯��");	cvrValue = strValue;	excelop.WriteItem(nRow, 40, cvrValue);
	//		strValue = _T("�仯ʱ��");		cvrValue = strValue;	excelop.WriteItem(nRow, 41, cvrValue);

	//		++nRow;

	//		//!< ���ر�����Ϣ
	//		for (UINT id : ltItem)
	//		{
	//			CGbl::SetProgressStep(1);
	//			pItem = GetItem(id);
	//			if(!pItem)		continue;
	//			pSrc = pItem->getSrcInfo();
	//			pAlarm = pItem->getAlarmInfo();

	//			cvrValue = pItem->getID();				excelop.WriteItem(nRow, 1, cvrValue);	//!< ����ID��
	//			cvrValue = pItem->getName();			excelop.WriteItem(nRow, 2, cvrValue);	//!< ��������
	//			cvrValue = pItem->getTag();				excelop.WriteItem(nRow, 3, cvrValue);	//!< ������ǩ
	//			cvrValue = pItem->getDescription();		excelop.WriteItem(nRow, 4, cvrValue);	//!< �����ı�ע��Ϣ
	//			cvrValue = pItem->getValType();			excelop.WriteItem(nRow, 5, cvrValue);	//!< �������ͣ�0-bit��1-char��2-byte��3-short��4-word��5-int��6-dword��7-float��8-double
	//			cvrValue = pItem->getDefault();			excelop.WriteItem(nRow, 6, cvrValue);	//!< ����Ĭ��ֵ
	//			cvrValue = pItem->getSrcType();			excelop.WriteItem(nRow, 7, cvrValue);	//!< ����Դ���ͣ�0�ڴ������1I/O����
	//			cvrValue = pItem->getAccessRight();		excelop.WriteItem(nRow, 8, cvrValue);	//!< ����Ȩ�ޣ�0�ɶ���д��1ֻ����ֻд
	//			cvrValue = pItem->getReservFlag();		excelop.WriteItem(nRow, 9, cvrValue);	//!< �Ƿ����óɱ���ֵ��true���ã�false������
	//			cvrValue = pItem->getReservDB();		excelop.WriteItem(nRow, 10, cvrValue);	//!< �Ƿ񱣴���ʷ����
	//			strValue = _T("2009-12-28");	cvrValue = strValue;	excelop.WriteItem(nRow, 11, cvrValue);	//!< ����ʱ��
	//			cvrValue = pSrc->getScriptText();		excelop.WriteItem(nRow, 12, cvrValue);	//!< ��ֵ�ű��ı�
	//			cvrValue = pSrc->getDeviceID();			excelop.WriteItem(nRow, 13, cvrValue);	//!< �豸���
	//			cvrValue = pSrc->getAreaID();			excelop.WriteItem(nRow, 14, cvrValue);	//!< �����
	//			cvrValue = pSrc->getIOType();			excelop.WriteItem(nRow, 15, cvrValue);	//!< IO���ͣ��������ݿ������ļ�
	//			cvrValue = pSrc->getUnitIndex();		excelop.WriteItem(nRow, 16, cvrValue);	//!< Ѱַƫ��
	//			cvrValue = pSrc->getBitIndex();			excelop.WriteItem(nRow, 17, cvrValue);	//!< λƫ��
	//			cvrValue = pSrc->getFreshTime();		excelop.WriteItem(nRow, 18, cvrValue);	//!< ����ˢ��ʱ��,��λ100����
	//			cvrValue = pSrc->getDelayFlag();		excelop.WriteItem(nRow, 19, cvrValue);	//!< �ɷ������ͺ�
	//			cvrValue = pSrc->getConvertType();		excelop.WriteItem(nRow, 20, cvrValue);	//!< ת�����ͣ�0��ת����1�Ƿ�ȡ��(λ����ʹ��)��1����ת��
	//			cvrValue = pSrc->getProjMin();			excelop.WriteItem(nRow, 21, cvrValue);	//!< ��С����ֵ
	//			cvrValue = pSrc->getProjMax();			excelop.WriteItem(nRow, 22, cvrValue);	//!< ��󹤳�ֵ
	//			cvrValue = pSrc->getIOMin();			excelop.WriteItem(nRow, 23, cvrValue);	//!< ��СIOֵ
	//			cvrValue = pSrc->getIOMax();			excelop.WriteItem(nRow, 24, cvrValue);	//!< ���IOֵ
	//			cvrValue = pAlarm->getBitAlarmType();	excelop.WriteItem(nRow, 25, cvrValue);	//!< ���ر������ͣ�0��������1��ʱ������2��ʱ������3�����ر�����4�ص���������5�仯�ͱ���
	//			cvrValue = pAlarm->getDeadArea();		excelop.WriteItem(nRow, 26, cvrValue);	//!< ��������ʼ�����ͻָ�����֮��Ľ��ޣ�Ϊ�˷��ñ��������������ı���ֵ�ͻָ�ֵ = ����ֵ �� ����ֵ
	//			cvrValue = pAlarm->getLoloActive();		excelop.WriteItem(nRow, 27, cvrValue);	//!< �����ޱ����Ƿ�����
	//			cvrValue = pAlarm->getLoloValue();		excelop.WriteItem(nRow, 28, cvrValue);	//!< �����ޱ���ֵ
	//			cvrValue = pAlarm->getLowActive();		excelop.WriteItem(nRow, 29, cvrValue);	//!< ���ޱ����Ƿ�����
	//			cvrValue = pAlarm->getLowValue();		excelop.WriteItem(nRow, 30, cvrValue);	//!< ���ޱ���ֵ
	//			cvrValue = pAlarm->getHighActive();		excelop.WriteItem(nRow, 31, cvrValue);	//!< ���ޱ����Ƿ�����
	//			cvrValue = pAlarm->getHighValue();		excelop.WriteItem(nRow, 32, cvrValue);	//!< ���ޱ���ֵ
	//			cvrValue = pAlarm->getHihiActive();		excelop.WriteItem(nRow, 33, cvrValue);	//!< �����ޱ����Ƿ�����
	//			cvrValue = pAlarm->getHihiValue();		excelop.WriteItem(nRow, 34, cvrValue);	//!< �����ޱ���ֵ
	//			cvrValue = pAlarm->getAimActive();		excelop.WriteItem(nRow, 35, cvrValue);	//!< Ŀ�걨���Ƿ�����
	//			cvrValue = pAlarm->getAimValue();		excelop.WriteItem(nRow, 36, cvrValue);	//!< Ŀ��ֵ
	//			cvrValue = pAlarm->getAimPercent();		excelop.WriteItem(nRow, 37, cvrValue);	//!< Ŀ��ƫ��
	//			cvrValue = pAlarm->getAimDeadPercent();	excelop.WriteItem(nRow, 38, cvrValue);	//!< Ŀ��ƫ������
	//			cvrValue = pAlarm->getShiftActive();	excelop.WriteItem(nRow, 39, cvrValue);	//!< �仯�ʱ����Ƿ�����
	//			cvrValue = pAlarm->getShiftDelta();		excelop.WriteItem(nRow, 40, cvrValue);	//!< ���仯��
	//			cvrValue = pAlarm->getShiftTime();		excelop.WriteItem(nRow, 41, cvrValue);	//!< �仯ʱ�䣬��λ��
	//			++nRow;
	//		}
	//		excelop.SaveFile();
	//		CGbl::SetProgressEnd();
	//	}
	//	else
	//	{
	//		TiXmlDocument pTiXml(saveDlg.GetPathName());
	//		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< ��ʼ����
	//		pTiXml.LinkEndChild(decl);
	//		TiXmlComment * comment = new TiXmlComment();							//!< ע����Ϣ
	//		comment->SetValue(_T("�����ֹ��༭���ĵ�"));
	//		pTiXml.LinkEndChild(comment);
	//		TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					//!< д����ڵ�
	//		SerializeOut(pRoot, ltItem);											//!< ��ӹ��̽ڵ�,д����
	//		pTiXml.LinkEndChild(pRoot);
	//		if(!pTiXml.SaveFile())
	//			MessageBox(NULL, _T("����ʧ�ܣ�"), _T("����"), MB_ICONEXCLAMATION);
	//		else
	//			MessageBox(NULL, _T("�����ɹ���"), _T("��ʾ"), MB_ICONINFORMATION);
	//	}
	//}
}

//!< ��������
bool CItemMgr::SerializeOut(TiXmlElement* pNode, std::list<UINT> ltItem)
{
	pNode->SetAttribute(_T("FileType"), ITEM_OUTFILE);
	pNode->SetAttribute(_T("Version"), _T("1.0"));

	TiXmlElement* pChild;
	std::shared_ptr<CItem> item;
	for (UINT id : ltItem){
		item = GetItem(id);
		if(!item)		continue;
		pChild = pNode->AddTiXmlChild((LPCTSTR)ITEM);
		item->SerializeXml(pChild, false, 1);
	}
	return true;
}

void CItemMgr::OnCreate()
{
	m_vtItemGroup.clear();
	m_mpItem.clear();
	m_vtItem.clear();
	if(m_pItemDoc)		m_pItemDoc->OnCloseDocument();
	SetModify();		//!< �½��Ĺ���Ĭ��Ϊ���޸�״̬

	//!< �������������ͱ����飬����ֻ��һ��System������
	std::shared_ptr<CItemGroup> group;
	group = std::shared_ptr<CItemGroup>(new CItemGroup(_T(""),UINT(-1)));
	group->InitDefault();
	m_vtItemGroup.push_back(group);

	//!< ��ʼ��·����Ϣ
	m_strName = _T("������");													//!< ���ֽ�"������"����������"p"
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	m_strFileName = m_strName + _T(".") + ITEM_EXPAND_NAME;						//!< ·����"p_������.var"
}

void CItemMgr::OnClose()
{
	m_vtItemGroup.clear();
	m_mpItem.clear();
	m_vtItem.clear();
	m_pItemDoc = NULL;
}

void CItemMgr::OpenDoc()
{
	if(!m_pItemDoc){
		m_pItemDoc=(MVC::Item::CItemDoc*)((CDXPEditorApp *)AfxGetApp())->m_pItemDocMgr->OpenDocumentFile(NULL);
		m_pItemDoc->ShowAllItem();
	}
	else{
		CView* view = (CView*)m_pItemDoc->GetView();
		CFrameWnd* frm = view->GetParentFrame();
		ASSERT(frm);
		if(frm->IsKindOf(RUNTIME_CLASS(CItemFrame)))
			frm->ActivateFrame();
	}
	m_pItemDoc->SetTitle(_T(""));
}

void CItemMgr::WriteItemValue(UINT id, bool proj, CString value)
{
	std::shared_ptr<CItem> pItem = GetItem(id);
	ASSERT(pItem);
	CMainFrame * mf = (CMainFrame *)g_App.GetMainWnd();
	CComVariant cvr = value;
	HRESULT hr;
	if(proj)	hr = cvr.ChangeType(pItem->getValue().vt);
	else		hr = cvr.ChangeType(pItem->getIOValue().vt);
	if(!SUCCEEDED(hr))
	{
		CGbl::PrintOut(_T("��ֵ���Ͳ�ƥ�䣬��鿴�Ƿ���ֵԽ��..."));
		return;
	}
	if(proj){
		mf->m_SevCommer.Write(id, cvr);
	}
	else{
		CComVariant _ccomvar;
		_ccomvar.ChangeType(VT_ARRAY|VT_VARIANT);
		_ccomvar.parray = ::SafeArrayCreateVector(VT_VARIANT,0,1);
		void* pDest;
		AfxCheckError(::SafeArrayAccessData(_ccomvar.parray, &pDest));
		memcpy(pDest, &cvr, sizeof(VARIANT));
		AfxCheckError(::SafeArrayUnaccessData(_ccomvar.parray));

		mf->m_SevCommer.Execute(5, _ccomvar, -1, id, 1);
	}
}

//!< �ı���״̬
void CItemMgr::SetItemWatch(const bool watch)
{
	if(!watch)		return;
	std::shared_ptr<CItem> item;
	for (auto item : m_vtItem){
		if(!item)		continue;
		item->WatchInit();
	}
}

void CItemMgr::ReadItemValue(bool bProj)
{
	std::shared_ptr<CItem> last_item = GetLastItem();
	if(!last_item)								return;		//!< �����ھ��˳�
	std::shared_ptr<CItem> item;
	UINT last_id = last_item->getID();
	UINT item_count = last_id + 1;

	CComVariant cvr;
	VARIANT safeArray;
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();

	//!< �����б����Ĺ���ֵ
	if(bProj)		safeArray = mf->m_SevCommer.ReadEx(0, last_id);
	else			safeArray = mf->m_SevCommer.Execute(4, cvr, -1, 0, item_count);

	VARIANT *pValue = NULL;
	if(safeArray.vt == (VT_ARRAY|VT_VARIANT))
	{
		SAFEARRAY *pArray = safeArray.parray;
		HRESULT hr = SafeArrayAccessData(pArray, (void**)&pValue);
		if(pArray->rgsabound->cElements != item_count)	return;		//!< ������ص������������˳�
		if(SUCCEEDED(hr))
		{
			for(UINT i = 0, j = 0; i <= last_id && j < item_count; ++i, ++j)
			{
				item = GetItem(i);
				if(!item || pValue[j].vt == VT_EMPTY)	continue;
				memcpy(&cvr, &pValue[j], sizeof(VARIANT));
				if(bProj)
				{
					if(cvr.vt != item->getValue().vt && cvr.vt == VT_BSTR)
						continue;
					item->setValue(cvr);
				}
				else
				{
					if(cvr.vt != item->getIOValue().vt && cvr.vt == VT_BSTR)
						continue;
					item->setIOValue(cvr);
				}
			}
		}
		SafeArrayUnaccessData(pArray);
		::VariantClear(&safeArray);
	}
}

//!< ��ʾ�����Ϊid�ı�����idΪ-1��ʾ��ʾ����һ����
void CItemMgr::ShowItem(UINT id)
{
	if(!CProjectMgr::GetMe().GetProj())	return;

	// -1��ʾ��ʾ����һ����
	if (id != -1)
	{
		std::shared_ptr<CItem> item = GetItem(id);
		if(!item)		return;

		//!< ���������û�򿪣���ô�򿪱�����һ��
		BOOL isMax;
		CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
		CFrameWnd* pFrame = mf->MDIGetActive(&isMax);		//!< �ҵ���ǰ��������Ӵ���
		if(pFrame)
		{
			CDocument* pDoc = pFrame->GetActiveDocument();
			if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CItemDoc)))
			{
				if(((CItemDoc *)pDoc)->GetView()->ShowItem(id))
					return;
			}
		}
	}
	OpenDoc();
	m_pItemDoc->GetView()->ShowItem(id);
}

//!< ���ҷ��ϸ��ַ����ı���������ƥ�������
int CItemMgr::SearchItem(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex /* = false */)
{
	int nMatchCount = 0;
	std::shared_ptr<CItem> item;
	if(!bMatchCase && !bRegex)		str = str.MakeUpper();

	for (auto item : m_vtItem)
	{
		if(item)
			if(item->DoSearch(str, bMatchWhole, bMatchCase, bRegex))
				++nMatchCount;
//		CGbl::SetProgressStep(1);
	}
	return nMatchCount;
}

//!< ���һ����������֣�����Ҳ���ԣ�����֤û�б��Ժͱ���ʹ�õ�
CString CItemMgr::GetSimilarName(CString name)
{
	int number = 0;
	CString noNumberName = name;
	bool haveNumber = CGbl::GetNumberFromString(noNumberName, number, 1);
	//!< �������ַ����в���������
	if(!haveNumber)
	{
		if(!GetItem(name))			return name;
		name = name + _T("0");
		return GetSimilarName(name);
	}
	//!< �������������
	if(!GetItem(name))				return name;
	CString newName;
	newName.Format("%d", number + 1);
	//!< �������9����ʾ+1ʱ���λ���Ǻǣ���ʱ���ǰ�ߵ��ַ�����󻹸����㣬Ϊ�˹����������Ҫ��������ȥ��
	CString strNumber;
	strNumber.Format("%d",number);
	bool bAll9 = true;
	for(int i = 0; i < strNumber.GetLength(); ++i)
	{
		char c = strNumber.GetAt(i);
		if(c != '9'){
			bAll9 = false;
			break;
		}
	}
	if(bAll9 && noNumberName.GetLength() > 0 && noNumberName.Right(1) == _T("0"))
	{
		newName = noNumberName.Left(noNumberName.GetLength() - 1) + newName;
	}
	else
	{
		newName = noNumberName + newName;
	}
	return GetSimilarName(newName);
}

//!< ��Ӹոձ��޸ĵı���
void CItemMgr::AddEditItem(UINT id)
{
	m_ltEditItemID.push_back(id);
}

//!< �����������������
void CItemMgr::SetEditEnd()
{
	if(m_ltEditItemID.empty())		return;
	POSITION pos = g_App.m_pItemDocMgr->GetFirstDocPosition();
	CItemDoc* pDoc;
	CItemView* pView;
	while(pos)
	{
		pDoc = (CItemDoc *)g_App.m_pItemDocMgr->GetNextDoc(pos);
		pView = pDoc->GetView();
		pView->m_ItemGrid.FreshItemRows(m_ltEditItemID);
	}
	m_ltEditItemID.clear();
}

//!< ˢ�����б����ʾ
void CItemMgr::FreshAllGrid()
{
	POSITION pos = g_App.m_pItemDocMgr->GetFirstDocPosition();
	CItemDoc* pDoc;
	CItemView* pView;
	while(pos)
	{
		pDoc = (CItemDoc *)g_App.m_pItemDocMgr->GetNextDoc(pos);
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

//!< ��������
void CItemMgr::OnVariableAlarm(int nAlarmType,int nID,VARIANT* varValue)
{
	std::shared_ptr<CItem> item = GetItem(nID);
	if(!item)					return;
	if(!varValue)				return;
	SYSTEMTIME t;
	GetLocalTime(&t);
	CComVariant cvr = *varValue;
	cvr.ChangeType(VT_BSTR);
	CString text;
	byte* alarmByte = (byte *)&nAlarmType;
	if(alarmByte[0] == 0){
		if(alarmByte[1] == 1)		text = _T("��");
		if(alarmByte[1] == 2)		text = _T("��");
		if(alarmByte[1] == 3)		text = _T("��->��");
		if(alarmByte[1] == 4)		text = _T("��->��");
		if(alarmByte[1] == 5)		text = _T("���ظı�");
	}
	else{
		if(alarmByte[0] == 1)		text = _T("�����ޱ���");
		else if(alarmByte[0] == 2)	text = _T("���ޱ���");
		else if(alarmByte[0] == 3)	text = _T("���ޱ���");
		else if(alarmByte[0] == 4)	text = _T("�����ޱ���");
		else if(alarmByte[0] == 5)	text = _T("Ŀ�걨��");
		else if(alarmByte[0] == 6)	text = _T("�仯�ʱ���");
	}
	CString title;
	if(alarmByte[3] == 0){
		item->SetAlarm(alarmByte[0], true);
		title = _T("ϵͳ��ʾ-������������������ [");
	}
	else if(alarmByte[3] == 0xFF){
		item->SetAlarm(alarmByte[0], false);
		title = _T("ϵͳ��ʾ-����������������� [");
	}
	else{
		return;
	}
	text = title + item->getName() + _T("] ���� [") + text + _T("]");

	CString strTime;
	strTime.Format("%.4d/%.2d/%.2d %.2d:%.2d:%.2d(%.3d) ���� ",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);
	CGbl::PrintOut(strTime + text);
}