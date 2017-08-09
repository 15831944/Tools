#pragma once

namespace MVC{
namespace Item{
class CItem;
class CPropertyAlarm
{
private:
	CItem* m_pParent;			//!< ����������ID��
	UINT m_uiBitAlarmType;		//!< ���ر������ͣ�0��������1��ʱ������2��ʱ������3�����ر�����4�ص���������5�仯�ͱ���
	UINT m_uiDeadArea;			//!< ��������ʼ�����ͻָ�����֮��Ľ��ޣ�Ϊ�˷��ñ��������������ı���ֵ�ͻָ�ֵ = ����ֵ �� ����ֵ
	BOOL m_bLoloActive;			//!< �����ޱ����Ƿ�����
	float m_LoloValue;			//!< �����ޱ���ֵ
	BOOL m_bLowActive;			//!< ���ޱ����Ƿ�����
	float m_LowValue;			//!< ���ޱ���ֵ
	BOOL m_bHighActive;			//!< ���ޱ����Ƿ�����
	float m_HighValue;			//!< ���ޱ���ֵ
	BOOL m_bHihiActive;			//!< �����ޱ����Ƿ�����
	float m_HihiValue;			//!< �����ޱ���ֵ
	BOOL m_bAimActive;			//!< Ŀ�걨���Ƿ�����
	float m_AimPercent;			//!< Ŀ��ƫ��ٷֱ�
	float m_AimDeadPercent;		//!< Ŀ��ƫ�������ٷֱ�
	float m_AimValue;			//!< Ŀ��ֵ
	BOOL m_bShiftActive;		//!< �仯�ʱ����Ƿ�����
	float m_ShiftDelta;			//!< ���仯��
	UINT m_uiShiftTime;			//!< �仯ʱ�䣬��λ��

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

	bool Serialize(CArchive& ar);							//!< �����������Ϣ
	bool SerializeXml(TiXmlElement* pNode, bool bRead);		//!< ����xml��ʽ�ı༭��Ϣ
	void ReadFromDViewExcel(std::vector<CString>& vtCell);	//!< ��������Դ����
	CString GetBitAlarmInfo(bool bShowNone = true);			//!< ��õ�ǰλ�������ַ�����Ϣ,������ʾ�����ڲ�����ʱ�Ƿ񷵻ز������ַ���

	CPropertyAlarm& operator = (CPropertyAlarm& alarm);
	bool operator == (CPropertyAlarm& alarm);
};
}
}