// Tooltip.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "ProjectMgr.h"
#include "XTooltip.h"

// CXTooltip
using namespace Tool;
IMPLEMENT_DYNAMIC(CXTooltip, CXSuperTooltip)
CXTooltip::CXTooltip()
{
	m_drawer.SetMaxWidth(30);
	m_Sti.nVirtualKeyCode = VK_F1;
	m_Sti.nKeyCodeId = 12345;			// can be anything you want
	m_Sti.nSizeX = 100;
	m_Sti.nBodyImage = IDB_TOOLTIP_DEVICE;

	m_Sti.bLineAfterHeader  = FALSE;			//!< 是否画线
	m_Sti.bLineBeforeFooter = TRUE;
	m_Sti.nFooterImage = IDB_HELP;

/*	黄色调
	m_Sti.rgbBegin = RGB(255,255,128);
	m_Sti.rgbMid   = RGB(255,255,0);
	m_Sti.rgbEnd   = RGB(255,128,0);
*/
/*	淡蓝色调 */
	m_Sti.rgbBegin = RGB(216, 228, 248);
	m_Sti.rgbMid   = RGB(255, 255, 255);
	m_Sti.rgbEnd   = RGB(216, 228, 227);

	m_Sti.rgbText  = RGB(76,76,76);
}

CXTooltip::~CXTooltip()
{
}


BEGIN_MESSAGE_MAP(CXTooltip, CXSuperTooltip)
END_MESSAGE_MAP()


// CXTooltip 消息处理程序

void CXTooltip::AddTooltipText(CWnd* pWnd,TooltipText& txtGroup)
{
	RemoveTool(pWnd);
	SetCallbackHyperlink(pWnd->GetSafeHwnd(), UNM_HYPERLINK_CLICKED);

	if(CProjectMgr::GetMe().IsWatch())				m_Sti.nSizeX = 220;
	else											m_Sti.nSizeX = 180;
	m_Sti.strHeader=txtGroup.txtHead;
	m_Sti.strBody=txtGroup.txtBody;
	m_Sti.strFooter=txtGroup.txtRemark;
	m_Sti.pWnd            = pWnd;

	AddTool(&m_Sti);
}
