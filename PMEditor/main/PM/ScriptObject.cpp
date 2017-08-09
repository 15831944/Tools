///////////////////////////////////////////////////////////////////////////////
//	File:		ScriptObject.cpp
//	Version:	1.0
//
//	Author:		Ernest Laurentin
//	E-mail:		elaurentin@sympatico.ca
//
//	This class implements MSScript control
//	It can interface script from resource file or text file
//
//	This code may be used in compiled form in any way you desire. This
//	file may be redistributed unmodified by any means PROVIDING it is
//	not sold for profit without the authors written consent, and
//	providing that this notice and the authors name and all copyright
//	notices remains intact.
//
//	An email letting me know how you are using it would be nice as well.
//
//	This file is provided "as is" with no expressed or implied warranty.
//	The author accepts no liability for any damage/loss of business that
//	this c++ class may cause.
//
//	Version history
//
//	1.0	- Initial release.
//	1.1 - Bug fixes for VC7 and Unicode
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <comutil.h>
#include "ScriptObject.h"


///////////////////////////////////////////////////////////////////////////////
// Construction
CScriptObject::CScriptObject()
{
	CommonConstruct(); // will throw exception if failed
}

CScriptObject::~CScriptObject()
{
	m_pScript = NULL;	// Destroy object- and release
}

///////////////////////////////////////////////////////////////////////////////
// Members

///////////////////////////////////////////////////////////////////////////////
// CommonConstruct
void CScriptObject::CommonConstruct()
{
	HRESULT hr = m_pScript.CreateInstance(__uuidof(ScriptControl));
	_com_util::CheckError( hr ); // will throw an exception if failed

	// will not come here if exception
	_tcscpy(m_szLanguage, LANGUAGE_DEFAULT);
	m_pScript->PutAllowUI( VARIANT_TRUE );
	m_pScript->PutLanguage( _bstr_t(m_szLanguage ) );
}

///////////////////////////////////////////////////////////////////////////////
// SetLanguage : Set current script language
void CScriptObject::SetLanguage(LPCTSTR szLanguage)
{
	_tcscpy(m_szLanguage, szLanguage);

	if (m_pScript != NULL)
	{
		m_pScript->PutLanguage( _bstr_t( szLanguage ) );
		m_pScript->Reset();
		m_FunctionList.clear();
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetNameAt : Get method name at specified index
LPCTSTR CScriptObject::GetMethodNameAt(int index)
{
	if(index<0 && index>=m_FunctionList.size())	return TEXT("");

	stl_string_list::iterator iter = m_FunctionList.begin();
	while(index>0) { iter++;	index--; }
	return (*iter).c_str();
}

///////////////////////////////////////////////////////////////////////////////
// Reset : Reset script control object
void CScriptObject::Reset()
{
	if(m_pScript){
		m_pScript->Reset();
		m_FunctionList.clear();
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetErrorString : Get Script error string
LPCTSTR CScriptObject::GetErrorString()
{
	m_szError[0] = 0;
	if(m_pScript==NULL)		return m_szError;

	try{
		IScriptErrorPtr pError = m_pScript->GetError();
		if(pError != NULL){
			_bstr_t desc  = _bstr_t("Error: ") + pError->GetDescription() + _bstr_t(", ");
					desc += pError->GetText() + _bstr_t("; in line ");
					desc += _bstr_t(pError->GetLine());
			int count = __min(desc.length(), ERROR_DESC_LEN); // string may be truncated...
			_tcsncpy(m_szError, (LPCTSTR) desc, count);
			m_szError[count] = 0;
			pError->Clear();
		}
	}catch(_com_error& e){
		TRACE((LPSTR)(e.Description() +"\n"));
	}
	return m_szError;
}

///////////////////////////////////////////////////////////////////////////////
// GetMethodsName: Get methods name list, 解析函数名字，得到函数名列表
bool CScriptObject::AnalyseMethodsName()
{
	bool bResult = false;
	if (m_pScript==NULL)	return bResult;

	m_FunctionList.clear();
	IScriptProcedureCollectionPtr pIProcedures = m_pScript->GetProcedures();

	try{
		long count = pIProcedures->GetCount();
		for(long index=1; index <= count; index++){
			IScriptProcedurePtr pIProcPtr = pIProcedures->GetItem( _variant_t(index) );
			_bstr_t name = pIProcPtr->GetName();
			m_FunctionList.insert(m_FunctionList.end(), (LPCTSTR) name );
			pIProcPtr = NULL;
		}
		bResult = true;
	}catch(...){}// Just catch the exception, call GetErrorString(), to retreive last error

	pIProcedures = NULL;
	return bResult;
}

///////////////////////////////////////////////////////////////////////////////
// GetScriptFunction
//		Get Script function name, this is useful for script with case sensitive
//		function name.
LPCTSTR CScriptObject::GetScriptFunction(LPCTSTR szName)
{
	stl_string_list::iterator iter = m_FunctionList.begin();
	while(iter != m_FunctionList.end()){
		if ((*iter).compare(szName) == 0)
			return (*iter).c_str();
		iter++;
	}
	return TEXT("");
}

//////////////////////////////////////////////////////////////////////////
// LoadScriptResource:
//		Load a Script resource.  This function loads and insert all functions
//		and procedures to the component.  The script resource may contain comments
//		as well.  Global variables may also be defined. You may want to see the
//		script resource as a Module file in Visual Basic.
//////////////////////////////////////////////////////////////////////////
bool CScriptObject::LoadScriptResource(LPCTSTR lpName, LPCTSTR lpType, HINSTANCE hInstance )
{
	try{
		if (m_pScript){
			HRSRC res = ::FindResource(hInstance, lpName, lpType);
			ASSERT( res != NULL);
			BYTE* pbytes = (BYTE*) LockResource(LoadResource(hInstance, res ));
			ASSERT( pbytes != NULL);
			_bstr_t strCode = (LPCSTR) (pbytes);
			m_pScript->AddCode( strCode );
			AnalyseMethodsName();
			return true;
		}
	}catch(...){}	// Just catch the exception, call GetErrorString(), to retreive last error

	return false;
}

///////////////////////////////////////////////////////////////////////////////
// LoadScript
//		Load a Script File.  This function loads and insert all functions and
//		procedures to the component.  The script file may contain comments as well.
//		Global variables may also be define. You may want to see the script file
//		as a Module file in Visual Basic.
//		The script file is probably a simple text file (ASCII format)
///////////////////////////////////////////////////////////////////////////////
bool CScriptObject::LoadScript(LPCTSTR szFilename)
{
	HANDLE hFile = CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)		return false;

	DWORD dwSize = GetFileSize(hFile, NULL);
	if (0xFFFFFFFF != dwSize)
	{
		BYTE* pbytes = (BYTE*) GlobalAlloc(GPTR, dwSize+1);
		if (pbytes != NULL){
			DWORD dwRead = 0;
			bool bResult = false;
			if (ReadFile(hFile, pbytes, dwSize, &dwRead, NULL)){
				try{
					if (m_pScript){
						_bstr_t strCode = (LPCSTR) (pbytes);
						m_pScript->AddCode( strCode );
						AnalyseMethodsName();
						bResult = true;
					}
				}catch(...){}	// Just catch the exception, call GetErrorString(), to retreive last error

				GlobalFree( (HGLOBAL) pbytes);
				CloseHandle(hFile);
				return bResult;
			}

			GlobalFree( (HGLOBAL) pbytes);
		}
	}

	CloseHandle(hFile);
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// AddScriptText
//		Use this function to add a script function, useful for internal use
//		not global script (resource file).
///////////////////////////////////////////////////////////////////////////////
bool CScriptObject::AddScriptText(LPCTSTR szCode)
{
	if(m_pScript==NULL)		return false;
	try {
		ULONG ref = m_pScript->AddRef();
		_bstr_t strCode = szCode;
		m_pScript->AddCode( strCode );
		AnalyseMethodsName();
		ref = m_pScript->Release();
	}catch(...){	// Just catch the exception, call GetErrorString(), to retreive last error
		ULONG ref = m_pScript->Release();
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// AddScript
//		Use this function to add a script function, useful for internal use
//		not global script (resource file).
///////////////////////////////////////////////////////////////////////////////
bool CScriptObject::AddObject(LPCTSTR szObject,LPDISPATCH Object ,bool AddMembers)
{
	if(m_pScript==NULL)		return false;
	try {
		ULONG ref = m_pScript->AddRef();
		_bstr_t strObject = szObject;
		m_pScript->AddObject( strObject, Object,AddMembers);
		ref = m_pScript->Release();
	}catch(...){	// Just catch the exception, call GetErrorString(), to retreive last error
		ULONG ref = m_pScript->Release();
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// ExecuteStatement
//		Use this function to execute a "Sub routine" - no arguments
///////////////////////////////////////////////////////////////////////////////
bool CScriptObject::ExecuteStatement(LPCTSTR szStatement)
{
	if(m_pScript == NULL)	return false;
	try{
		ULONG ref = m_pScript->AddRef();
		m_pScript->ExecuteStatement( _bstr_t(szStatement) );
		ref = m_pScript->Release();
	}catch(...){	// Just catch the exception, call GetErrorString(), to retreive last error
		ULONG ref = m_pScript->Release();
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// RunProcedure
//		Use this function to run a "Procedure" or "Function" - with arguments
///////////////////////////////////////////////////////////////////////////////
bool CScriptObject::RunProcedure(LPCTSTR szProcName, SAFEARRAY** saParameters, VARIANT* varRet)
{
	// required argument
	ASSERT( saParameters != NULL);
	ASSERT( varRet  != NULL);

	if(m_pScript == NULL)	return false;
	try {
		ULONG ref = m_pScript->AddRef();
		bool  bResult = false;
		_bstr_t szFunc  = GetScriptFunction(szProcName);
		if(szFunc.length() > 0){
			*varRet = m_pScript->Run(szFunc, saParameters);
			bResult = true;
		}
		ref = m_pScript->Release();
		return bResult;
	}catch(...){	// Just catch the exception, call GetErrorString(), to retreive last error
		ULONG ref = m_pScript->Release();
	}
	return false;
}

BOOL CScriptObject::SetTimeout(long lTime)
{
	if(m_pScript==NULL)		return FALSE;
	try {
		ULONG ref = m_pScript->AddRef();
		m_pScript->PutTimeout(lTime);
		ref = m_pScript->Release();
	}catch(...){	// Just catch the exception, call GetErrorString(), to retreive last error
		ULONG ref = m_pScript->Release();
		return FALSE;
	}
	return TRUE;
}


