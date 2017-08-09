#pragma once

namespace MVC{
namespace Item{
class CItem;
class CItemDoc;
class CItemGroup
{
private:
	UINT m_uiID;								//!< 组编号
	CString m_strName;							//!< 组名称
	UINT m_uiParentID;							//!< 父组的ID号

public:
	CItemDoc* m_ItemDoc;						//!< 显示变量的文档

public:
	UINT getID(){return m_uiID;}
	CString getName(){return m_strName;}
	UINT getParentID(){return m_uiParentID;}
	void setID(UINT id){m_uiID = id;}
	void setParentID(UINT id){m_uiParentID = id;}
	void setName(CString name){m_strName = name;}

public:
	CItemGroup(CString name, UINT parentid);
	~CItemGroup(void);

	bool Serialize(CArchive& ar);					//!< 保存二进制信息
	bool SerializeXml(TiXmlElement* pNode, bool bRead);		//!< 保存xml格式的编辑信息

	std::list<UINT> getGroupIDList();				//!< 获得子组列表
	std::list<UINT> getMyItem();					//!< 获得子变量列表
	void InitDefault();								//!< 初始化成默认组，就是System组
	bool IsChildGroup(UINT id, bool incMe = false);	//!< 判断此变量组是否是自己的子组，多少层都行，incMe是否包括自己
//	void OnDelete(bool silenced=true);				//!< 当删除自己时，如果表示沉默就不提示用户直接删除
};
}
}