#include "StdAfx.h"

#include "Model/TagGroup.h"
#include "Model/Tag.h"

#define TAG_NS Model::Item
#define TAG_GRP Model::Item::CTagGroup

TAG_GRP::CTagGroup(const CString& name)
	: m_name(name)
{
	m_id = CGuid::NewGuid();
}

TAG_GRP::~CTagGroup()
{

}

bool TAG_GRP::GroupExist(const CGuid& id)
{
	return m_mpGroup_ID.find(id) != m_mpGroup_ID.end();
}

bool TAG_GRP::GroupExist(const CString& name)
{
	CString strNameNoCase = name;
	strNameNoCase.MakeLower();
	return m_mpGroup.find(strNameNoCase) != m_mpGroup.end();
}

bool TAG_GRP::AddGroup(const CString& name)
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

bool TAG_GRP::RemoveGroup(const CString& name)
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

bool TAG_GRP::RemoveGroup(const CGuid& id)
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

std::shared_ptr<TAG_NS::CTagGroup> TAG_GRP::GetGroup(const CGuid& id)
{
	if (!GroupExist(id)){ std::shared_ptr<TAG_NS::CTagGroup> g; return g; }
	return m_mpGroup_ID[id];
}

std::shared_ptr<TAG_NS::CTagGroup> TAG_GRP::GetGroup(const CString& name)
{
	if (!GroupExist(name)){ std::shared_ptr<TAG_NS::CTagGroup> g; return g; }
	CString strNameNoCase = name;
	strNameNoCase.MakeLower();
	return m_mpGroup[strNameNoCase];
}

bool TAG_GRP::TagExist(const CGuid& id)
{
	return m_mpTag_ID.find(id) != m_mpTag_ID.end();
}

bool TAG_GRP::TagExist(const CString& name)
{
	CString strNameNoCase = name;
	strNameNoCase.MakeLower();
	return m_mpTag.find(strNameNoCase) != m_mpTag.end();
}

bool TAG_GRP::AddTag(const CString& name)
{
	if (TagExist(name)) return false;	// already exist
	CString strNameNoCase = name;
	strNameNoCase.MakeLower();

	auto tag = std::shared_ptr<TAG_NS::CTag>(new TAG_NS::CTag(name));
	m_mpTag[strNameNoCase] = tag;
	m_mpTag_ID[tag->ID()] = tag;
	return true;
}

bool TAG_GRP::RemoveTag(const CString& name)
{
	if (!TagExist(name))	return false;
	auto tag = GetTag(name);

	CString strNameNoCase = tag->Name();
	strNameNoCase.MakeLower();
	auto giter = m_mpTag.find(strNameNoCase);
	if (giter != m_mpTag.end()) m_mpTag.erase(giter);

	auto giterid = m_mpTag_ID.find(tag->ID());
	if (giterid != m_mpTag_ID.end()) m_mpTag_ID.erase(giterid);

	return true;
}

bool TAG_GRP::RemoveTag(const CGuid& id)
{
	if (!TagExist(id))	return false;
	auto tag = GetTag(id);

	CString strNameNoCase = tag->Name();
	strNameNoCase.MakeLower();
	auto giter = m_mpTag.find(strNameNoCase);
	if (giter != m_mpTag.end()) m_mpTag.erase(giter);

	auto giterid = m_mpTag_ID.find(tag->ID());
	if (giterid != m_mpTag_ID.end()) m_mpTag_ID.erase(giterid);

	return true;
}

std::shared_ptr<TAG_NS::CTag> TAG_GRP::GetTag(const CGuid& id)
{
	if (!TagExist(id)){ std::shared_ptr<TAG_NS::CTag> t; return t; }
	return m_mpTag_ID[id];
}

std::shared_ptr<TAG_NS::CTag> TAG_GRP::GetTag(const CString& name)
{
	if (!TagExist(name)){ std::shared_ptr<TAG_NS::CTag> t; return t; }
	CString strNameNoCase = name;
	strNameNoCase.MakeLower();
	return m_mpTag[strNameNoCase];
}