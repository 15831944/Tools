// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#if _MSC_VER < 1600
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
#define WINVER 0x0500		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
#define _WIN32_WINNT 0x0500	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0510 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
#define _WIN32_IE 0x0500	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#endif
#else	//_MSC_VER < 1600
#include <SDKDDKVer.h>
#endif //_MSC_VER < 1600


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxpriv.h>
#include <afxtempl.h>


#define 	GUISTYLE_XP   0x08001 	
#define		GUISTYLE_2003 0x08002
#define		GUISTYLE_2003MENUBTN 0x08021
#define		WIN_NT		  0x08003 	
#define		WIN_2000	0x08004
#define		WIN_XP		0x08005
#define		WIN_95		0x08006
#define		WIN_98		0x08007
#define		WIN_32		0x08008
#define		WIN_ME		0x08009
#define		WIN_95OSR2	0x0800A
#define		WIN_98_SE	0x0800B

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#include <assert.h>
#include <string>

using namespace std;

#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef TOOLTIP_EXPORTS
#define TOOLTIP_API __declspec(dllexport)
#else
#define TOOLTIP_API __declspec(dllimport)
#endif
