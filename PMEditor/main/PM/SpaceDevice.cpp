#include "StdAfx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "MainFrm.h"
#include "SpaceDevice.h"

using namespace Space;

CSpaceDevice::CSpaceDevice(void)
{
}

CSpaceDevice::~CSpaceDevice(void)
{
}

BEGIN_MESSAGE_MAP(Space::CSpaceDevice, CXTPTaskPanel)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

bool CSpaceDevice::CreateSpace()
{
	CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
	if (!Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), mf, 2)){
		TRACE0("Failed to create outlook bar.");
		return FALSE;
	}

#ifdef DEBUG
	//SetTheme(xtpTaskPanelThemeShortcutBarOffice2003);
#else
	//SetTheme(xtpTaskPanelThemeToolboxWhidbey);
#endif
	SetTheme(xtpTaskPanelThemeVisualStudio2010);

	// Tell the control to send message to this window (the mainframe)
	// and not to its real parent (the splitter)
	SetOwner(mf);

	// Add icons to the large and small image lists.
	int nFolder;

	SetIconSize(CSize(32, 32));
//	SetIconSize(CSize(16, 16));

	// 添加元素
	FreshCompanyDevice();
	SetBehaviour(xtpTaskPanelBehaviourList);
//	SetBehaviour(xtpTaskPanelBehaviourToolbox);
	SetSelectItemOnFocus(TRUE);

	// Select the first folder.
	AllowDrag(TRUE);

	if(GetGroupCount() > 0)
		GetAt(0)->SetExpanded(TRUE);

	return true;
}

void CSpaceDevice::FreshCompanyDevice()
{
}

void CSpaceDevice::OnClick(CXTPTaskPanelGroupItem* pItem)
{
	UINT id = pItem->GetID();
}

BOOL Space::CSpaceDevice::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("支持的设备"));
	return CXTPTaskPanel::OnHelpInfo(pHelpInfo);
}
