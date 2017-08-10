#include "stdafx.h"
#include "PMApp.h"
#include "PMSplashDlg.h"
#include "pmsplash.h"


Dialog::CPMSplashDlg* CPMSplash::m_pSplash=NULL;

CPMSplash::CPMSplash()
{
}

CPMSplash::~CPMSplash(void)
{
}

void CPMSplash::Init(int maxInitNO , BOOL bSilentMode)
{
	if (m_pSplash==NULL) {
		m_pSplash = new Dialog::CPMSplashDlg(maxInitNO,bSilentMode);
	}
}

void CPMSplash::StepOne(const CString& initInfor)
{
	if (m_pSplash!=NULL) {
		m_pSplash->TopMost();
		m_pSplash->StepOne(initInfor);
	}
	
}

void CPMSplash::Hide()
{
	if (m_pSplash!=NULL) {
		m_pSplash->Hide();
		m_pSplash=NULL;
	}
	
}

bool CPMSplash::Visible()
{
	return !!m_pSplash;
}