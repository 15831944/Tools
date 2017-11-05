#pragma once

namespace MVC{namespace Item{class CItemDoc;}}
namespace Tool{class CDTreeCtrl;}

class CProject
{
private:
	bool m_bCompiled;			// 是否被编译过
	bool m_bModify;				// 是否被修改过
	int m_nProjectType;			// 工程类型：0-本地/Web工程；1-跨平台工程
	CString m_strFileName;		// 工程文件名，不包含扩展名,"projOne"
	CString m_strProjName;		// 工程名, "工程x"
	CString m_strPath;			// 工程路径，最后带'\'，"C:\Proj\"
	CString m_strAuthor;		// 作者名
	CString m_strDescription;	// 工程备注信息
	CString m_strVersion;		// 工程版本号
	SYSTEMTIME m_ctBuildTime;	// 工程创建时间
	SYSTEMTIME m_ctUpdateTime;	// 工程修改时间

	HTREEITEM m_hProjectItem;	// 工程在树中的节点
	HTREEITEM m_hItemItem;		// 变量在树中的节点

public:
	bool IsCompiled(){return m_bCompiled;}
	CString GetProjFileName(){return m_strFileName;}
	CString GetProjName(){return m_strProjName;}
	CString GetPath(){return m_strPath;}
	CString GetAuthor(){return m_strAuthor;}
	CString GetDescription(){return m_strDescription;}
	CString GetVersion(){return m_strVersion;}

	void SetModify(bool b){m_bModify = b;}
	void SetCompiled(bool b){m_bCompiled = b;}
	void SetName(CString name){m_strFileName = name;}
	void SetPath(CString path){m_strPath = path;}
	void SetAuthor(CString auth){m_strAuthor = auth;}
	void SetDescription(CString desc){m_strDescription = desc;}
	void SetVersion(CString vers){m_strVersion = vers;}

private:
	void ReName(CString nameNew);										// 将工程定义为这个名字
	bool SerializeXml(TiXmlElement* pNode,bool bRead=true);				// 读写工程文件节点

public:
	CProject();
	CProject(CString name,CString path,CString author,CString description,CString version);
	CProject(int type, CString name, CString path, CString author, CString description, CString version);
	~CProject(void);

	CString GetWholePathName();											// 获得文件的全路径路径

	bool IsModify();
	bool CreateProject();												// 创建工程的内部信息
	bool OnClose();														// 关闭工程时要处理的事情
	bool OpenProject(CString title, CString name, CString path);		// 打开一个工程
	bool SaveProject();													// 保存本工程
	bool BackUpProject(CString name,CString pathname);					// 备份工程到别的地方
	bool OnReName();													// 工程要重命名了
	void ShowInfo();													// 显示自己的属性对话框，让用户修改

	void OnTreeDblClick(CTreeCtrl* treeCtrl, HTREEITEM item);			// 响应双击树的事件
	void OnTreeRClick(CTreeCtrl* treeCtrl, HTREEITEM item,CPoint point);// 响应右键鼠标事件
	void OnTreeEnter(CTreeCtrl* treeCtrl, HTREEITEM item);				// 响应键盘回车事件
	void UpdateProjView(Tool::CDTreeCtrl& pTreeCtrl);					// 刷新工程树
	void OnHelpInfo(HTREEITEM hItem);									// 点击F1帮助
};
