#pragma once

namespace MVC{
namespace Item{
class CItem;
class CPropertyAlarm
{
private:
	CItem* m_pParent;			//!< 所属变量的ID号
	UINT m_uiBitAlarmType;		//!< 开关报警类型，0不报警，1开时报警，2关时报警，3开到关报警，4关到开报警，5变化就报警
	UINT m_uiDeadArea;			//!< 死区，开始报警和恢复报警之间的界限，为了放置报警波动。真正的报警值和恢复值 = 报警值 ± 死区值
	BOOL m_bLoloActive;			//!< 下下限报警是否启动
	float m_LoloValue;			//!< 下下限报警值
	BOOL m_bLowActive;			//!< 下限报警是否启动
	float m_LowValue;			//!< 下限报警值
	BOOL m_bHighActive;			//!< 上限报警是否启动
	float m_HighValue;			//!< 上限报警值
	BOOL m_bHihiActive;			//!< 上上限报警是否启动
	float m_HihiValue;			//!< 上上限报警值
	BOOL m_bAimActive;			//!< 目标报警是否启动
	float m_AimPercent;			//!< 目标偏差百分比
	float m_AimDeadPercent;		//!< 目标偏差死区百分比
	float m_AimValue;			//!< 目标值
	BOOL m_bShiftActive;		//!< 变化率报警是否启动
	float m_ShiftDelta;			//!< 最大变化量
	UINT m_uiShiftTime;			//!< 变化时间，单位秒

public:
	void setBitAlarmType(UINT type){m_uiBitAlarmType=type;}
	void setDeadArea(int da){if(da < 0)		da = abs(da);	m_uiDeadArea=da;}
	void setLoloActive(BOOL active){m_bLoloActive = active;}
	void setLoloValue(float val){m_LoloValue = val;}
	void setLowActive(BOOL active){m_bLowActive = active;}
	void setLowValue(float val){m_LowValue = val;}
	void setHighActive(BOOL active){m_bHighActive = active;}
	void setHighValue(float val){m_HighValue = val;}
	void setHihiActive(BOOL active){m_bHihiActive = active;}
	void setHihiValue(float val){m_HihiValue = val;}
	void setAimActive(BOOL active){m_bAimActive = active;}
	void setAimValue(float val){m_AimValue = val;}
	void setAimPercent(float percent){m_AimPercent = percent<0?abs(percent):percent;}
	void setAimDeadPercent(float dead){m_AimDeadPercent = dead<0?abs(dead):dead;}
	void setShiftActive(BOOL active){m_bShiftActive = active;}
	void setShiftDelta(float percent){m_ShiftDelta = percent;}
	void setShiftTime(UINT num){m_uiShiftTime = num;}

	CItem* getItem(){return m_pParent;}
	UINT getBitAlarmType(){return m_uiBitAlarmType;}
	UINT getDeadArea(){return m_uiDeadArea;}
	BOOL getLoloActive(){return m_bLoloActive;}
	float getLoloValue(){return m_LoloValue;}
	BOOL getLowActive(){return m_bLowActive;}
	float getLowValue(){return m_LowValue;}
	BOOL getHighActive(){return m_bHighActive;}
	float getHighValue(){return m_HighValue;}
	BOOL getHihiActive(){return m_bHihiActive;}
	float getHihiValue(){return m_HihiValue;}
	BOOL getAimActive(){return m_bAimActive;}
	float getAimValue(){return m_AimValue;}
	float getAimPercent(){return m_AimPercent;}
	float getAimDeadPercent(){return m_AimDeadPercent;}
	BOOL getShiftActive(){return m_bShiftActive;}
	float getShiftDelta(){return m_ShiftDelta;}
	UINT getShiftTime(){return m_uiShiftTime;}

public:
	CPropertyAlarm(CItem* item);
	~CPropertyAlarm(void);

	bool Serialize(CArchive& ar);							//!< 保存二进制信息
	bool SerializeXml(TiXmlElement* pNode, bool bRead);		//!< 保存xml格式的编辑信息
	void ReadFromDViewExcel(std::vector<CString>& vtCell);	//!< 解析数据源属性
	CString GetBitAlarmInfo(bool bShowNone = true);			//!< 获得当前位报警的字符串信息,参数表示当处于不报警时是否返回不报警字符串

	CPropertyAlarm& operator = (CPropertyAlarm& alarm);
	bool operator == (CPropertyAlarm& alarm);
};
}
}