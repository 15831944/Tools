#pragma once
#include "Compiler.h"

namespace Servers{
namespace Config{
class CConfig : public CWnd, public Compile::SNeedRunObj
{
	DECLARE_DYNAMIC(CConfig)
private:
	int m_nID;											//!< 设备编号
	HWND m_hConfigWnd;									//!< 服务器程序的句柄
	PROCESS_INFORMATION m_pi;
	boost::thread* m_ThreadToWati;

public:
	CConfig (int id);
	virtual ~CConfig (){;}
	int GetID(){return m_nID;}
	bool IsCreate(){return m_pi.hProcess != NULL;}		//!< 是否打开了Config软件

public:
	bool InitWnd();										//!< 初始化窗口
	virtual void OnRun();								//!< 启动运行
	void OnConfigQuit();								//!< 当Config软件关闭退出时

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
}}

