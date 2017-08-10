#pragma once

namespace MVC{namespace Device{namespace InterfaceSet{
class CDSerial
{
private:
	UINT m_uiNumber;			//!< 串口代号，0代表COM1，1代表COM2，依此类推
	UINT m_uiBaud;				//!< 波特率索引，单位bps
	UINT m_uiCheckStyle;		//!< 检验类型索引，0-无校验，1-奇校验，2-偶校验，3-空格校验，4-符号校验
	UINT m_uiDataBitNum;		//!< 数据位索引
	UINT m_uiStopBitNum;		//!< 停止位索引，0-1，1-2
	UINT m_uiRetryNum;			//!< 重试次数
	UINT m_uiOverTime;			//!< 通信超时时间
	UINT m_uiCoolTime;			//!< 冷却时间
	UINT m_uiReviveTime;		//!< 通信恢复时间

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

	CString getName();			//!< 获得串口名
	bool SerializeXml(TiXmlElement* pNode, bool bRead);
	CDSerial& operator = (CDSerial& serial);
	void CopyFrom(CDSerial& serial);
};
}}}