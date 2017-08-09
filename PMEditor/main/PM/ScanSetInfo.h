#pragma once

namespace MVC{namespace Device{
//!< 扫描接口配置信息
class CScanSetInfo
{
private:
	bool m_bScanCompiled;					//!< 是否已经经过了编译
	int m_nScanLevel;					//!< 扫描级数,1~5
	bool m_bUseEthernet;				//!< 是否启用以太网扫描
	bool m_bUseSerial;					//!< 是否启用串口扫描
	bool m_bSync;						//!< 是否将扫描配置同步到工程师站配置中
	DWORD m_uiIPStart;					//!< 扫描起始IP地址
	DWORD m_uiIPEnd;					//!< 扫描终止IP地址
	UINT m_nSerialStart;				//!< 起始模块地址
	UINT m_nSerialEnd;					//!< 终止模块地址
	CString m_strCom;					//!< 串口
	CString m_strBaud;					//!< 波特率
	CString m_strParity;				//!< 校验方式
	CString m_strDataBits;				//!< 数据位
	CString m_strStopBits;				//!< 停止位

public:
	bool IsCompiled(){return m_bScanCompiled;}			//!< 是否已经经过了编译
	int GetScanLevel(){return m_nScanLevel;}		//!< 扫描级数
	bool IsUseEthernet(){return m_bUseEthernet;}	//!< 是否启用以太网扫描
	bool IsUseSerial(){return m_bUseSerial;}		//!< 是否启用串口扫描
	bool IsSync(){return m_bSync;}					//!< 是否将扫描配置同步到工程师站配置中
	DWORD GetIPStart(){return m_uiIPStart;}			//!< 扫描起始IP地址
	DWORD GetIPEnd(){return m_uiIPEnd;}				//!< 扫描终止IP地址
	UINT GetSerialStart(){return m_nSerialStart;}	//!< 起始模块地址
	UINT GetSerialEnd(){return m_nSerialEnd;}		//!< 终止模块地址
	CString GetCom(){return m_strCom;}				//!< 串口
	CString GetBaud(){return m_strBaud;}			//!< 波特率
	CString GetParity(){return m_strParity;}		//!< 校验方式
	CString GetDataBit(){return m_strDataBits;}		//!< 数据位
	CString GetStopBit(){return m_strStopBits;}		//!< 停止位

	void SetScanCompiled(){m_bScanCompiled = true;}			//!< 已经经过了编译
	void SetUnCompiled(){m_bScanCompiled = false;}		//!< 还没有经过编译
	void SetScanLevel(int level);					//!< 扫描级数
	void SetSync(bool bSync);						//!< 是否将扫描配置同步到工程师站配置中
	void SetUseEthernet(bool b);					//!< 是否启用以太网扫描
	void SetUseSerial(bool b);						//!< 是否启用串口扫描
	void SetIPStart(DWORD start);					//!< 扫描起始IP地址
	void SetIPEnd(DWORD end);						//!< 扫描终止IP地址
	void SetSerialStart(UINT start);				//!< 起始模块地址
	void SetSerialEnd(UINT end);					//!< 终止模块地址
	void SetCom(CString com);						//!< 串口
	void SetBaud(CString baud);						//!< 波特率
	void SetParity(CString parity);					//!< 校验方式
	void SetDataBit(CString databit);				//!< 数据位
	void SetStopBit(CString stopbit);				//!< 停止位

public:
	CScanSetInfo(void);
	~CScanSetInfo(void);
	void SaveFile();
	void LoadFile();
	void Init();									//!< 数据初始化
	void OnSync();									//!< 同步到工程师站配置

private:
	void SerializeXml(TiXmlElement* node, bool bRead);
};
}}