#pragma once
#include "DTreeCtrl.h"

class CProject;
// class CDTreeCtrl;
// Struct IDTreeOwner;
/*!
	工程管理类
*/
class CProjectMgr
{
private:
	std::shared_ptr<CProject> m_CulProject;				// 当前工程的智能指针
	bool m_bWatch;						// 当前是否处于监控状态

private:
	CProjectMgr(void);
	~CProjectMgr(void);

public:
	bool IsWatch(){return m_bWatch && m_CulProject;}

public:
	std::shared_ptr<CProject> GetProj(){return m_CulProject;}
	static CProjectMgr& GetMe();
	bool IsModify();					// 看看工程是否被改了
	bool NewProject();					// 新建工程的函数入口
	bool OpenProject(CString path = _T(""));	// 打开工程的函数入口，这里需要工程绝对路径名
	bool SaveProject();					// 保存工程的函数入口
	bool BackUpProject();				// 备份工程的函数入口
	bool CloseProject();				// 关闭工程的函数入口
	void EditProjInfo();				// 编辑工程信息
	void SetWatch(const bool b);		// 设置/取消监控

	bool SayWatch();					// 填出监控状态不能操作的提示
	void UpdateTreeView();				// 刷新树
};
