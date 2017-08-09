#pragma once

namespace MVC{namespace Device{
//!< ɨ��ӿ�������Ϣ
class CScanSetInfo
{
private:
	bool m_bScanCompiled;					//!< �Ƿ��Ѿ������˱���
	int m_nScanLevel;					//!< ɨ�輶��,1~5
	bool m_bUseEthernet;				//!< �Ƿ�������̫��ɨ��
	bool m_bUseSerial;					//!< �Ƿ����ô���ɨ��
	bool m_bSync;						//!< �Ƿ�ɨ������ͬ��������ʦվ������
	DWORD m_uiIPStart;					//!< ɨ����ʼIP��ַ
	DWORD m_uiIPEnd;					//!< ɨ����ֹIP��ַ
	UINT m_nSerialStart;				//!< ��ʼģ���ַ
	UINT m_nSerialEnd;					//!< ��ֹģ���ַ
	CString m_strCom;					//!< ����
	CString m_strBaud;					//!< ������
	CString m_strParity;				//!< У�鷽ʽ
	CString m_strDataBits;				//!< ����λ
	CString m_strStopBits;				//!< ֹͣλ

public:
	bool IsCompiled(){return m_bScanCompiled;}			//!< �Ƿ��Ѿ������˱���
	int GetScanLevel(){return m_nScanLevel;}		//!< ɨ�輶��
	bool IsUseEthernet(){return m_bUseEthernet;}	//!< �Ƿ�������̫��ɨ��
	bool IsUseSerial(){return m_bUseSerial;}		//!< �Ƿ����ô���ɨ��
	bool IsSync(){return m_bSync;}					//!< �Ƿ�ɨ������ͬ��������ʦվ������
	DWORD GetIPStart(){return m_uiIPStart;}			//!< ɨ����ʼIP��ַ
	DWORD GetIPEnd(){return m_uiIPEnd;}				//!< ɨ����ֹIP��ַ
	UINT GetSerialStart(){return m_nSerialStart;}	//!< ��ʼģ���ַ
	UINT GetSerialEnd(){return m_nSerialEnd;}		//!< ��ֹģ���ַ
	CString GetCom(){return m_strCom;}				//!< ����
	CString GetBaud(){return m_strBaud;}			//!< ������
	CString GetParity(){return m_strParity;}		//!< У�鷽ʽ
	CString GetDataBit(){return m_strDataBits;}		//!< ����λ
	CString GetStopBit(){return m_strStopBits;}		//!< ֹͣλ

	void SetScanCompiled(){m_bScanCompiled = true;}			//!< �Ѿ������˱���
	void SetUnCompiled(){m_bScanCompiled = false;}		//!< ��û�о�������
	void SetScanLevel(int level);					//!< ɨ�輶��
	void SetSync(bool bSync);						//!< �Ƿ�ɨ������ͬ��������ʦվ������
	void SetUseEthernet(bool b);					//!< �Ƿ�������̫��ɨ��
	void SetUseSerial(bool b);						//!< �Ƿ����ô���ɨ��
	void SetIPStart(DWORD start);					//!< ɨ����ʼIP��ַ
	void SetIPEnd(DWORD end);						//!< ɨ����ֹIP��ַ
	void SetSerialStart(UINT start);				//!< ��ʼģ���ַ
	void SetSerialEnd(UINT end);					//!< ��ֹģ���ַ
	void SetCom(CString com);						//!< ����
	void SetBaud(CString baud);						//!< ������
	void SetParity(CString parity);					//!< У�鷽ʽ
	void SetDataBit(CString databit);				//!< ����λ
	void SetStopBit(CString stopbit);				//!< ֹͣλ

public:
	CScanSetInfo(void);
	~CScanSetInfo(void);
	void SaveFile();
	void LoadFile();
	void Init();									//!< ���ݳ�ʼ��
	void OnSync();									//!< ͬ��������ʦվ����

private:
	void SerializeXml(TiXmlElement* node, bool bRead);
};
}}