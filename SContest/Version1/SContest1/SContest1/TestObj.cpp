#include "stdafx.h"
#include "Gbl.h"
#include "TestObj.h"

#include "CreateDB.h"
#include "LoadDB.h"

void CTestObj::StartTest()
{
	CString strPath = CGbl::GetMe().getExePath();
	strPath += _T("DB_UNICODE.xml");
	//CCreateDB db(strPath);
	CLoadDB db;
	db.LoadDB(strPath);
}

