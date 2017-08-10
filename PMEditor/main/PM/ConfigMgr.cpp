#include "StdAfx.h"
#include "Config.h"
#include "ConfigMgr.h"

using namespace Servers;
using namespace Config;

// 添加一个Config,参数是设备ID
bool CConfigMgr::AddConfig(int id)
{
	std::shared_ptr<CConfig> config = GetConfig(id);
	if (!config){
		config = std::shared_ptr<CConfig>(new CConfig(id));
		if (!config->InitWnd())	return false;
		m_ltConfig.push_back(config);
	}

	Servers::Compile::CCompiler::GetMe().AddRunObj(config.get());
	Servers::Compile::CCompiler::GetMe().CompileProj();
	return true;
}

// 获得一个Config
std::shared_ptr<CConfig> CConfigMgr::GetConfig(const int& id)
{
	std::shared_ptr<CConfig> empty;
	for (auto config : m_ltConfig){
		if (config->GetID() == id)
			return config;
	}
	return empty;
}

//!< 获得被打开的DConfig数量
int CConfigMgr::GetConfigOpenCount()
{
	int count = 0;
	//std::shared_ptr<CConfig> config;
	for (auto config : m_ltConfig)
	{
		if (config->IsCreate())
			++count;
	}
	return count;
}