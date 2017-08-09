#pragma once
#include "Compiler.h"

namespace Servers{
namespace HMI{
class CHmi : public CWnd, public Compile::SNeedRunObj
{
private:
	HWND m_hHmiWnd;										//!< 服务器程序的句柄
	bool m_bCreateHmi;									//!< 打开HMI进行,强行等待一段时间
	PROCESS_INFORMATION m_pi;
	boost::thread* m_ThreadToWati;

private:
	CHmi(void);
	~CHmi(void);

private:
	bool FindHmi();										//!< 找到Hmi窗体句柄
	virtual void OnRun();								//!< 启动HMI程序运行
	bool Synchronization();								//!< 同步工程

public:
	static CHmi& GetMe(){static CHmi one;return one;}
	bool IsHmiCreate(){return m_bCreateHmi;}			//!< 查看HMI是否启动
	void InitHmiMgr(CWnd* pParent);						//!< 初始化窗体
	bool RunHmiExe();									//!< 启动HMI程序
	bool IsINeed(UINT dwData);							//!< 看看是不是自己要的数字
	void OnReceive(COPYDATASTRUCT* pCopyDataStruct);	//!< 接收服务器发回来的消息
	void OnHmiQuit();									//!< DHMI退出的事件
	void SetClose(bool bSave);							//!< 关闭DHMI软件

protected:
	//{{AFX_MSG(CPLCSocket)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
}
}