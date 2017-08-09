#pragma once
#include "ObjectInProperty.h"

namespace XmlInfo{class CXmlDevice;}

namespace MVC{
namespace Device{
class CDeviceOne;
class CPropertyDevice : public Dialog::CObjectInProperty
{
public:
	UINT m_uiType;				//!< ���������ͣ�0��ʾ������1��ʾ�޸�
	UINT m_uiID;				//!< ������ʱ�����ID���豸�����ı�ţ����޸�ʱ�����ID���豸�ı��
	UINT m_uiInterfaceCount;	//!< �ӿڵ�����

public:
	boost::shared_ptr<CDeviceOne> m_ShowDev;

public:
	CPropertyDevice(void);
	~CPropertyDevice(void);

private:
	void ShowAndHide(CXTPPropertyGrid& grid);						//!< ��ʾ�����������
	void SetOtherDevName();											//!< ��m_ShowDev����һ���µ�Ĭ������
	void ReSetInteface();											//!< ���������豸�Ľӿ�

public:
	bool SetType(UINT type, UINT id);								//!< �����������ͣ�type=0������id��ʾ�豸������ID�ţ�type=1��ʾ�޸ģ�id��ʾ�豸ID��

	virtual void OnShowHelp();										//!< ��ʾ������Ϣ
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< ��ʾ���ĺ���
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);			//!< ����ʼ����ɺ�
	virtual CString GetTitle();										//!< ��ñ����������
	virtual void OnCloseGrid();										//!< ���رձ��ʱ
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< Ҫ�����޸ĵ�����
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID);		//!< ��ĳ���ֵ�����ı�ʱ
};
}
}