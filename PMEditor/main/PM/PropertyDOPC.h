#pragma once
#include "ObjectInProperty.h"

namespace Servers{
namespace DOPC{
class CPropertyDOPC : public Dialog::CObjectInProperty
{
public:
	CPropertyDOPC(void);
	~CPropertyDOPC(void);

private:
	virtual void OnShowHelp();										//!< 显示帮助信息
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< 显示表格的函数
	virtual CString GetTitle();										//!< 获得本窗体的名称
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< 要保存被改变的内容
};
}}