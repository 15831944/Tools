#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "ItemProperty.h"
#include "Item.h"
#include "SourceProperty.h"
#include "AlarmProperty.h"
#include "ItemMgr.h"
#include "ItemGrid.h"
#include "ItemGroup.h"
#include "ScriptEditDlg.h"
#include "SoftInfo.h"
//#include "Power.h"

#include "DeviceOne.h"
#include "DevMgr.h"

#include "XmlMgr.h"
#include "XmlDevice.h"
#include "XmlArea.h"

const UINT ID_BUTTON_SCRIPT = 1001;

const CString ITEM_TITLE = _T(" �������Ա�");
const CString ITEM_NEW = _T("�½�����");

const CString ITEM_BASE_INFO = _T("����������Ϣ");
const CString ITEM_BASE_INFO_TOOLTIP = _T("����������Ϣ");
const CString ITEM_NAME = _T("��������");
const CString ITEM_NAME_TOOLTIP = _T("��������");
const CString ITEM_TAG = _T("������ǩ");
const CString ITEM_TAG_TOOLTIP = _T("������ǩ");
const CString ITEM_DESCRIPTION = _T("������ע");
const CString ITEM_DESCRIPTION_TOOLTIP = _T("ѡ���������ʱ������Enter����ʾ���У�����Ctrl+Enter��ϼ���ʾȷ������");
const CString ITEM_GROUP = _T("����������");
const CString ITEM_GROUP_TOOLTIP = _T("����������");
const CString ITEM_VALTYPE = _T("����ֵ����");
const CString ITEM_VALTYPE_TOOLTIP = _T("����ֵ����");
const CString ITEM_SRCTYPE = _T("����Դ����");
const CString ITEM_SRCTYPE_TOOLTIP = _T("����Դ����");
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
const CString SRC_SCRIPT = _T("�ڴ������ֵ�ű�");
const CString SRC_SCRIPT_TOOLTIP = _T("�ڴ������ֵ�ű�");
const CString SRC_FRESHTIME = _T("����ˢ��ʱ��,��λ1ms");
const CString SRC_FRESHTIME_TOOLTIP = _T("����ˢ��ʱ��,��λ1ms");
const CString SRC_DEVICEID = _T("�����豸");
const CString SRC_DEVICEID_TOOLTIP = _T("�����豸");
const CString SRC_AREAID = _T("����ַ");
const CString SRC_AREAID_TOOLTIP = _T("����ַ");
const CString SRC_UNITINDEX = _T("Ѱַƫ��");
const CString SRC_UNITINDEX_TOOLTIP = _T("Ѱַƫ��");
const CString SRC_BITINDEX = _T("λƫ��");
const CString SRC_BITINDEX_TOOLTIP = _T("λƫ��");
const CString SRC_IOTYPE = _T("IO����");
const CString SRC_IOTYPE_TOOLTIP = _T("IO����");
const CString SRC_DELAYFLAG = _T("�Ƿ������ͺ�");
const CString SRC_DELAYFLAG_TOOLTIP = _T("�ͺ��ʾ��ȡ����ˢ��ֵ���������ͺ��ʾ��ȡ���Ǵ洢ֵ");
const CString SRC_CONVERT = _T("ת������");
const CString SRC_CONVERT_TOOLTIP = _T("ת������");
const CString SRC_MINPROJ = _T("��С����ֵ");
const CString SRC_MINPROJ_TOOLTIP = _T("��С����ֵ");
const CString SRC_MAXPROJ = _T("��󹤳�ֵ");
const CString SRC_MAXPROJ_TOOLTIP = _T("��󹤳�ֵ");
const CString SRC_MINIO = _T("��Сԭʼֵ");
const CString SRC_MINIO_TOOLTIP = _T("��Сԭʼֵ");
const CString SRC_MAXIO = _T("���ԭʼֵ");
const CString SRC_MAXIO_TOOLTIP = _T("���ԭʼֵ");

const CString ALARM_INFO = _T("��������");
const CString ALARM_INFO_TOOLTIP = _T("��������");
const CString ALARM_BITTYPE = _T("���ر�������");
const CString ALARM_BITTYPE_TOOLTIP = _T("���ر�������");
const CString ALARM_DEADAREA = _T("��ֵ����");
const CString ALARM_DEADAREA_TOOLTIP = _T("��ֵ����");
const CString ALARM_LOLOACTIVE = _T("�����ޱ�������");
const CString ALARM_LOLOACTIVE_TOOLTIP = _T("�����ޱ�������");
const CString ALARM_LOLOVALUE = _T("�����ޱ���ֵ");
const CString ALARM_LOLOVALUE_TOOLTIP = _T("���ޱ���ֵ");
const CString ALARM_LOWACTIVE = _T("���ޱ�������");
const CString ALARM_LOWACTIVE_TOOLTIP = _T("���ޱ�������");
const CString ALARM_LOWVALUE = _T("���ޱ���ֵ");
const CString ALARM_LOWVALUE_TOOLTIP = _T("���ޱ���ֵ");
const CString ALARM_HIGHACTIVE = _T("���ޱ�������");
const CString ALARM_HIGHACTIVE_TOOLTIP = _T("���ޱ�������");
const CString ALARM_HIGHVALUE = _T("���ޱ���ֵ");
const CString ALARM_HIGHVALUE_TOOLTIP = _T("���ޱ���ֵ");
const CString ALARM_HIHIACTIVE = _T("�����ޱ�������");
const CString ALARM_HIHIACTIVE_TOOLTIP = _T("�����ޱ�������");
const CString ALARM_HIHIVALUE = _T("�����ޱ���ֵ");
const CString ALARM_HIHIVALUE_TOOLTIP = _T("�����ޱ���ֵ");
const CString ALARM_AIMACTIVE = _T("Ŀ�걨������");
const CString ALARM_AIMACTIVE_TOOLTIP = _T("Ŀ�걨������");
const CString ALARM_AIMVALUE = _T("Ŀ�걨��ֵ");
const CString ALARM_AIMVALUE_TOOLTIP = _T("Ŀ�걨��ֵ");
const CString ALARM_AIMPERCENT = _T("ƫ��ֵ");
const CString ALARM_AIMPERCENT_TOOLTIP = _T("Ŀ�걨��ƫ��ֵ����ʾ�ﵽĿ��ֵ���¸�����Χ��ʱ����");
const CString ALARM_AIMDEAD = _T("Ŀ�걨������ֵ");
const CString ALARM_AIMDEAD_TOOLTIP = _T("Ŀ�걨������ֵ");
const CString ALARM_SHIFTACTIVE = _T("�仯�ʱ�������");
const CString ALARM_SHIFTACTIVE_TOOLTIP = _T("���仯��X���仯ʱ��T�룬ÿ��ȡֵ����һ��ȡֵ�Ĳ�ľ���ֵΪ��x��\
����ȡֵ��ʱ���Ϊ��t�룬���ҽ��� (��x/��t)>=(X/T) ʱ������");
const CString ALARM_SHIFTDELTA = _T("���仯��");
const CString ALARM_SHIFTDELTA_TOOLTIP = _T("���仯��X���仯ʱ��T�룬ÿ��ȡֵ����һ��ȡֵ�Ĳ�ľ���ֵΪ��x��\
����ȡֵ��ʱ���Ϊ��t�룬���ҽ��� (��x/��t)>=(X/T) ʱ������");
const CString ALARM_SHIFTTIME = _T("�仯ʱ�䣬��λ��");
const CString ALARM_SHIFTTIME_TOOLTIP = _T("���仯��X���仯ʱ��T�룬ÿ��ȡֵ����һ��ȡֵ�Ĳ�ľ���ֵΪ��x��\
����ȡֵ��ʱ���Ϊ��t�룬���ҽ��� (��x/��t)>=(X/T) ʱ������");

const UINT ITEM_NAME_ID = 1;
const UINT ITEM_TAG_ID = ITEM_NAME_ID + 1;
const UINT ITEM_DESCRIPTION_ID = ITEM_TAG_ID + 1;
const UINT ITEM_GROUP_ID = ITEM_DESCRIPTION_ID + 1;
const UINT ITEM_VALTYPE_ID = ITEM_GROUP_ID + 1;
const UINT ITEM_SRCTYPE_ID = ITEM_VALTYPE_ID + 1;
const UINT ITEM_ACCESSRIGHT_ID = ITEM_SRCTYPE_ID + 1;
const UINT ITEM_DEFAULTVALUE_ID = ITEM_ACCESSRIGHT_ID + 1;
const UINT ITEM_RESERVE_ID = ITEM_DEFAULTVALUE_ID + 1;
const UINT ITEM_RESFDB_ID = ITEM_RESERVE_ID + 1;
const UINT ITEM_MODBUS485_ID = ITEM_RESFDB_ID + 1;

const UINT SRC_SCRIPT_ID = 101;
const UINT SRC_FRESHTIME_ID = SRC_SCRIPT_ID + 1;
const UINT SRC_DEVICE_ID = SRC_FRESHTIME_ID + 1;
const UINT SRC_AREA_ID = SRC_DEVICE_ID + 1;
const UINT SRC_IOTYPE_ID = SRC_AREA_ID  + 1;
const UINT SRC_UNITINDEX_ID = SRC_IOTYPE_ID + 1;
const UINT SRC_BITINDEX_ID = SRC_UNITINDEX_ID + 1;
const UINT SRC_DELAYFLAG_ID = SRC_BITINDEX_ID + 1;
const UINT SRC_CONVERT_ID = SRC_DELAYFLAG_ID + 1;
const UINT SRC_MINPROJ_ID = SRC_CONVERT_ID + 1;
const UINT SRC_MAXPROJ_ID = SRC_MINPROJ_ID + 1;
const UINT SRC_MINIO_ID = SRC_MAXPROJ_ID + 1;
const UINT SRC_MAXIO_ID = SRC_MINIO_ID + 1;

const UINT ALARM_BITTYPE_ID = 201;
const UINT ALARM_DEADAREA_ID = ALARM_BITTYPE_ID + 1;
const UINT ALARM_LOLOACTIVE_ID = ALARM_DEADAREA_ID + 1;
const UINT ALARM_LOLOVALUE_ID = ALARM_LOLOACTIVE_ID + 1;
const UINT ALARM_LOWACTIVE_ID = ALARM_LOLOVALUE_ID + 1;
const UINT ALARM_LOWVALUE_ID = ALARM_LOWACTIVE_ID + 1;
const UINT ALARM_HIGHACTIVE_ID = ALARM_LOWVALUE_ID + 1;
const UINT ALARM_HIGHVALUE_ID = ALARM_HIGHACTIVE_ID + 1;
const UINT ALARM_HIHIACTIVE_ID = ALARM_HIGHVALUE_ID + 1;
const UINT ALARM_HIHIVALUE_ID = ALARM_HIHIACTIVE_ID + 1;
const UINT ALARM_AIMACTIVE_ID = ALARM_HIHIVALUE_ID + 1;
const UINT ALARM_AIMVALUE_ID = ALARM_AIMACTIVE_ID + 1;
const UINT ALARM_AIMPERCENT_ID = ALARM_AIMVALUE_ID + 1;
const UINT ALARM_AIMDEAD_ID = ALARM_AIMPERCENT_ID + 1;
const UINT ALARM_SHIFTACTIVE_ID = ALARM_AIMDEAD_ID + 1;
const UINT ALARM_SHIFTDELTA_ID = ALARM_SHIFTACTIVE_ID + 1;
const UINT ALARM_SHIFTTIME_ID = ALARM_SHIFTDELTA_ID + 1;

using namespace MVC;
using namespace Item;
MVC::Item::CPropertyItem::CPropertyItem(void)
:m_GroupID(0)		//!< ��������ӻ����޸ģ���Ҫ��¼һ�±����������ID��
,m_Item(NULL)		//!< ��ǰ��ָ��ı�����������½������Ϊ��
,m_DevIDOld(-1)		//!< ��¼�ɵ������豸�ڵ��Ӧ�豸�ı��,�����µıȽ�,���һ���Ͳ�ˢ�±������б���
{
	m_NewItem = std::shared_ptr<CItem>(new CItem(_T("�½�����")));
	m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));
}

MVC::Item::CPropertyItem::~CPropertyItem(void)
{
}

//!< ��ñ����������
CString MVC::Item::CPropertyItem::GetTitle()
{
	if(m_bAdd)			return ITEM_NEW;
	else if(!m_Item)	return _T("");
	return m_Item->getName() + ITEM_TITLE;
}

//!< ��ʾ���ĺ���
void MVC::Item::CPropertyItem::ShowInfo(CXTPPropertyGrid& grid)
{
	if(m_bAdd){
		CreateNew();
		grid.SetWindowText(_T("�½�����"));
	}
	else{
		CreateEdit();
		grid.SetWindowText(_T("�޸ı���"));
	}

	CXTPPropertyGridItem *pGroup, *pItem;			//!< ������
	CXTPPropertyGridInplaceButton* pButton;
	std::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CPMApp *)AfxGetApp())->m_XmlMgr;		//!< ������Ϣ
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();
	std::shared_ptr<CPropertyAlarm> showAlarm = m_ShowItem->getAlarmInfo();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	ASSERT(showSrc);
	ASSERT(showAlarm);
	std::list<CString> strList, boolList;			//!< �����б������
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));

	//!< ����������Ϣ
	pGroup = grid.AddCategory(ITEM_BASE_INFO);
	pGroup->SetTooltip(ITEM_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< ��������
	pItem = AddItemText(*pGroup, ITEM_NAME, ITEM_NAME_TOOLTIP, m_ShowItem->getName(), ITEM_NAME_ID);
	if(!m_bAdd && !SoftInfo::CSoftInfo::GetMe().isItemNabled())		pItem->SetReadOnly(TRUE);

	//!< ������ǩ
	AddItemText(*pGroup, ITEM_TAG, ITEM_TAG_TOOLTIP, m_ShowItem->getTag(), ITEM_TAG_ID);

	//!< ������ע
	AddItemMultiText(*pGroup, ITEM_DESCRIPTION, ITEM_DESCRIPTION_TOOLTIP, m_ShowItem->getDescription(), ITEM_DESCRIPTION_ID);

	//!< ����������
	strList.clear();
	std::shared_ptr<CItemGroup> group;
	int dftGroup = 0;
	for (auto group : itemMgr->m_vtItemGroup)
	{
		if(!group)			continue;
		if(group->getID() == m_GroupID)		dftGroup = strList.size();
		strList.push_back(group->getName());
	}
	//CXTPPropertyGridItem* pItemText = AddItemText(*pGroup, ITEM_GROUP, ITEM_GROUP_TOOLTIP, m_ShowItem->GetGroupText(), ITEM_GROUP_ID);
	AddItemList(*pGroup, ITEM_GROUP, ITEM_GROUP_TOOLTIP, strList, dftGroup, ITEM_GROUP_ID);

	//!< ����ֵ����
	strList.clear();
	strList.push_back(_T("λ(bit)"));
	strList.push_back(_T("�з����ֽ�(char)"));
	strList.push_back(_T("�޷����ֽ�(byte)"));
	strList.push_back(_T("�з�����(short)"));
	strList.push_back(_T("�޷�����(unsigned short)"));
	strList.push_back(_T("�з���˫��(int32)"));
	strList.push_back(_T("�޷���˫��(unsigned int32)"));
	strList.push_back(_T("�����ȸ���(float)"));
	AddItemList(*pGroup, ITEM_VALTYPE, ITEM_VALTYPE_TOOLTIP, strList,m_ShowItem->getValType(), ITEM_VALTYPE_ID);

	//!< ��������Դ����
	strList.clear();
	strList.push_back(_T("�ڴ����"));
	strList.push_back(_T("I/O����"));
	AddItemList(*pGroup, ITEM_SRCTYPE, ITEM_SRCTYPE_TOOLTIP, strList, m_ShowItem->getSrcType(), ITEM_SRCTYPE_ID);

	//!< �����ķ���Ȩ��
	strList.clear();
	strList.push_back(_T("�ɶ���д"));
	strList.push_back(_T("ֻ��"));
	strList.push_back(_T("ֻд"));
	AddItemList(*pGroup, ITEM_ACCESSRIGHT, ITEM_ACCESSRIGHT_TOOLTIP, strList, m_ShowItem->getAccessRight(), ITEM_ACCESSRIGHT_ID);

	//!< Ĭ��ֵ, �������Ͳ��벻ͬ��item
	AddItemVariant(*pGroup, ITEM_DEFAULTVALUE, ITEM_DEFAULTVALUE_TOOLTIP, m_ShowItem->getDefault(), ITEM_DEFAULTVALUE_ID);

	//!< �Ƿ���
	AddItemList(*pGroup, ITEM_RESERVE, ITEM_RESERVE_TOOLTIP, boolList, m_ShowItem->getReservFlag()?1:0, ITEM_RESERVE_ID);

	//!< �Ƿ񱣴���ʷ����
	AddItemList(*pGroup, ITEM_RESFDB, ITEM_RESFDB_TOOLTIP, boolList, m_ShowItem->getReservDB()?1:0, ITEM_RESFDB_ID);

	//!< �Ƿ񵼳�Modbus����
	if (SoftInfo::CSoftInfo::GetMe().IsModbus485())
	{
		AddItemList(*pGroup, ITEM_MODBUS485, ITEM_MODBUS485_TOOLTIP, boolList, m_ShowItem->getModbus485()?1:0, ITEM_MODBUS485_ID);
	}

	//!< ����Դ��Ϣ
	pGroup = grid.AddCategory(SRC_INFO);
	pGroup->SetTooltip(SRC_INFO_TOOLTIP);
	pGroup->Expand();

	//!< ��ֵ�ű�
	grid.SetVariableItemsHeight(TRUE);
	CXTPPropertyGridItem* pItemText = AddItemText(*pGroup, SRC_SCRIPT, SRC_SCRIPT_TOOLTIP, showSrc->getScriptText(), SRC_SCRIPT_ID);
	int lineCount = showSrc->getScriptText().GetLength() * 12 / pItemText->GetValueRect().Width();
	pItemText->SetMultiLinesCount(lineCount);
	pButton = pItemText->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(ID_BUTTON_SCRIPT));
	pButton->SetCaption(_T("�༭"));

	//!< �豸����
	strList.clear();
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> myDev;		//!< �������������豸
	int defIndex = showSrc->getDeviceID();
	myDev = devMgr->GetDeviceNameList(strList, defIndex);
	if(myDev)		m_DevIDOld = myDev->getID();
	else			m_DevIDOld = -1;
	AddItemList(*pGroup, SRC_DEVICEID, SRC_DEVICEID_TOOLTIP, strList, defIndex, SRC_DEVICE_ID);

	//!< ����ַ
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev;			//!< ���������豸��������Ϣ
	std::shared_ptr<XmlInfo::CXmlArea> myArea;			//!< ��������������
	if(myDev)		//!< ������������豸���ڣ���������Ҳ������
	{
		xmlDev = myDev->GetXmlInfo();
		defIndex = showSrc->getAreaID();
		if(xmlDev)
		{
			strList.clear();
			myArea = xmlDev->GetAreaNameList(strList, defIndex, true);
			if(!myArea || xmlDev->m_vtArea.empty())			strList.push_back(_T("��"));
			else if (!myArea)								myArea = xmlDev->m_vtArea.front();			//!< ���뱣֤����һ��

			CString strAreaName = SRC_AREAID;
			CString strAreaTooltip = SRC_AREAID_TOOLTIP;
			//!< Modbus�豸,�����벻������д��,�쵼�Ǳ���д��,����
			if(xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_SERIAL ||
				xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_UDP ||
				xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_TCP){
				strAreaName = _T("������ʽ");
				strAreaTooltip = strAreaName;
			}
			AddItemList(*pGroup, SRC_AREAID, SRC_AREAID_TOOLTIP, strList, defIndex, SRC_AREA_ID);
		}
	}
	else
	{
		strList.clear();
		strList.push_back(_T("��"));
		AddItemList(*pGroup, SRC_AREAID, SRC_AREAID_TOOLTIP, strList, 0, SRC_AREA_ID);
	}

	//!< IO����
	if(myArea)
	{
		strList.clear();
		defIndex = showSrc->getIOType();
		strList = myArea->GetSupportUnitList(defIndex);
		if(strList.empty())		strList.push_back(_T("��"));
		AddItemList(*pGroup, SRC_IOTYPE, SRC_IOTYPE_TOOLTIP, strList, defIndex, SRC_IOTYPE_ID);
	}
	else
	{
		strList.clear();
		strList.push_back(_T("��"));
		AddItemList(*pGroup, SRC_IOTYPE, SRC_IOTYPE_TOOLTIP, strList, 0, SRC_IOTYPE_ID);
	}

	//!< Ѱַƫ��
	CString areaName = SRC_UNITINDEX;
	if(xmlDev)
	{
		if(xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_SERIAL ||
			xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_UDP ||
			xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_TCP){	//!< Modbus�豸,�����벻������д��,�쵼�Ǳ���д��,����
			areaName = _T("Modbus��ַ");
		}
	}
	if(myArea)	areaName = areaName + _T("(��λ��") + myArea->GetUnitName() + _T(")");
	CString areaTip;
	if(myArea)	areaTip = myArea->GetOperateRange(myArea->GetOperType(showSrc->getIOType()));
	AddItemNumber(*pGroup, areaName, areaTip, showSrc->getUnitIndex(), SRC_UNITINDEX_ID);

	//!< λƫ��
	if(myArea)	areaTip.Format("��ʼλƫ�� %d����ֹλƫ�� %d",myArea->m_nUnitBitReadStart,myArea->m_nUnitBitReadEnd);
	AddItemNumber(*pGroup, SRC_BITINDEX, areaTip, showSrc->getBitIndex(), SRC_BITINDEX_ID);

	//!< ˢ��ʱ��
	AddItemNumber(*pGroup, SRC_FRESHTIME, SRC_FRESHTIME_TOOLTIP, showSrc->getFreshTime(), SRC_FRESHTIME_ID);

	//!< �Ƿ������ͺ�
	//AddItemList(*pGroup, SRC_DELAYFLAG, SRC_DELAYFLAG_TOOLTIP, boolList, showSrc->getDelayFlag()?1:0, SRC_DELAYFLAG_ID);

	//!< ת������
	strList.clear();
	strList.push_back(_T("��ת��"));
	strList.push_back(_T("����ת��"));
//	strList.push_back(_T("ƽ����ת��"));
	AddItemList(*pGroup, SRC_CONVERT, SRC_CONVERT_TOOLTIP, strList, showSrc->getConvertType(), SRC_CONVERT_ID);

	//!< ��С����ֵ
	AddItemVariant(*pGroup, SRC_MINPROJ, SRC_MINPROJ_TOOLTIP, showSrc->getProjMin(), SRC_MINPROJ_ID);

	//!< ��󹤳�ֵ
	AddItemVariant(*pGroup, SRC_MAXPROJ, SRC_MAXPROJ_TOOLTIP, showSrc->getProjMax(), SRC_MAXPROJ_ID);

	//!< ��СIOֵ
	AddItemVariant(*pGroup, SRC_MINIO, SRC_MINIO_TOOLTIP, showSrc->getIOMin(), SRC_MINIO_ID);

	//!< ���IOֵ
	AddItemVariant(*pGroup, SRC_MAXIO, SRC_MAXIO_TOOLTIP, showSrc->getIOMax(), SRC_MAXIO_ID);

	//!< ������������
	pGroup = grid.AddCategory(ALARM_INFO);
	pGroup->SetTooltip(ALARM_INFO_TOOLTIP);
	pGroup->Expand();

	//!< λ������������
	strList.clear();
	strList.push_back(_T("������"));
	strList.push_back(_T("��ʱ����"));
	strList.push_back(_T("��ʱ����"));
	strList.push_back(_T("������ʱ����"));
	strList.push_back(_T("�ص���ʱ����"));
	strList.push_back(_T("�仯�ͱ���"));
	AddItemList(*pGroup, ALARM_BITTYPE, ALARM_BITTYPE_TOOLTIP, strList, showAlarm->getBitAlarmType(), ALARM_BITTYPE_ID);

	//!< �����ޱ�������ֵ
	AddItemNumber(*pGroup, ALARM_DEADAREA, ALARM_DEADAREA_TOOLTIP, showAlarm->getDeadArea(), ALARM_DEADAREA_ID);

	//!< �����ޱ�������
	AddItemList(*pGroup, ALARM_LOLOACTIVE, ALARM_LOLOACTIVE_TOOLTIP, boolList, showAlarm->getLoloActive()?1:0, ALARM_LOLOACTIVE_ID);

	//!< �����ޱ���ֵ
	AddItemVariant(*pGroup, ALARM_LOLOVALUE, ALARM_LOLOVALUE_TOOLTIP, showAlarm->getLoloValue(), ALARM_LOLOVALUE_ID);

	//!< ���ޱ�������
	AddItemList(*pGroup, ALARM_LOWACTIVE, ALARM_LOWACTIVE_TOOLTIP, boolList, showAlarm->getLowActive()?1:0, ALARM_LOWACTIVE_ID);

	//!< ���ޱ���ֵ
	AddItemVariant(*pGroup, ALARM_LOWVALUE, ALARM_LOWVALUE_TOOLTIP, showAlarm->getLowValue(), ALARM_LOWVALUE_ID);

	//!< ���ޱ�������
	AddItemList(*pGroup, ALARM_HIGHACTIVE, ALARM_HIGHACTIVE_TOOLTIP, boolList, showAlarm->getHighActive()?1:0, ALARM_HIGHACTIVE_ID);

	//!< ���ޱ���ֵ
	AddItemVariant(*pGroup, ALARM_HIGHVALUE, ALARM_HIGHVALUE_TOOLTIP, showAlarm->getHighValue(), ALARM_HIGHVALUE_ID);

	//!< �����ޱ�������
	AddItemList(*pGroup, ALARM_HIHIACTIVE, ALARM_HIHIACTIVE_TOOLTIP, boolList, showAlarm->getHihiActive()?1:0, ALARM_HIHIACTIVE_ID);

	//!< �����ޱ���ֵ
	AddItemVariant(*pGroup, ALARM_HIHIVALUE, ALARM_HIHIVALUE_TOOLTIP, showAlarm->getHihiValue(), ALARM_HIHIVALUE_ID);

	//!< Ŀ�걨������
	AddItemList(*pGroup, ALARM_AIMACTIVE, ALARM_AIMACTIVE_TOOLTIP, boolList, showAlarm->getAimActive()?1:0, ALARM_AIMACTIVE_ID);

	//!< Ŀ��ֵ
	AddItemVariant(*pGroup, ALARM_AIMVALUE, ALARM_AIMVALUE_TOOLTIP, showAlarm->getAimValue(), ALARM_AIMVALUE_ID);

	//!< Ŀ�걨��ƫ�Ʊ���
	AddItemDouble(*pGroup, ALARM_AIMPERCENT, ALARM_AIMPERCENT_TOOLTIP, showAlarm->getAimPercent(), ALARM_AIMPERCENT_ID);

	//!< Ŀ�걨������
	AddItemDouble(*pGroup, ALARM_AIMDEAD, ALARM_AIMDEAD_TOOLTIP, showAlarm->getAimDeadPercent(), ALARM_AIMDEAD_ID);

	//!< �仯�ʱ�������
	AddItemList(*pGroup, ALARM_SHIFTACTIVE, ALARM_SHIFTACTIVE_TOOLTIP, boolList, showAlarm->getShiftActive()?1:0, ALARM_SHIFTACTIVE_ID);

	//!< �����仯����
	AddItemDouble(*pGroup, ALARM_SHIFTDELTA, ALARM_SHIFTDELTA_TOOLTIP, showAlarm->getShiftDelta(), ALARM_SHIFTDELTA_ID);

	//!< ȡֵʱ����ֵ
	AddItemNumber(*pGroup, ALARM_SHIFTTIME, ALARM_SHIFTTIME_TOOLTIP, showAlarm->getShiftTime(), ALARM_SHIFTTIME_ID);
}

void MVC::Item::CPropertyItem::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	ShowAndHide(grid);
	EnableAndDisenable(grid);
}

//!< �����½��ı�����Ϣ
void MVC::Item::CPropertyItem::CreateNew()
{
	if(!m_ShowItem)		m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));

	//!< ��������������Ҫ������һ�δ����ı�������� + 1
	CString strHead;
	if(!m_NewItem)		strHead = _T("�½�����");
	else				strHead = m_NewItem->getName();

	int id = 1;
	//if(CGbl::GetNumberFromString(strHead, id, 1))	++id;
	CGbl::GetNumberFromString(strHead, id, 1);
	CString name;
	CItemMgr* mgr = &CItemMgr::GetMe();
	do{										//!< ���������Ϊ���ҵ�һ��
		if(id == 0)		name = strHead;
		else{
			name.Format(strHead + "%d",id);
//			name = name;
		}
		if(!mgr->GetItem(name))				break;
	} while(++id);							//!< ��������Ͼ�����ѭ�������Ժ�߲��ÿ���û�ҵ������⣬һ�����ҵ�

	m_NewItem->setItemName(name);
	m_NewItem->setMyGroupID(m_GroupID);
	*m_ShowItem = *m_NewItem;
}

//!< �����༭�ı�����Ϣ
void MVC::Item::CPropertyItem::CreateEdit()
{
	if(!m_ShowItem)		m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));
	if(!m_Item)			return;
	*m_ShowItem = *m_Item;
	m_GroupID = m_ShowItem->getMyGroupID();	//!< ��¼���,�Ա��´����ʱ�ܹ��Զ���Ϊ����
}

//!< �Ի���ǰ������������Ϣ��������
void MVC::Item::CPropertyItem::OnCloseGrid()
{
	if(m_bAdd){			//!< �������ӣ����¼��m_NewItem��
		*m_NewItem = *m_ShowItem;
	}
	else{				//!< 
	}
}

//!< �������ʼ����Щ�໥������IO����
void MVC::Item::CPropertyItem::ShowAndHide(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *itemProjType, *itemSrc, *itemDev, *itemArea, *itemIOType, *itemBitlen;
	CXTPPropertyGridItem *tmp;
	itemProjType = grid.FindItem(ITEM_VALTYPE_ID);
	itemSrc = grid.FindItem(ITEM_SRCTYPE_ID);
	itemDev = grid.FindItem(SRC_DEVICE_ID);
	itemArea = grid.FindItem(SRC_AREA_ID);
	itemIOType = grid.FindItem(SRC_IOTYPE_ID);
	itemBitlen = grid.FindItem(SRC_BITINDEX_ID);
	if(!itemProjType || !itemSrc || !itemDev || !itemArea || !itemIOType || !itemBitlen)	return;

	//!< ������ʾ��Щ
	if(itemProjType->GetConstraints()->GetCurrent() == 0){		//!< λ
		tmp = grid.FindItem(ALARM_BITTYPE_ID);			if(tmp)		tmp->SetHidden(FALSE);	//!< ��ʾ����λ��
		for(int i = ALARM_DEADAREA_ID; i <= ALARM_SHIFTTIME_ID; ++i){
			tmp = grid.FindItem(i);						if(tmp)		tmp->SetHidden(TRUE);	//!< ���ر���������
		}
	}
	else{
		tmp = grid.FindItem(ALARM_BITTYPE_ID);			if(tmp)		tmp->SetHidden(TRUE);	//!< ���ر���λ��
		for(int i = ALARM_DEADAREA_ID; i <= ALARM_SHIFTTIME_ID; ++i){
			tmp = grid.FindItem(i);						if(tmp)		tmp->SetHidden(FALSE);	//!< ��ʾ����������
		}
	}

	//!< ����ԴӰ�������
	int index = itemSrc->GetConstraints()->GetCurrent();
	if(itemSrc->GetConstraints()->GetCurrent() == 0){			//!< �ڴ����
		tmp = grid.FindItem(SRC_SCRIPT_ID);				if(tmp)		tmp->SetHidden(FALSE);	//!< ��ʾ�ű��༭
		for( int i = SRC_DEVICE_ID; i <= SRC_MAXIO_ID; ++i){
			tmp = grid.FindItem(i);						if(tmp)		tmp->SetHidden(TRUE);	//!< ���ع���IO����
		}
	}
	else{														//!< IO����
		tmp = grid.FindItem(SRC_SCRIPT_ID);				if(tmp)		tmp->SetHidden(TRUE);	//!< ���ؽű��༭
		for( int i = SRC_DEVICE_ID; i <= SRC_MAXIO_ID; ++i){
			tmp = grid.FindItem(i);						if(tmp)		tmp->SetHidden(FALSE);	//!< ��ʾ����IO����
		}
		//!< ����ת���ķ�ʽ���յ��������͵�Ӱ��
		if(itemProjType->GetConstraints()->GetCurrent() == 0){
			for( int i = SRC_CONVERT_ID; i <= SRC_MAXIO_ID; ++i){
				tmp = grid.FindItem(i);					if(tmp)		tmp->SetHidden(TRUE);	//!< ���ع���IOת������
			}
		}
		//!< λƫ���Ƿ���ʾ
		MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
		std::shared_ptr<MVC::Device::CDeviceOne> projDev = devMgr->GetDevice(itemDev->GetValue());
		if(projDev)
		{
			std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();
			std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(itemArea->GetValue());
			if(xmlArea)
			{
				itemBitlen->SetHidden(TRUE);												//!< ������λƫ��
				int operType = xmlArea->GetOperType(xmlArea->GetSupportType(itemIOType->GetValue()));
				if(operType == 0)	itemBitlen->SetHidden(FALSE);
			}

			//!< Modbus�豸��Ӧ����λƫ��,�����벻������д��,�쵼�Ǳ���д��,����
			if(xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_SERIAL ||
				xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_UDP ||
				xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_TCP)	//!< Modbus�豸
			{
				itemBitlen->SetHidden(TRUE);
			}
		}
	}
}

//!< ������ͳһʹ��
void MVC::Item::CPropertyItem::EnableAndDisenable(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *item, *tmp;

	//!< ת��
	item = grid.FindItem(SRC_CONVERT_ID);
	if(item->GetConstraints()->GetCurrent() == 0){
		tmp = grid.FindItem(SRC_MINPROJ_ID);			if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(SRC_MAXPROJ_ID);			if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(SRC_MINIO_ID);				if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(SRC_MAXIO_ID);				if(tmp)		tmp->SetReadOnly(TRUE);
	}
	else{
		tmp = grid.FindItem(SRC_MINPROJ_ID);			if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(SRC_MAXPROJ_ID);			if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(SRC_MINIO_ID);				if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(SRC_MAXIO_ID);				if(tmp)		tmp->SetReadOnly(FALSE);
	}

	//!< �����ޱ���
	item = grid.FindItem(ALARM_LOLOACTIVE_ID);
	tmp = grid.FindItem(ALARM_LOLOVALUE_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< ���ޱ���
	item = grid.FindItem(ALARM_LOWACTIVE_ID);
	tmp = grid.FindItem(ALARM_LOWVALUE_ID);				if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< ���ޱ���
	item = grid.FindItem(ALARM_HIGHACTIVE_ID);
	tmp = grid.FindItem(ALARM_HIGHVALUE_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< �����ޱ���
	item = grid.FindItem(ALARM_HIHIACTIVE_ID);
	tmp = grid.FindItem(ALARM_HIHIVALUE_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< Ŀ�걨��
	item = grid.FindItem(ALARM_AIMACTIVE_ID);
	tmp = grid.FindItem(ALARM_AIMVALUE_ID);				if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());
	tmp = grid.FindItem(ALARM_AIMPERCENT_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());
	tmp = grid.FindItem(ALARM_AIMDEAD_ID);				if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< �仯�ʱ���
	item = grid.FindItem(ALARM_SHIFTACTIVE_ID);
	tmp = grid.FindItem(ALARM_SHIFTDELTA_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());
	tmp = grid.FindItem(ALARM_SHIFTTIME_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());
}

//!< ���ı������豸ʱ
void MVC::Item::CPropertyItem::OnChangeDev(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *itemDev, *itemArea;
	itemDev = grid.FindItem(SRC_DEVICE_ID);
	itemArea = grid.FindItem(SRC_AREA_ID);
	if(!itemDev || !itemArea)							return;
	if(IfSameArea(itemDev->GetValue(), m_DevIDOld))		return;	//!< �����������ȫһ��,�Ͳ���ˢ�±�������

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> projNewDev = devMgr->GetDevice(itemDev->GetValue());
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev;
	if(!projNewDev)										return;
	m_DevIDOld = projNewDev->getID();
	xmlDev = projNewDev->GetXmlInfo();							//!< ������豸����

	itemArea->GetConstraints()->RemoveAll();
	if(!projNewDev || !xmlDev){									//!< ���û���豸����������豸û���ҵ�������Ϣ
		itemArea->SetValue(_T(""));
	}
	else{														//!< �����������豸������������Ϣ
		std::shared_ptr<XmlInfo::CXmlArea> xmlArea;
		std::list<CString> strList;
		for (auto xmlArea : xmlDev->m_vtArea){
			if(!xmlArea)								continue;
			strList.push_back(xmlArea->getName());
		}
		strList.sort();		//!< ���������Ÿ���
		if(strList.empty())
			strList.push_back(_T("��"));
		for (CString str : strList)
			itemArea->GetConstraints()->AddConstraint(str);
		itemArea->SetValue(_T(""));
		itemArea->SetDefaultValue(_T(""));

		if(xmlDev->getDevType() == 7 || xmlDev->getDevType() == 8 || xmlDev->getDevType() == 9)
		{
//			CString areaName = _T("Modbus��ַ(��λ��") + xmlArea->GetUnitName() + _T(")");
			itemArea->SetCaption(_T("������ʽ"));
		}
	}
	OnChangeArea(grid);
}

//!< ���ı�������ʱ
void MVC::Item::CPropertyItem::OnChangeArea(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *itemDev, *itemArea, *itemIOType, *itemUnit, *itemBit;
	itemDev = grid.FindItem(SRC_DEVICE_ID);
	itemArea = grid.FindItem(SRC_AREA_ID);
	itemIOType = grid.FindItem(SRC_IOTYPE_ID);
	itemUnit = grid.FindItem(SRC_UNITINDEX_ID);
	itemBit = grid.FindItem(SRC_BITINDEX_ID);
	if(!itemDev || !itemArea || !itemIOType)		return;

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> projDev = devMgr->GetDevice(itemDev->GetValue());
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev;
	if(projDev)		xmlDev = projDev->GetXmlInfo();
	std::shared_ptr<XmlInfo::CXmlArea> xmlArea;
	if(xmlDev)		xmlArea = xmlDev->getArea(itemArea->GetValue());
	itemIOType->GetConstraints()->RemoveAll();
	if(!projDev || !xmlDev || !xmlArea){				//!< ���û���豸����������豸û���ҵ�������Ϣ
		itemIOType->SetValue(_T(""));
	}
	else{												//!< �����������豸������������Ϣ
		int def = 0;
		std::list<CString> strList = xmlArea->GetSupportUnitList(def);
		for (CString text : strList){
			itemIOType->GetConstraints()->AddConstraint(text);
		}
		itemIOType->SetValue(_T(""));
		itemIOType->SetDefaultValue(_T(""));

		CString name = SRC_UNITINDEX;
		if(xmlDev->getDevType() == 7 || xmlDev->getDevType() == 8 || xmlDev->getDevType() == 9)
		{
			name = _T("Modbus��ַ");
		}
		name = name + _T("(��λ��") + xmlArea->GetUnitName() + _T(")");
		if(itemUnit)	itemUnit->SetCaption(name);
//		xmlArea->GetSupportUnitList()
		name = _T("��ѡ��IO����");
		itemUnit->SetDescription(name);
		itemUnit->SetTooltip(name);

		name.Format("��ʼλƫ�� %d����ֹλƫ�� %d",xmlArea->m_nUnitBitReadStart,xmlArea->m_nUnitBitReadEnd);
		if(itemBit){
			itemBit->SetTooltip(name);
			itemBit->SetDescription(name);
		}
	}
	OnChangeIOType(grid);
}

//!< ���ı�������������ʱ
void MVC::Item::CPropertyItem::OnChangeIOType(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *itemDev, *itemArea, *itemOperate, *itemUnit, *itemBitlen;
	itemDev = grid.FindItem(SRC_DEVICE_ID);
	itemArea = grid.FindItem(SRC_AREA_ID);
	itemOperate = grid.FindItem(SRC_IOTYPE_ID);
	itemUnit = grid.FindItem(SRC_UNITINDEX_ID);
	itemBitlen = grid.FindItem(SRC_BITINDEX_ID);
	if(!itemDev || !itemArea || !itemOperate || !itemBitlen)		return;
	itemBitlen->SetHidden(TRUE);			//!< �Ȱ�����������

	if(itemOperate->GetValue() == _T(""))							return;

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> projDev = devMgr->GetDevice(itemDev->GetValue());
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev;
	if(projDev)		xmlDev = projDev->GetXmlInfo();
	std::shared_ptr<XmlInfo::CXmlArea> xmlArea;
	if(xmlDev)		xmlArea = xmlDev->getArea(itemArea->GetValue());
	if(xmlArea){
		int supportUnitCount = xmlArea->GetSupportType(itemOperate->GetValue());
		if(0 == supportUnitCount)	itemBitlen->SetHidden(FALSE);		//!< 0��λ������������ʾλƫ��

		CString areaTip;
		areaTip = xmlArea->GetOperateRange(xmlArea->GetOperType(supportUnitCount));
		itemUnit->SetTooltip(areaTip);
		itemUnit->SetDescription(areaTip);
	}
}

//!< �������ר�Ÿ�����Ӧ�û��Ա������޸�
void MVC::Item::CPropertyItem::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)			return;
	CString itemValue = item->GetValue();
	if(id == ITEM_NAME){						// = _T("��������");
		//!< �������жϲ����������������
	}
	else if(id == SRC_DEVICE_ID){				// = _T("�����豸");
		OnChangeDev(grid);
	}
	else if(id == SRC_AREA_ID){					// = _T("��");
		OnChangeArea(grid);
	}
	else if(id == SRC_IOTYPE_ID){
		OnChangeIOType(grid);
	}
	else if(id == SRC_FRESHTIME_ID){
		UINT uiVal = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(uiVal < 10)	 ((CXTPPropertyGridItemNumber *)item)->SetNumber(10);
	}

	//!< ��������Ϊ����
	if(id == ALARM_DEADAREA_ID){
		int iVal = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(iVal < 0){
			iVal = abs(iVal);
			((CXTPPropertyGridItemNumber *)item)->SetNumber(iVal);
		}
	}
	else if(id == ALARM_AIMDEAD_ID || id == ALARM_AIMPERCENT_ID){
		double fVal = ((CXTPPropertyGridItemDouble*)item)->GetDouble();
		if(fVal < 0){
			fVal = abs(fVal);
			((CXTPPropertyGridItemDouble*)item)->SetDouble(fVal);
		}
	}
	ShowAndHide(grid);
	EnableAndDisenable(grid);
}

bool MVC::Item::CPropertyItem::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	CString itemValue;
	UINT itemID;
	CComVariant cvr;
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();
	std::shared_ptr<CPropertyAlarm> showAlarm = m_ShowItem->getAlarmInfo();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	ASSERT(showSrc);
	ASSERT(showAlarm);
	for(int i = 0; i < num; ++i){
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< ֻ�б��޸ĵĲŻ���б��棬�Ż���һ��
		itemID = item->GetID();
		itemValue = item->GetValue();

		if(itemID == ITEM_NAME_ID){							// = _T("��������");
			if(itemMgr->GetItem(item->GetValue())){
				//AfxMessageBox(_T("����Ϊ \"") + item->GetValue() + _T("\" �ı����Ѵ��ڣ�����������..."), MB_OK | MB_ICONEXCLAMATION);
				MessageBox(grid.GetParent()->GetSafeHwnd(), _T("����Ϊ \"") + item->GetValue() + _T("\" �ı����Ѵ��ڣ�����������..."),
					_T("��ʾ"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			if(itemValue.Trim() == _T("")){
				MessageBox(grid.GetParent()->GetSafeHwnd(), _T("���Ʋ���Ϊ��..."), _T("��ʾ"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			if(!IfRightItemName(itemValue.Trim())){
				MessageBox(grid.GetParent()->GetSafeHwnd(), _T("���������ڷǷ��ַ�..."), _T("��ʾ"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			m_ShowItem->setItemName(itemValue.Trim());
		}
		else if(itemID == ITEM_TAG_ID){
			m_ShowItem->setTag(itemValue.Trim());			// = _T("������ǩ");
		}
		else if(itemID == ITEM_DESCRIPTION_ID){
			m_ShowItem->setDescription(itemValue.Trim());	// = _T("������ע");
		}
		else if(itemID == ITEM_GROUP_ID){					// = _T("������");
			std::shared_ptr<CItemGroup> itemGroup = itemMgr->GetGroup(itemValue.Trim());
			if(!itemGroup)			return false;
			m_ShowItem->setMyGroupID(itemGroup->getID());
			m_GroupID = itemGroup->getID();
		}
		else if(itemID == ITEM_VALTYPE_ID){					// = _T("����ֵ����");
			m_ShowItem->setValType(item->GetConstraints()->GetCurrent());
			m_ShowItem->ChangeVarType(m_ShowItem->getValType());
		}
		else if(itemID == ITEM_SRCTYPE_ID){					// = _T("����Դ����");
			m_ShowItem->setSrcType(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ITEM_ACCESSRIGHT_ID){				// = _T("����Ȩ��");
			m_ShowItem->setAccessRight(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ITEM_DEFAULTVALUE_ID){			// = _T("Ĭ��ֵ");
			CComVariant cvrProj = item->GetValue();
			cvrProj.ChangeType(m_ShowItem->getDefault().vt);
			m_ShowItem->setDefault(cvrProj);
		}
		else if(itemID == ITEM_RESERVE_ID){					// = _T("�Ƿ���ֵ");
			m_ShowItem->setReservFlag(item->GetConstraints()->GetCurrent());
			CProjectMgr::GetMe().GetProj()->SetModify(true);	// �������ҪӰ��һ�¹����ļ���
		}
		else if(itemID == ITEM_RESFDB_ID){					// = _T("�Ƿ񱣴���ʷ����");
			m_ShowItem->setReservDB(item->GetConstraints()->GetCurrent());
			CProjectMgr::GetMe().GetProj()->SetModify(true);	// �������ҪӰ��һ�¹����ļ���
		}
		else if(itemID == ITEM_MODBUS485_ID){				// = _T("�Ƿ񵼳�Modbus����");
			m_ShowItem->setModbus485(item->GetConstraints()->GetCurrent());
			CProjectMgr::GetMe().GetProj()->SetModify(true);
		}
		else if(itemID == SRC_SCRIPT_ID){					// = _T("�ڴ������ֵ�ű�");
			showSrc->setScriptText(itemValue);
		}
		else if(itemID == SRC_FRESHTIME_ID){				// = _T("����ˢ��ʱ��");
			showSrc->setFreshTime(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == SRC_DEVICE_ID){					// = _T("�����豸");
			MVC::Device::CDevMgr* mgr = &MVC::Device::CDevMgr::GetMe();
			std::shared_ptr<MVC::Device::CDeviceOne> device;
			device = mgr->GetDevice(itemValue);
			if(device)
				showSrc->setDevID(device->getID());
		}
		else if(itemID == SRC_AREA_ID){					// = _T("����ַ");
			MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();			//!< ��ù��̹���
			std::shared_ptr<MVC::Device::CDeviceOne> projDev;
			projDev = devMgr->GetDevice(showSrc->getDeviceID());	//!< ��ö�Ӧ�Ĺ����豸
			if(!projDev)		{AfxMessageBox(_T("δ�ҵ���Ӧ�豸..."), MB_OK | MB_ICONEXCLAMATION);	return false;}
			std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();	//!< ͨ�������豸���ͣ���������豸
			if(!xmlDev)			{AfxMessageBox(_T("δ�ҵ���Ӧ�豸����..."), MB_OK | MB_ICONEXCLAMATION);return false;}
			showSrc->setAreaID(xmlDev->GetAreaID(itemValue));
		}
		else if(itemID == SRC_IOTYPE_ID){					// = _T("������");
			MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();			//!< ��ù��̹���
			std::shared_ptr<MVC::Device::CDeviceOne> projDev;
			projDev = devMgr->GetDevice(showSrc->getDeviceID());	//!< ��ö�Ӧ�Ĺ����豸
			if(!projDev)		{AfxMessageBox(_T("δ�ҵ���Ӧ�豸..."), MB_OK | MB_ICONEXCLAMATION);	return false;}
			std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();	//!< ͨ�������豸���ͣ���������豸
			if(!xmlDev)			{AfxMessageBox(_T("δ�ҵ���Ӧ�豸����..."), MB_OK | MB_ICONEXCLAMATION);	return false;}
			CString areaName = grid.FindItem(SRC_AREA_ID)->GetValue();
			std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(areaName);
			if(!xmlArea)		{AfxMessageBox(_T("δ�ҵ���Ӧ������..."), MB_OK | MB_ICONEXCLAMATION);	return false;}
			UINT valResult = xmlArea->GetSupportType(itemValue);
			showSrc->setIOType(valResult);
		}
		else if(itemID == SRC_UNITINDEX_ID){				// = _T("Ѱַƫ��");
			showSrc->setUnitIndex(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == SRC_BITINDEX_ID){					// = _T("λƫ��");
			showSrc->setBitIndex(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == SRC_DELAYFLAG_ID){				// = _T("�Ƿ������ͺ�");
			showSrc->setDealyFlag(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == SRC_CONVERT_ID){					// = _T("ת������");
			showSrc->setConvertType(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == SRC_MINPROJ_ID){					// = _T("��С����ֵ");
			CComVariant cvrMinProj = item->GetValue();
			cvrMinProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMin(cvrMinProj);
		}
		else if(itemID == SRC_MAXPROJ_ID){					// = _T("��󹤳�ֵ");
			CComVariant cvrMaxProj = item->GetValue();
			cvrMaxProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMax(cvrMaxProj);
		}
		else if(itemID == SRC_MINIO_ID){					// = _T("��СIOֵ");
			CComVariant cvrMinIO = item->GetValue();
			cvrMinIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMin(cvrMinIO);
		}
		else if(itemID == SRC_MAXIO_ID){					// = _T("���IOֵ");
			CComVariant cvrMaxIO = item->GetValue();
			cvrMaxIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMax(cvrMaxIO);
		}
		else if(itemID == ALARM_BITTYPE_ID){				// = _T("���ر�������");
			showAlarm->setBitAlarmType(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_DEADAREA_ID){				// = _T("��ֵ����");
			showAlarm->setDeadArea(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == ALARM_LOLOACTIVE_ID){				// = _T("�����ޱ�������");
			showAlarm->setLoloActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_LOLOVALUE_ID){				// = _T("���ޱ���ֵ");
			showAlarm->setLoloValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_LOWACTIVE_ID){				// = _T("���ޱ�������");
			showAlarm->setLowActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_LOWVALUE_ID){				// = _T("���ޱ���ֵ");
			showAlarm->setLowValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_HIGHACTIVE_ID){				// = _T("���ޱ�������");
			showAlarm->setHighActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_HIGHVALUE_ID){				// = _T("���ޱ���ֵ");
			showAlarm->setHighValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_HIHIACTIVE_ID){				// = _T("�����ޱ�������");
			showAlarm->setHihiActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_HIHIVALUE_ID){				// = _T("�����ޱ���ֵ");
			showAlarm->setHihiValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_AIMACTIVE_ID){				// = _T("Ŀ�걨������");
			showAlarm->setAimActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_AIMVALUE_ID){				// = _T("Ŀ�걨��ֵ");
			showAlarm->setAimValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_AIMPERCENT_ID){				// = _T("Ŀ�걨��ƫ��ֵ");
			showAlarm->setAimPercent((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_AIMDEAD_ID){				// = _T("Ŀ�걨������ֵ");
			showAlarm->setAimDeadPercent((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_SHIFTACTIVE_ID){			// = _T("�仯�ʱ�������");
			showAlarm->setShiftActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_SHIFTDELTA_ID){			// = _T("�仯�ʱ����仯����");
			showAlarm->setShiftDelta((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_SHIFTTIME_ID){				// = _T("�仯�ʱ���ȡֵʱ�䳤��");
			showAlarm->setShiftTime(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		item->SetDefaultValue(item->GetValue());
	}
	if(m_bAdd){
		m_bAdd = false;
		grid.SetWindowText(_T("�޸ı���"));
		if(!m_NewItem)		m_NewItem = std::shared_ptr<CItem>(new CItem(_T("")));
		*m_NewItem = *m_ShowItem;							//!< m_NewItemÿ�β�һ��
		std::shared_ptr<CItem> newItem = std::shared_ptr<CItem>(new CItem(_T("")));
		*newItem = *m_ShowItem;
		CItemMgr::GetMe().AddItem(newItem,
			MAX_ITEM_COUNT,
			newItem->getMyGroupID());		//!< 
		m_NewItem->setID(newItem->getID());
	}
	else{
		if(m_Item)
			*m_Item = * m_ShowItem;
	}
	return true;
}

//!< ��ť������
void MVC::Item::CPropertyItem::OnButtonClick(CXTPPropertyGrid &grid, UINT btID)
{
	if(ID_BUTTON_SCRIPT == btID)
	{
		Dialog::CScriptEditDlg dlg(m_ShowItem->getSrcInfo()->getScriptText(), m_ShowItem->getName(), m_ShowItem->getDescription());
		int lr = dlg.DoModal();
		if(IDOK == lr)
		{
			m_ShowItem->getSrcInfo()->setScriptText(dlg.m_strScriptText);
			CXTPPropertyGridItem* item = grid.FindItem(SRC_SCRIPT_ID);
			if(item)		item->SetValue(dlg.m_strScriptText);
		}
	}
}

//!< �������Ƿ�Ϸ�
bool MVC::Item::CPropertyItem::IfRightItemName(CString strName)
{
	if(strName.Find('\\') != -1)	return false;
	if(strName.Find('\"') != -1)	return false;
	if(strName.Find('[') != -1)		return false;
	if(strName.Find(']') != -1)		return false;
	return true;
}

//!< ��ʾ����
void MVC::Item::CPropertyItem::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("��������˵��"));
}

//!< �ж��������豸�Ƿ������ȫ��ͬ����
bool MVC::Item::CPropertyItem::IfSameArea(CString name1, UINT id2)
{
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> devLhs = devMgr->GetDevice(name1);
	std::shared_ptr<MVC::Device::CDeviceOne> devRhs = devMgr->GetDevice(id2);

	if(!devLhs && !devRhs)								return true;		// ��Ϊ��
	if(devLhs && !devRhs)								return false;		// ֻ��һ��Ϊ��
	if(!devLhs && devRhs)								return false;		// ֻ��һ��Ϊ��
	if(devLhs->getDevType() == devRhs->getDevType())	return true;		// ������,������ͬ

	// ������,���ͻ���ͬ,�Ǿ͵�һ����һ�����Ƚ���
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDevLhs, xmlDevRhs;
	xmlDevLhs = devLhs->GetXmlInfo();
	xmlDevRhs = devRhs->GetXmlInfo();
	if(!xmlDevLhs || !xmlDevRhs)						return false;		// �Ҳ����豸����
	int sizeLth = (int)xmlDevLhs->m_vtArea.size();
	int sizeRth = (int)xmlDevRhs->m_vtArea.size();
	if(sizeLth != sizeRth)								return false;		// ������������ͬ
	std::shared_ptr<XmlInfo::CXmlArea> xmlAreaLhs, xmlAreaRhs;
	std::vector<std::shared_ptr<XmlInfo::CXmlArea> >::iterator iterLhs = xmlDevLhs->m_vtArea.begin();
	std::vector<std::shared_ptr<XmlInfo::CXmlArea> >::iterator iterRhs = xmlDevRhs->m_vtArea.begin();
	for(; iterLhs != xmlDevLhs->m_vtArea.end(); iterLhs++, iterRhs++)
	{
		xmlAreaLhs = (*iterLhs);
		xmlAreaRhs = (*iterRhs);
		if(!xmlAreaLhs && xmlAreaRhs)					return false;		// ��һ��������Ϊ��
		if(xmlAreaLhs && !xmlAreaRhs)					return false;		// ��һ��������Ϊ��
		if(!xmlAreaLhs && !xmlAreaRhs)					continue;			// ��Ϊ��,�����ж���һ��
		if(xmlAreaLhs->IsSame(xmlAreaRhs.get()))		continue;			// ��ͬ,�����ж���һ��
	}
	return true;															// ���Ӧ������ͬ��
}
