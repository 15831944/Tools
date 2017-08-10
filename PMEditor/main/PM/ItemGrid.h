#pragma once
#include "ugctrl.h"
#include "UGCTsarw.h"	// Added by ClassView

// CItemGrid
namespace MVC{
namespace Item{
class CItem;
class CItemGrid : public CUGCtrl
{
public:
	enum{
		MENU_FIND = 2000,
		MENU_REPLACE = 2001,
		MENU_BOOL_ALLCOL = 2002
	};
	enum{
		COL_NAME = 0,					//!< 名称
		COL_TAG = COL_NAME + 1,			//!< 标签
		COL_ID = COL_TAG + 1,			//!< 编号
		COL_SRC = COL_ID + 1,			//!< 数据源类型
		COL_ACCESS = COL_SRC + 1,		//!< 访问权限
		COL_TYPE = COL_ACCESS + 1,		//!< 工程类型
		COL_IOTYPE = COL_TYPE + 1,		//!< IO类型
		COL_VALUE = COL_IOTYPE + 1,		//!< 工程值
		COL_IOVALUE = COL_VALUE + 1,	//!< 原始值
		COL_CONVERT = COL_IOVALUE + 1,	//!< 转换
		COL_GROUP = COL_CONVERT + 1,	//!< 变量组
		COL_DESCRIPT = COL_GROUP + 1,	//!< 备注信息
		COL_SCRIPT = COL_DESCRIPT + 1,	//!< 脚本
		COL_DEVICE = COL_SCRIPT + 1,	//!< 设备
		COL_ADDR = COL_DEVICE + 1,		//!< 地址
		COL_FRESHTIME = COL_ADDR + 1,	//!< 刷新时间
		COL_BIT = COL_FRESHTIME + 1,	//!< 变量报警
		COL_LOLO = COL_BIT + 1,			//!< 下下限
		COL_LOW = COL_LOLO + 1,			//!< 下限
		COL_HIGH = COL_LOW + 1,			//!< 上限
		COL_HIHI = COL_HIGH + 1,		//!< 上上限
		COL_AIM = COL_HIHI + 1,			//!< 目标
		COL_SHIFT = COL_AIM + 1,		//!< 变化量
		COL_MODBUS485 = COL_SHIFT + 1,	//!< Modbus485
		COL_REV_DB = COL_MODBUS485 + 1,	//!< 保留历史
	};
	UINT m_uiShowType;				//!< 显示类型，0所有变量，1内存，2IO，3变量分组，4按设备显示
	UINT m_uiCulGroupID;			//!< 当先显示的组号，只在 m_uiShowType = 3 时有效

private:
	bool m_bWatch;					//!< 变量表是否监控
	BOOL m_bSortType;				//!< 记录当前列的排序方向
	int m_iArrowIndex;				//!< 排序‘三角号’小标志
	int m_iSortCol;
//	static CString m_strSortCol;
	CUGSortArrowType m_sortArrow;
	CFont m_ItemFont,m_HeadFont;
	UINT m_uiCulDevID;				//!< 当先显示的设备号，只在 m_uiShowType = 4 时有效
	int m_nProjCol;					//!< 工程值所在的列，为了刷数据方便，所以要先记录这两列
	int m_nIOCol;					//!< IO值所在的列

	std::map<CString, UINT> m_mpValType;
	std::map<UINT, CString> m_mpColName;	//!< 记录了列的名字

	bool m_bHoldToClone;			//!< 通过鼠标按下进行多重复制的标志
	int m_nHoldToCloneID;			//!< 等待克隆的变量ID
	int m_nHoldToCloneRow;			//!< 等待克隆的变量在第几行
	int m_nHoleToCloneCurCount;		//!< 等待克隆的变量的当前数量

public:
	UINT GetGroupID(){return m_uiCulGroupID;}

private:
	friend class CItemView;
	bool IsGridWatch(){return m_bWatch;}
	void SetGridWatch(bool b){m_bWatch = b;}
	static bool SortItem(UINT& lth, UINT& rth);	//!< 对该两个ID的变量进行排序

private:
	void InitCol();							//!< 初始化列标题
	void InitAddRow();						//!< 初始化默认“添加变量”一项
	void SortGrid();						//!< 排序表格
	void SetMyCell(int nCol, long lRow, CUGCell* cell, UINT id, bool bAlarm = false);

	void WriteValue();						//!< 写变量的实时值
	void EditItem(std::shared_ptr<CItem> item);					//!< 编辑变量基本属性
	void EditItems(std::list<std::shared_ptr<CItem> > ltItem);	//!< 统一编辑变量基本属性
	void ItemRemove(std::list<std::shared_ptr<CItem> > ltItem);	//!< 删除变量


	CString GetFloatStr(float flt);			//!< 获得浮点字符串

public:
	CItemGrid();
	~CItemGrid();
	void OnEditEnd();						//!< 编辑结束，统一处理
	void InitAll();
	bool InitItemOne(UINT id, UINT row);	//!< 初始化其中一个变量，修改也用这个
	void InitItemValue(UINT id, UINT row, CUGCell* cell);			//!< 刷新变量值
	void ShowItem(std::list<UINT> ltItemID);//!< 显示指定的变量
	void SelectRowAt(long row);				//!< 选中某一行
	void SelectAllItem();					//!< 选中所有变量
	void OutItemAll();						//!< 导出所有变量
	void OutItemCurrent();					//!< 导出当前所有变量
	void OutItemSelect();					//!< 导出所有选中变量
	void FreshData();						//!< 刷新一遍变量数值
	void ReadItemValue(bool bProj);			//!< 获取变量值

	void ItemEditAll();						//!< 统一修改变量的属性
	void ItemEdit();						//!< 修改变量
	std::shared_ptr<CItem> AddNewItem();	//!< 增加新变量
	void ItemClone();						//!< 变量克隆
	void ItemUp();
	void ItemDown();
	void ItemRemove(bool bAsk = true);		//!< 移除变量，参数表示是否要询问一下用户，剪切时候要删除，就不需要通知用户
	void ItemCopy();						//!< 变量拷贝
	void ItemCut();							//!< 变量剪切
	void ItemPaste();						//!< 变量粘贴

	long GetItemRow(UINT id);				//!< 找到变量所在的行，如果没找到，返回-1
	UINT GetRowItem(long row);				//!< 找到该行所显示的变量id，如果没找到，返回-1
	void RedrawGrid();						//!< 重新绘制表格，从0开始绘制
	void DeleteRowAll();					//!< 删除所有行
	void DeleteColAll();					//!< 删除所有列

	void FreshItemRows(std::list<UINT> ltItemID);	//!< 刷新这些变量

	void OnLButtonDown(int col, long row, RECT *rect, POINT *point, BOOL processed);		//!< 鼠标左键按下
	void OnLButtonUp(int col, long row, RECT *rect, POINT *point, BOOL processed);			//!< 鼠标左键抬起

	void OnDrawFocusBackRect(CDC *dc);														//!< 重绘选中框，使其更醒目
	void OnDrawFocusHodeText(CDC *dc);														//!< 重绘选中框，使其更醒目

	DECLARE_MESSAGE_MAP()
	//***** Over-ridable Notify Functions *****
	virtual void OnSetup();
	virtual void OnTH_LClicked( int iCol, long lRow, int iUpdn, RECT* pRect, POINT* pPoint, BOOL bProcessed = 0 );
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);		//!< 双击某一行
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);		//!< 鼠标左键点击
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);		//!< 鼠标右键点击
	virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed);		//!< 鼠标移动事件
	virtual void OnMouseMoveOver(int col,long row,BOOL bUp);								//!< 鼠标按住移动超过屏幕的事件
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);										//!< 按下键盘键
	virtual int  OnMenuStart(int iCol, long lRow, int iSection);							//!< 
	virtual int OnSortEvaluate(CUGCell* pCell1, CUGCell* pCell2, int iFlags);				//!< 
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);										//!< 重绘选中框，使其更醒目
	virtual void OnDrawBackBitmap(CDC *dc);													//!< 画背景
	virtual void OnDrawBackBitmap(CDC *dc, int l ,int t, int r, int b);						//!< 画背景
	virtual void OnMenuCommand( int iCol, long lRow, int iSection, int iItem );				//!< 接收右键菜单的消息
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
}
}
