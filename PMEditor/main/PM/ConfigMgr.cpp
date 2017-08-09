#include "StdAfx.h"
#include "Config.h"
#include "ConfigMgr.h"

using namespace Servers;
using namespace Config;

// ���һ��Config,�������豸ID
bool CConfigMgr::AddConfig(int id)
{
	boost::shared_ptr<CConfig> config = GetConfig(id);
	if (!config){
		config = boost::shared_ptr<CConfig>(new CConfig(id));
		if (!config->InitWnd())	return false;
		m_ltConfig.push_back(config);
	}

	Servers::Compile::CCompiler::GetMe().AddRunObj(config.get());
	Servers::Compile::CCompiler::GetMe().CompileProj();
	return true;
}

// ���һ��Config
boost::shared_ptr<CConfig> CConfigMgr::GetConfig(const int& id)
{
	boost::shared_ptr<CConfig> config, empty;
	foreach (config, m_ltConfig){
		if (config->GetID() == id)
			return config;
	}
	return empty;
}

//!< ��ñ��򿪵�DConfig����
int CConfigMgr::GetConfigOpenCount()
{
	int count = 0;
	boost::shared_ptr<CConfig> config;
	foreach (config, m_ltConfig)
	{
		if (config->IsCreate())
			++count;
	}
	return count;
}