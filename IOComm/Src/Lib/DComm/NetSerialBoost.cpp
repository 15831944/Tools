#include "Stdafx.h"
#include "Gbl.h"
#include "SerialPortBoost.h"
#include "NetSerialBoost.h"
#include "Data.h"

const CString VT_COM_BOOST[4] = {"COM1", "COM2", "COM3", "COM4"};
const CString VT_BAUD_BOOST[10] = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200", "128000", "256000"};
const UINT VT_BAUD_VAL_BOOST[10] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 128000, 256000};
const CString VT_CHECK_BOOST[5] = {"无校验", "奇校验", "偶校验", "0校验", "1校验"};
const char VT_CHECK_BOOST1[5] = {'N', 'O', 'E', 'S', 'M'};
const UINT VT_PARITY_BOOST[5] = {0, 1, 2, 3, 4};
const CString VT_DATA_BOOST[2] = {"8", "7"};
const UINT VT_DATA_VAL_BOOST[2] = {8, 7};
const CString VT_STOP_BOOST[2] = {"1", "2"};
const UINT VT_STOP_VAL_BOOST[2] = {1, 2};

using namespace Comm;
CNetSerialBoost::CNetSerialBoost(void)
:m_bBusy(false)		//!< 是否正处于通信中
,m_nComId(-1)		//!< COM口号，0表示COM1，依此类推
,m_nBaud(0)			//!< 波特率，0表示1200，9表示256000
,m_nCheck(0)		//!< 校验方式
,m_nDataBit(0)		//!< 数据位
,m_nStopBit(0)		//!< 停止位
,m_SendData(NULL)
{
}

CNetSerialBoost::~CNetSerialBoost(void)
{
	KillDTimer();
	m_ltListener.clear();
	foreach(CData* cd, m_dqSendData)		delete cd;
	m_dqSendData.clear();
}

//!< 发送数据
bool CNetSerialBoost::Send(bool needRet,	byte* data, int nLen, int nTime, ICommListener* pListener)
{
	CData* send_data = new CData(nLen);
	send_data->m_bNeedRet = needRet;
	send_data->m_nLen = nLen;
	send_data->m_nTime = nTime;
	send_data->m_nListener = pListener;
	memcpy(send_data->m_byData, data, nLen);
	if(IsBusy()){
		m_dqSendData.push_back(send_data);
	}
	else{
		m_dqSendData.push_back(send_data);
		OnSend();
	}
	return true;
}

void CNetSerialBoost::OnSend()
{
	if(m_dqSendData.empty())	return;
	if(m_SendData)				delete m_SendData;
	m_SendData = m_dqSendData.front();
	m_dqSendData.pop_front();
	CString str,text;
	for(int i = 0; i < m_SendData->m_nLen; ++i)
	{
		text.Format("%.2X ", m_SendData->m_byData[i]);
		str += text;
	}
	str += _T("\r\n");
	CGbl::DebugMsg(str, CGbl::REPORT_WATERBLUE, true);
	SetBusy();
	m_SerialPortBoost.SendSerialData(m_SendData->m_byData, m_SendData->m_nLen);
	SetDTimer(m_SendData->m_nTime, 0, true);
}

//!< 开始通信
bool CNetSerialBoost::StartCom()
{
//	if(!m_SerialPort)	m_SerialPort = boost::shared_ptr<CSerialPort>(new CSerialPort);
	return (bool)m_SerialPortBoost.SetStart(true);
}

//!< 停止通信
bool CNetSerialBoost::StopCom()
{
	/*if(m_SerialPort)	*/m_SerialPortBoost.SetStart(false);
	m_strInfo = _T("");
	return true;
}

//!< 设置信息，例如“COM1:115200:无校验:8:1”
bool CNetSerialBoost::SetInfo(CString info)
{
	if(info == GetInfo())	return true;
	std::vector<CString> vtStr;
	CGbl::SpliteBy(info, ":", vtStr);
	if(vtStr.size() != 5)	return false;
	int i, nComId = -1, nBaud = -1, nCheck = -1, nDataBit = -1, nStopBit = -1;	//!< 临时信息
	for(i = 0; i < 4; ++i)	if(vtStr[0] == VT_COM_BOOST[i]){nComId = i;break;}		//!< 串口号
	for(i = 0; i < 10; ++i)	if(vtStr[1] == VT_BAUD_BOOST[i]){nBaud = i;break;}		//!< 波特率
	for(i = 0; i < 5; ++i)	if(vtStr[2] == VT_CHECK_BOOST[i] || vtStr[2] == (CString)VT_CHECK_BOOST1[i]){nCheck = i;break;}		//!< 校验方式
	for(i = 0; i < 2; ++i)	if(vtStr[3] == VT_DATA_BOOST[i]){nDataBit = i;break;}		//!< 数据位
	for(i = 0; i < 2; ++i)	if(vtStr[4] == VT_STOP_BOOST[i]){nStopBit = i;break;}		//!< 停止位
	if(nComId == -1 || nBaud == -1 || nCheck == -1 || nDataBit == -1 || nStopBit == -1)		return false;

	m_nComId = nComId;			//!< COM口号，0表示COM1，依此类推
	m_nBaud = nBaud;			//!< 波特率，0表示1200，9表示256000
	m_nCheck = nCheck;			//!< 校验方式
	m_nDataBit = nDataBit;		//!< 数据位
	m_nStopBit = nStopBit;		//!< 停止位

	if (m_SerialPortBoost.IsStart()){
		m_SerialPortBoost.SetStart(false);
	}
	if(!m_SerialPortBoost.InitPort(this, m_nComId + 1, VT_BAUD_VAL_BOOST[m_nBaud], VT_PARITY_BOOST[m_nCheck],
		VT_DATA_VAL_BOOST[m_nDataBit], VT_STOP_VAL_BOOST[m_nStopBit]))
	{	m_strInfo = _T("");	return false;}

	return IComm::SetInfo(info);
}

//!< 监听数据
bool CNetSerialBoost::WatchData(ICommListener* listener)
{
	if(!listener)		return false;
	m_ltListener.remove(listener);
	m_ltListener.push_back(listener);
	return true;
}

//!< 取消监听
void CNetSerialBoost::DisWatch(ICommListener* listener)
{
	m_ltListener.remove(listener);
}

//!< 收到数据
void CNetSerialBoost::OnReceive(byte* data, int len)
{
	CGbl::DebugMsg("收到报文 <-- ", data, len, CGbl::REPORT_GREEN, true);
	if(m_SendData && m_SendData->m_bNeedRet)
		m_SendData->m_nListener->OnReceive(this, data, len);
	ICommListener* listener;
	foreach(listener, m_ltListener)
	{
		if(m_SendData)
		{
			if(listener != m_SendData->m_nListener || !m_SendData->m_bNeedRet)	//!< 如果已经回过了就不用再回了
				listener->OnReceive(this, data, len);
		}
		else	listener->OnReceive(this, data, len);
	}
	KillDTimer();
	EmptyData();
}

//!< 时间到了，nMilSec表示距离上一次的时间报告，经过了多少个毫秒
void CNetSerialBoost::OnDTimer(int nID)
{
	if(nID == 0){
		if(!m_SendData)		return;
		m_SendData->m_nListener->OnOverTime(this);
		EmptyData();
	}
}

void CNetSerialBoost::EmptyData()
{
	if(m_SendData)			delete m_SendData;
	m_SendData = NULL;
	SetBusy(false);
	OnSend();
}
