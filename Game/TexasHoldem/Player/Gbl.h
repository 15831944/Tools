#pragma once
class CGbl
{
public:
	CGbl();
	~CGbl();
	static CString UnEncrypt(CString strIn);
	static CString GetAppPath();	// Last c is '\'
	static void SpliteBy(CString text, CString rule, std::vector<CString>& vtStr);
};

