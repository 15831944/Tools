#pragma once
#ifndef _PMSPLASH_H_
#define _PMSPLASH_H_

namespace Dialog{class CPMSplashDlg;}

/*!
	�����¼ʱ�����ֹ�˾��ƷͼƬ����
	����ʵ��ͼƬ�ı���͸��Ч��
	\brief ��¼ҳ��ͼƬ��
	\author ����
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
		���캯��
	*/
	CPMSplash();
	/*!
		��������
	*/
	~CPMSplash(void);
};

#endif //_PMSPLASH_H_