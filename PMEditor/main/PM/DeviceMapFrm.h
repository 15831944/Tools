#pragma once
#define CChildFrameBase CXTPFrameWndBase<CMDIChildWnd>

// CDeviceMapFrm ���
namespace MVC{
namespace Device{
class CDeviceMapFrm : public CChildFrameBase
{
	DECLARE_DYNCREATE(CDeviceMapFrm)
private:
	bool m_bFirstLoad;			//!< �Ƿ��ǵ�һ�μ���

protected:
	CDeviceMapFrm();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDeviceMapFrm();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
};
}
}
