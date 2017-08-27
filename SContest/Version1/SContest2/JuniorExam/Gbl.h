#pragma once
class CGbl
{
private:
	CString m_strExePath;

public:
	static CGbl& GetMe(){ static CGbl one; return one; }
	const CString getExePath(){ return m_strExePath; }

private:
	CGbl();
	~CGbl();
};

