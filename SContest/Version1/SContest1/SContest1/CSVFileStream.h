#pragma once
class CCSVFileStream
{
private:
	static const std::string WriteCsvData(const std::vector<std::list<std::string>>& vlStr);

public:
	CCSVFileStream();
	~CCSVFileStream();
	static const int WriteCsvData(LPCTSTR lpszFilename, const std::vector<std::list<std::string>>& vlStr);
};

