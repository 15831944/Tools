#pragma once
#include "Compiler.h"

namespace Servers{
namespace DXP{
class CServerCtrl : public CWnd, public Compile::SNeedRunObj
{
public:
	enum SEV_TIMER{
		FIND_STATE = 1,			//!< ��ʱ��ȡ����������״̬
		RUN_CMDLINE = 2,		//!< �����������󣬸���Ӧʱ����������
		RUN_SCANCMDLINE = 3,	//!< �����������󣬸���Ӧʱ������ɨ��
	};

private:
	int m_nTryNum;						//!< ����Ĵ���
	CString m_strCmdLine;
	CString m_strProjSev;				//!< ��¼��ǰDXP�����еĹ��̵ľ���·��
	UINT m_uiState;						//!< �����״̬��0δ���ع��̣�1�������У�2��ͣ��3û������
	HWND m_hServerWnd;					//!< ����������ľ��
	bool m_bLoadingScanFile;			//!< ���л����Ƿ������ڼ����ĵ���

public:
	UINT GetState(){return m_uiState;}	//!< ��ȡ����״̬��1�������У�2��ͣ��3û������
	void SetCmdLine(CString str){m_strCmdLine = str;}
	bool IsLoadingFile(){return m_bLoadingScanFile;}	//!< ��ȡ���л����Ƿ������ڼ����ĵ���
	void SetLoadingFile(bool b){m_bLoadingScanFile = b;}//!< �������л����Ƿ������ڼ����ĵ���

private:
	CServerCtrl(void);
	~CServerCtrl(void);
	bool FindServer();
	void FindState();
	void SetState(UINT state);

	void InitState(COPYDATASTRUCT& pCopyDataStruct);	//!< �������ص�DXP״̬��Ϣ
	void InitStateXml(COPYDATASTRUCT& pCopyDataStruct);	//!< �������ص�DXP������ʽ��״̬��Ϣ

	bool RunServer(CString cmdLine);	//!< ��������
	bool IsSameProj();					//!< �ж��ǲ���ͬһ������

public:
	static CServerCtrl& GetMe();
	void InitServerMgr(CWnd* pParent);	//!< ��ʼ��
	void CreateServer();				//!< �������񻷾�

	virtual void OnRun();				//!< ��������
	void OnRunSev(CString cmdLine);		//!< ��������
	void OnStopSev();					//!< ֹͣ����
	void SendFreshDev(int devID);		//!< ����ǿ��ɨ���豸�Ĺ���
	void RegistSoft();					//!< ֪ͨDXP��ע��ҳ��

	bool IsINeed(UINT dwData);			//!< �����ǲ����Լ�Ҫ������
	void OnReceive(COPYDATASTRUCT* pCopyDataStruct);	//!< ���շ���������������Ϣ

protected:
	//{{AFX_MSG(CPLCSocket)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
}}