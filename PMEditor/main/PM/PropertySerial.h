#pragma once
#include "ObjectInProperty.h"

namespace Dialog{class CSerialSetDlg;}
namespace MVC{namespace Device{namespace InterfaceSet{
class CPropertySerial : public Dialog::CObjectInProperty
{
private:
	CString m_strSerialName;	//!< 串口名字

public:
	CPropertySerial();
	~CPropertySerial(void);
	Dialog::CSerialSetDlg* m_pParent;

public:
	void SetInfo(CString serialName);									//!< 设置串口

	virtual CString GetTitle();											//!< 获得本窗体的名称
	virtual void OnShowHelp();											//!< 显示帮助信息
	virtual void ShowInfo(CXTPPropertyGrid& grid);						//!< 显示表格的函数
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID);			//!< 当某项的值发生改变时
	virtual bool OnSaveModify(CXTPPropertyGrid& grid){return true;}
	virtual void OnCloseGrid(){;}
};
}}}