#pragma once

namespace MVC{namespace Device{
class CDeviceLine
{
private:
	UINT m_uiType;			//!< 线的类型
	CPoint m_ptStart;		//!< 起始地址
	CPoint m_ptEnd;			//!< 终止地址
	UINT m_uiState;			//!< 当前状态

public:
	CDeviceLine(void);
	~CDeviceLine(void);
};
}}