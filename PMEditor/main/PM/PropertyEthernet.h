#pragma once
#include "objectinproperty.h"

namespace MVC{namespace Device{namespace InterfaceSet{
class CPropertyEthernet : public Dialog::CObjectInProperty
{
private:
	UINT m_uiLocalPort;			//!< 本地通讯端口，0为随机端口
	UINT m_uiRetryNum;			//!< 重试次数
	UINT m_uiOverTime;			//!< 超时时间
	UINT m_uiCoolTime;			//!< 冷却时间
	UINT m_uiReviveTime;		//!< 恢复时间
	UINT m_uiScanTime;			//!< 扫描周期

private:
	void DataReady();

public:
	virtual void OnCloseGrid(){;}

public:
	CPropertyEthernet(void);
	~CPropertyEthernet(void);

	virtual CString GetTitle();											//!< 获得本窗体的名称
	virtual void OnShowHelp();											//!< 显示帮助信息
	virtual void ShowInfo(CXTPPropertyGrid& grid);						//!< 显示表格的函数
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID);			//!< 当某项的值发生改变时
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);
};
}}}