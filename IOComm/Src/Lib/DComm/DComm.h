// DComm.h : DComm DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Comm.h"


// CDCommApp
// �йش���ʵ�ֵ���Ϣ������� DComm.cpp
//

class CDCommApp : public CWinApp
{
public:
	CDCommApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
