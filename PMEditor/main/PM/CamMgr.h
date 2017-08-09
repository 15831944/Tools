#pragma once
#include "PropertyCam.h"

namespace MVC{namespace Camera{
class CCamera;
class CCamDoc;
class CCamMgr
{
public:
	std::vector<boost::shared_ptr<CCamera> > m_vtCam;			//!< 摄像头数组
	std::list<boost::shared_ptr<CCamera> > m_ltItemClipBoard;	//!< 变量的剪贴板
	std::list<UINT> m_ltEditItemID;								//!< 当前刚刚被修改的变量ID列表
	CCamDoc* m_pItemDoc;
	CPropertyCamera m_ItemProperty;

private:
	bool m_bModify;					//!< 是否被修改了
	bool m_bActive;					//!< 是否启动视频服务器
	CString m_strName;
	CString m_strFileName;
	CString m_strVersion;

private:
	CCamMgr(void);
	~CCamMgr(void);

public:
	static CCamMgr& GetMe(){static CCamMgr one; return one;}
	bool IsActive(){return GetItemSize() > 0 ? true : false;}
	bool IsModify(){return m_bModify;}
	CString getName(){return m_strName;}
	CString getFileName(){return _T("Camera.cam");}
	CString getVersion(){return _T("1.0");}

	void SetModify(bool bModify){m_bModify = bModify;}

public:
	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< 保存xml格式的编辑信息
	void OnCreate();
	void OnClose();
	void OpenDoc();
	void SaveFile();											//!< 保存文件
	bool OpenFile(CString name, CString pathall, CString ver);	//!< 打开视频配置文件，参数 ： 名称 + 路径 + 版本号
	boost::shared_ptr<CCamera> GetCam(ULONG id);				//!< 获得id号变量
	boost::shared_ptr<CCamera> GetFirstItem();					//!< 获得第一个变量
	boost::shared_ptr<CCamera> GetLastItem();					//!< 获得最后一个变量
	bool FindItem(ULONG id);									//!< 查找编号为id的变量
	bool AddItem(boost::shared_ptr<CCamera> item);				//!< 添加变量,这时变量还没有ID，组内变量不能重名
	void DeleteItem(ULONG id);									//!< 删除编号为id的变量
	UINT GetItemSize();											//!< 获得变量的真实数量
	void ExChangeItem(UINT id1, UINT id2);						//!< 交换这两个变量
	void ShowItem(UINT id);										//!< 显示出编号为id的变量
	int SearchItem(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex = false);	//!< 查找符合该字符串的变量，返回匹配的数量
	int GetDifferentPort(int nDefPort);							//!< 获得一个不同的端口号

	void AddEditItem(UINT id);									//!< 添加刚刚被修改的变量
	void SetEditEnd();											//!< 设置这个操作结束了
	void FreshAllGrid();										//!< 刷新所有表格显示
};
}}