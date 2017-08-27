#include "stdafx.h"
#include "Gbl.h"

#include "Junior.h"
#include "JuniorInfo.h"


CJuniorInfo::CJuniorInfo()
	: m_bLoadData(false)
	, m_Juniors(nullptr)
	, m_nJuniorCount(0)
{
}


CJuniorInfo::~CJuniorInfo()
{
	if (m_Juniors != nullptr){
		delete[] m_Juniors;
		m_Juniors = nullptr;
		m_nJuniorCount = 0;
	}
}

void CJuniorInfo::InitData()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(CGbl::GetMe().getExePath() + _T("JuniorSchools.xml"), pugi::parse_default, pugi::encoding_auto);
	if (result)
	{
		m_bLoadData = true;
		pugi::xml_node root = doc.child("JuniorSchools");
		m_nJuniorCount = 0;

		auto children = root.children();
		for (auto it = children.begin(); it != children.end(); it++)
		{
			++m_nJuniorCount;
		}
		if (m_nJuniorCount > 0)
		{
			m_Juniors = new CJunior[m_nJuniorCount];
		}
		int index = 0;
		for (auto it = children.begin(); it != children.end(); it++)
		{
			m_Juniors[index++].InitData(*it);
		}
	}
}