#include "Stdafx.h"
#include "Gbl.h"
#include "Comm.h"
#include "SerialPortBoost.h"

using namespace Comm;
void ListenThread(LPVOID pParam);

CSerialPortBoost::CSerialPortBoost(void)
:m_bActive(false)
,m_bStart(false)
,m_nID(0)
,m_nBaud(9600)
,m_nParity(0)
,m_nDataBit(8)
,m_nStopBit(1)
{
	m_ioSev = boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service());
}

CSerialPortBoost::~CSerialPortBoost(void)
{
}

bool CSerialPortBoost::InitPort(CWnd* pPortOwner,
								UINT portnr/* = 1 */,
								UINT baud/* = 19200 */,
								UINT parity/* = '0' */,
								UINT databits/* = 8 */,
								UINT stopbits/* = 1 */)
{
	m_pOwner = pPortOwner;
	m_bActive = true;
	m_nID = portnr;
	m_nBaud = baud;
	m_nParity = parity;
	m_nDataBit = databits;
	m_nStopBit = stopbits;
	return true;
}

bool CSerialPortBoost::SetStart(bool bStart)
{
	if (!m_bActive)			return false;
	if (bStart && !m_bStart)
	{
		m_bStart = true;
		m_pThread.reset();
		m_pPort.reset();

		// 串口COM1, Linux下为“/dev/ttyS0”
		try
		{
			m_pPort = boost::shared_ptr<boost::asio::serial_port>(new boost::asio::serial_port(*m_ioSev, GetStrID()));

			// 设置参数
			m_pPort->set_option(boost::asio::serial_port::baud_rate(m_nBaud));
			m_pPort->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::software));
			m_pPort->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::type(m_nParity)));
			m_pPort->set_option(boost::asio::serial_port::character_size(m_nDataBit));
			if (m_nStopBit == 1)
				m_pPort->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
			else
				m_pPort->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::two));

			m_pThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::BOOST_BIND(&ListenThread, this)));
			m_ioSev->run(); 
		}
		catch(...)
		{
			m_bStart = false;
			if (m_pThread){
				m_pThread->join();
				m_pThread.reset();
			}
			if (m_pPort){
				m_pPort->cancel();
				m_pPort->close();
				m_pPort.reset();
			}
			m_ioSev->stop();
			return false;
		}
	}
	else if (!bStart && m_bStart)
	{
		m_bStart = false;
		try
		{
			if (m_pPort){
				m_pPort->cancel();
			}
			m_pThread->join();
		}
		catch (...)
		{
		}
		m_pThread.reset();
		if (m_pPort)
		{
			//m_pPort->cancel();
			m_pPort->close();
			m_pPort.reset();
		}
		m_ioSev->stop();
	}
	return true;
}

void OnRev(CSerialPortBoost *pPort, char *buf, int len)
{
	if (!pPort->IsStart()){
		return ;
	}
	if (len > 0){
		::SendMessage(pPort->m_pOwner->m_hWnd, WM_COMM_RXCHAR, (WPARAM)buf, (LPARAM)len);
	}
}

void ListenThread(LPVOID pParam)
{
	CSerialPortBoost *pPort = (CSerialPortBoost *)pParam;
	if (!pPort)		return;
	char reply[256] = {0};
	while (pPort->IsStart()){
		try
		{
			size_t len = pPort->m_pPort->read_some<boost::asio::mutable_buffers_1>(boost::asio::buffer(reply, 256));
			if (!pPort->IsStart()) break;
			if (len > 0)	OnRev(pPort, reply, len);
			Sleep(1);
		}
		catch (...)
		{
			break;
		}
	}
}

char CSerialPortBoost::ParityChar()
{
	char VT_PARITY[5] = {'N', 'O', 'E', 'S', 'M'};
	if (m_nParity >= 0 && m_nParity <= 5)
		return VT_PARITY[m_nParity];
	return 0;
}

void CSerialPortBoost::SendSerialData(byte *pData, int len)
{
	if (len <= 0)		return;
	if (!m_pPort)		return;
	boost::asio::write(*m_pPort, boost::asio::buffer((char *)pData, len));
}

CString CSerialPortBoost::GetStrID()
{
	CString strID;
	strID.Format("Com%d", m_nID);
	return strID;
}

int CSerialPortBoost::GetBaudIdx()
{
	if (m_nBaud == 1200)		return 0;
	else if (m_nBaud == 2400)	return 1;
	else if (m_nBaud == 4800)	return 2;
	else if (m_nBaud == 9600)	return 3;
	else if (m_nBaud == 19200)	return 4;
	else if (m_nBaud == 38400)	return 5;
	else if (m_nBaud == 57600)	return 6;
	else if (m_nBaud == 115200)	return 7;
	return -1;
}

void CSerialPortBoost::SetBaudFromIdx(int idx)
{
	if (idx == 0)				m_nBaud = 1200;
	else if (idx == 1)			m_nBaud = 2400;
	else if (idx == 2)			m_nBaud = 4800;
	else if (idx == 3)			m_nBaud = 9600;
	else if (idx == 4)			m_nBaud = 19200;
	else if (idx == 5)			m_nBaud = 38400;
	else if (idx == 6)			m_nBaud = 57600;
	else if (idx == 7)			m_nBaud = 115200;
}

int CSerialPortBoost::GetStopIdx()
{
	return m_nStopBit - 1;
}

void CSerialPortBoost::SetStopFromIdx(int idx)
{
	m_nStopBit = idx + 1;
	if (m_nStopBit != 1 && m_nStopBit != 2)
		m_nStopBit = 1;
}

int CSerialPortBoost::GetDataIdx()
{
	return abs(m_nDataBit - 8);
}

void CSerialPortBoost::SetDataFromIdx(int idx)
{
	m_nDataBit = abs(idx - 8);
	if (m_nDataBit != 8 && m_nDataBit != 7)
		m_nDataBit = 8;
}
