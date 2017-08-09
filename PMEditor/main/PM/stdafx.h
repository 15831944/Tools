// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0410 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0600	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS
#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#include <afxdhtml.h>

#ifndef _WINUSER_
#define _WINUSER_
#endif

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC �׽�����չ
#include <afxhtml.h>

#define _XTP_STATICLINK
#include <XTToolkitPro.h>

//#import <msxml.dll>

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#define rforeach BOOST_REVERSE_FOREACH
#include <boost/thread/thread.hpp>			//!< �߳�
#include <boost/thread/mutex.hpp>			//!< ��
#include <boost/bind.hpp>					//!< �̰߳󶨺���
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
