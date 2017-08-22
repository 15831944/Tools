// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // remove support for MFC controls in dialogs

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>



// TODO: reference additional headers your program requires here
#include <wchar.h>
#include <xstring>
#include <winsock2.h>
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"Version.lib")
#include <direct.h>
#include <shlobj.h>

#include <list>
#include <vector>
#include <map>
#include <hash_map>

#include <memory>
#include <thread>
#include <mutex>
#include <functional>

#include "..\..\pugixml1.8\src\pugixml.hpp"

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
