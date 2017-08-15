#include "stdafx.h"
#include "CreateDB.h"

const int ItemCount = 100000;
const int ItemAttrCount = 10;
const int ChildCount = 10;
const int ChildAttrCount = 4;

CCreateDB::CCreateDB(CString pathname)
	: m_strPN(pathname)
{
	CreateData();
}


CCreateDB::~CCreateDB()
{
}

void CCreateDB::CreateData()
{
	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child(_T("Items"));
	SerializeXml(root);
	bool bSave = doc.save_file(m_strPN);
	ASSERT(bSave);
}

void CCreateDB::SerializeXml(pugi::xml_node root)
{
	CString strTemp;
	for (int iItem = 0; iItem < ItemCount; iItem++)
	{
		pugi::xml_node item = root.append_child(_T("Item"));
		item.append_attribute(_T("ID")).set_value(iItem);

		for (int iItemAttr = 0; iItemAttr < ItemAttrCount; iItemAttr++)
		{
			strTemp.Format(_T("Attr%d"), iItemAttr);
			item.append_attribute(strTemp).set_value(iItemAttr);
		}

		for (int iChild = 0; iChild < ChildCount; iChild++)
		{
			pugi::xml_node child = item.append_child(_T("Child"));
			child.append_attribute(_T("ID")).set_value(iChild);

			for (int iChildAttr = 0; iChildAttr < ChildAttrCount; iChildAttr++)
			{
				strTemp.Format(_T("ChildAttr%d"), iChildAttr);
				child.append_attribute(strTemp).set_value(iChildAttr);
			}
		}
	}
}
