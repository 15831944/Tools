#pragma once
#include "objectinproperty.h"

namespace MVC{namespace Device{namespace InterfaceSet{
class CPropertyEthernet : public Dialog::CObjectInProperty
{
private:
	UINT m_uiLocalPort;			//!< ����ͨѶ�˿ڣ�0Ϊ����˿�
	UINT m_uiRetryNum;			//!< ���Դ���
	UINT m_uiOverTime;			//!< ��ʱʱ��
	UINT m_uiCoolTime;			//!< ��ȴʱ��
	UINT m_uiReviveTime;		//!< �ָ�ʱ��
	UINT m_uiScanTime;			//!< ɨ������

private:
	void DataReady();

public:
	virtual void OnCloseGrid(){;}

public:
	CPropertyEthernet(void);
	~CPropertyEthernet(void);

	virtual CString GetTitle();											//!< ��ñ����������
	virtual void OnShowHelp();											//!< ��ʾ������Ϣ
	virtual void ShowInfo(CXTPPropertyGrid& grid);						//!< ��ʾ���ĺ���
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID);			//!< ��ĳ���ֵ�����ı�ʱ
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);
};
}}}