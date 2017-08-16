#pragma once
class CLoadDB
{
private:
	std::vector<std::list<std::string>> m_vlData;
	void AnalyzeData(pugi::xml_node root);

public:
	CLoadDB();
	~CLoadDB();
	void LoadDB(CString strPath);
	void SaveCSV(CString strPath);
};

