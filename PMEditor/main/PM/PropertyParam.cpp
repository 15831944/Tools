#include "StdAfx.h"
#include "Gbl.h"
#include "DevMgr.h"
#include "PropertyParam.h"
#include "XmlBehavior.h"
#include "XmlDevice.h"
#include "XmlParaInfo.h"
#include "XmlEnum.h"
#include "DeviceOne.h"
#include "DeviceParam.h"

using namespace MVC;
using namespace Device;

CPropertyParam::CPropertyParam(void)
{
	m_uiGroupIndex = UINT(0);
}

CPropertyParam::~CPropertyParam(void)
{
}

//!< ��ȡ�豸��ţ�����Ϊ����
void CPropertyParam::SetInfo(UINT devID, CString behaviorName, UINT groupIndex, bool bReadOnly /* = false */)
{
	m_bReadOnly = bReadOnly;
	m_ltParamID.clear();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	m_ShowDev = devMgr->GetDevice(devID);
	if(!m_ShowDev)		return;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	std::list< std::shared_ptr<XmlInfo::CXmlBehavior> > ltBehavior = xmlDev->getBehavior(behaviorName);
	ASSERT(!ltBehavior.empty());
	std::shared_ptr<XmlInfo::CXmlBehavior> behavior;
	std::shared_ptr<XmlInfo::CXmlParaInfo> para;

	//!< ��ʼ����Ҫ��ʾ�Ĳ���
	for (auto behavior : ltBehavior)
	{
		for (UINT id : behavior->m_ltParaID)
		{
			if(!FindPara(id))	m_ltParamID.push_back(id);
		}
	}

	//!< �����ǲ���һ���Ǵ��豸����Ϣ
	bool bGroup =  false;
	std::shared_ptr<XmlInfo::CXmlBehavior> xmlBehavior;
	for (auto xmlBehavior : ltBehavior)
	{
		if(xmlBehavior->m_uiArrayNum > 1)		bGroup = true;
	}
	if(bGroup)	m_uiGroupIndex = groupIndex;
	else		m_uiGroupIndex = 0;
}

bool CPropertyParam::FindPara(UINT id)
{
	for (UINT index : m_ltParamID){
		if(index == id)
			return true;
	}
	return false;
}

//!< ��ñ����������
CString CPropertyParam::GetTitle()
{
	return _T("�豸��������");
}

//!< ��ʾ���ĺ���
void CPropertyParam::ShowInfo(CXTPPropertyGrid& grid)
{
	grid.ResetContent();
	if(m_ltParamID.empty())		return;
	ASSERT(m_ShowDev);
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	std::shared_ptr<XmlInfo::CXmlParaInfo> xmlPara;
	std::shared_ptr<MVC::Device::CDeviceParam> devParam;

	CXTPPropertyGridItem* pGroupRoot;				//!< ��������
	pGroupRoot = grid.AddCategory(_T("�����б�"));
	pGroupRoot->SetTooltip("�����б�");
	pGroupRoot->Expand();

	std::map<CString, CXTPPropertyGridItem*> mpParaGroup;				//!< ������
	std::list<std::shared_ptr<MVC::Device::CDeviceParam> > ltPara;	//!< ��Ҫ�ŵ��������еĲ���

	for (UINT id : m_ltParamID)
	{
		devParam = m_ShowDev->GetParam(id);
		if(!devParam)		continue;		// ��ǰ��return,����������ٸĻ���
		xmlPara = devParam->getXmlPara();
		ASSERT(xmlPara);

		if(xmlPara->m_strGroup == _T(""))	// ���û����,�ͷ��ڸ�����
		{
			AddItem(devParam, pGroupRoot);
		}
		else
		{
			mpParaGroup[xmlPara->m_strGroup] = NULL;
			ltPara.push_back(devParam);
		}
	}

	std::map<CString, CXTPPropertyGridItem*>::iterator iter;
	for(iter = mpParaGroup.begin(); iter != mpParaGroup.end(); ++iter)
	{
		iter->second = grid.AddCategory(iter->first);	// �����е��鶼����
		iter->second->SetTooltip(iter->first);
		iter->second->Expand();
	}

	for (auto devParam : ltPara)
	{
		xmlPara = devParam->getXmlPara();
		ASSERT(xmlPara);
		if(!mpParaGroup[xmlPara->m_strGroup])	ASSERT(FALSE);
		AddItem(devParam, mpParaGroup[xmlPara->m_strGroup]);
	}

// 			bool bExist = false;
// 			foreach(CXTPPropertyGridItem* pg, mpParaGroup)
// 			{
// 				if(pg->GetCaption() == xmlPara->m_strGroup)
// 				{
// 					pParaGroup = pg;
// 					bExist = true;
// 					break;
// 				}
// 			}
// 			if(!bExist)
// 			{
// 				pParaGroup = grid.AddCategory(xmlPara->m_strGroup);
// 				pParaGroup->SetTooltip(xmlPara->m_strGroup);
// 				pParaGroup->Expand();
// 				mpParaGroup[xmlPara->m_strGroup] = pParaGroup;
// 			}
// 		}
}

void CPropertyParam::AddItem(std::shared_ptr<CDeviceParam> devPara, CXTPPropertyGridItem* pParaGroup)
{
	CXTPPropertyGridItem* pItem;
	std::shared_ptr<XmlInfo::CXmlEnum> xmlEnum;
	std::shared_ptr<XmlInfo::CXmlEnumItem> xmlEnumItem;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	std::shared_ptr<XmlInfo::CXmlParaInfo> xmlPara = devPara->getXmlPara();
	std::list<CString> strList;
	CComVariant cvr = devPara->getValue(m_uiGroupIndex);
	ASSERT(xmlDev);
	ASSERT(xmlPara);

#ifdef _DEBUG
	//;
#endif

	if(xmlPara->m_uiEnumID != UINT(-1))
	{
		strList.clear();
		xmlEnum = xmlDev->getEnum(xmlPara->m_uiEnumID);
		ASSERT(xmlEnum);
		for (auto xmlEnumItem : xmlEnum->m_vtEnumItem)
			strList.push_back(xmlEnumItem->m_strShowText);
		ASSERT(strList.size() > 0);
		cvr.ChangeType(VT_I4);
		int index = xmlEnum->GetIndex(cvr);
		pItem = AddItemList(*pParaGroup, xmlPara->m_strName, xmlPara->m_strComment, strList, index, xmlPara->m_uiID + 1);
	}
	else if(xmlPara->m_uiValType == 10)
	{
		cvr.ChangeType(VT_I8);
		pItem = AddItemIP(*pParaGroup, xmlPara->m_strName, xmlPara->m_strComment, (UINT)cvr.llVal, xmlPara->m_uiID + 1);
	}
	else
	{
		pItem = AddItemVariant(*pParaGroup, xmlPara->m_strName, xmlPara->m_strComment, devPara->getValue(m_uiGroupIndex), xmlPara->m_uiID + 1);
	}
	if(xmlPara->m_uiAccessRight == 0 || m_bReadOnly)	pItem->SetReadOnly();		//!< 0ֻ��
	if(xmlPara->m_uiAccessRight == 3)					pItem->SetHidden(TRUE);		//!< 3����
}

//!< ����ʼ����ɺ�
void CPropertyParam::OnGridFirstShow(CXTPPropertyGrid& grid)
{
}

//!< Ҫ�����޸ĵ�����
bool CPropertyParam::OnSaveModify(CXTPPropertyGrid& grid)
{
	return true;
}

//!< ��ĳ���ֵ�����ı�ʱ
void CPropertyParam::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	std::shared_ptr<CDeviceParam> param = m_ShowDev->GetParam(ID - 1);
	ASSERT(param);

	std::shared_ptr<XmlInfo::CXmlParaInfo> xmlParam = param->getXmlPara();
	ASSERT(xmlParam);

	CXTPPropertyGridItem* item = grid.FindItem(ID);
	ASSERT(item);

	m_ShowDev->SetModify();
	CString strValue = item->GetValue();
	CComVariant cvr;
	if(xmlParam->m_uiEnumID != UINT(-1))
	{
		std::shared_ptr<XmlInfo::CXmlDevice> xmlDevice = m_ShowDev->GetXmlInfo();
		ASSERT(xmlDevice);
		std::shared_ptr<XmlInfo::CXmlEnum> xmlEnum = xmlDevice->getEnum(xmlParam->m_uiEnumID);
		ASSERT(xmlEnum);
		int nValue = xmlEnum->GetValue((int)item->GetConstraints()->GetCurrent());
		cvr = nValue;
		param->setValue(nValue, m_uiGroupIndex);
		if(xmlParam->m_nSpecialID == 1)			//!< �����޸Ĳ�����
		{
			CString text = xmlParam->m_strName;
			CString serialID;
			if(text.Find('1') != -1)	serialID = "1";		//!< ȷ�����޸����ĸ�����
			else						serialID = "2";
			CXTPPropertyGridItem* itemTime;
			int count = grid.GetCount();
			for(int id = 0; id < count; ++id)
			{
				itemTime = grid.GetItem(id);
				if(!itemTime)		continue;
				text = itemTime->GetCaption();
				CGbl::ChangeVariantType(cvr, VT_I8);
				LONGLONG llVal = cvr.llVal;
				if(text.Find("���ݰ����ʱ��") != -1 && text.Find(serialID) != -1)
				{
					if(nValue == 0)				((CXTPPropertyGridItemNumber*)itemTime)->SetNumber(32);
					else if(nValue == 1)		((CXTPPropertyGridItemNumber*)itemTime)->SetNumber(16);
					else if(nValue == 2)		((CXTPPropertyGridItemNumber*)itemTime)->SetNumber(8);
					else if(nValue == 3)		((CXTPPropertyGridItemNumber*)itemTime)->SetNumber(4);
					else						((CXTPPropertyGridItemNumber*)itemTime)->SetNumber(2);
					OnItemModify(grid, itemTime->GetID());
				}
			}
		}
	}
	else{
		switch(param->getType())
		{
		case 0:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_BOOL);	break;
		case 1:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_I1);	break;
		case 2:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_UI1);	break;
		case 3:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_I2);	break;
		case 4:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_UI2);	break;
		case 5:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_I4);	break;
		case 6:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_UI4);	break;
		case 7:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_R4);	break;
		case 8:		cvr = strValue;		CGbl::ChangeVariantType(cvr, VT_R8);	break;
		case 10:	cvr = CGbl::GetNumberFromIP(strValue);						break;
		default:	cvr = strValue;		break;
		}
		param->setValue(cvr, m_uiGroupIndex);
		cvr = param->getValue(m_uiGroupIndex);
		if(cvr.vt == VT_BOOL)	item->SetValue(cvr.boolVal ? _T("1") : _T("0"));
		else
		{
			cvr.ChangeType(VT_BSTR);
			if(param->getType() != 10)		//!< IP��ַ����ʾ�ַ���
				item->SetValue((CString)cvr.bstrVal);
		}
	}
}

//!< ��ʾ�����������
void CPropertyParam::ShowAndHide(CXTPPropertyGrid& grid)
{
}

//!< ���رձ��ʱ
void CPropertyParam::OnCloseGrid()
{
}

//!< ˢ�µ�ǰ��ʾ����������
void CPropertyParam::FreshAllData(CXTPPropertyGrid& grid)
{
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	std::shared_ptr<CDeviceParam> devParam;
	std::shared_ptr<XmlInfo::CXmlParaInfo> xmlPara;
	std::shared_ptr<XmlInfo::CXmlEnum> xmlEnum;
	std::shared_ptr<XmlInfo::CXmlEnumItem> xmlEnumItem;
	CComVariant cvr;
	for (UINT id : m_ltParamID)
	{
		CXTPPropertyGridItem* item = grid.FindItem(id + 1);
		if(!item)		continue;
		devParam = m_ShowDev->GetParam(id);
		ASSERT(devParam);
		cvr = devParam->getValue(m_uiGroupIndex);
		xmlPara = devParam->getXmlPara();
		ASSERT(xmlPara);

		if(xmlPara->m_uiEnumID != UINT(-1))
		{
			cvr.ChangeType(VT_I4);
			xmlEnum = xmlDev->getEnum(xmlPara->m_uiEnumID);
			ASSERT(xmlEnum);
			cvr.ChangeType(VT_I4);
			item->GetConstraints()->SetCurrent(xmlEnum->GetIndex(cvr));
			item->SetValue(item->GetConstraints()->GetAt(xmlEnum->GetIndex(cvr)));
		}
		else if(xmlPara->m_uiValType == 10)
		{
			cvr.ChangeType(VT_I8);
			item->SetValue(CGbl::GetIPFromNumber((UINT)cvr.llVal));
		}
		else if(cvr.vt == VT_BOOL)
		{
			item->SetValue(cvr.boolVal ? _T("1") : _T("0"));
		}
		else
		{
			cvr.ChangeType(VT_BSTR);
			item->SetValue((CString)cvr.bstrVal);
		}
	}
}

//!< ��ʾ������Ϣ
void CPropertyParam::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("�豸����������"));
}