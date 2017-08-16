#include "stdafx.h"
#include "Gbl.h"
#include "LoadDB.h"
#include "CSVFileStream.h"

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

std::string TCHAR2STRING(CString cstr)
{
	//int iLen = ::WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	//char* chRtn = new char[iLen*sizeof(char)];
	//::WideCharToMultiByte(CP_ACP, 0, wstr, -1, chRtn, iLen, NULL, NULL);
	//std::string str(chRtn);
	char *chr = new char[cstr.GetLength() + 1];
	WideCharToMultiByte(CP_ACP, 0, cstr.GetBuffer(), -1, chr, cstr.GetLength() + 1, NULL, NULL);
	std::string str = chr;
	return str;
}

void CLoadDB::AnalyzeData(pugi::xml_node root)
{
	PrintTime pt;
	SYSTEMTIME st1, st2;
	::GetLocalTime(&st1);

	int count = 0, childCount = 0;
	pugi::xml_node item = root.first_child();
	m_vlData.clear();
	while (item)
	{
		++count;
		pugi::xml_node child = item.first_child();
		while (child)
		{
			++childCount;
			pugi::xml_attribute attr = child.first_attribute();
			while (attr)
			{
				CString strValue = attr.value();
				attr = attr.next_attribute();
			}
			child = child.next_sibling();
		}
		item = item.next_sibling();
	}

	int idx = 0;
	item = root.first_child();
	m_vlData.resize(childCount);
	while (item)
	{
		pugi::xml_node child = item.first_child();
		while (child)
		{
			++childCount;
			pugi::xml_attribute attr = child.first_attribute();
			std::list<std::string>& ltData = m_vlData[idx];
			while (attr)
			{
				CString strValue = attr.value();
#ifdef _UNICODE
				ltData.push_back(TCHAR2STRING(strValue));
#else
				ltData.push_back(attr.value());
#endif // _UNICODE
				attr = attr.next_attribute();
			}
			child = child.next_sibling();
		}
		++idx;
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

void CLoadDB::SaveCSV(CString strPath)
{
	CCSVFileStream::WriteCsvData(strPath, m_vlData);
}
