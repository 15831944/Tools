#pragma once
#include "MyHelp.h"

namespace Dialog{
//!< 属性数据管理类的父类
class CObjectInProperty
{
public:
	CObjectInProperty(){;}
	virtual ~CObjectInProperty(){;}
	virtual void OnShowHelp() = 0;											//!< 显示帮助
	virtual void ShowInfo(CXTPPropertyGrid& grid) = 0;						//!< 显示表格的函数
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid){;}					//!< 表格初始化完成后
	virtual CString GetTitle()=0;											//!< 获得本窗体的名称
	virtual void OnCloseGrid(){;}											//!< 当关闭表格时
	virtual bool OnSaveModify(CXTPPropertyGrid& grid) = 0;					//!< 要保存修改的内容
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID){;}			//!< 当某项的值发生改变时
	virtual void OnButtonClick(CXTPPropertyGrid& grid, UINT btID){;}		//!< 按钮被按下

	//!< 添加编辑文本项
	CXTPPropertyGridItem* AddItemText(CXTPPropertyGridItem& parent, CString title, CString tooltip, CString value, UINT id);
	//!< 添加可多行编辑的文本项
	CXTPPropertyGridItem* AddItemMultiText(CXTPPropertyGridItem& parent, CString title, CString tooltip, CString value, UINT id);
	//!< 添加数字项
	CXTPPropertyGridItem* AddItemNumber(CXTPPropertyGridItem& parent, CString title, CString tooltip, long value, UINT id, BOOL empty = FALSE);
	//!< 添加浮点项,最后一个参数表示保留小数点多少位
	CXTPPropertyGridItem* AddItemDouble(CXTPPropertyGridItem& parent, CString title, CString tooltip, double value, UINT id, BOOL empty = FALSE, int fFmt = 2);
	//!< 添加下拉列表项
	CXTPPropertyGridItem* AddItemList(CXTPPropertyGridItem& parent, CString title, CString tooltip, std::list<CString>& items, int def, UINT id);
	//!< 添加Variant型项，只识别bool,char,byte,short,word,int32,uint32,float,double,bstr等类型
	CXTPPropertyGridItem* AddItemVariant(CXTPPropertyGridItem& parent, CString title, CString tooltip, CComVariant var, UINT id, BOOL empty = FALSE);
	//!< 添加IP类型项
	CXTPPropertyGridItem* AddItemIP(CXTPPropertyGridItem& parent, CString title, CString tooltip, UINT ip, UINT id, BOOL empty = FALSE);
	//!< 设置列表项的内容
	void SetList(CXTPPropertyGrid& grid, UINT id, std::list<CString>& items, int def);
	void SetText(CXTPPropertyGrid& grid, UINT id, CString value, bool def);
};
}