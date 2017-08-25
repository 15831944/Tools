#include "stdafx.h"
#include "CSVFileStream.h"
#include <fstream>
#include <algorithm>

CCSVFileStream::CCSVFileStream()
{
}

CCSVFileStream::~CCSVFileStream()
{
}

const TCHAR chQte = _T('"');
const TCHAR chCma = _T(',');

const int CCSVFileStream::WriteCsvData(LPCTSTR lpszFilename, const std::vector<std::list<std::string>>& vlStr)
{
	std::string strStr = CCSVFileStream::WriteCsvData(vlStr);
	std::ofstream _streamToFile(lpszFilename);
	_streamToFile << strStr;
	//_streamToFile.close();
	return S_OK;
}

const std::string CCSVFileStream::WriteCsvData(const std::vector<std::list<std::string>>& vlStr)
{
	std::string str{ "" };
	// Loop through each list of string in vector
	for (auto& vIt : vlStr)
	{
		bool bFirst = true;
		// Loop through each string in list
		for (auto& lIt : vIt)
		{
			// Separate this value from previous
			if (!bFirst)
				str.push_back(chCma);
			bFirst = false;

			// convert , and "
			bool bComma = (lIt.find(chCma) != lIt.npos);	// ,
			bool bQuote = (lIt.find(chQte) != lIt.npos);	// "
			/// 真的含有,或"的情况
			if (bComma || bQuote)
			{
				str.push_back(chQte);	// "
				if (bQuote)
				{
					for (auto& chIt : lIt)
					{
						// Pairs of quotes interpreted as single quote
						if (chQte == chIt)
						{
							str.push_back(chQte);
						}
						str.push_back(chIt);
					}
				}
				else
				{
					//_streamToFile << lIt;
					str.append(lIt);
				}
				str.push_back(chQte);
			}
			else
			{
				str.append(lIt);
			}
		}
		/// 换行
		str.append("\n");
	}
	return str;
}
