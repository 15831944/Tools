#pragma once

namespace Comm{
class ICommListener;
class /*RWD_API*/ CData
{
public:
	bool m_bNeedRet;			//!< ��Ҫ����
	byte* m_byData;				//!< ����
	int m_nLen;					//!< ���ݳ���
	int m_nTime;				//!< ��ʱʱ�䣬��λ�ٺ���
	ICommListener* m_nListener;	//!< ������
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