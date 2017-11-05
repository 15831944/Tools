#pragma once
#include "ObjectInProperty.h"

namespace MVC{
namespace Item{
class CItem;
class CPropertyItem : public Dialog::CObjectInProperty
{
private:
	bool m_bAdd;		//!< 表示是否是添加状态，true表示添加变量，false表示修改变量
	UINT m_GroupID;		//!< 无论是添加还是修改，都要记录一下变量所属组的ID号
	CItem* m_Item;		//!< 当前所指向的变量，如果是新建，这个为空
	std::shared_ptr<CItem> m_ShowItem;			//!< 要显示用的变量

public:
	std::shared_ptr<CItem> m_NewItem;				//!< 记录上次添加的变量的所有信息，这次添加时显示上次添加的信息
	void SetType(bool add = true){m_bAdd = add;}
	void SetItem(CItem* item){m_Item = item;}
	void SetGroupID(UINT id){m_GroupID = id;}
	void OnChangeType(CXTPPropertyGrid& grid);						//!< 当类型发生改变时，这个类型包括工程值类型、数据源类型、IO区操作类型

public:
	CPropertyItem(void);
	~CPropertyItem(void);

private:
	virtual void OnShowHelp();										//!< 显示帮助
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< 显示表格的函数
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);			//!< 表格初始化完成后
	virtual CString GetTitle();										//!< 获得本窗体的名称
	virtual void OnCloseGrid();										//!< 当关闭表格时
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT id);		//!< 当某一项被修改时
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< 要保存被改变的内容
	virtual void OnButtonClick(CXTPPropertyGrid& grid, UINT btID);	//!< 按钮被按下
	void CreateNew();
	void CreateEdit();

	void ShowAndHide(CXTPPropertyGrid& grid);						//!< 在这里统一做显示与隐藏
	void EnableAndDisenable(CXTPPropertyGrid& grid);				//!< 在这里统一使能
	bool IfRightItemName(CString strName);							//!< 变量名是否合法
};
}
}