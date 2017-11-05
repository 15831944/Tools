#pragma once
// #include "c:\program files\codejock software\mfc\xtreme toolkitpro v12.1.1\source\taskpanel\xtptaskpanel.h"

namespace Space{
class CSpaceDevice : public CXTPTaskPanel
{
public:
	CSpaceDevice(void);
	~CSpaceDevice(void);

	bool CreateSpace();
	void FreshCompanyDevice();		// 画公司设备列表
	virtual void OnClick(CXTPTaskPanelGroupItem* pItem);
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}