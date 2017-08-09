#pragma once
#include "ObjectInProperty.h"

namespace Servers{
namespace RongYu{
class CPropertyRongYu : public Dialog::CObjectInProperty
{
public:
	CPropertyRongYu(void){;}
	~CPropertyRongYu(void){;}

private:
	virtual void OnShowHelp();										//!< 显示帮助信息
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< 显示表格的函数
	virtual CString GetTitle();										//!< 获得本窗体的名称
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< 要保存被改变的内容
};
}}