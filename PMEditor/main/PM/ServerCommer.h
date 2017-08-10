// ServerCommer.h : 由 Microsoft Visual C++ 创建的 ActiveX 控件包装类的声明

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CServerCommer
namespace Servers{
namespace DXP{
class CServerCommer : public CWnd
{
protected:
	DECLARE_DYNCREATE(CServerCommer)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x6301125A, 0x4B83, 0x402E, { 0x93, 0x61, 0x48, 0x29, 0xC6, 0x59, 0x1B, 0x31 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:

// 操作
public:

	long DataInit(long lShakeInterval)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lShakeInterval);
		return result;
	}
	VARIANT Read(long lID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, lID);
		return result;
	}
	long Write(long lID, VARIANT varValue)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lID, &varValue);
		return result;
	}
	VARIANT ReadEx(long lStartID, long lEndID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, lStartID, lEndID);
		return result;
	}
	long WriteEx(long lStartID, long lWriteCount, VARIANT varValue)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lStartID, lWriteCount, &varValue);
		return result;
	}
	VARIANT Execute(long lMethordType, VARIANT varValue, long lDeviceID, long lStartID, long lRWCount)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, lMethordType, &varValue, lDeviceID, lStartID, lRWCount);
		return result;
	}
	long ExecBehavior(long lBehaviorID, VARIANT varValue, long lDeviceID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lBehaviorID, &varValue, lDeviceID);
		return result;
	}
};
}}