// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once

#include "SpaceProject.h"
#include "SpaceItem.h"
#include "SpaceDevice.h"
#include "OutputPane.h"
#include "ServerCommer.h"

#define VARIABLELEN_MAX 64000

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	enum{
		TIME_WATCH = 1,
		TIME_SCAN = 2,
		SHOW_ITEM = 9998,	//!< ��ʾ����
		PRINT = 9999,		//!< ��ӡ��Ϣ
	};
	CMainFrame();
	Servers::DXP::CServerCommer	m_SevCommer;				//!< ���������ͨ����
	CProgressCtrl	m_wndProgCtrl;							//!< �ױ߽�����
	CComVariant m_VarArray[VARIABLELEN_MAX];				//!< ����һ��VARIANT�����飬�Ա��Ժ�ʹ��

protected:  // �ؼ���Ƕ���Ա
	CXTPStatusBar	m_wndStatusBar;							//!< ״̬��
	CToolBar		m_wndToolBar;							//!< ������
	CAnimateCtrl	m_wndAnimCtrl;							//!< ״̬����ʾ����Ч��

	HICON			m_hIcon;								//!< ͼ��
	CXTPDockingPaneManager m_paneManager;					//!< pan������
	CXTPTabClientWnd m_MTIClientWnd;						//!< �Ӵ��������
	Space::CSpaceOutput m_SpaceOutput;						//!< �ı������
	CXTPOfficeBorder<Space::CSpaceProject>	m_SpaceProject;	//!< ���������pane
	CXTPOfficeBorder<Space::CSpaceItem>	m_SpaceItem;		//!< ��������pane
	CXTPOfficeBorder<Space::CSpaceDevice>	m_SpaceDevice;	//!< �豸��ͼ
	bool			m_bLimitTimer;							//!< �Ƿ���ʱ��ʱ������

public:
	Space::CSpaceProject* GetSpaceProject(){return &m_SpaceProject;}
	Space::CSpaceItem* GetSpaceItem(){return &m_SpaceItem;}
	Space::CSpaceDevice* GetSpaceDevice(){return &m_SpaceDevice;}
	Space::CSpaceOutput* GetOutPut(){return &m_SpaceOutput;}

public:
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void UpdateTreeView();
	void SetTitle(CString title);
	void InitCommerTime();
	void SetTimeLimit(bool bLimit);				//!< ������ֹͣʱ������
	void WaitPower2OpenProject(CString path);	//!< �ô����Ȩ����Ϣ���ٴ򿪹���
	bool AskAndProjClose();						//!< ѯ�ʲ��رչ���

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool CreateToolBar();
	bool CreateStatusBar();
	bool CreateSpaceDevice();
	void FillToStatusBar();						//!< ���״̬����ͼ�ꡢ���������л���ť����̬ͼƬ��
	void CompileRun(bool bRunServer = false);	//!< �������룬������ʾ����ɹ����Ƿ�Ҫ�������������иñ����ļ�
	void CompileScan();							//!< ��������ɨ�裬����ɹ���Ҫ����ɨ��
	void ProjClose();							//!< ������Ϣ�Ĺرչ���
	void SetTheme(int index);					//!< ���ô�����ʽ
	void SetPaneTheme(int index);				//!< ����Pane��ʽ

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomize();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnProjNew();					//!< �½�����
	afx_msg void OnProjOpen();					//!< �򿪹���
	afx_msg void OnProjSave();					//!< ���湤��
	afx_msg void OnProjBackUp();				//!< ���ݹ���
	afx_msg void OnProjClose();					//!< �رչ���
	afx_msg void OnCompileCheck();				//!< ���
	afx_msg void OnServerRun();					//!< ��������������
	afx_msg void OnServerStop();				//!< ����������ֹͣ
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);		//!< ���ձ��������򴫻���������
	afx_msg void OnAddDevice();
	afx_msg void OnAddItem();
	afx_msg void OnRename();					//!< ������������
	afx_msg void OnProjInfo();
	afx_msg void OnSoftSet();
	afx_msg void OnFind();						//!< ���ҹ���
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowViewProject();			//!< ��ʾ������ͼ
	afx_msg void OnShowViewItem();				//!< ��ʾ��������ͼ
	afx_msg void OnShowViewDevice();			//!< ��ʾ�豸��ͼ
	afx_msg void OnShowViewOutput();			//!< ��ʾ�������ͼ
	afx_msg void OnHmiStart();					//!< ����HMI����
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnScanSet();
	afx_msg void OnScanStart();
	afx_msg void OnScanStop();
	afx_msg void OnScanandshow();
	afx_msg void OnRegist();
	afx_msg void OnStartpage();
	afx_msg void OnHelpShow();

public:
	afx_msg void OnUpdateWithProj(CCmdUI *pCmdUI);		//!< ���빤�̴��ڲ�ʹ�ܵ����в˵���
	afx_msg void OnUpdateServerRun(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServerStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSoftSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateScanStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateScanStop(CCmdUI *pCmdUI);

public:
	DECLARE_EVENTSINK_MAP()
	void OnDataReady(long nShakeInterval);				//!< �����¼�
	void OnVariableAlarm(int nAlarmType,int nID,VARIANT* varValue);	//!< ���������¼�
	void OnVariableLag(int nID,int nDevice,VARIANT* varValue);		//!< 
	void OnLoadOver(int nLoadType,int nLoadSize);					//!< 
	void OnDeviceStatus(long nDeviceID,long nDeviceInterface,long nDeviceStatus);		//!< �豸״̬�ı��¼�
	void OnBehavior(long lBehaviorID, long lDeviceID, VARIANT& varValue, long lResult);	//!< ��Ϊִ�н����¼�
	void OnScanBehavior(long devType, long infType, long level, long bevID, long* plAddr, VARIANT& varValue, long lResult);	//!< ɨ����Ϊִ���¼�
	void OnScan(VARIANT& varValue, long lResult);		//!< ɨ���豸�¼�
};

