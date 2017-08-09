#pragma once

namespace Comm{
class ICommListener;
class /*RWD_API*/ CData
{
public:
	bool m_bNeedRet;			//!< 需要返回
	byte* m_byData;				//!< 数据
	int m_nLen;					//!< 数据长度
	int m_nTime;				//!< 超时时间，单位百毫秒
	ICommListener* m_nListener;	//!< 接收者
	CData(int nLen):m_bNeedRet(false),m_nLen(nLen),m_nTime(-1),m_nListener(NULL){if(m_nLen > 0)	m_byData = new byte[m_nLen];}
	~CData(){if(m_nLen > 0 && m_byData != NULL)	{delete[] m_byData; m_byData = NULL;}}
	void Init(bool bNeedRet, int nTime, ICommListener& listener, byte* data)
	{
		m_bNeedRet = bNeedRet;
		m_nTime = nTime;
		m_nListener = &listener;
		if(m_nLen > 0)	memcpy(m_byData, data, m_nLen);
	}
};
}