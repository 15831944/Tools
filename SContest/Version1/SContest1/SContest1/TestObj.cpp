#include "stdafx.h"
#include "Gbl.h"
#include "TestObj.h"

#include "CreateDB.h"
#include "CSVFileStream.h"
#include "LoadDB.h"
#include "ThreadWorker.h"
#include "MySort.h"

#include <iostream>
#include <sstream>


void CTestObj::StartTest()
{
	CString strPath = CGbl::GetMe().getExePath();

//	CString strXmlName2 = strPath + _T("DB.xml");
//	CString strCSVName2 = strPath + _T("DB.csv");
//	CLoadDB db2;
//	PrintTime pt21;
//	db2.LoadDB(strXmlName2);
//	pt21.MessageBoxTime();
//	PrintTime pt22;
//	db2.SaveCSV(strCSVName2);
//	pt22.MessageBoxTime();


//	CString strXmlName = strPath + _T("DB_UNICODE.xml");
//	CString strCSVName = strPath + _T("DB_UNICODE.csv");
//	//CCreateDB db(strXmlName);
//	CLoadDB db;
//	//PrintTime pt11;
//	db.LoadDB(strXmlName, pugi::encoding_wchar);
//	//pt11.MessageBoxTime();
//	//PrintTime pt12;
//	db.SaveCSV(strCSVName);
//	//pt12.MessageBoxTime();


	//CThreadWorker t;
	//t.DoWork(strXmlName, strCSVName);
	TestSort();

	//CString strPath1 = _T("C:\\Users\\aliu7\\Desktop\\Text1.csv");
	//CString strPath2 = _T("C:\\Users\\aliu7\\Desktop\\Text2.csv");
	//std::vector<std::list<std::string>> vlStr;
	//int countRow = 10000;
	//int countCol = 20;
	//vlStr.resize(countRow);
	//for (int i = 0; i < countRow; i++)
	//{
	//	auto& lt = vlStr[i];
	//	for (int j = 0; j < countCol; j++)
	//	{
	//		std::stringstream ss;
	//		ss << "value" << j;
	//		lt.push_back(ss.str());
	//	}
	//}
	//
	//PrintTime pt1;
	//CCSVFileStream::WriteCsvData(strPath1, vlStr);
	//pt1.MessageBoxTime();
}

