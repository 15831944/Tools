#pragma once
#include "Compiler.h"

namespace Servers{
namespace DXP{
class CServerCtrl : public CWnd, public Compile::SNeedRunObj
{
public:
	enum SEV_TIMER{
		FIND_STATE = 1,			//!< 定时读取服务器运行状态
		RUN_CMDLINE = 2,		//!< 启动服务器后，隔相应时间启动服务
		RUN_SCANCMDLINE = 3,	//!< 启动服务器后，隔相应时间启动扫描
	};

private:
	int m_nTryNum;						//!< 试验的次数
	CString m_strCmdLine;
	CString m_strProjSev;				//!< 记录当前DXP中运行的工程的绝对路径
	UINT m_uiState;						//!< 服务的状态，0未加载工程，1正在运行，2暂停，3没有运行
	HWND m_hServerWnd;					//!< 服务器程序的句柄
	bool m_bLoadingScanFile;			//!< 运行环境是否处于正在加载文档中

public:
	UINT GetState(){return m_uiState;}	//!< 获取服务状态，1正在运行，2暂停，3没有运行
	void SetCmdLine(CString str){m_strCmdLine = str;}
	bool IsLoadingFile(){return m_bLoadingScanFile;}	//!< 获取运行环境是否处于正在加载文档中
	void SetLoadingFile(bool b){m_bLoadingScanFile = b;}//!< 设置运行环境是否处于正在加载文档中

private:
	CServerCtrl(void);
	~CServerCtrl(void);
	bool FindServer();
	void FindState();
	void SetState(UINT state);

	void InitState(COPYDATASTRUCT& pCopyDataStruct);	//!< 解析返回的DXP状态信息
	void InitStateXml(COPYDATASTRUCT& pCopyDataStruct);	//!< 解析返回的DXP描述格式的状态信息

	bool RunServer(CString cmdLine);	//!< 启动运行
	bool IsSameProj();					//!< 判断是不是同一个工程

public:
	static CServerCtrl& GetMe();
	void InitServerMgr(CWnd* pParent);	//!< 初始化
	void CreateServer();				//!< 启动服务环境

	virtual void OnRun();				//!< 启动运行
	void OnRunSev(CString cmdLine);		//!< 启动运行
	void OnStopSev();					//!< 停止运行
	void SendFreshDev(int devID);		//!< 发送强制扫描设备的功能
	void RegistSoft();					//!< 通知DXP打开注册页面

	bool IsINeed(UINT dwData);			//!< 看看是不是自己要的数字
	void OnReceive(COPYDATASTRUCT* pCopyDataStruct);	//!< 接收服务器发回来的消息

protected:
	//{{AFX_MSG(CPLCSocket)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
}}