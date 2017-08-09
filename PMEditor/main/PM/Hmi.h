#pragma once
#include "Compiler.h"

namespace Servers{
namespace HMI{
class CHmi : public CWnd, public Compile::SNeedRunObj
{
private:
	HWND m_hHmiWnd;										//!< ����������ľ��
	bool m_bCreateHmi;									//!< ��HMI����,ǿ�еȴ�һ��ʱ��
	PROCESS_INFORMATION m_pi;
	boost::thread* m_ThreadToWati;

private:
	CHmi(void);
	~CHmi(void);

private:
	bool FindHmi();										//!< �ҵ�Hmi������
	virtual void OnRun();								//!< ����HMI��������
	bool Synchronization();								//!< ͬ������

public:
	static CHmi& GetMe(){static CHmi one;return one;}
	bool IsHmiCreate(){return m_bCreateHmi;}			//!< �鿴HMI�Ƿ�����
	void InitHmiMgr(CWnd* pParent);						//!< ��ʼ������
	bool RunHmiExe();									//!< ����HMI����
	bool IsINeed(UINT dwData);							//!< �����ǲ����Լ�Ҫ������
	void OnReceive(COPYDATASTRUCT* pCopyDataStruct);	//!< ���շ���������������Ϣ
	void OnHmiQuit();									//!< DHMI�˳����¼�
	void SetClose(bool bSave);							//!< �ر�DHMI���

protected:
	//{{AFX_MSG(CPLCSocket)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
}
}