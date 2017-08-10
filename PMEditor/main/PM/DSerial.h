#pragma once

namespace MVC{namespace Device{namespace InterfaceSet{
class CDSerial
{
private:
	UINT m_uiNumber;			//!< ���ڴ��ţ�0����COM1��1����COM2����������
	UINT m_uiBaud;				//!< ��������������λbps
	UINT m_uiCheckStyle;		//!< ��������������0-��У�飬1-��У�飬2-żУ�飬3-�ո�У�飬4-����У��
	UINT m_uiDataBitNum;		//!< ����λ����
	UINT m_uiStopBitNum;		//!< ֹͣλ������0-1��1-2
	UINT m_uiRetryNum;			//!< ���Դ���
	UINT m_uiOverTime;			//!< ͨ�ų�ʱʱ��
	UINT m_uiCoolTime;			//!< ��ȴʱ��
	UINT m_uiReviveTime;		//!< ͨ�Żָ�ʱ��

public:
	UINT getNumber(){return m_uiNumber;}
	UINT getBaud(){return m_uiBaud;}
	UINT getCheckStyle(){return m_uiCheckStyle;}
	UINT getDataBitNum(){return m_uiDataBitNum;}
	UINT getStopBitNum(){return m_uiStopBitNum;}
	UINT getRetryNum(){return m_uiRetryNum;}
	UINT getOverTime(){return m_uiOverTime;}
	UINT getCoolTime(){return m_uiCoolTime;}
	UINT getReviveTime(){return m_uiReviveTime;}

	void setNumber(UINT no){m_uiNumber = no;}
	void setBaud(UINT baud){m_uiBaud = baud;}
	void setCheckStyle(UINT style){m_uiCheckStyle = style;}
	void setDataBitNum(UINT num){m_uiDataBitNum = num;}
	void setStopBitNum(UINT num){m_uiStopBitNum = num;}
	void setRetryNum(UINT num){m_uiRetryNum = num;}
	void setOverTime(UINT t){m_uiOverTime = t;}
	void setCoolTime(UINT t){m_uiCoolTime = t;}
	void setReviveTime(UINT t){m_uiReviveTime = t;}

public:
	explicit CDSerial();
	~CDSerial(void);

	CString getName();			//!< ��ô�����
	bool SerializeXml(TiXmlElement* pNode, bool bRead);
	CDSerial& operator = (CDSerial& serial);
	void CopyFrom(CDSerial& serial);
};
}}}