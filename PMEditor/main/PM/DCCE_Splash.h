#pragma once
#ifndef _DCCE_SPLASH_H_
#define _DCCE_SPLASH_H_

namespace Dialog{class CDCCE_SplashDlg;}

/*!
	�����¼ʱ�����ֹ�˾��ƷͼƬ����
	����ʵ��ͼƬ�ı���͸��Ч��
	\brief ��¼ҳ��ͼƬ��
	\author ����
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
		���캯��
	*/
	CDCCE_Splash();
	/*!
		��������
	*/
	~CDCCE_Splash(void);
};

#endif //_ABHI_SPLASH_H_