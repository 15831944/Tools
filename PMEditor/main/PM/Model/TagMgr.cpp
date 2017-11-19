#include "StdAfx.h"

#include "Model/TagMgr.h"
#include "Model/TagGroup.h"

#define TAG_NS Model::Item
#define TAG_MGR Model::Item::CTagMgr

TAG_MGR::CTagMgr()
{

}

TAG_MGR::~CTagMgr()
{

}

bool TAG_MGR::GroupExist(const CGuid& id)
{
	return m_mpGroup_ID.find(id) != m_mpGroup_ID.end();
}

bool TAG_MGR::GroupExist(const CString& name)
{
	CString strNameNoCase = name;
	strNameNoCase.MakeLower();
	return m_mpGroup.find(strNameNoCase) != m_mpGroup.end();
}

bool TAG_MGR::AddGroup(const CString& name)
{
	if (GroupExist(name)) return false;	// already exist
	CString strNameNoCase = name;
	strNameNoCase.MakeLower();

	auto group = std::shared_ptr<TAG_NS::CTagGroup>(new TAG_NS::CTagGroup(name));
	m_ltGroup.push_back(group);
	m_mpGroup[strNameNoCase] = group;
	m_mpGroup_ID[group->ID()] = group;
	return true;
}

bool TAG_MGR::RemoveGroup(const CString& name)
{
	if (!GroupExist(name))	return false;
	auto group = GetGroup(name);
	m_ltGroup.remove(group);

	CString strNameNoCase = group->Name();
	strNameNoCase.MakeLower();
	auto giter = m_mpGroup.find(strNameNoCase);
	if (giter != m_mpGroup.end()) m_mpGroup.erase(giter);

	auto giterid = m_mpGroup_ID.find(group->ID());
	if (giterid != m_mpGroup_ID.end()) m_mpGroup_ID.erase(giterid);

	return true;
}

bool TAG_MGR::RemoveGroup(const CGuid& id)
{
	if (!GroupExist(id))	return false;
	auto group = GetGroup(id);
	m_ltGroup.remove(group);

	CString strNameNoCase = group->Name();
	strNameNoCase.MakeLower();
	auto giter = m_mpGroup.find(strNameNoCase);
	if (giter != m_mpGroup.end()) m_mpGroup.erase(giter);

	auto giterid = m_mpGroup_ID.find(group->ID());
	if (giterid != m_mpGroup_ID.end()) m_mpGroup_ID.erase(giterid);

	return true;
}

std::shared_ptr<TAG_NS::CTagGroup> TAG_MGR::GetGroup(const CGuid& id)
{
	if (!GroupExist(id)){ std::shared_ptr<TAG_NS::CTagGroup> g; return g; }
	return m_mpGroup_ID[id];
}

std::shared_ptr<TAG_NS::CTagGroup> TAG_MGR::GetGroup(const CString& name)
{
	if (!GroupExist(name)){ std::shared_ptr<TAG_NS::CTagGroup> g; return g; }
	CString strNameNoCase = name;
	strNameNoCase.MakeLower();
	return m_mpGroup[strNameNoCase];
}
