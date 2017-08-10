#include "StdAfx.h"
#include "DXPEditor.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "PropertyItems.h"
#include "ProjectMgr.h"
#include "Project.h"

#include "Item.h"
#include "SourceProperty.h"
#include "AlarmProperty.h"
#include "ItemMgr.h"
#include "ItemGroup.h"
#include "ScriptEditDlg.h"

#include "DevMgr.h"
#include "DeviceOne.h"

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
const CString SRC_DEVICEID = _T("�����豸");
const CString SRC_DEVICEID_TOOLTIP = _T("�����豸������IO������Ч");
const CString SRC_DELAYFLAG = _T("�Ƿ������ͺ�");
const CString SRC_DELAYFLAG_TOOLTIP = _T("�ͺ��ʾ��ȡ����ˢ��ֵ���������ͺ��ʾ��ȡ���Ǵ洢ֵ������IO������Ч");
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

const CString ALARM_INFO = _T("��������");
const CString ALARM_INFO_TOOLTIP = _T("��������");
const CString ALARM_BITTYPE = _T("���ر�������");
const CString ALARM_BITTYPE_TOOLTIP = _T("���ر������ͣ����Թ���ֵ�����ǡ�λ����������Ч");
const CString ALARM_DEADAREA = _T("��ֵ����");
const CString ALARM_DEADAREA_TOOLTIP = _T("��ֵ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_LOLOACTIVE = _T("�����ޱ�������");
const CString ALARM_LOLOACTIVE_TOOLTIP = _T("�����ޱ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_LOLOVALUE = _T("���ޱ���ֵ");
const CString ALARM_LOLOVALUE_TOOLTIP = _T("���ޱ���ֵ�����Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_LOWACTIVE = _T("���ޱ�������");
const CString ALARM_LOWACTIVE_TOOLTIP = _T("���ޱ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_LOWVALUE = _T("���ޱ���ֵ");
const CString ALARM_LOWVALUE_TOOLTIP = _T("���ޱ���ֵ�����Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_HIGHACTIVE = _T("���ޱ�������");
const CString ALARM_HIGHACTIVE_TOOLTIP = _T("���ޱ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_HIGHVALUE = _T("���ޱ���ֵ");
const CString ALARM_HIGHVALUE_TOOLTIP = _T("���ޱ���ֵ�����Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_HIHIACTIVE = _T("�����ޱ�������");
const CString ALARM_HIHIACTIVE_TOOLTIP = _T("�����ޱ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_HIHIVALUE = _T("�����ޱ���ֵ");
const CString ALARM_HIHIVALUE_TOOLTIP = _T("�����ޱ���ֵ�����Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_AIMACTIVE = _T("Ŀ�걨������");
const CString ALARM_AIMACTIVE_TOOLTIP = _T("Ŀ�걨��������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_AIMVALUE = _T("Ŀ�걨��ֵ");
const CString ALARM_AIMVALUE_TOOLTIP = _T("Ŀ�걨��ֵ�����Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_AIMPERCENT = _T("ƫ��ֵ");
const CString ALARM_AIMPERCENT_TOOLTIP = _T("Ŀ�걨��ƫ��ֵ����ʾ�ﵽĿ��ֵ���¸�����Χ��ʱ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_AIMDEAD = _T("Ŀ�걨������ֵ");
const CString ALARM_AIMDEAD_TOOLTIP = _T("Ŀ�걨������ֵ�����Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_SHIFTACTIVE = _T("�仯�ʱ�������");
const CString ALARM_SHIFTACTIVE_TOOLTIP = _T("���仯��X���仯ʱ��T�룬ÿ��ȡֵ����һ��ȡֵ�Ĳ�ľ���ֵΪ��x��\
����ȡֵ��ʱ���Ϊ��t�룬���ҽ��� (��x/��t)>=(X/T) ʱ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_SHIFTDELTA = _T("���仯��");
const CString ALARM_SHIFTDELTA_TOOLTIP = _T("���仯��X���仯ʱ��T�룬ÿ��ȡֵ����һ��ȡֵ�Ĳ�ľ���ֵΪ��x��\
����ȡֵ��ʱ���Ϊ��t�룬���ҽ��� (��x/��t)>=(X/T) ʱ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");
const CString ALARM_SHIFTTIME = _T("�仯ʱ�䣬��λ��");
const CString ALARM_SHIFTTIME_TOOLTIP = _T("���仯��X���仯ʱ��T�룬ÿ��ȡֵ����һ��ȡֵ�Ĳ�ľ���ֵΪ��x��\
����ȡֵ��ʱ���Ϊ��t�룬���ҽ��� (��x/��t)>=(X/T) ʱ���������Թ���ֵ���Ͳ��ǡ�λ����������Ч");

const UINT ITEM_VALTYPE_ID = 1;
const UINT ITEM_SRCTYPE_ID = ITEM_VALTYPE_ID + 1;
const UINT ITEM_GROUP_ID = ITEM_SRCTYPE_ID + 1;
const UINT ITEM_ACCESSRIGHT_ID = ITEM_GROUP_ID + 1;
const UINT ITEM_DEFAULTVALUE_ID = ITEM_ACCESSRIGHT_ID + 1;
const UINT ITEM_RESERVE_ID = ITEM_DEFAULTVALUE_ID + 1;
const UINT ITEM_RESFDB_ID = ITEM_RESERVE_ID + 1;
const UINT ITEM_MODBUS485_ID = ITEM_RESFDB_ID + 1;

const UINT SRC_FRESHTIME_ID = 101;
const UINT SRC_DEVICE_ID = SRC_FRESHTIME_ID + 1;
const UINT SRC_DELAYFLAG_ID = SRC_DEVICE_ID + 1;
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
CPropertyItems::CPropertyItems(void)
{
	m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));
}

CPropertyItems::~CPropertyItems(void)
{
}

//!< ��ñ����������
CString CPropertyItems::GetTitle()
{
	return ITEM_TITLE;
}

//!< ��ʾ���ĺ���
void CPropertyItems::ShowInfo(CXTPPropertyGrid& grid)
{
	CreateEdit();

	CXTPPropertyGridItem* pGroup;					//!< ������
	std::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CDXPEditorApp *)AfxGetApp())->m_XmlMgr;		//!< ������Ϣ
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::list<CString> strList, boolList;			//!< �����б������
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));
	CComVariant cvr = _T("");
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();
	std::shared_ptr<CPropertyAlarm> showAlarm = m_ShowItem->getAlarmInfo();

	//!< ����������Ϣ
	pGroup = grid.AddCategory(ITEM_BASE_INFO);
	pGroup->SetTooltip(ITEM_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< ����ֵ����
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

	//!< ����Դ����
	strList.clear();
	strList.push_back(_T("�ڴ����"));
	strList.push_back(_T("I/O����"));
	if(m_bSrcType)
		AddItemList(*pGroup, ITEM_SRCTYPE, ITEM_SRCTYPE_TOOLTIP, strList, m_ShowItem->getSrcType(), ITEM_SRCTYPE_ID);
	else
		AddItemList(*pGroup, ITEM_SRCTYPE, ITEM_SRCTYPE_TOOLTIP, strList, -1, ITEM_SRCTYPE_ID);

	//!< ����������
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

	//!< ����Ȩ��
	strList.clear();
	strList.push_back(_T("�ɶ���д"));
	strList.push_back(_T("ֻ��"));
	strList.push_back(_T("ֻд"));
	if(m_bAccessRight)
		AddItemList(*pGroup, ITEM_ACCESSRIGHT, ITEM_ACCESSRIGHT_TOOLTIP, strList, m_ShowItem->getAccessRight(), ITEM_ACCESSRIGHT_ID);
	else
		AddItemList(*pGroup, ITEM_ACCESSRIGHT, ITEM_ACCESSRIGHT_TOOLTIP, strList, -1, ITEM_ACCESSRIGHT_ID);

	//!< Ĭ��ֵ����
	AddItemVariant(*pGroup, ITEM_DEFAULTVALUE, ITEM_DEFAULTVALUE_TOOLTIP, m_ShowItem->getDefault(), ITEM_DEFAULTVALUE_ID, !m_bDefValue);

	//!< �Ƿ��Ǳ���ֵ
	if(m_bReservFlag)
		AddItemList(*pGroup, ITEM_RESERVE, ITEM_RESERVE_TOOLTIP, boolList, m_ShowItem->getReservFlag()?1:0, ITEM_RESERVE_ID);
	else
		AddItemList(*pGroup, ITEM_RESERVE, ITEM_RESERVE_TOOLTIP, boolList, -1, ITEM_RESERVE_ID);

	//!< �Ƿ��Ǳ������ݿ�
	if(m_bReservDB)
		AddItemList(*pGroup, ITEM_RESFDB, ITEM_RESFDB_TOOLTIP, boolList, m_ShowItem->getReservDB()?1:0, ITEM_RESFDB_ID);
	else
		AddItemList(*pGroup, ITEM_RESFDB, ITEM_RESFDB_TOOLTIP, boolList, -1, ITEM_RESFDB_ID);

	//!< �Ƿ񵼳�Modbus����
	if (SoftInfo::CSoftInfo::GetMe().IsModbus485())
	{
		if (m_bModbus485)
			AddItemList(*pGroup, ITEM_MODBUS485, ITEM_MODBUS485_TOOLTIP, boolList, m_ShowItem->getModbus485()?1:0, ITEM_MODBUS485_ID);
		else
			AddItemList(*pGroup, ITEM_MODBUS485, ITEM_MODBUS485_TOOLTIP, boolList, -1, ITEM_MODBUS485_ID);
	}

	//!< ����Դ��Ϣ
	pGroup = grid.AddCategory(SRC_INFO);
	pGroup->SetTooltip(SRC_INFO_TOOLTIP);
	pGroup->Expand();

	//!< �����豸
	strList.clear();
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> myDev;		//!< �������������豸
	int defIndex = -1;
	for (std::shared_ptr<MVC::Device::CDeviceOne> device : devMgr->m_vtDevice)
	{
		if(!device)					continue;
		if(!device->IsProj())		continue;
		if(device->getID() == showSrc->getDeviceID()){
			defIndex = (int)strList.size();
			myDev = device;
		}
		strList.push_back(device->getName());
	}
	if(m_bDev)
		AddItemList(*pGroup, SRC_DEVICEID, SRC_DEVICEID_TOOLTIP, strList, defIndex, SRC_DEVICE_ID);
	else
		AddItemList(*pGroup, SRC_DEVICEID, SRC_DEVICEID_TOOLTIP, strList, -1, SRC_DEVICE_ID);

	//!< ˢ��ʱ��
	AddItemNumber(*pGroup, SRC_FRESHTIME, SRC_FRESHTIME_TOOLTIP, showSrc->getFreshTime(), SRC_FRESHTIME_ID, !m_bFreshTime);

	//!< �Ƿ������ͺ�
	//if(m_bDelayFlag)
	//	AddItemList(*pGroup, SRC_DELAYFLAG, SRC_DELAYFLAG_TOOLTIP, boolList, showSrc->getDelayFlag()?1:0, SRC_DELAYFLAG_ID);
	//else
	//	AddItemList(*pGroup, SRC_DELAYFLAG, SRC_DELAYFLAG_TOOLTIP, boolList, -1, SRC_DELAYFLAG_ID);

	//!< ת������
	strList.clear();
	strList.push_back(_T("��ת��"));
	strList.push_back(_T("����ת��"));
//	strList.push_back(_T("ƽ����ת��"));
	if(m_bConvertFlag)
		AddItemList(*pGroup, SRC_CONVERT, SRC_CONVERT_TOOLTIP, strList, showSrc->getConvertType(), SRC_CONVERT_ID);
	else
		AddItemList(*pGroup, SRC_CONVERT, SRC_CONVERT_TOOLTIP, strList, -1, SRC_CONVERT_ID);

	//!< ��С����ֵ
	AddItemVariant(*pGroup, SRC_MINPROJ, SRC_MINPROJ_TOOLTIP, showSrc->getProjMin(), SRC_MINPROJ_ID, !m_bMinProj);

	//!< ��󹤳�ֵ
	AddItemVariant(*pGroup, SRC_MAXPROJ, SRC_MAXPROJ_TOOLTIP, showSrc->getProjMax(), SRC_MAXPROJ_ID, !m_bMaxProj);

	//!< ��Сԭʼֵ
	AddItemVariant(*pGroup, SRC_MINIO, SRC_MINIO_TOOLTIP, showSrc->getIOMin(), SRC_MINIO_ID, !m_bMinIO);

	//!< ���ԭʼֵ
	AddItemVariant(*pGroup, SRC_MAXIO, SRC_MAXIO_TOOLTIP, showSrc->getIOMax(), SRC_MAXIO_ID, !m_bMaxIO);

	//!< ������������
	pGroup = grid.AddCategory(ALARM_INFO);
	pGroup->SetTooltip(ALARM_INFO_TOOLTIP);
	pGroup->Expand();

	//!< ���ر�������
	strList.clear();
	strList.push_back(_T("������"));
	strList.push_back(_T("��ʱ����"));
	strList.push_back(_T("��ʱ����"));
	strList.push_back(_T("������ʱ����"));
	strList.push_back(_T("�ص���ʱ����"));
	strList.push_back(_T("�仯�ͱ���"));
	if(m_bBitAlarmType)
		AddItemList(*pGroup, ALARM_BITTYPE, ALARM_BITTYPE_TOOLTIP, strList, showAlarm->getBitAlarmType(), ALARM_BITTYPE_ID);
	else
		AddItemList(*pGroup, ALARM_BITTYPE, ALARM_BITTYPE_TOOLTIP, strList, -1, ALARM_BITTYPE_ID);

	//!< ��ֵ����
	AddItemNumber(*pGroup, ALARM_DEADAREA, ALARM_DEADAREA_TOOLTIP, showAlarm->getDeadArea(), ALARM_DEADAREA_ID, !m_bDeadArea);

	//!< �����ޱ�������
	if(m_bLoloActive)
		AddItemList(*pGroup, ALARM_LOLOACTIVE, ALARM_LOLOACTIVE_TOOLTIP, boolList, showAlarm->getLoloActive()?1:0, ALARM_LOLOACTIVE_ID);
	else
		AddItemList(*pGroup, ALARM_LOLOACTIVE, ALARM_LOLOACTIVE_TOOLTIP, boolList, -1, ALARM_LOLOACTIVE_ID);

	//!< �����ޱ���ֵ
	AddItemVariant(*pGroup, ALARM_LOLOVALUE, ALARM_LOLOVALUE_TOOLTIP, showAlarm->getLoloValue(), ALARM_LOLOVALUE_ID, !m_bLoloValue);

	//!< ���ޱ�������
	if(m_bLowActive)
		AddItemList(*pGroup, ALARM_LOWACTIVE, ALARM_LOWACTIVE_TOOLTIP, boolList, showAlarm->getLowActive()?1:0, ALARM_LOWACTIVE_ID);
	else
		AddItemList(*pGroup, ALARM_LOWACTIVE, ALARM_LOWACTIVE_TOOLTIP, boolList, -1, ALARM_LOWACTIVE_ID);

	//!< ���ޱ���ֵ
	AddItemVariant(*pGroup, ALARM_LOWVALUE, ALARM_LOWVALUE_TOOLTIP, showAlarm->getLowValue(), ALARM_LOWVALUE_ID, !m_bLowValue);

	//!< ���ޱ�������
	if(m_bHihiActive)
		AddItemList(*pGroup, ALARM_HIGHACTIVE, ALARM_HIGHACTIVE_TOOLTIP, boolList, showAlarm->getHighActive()?1:0, ALARM_HIGHACTIVE_ID);
	else
		AddItemList(*pGroup, ALARM_HIGHACTIVE, ALARM_HIGHACTIVE_TOOLTIP, boolList, -1, ALARM_HIGHACTIVE_ID);

	//!< ���ޱ���ֵ
	AddItemVariant(*pGroup, ALARM_HIGHVALUE, ALARM_HIGHVALUE_TOOLTIP, showAlarm->getHighValue(), ALARM_HIGHVALUE_ID, !m_bHighValue);

	//!< �����ޱ�������
	if(m_bHihiActive)
		AddItemList(*pGroup, ALARM_HIHIACTIVE, ALARM_HIHIACTIVE_TOOLTIP, boolList, showAlarm->getHihiActive()?1:0, ALARM_HIHIACTIVE_ID);
	else
		AddItemList(*pGroup, ALARM_HIHIACTIVE, ALARM_HIHIACTIVE_TOOLTIP, boolList, -1, ALARM_HIHIACTIVE_ID);

	//!< �����ޱ���ֵ
	AddItemVariant(*pGroup, ALARM_HIHIVALUE, ALARM_HIHIVALUE_TOOLTIP, showAlarm->getHihiValue(), ALARM_HIHIVALUE_ID, !m_bHihiValue);

	//!< Ŀ�걨������
	if(m_bAimActive)
		AddItemList(*pGroup, ALARM_AIMACTIVE, ALARM_AIMACTIVE_TOOLTIP, boolList, showAlarm->getAimActive()?1:0, ALARM_AIMACTIVE_ID);
	else
		AddItemList(*pGroup, ALARM_AIMACTIVE, ALARM_AIMACTIVE_TOOLTIP, boolList, -1, ALARM_AIMACTIVE_ID);

	//!< Ŀ�걨��ֵ
	AddItemVariant(*pGroup, ALARM_AIMVALUE, ALARM_AIMVALUE_TOOLTIP, showAlarm->getAimValue(), ALARM_AIMVALUE_ID, !m_bAimPercent);

	//!< Ŀ�걨���ٷֱ�
	AddItemDouble(*pGroup, ALARM_AIMPERCENT, ALARM_AIMPERCENT_TOOLTIP, showAlarm->getAimPercent(), ALARM_AIMPERCENT_ID, !m_bAimDeadPercent);

	//!< Ŀ�걨���ٷֱ�����
	AddItemDouble(*pGroup, ALARM_AIMDEAD, ALARM_AIMDEAD_TOOLTIP, showAlarm->getAimDeadPercent(), ALARM_AIMDEAD_ID, !m_bAimDeadPercent);

	//!< �仯�ʱ�������
	if(m_bShiftActive)
		AddItemList(*pGroup, ALARM_SHIFTACTIVE, ALARM_SHIFTACTIVE_TOOLTIP, boolList, showAlarm->getShiftActive()?1:0, ALARM_SHIFTACTIVE_ID);
	else
		AddItemList(*pGroup, ALARM_SHIFTACTIVE, ALARM_SHIFTACTIVE_TOOLTIP, boolList, -1, ALARM_SHIFTACTIVE_ID);

	//!< �仯�ʱ����ٷֱ�
	AddItemDouble(*pGroup, ALARM_SHIFTDELTA, ALARM_SHIFTDELTA_TOOLTIP, showAlarm->getShiftDelta(), ALARM_SHIFTDELTA_ID, !m_bShiftPercent);

	//!< �仯��ȡֵʱ��
	AddItemNumber(*pGroup, ALARM_SHIFTTIME, ALARM_SHIFTTIME_TOOLTIP, showAlarm->getShiftTime(), ALARM_SHIFTTIME_ID, !m_bShiftTime);
}

//!< �༭ǰ������׼������
void CPropertyItems::CreateEdit()
{
	//!< ����Ĭ�ϣ�����ͬ
	m_bValType = true;			//!< ����ֵ�����Ƿ���ͬ
	m_bSrcType = true;			//!< ����Դ�����Ƿ���ͬ
	m_bGroup = true;			//!< �����������Ƿ���ͬ
	m_bDefValue = true;			//!< Ĭ��ֵ�����Ƿ���ͬ
	m_bAccessRight = true;		//!< ����Ȩ���Ƿ���ͬ
	m_bReservFlag = true;		//!< �Ƿ��Ǳ���ֵ�Ƿ���ͬ
	m_bReservDB = true;			//!< �Ƿ��Ǳ������ݿ��Ƿ���ͬ
	m_bModbus485 = true;		//!< �Ƿ񵼳�Modbus�����Ƿ���ͬ
	m_bDev = true;				//!< �����豸�Ƿ���ͬ
	m_bFreshTime = true;		//!< ˢ��ʱ���Ƿ���ͬ
	m_bDelayFlag = true;		//!< �Ƿ������ͺ��Ƿ���ͬ
	m_bConvertFlag = true;		//!< ת�������Ƿ���ͬ
	m_bMinProj = true;			//!< ��С����ֵ�Ƿ���ͬ
	m_bMaxProj = true;			//!< ��󹤳�ֵ�Ƿ���ͬ
	m_bMinIO = true;			//!< ��Сԭʼֵ�Ƿ���ͬ
	m_bMaxIO = true;			//!< ���ԭʼֵ�Ƿ���ͬ
	m_bBitAlarmType = true;		//!< ���ر��������Ƿ���ͬ
	m_bDeadArea = true;			//!< ��ֵ�����Ƿ���ͬ
	m_bLoloActive = true;		//!< �����ޱ��������Ƿ���ͬ
	m_bLoloValue = true;		//!< �����ޱ���ֵ�Ƿ���ͬ
	m_bLowActive = true;		//!< ���ޱ��������Ƿ���ͬ
	m_bLowValue = true;			//!< ���ޱ���ֵ�Ƿ���ͬ
	m_bHighActive = true;		//!< ���ޱ��������Ƿ���ͬ
	m_bHighValue = true;		//!< ���ޱ���ֵ�Ƿ���ͬ
	m_bHihiActive = true;		//!< �����ޱ��������Ƿ���ͬ
	m_bHihiValue = true;		//!< �����ޱ���ֵ�Ƿ���ͬ
	m_bAimActive = true;		//!< Ŀ�걨�������Ƿ���ͬ
	m_bAimPercent = true;		//!< Ŀ�걨���ٷֱ��Ƿ���ͬ
	m_bAimDeadPercent = true;	//!< Ŀ�걨���ٷֱ������Ƿ���ͬ
	m_bAimValue = true;			//!< Ŀ�걨��ֵ�Ƿ���ͬ
	m_bShiftActive = true;		//!< �仯�ʱ��������Ƿ���ͬ
	m_bShiftPercent = true;		//!< �仯�ʱ����ٷֱ��Ƿ���ͬ
	m_bShiftTime = true;		//!< �仯��ȡֵʱ���Ƿ���ͬ

	//!< �Ƚ�ÿһ�����ҵ���Щ����һ������Щ���Բ�һ��
	*m_ShowItem = *m_ltEditItem.front();
	std::shared_ptr<CItem> item, front;
	front = m_ltEditItem.front();
	std::shared_ptr<CPropertySource> showSrc, itemSrc;
	std::shared_ptr<CPropertyAlarm> showAlarm, itemAlarm;
	showSrc = m_ShowItem->getSrcInfo();
	showAlarm = m_ShowItem->getAlarmInfo();
	ASSERT(showSrc);
	ASSERT(showAlarm);
	for (auto item : m_ltEditItem)
	{
		if(item == front)		continue;
		itemSrc = item->getSrcInfo();
		itemAlarm = item->getAlarmInfo();
		ASSERT(itemSrc);
		ASSERT(itemAlarm);
		//!< ��ʼ����ƥ��
		if(m_bValType)			m_bValType = (m_ShowItem->getValType() == item->getValType());
		if(m_bSrcType)			m_bSrcType = (m_ShowItem->getSrcType() == item->getSrcType());
		if(m_bGroup)			m_bGroup = (m_ShowItem->getMyGroupID() == item->getMyGroupID());
		if(m_bDefValue)			m_bDefValue = (m_ShowItem->getDefault() == item->getDefault());
		if(m_bAccessRight)		m_bAccessRight = (m_ShowItem->getAccessRight() == item->getAccessRight());
		if(m_bReservFlag)		m_bReservFlag = (m_ShowItem->getReservFlag() == item->getReservFlag());
		if(m_bReservDB)			m_bReservDB = (m_ShowItem->getReservDB() == item->getReservDB());
		if(m_bModbus485)		m_bModbus485 = (m_ShowItem->getModbus485() == item->getModbus485());

		if(m_bDev)				m_bDev = (showSrc->getDeviceID() == itemSrc->getDeviceID());
		if(m_bFreshTime)		m_bFreshTime = (showSrc->getFreshTime() == itemSrc->getFreshTime());
		if(m_bDelayFlag)		m_bDelayFlag = (showSrc->getDelayFlag() == itemSrc->getDelayFlag());
		if(m_bConvertFlag)		m_bConvertFlag = (showSrc->getConvertType() == itemSrc->getConvertType());
		if(m_bMinProj)			m_bMinProj = (showSrc->getProjMin() == itemSrc->getProjMin());
		if(m_bMaxProj)			m_bMaxProj = (showSrc->getProjMax() == itemSrc->getProjMax());
		if(m_bMinIO)			m_bMinIO = (showSrc->getIOMin() == itemSrc->getIOMin());
		if(m_bMaxIO)			m_bMaxIO = (showSrc->getIOMax() == itemSrc->getIOMax());

		if(m_bBitAlarmType)		m_bBitAlarmType = (showAlarm->getBitAlarmType() == itemAlarm->getBitAlarmType());
		if(m_bDeadArea)			m_bDeadArea = (showAlarm->getDeadArea() == itemAlarm->getDeadArea());
		if(m_bLoloActive)		m_bLoloActive = (showAlarm->getLoloActive() == itemAlarm->getLoloActive());
		if(m_bLoloValue)		m_bLoloValue = (showAlarm->getLoloValue() == itemAlarm->getLoloValue());
		if(m_bLowActive)		m_bLowActive = (showAlarm->getLowActive() == itemAlarm->getLowActive());
		if(m_bLowValue)			m_bLowValue = (showAlarm->getLowValue() == itemAlarm->getLowValue());
		if(m_bHighActive)		m_bHighActive = (showAlarm->getHighActive() == itemAlarm->getHighActive());
		if(m_bHighValue)		m_bHighValue = (showAlarm->getHighValue() == itemAlarm->getHighValue());
		if(m_bHihiActive)		m_bHihiActive = (showAlarm->getHihiActive() == itemAlarm->getHighActive());
		if(m_bHihiValue)		m_bHihiValue = (showAlarm->getHihiValue() == itemAlarm->getHihiValue());
		if(m_bAimActive)		m_bAimActive = (showAlarm->getAimActive() == itemAlarm->getAimActive());
		if(m_bAimPercent)		m_bAimPercent = (showAlarm->getAimPercent() == itemAlarm->getAimPercent());
		if(m_bAimDeadPercent)	m_bAimDeadPercent = (showAlarm->getAimDeadPercent() == itemAlarm->getAimDeadPercent());
		if(m_bAimValue)			m_bAimValue = (showAlarm->getAimValue() == itemAlarm->getAimValue());
		if(m_bShiftActive)		m_bShiftActive = (showAlarm->getShiftActive() == itemAlarm->getShiftActive());
		if(m_bShiftPercent)		m_bShiftPercent = (showAlarm->getShiftDelta() == itemAlarm->getShiftDelta());
		if(m_bShiftTime)		m_bShiftTime = (showAlarm->getShiftTime() == itemAlarm->getShiftTime());
	}
}

//!< Ҫ���汻�ı������
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
	std::shared_ptr<CPropertyAlarm> itemAlarm = pItem->getAlarmInfo();
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();
	std::shared_ptr<CPropertyAlarm> showAlarm = m_ShowItem->getAlarmInfo();
	ASSERT(pItem);
	ASSERT(itemSrc);
	ASSERT(itemAlarm);
	ASSERT(showSrc);
	ASSERT(showAlarm);

	m_bValType = true;			//!< ����ֵ�����Ƿ���ͬ
	m_bSrcType = true;			//!< ����Դ�����Ƿ���ͬ
	m_bGroup = true;			//!< �����������Ƿ���ͬ
	m_bDefValue = true;			//!< Ĭ��ֵ�����Ƿ���ͬ
	m_bAccessRight = true;		//!< ����Ȩ���Ƿ���ͬ
	m_bReservFlag = true;		//!< �Ƿ��Ǳ���ֵ�Ƿ���ͬ
	m_bReservDB = true;			//!< �Ƿ��Ǳ������ݿ��Ƿ���ͬ
	m_bModbus485 = true;		//!< �Ƿ񵼳�Modbus����
	m_bDev = true;				//!< �����豸�Ƿ���ͬ
	m_bFreshTime = true;		//!< ˢ��ʱ���Ƿ���ͬ
	m_bDelayFlag = true;		//!< �Ƿ������ͺ��Ƿ���ͬ
	m_bConvertFlag = true;		//!< ת�������Ƿ���ͬ
	m_bMinProj = true;			//!< ��С����ֵ�Ƿ���ͬ
	m_bMaxProj = true;			//!< ��󹤳�ֵ�Ƿ���ͬ
	m_bMinIO = true;			//!< ��Сԭʼֵ�Ƿ���ͬ
	m_bMaxIO = true;			//!< ���ԭʼֵ�Ƿ���ͬ
	m_bBitAlarmType = true;		//!< ���ر��������Ƿ���ͬ
	m_bDeadArea = true;			//!< ��ֵ�����Ƿ���ͬ
	m_bLoloActive = true;		//!< �����ޱ��������Ƿ���ͬ
	m_bLoloValue = true;		//!< �����ޱ���ֵ�Ƿ���ͬ
	m_bLowActive = true;		//!< ���ޱ��������Ƿ���ͬ
	m_bLowValue = true;			//!< ���ޱ���ֵ�Ƿ���ͬ
	m_bHighActive = true;		//!< ���ޱ��������Ƿ���ͬ
	m_bHighValue = true;		//!< ���ޱ���ֵ�Ƿ���ͬ
	m_bHihiActive = true;		//!< �����ޱ��������Ƿ���ͬ
	m_bHihiValue = true;		//!< �����ޱ���ֵ�Ƿ���ͬ
	m_bAimActive = true;		//!< Ŀ�걨�������Ƿ���ͬ
	m_bAimPercent = true;		//!< Ŀ�걨���ٷֱ��Ƿ���ͬ
	m_bAimDeadPercent = true;	//!< Ŀ�걨���ٷֱ������Ƿ���ͬ
	m_bAimValue = true;			//!< Ŀ�걨��ֵ�Ƿ���ͬ
	m_bShiftActive = true;		//!< �仯�ʱ��������Ƿ���ͬ
	m_bShiftPercent = true;		//!< �仯�ʱ����ٷֱ��Ƿ���ͬ
	m_bShiftTime = true;		//!< �仯��ȡֵʱ���Ƿ���ͬ

	for(int i = 0; i < num; ++i){
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< ֻ�б��޸ĵĲŻ���б��棬�Ż���һ��
		itemID = item->GetID();
		itemValue = item->GetValue();

		if(ITEM_VALTYPE_ID == itemID){						//!< ����ֵ����
			m_ShowItem->setValType(item->GetConstraints()->GetCurrent());
//			m_bValType = (m_ShowItem->getValType() == pItem->getValType());
			m_bValType = false;
		}
		else if(ITEM_SRCTYPE_ID == itemID){					//!< ����Դ����
			m_ShowItem->setSrcType(item->GetConstraints()->GetCurrent());
//			m_bSrcType = (m_ShowItem->getSrcType() == pItem->getSrcType());
			m_bSrcType = false;
		}
		else if(ITEM_GROUP_ID == itemID){					//!< �����������Ƿ���ͬ
			std::shared_ptr<CItemGroup> itemGroup = itemMgr->GetGroup(itemValue.Trim());
			m_ShowItem->setMyGroupID(itemGroup ? itemGroup->getID() : 0);
			m_bGroup = false;
		}
		else if(ITEM_ACCESSRIGHT_ID == itemID){				//!< ����Ȩ��
			m_ShowItem->setAccessRight(item->GetConstraints()->GetCurrent());
			m_bAccessRight = false;
		}
		else if(ITEM_DEFAULTVALUE_ID == itemID){			//!< Ĭ��ֵ
			CComVariant cvrDef = item->GetValue();
			cvrDef.ChangeType(m_ShowItem->getDefault().vt);
			m_ShowItem->setDefault(cvrDef);
			m_bDefValue = false;
		}
		else if(ITEM_RESERVE_ID == itemID){					//!< �Ƿ���
			m_ShowItem->setReservFlag(item->GetConstraints()->GetCurrent());
			m_bReservFlag = false;
		}
		else if(ITEM_RESFDB_ID == itemID){					//!< �Ƿ�����ݿ�
			m_ShowItem->setReservDB(item->GetConstraints()->GetCurrent());
			m_bReservDB = false;
		}
		else if(ITEM_MODBUS485_ID == itemID){				//!< �Ƿ񵼳�Modbus����
			m_ShowItem->setModbus485(item->GetConstraints()->GetCurrent());
			m_bModbus485 = false;
		}
		else if(SRC_FRESHTIME_ID == itemID){				//!< ˢ��ʱ��
			showSrc->setFreshTime(((CXTPPropertyGridItemNumber *)item)->GetNumber());
			m_bFreshTime = false;
		}
		else if(SRC_DEVICE_ID == itemID){					//!< �����豸
			MVC::Device::CDevMgr* mgr = &MVC::Device::CDevMgr::GetMe();
			std::shared_ptr<MVC::Device::CDeviceOne> device;
			device = mgr->GetDevice(itemValue);
			if(device)
				showSrc->setDevID(device->getID());
			m_bDev = false;
		}
		else if(SRC_DELAYFLAG_ID == itemID){				//!< �Ƿ������ͺ�
			showSrc->setDealyFlag(item->GetConstraints()->GetCurrent());
			m_bDelayFlag = false;
		}
		else if(SRC_CONVERT_ID == itemID){					//!< ת������
			showSrc->setConvertType(item->GetConstraints()->GetCurrent());
			m_bConvertFlag = false;
		}
		else if(SRC_MINPROJ_ID == itemID){					//!< ��С����ֵ
			CComVariant cvrMinProj = item->GetValue();
			cvrMinProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMin(cvrMinProj);
			m_bMinProj = false;
		}
		else if(SRC_MAXPROJ_ID == itemID){					//!< ��󹤳�ֵ
			CComVariant cvrMaxProj = item->GetValue();
			cvrMaxProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMax(cvrMaxProj);
			m_bMaxProj = false;
		}
		else if(SRC_MINIO_ID == itemID){					//!< ��Сԭʼֵ
			CComVariant cvrMinIO = item->GetValue();
			cvrMinIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMin(cvrMinIO);
			m_bMinIO = false;
		}
		else if(SRC_MAXIO_ID == itemID){					//!< ���ԭʼֵ
			CComVariant cvrMaxIO = item->GetValue();
			cvrMaxIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMax(cvrMaxIO);
			m_bMaxIO = false;
		}
		else if(ALARM_BITTYPE_ID == itemID){				//!< Ϊ������������
			showAlarm->setBitAlarmType(item->GetConstraints()->GetCurrent());
			m_bBitAlarmType = false;
		}
		else if(ALARM_DEADAREA_ID == itemID){				//!< ��ֵ����
			showAlarm->setDeadArea(((CXTPPropertyGridItemNumber *)item)->GetNumber());
			m_bDeadArea = false;
		}
		else if(ALARM_LOLOACTIVE_ID == itemID){				//!< �����ޱ�������
			showAlarm->setLoloActive(item->GetConstraints()->GetCurrent());
			m_bLoloActive = false;
		}
		else if(ALARM_LOLOVALUE_ID == itemID){				//!< �����ޱ���ֵ
			showAlarm->setLoloValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
			m_bLoloValue = false;
		}
		else if(ALARM_LOWACTIVE_ID == itemID){				//!< ���ޱ�������
			showAlarm->setLowActive(item->GetConstraints()->GetCurrent());
			m_bLowActive = false;
		}
		else if(ALARM_LOWVALUE_ID == itemID){				//!< ���ޱ���ֵ
			showAlarm->setLowValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
			m_bLowValue = false;
		}
		else if(ALARM_HIGHACTIVE_ID == itemID){				//!< ���ޱ�������
			showAlarm->setHighActive(item->GetConstraints()->GetCurrent());
			m_bHighActive = false;
		}
		else if(ALARM_HIGHVALUE_ID == itemID){				//!< ���ޱ���ֵ
			showAlarm->setHighValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
			m_bHighValue = false;
		}
		else if(ALARM_HIHIACTIVE_ID == itemID){				//!< �����ޱ�������
			showAlarm->setHihiActive(item->GetConstraints()->GetCurrent());
			m_bHihiActive = false;
		}
		else if(ALARM_HIHIVALUE_ID == itemID){				//!< �����ޱ���ֵ
			showAlarm->setHihiValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
			m_bHihiValue = false;
		}
		else if(ALARM_AIMACTIVE_ID == itemID){				//!< Ŀ�걨������
			showAlarm->setAimActive(item->GetConstraints()->GetCurrent());
			m_bAimActive = false;
		}
		else if(ALARM_AIMVALUE_ID == itemID){				//!< Ŀ�걨��ֵ
			showAlarm->setAimValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
			m_bAimValue = false;
		}
		else if(ALARM_AIMPERCENT_ID == itemID){				//!< Ŀ�걨��ƫ��ٷֱ�
			showAlarm->setAimPercent((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
			m_bAimPercent = false;
		}
		else if(ALARM_AIMDEAD_ID == itemID){				//!< Ŀ�걨������
			showAlarm->setAimDeadPercent((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
			m_bAimDeadPercent = false;
		}
		else if(ALARM_SHIFTACTIVE_ID == itemID){			//!< �仯�ʼ���
			showAlarm->setShiftActive(item->GetConstraints()->GetCurrent());
			m_bShiftActive = false;
		}
		else if(ALARM_SHIFTDELTA_ID == itemID){			//!< �仯�ʰٷֱ�
			showAlarm->setShiftDelta((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
			m_bShiftPercent = false;
		}
		else if(ALARM_SHIFTTIME_ID == itemID){				//!< �仯�ʱȶ�����
			showAlarm->setShiftTime(((CXTPPropertyGridItemNumber *)item)->GetNumber());
			m_bShiftTime = false;
		}
	}

	//!< ��ʼ�޸������豸������
	for (auto pItem : m_ltEditItem)
	{
		ASSERT(pItem);
		itemSrc = pItem->getSrcInfo();
		itemAlarm = pItem->getAlarmInfo();
		ASSERT(itemSrc);
		ASSERT(itemAlarm);

		if(!m_bValType)				pItem->setValType(m_ShowItem->getValType());
		if(!m_bSrcType)				pItem->setSrcType(m_ShowItem->getSrcType());
		if(!m_bGroup)				pItem->setMyGroupID(m_ShowItem->getMyGroupID());
		if(!m_bDefValue)			pItem->setDefault(m_ShowItem->getDefault());
		if(!m_bAccessRight)			pItem->setAccessRight(m_ShowItem->getAccessRight());
		if(!m_bReservFlag)			pItem->setReservFlag(m_ShowItem->getReservFlag());
		if(!m_bReservDB)			pItem->setReservDB(m_ShowItem->getReservDB());
		if(!m_bModbus485)			pItem->setModbus485(m_ShowItem->getModbus485());
		if(!m_bDev)					itemSrc->setDevID(showSrc->getDeviceID());
		if(!m_bFreshTime)			itemSrc->setFreshTime(showSrc->getFreshTime());
		if(!m_bDelayFlag)			itemSrc->setDealyFlag(showSrc->getDelayFlag());
		if(!m_bConvertFlag)			itemSrc->setConvertType(showSrc->getConvertType());
		if(!m_bMinProj)				itemSrc->setProjMin(showSrc->getProjMin());
		if(!m_bMaxProj)				itemSrc->setProjMax(showSrc->getProjMax());
		if(!m_bMinIO)				itemSrc->setIOMin(showSrc->getIOMin());
		if(!m_bMaxIO)				itemSrc->setIOMax(showSrc->getIOMax());
		if(!m_bBitAlarmType)		itemAlarm->setBitAlarmType(showAlarm->getBitAlarmType());
		if(!m_bDeadArea)			itemAlarm->setDeadArea(showAlarm->getDeadArea());
		if(!m_bLoloActive)			itemAlarm->setLoloActive(showAlarm->getLoloActive());
		if(!m_bLoloValue)			itemAlarm->setLoloValue(showAlarm->getLoloValue());
		if(!m_bLowActive)			itemAlarm->setLowActive(showAlarm->getLowActive());
		if(!m_bLowValue)			itemAlarm->setLowValue(showAlarm->getLowValue());
		if(!m_bHighActive)			itemAlarm->setHighActive(showAlarm->getHighActive());
		if(!m_bHighValue)			itemAlarm->setHighValue(showAlarm->getHighValue());
		if(!m_bHihiActive)			itemAlarm->setHihiActive(showAlarm->getHihiActive());
		if(!m_bHihiValue)			itemAlarm->setHihiValue(showAlarm->getHihiValue());
		if(!m_bAimActive)			itemAlarm->setAimActive(showAlarm->getAimActive());
		if(!m_bAimPercent)			itemAlarm->setAimPercent(showAlarm->getAimPercent());
		if(!m_bAimDeadPercent)		itemAlarm->setAimDeadPercent(showAlarm->getAimDeadPercent());
		if(!m_bAimValue)			itemAlarm->setAimValue(showAlarm->getAimValue());
		if(!m_bShiftActive)			itemAlarm->setShiftActive(showAlarm->getShiftActive());
		if(!m_bShiftPercent)		itemAlarm->setShiftDelta(showAlarm->getShiftDelta());
		if(!m_bShiftTime)			itemAlarm->setShiftTime(showAlarm->getShiftTime());

		if(!m_bReservFlag || !m_bReservDB)
			CProjectMgr::GetMe().GetProj()->SetModify(true);
		if(SoftInfo::CSoftInfo::GetMe().IsModbus485() && !m_bModbus485)
			CProjectMgr::GetMe().GetProj()->SetModify(true);
	}
	return true;
}

//!< ��ĳһ��޸�ʱ
void CPropertyItems::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)			return;
	CString itemValue = item->GetValue();

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
}

//!< ��ʾ������Ϣ
void CPropertyItems::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("ͳһ�޸�"));
}