// MainFrm.h : CMainFrame 类的接口
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
		SHOW_ITEM = 9998,	//!< 显示变量
		PRINT = 9999,		//!< 打印信息
	};
	CMainFrame();
	Servers::DXP::CServerCommer	m_SevCommer;				//!< 与服务器的通信器
	CProgressCtrl	m_wndProgCtrl;							//!< 底边进度条
	CComVariant m_VarArray[VARIABLELEN_MAX];				//!< 建立一个VARIANT的数组，以备以后使用

protected:  // 控件条嵌入成员
	CXTPStatusBar	m_wndStatusBar;							//!< 状态栏
	CToolBar		m_wndToolBar;							//!< 工具条
	CAnimateCtrl	m_wndAnimCtrl;							//!< 状态栏显示动画效果

	HICON			m_hIcon;								//!< 图标
	CXTPDockingPaneManager m_paneManager;					//!< pan管理器
	CXTPTabClientWnd m_MTIClientWnd;						//!< 子窗体管理器
	Space::CSpaceOutput m_SpaceOutput;						//!< 文本输出框
	CXTPOfficeBorder<Space::CSpaceProject>	m_SpaceProject;	//!< 解决方案子pane
	CXTPOfficeBorder<Space::CSpaceItem>	m_SpaceItem;		//!< 变量组子pane
	CXTPOfficeBorder<Space::CSpaceDevice>	m_SpaceDevice;	//!< 设备视图
	bool			m_bLimitTimer;							//!< 是否限时定时器开启

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
	void SetTimeLimit(bool bLimit);				//!< 启动或停止时间限制
	void WaitPower2OpenProject(CString path);	//!< 得带获得权限信息后再打开工程
	bool AskAndProjClose();						//!< 询问并关闭工程

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool CreateToolBar();
	bool CreateStatusBar();
	bool CreateSpaceDevice();
	void FillToStatusBar();						//!< 添加状态栏，图标、进度条、切换按钮、动态图片等
	void CompileRun(bool bRunServer = false);	//!< 启动编译，参数表示编译成功后是否要启动服务器运行该编译文件
	void CompileScan();							//!< 启动编译扫描，编译成功后要启动扫描
	void ProjClose();							//!< 无声无息的关闭工程
	void SetTheme(int index);					//!< 设置窗口样式
	void SetPaneTheme(int index);				//!< 设置Pane样式

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomize();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnProjNew();					//!< 新建工程
	afx_msg void OnProjOpen();					//!< 打开工程
	afx_msg void OnProjSave();					//!< 保存工程
	afx_msg void OnProjBackUp();				//!< 备份工程
	afx_msg void OnProjClose();					//!< 关闭工程
	afx_msg void OnCompileCheck();				//!< 检查
	afx_msg void OnServerRun();					//!< 启动服务器运行
	afx_msg void OnServerStop();				//!< 启动服务器停止
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);		//!< 接收被启动程序传回来的数据
	afx_msg void OnAddDevice();
	afx_msg void OnAddItem();
	afx_msg void OnRename();					//!< 重命名工程名
	afx_msg void OnProjInfo();
	afx_msg void OnSoftSet();
	afx_msg void OnFind();						//!< 查找功能
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowViewProject();			//!< 显示工程试图
	afx_msg void OnShowViewItem();				//!< 显示变量组视图
	afx_msg void OnShowViewDevice();			//!< 显示设备视图
	afx_msg void OnShowViewOutput();			//!< 显示输出栏视图
	afx_msg void OnHmiStart();					//!< 启动HMI运行
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnScanSet();
	afx_msg void OnScanStart();
	afx_msg void OnScanStop();
	afx_msg void OnScanandshow();
	afx_msg void OnRegist();
	afx_msg void OnStartpage();
	afx_msg void OnHelpShow();

public:
	afx_msg void OnUpdateWithProj(CCmdUI *pCmdUI);		//!< 必须工程存在才使能的所有菜单项
	afx_msg void OnUpdateServerRun(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServerStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSoftSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateScanStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateScanStop(CCmdUI *pCmdUI);

public:
	DECLARE_EVENTSINK_MAP()
	void OnDataReady(long nShakeInterval);				//!< 接收事件
	void OnVariableAlarm(int nAlarmType,int nID,VARIANT* varValue);	//!< 变量报警事件
	void OnVariableLag(int nID,int nDevice,VARIANT* varValue);		//!< 
	void OnLoadOver(int nLoadType,int nLoadSize);					//!< 
	void OnDeviceStatus(long nDeviceID,long nDeviceInterface,long nDeviceStatus);		//!< 设备状态改变事件
	void OnBehavior(long lBehaviorID, long lDeviceID, VARIANT& varValue, long lResult);	//!< 行为执行结束事件
	void OnScanBehavior(long devType, long infType, long level, long bevID, long* plAddr, VARIANT& varValue, long lResult);	//!< 扫描行为执行事件
	void OnScan(VARIANT& varValue, long lResult);		//!< 扫描设备事件
};

