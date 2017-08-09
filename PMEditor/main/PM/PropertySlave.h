#pragma once
#include "propertydlg.h"

namespace XmlInfo{class CXmlDevice;}
namespace MVC{
namespace Device{
class CDeviceOne;
class CPropertySlave : public Dialog::CObjectInProperty
{
public:
	UINT m_uiType;		//!< 本属性类型，0表示新增，1表示修改
	UINT m_uiID;		//!< 当新增时，这个ID是设备描述的编号；当修改时，这个ID是设备的编号
	UINT m_uiParentID;	//!< 所属设备的ID

public:
	boost::shared_ptr<CDeviceOne> m_ShowDev;

public:
	CPropertySlave(void);
	~CPropertySlave(void);

private:
	void ShowAndHide(CXTPPropertyGrid& grid);							//!< 显示或隐藏相关项
	void SetOtherDevName();												//!< 给m_ShowDev设置一个新的默认名称

public:
	void SetType(UINT type, UINT id, UINT parentID);	//!< 设置属性类型，type=0新增，id表示设备描述的ID号，type=1表示修改，id表示设备ID号

	virtual void OnShowHelp();											//!< 显示帮助信息
	virtual void ShowInfo(CXTPPropertyGrid& grid);						//!< 显示表格的函数
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);				//!< 表格初始化完成后
	virtual CString GetTitle();											//!< 获得本窗体的名称
	virtual void OnCloseGrid();											//!< 当关闭表格时
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);					//!< 要保存修改的内容
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID);			//!< 当某项的值发生改变时

};
}
}