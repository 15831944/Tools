#pragma once
class CCSVFileStream
{
public:
	CCSVFileStream();
	~CCSVFileStream();

	static const int WriteCsvData(LPCTSTR lpszFilename, const std::vector<std::list<std::string>>& vlStr);
};

