#include "stdafx.h"
#include "DXPEditor.h"
#include "DCCE_SplashDlg.h"
#include "dcce_splash.h"


Dialog::CDCCE_SplashDlg* CDCCE_Splash::m_pSplash=NULL;

CDCCE_Splash::CDCCE_Splash()
{
}

CDCCE_Splash::~CDCCE_Splash(void)
{
}

void CDCCE_Splash::Init(int maxInitNO , BOOL bSilentMode)
{
	if (m_pSplash==NULL) {
		m_pSplash = new Dialog::CDCCE_SplashDlg(maxInitNO,bSilentMode);
	}
}

void CDCCE_Splash::StepOne(const CString& initInfor)
{
	if (m_pSplash!=NULL) {
		m_pSplash->TopMost();
		m_pSplash->StepOne(initInfor);
	}
	
}

void CDCCE_Splash::Hide()
{
	if (m_pSplash!=NULL) {
		m_pSplash->Hide();
		m_pSplash=NULL;
	}
	
}

bool CDCCE_Splash::Visible()
{
	return !!m_pSplash;
}