#include "stdafx.h"
#include "Gbl.h"

#include "Senior.h"
#include "SeniorInfo.h"


CSeniorInfo::CSeniorInfo()
	: m_nSeniorCount(0)
	, m_Seniors(nullptr)
{
}

CSeniorInfo::~CSeniorInfo()
{
	if (m_Seniors != nullptr){
		delete[] m_Seniors;
		m_Seniors = nullptr;
		m_nSeniorCount = 0;
	}
}

void CSeniorInfo::InitData()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(CGbl::GetMe().getExePath() + _T("SeniorSchools.xml"), pugi::parse_default, pugi::encoding_auto);
	if (result)
	{
		pugi::xml_node root = doc.child("SeniorSchools");
		m_nSeniorCount = 0;

		auto children = root.children();
		for (auto it = children.begin(); it != children.end(); it++)
		{
			++m_nSeniorCount;
		}
		if (m_nSeniorCount > 0)
		{
			m_Seniors = new CSenior[m_nSeniorCount];
		}
		int index = 0;
		for (auto it = children.begin(); it != children.end(); it++)
		{
			m_Seniors[index].InitData(*it);
		}
	}
}