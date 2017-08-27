#include "stdafx.h"
#include "Junior.h"

CJunior2Senior::CJunior2Senior()
	: m_nTargetCount(0)
{}

void CJunior2Senior::InitData(pugi::xml_node node)
{
	for (auto it = node.attributes_begin(); it != node.attributes_end(); it++)
	{
		CString name = it->name();
		if (name == _T("ID"))						m_strID = it->value();
		else if (name == _T("EnrollNum"))			m_nTargetCount = _ttoi(it->value());
	}
}

//////////////////////////////////////////////////////////////////////////

CJunior::CJunior()
{
}


CJunior::~CJunior()
{
}

void CJunior::InitData(pugi::xml_node node)
{
	for (auto it = node.attributes_begin(); it != node.attributes_end(); it++)
	{
		CString name = it->name();
		if (name == _T("ID"))						m_strID = it->value();
	}
	m_nSeniorCount = 0;
	auto children = node.children();
	for (auto it = children.begin(); it != children.end(); it++)
	{
		++m_nSeniorCount;
	}
	if (m_nSeniorCount > 0)
	{
		m_arrJ2S = new CJunior2Senior[m_nSeniorCount];
		int index = 0;
		for (auto it = children.begin(); it != children.end(); it++)
		{
			m_arrJ2S[index++].InitData(*it);
		}
	}
}
