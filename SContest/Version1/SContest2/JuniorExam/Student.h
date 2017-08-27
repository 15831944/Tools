#pragma once
class CStudent
{
private:
	CString m_strID;
	CString m_strJuniorID;
	CString m_strTargetSenior;	// 报考的指标高中
	bool m_bTarget;				// 是否是指标生
	bool m_bAdjust;				// 是否服从
	short m_nScore[5];			// 分数，以此是语文，数学，英语，综合

	CString m_strUnified[3];	// 3个统招志愿
	CString m_strTarget[2];		// 2个指标到校统调志愿

public:
	CStudent();
	~CStudent();

	void InitData(pugi::xml_node node);
};

