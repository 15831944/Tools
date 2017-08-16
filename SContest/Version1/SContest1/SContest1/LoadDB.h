#pragma once
class CLoadDB
{
private:
	void AnalyzeData(pugi::xml_node root);

public:
	CLoadDB();
	~CLoadDB();
	void LoadDB(CString strPath);
};

