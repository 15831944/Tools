#pragma once

#ifdef TOOLTIP_EXPORTS
#define TOOLTIP_API __declspec(dllexport)
#else
#define TOOLTIP_API __declspec(dllimport)
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Additional MFC headers
#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif //__AFXTEMPL_H__

#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#endif //__AFXPRIV_H__

#if _MSC_VER >= 1300
#include <..\atlmfc\src\mfc\afximpl.h>
#else
#include <..\src\afximpl.h>
#endif


#include <string>

using namespace std;

#ifdef _DEBUG
#define  TRACEERROR(str)		TraceError(str)
#else
#define  TRACEERROR(str)		(void *)NULL
#endif

//! 颜色设置
#define RGB_WHITE	(RGB(255,255,255))


#pragma comment(lib,"XToolTip.lib")


#include "stdafx.h"
#include "XSuperTooltip.h"

/*			//	这部分已经在GUITON\OLKIT.H中定义
enum GUIUserMsg{
	GUM_SHOWTITLE = (WM_USER+1000)
};
static void TraceError(const string& str)
{
	TRACE("Error %s@%s,%d",str.c_str(),__FILE__,__LINE__);
	ASSERT(FALSE);
}

*/