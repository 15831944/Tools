#pragma once
#include "ObjectInProperty.h"

namespace Servers{
namespace DB{
class CPropertyDB : public Dialog::CObjectInProperty
{
public:
	CPropertyDB(void);
	~CPropertyDB(void);
private:
	virtual void OnShowHelp();										//!< 显示帮助信息
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< 显示表格的函数
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);			//!< 表格初始化完成后
	virtual void EnableAndDisenable(CXTPPropertyGrid& grid);		//!< 在这里统一使能
	virtual CString GetTitle();										//!< 获得本窗体的名称
	virtual void OnCloseGrid();										//!< 当关闭表格时
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT id);		//!< 当某一项被修改时
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< 要保存被改变的内容
	virtual void OnButtonClick(CXTPPropertyGrid& grid, UINT btID);	//!< 按钮被按下
};
}}