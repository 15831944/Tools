/************************************************************
* FILENAME : ScriptObject.h
*
* class CScriptObject
*		This class implements MSScript control
*		It can interface script from resource file or text file
*
* INITIAL CODING : Ernest Laurentin (EL)
*************************************************************/

#ifndef _SCRIPT_OBJECT_
#define _SCRIPT_OBJECT_
#include <xstring>
#include <list>

// Add this to your stdafx.h
//#pragma warning( disable: 4786)

// Adjust the following path if required
#import "msscript.ocx" no_namespace

#define LANGUAGE_DEFAULT	_T("VBScript")
#define RT_SCRIPT			_T("SCRIPT")
#define LANGUAGE_NAME_LEN	40
#define ERROR_DESC_LEN		256

using namespace std;

#ifndef _UNICODE
typedef list<string> stl_string_list;
#else
typedef list<wstring> stl_string_list;
#endif

class CScriptObject
{
protected:
	IScriptControlPtr m_pScript;				// The one and only script control
	stl_string_list m_FunctionList;				// Function list
	TCHAR	m_szLanguage[LANGUAGE_NAME_LEN+1];	// Current language
	TCHAR	m_szError[ERROR_DESC_LEN+1];		// Description error

// Construction
public:
	CScriptObject();
	~CScriptObject();

// Function
public:
	LPCTSTR GetLanguage(){return m_szLanguage;}	// GetLanguage : Get current script language
	int  GetMethodsCount() const{return (int)m_FunctionList.size();}

public:
	BOOL SetTimeout(long lTime);
	LPCTSTR GetMethodNameAt(int index);
	LPCTSTR GetErrorString();
	void SetLanguage(LPCTSTR szLanguage);
	void Reset();
	bool LoadScript(LPCTSTR szFilename);
	bool LoadScriptResource(LPCTSTR lpName, LPCTSTR lpType, HINSTANCE hInstance);
	bool AddScriptText(LPCTSTR szCode);
	bool AddObject(LPCTSTR szObject,LPDISPATCH Object,bool AddMembers);
	bool ExecuteStatement(LPCTSTR szStatement);
	bool RunProcedure(LPCTSTR szProcName, SAFEARRAY** saParameters, VARIANT* varRet);

protected:
	bool AnalyseMethodsName();
	void CommonConstruct();
	LPCTSTR GetScriptFunction(LPCTSTR name);
};

#endif // _SCRIPT_OBJECT_


