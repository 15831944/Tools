#include "StdAfx.h"

#include "Model/Model.h"
#include "Model/TagGroup.h"

#define MODEL Model::CModel

MODEL::CModel()
{
}

void MODEL::OnCreateModel()
{
	m_Root = std::shared_ptr<Model::Item::CTagGroup>(new Model::Item::CTagGroup(_T("Root")));
}

