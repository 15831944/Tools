#include "stdafx.h"
#include "Gbl.h"
#include "LoadDB.h"


CLoadDB::CLoadDB()
{
}


CLoadDB::~CLoadDB()
{
}

void CLoadDB::LoadDB(CString strPath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(strPath, pugi::parse_default, pugi::encoding_wchar);
	if (result)
	{
		AnalyzeData(doc.child(_T("Items")));
	}
}

void CLoadDB::AnalyzeData(pugi::xml_node root)
{
	PrintTime pt;
	SYSTEMTIME st1, st2;
	::GetLocalTime(&st1);

	int count = 0, childCount = 0;
	pugi::xml_node item = root.first_child();
	while (item)
	{
		++count;

		pugi::xml_node child = item.first_child();
		while (child)
		{
			++childCount;
			child = child.next_sibling();
		}

		item = item.next_sibling();
	}

	::GetLocalTime(&st2);
	pt.MessageBoxTime();
	CString strTime;
	strTime.Format(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.3d \r\n%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.3d\r\n"),
		st1.wYear, st1.wMonth, st1.wDay, st1.wHour, st1.wMinute, st1.wSecond, st1.wMilliseconds,
		st2.wYear, st2.wMonth, st2.wDay, st2.wHour, st2.wMinute, st2.wSecond, st2.wMilliseconds);
	//pt.MessageBoxTime();
	CString str;
	str.Format(_T("Item count is %d, Child count is %d"), count, childCount);
	MessageBox(NULL, strTime + str, _T("Infi"), MB_OK);
}