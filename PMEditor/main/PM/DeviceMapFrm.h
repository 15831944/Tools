#pragma once
#define CChildFrameBase CXTPFrameWndBase<CMDIChildWnd>

// CDeviceMapFrm 框架
namespace MVC{
namespace Device{
class CDeviceMapFrm : public CChildFrameBase
{
	DECLARE_DYNCREATE(CDeviceMapFrm)
private:
	bool m_bFirstLoad;			//!< 是否是第一次加载

protected:
	CDeviceMapFrm();           // 动态创建所使用的受保护的构造函数
	virtual ~CDeviceMapFrm();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
};
}
}
