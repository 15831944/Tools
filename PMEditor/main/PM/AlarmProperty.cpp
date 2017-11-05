#include "stdafx.h"
#include "PMApp.h"
#include "Item.h"
#include "AlarmProperty.h"

using namespace MVC;
using namespace Item;

const CString ALARM_TYPE = _T("AlarmType");
const CString ALARM_DEADVALUE = _T("DeadValue");
const CString ALARM_LOLOACTIVE = _T("LoloActive");
const CString ALARM_LOLOVALUE = _T("LoloValue");
const CString ALARM_LOWACTIVE = _T("LowActive");
const CString ALARM_LOWVALUE = _T("LowValue");
const CString ALARM_HIGHACTIVE = _T("HighActive");
const CString ALARM_HIGHVALUE = _T("HighValue");
const CString ALARM_HIHIACTIVE = _T("HihiActive");
const CString ALARM_HIHIVALUE = _T("HihiValue");
const CString ALARM_AIMACTIVE = _T("AimActive");
const CString ALARM_AIMPERCENT = _T("AimPercent");
const CString ALARM_AIMDEADAREA = _T("AimDeadArea");
const CString ALARM_AIMVALUE = _T("AimValue");
const CString ALARM_SHIFTACTIVE = _T("ShiftActive");
const CString ALARM_SHIFTDELTA = _T("ShiftPercent");
const CString ALARM_SHIFTUINTTYPE = _T("ShiftUnitType");
const CString ALARM_SHIFTVALUE = _T("ShiftUnitNum");

CPropertyAlarm::CPropertyAlarm(CItem* item)
{
	m_pParent=item;

	m_uiBitAlarmType = 0;		// 开关报警类型，0不报警，1开时报警，2关时报警，3开到关报警，4关到开报警，5变化就报警
	m_uiDeadArea = 0;			// 死区，开始报警和恢复报警之间的界限，为了放置报警波动。真正的报警值和恢复值 = 报警值 ± 死区值
	m_bLoloActive = FALSE;		// 下下限报警是否启动
	m_bLowActive = FALSE;		// 下限报警是否启动
	m_bHighActive = FALSE;		// 上限报警是否启动
	m_bHihiActive = FALSE;		// 上上限报警是否启动
	m_bAimActive = FALSE;		// 目标报警是否启动
	m_bShiftActive = FALSE;		// 变化率报警是否启动
	m_uiShiftTime = 10;			// 变化率报警的取值时间，单位秒

	m_LoloValue = m_LowValue = m_HighValue = m_HihiValue = m_AimValue = m_AimPercent = m_AimDeadPercent = m_ShiftDelta = 0.0;
}

CPropertyAlarm::~CPropertyAlarm(void)
{
}

bool CPropertyAlarm::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		int iValue;
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;

			if(name == ALARM_TYPE)				{	cv.ChangeType(VT_I4);	iValue = cv.intVal;	m_uiBitAlarmType = UINT(iValue);}
			else if(name == ALARM_DEADVALUE)	{	cv.ChangeType(VT_I4);	iValue = cv.intVal;	m_uiDeadArea = (iValue < 0)?abs(iValue):iValue;}
			else if(name == ALARM_LOLOACTIVE)	{	cv.ChangeType(VT_BOOL);	m_bLoloActive = cv.boolVal;}
			else if(name == ALARM_LOLOVALUE)	{	cv.ChangeType(VT_R4);	m_LoloValue = cv.fltVal;}
			else if(name == ALARM_LOWACTIVE)	{	cv.ChangeType(VT_BOOL);	m_bLowActive = cv.boolVal;}
			else if(name == ALARM_LOWVALUE)		{	cv.ChangeType(VT_R4);	m_LowValue = cv.fltVal;}
			else if(name == ALARM_HIGHACTIVE)	{	cv.ChangeType(VT_BOOL);	m_bHighActive = cv.boolVal;}
			else if(name == ALARM_HIGHVALUE)	{	cv.ChangeType(VT_R4);	m_HighValue= cv.fltVal;}
			else if(name == ALARM_HIHIACTIVE)	{	cv.ChangeType(VT_BOOL);	m_bHihiActive = cv.boolVal;}
			else if(name == ALARM_HIHIVALUE)	{	cv.ChangeType(VT_R4);	m_HihiValue = cv.fltVal;}
			else if(name == ALARM_AIMACTIVE)	{	cv.ChangeType(VT_BOOL);	m_bAimActive = cv.boolVal;}
			else if(name == ALARM_AIMPERCENT)	{	cv.ChangeType(VT_R4);	m_AimPercent = cv.fltVal < 0 ? abs(cv.fltVal) : cv.fltVal;}
			else if(name == ALARM_AIMDEADAREA)	{	cv.ChangeType(VT_R4);	m_AimDeadPercent = cv.fltVal < 0 ? abs(cv.fltVal) : cv.fltVal;}
			else if(name == ALARM_AIMVALUE)		{	cv.ChangeType(VT_R4);	m_AimValue = cv.fltVal;}
			else if(name == ALARM_SHIFTACTIVE)	{	cv.ChangeType(VT_BOOL);	m_bShiftActive = cv.boolVal;}
			else if(name == ALARM_SHIFTDELTA)	{	cv.ChangeType(VT_R4);	m_ShiftDelta = cv.fltVal;}
			else if(name == ALARM_SHIFTVALUE)	{	cv.ChangeType(VT_I4);	iValue = cv.intVal;	m_uiShiftTime = UINT(iValue);}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(ALARM_TYPE, getBitAlarmType());
		pNode->SetAttribute(ALARM_DEADVALUE, getDeadArea());
		pNode->SetAttribute(ALARM_LOLOACTIVE, getLoloActive()?1:0);
		pNode->SetDoubleAttribute(ALARM_LOLOVALUE, m_LoloValue);
		pNode->SetAttribute(ALARM_LOWACTIVE, getLowActive()?1:0);
		pNode->SetDoubleAttribute(ALARM_LOWVALUE, m_LowValue);
		pNode->SetAttribute(ALARM_HIGHACTIVE, getHighActive()?1:0);
		pNode->SetDoubleAttribute(ALARM_HIGHVALUE, m_HighValue);
		pNode->SetAttribute(ALARM_HIHIACTIVE, getHihiActive()?1:0);
		pNode->SetDoubleAttribute(ALARM_HIHIVALUE, m_HihiValue);
		pNode->SetAttribute(ALARM_AIMACTIVE, getAimActive()?1:0);
		pNode->SetDoubleAttribute(ALARM_AIMPERCENT, m_AimPercent);
		pNode->SetDoubleAttribute(ALARM_AIMDEADAREA, m_AimDeadPercent);
		pNode->SetDoubleAttribute(ALARM_AIMVALUE, m_AimValue);
		pNode->SetAttribute(ALARM_SHIFTACTIVE, getShiftActive()?1:0);
		pNode->SetDoubleAttribute(ALARM_SHIFTDELTA, m_ShiftDelta);
		pNode->SetAttribute(ALARM_SHIFTUINTTYPE, 0);
		pNode->SetAttribute(ALARM_SHIFTVALUE, getShiftTime());
	}
	return true;
}

// 解析数据源属性
void CPropertyAlarm::ReadFromPMExcel(std::vector<CString>& vtCell)
{
	m_uiBitAlarmType = (UINT)atoi(vtCell[24].Trim());
	m_uiDeadArea = (UINT)atoi(vtCell[25].Trim());
	m_bLoloActive = vtCell[26].Trim() == "1";
	m_LoloValue = atof(vtCell[27].Trim());
	m_bLowActive = vtCell[28].Trim() == "1";
	m_LowValue = atof(vtCell[29].Trim());
	m_bHighActive = vtCell[30].Trim() == "1";
	m_HighValue = atof(vtCell[31].Trim());
	m_bHihiActive = vtCell[32].Trim() == "1";
	m_HihiValue = atof(vtCell[33].Trim());
	m_bAimActive = vtCell[34].Trim() == "1";
	m_AimValue = atof(vtCell[35].Trim());
	m_AimPercent = atof(vtCell[36].Trim());
	m_AimDeadPercent = atof(vtCell[37].Trim());
	m_bShiftActive = vtCell[38].Trim() == "1";
	m_ShiftDelta = atof(vtCell[39].Trim());
	m_uiShiftTime = (UINT)atoi(vtCell[40].Trim());
}

CPropertyAlarm& CPropertyAlarm::operator = (CPropertyAlarm& alarm)
{
	m_uiBitAlarmType = alarm.getBitAlarmType();
	m_uiDeadArea = alarm.getDeadArea();
	m_bLoloActive = alarm.getLoloActive();
	m_LoloValue = alarm.getLoloValue();
	m_bLowActive = alarm.getLowActive();
	m_LowValue = alarm.getLowValue();
	m_bHighActive = alarm.getHighActive();
	m_HighValue = alarm.getHighValue();
	m_bHihiActive = alarm.getHihiActive();
	m_HihiValue = alarm.getHihiValue();
	m_bAimActive = alarm.getAimActive();
	m_AimValue = alarm.getAimValue();
	m_AimPercent = alarm.getAimPercent();
	m_AimDeadPercent = alarm.getAimDeadPercent();
	m_bShiftActive = alarm.getShiftActive();
	m_ShiftDelta = alarm.getShiftDelta();
	m_uiShiftTime = alarm.getShiftTime();
	return *this;
}

bool CPropertyAlarm::operator == (CPropertyAlarm& alarm)
{
	if(m_uiBitAlarmType != alarm.getBitAlarmType())			return false;
	if(m_uiDeadArea != alarm.getDeadArea())					return false;
	if(m_bLoloActive != alarm.getLoloActive())				return false;
	if(m_LoloValue != alarm.getLoloValue())					return false;
	if(m_bLowActive != alarm.getLowActive())				return false;
	if(m_LowValue != alarm.getLowValue())					return false;
	if(m_bHighActive != alarm.getHighActive())				return false;
	if(m_HighValue != alarm.getHighValue())					return false;
	if(m_bHihiActive != alarm.getHihiActive())				return false;
	if(m_HihiValue != alarm.getHihiValue())					return false;
	if(m_bAimActive != alarm.getAimActive())				return false;
	if(m_AimValue != alarm.getAimValue())					return false;
	if(m_AimPercent != alarm.getAimPercent())				return false;
	if(m_AimDeadPercent != alarm.getAimDeadPercent())		return false;
	if(m_bShiftActive != alarm.getShiftActive())			return false;
	if(m_ShiftDelta != alarm.getShiftDelta())				return false;
	if(m_uiShiftTime != alarm.getShiftTime())				return false;
	return true;
}

// 获得当前位报警的字符串信息,参数表示当处于不报警时是否返回不报警字符串
CString CPropertyAlarm::GetBitAlarmInfo(bool bShowNone /* = true */)
{
	switch(m_uiBitAlarmType)
	{
	case 0:		return bShowNone ? _T("不报警") : _T("");	break;
	case 1:		return _T("开时报警");		break;
	case 2:		return _T("关时报警");		break;
	case 3:		return _T("开到关报警");	break;
	case 4:		return _T("关到开报警");	break;
	case 5:		return _T("变化就报警");	break;
	}
	return bShowNone ? _T("不报警") : _T("");
}