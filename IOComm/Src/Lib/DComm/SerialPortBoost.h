#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace Comm{
class CSerialPortBoost
{
public:
	boost::shared_ptr<boost::asio::io_service> m_ioSev;
	boost::shared_ptr<boost::asio::serial_port> m_pPort;
	boost::shared_ptr<boost::thread> m_pThread;
	CWnd *m_pOwner;

private:
	//CVirtualDevSerialDlg *m_pDlg;
	bool m_bActive;
	bool m_bStart;
	int m_nID;
	int m_nBaud;
	int m_nParity;
	int m_nDataBit;
	int m_nStopBit;

public:
	void SetComID(int id){m_nID = id;}
	void SetActive(bool b){m_bActive = b;}
	void SetBaud(int nBaud){m_nBaud = nBaud;}
	void SetParity(int nParity){m_nParity = nParity;}
	void SetDataBit(int nDataBit){m_nDataBit = nDataBit;}
	void SetStopBit(int nStopBit){m_nStopBit = nStopBit;}

	//CVirtualDevSerialDlg *GetDlg(){return m_pDlg;}
	bool IsAcitve(){return m_bActive;}
	bool IsStart(){return m_bStart;}
	int GetComID(){return m_nID;}
	int GetBaud(){return m_nBaud;}
	int GetParity(){return m_nParity;}
	int GetDataBit(){return m_nDataBit;}
	int GetStopBit(){return m_nStopBit;}
	boost::shared_ptr<boost::asio::serial_port> GetSerialPort(){return m_pPort;}

private:
	char ParityChar();

public:
	CSerialPortBoost(void);
	~CSerialPortBoost(void);
	bool InitPort(CWnd* pPortOwner, UINT portnr = 1, UINT baud = 19200, UINT parity = 0, UINT databits = 8, UINT stopbits = 1);
	bool SetStart(bool bStart);
	void WaitData();
	void SendSerialData(byte *pData, int len);
	CString GetStrID();
	int GetBaudIdx();
	int GetStopIdx();
	int GetDataIdx();
	void SetBaudFromIdx(int idx);
	void SetStopFromIdx(int idx);
	void SetDataFromIdx(int idx);
};
}
