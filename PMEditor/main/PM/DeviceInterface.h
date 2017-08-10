#pragma once

namespace MVC{
namespace Device{
class CDeviceOne;
class CDeviceInterface
{
public:
	enum Inf_Color{
		INF_ETHERNET = RGB(0, 0, 255),
		INF_SERIAL = RGB(160, 40, 0),
		INF_SLAVE = RGB(150, 150, 40)
	};
private:
	bool m_bProj;					//!< �Ƿ��ǹ��̵Ľӿ�
	int m_nState;					//!< ����״̬��0���ߣ�1���ߣ�2����δ����(ֻ�����ڴ��豸)
	CDeviceOne* m_pDevice;
	UINT m_uiID;					//!< �ӿڵı��
	UINT m_uiType;					//!< ���ͣ�0��̫����1���ڣ�2ZigBee��16���豸
	UINT m_uiSerialNo;				//!< ���ںţ�0����COM1���Ժ���������
//	UINT m_uiIP;					//!< IP��ַ
	int	 m_uiDevPort;					//!<�豸�˿ں�
	UINT m_uiHandle;				//!< ���ڵ�ַ����������豸����ģ���ַ������Ǵ��豸����Handle��

	CString m_strDoMain;  //!<�豸������

public:
	CDeviceInterface(CDeviceOne* pDevice);
	~CDeviceInterface(void){;}

	bool IsOnline(){return m_nState == 1;}
	int GetState(){return m_nState;}
	CDeviceOne* GetDevice(){return m_pDevice;}
	UINT GetID(){return m_uiID;}
	UINT GetType(){return m_uiType;}
	UINT GetSerialNo(){return m_uiSerialNo;}
//	UINT GetIP(){return m_uiIP;}
	CString GetDoMain(){return m_strDoMain;}
	UINT GetHandle(){return m_uiHandle;}
	UINT GetDevPort(){return m_uiDevPort;}
	bool IsProj(){return m_bProj;}

	void SetState(int nState){m_nState = nState % 3;}
	void SetID(UINT id){m_uiID = id;}
	void SetType(UINT type){m_uiType = type;}
	void SetSerialNo(UINT no){m_uiSerialNo = no;}
//	void SetIP(UINT ip){m_uiIP = ip;}
	void SetDoMain(CString domain){m_strDoMain=domain;}
	void SetHandle(UINT handle){m_uiHandle = handle;}
	void SetProj(bool b = true){m_bProj = b;}
	void SetDevPort(UINT DevPort){m_uiDevPort=DevPort;}

public:
	CDeviceInterface& operator = (CDeviceInterface& inf);
	bool operator != (CDeviceInterface& inf) const;
	CString GetName(bool bShowScan = true);
	CString GetTypeName();
	bool SerializeXml(TiXmlElement* pNode, bool bRead);
	COLORREF GetInfColor();			//!< ����ߵ���ɫ
};
}
}
