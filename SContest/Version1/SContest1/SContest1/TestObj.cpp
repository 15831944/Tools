#include "stdafx.h"
#include "TestObj.h"
#include "CreateDB.h"
#include "Gbl.h"

void CTestObj::StartTest()
{
	CString strPath = CGbl::GetMe().getExePath();
	strPath += _T("DB.xml");
	CCreateDB db(strPath);
}

