#pragma once
#include "DeviceInterface.h"

namespace MVC{namespace Device{
class CDeviceOne;
//!< ɨ��Ľӿ��࣬Ҫ����¼һЩ��Ϣ
class CScanInterface : public CDeviceInterface
{
public:
	UINT m_uiLevel;					//!< �Լ����ڼ���
	CString  m_uiStrParentID;		//!< ���豸�ı��
	CScanInterface(void);
	~CScanInterface(void){;}
};
class CScanMgr
{
	friend class CDevMgr;
private:
	std::list<boost::shared_ptr<CDeviceOne> > m_ltScanDev;
	char m_cOldData[64000];
	UINT m_uiOldNum;

private:
	CScanMgr(void);
	~CScanMgr(void);
	static CScanMgr& GetMe();

	template <class T>
	bool ReadData(T& t, int& index, const char* data, int size = 0, const int maxsize = -1);	//!< ��data��indexλ���϶�size��С����Ϣ�ŵ�t��

	bool OnCheckDev();			//!< ��鷵�ص����ݵ���ȷ��
	void ReBuildData();			//!< �ؽ����ݹ�ϵ
	void ReBuildOneParent();	//!< �ؽ����ݹ�ϵ����֤ÿ���豸�����ֻ��һ�����豸
	void ReBuildMain();			//!< �ؽ����ݹ�ϵ����ԭ�豸��Ϣ�����εĽṹǰ�������ѭ�������Ų�
	void ReBuildSlave(boost::shared_ptr<CDeviceOne> device, std::list<boost::shared_ptr<CDeviceOne> >& ltDevice);	//!< �ؽ����豸��ϵ
	void OnConnectToProj();		//!< ���ӵ�������
	void OnBuildConnect();		//!< �ؽ�CDevMgr����豸�����ӹ�ϵ
	bool SetOnLine(boost::shared_ptr<CDeviceOne> scanDev, boost::shared_ptr<CDeviceOne> projDev);					//!< ���ù����豸����
	int IsSameDevice(boost::shared_ptr<CDeviceOne> scanDev, boost::shared_ptr<CDeviceOne> projDev);	//!< �Ƿ���ͬһ���豸
	bool IsSameParent(boost::shared_ptr<CDeviceOne> scanDev, boost::shared_ptr<CDeviceOne> projDev);//!< �Ƿ���ͬһ������
	boost::shared_ptr<CDeviceOne> GetDevice(CString strDevID);				//!< ��ɨ���豸�б���ҵ����豸
	boost::shared_ptr<CDeviceOne> GetDeviceFromProj(CString strDevID);		//!< �ӹ����豸�б���ҵ����豸
	UINT GetDeviceCount(CString strDevID);									//!< �ҵ���strDevID���豸������
	void DelOffLineScanDev();	//!< ɾ�������в����ߵĹ����е�ɨ���豸

public:
	void ClearData(){memset(m_cOldData,0,64000);m_uiOldNum = 0;}			//!< �������
	void OnScanRev(const char* data, const int num);
	void DelAllScanDev();		//!< ɾ������ɨ���豸
};
}
}
