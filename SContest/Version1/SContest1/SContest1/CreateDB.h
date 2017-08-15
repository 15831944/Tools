#pragma once
#include "PugiXml\pugixml.hpp"
class CCreateDB
{
private:
	CString m_strPN;
	void CreateData();
	void SerializeXml(pugi::xml_node root);

public:
	CCreateDB(CString pathname);
	~CCreateDB();
};

