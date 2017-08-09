#pragma once
#include "Compiler.h"

namespace Servers{
namespace Config{
class CConfig : public CWnd, public Compile::SNeedRunObj
{
	DECLARE_DYNAMIC(CConfig)
private:
	int m_nID;											//!< �豸���
	HWND m_hConfigWnd;									//!< ����������ľ��
	PROCESS_INFORMATION m_pi;
	boost::thread* m_ThreadToWati;

public:
	CConfig (int id);
	virtual ~CConfig (){;}
	int GetID(){return m_nID;}
	bool IsCreate(){return m_pi.hProcess != NULL;}		//!< �Ƿ����Config���

public:
	bool InitWnd();										//!< ��ʼ������
	virtual void OnRun();								//!< ��������
	void OnConfigQuit();								//!< ��Config����ر��˳�ʱ

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
}}

