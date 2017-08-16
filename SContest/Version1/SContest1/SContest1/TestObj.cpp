#include "stdafx.h"
#include "Gbl.h"
#include "TestObj.h"

#include "CreateDB.h"
#include "LoadDB.h"

void CTestObj::StartTest()
{
	CString strPath = CGbl::GetMe().getExePath();
	CString strXmlName = strPath + _T("DB_UNICODE.xml");
	CString strCSVName = strPath + _T("DB.csv");
	//CCreateDB db(strXmlName);
	CLoadDB db;
	db.LoadDB(strXmlName);
	db.SaveCSV(strCSVName);
}

