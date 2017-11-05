#pragma once
//#include "DataStruct.h"

#include "ItemProperty.h"
#include "PropertyItems.h"

namespace MVC{
namespace Item{
class CItemGroup;
class CItem;
class CScriptItem;
class CItemDoc;

class CItemMgr
{
public:
	std::vector< std::shared_ptr<CItem> > m_vtItem;				//!< 变量数组
	std::map< CString, std::shared_ptr<CItem> > m_mpItem;			//!< 变量表,这个名称用于快速检索
	std::list<std::shared_ptr<CItem> > m_ltItemClipBoard;			//!< 变量的剪贴板
	std::vector< std::shared_ptr<CItemGroup> > m_vtItemGroup;		//!< 变量组数组
	std::list<UINT> m_ltEditItemID;									//!< 当前刚刚被修改的变量ID列表
	CItemDoc* m_pItemDoc;
	CPropertyItem m_ItemProperty;									//!< 变量的属性
	CPropertyItems m_ItemPropertys;									//!< 多个变量的共有属性

private:
	SYSTEMTIME m_EditTime;		//!< 最近修改的事件
	bool m_bModify;				//!< 是否被修改
	CString m_strName;
	CString m_strFileName;
	CString m_strVersion;

public:
	bool IsModify(){return m_bModify;}
	CString getName(){return m_strName;}
	CString getFileName(){return m_strFileName;}
	CString getVersion(){return m_strVersion;}

	void SetModify(){m_bModify = true;}

private:
	CItemMgr(void);
	~CItemMgr(void);

	bool Serialize(CArchive& ar);									//!< 保存二进制信息
	bool SerializeXmlItem(TiXmlElement* pNode, bool bRead);				//!< 保存xml格式的编辑信息
	bool SerializeOut(TiXmlElement* pNode, std::list<UINT> ltItem);	//!< 保存xml格式的变量表导出文件

public:
	static CItemMgr& GetMe();

	CString GetEditTime();
	void OnCreate();
	void OnClose();
	bool OpenItemFile(CString name, CString pathall, CString ver, CString stime);	//!< 打开变量表文件，参数 ： 名称 + 路径 + 版本号，最近修改时间
	void OpenDoc();
	void SaveItemFile();												//!< 保存文件
	void OutItem(std::list<UINT> ltItem);							//!< 导出变量

	std::list<UINT> GetItemIDAll();
	std::list<UINT> GetItemIDMem();
	std::list<UINT> GetItemIDIO();
	std::shared_ptr<CItem> GetItem(ULONG id);					//!< 获得id号变量
	std::shared_ptr<CItem> GetItem(CString name);				//!< 获得名字等于name的第一个变量
	std::shared_ptr<CItem> GetItemFast(CString name);			//!< 获得变量,用快速的获得方式
	std::shared_ptr<CItem> GetFirstItem();					//!< 获得第一个变量
	std::shared_ptr<CItem> GetLastItem();						//!< 获得最后一个变量
	std::shared_ptr<CItemGroup> GetGroup(UINT id);			//!< 获得id号变量组
	std::shared_ptr<CItemGroup> GetGroup(CString name);		//!< 获得名字为name的变量组，变量组不能重名
	bool FindItem(ULONG id);									//!< 查找编号为id的变量
	bool FindGroup(UINT id);									//!< 查找编号为id的变量组
	bool AddGroup(std::shared_ptr<CItemGroup> group,UINT parentid=0);			//!< 添加变量组，这时变量组还没有ID，不能重名
	bool AddItem(std::shared_ptr<CItem> item,int maxID,UINT groupid);//!< 添加变量,这时变量还没有ID，组内变量不能重名
	bool AddItemBack(std::shared_ptr<CItem> item, int maxID, UINT groupid);	//!< 在变量的最后边增加
//	bool AddItem(UINT id, std::shared_ptr<CItem> item, UINT groupid=0, UINT uiAsk=0);	//!< 添加变量，指定ID，如果存在是否询问，0问，1不问覆盖，2问跳过
	void RemoveItemBackEmpty();									//!< 删除结尾的空变量
	void DeleteItem(ULONG id);									//!< 删除编号为id的变量
	void RemoveGroup(UINT groupid);								//!< 移除变量组
	UINT GetItemSize();											//!< 获得变量的真实数量
	UINT GetGroupSize();										//!< 获得变量组的真实数量
	void ExChangeItem(UINT id1, UINT id2);						//!< 交换这两个变量
	void WriteItemValue(UINT id, bool proj, CString value);		//!< 写入某个变量的值，需要通信，proj表示是否是写工程值
	void SetItemWatch(const bool watch);						//!< 改变监控状态
	void ReadItemValue(bool bProj);								//!< 从服务器中读取变量值，true表示读工程值，false表示读IO值
	void ShowItem(UINT id);										//!< 显示出编号为id的变量，id为-1表示显示变量一览表
	int SearchItem(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex = false);	//!< 查找符合该字符串的变量，返回匹配的数量
	CString GetSimilarName(CString name);						//!< 获得一个相近的名字，等于也可以，但保证没有被仍和变量使用的

	void AddEditItem(UINT id);									//!< 添加刚刚被修改的变量
	void SetEditEnd();											//!< 设置这个操作结束了
	void FreshAllGrid();										//!< 刷新所有表格显示
};
}
}
