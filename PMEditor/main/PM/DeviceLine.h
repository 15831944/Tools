#pragma once

namespace MVC{namespace Device{
class CDeviceLine
{
private:
	UINT m_uiType;			//!< �ߵ�����
	CPoint m_ptStart;		//!< ��ʼ��ַ
	CPoint m_ptEnd;			//!< ��ֹ��ַ
	UINT m_uiState;			//!< ��ǰ״̬

public:
	CDeviceLine(void);
	~CDeviceLine(void);
};
}}