#pragma once
#ifndef _PMSPLASH_H_
#define _PMSPLASH_H_

namespace Dialog{class CPMSplashDlg;}

/*!
	负责登录时，闪现公司产品图片的类
	可以实现图片的背景透明效果
	\brief 登录页面图片类
	\author 刘鑫
	\date $LastChangedDate$
*/
class CPMSplash
{
public:
	static void Init(int maxInitNO , BOOL bSilentMode=TRUE);
	static void StepOne(const CString& initInfor);
	static void Hide();
	static bool Visible();
private:
	static Dialog::CPMSplashDlg* m_pSplash;
	/*!
		构造函数
	*/
	CPMSplash();
	/*!
		析构函数
	*/
	~CPMSplash(void);
};

#endif //_PMSPLASH_H_