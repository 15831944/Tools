#include "StdAfx.h"

#include "Model/Tag.h"

#define TAG_NS Model::Item
#define TAG_TAG Model::Item::CTag

TAG_TAG::CTag(const CString& name)
	: m_name(name)
{
	m_id = CGuid::NewGuid();
}

TAG_TAG::~CTag()
{

}

