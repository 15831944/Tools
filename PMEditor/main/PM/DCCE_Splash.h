#pragma once
#ifndef _DCCE_SPLASH_H_
#define _DCCE_SPLASH_H_

namespace Dialog{class CDCCE_SplashDlg;}

/*!
	负责登录时，闪现公司产品图片的类
	可以实现图片的背景透明效果
	\brief 登录页面图片类
	\author 刘鑫
	\date $LastChangedDate$
*/
class CDCCE_Splash
{
public:
	static void Init(int maxInitNO , BOOL bSilentMode=TRUE);
	static void StepOne(const CString& initInfor);
	static void Hide();
	static bool Visible();
private:
	static Dialog::CDCCE_SplashDlg* m_pSplash;
	/*!
		构造函数
	*/
	CDCCE_Splash();
	/*!
		析构函数
	*/
	~CDCCE_Splash(void);
};

#endif //_ABHI_SPLASH_H_