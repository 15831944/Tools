#include "stdafx.h"
#include "DXPEditor.h"
#include "SoftInfo.h"
#include "Gbl.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ItemDoc.h"
#include "AlarmProperty.h"
#include "SourceProperty.h"
#include "ItemGroup.h"

#include "DevMgr.h"
#include "DeviceOne.h"
#include "XmlDevice.h"
#include "XmlArea.h"

using namespace MVC;
using namespace Item;

const CString ITEM_ID = _T("id");
const CString ITEM_NAME = _T("Name");
const CString ITEM_GROUP = _T("GroupID");
const CString ITEM_TAG = _T("Tag");
const CString ITEM_DESCRIPTION = _T("Description");
const CString ITEM_VALTYPE = _T("ValType");
const CString ITEM_SRCTYPE = _T("SrcType");
const CString ITEM_ACCESSRIGHT = _T("OprAttribute");			//!< �ɶ���д��ֻ����ֻд
const CString ITEM_DEFVALUE = _T("DefaultValue");
const CString ITEM_RESFLAG = _T("ReservFlag");
const CString ITEM_RESFDB = _T("ReservDB");
const CString ITEM_MODBUS485 = _T("Modbus485");
const CString ITEM_ARRAYFLAG = _T("ArrayFlag");

const CString SRC = _T("SrcProp");
const CString ALARM = _T("AlarmProp");

const CString VAL_TYPE[] = {_T("λ����"), _T("�з����ֽ�"), _T("�޷����ֽ�"), _T("�з��ŵ���"), _T("�޷��ŵ���"),
_T("�з���˫��"), _T("�޷���˫��"), _T("�����ȸ���")/*, _T("˫���ȸ���")*/};

CItem::CItem(CString name)
{
	m_uiIRefCount = 0;
	m_strName = name;
	m_strTag = _T("");
	m_strDescription = _T("");
	m_uiValType = 5;					//!< Ĭ�ϳ�int32
	m_cvrValueOld = m_cvrValue = 0;
	m_cvrIOValueOld = m_cvrIOValue = 0;
	m_uiSrcType = 1;					//!< Ĭ�ϳ�IO����
	m_uiGroupID = 0;					//!< ���������������ID��
	m_uiAccessRight = 0;
	m_bReservFlag = FALSE;
	m_bReservDB = FALSE;
	m_bModbus485 = FALSE;				//!< �Ƿ񵼳�Modbus����
	m_bAlarmBit = false;				//!< λ�����Ƿ񱨾�
	m_bAlarmLolo = false;				//!< �������Ƿ񱨾�
	m_bAlarmLow = false;				//!< �����Ƿ񱨾�
	m_bAlarmHigh = false;				//!< �����Ƿ񱨾�
	m_bAlarmHihi = false;				//!< �������Ƿ񱨾�
	m_bAlarmAim = false;				//!< Ŀ���Ƿ񱨾�
	m_bAlarmShift = false;				//!< �仯���Ƿ񱨾�

	m_spSrcInfo = std::shared_ptr<CPropertySource>(new CPropertySource(this));
	m_spAlarmInfo = std::shared_ptr<CPropertyAlarm>(new CPropertyAlarm(this));

	ChangeVarType(VT_I4);
	::GetLocalTime(&m_stCreateTime);
	m_stUpdateTime = m_stCreateTime;
}

CItem::~CItem(void)
{
}

//!< ����xml��ʽ�ı༭��Ϣ,iExport=0������1������2����
bool CItem::SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport/* = false*/)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;

			if(name == ITEM_NAME)				{m_strName = cv;}
			else if(name == ITEM_TAG)			{m_strTag = cv;}
			else if(name == ITEM_ID)			{m_uiID = (UINT)pAttr->IntValue();}
			else if(name == ITEM_GROUP)			{m_uiGroupID = (UINT)pAttr->IntValue();}
			else if(name == ITEM_VALTYPE)		{m_uiValType = (UINT)pAttr->IntValue();		SetVarType();}
			else if(name == ITEM_SRCTYPE)		{m_uiSrcType = (UINT)pAttr->IntValue();}
			else if(name == ITEM_ACCESSRIGHT)	{m_uiAccessRight = (UINT)pAttr->IntValue();}
			else if(name == ITEM_DEFVALUE)		{m_cvrDefaultValue = cv;}
			else if(name == ITEM_RESFLAG)		{m_bReservFlag = (strValue == "1") ? TRUE : FALSE;}
			else if(name == ITEM_RESFDB)		{m_bReservDB = (strValue == "1") ? TRUE : FALSE;}
			else if(name == ITEM_MODBUS485)		{m_bModbus485 = (strValue == "1") ? TRUE : FALSE;}		//!< �Ƿ񵼳�Modbus����}
			else if(name == ITEM_DESCRIPTION)	{m_strDescription = cv;}
			else if(name == ITEM_ARRAYFLAG)		{}
			pAttr = pAttr->Next();
		}

		TiXmlElement* pChild = pNode->FirstChildElement();
		while(pChild)
		{
			name = pChild->Value();
			if(name == SRC)				m_spSrcInfo->SerializeXml(pChild, bRead, iExport);
			else if(name == ALARM)		m_spAlarmInfo->SerializeXml(pChild, bRead);
			pChild = pChild->NextSiblingElement();
		}
	}
	else
	{
		int t = m_cvrDefaultValue.vt;
		m_cvrDefaultValue.ChangeType(VT_BSTR);
		pNode->SetAttribute(ITEM_ID, getID());
		pNode->SetAttribute(ITEM_NAME, getName());
		pNode->SetAttribute(ITEM_TAG, getTag());
		pNode->SetAttribute(ITEM_DESCRIPTION, getDescription());
		pNode->SetAttribute(ITEM_VALTYPE, getValType());
		pNode->SetAttribute(ITEM_SRCTYPE, getSrcType());
		pNode->SetAttribute(ITEM_ACCESSRIGHT, getAccessRight());
		pNode->SetAttribute(ITEM_DEFVALUE, (CString)m_cvrDefaultValue.bstrVal);
		pNode->SetAttribute(ITEM_RESFLAG, getReservFlag()?1:0);
		pNode->SetAttribute(ITEM_RESFDB, getReservDB()?1:0);
		pNode->SetAttribute(ITEM_MODBUS485, getModbus485()?1:0);
		pNode->SetAttribute(ITEM_GROUP, getMyGroupID());
		m_cvrDefaultValue.ChangeType(t);

		m_spSrcInfo->SerializeXml(pNode->AddTiXmlChild((LPCTSTR)SRC), bRead, iExport);
		m_spAlarmInfo->SerializeXml(pNode->AddTiXmlChild((LPCTSTR)ALARM), bRead);
	}
	return true;
}

//!< ��PLC_Config�����ı������ж�ȡ����
//bool MVC::Item::CItem::ReadFromPlcConfig(COperation& reader, int nRow, int devID)
bool MVC::Item::CItem::ReadFromPlcConfig(CString strLine, int devID)
{
	std::vector<CString> vtCell;
	CGbl::SpliteBy(strLine, "\t", vtCell);
	if (vtCell.size() > 6 || vtCell.size() < 4)			return false;
// 	CString str = reader.ReadRow(nRow, 1, 5);
	if (vtCell[0].Trim() == _T(""))						return false;
	if (vtCell[3].Trim() == _T(""))						return false;

	CString strAddr = vtCell[0].Trim();					//!< ������ַ
	if(CGbl::GetMe().IsNumber(strAddr))  return false;	//!< ��һ��Ϊ����֤����DView������Excle��ʽ�ļ�
	CString strType = vtCell[3].Trim();					//!< ��������
	CString strName = vtCell[2].Trim();					//!< ������
	if (strName != _T(""))		setItemName(strName);
	else						setItemName(strAddr);
	setTag(vtCell[2].Trim());							//!< ������ǩ
	if (vtCell.size() > 4)
		setDescription(vtCell[4].Trim());				//!< ����ע��
	setSrcType(CItem::SRC_TYPE_IO);										//!< ��������Դ����
	if (getSrcInfo()->SetPlcConfigAddr(devID, strAddr, strType))		//!< ����Դ��Ϣ
		setValType(getSrcInfo()->getIOType());
	else
		setValType(strType);
	return true;
}

//!< ��DView�����ı������ж�ȡ����
//bool CItem::ReadFromDView(COperation& reader, int nRow, int devID)
bool MVC::Item::CItem::ReadFromDView(CString strLine, int devID)
{
	std::vector<CString> vtCell;
	CGbl::SpliteBy(strLine, "\t", vtCell);
	if (vtCell.size() < 40)								return false;
	CString str = vtCell[40];
	if (vtCell[1].Trim() == _T(""))						return false;
	if (vtCell[6].Trim() == _T(""))						return false;
	if (vtCell[7].Trim() == _T(""))						return false;

	CString strID = vtCell[0].Trim();					//!< �������
	m_strName = vtCell[1].Trim();						//!< ��������
	m_strTag = vtCell[2].Trim();						//!< ������ǩ
	m_strDescription = vtCell[3].Trim();				//!< ������ע
	setDefault(vtCell[5].Trim().GetBuffer());			//!< ����Ĭ��ֵ
	setSrcType((UINT)atoi(vtCell[6].Trim()));			//!< ��������Դ����
	setAccessRight((UINT)atoi(vtCell[7].Trim()));		//!< ��������Ȩ��
	setReservFlag(vtCell[8].Trim() == "1");				//!< ��������ֵ
	setReservDB(vtCell[9].Trim() == "1");				//!< ����������ʷ����

	m_spSrcInfo->ReadFromDViewExcel(vtCell);			//!< ��������Դ����
	m_spSrcInfo->setDevID(devID);						//!< ���������豸
	m_spAlarmInfo->ReadFromDViewExcel(vtCell);			//!< ������������

	setValType((UINT)atoi(vtCell[4].Trim())); 			//!< ��������
	return true;
}

//!< ȷ�ϱ���������
void CItem::SetVarType()
{
	//!< �޸�VARIANT����������
	if(m_uiValType == 0)			ChangeVarType(VT_BOOL);
	else if(m_uiValType == 1)		ChangeVarType(VT_I1);
	else if(m_uiValType == 2)		ChangeVarType(VT_UI1);
	else if(m_uiValType == 3)		ChangeVarType(VT_I2);
	else if(m_uiValType == 4)		ChangeVarType(VT_UI2);
	else if(m_uiValType == 5)		ChangeVarType(VT_I4);
	else if(m_uiValType == 6)		ChangeVarType(VT_UI4);
	else if(m_uiValType == 7)		ChangeVarType(VT_R4);
	else if(m_uiValType == 8)		ChangeVarType(VT_R8);
	else							ChangeVarType(VT_BSTR);
}

//!< ����valType��������ͣ�����������variant���͵�����ת��������Ӧ������
void CItem::ChangeVarType(UINT type)
{
	if(type == VT_BOOL)			m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = false;
	else if(type == VT_I1)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = char(0);
	else if(type == VT_UI1)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = byte(0);
	else if(type == VT_I2)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = short(0);
	else if(type == VT_UI2)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = unsigned short(0);
	else if(type == VT_I4)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = int(0);
	else if(type == VT_UI4)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = UINT(0);
	else if(type == VT_R4)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = float(0.0);
	else if(type==VT_R8)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = double(0.0);
}

//!< ���ñ�����ֵ����
void CItem::setValType(UINT type)
{
	if(m_uiValType != type){
		m_uiValType = type;
		if(m_uiValType == 0)			ChangeVarType(VT_BOOL);
		else if(m_uiValType == 1)		ChangeVarType(VT_I1);
		else if(m_uiValType == 2)		ChangeVarType(VT_UI1);
		else if(m_uiValType == 3)		ChangeVarType(VT_I2);
		else if(m_uiValType == 4)		ChangeVarType(VT_UI2);
		else if(m_uiValType == 5)		ChangeVarType(VT_I4);
		else if(m_uiValType == 6)		ChangeVarType(VT_UI4);
		else if(m_uiValType == 7)		ChangeVarType(VT_R4);
		else if(m_uiValType == 8)		ChangeVarType(VT_R8);
		else							ChangeVarType(VT_BSTR);
	}
	if(m_uiValType == 0){	//!< λ����
		getSrcInfo()->setConvertType(0);
		getAlarmInfo()->setLoloActive(false);
		getAlarmInfo()->setLowActive(false);
		getAlarmInfo()->setHighActive(false);
		getAlarmInfo()->setHihiActive(false);
		getAlarmInfo()->setAimActive(false);
		getAlarmInfo()->setShiftActive(false);
	}
	else
	{
		getAlarmInfo()->setBitAlarmType(0);
	}
}

//!< ���ñ�����ֵ����
void CItem::setValType(CString strType)
{
	if (strType == _T("λ����"))					setValType(0);
	else if (strType == _T("�з���˫�ֽ�"))		setValType(3);
	else if (strType == _T("�޷���˫�ֽ�"))		setValType(4);
	else if (strType == _T("�з������ֽ�"))		setValType(5);
	else if (strType == _T("�޷������ֽ�"))		setValType(6);
	else if (strType == _T("���ֽڸ�����"))		setValType(7);
}

CItem& CItem::operator = (CItem& item)
{
	setID(item.getID());
	setItemName(item.getName());
	setTag(item.getTag());
	setDescription(item.getDescription());
	setMyGroupID(item.getMyGroupID());
	setDefault(item.getDefault());
	setValType(item.getValType());
	setSrcType(item.getSrcType());
	setAccessRight(item.getAccessRight());
	setReservFlag(item.getReservFlag());
	setReservDB(item.getReservDB());
	setModbus485(item.getModbus485());

	(*m_spSrcInfo) = (*item.getSrcInfo());
	(*m_spAlarmInfo) = (*item.getAlarmInfo());
	return *this;
}

bool CItem::operator == (CItem &item) const
{
	if (m_strName != item.getName())					return false;	//!< ��������
	if (m_strTag != item.getTag())						return false;	//!< ������ǩ
	if (m_strDescription != item.getDescription())		return false;	//!< ������ע
	if (m_uiGroupID != item.getMyGroupID())				return false;	//!< ������
	if (m_cvrDefaultValue != item.getDefault())			return false;	//!< ����Ĭ��ֵ
	if (m_uiValType != item.getValType())				return false;	//!< ��������
	if (m_uiSrcType != item.getSrcType())				return false;	//!< ����Դ���ͣ�0�ڴ������1I/O����
	if (m_uiAccessRight != item.getAccessRight())		return false;	//!< ����Ȩ�ޣ�0�ɶ���д��1ֻ����ֻд
	if (m_bReservFlag != item.getReservFlag())			return false;	//!< �Ƿ����óɱ���ֵ��true���ã�false������
	if (m_bReservDB != item.getReservDB())				return false;	//!< �Ƿ񱣴���ʷ���ݣ�true������false������
	if (m_bModbus485 != item.getModbus485())			return false;	//!< �Ƿ񵼳�Modbus����

	if (!(*m_spSrcInfo == *item.getSrcInfo()))			return false;
	if (!(*m_spAlarmInfo == *item.getAlarmInfo()))		return false;
	return true;
}

//!< Ҫ��¡һ���Լ����������ﻹ��һ��������ID��������ܲ��ˣ����ڱ�ĵط���
bool CItem::OnCloneMe(CItem& item, int index, int addrUnit, int nType /* = 0 */)
{
	item = *this;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	item.setMyGroupID(getMyGroupID());
	//!< ��IO�������⴦��
	if(m_uiSrcType > 0)		OnCloneDealIOInfo(item, index * addrUnit);

	//!< 
	item.setID(UINT(-1));
	CString name = getName();
	CString text;
	if(nType == 0)
	{
		int number = 0;
		CGbl::GetNumberFromString(name, number, 1);
		CString strNumber;
		strNumber.Format("%d",number);
		int clonenumber = SoftInfo::CSoftInfo::GetMe().getCloneNumber();
		text.Format("%d", number + index * clonenumber);
		if(text.GetLength() > strNumber.GetLength() && !name.IsEmpty() && name.Right(1) == _T("0")){
			name = name.Left(name.GetLength() - 1);
		}
		name += text;
	}
	else
	{
		CString strInt, strDeci;
		CGbl::GetFloatFromString(name, strInt, strDeci, 1);
		if(strInt == _T(""))		strDeci = _T("0");
		if(strDeci == _T(""))		strDeci = _T("0");
		int uiInt, uiDeci;
		uiInt = atoi(strInt);
		uiDeci = atoi(strDeci);
		int uiFlt = SoftInfo::CSoftInfo::GetMe().getCloneFloat();
		if(uiDeci >= uiFlt)
		{
			uiDeci = 0;
			++uiInt;
		}
		uiDeci += index;
		while(uiDeci >= uiFlt)
		{
			uiDeci -= uiFlt;
			++uiInt;
		}
		while(uiDeci < 0)
		{
			uiDeci += uiFlt;
			--uiInt;
		}
		text.Format("%d.%d", uiInt, uiDeci);
		name += text;
	}
	if(itemMgr->GetItem(name))
	{
		CString strNewName = name;
		strNewName = strNewName + _T("���ظ���0");
		strNewName = itemMgr->GetSimilarName(strNewName);
		text = _T("������")+ name +_T("���Ѵ��ڣ������Ķ��ظ��Ʊ�������Ϊ��") + strNewName + _T("��");
		name = strNewName;
		CGbl::PrintOut(text);
	}
	item.setItemName(name);
	return true;
}

void CItem::OnCloneDealIOInfo(CItem& item, int addrUnit)
{
	std::shared_ptr<MVC::Device::CDeviceOne> projDev = MVC::Device::CDevMgr::GetMe().GetDevice(item.getSrcInfo()->getDeviceID());
	if(!projDev)		return;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();
	if(!xmlDev)			return;
	std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(item.getSrcInfo()->getAreaID());
	if(!xmlArea)		return;
	UINT bitLen = xmlArea->m_uiUnitBitLen;			//!< ���Ѱַ���ȣ�PEC8000����Ѱַ��������16

	UINT ioType = item.getSrcInfo()->GetOperateType();
	if(ioType == 0)		//!< �������������λ����������λƫ���� + addrUnit
	{
		item.getSrcInfo()->setBitIndex(item.getSrcInfo()->getBitIndex() + addrUnit);
		if(item.getSrcInfo()->getBitIndex() >= bitLen)
		{
			UINT unitNum = (UINT)item.getSrcInfo()->getBitIndex() / bitLen;
			item.getSrcInfo()->setBitIndex(item.getSrcInfo()->getBitIndex() % bitLen);
			item.getSrcInfo()->setUnitIndex(item.getSrcInfo()->getUnitIndex() + unitNum);
		}
	}
	else if(ioType == 1)//!< ����������Ͳ���λ����������Ѱַƫ���� + addrUnit
	{
		item.getSrcInfo()->setUnitIndex(item.getSrcInfo()->getUnitIndex() + addrUnit);
	}
	else if(ioType == 2)//!< ����������Ͳ���λ����������Ѱַƫ���� + index
	{
		item.getSrcInfo()->setUnitIndex(item.getSrcInfo()->getUnitIndex() + addrUnit);
	}
	else if(ioType == 4)//!< ����������Ͳ���λ����������Ѱַƫ���� + index
	{
		item.getSrcInfo()->setUnitIndex(item.getSrcInfo()->getUnitIndex() + addrUnit);
	}
}

//!< ���һ������ֵ
CString CItem::GetCheckValue()
{
	CString value;
	value.Format("%d", getID() + 1);	//!< +1 ��Ϊ�˷�ֹ��Щ��ʽд0��ȥ�Ƿ����������������0
	if(m_uiValType == 0)			return _T("1.000001");
	else							return _T("0.000001");
// 	else if(m_uiValType == 1)		return value;
// 	else if(m_uiValType == 2)		return value;
// 	else if(m_uiValType == 3)		return value;
// 	else if(m_uiValType == 4)		return value;
// 	else if(m_uiValType == 5)		return value;
// 	else if(m_uiValType == 6)		return value;
// 	else if(m_uiValType == 7)		return value + _T(".1");
// 	else if(m_uiValType == 8)		return value + _T(".1");
	return value;
}

//!< ���ַ�������ʽ���ع���ֵ����
CString CItem::GetValTypeStr()
{
	CString type[] = {_T("λ����"), _T("�з����ֽ�"), _T("�޷����ֽ�"), _T("�з�����"), _T("�޷�����"), \
		_T("�з���˫��"), _T("�޷���˫��"), _T("����"), _T("˫����"), _T("�ַ���")};
	if(m_uiValType > 9)		return _T("��������");
	return type[m_uiValType];
}

//!< ���ر��������������
CString CItem::GetGroupName()
{
	std::shared_ptr<CItemGroup> group = CItemMgr::GetMe().GetGroup(getMyGroupID());
	ASSERT(group);
	if(!group)		return _T("");
	return group->getName();
}

//!< ���ñ���
void CItem::SetAlarm(UINT type, bool alarm /* = false */)
{
	bool bAlarmOld = false, bAlarmNew = false;

	//!< ���ԭʼ�ı�����ɫ
	bAlarmOld = IsAlarm();

	if(type == 0)			m_bAlarmBit = alarm;
	else if(type == 1)		m_bAlarmLolo = alarm;
	else if(type == 2)		m_bAlarmLow = alarm;
	else if(type == 3)		m_bAlarmHigh = alarm;
	else if(type == 4)		m_bAlarmHihi = alarm;
	else if(type == 5)		m_bAlarmAim = alarm;
	else if(type == 6)		m_bAlarmShift = alarm;

	//!< ������ڵı�����ɫ
	bAlarmNew = IsAlarm();

	//!< ��ɫ��ͬ��֤����Ҫ�ػ�
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	if(bAlarmOld != bAlarmNew){
		itemMgr->AddEditItem(getID());
	}
	itemMgr->SetEditEnd();
}

bool CItem::IsAlarm()
{
	if(getValType() == 0)		return IsAlarmBit();
	return IsAlarmLolo() | IsAlarmLow() | IsAlarmHigh() | IsAlarmHihi() | IsAlarmAim() | IsAlarmShift();
}

//!< ��س�ʼ�������״̬
void CItem::WatchInit()
{
	SetAlarm(0, false);		//!< λ�����Ƿ񱨾�
	SetAlarm(1, false);		//!< �������Ƿ񱨾�
	SetAlarm(2, false);		//!< �����Ƿ񱨾�
	SetAlarm(3, false);		//!< �����Ƿ񱨾�
	SetAlarm(4, false);		//!< �������Ƿ񱨾�
	SetAlarm(5, false);		//!< Ŀ���Ƿ񱨾�
	SetAlarm(6, false);		//!< �仯���Ƿ񱨾�
}

//!< �жϱ���ֵ�Ƿ�ı�
bool CItem::IsValueChange()
{
	int a[4], b[4];
	memcpy(a, &m_cvrValue, sizeof(CComVariant));
	memcpy(b, &m_cvrValueOld, sizeof(CComVariant));
	if(a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])
		return false;
	m_cvrValueOld = m_cvrValue;
	return true;
}

//!< �жϱ���ԭʼֵ�Ƿ�ı�
bool CItem::IsIOValueChange()
{
	int a[4], b[4];
	memcpy(a, &m_cvrIOValue, sizeof(CComVariant));
	memcpy(b, &m_cvrIOValueOld, sizeof(CComVariant));
	if(a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])
		return false;
	m_cvrIOValueOld = m_cvrIOValue;
	return true;
}

//!< ���ұ�������Ҫƥ����ı���ȫ�ַ�ƥ�䣬��Сдƥ��
bool CItem::DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex /* = false */)
{
	CString strInfo;
	bool bMatch = false;
	int nMatchCount = 0;
	CComVariant cvr;
	strInfo.Format("%d", m_uiID);
	strInfo = _T("Information:����") + strInfo + _T(":���� ") + m_strName + _T(" �����") + strInfo + _T("��  ����  ");

	//!< �����Ƿ�ƥ��
	cvr = m_strName;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("����"), strInfo))		++nMatchCount;

	//!< ����Ƿ�ƥ��
	cvr = m_uiID;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("���"), strInfo))		++nMatchCount;

	//!< ��ǩ�Ƿ�ƥ��
	cvr = m_strTag;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("��ǩ"), strInfo))		++nMatchCount;

	//!< ��ע�Ƿ�ƥ��
	cvr = m_strDescription;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("��ע"), strInfo))		++nMatchCount;

	//!< ��ַ�Ƿ�ƥ��
	if(getSrcType() == SRC_TYPE_IO)
	{
		MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
		std::shared_ptr<MVC::Device::CDeviceOne> dev = devMgr->GetDevice(getSrcInfo()->getDeviceID());
		if(dev){
			cvr = dev->getName();
			if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("�����豸"), strInfo))	++nMatchCount;
		}

		cvr = getSrcInfo()->GetAreaString();
		if(CGbl::SearchT(cvr, str, bMatchWhole, false, bRegex, _T("��ַ"), strInfo))		++nMatchCount;
	}
	//!< �ű��Ƿ�ƥ��
	else if(getSrcType() == SRC_TYPE_MEM)
	{
		cvr = getSrcInfo()->getScriptText();
		if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("�ű�"), strInfo))	++nMatchCount;
	}

	if(nMatchCount > 0){
		CGbl::PrintOut(strInfo);
		return true;
	}
	return false;
}
CString CItem::getStrValue(bool ishex)
{
	CString temp;
	CComVariant cvr = getValue();
	switch (cvr.vt)
	{
	case VT_BOOL:	temp = cvr.boolVal ? _T("1") : _T("0");	break;
	case VT_I1:		ishex ? temp.Format("0x%.2X", (byte)cvr.cVal) : temp.Format("%d", cvr.cVal);	break;
	case VT_I2:		ishex ? temp.Format("0x%.4X", (WORD)cvr.iVal) : temp.Format("%d", cvr.iVal);	break;
	case VT_I4:		ishex ? temp.Format("0x%.8X", (UINT)cvr.intVal) : temp.Format("%d", cvr.intVal);	break;
	case VT_UI1:	ishex ? temp.Format("0x%.2X", (byte)cvr.bVal) : temp.Format("%d", cvr.bVal);	break;
	case VT_UI2:	ishex ? temp.Format("0x%.4X", (byte)cvr.uiVal) : temp.Format("%d", cvr.intVal);	break;
	case VT_UI4:	ishex ? temp.Format("0x%.8X", (byte)cvr.uintVal) : temp.Format("%d", cvr.uintVal);	break;
	case VT_R4:		temp = GetFloatStr(cvr.fltVal);	break;
	default:
		if (cvr.vt != VT_BSTR) cvr.ChangeType(VT_BSTR);
		temp = cvr.bstrVal;
	}
	return temp;
}
CString CItem::getStrIOValue(bool ishex)
{	
	CString temp;
	CComVariant cvr = m_cvrIOValue;
	switch (cvr.vt)
	{
	case VT_BOOL:	temp = cvr.boolVal ? _T("1") : _T("0");	break;
	case VT_I1:		ishex ? temp.Format("0x%.2X", (byte)cvr.cVal) : temp.Format("%d", cvr.cVal);	break;
	case VT_I2:		ishex ? temp.Format("0x%.4X", (WORD)cvr.iVal) : temp.Format("%d", cvr.iVal);	break;
	case VT_I4:		ishex ? temp.Format("0x%.8X", (UINT)cvr.intVal) : temp.Format("%d", cvr.intVal);	break;
	case VT_UI1:	ishex ? temp.Format("0x%.2X", (byte)cvr.bVal) : temp.Format("%d", cvr.bVal);	break;
	case VT_UI2:	ishex ? temp.Format("0x%.4X", (byte)cvr.uiVal) : temp.Format("%d", cvr.intVal);	break;
	case VT_UI4:	ishex ? temp.Format("0x%.8X", (byte)cvr.uintVal) : temp.Format("%d", cvr.uintVal);	break;
	case VT_R4:		temp = GetFloatStr(cvr.fltVal);	break;
	default:
		if (cvr.vt != VT_BSTR) cvr.ChangeType(VT_BSTR);
		temp = cvr.bstrVal;
	}
	return temp;
}
CString CItem::GetFloatStr(float flt)
{
	UINT width = SoftInfo::CSoftInfo::GetMe().getItemFloatWidth();
	CString strFormat;
	strFormat.Format("%d", width);
	strFormat = _T("\%.") + strFormat + _T("f");
	CString strRet;
	strRet.Format(strFormat, flt);
	return strRet;
}
