
#include "pugixml.hpp"

//cfg_file XML文件名
//cfg 保存的参数结构体
int GetCfg(const char *cfg_file, DataServerCfg& cfg)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(cfg_file);
    if (result)
    {
        pugi::xml_node cfg_node = doc.child("QDadaServerCfg");

        std::string str_host = cfg_node.child_value("RouterHost");
        std::string str_port = cfg_node.child_value("RouterPort");

        std::vector<std::string> vecSegTag;
        boost::split(vecSegTag, str_port, boost::is_any_of(","));
        if (vecSegTag.size() != 3)
        {
            return QDATA_CONFIG_ERROR;
        }

        cfg.pull = "tcp://" + str_host + ":" + vecSegTag[0];
        cfg.reply = "tcp://" + str_host + ":" + vecSegTag[1];
        cfg.push = "tcp://" + str_host + ":" + vecSegTag[2];

        cfg.mem_size = atoi(cfg_node.child_value("MemPool"));
        pugi::xml_node plugin_node = cfg_node.child("Plugin");
        cfg.plugin_file = plugin_node.attribute("file").as_string();
        for (pugi::xml_node para = plugin_node.first_child(); para; para = para.next_sibling())
        {
            cfg.plugin_paras[para.attribute("name").as_string()] = para.attribute("value").as_string();
        }
        if (cfg.pull.empty() || cfg.push.empty() ||cfg.plugin_file.empty() || cfg.mem_size <= 0)
        {
            LOG_ERROR("Get cfg error");
            return QDATA_CONFIG_ERROR;
        }
    }
    else
    {
        LOG_ERROR("error: %s",result.description());
        return QDATA_CONFIG_ERROR;
    }
    return 0;
}