#pragma once
#include "ugctrl.h"
#include "UGCTsarw.h"	// Added by ClassView

// CCamGrid
namespace MVC{
namespace Camera{
class CCamera;
class CCamGrid : public CUGCtrl
{
public:
	enum{
		MENU_FIND = 2000,
		MENU_REPLACE = 2001,
		MENU_BOOL_ALLCOL = 2002
	};
	enum{
		COL_ID = 0,									//!< 说明
		COL_TAG = COL_ID + 1,						//!< 编号
		COL_RECORD = COL_TAG + 1,					//!< 是否启动录制功能
		//COL_AUTOSAVE = COL_RECORD + 1,				//!< 是否自动保存视频文件
		COL_PATH = COL_RECORD + 1,					//!< 视频文件存储位置,最后带'\',默认我的文档
		COL_ENCODE = COL_PATH + 1,					//!< 视频编码格式,目前只能选WMV2
		COL_DISTINGUISH = COL_ENCODE + 1,			//!< 录制分辨率,160*120,176*144,......,720*480,默认320*240
		COL_FPS = COL_DISTINGUISH + 1,				//!< 录制帧速率29.97,15,10,5,1,0.5 fps,默认1fps
		COL_BROAD = COL_FPS + 1,					//!< 是否启动网络广播
		COL_PORT = COL_BROAD + 1,					//!< 广播端口号,8080
		COL_MAXCLIENT = COL_PORT + 1,				//!< 最大连接数,默认5,范围0~50
		COL_BROADENCODE = COL_MAXCLIENT + 1,		//!< 广播视频编码格式,目前只能选WMV2
		COL_BROADDISTINGUISH = COL_BROADENCODE + 1,	//!< 广播分辨率,160*120,176*144,......,720*480
		COL_BROADFPS = COL_BROADDISTINGUISH + 1,	//!< 广播帧速率29.97,15,10,5,1,0.5 fps
	};

private:
	bool m_bWatch;					//!< 变量表是否监控
	BOOL m_bSortType;				//!< 记录当前列的排序方向
	int m_iArrowIndex;				//!< 排序‘三角号’小标志
	int m_iSortCol;
	CUGSortArrowType m_sortArrow;
	CFont m_ItemFont, m_HeadFont;
	std::map<UINT, CString> m_mpColName;	//!< 记录了列的名字

private:
	friend class CCamView;
	bool IsGridWatch(){return m_bWatch;}
	void SetGridWatch(bool b){m_bWatch = b;}
	static bool SortItem(UINT& lth, UINT& rth);	//!< 对该两个ID的变量进行排序

private:
	void InitCol();							//!< 初始化列标题
	void InitAddRow();						//!< 初始化默认“添加变量”一项
	void SortGrid();						//!< 排序表格
	void SetMyCell(int nCol, long lRow, CUGCell* cell, UINT id);

	void EditItem(std::shared_ptr<CCamera> item);					//!< 编辑变量基本属性
//	void EditItems(std::list<std::shared_ptr<CCamera> > ltItem);	//!< 统一编辑变量基本属性
	void ItemRemove(std::list<std::shared_ptr<CCamera> > ltItem);	//!< 删除变量

	CString GetFloatStr(float flt);			//!< 获得浮点字符串

public:
	CCamGrid();
	~CCamGrid();
	void OnEditEnd();						//!< 编辑结束，统一处理
	void InitAll();
	bool InitItemOne(UINT id, UINT row);	//!< 初始化其中一个变量，修改也用这个
	void ShowItem(std::list<UINT> ltItemID);//!< 显示指定的变量
	void SelectRowAt(long row);				//!< 选中某一行
	void SelectAllItem();					//!< 选中所有变量

	//void ItemEditAll();						//!< 统一修改变量的属性
	void ItemEdit();						//!< 修改变量
	std::shared_ptr<CCamera> AddNewItem();	//!< 增加新变量
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

	DECLARE_MESSAGE_MAP()
	//***** Over-ridable Notify Functions *****
	virtual void OnSetup();
	virtual void OnTH_LClicked( int iCol, long lRow, int iUpdn, RECT* pRect, POINT* pPoint, BOOL bProcessed = 0 );
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);		//!< 双击某一行
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);		//!< 鼠标左键点击
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);		//!< 鼠标右键点击
	virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed);		//!< 鼠标移动事件
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);										//!< 按下键盘键
	virtual int  OnMenuStart(int iCol, long lRow, int iSection);							//!< 
	virtual int OnSortEvaluate(CUGCell* pCell1, CUGCell* pCell2, int iFlags);				//!< 
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);										//!< 重绘选中框，使其更醒目
	virtual void OnMenuCommand( int iCol, long lRow, int iSection, int iItem );				//!< 接收右键菜单的消息
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
}
}
