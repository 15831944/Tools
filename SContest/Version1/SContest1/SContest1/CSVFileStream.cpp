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
	// open CSV file
	std::ofstream _streamToFile(lpszFilename);

	// Loop through each list of string in vector
	bool bFirst = true;
	for (auto& vIt : vlStr)
	{
		// Loop through each string in list
		for (auto& lIt : vIt)
		{
			// Separate this value from previous
			if (!bFirst)
			{
				_streamToFile.put(chCma);
			}

			// convert , and "
			bool bComma = (lIt.find(chCma) != lIt.npos);
			bool bQuote = (lIt.find(chQte) != lIt.npos);
			/// 真的含有,或"的情况
			if (bComma || bQuote)
			{
				_streamToFile.put(chQte);
				if (bQuote)
				{
					for (auto& chIt : lIt)
					{
						// Pairs of quotes interpreted as single quote
						if (chQte == chIt)
						{
							_streamToFile.put(chQte);
						}
						_streamToFile.put(chIt);
					}
				}
				else
				{
					_streamToFile << lIt;
				}
				_streamToFile.put(chQte);
			}
			else
			{
				_streamToFile << lIt;
			}
		}
		/// 换行
		_streamToFile << std::endl;
		bFirst = false;
	}
	return S_OK;
}
