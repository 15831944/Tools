#pragma once
#include "ObjectInProperty.h"

namespace XmlInfo{class CXmlParaInfo;}

namespace MVC{
namespace Device{
class CDeviceOne;
class CDeviceParam;
class CPropertyParam : public Dialog::CObjectInProperty
{
public:
	boost::shared_ptr<CDeviceOne> m_ShowDev;
	std::list<UINT> m_ltParamID;
	UINT m_uiGroupIndex;
	bool m_bReadOnly;

public:
	CPropertyParam(void);
	~CPropertyParam(void);

private:
	void ShowAndHide(CXTPPropertyGrid& grid);							//!< ��ʾ�����������
	bool FindPara(UINT id);
	void AddItem(boost::shared_ptr<CDeviceParam> xmlPara, CXTPPropertyGridItem* pParaGroup);

public:
	void SetInfo(UINT devID, CString behaviorName, UINT slaveID, bool bReadOnly = false);		//!< ��ȡ�豸��ţ�����Ϊ����

	virtual void OnShowHelp();											//!< ��ʾ������Ϣ
	virtual void ShowInfo(CXTPPropertyGrid& grid);						//!< ��ʾ���ĺ���
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);				//!< ����ʼ����ɺ�
	virtual CString GetTitle();											//!< ��ñ����������
	virtual void OnCloseGrid();											//!< ���رձ��ʱ
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);					//!< Ҫ�����޸ĵ�����
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID);			//!< ��ĳ���ֵ�����ı�ʱ
	virtual void FreshAllData(CXTPPropertyGrid& grid);					//!< ˢ�µ�ǰ��ʾ����������
};
}
}