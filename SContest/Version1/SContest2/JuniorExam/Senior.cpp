#include "stdafx.h"
#include "Senior.h"


CSenior::CSenior()
{
}


CSenior::~CSenior()
{
}

void CSenior::InitData(pugi::xml_node node)
{
	for (auto it = node.attributes_begin(); it != node.attributes_end(); it++)
	{
		CString name = it->name();
		if (name == _T("ID"))						m_strID = it->value();
		else if (name == _T("UnifiedEnrollNum"))	m_nUnifiedCount = _ttoi(it->value());
		else if (name == _T("TargetEnrollNum"))		m_nTargetCount = _ttoi(it->value());
	}
}
