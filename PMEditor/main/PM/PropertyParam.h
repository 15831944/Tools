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
	void ShowAndHide(CXTPPropertyGrid& grid);							//!< 显示或隐藏相关项
	bool FindPara(UINT id);
	void AddItem(boost::shared_ptr<CDeviceParam> xmlPara, CXTPPropertyGridItem* pParaGroup);

public:
	void SetInfo(UINT devID, CString behaviorName, UINT slaveID, bool bReadOnly = false);		//!< 获取设备编号，和行为名称

	virtual void OnShowHelp();											//!< 显示帮助信息
	virtual void ShowInfo(CXTPPropertyGrid& grid);						//!< 显示表格的函数
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);				//!< 表格初始化完成后
	virtual CString GetTitle();											//!< 获得本窗体的名称
	virtual void OnCloseGrid();											//!< 当关闭表格时
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);					//!< 要保存修改的内容
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID);			//!< 当某项的值发生改变时
	virtual void FreshAllData(CXTPPropertyGrid& grid);					//!< 刷新当前显示的所有数据
};
}
}