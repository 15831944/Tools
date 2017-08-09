#pragma once
#include "PropertyDevice.h"
#include "PropertySlave.h"
#include "DEthernet.h"

namespace MVC{
namespace Device{
class CDeviceOne;
class CDeviceMapDoc;
class CScanMgr;
class CScanSetInfo;
namespace InterfaceSet{class CDSerial;}

typedef struct tagBehaviorRequest{
	virtual void OnDataReady() = 0;							//!< �����Ѿ�ˢ�º���
	virtual void OnDeviceChange() = 0;						//!< �����������ı�ʱ
}SBehaviorRequest;

class CDevMgr
{
public:
	enum{							//!< Modbus�豸��Ҫ���⴦��,��
		SUPPORT_DNS = 1,			// �Ƿ���ʾ�豸����
		MODBUS_SERIAL = 7,
		MODBUS_UDP = 8,
		MODBUS_TCP = 9
	};
	CDeviceMapDoc* m_DevMapDoc;
	std::vector<boost::shared_ptr<CDeviceOne> > m_vtDevice;
	std::list<boost::shared_ptr<CDeviceOne> > m_ltDevClipBoard;			//!< �豸�ļ�����
	std::list<boost::shared_ptr<InterfaceSet::CDSerial> > m_ltSerial;
	CPropertyDevice m_DeviceProperty;
	CPropertySlave m_SlaveProperty;
	CScanMgr* m_ScanMgr;

private:
	SYSTEMTIME m_EditTime;			//!< ����޸ĵ��¼�
	bool m_bModify;					//!< �Ƿ��޸�
	CString m_strName;
	CString m_strFileName;
	CString m_strVersion;
	UINT m_uiSerialNum;
	boost::shared_ptr<InterfaceSet::CDEthernet> m_Ethernet;		//!< ��̫��ͨ�����ö���Ŀǰֻ֧��һ��
	boost::shared_ptr<CScanSetInfo> m_ScanSetInfo;				//!< ɨ����Ϣ������
	std::list<SBehaviorRequest *> m_ltBRequest;					//!< ��������ļ�����

public:
	bool IsModify(){return m_bModify;}
	CString getName(){return m_strName;}
	CString getFileName(){return m_strFileName;}
	CString getVersion(){return m_strVersion;}
	boost::shared_ptr<CScanSetInfo> GetScanInfo(){return m_ScanSetInfo;}	//!< ɨ����Ϣ������

	void SetModify(){m_bModify = true;}

private:
	CDevMgr(void);
	~CDevMgr(void);

	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< ����xml��ʽ�ı༭��Ϣ
	void AwayFromSameInf(boost::shared_ptr<CDeviceOne> device);	//!< ��֤�豸ӵ������˲�ͬ�Ľӿ�
	void MakeSureDfrtInf(boost::shared_ptr<CDeviceOne> device);	//!< ��֤�豸ӵ������˲�ͬ�Ľӿڣ������ͬ����������
	void OnDeleteDeviceChild(UINT id);							//!< ɾ�����豸�����к��ӣ��������ӵĺ��ӣ��ȵ�
	void InitComputerSerial();									//!< ��ʼ��������Ϣ���Ѽ����ɨ������Ĵ���ǿ�м��ؽ�������

public:
	static CDevMgr& GetMe();
	CString GetEditTime();
	void OnCreate();
	void OnClose();
	void SaveFile();
	bool CheckAndConnect();										//!< Ϊ�豸֮�佨������
	void ShowDevice(UINT id);									//!< ��������ѡ������豸������ʾ

	void OpenDoc();
	void SetDevWatch(const bool bWatch);						//!< ���ü��״̬
	void SetDevScan(const bool bScan);							//!< ����ɨ��״̬
	boost::shared_ptr<CDeviceOne> AddNewDevice(boost::shared_ptr<CDeviceOne> device);
	boost::shared_ptr<CDeviceOne> GetDevice(UINT id);
	boost::shared_ptr<CDeviceOne> GetDevice(CString name);
	boost::shared_ptr<CDeviceOne> GetDevice(UINT devType, UINT infType, UINT level, long* plAddr);
	boost::shared_ptr<CDeviceOne> GetDeviceNameList(std::list<CString>& strList, int& defIndex);	//!< ��������豸�������б�����defIndex�ŵ��豸
	boost::shared_ptr<InterfaceSet::CDSerial> GetSerial(CString name);
	boost::shared_ptr<InterfaceSet::CDEthernet> GetEthernet(){return m_Ethernet;}
	UINT GetDeviceSize();										//!< ����豸����ʵ����
	void DeleteDevice(UINT id);									//!< ɾ���豸�����������豸�������κδ���
	void UndoDelDevice(UINT id);								//!< ɾ���豸����Ҫ��¼������Ϣ
	void UndoUpdDevice(UINT id);								//!< �޸��豸����Ҫ��¼������Ϣ
	bool OpenDevMgrFile(CString name, CString pathall, CString ver, CString stime);					//!< �������ļ������� �� ���� + ·�� + �汾�ţ�����޸�ʱ��
	void OnSetInf();											//!< �������ô���
	void OnDevCopy(UINT id);									//!< ���������ŵ��豸��������
	void OnDevCopyWithChild(UINT id);							//!< ���������ŵ��豸�Լ��������д��豸��������
	void OnDevCut(CDeviceMapDoc* pDoc, UINT id);				//!< ���������ŵ��豸�Լ��������д��豸��������
	void OnDevPaste(CDeviceMapDoc* pDoc, UINT id = UINT(-1));	//!< ������������豸ճ��������豸���棬��������ھ�ֱ������ʦվ
	int SearchDev(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex = false);	//!< ���ҷ��ϸ��ַ����ı���������ƥ�������
	void SetScanInfo();											//!< ����ɨ�����
	void OnScanRev(char* data, UINT num);						//!< ���ɨ�����������
	void OnDeviceStatus(long nDeviceID,long nDeviceInterface,long nDeviceStatus);		//!< �豸״̬�ı��¼���nDeviceInterface
	void OnBehavior(long lBehaviorID, long lDeviceID, VARIANT& varValue, long lResult);

	void FreshView();											//!< �����������Ҳ��ˢһ��ҳ��
	CString GetSimilarName(CString);							//!< ���һ���������������

	void OnBehaviorOver();										//!< ����Ϊ������Ҫ֪ͨ���м�����
	void RegisteBRequest(SBehaviorRequest* one);				//!< ע�������
	void UnRegisteBRequest(SBehaviorRequest* one);				//!< ע��������

	int GetMaxOverTime(boost::shared_ptr<CDeviceOne> device);	//!< ��ø��豸��ʹ�õ����ĳ�ʱʱ��
};
}
}