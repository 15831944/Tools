// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0410 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0600	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS
#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#include <afxdhtml.h>

#ifndef _WINUSER_
#define _WINUSER_
#endif

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC 套接字扩展
#include <afxhtml.h>

#define _XTP_STATICLINK
#include <XTToolkitPro.h>

//#import <msxml.dll>

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#define rforeach BOOST_REVERSE_FOREACH
#include <boost/thread/thread.hpp>			//!< 线程
#include <boost/thread/mutex.hpp>			//!< 锁
#include <boost/bind.hpp>					//!< 线程绑定函数
#include <boost/signals.hpp>
#include <boost/signals/connection.hpp>

#include <vector>
#include <list>
#include <map>
#include <stack>
//#include <deque>
#include <afxdisp.h>
#include <afxdisp.h>
#include <afxdisp.h>

#include "../../Libs/TinyXml/TinyXmlInclude.h"

#include "../../Libs/XToolTip/XToolTipInclude.h"
#include "../../Libs/FillGuide/Operation.h"
#include <afxcontrolbars.h>

const int INIT_0 = 0;
const int INTI_256 = 256;
const int INIT_1K = 1024;
const int INIT_1M = 1024 * 1024;
const int MAX_ITEM_COUNT = 99999999;
