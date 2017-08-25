#pragma once
class CLoadDB
{
private:
	std::vector<std::list<std::string>> m_vlData;
	void AnalyzeData(pugi::xml_node root);

public:
	CLoadDB();
	~CLoadDB();
	void LoadDB(CString strPath, pugi::xml_encoding mode = pugi::encoding_utf8);
	void SaveCSV(CString strPath);
};

