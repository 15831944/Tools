#pragma once

// CDeviceMapDoc 文档
namespace MVC{
namespace Device{
class CDeviceMapView;
class CDeviceOne;

typedef struct tagDevUndo
{
	boost::shared_ptr<CDeviceOne> m_Device;	//!< 记录被操作的设备的指针，删除的时候用
	UINT m_uiEditType;						//!< 操作类型，1添加，2删除，3修改。
	bool m_bEnd;							//!< 是否是单次操作的结尾，一次操作可以对多个变量，所以有一个结尾标志

	tagDevUndo(){m_uiEditType = 0;		m_bEnd = false;}
	tagDevUndo(UINT type, boost::shared_ptr<CDeviceOne> device){m_uiEditType = type;	m_Device = device;		m_bEnd = false;}
	void SetEnd(bool b = true){m_bEnd = true;}
}SDevUndo, *PSDevUndo;

class CDeviceMapDoc : public CDocument
{
	DECLARE_DYNCREATE(CDeviceMapDoc)
public:
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	enum DevDraw{
		DEV_WIDTH=80,					//!< 设备格的宽度
		DEV_HEIGH=60,					//!< 设备格的高度
		DEV_DIS_X=160,					//!< 设备横向间距
		DEV_DIS_Y=150,					//!< 设备纵向间距
		DEV_MAXNUM=256,					//!< 设备最大数量
		DEV_EDGE=30,					//!< 设备与原点的距离，横纵坐标有相同的距离
		DEV_HOSTLINE_X=25,				//!< 母线的起始x坐标
		DEV_HOSTLINE_Y=25				//!< 母线的起始y坐标
	};
	UINT m_MaxX[DEV_MAXNUM];			//!< 每行最右侧的横坐标值，横向显示时用
	UINT m_MaxY[DEV_MAXNUM];			//!< 每行最下侧的纵坐标值，纵向显示时用

private:
	std::stack< boost::shared_ptr<SDevUndo> > m_stDevUndo;
	std::stack< boost::shared_ptr<SDevUndo> > m_stDevRedo;
	void ClearUndo();
	void ClearRedo();
	void UndoAdd();
	void UndoDel();
	void UndoUpd();
	void RedoAdd();
	void RedoDel();
	void RedoUpd();

public:
	bool IsUndoEmpty(){return m_stDevUndo.empty();}
	bool IsRedoEmpty(){return m_stDevRedo.empty();}
	void OnUndo();
	void OnRedo();
	void AddUndoMember(boost::shared_ptr<SDevUndo> devUndo);
	void SetUndoEnd();

public:
	CDeviceMapDoc();
	virtual ~CDeviceMapDoc();

	void OnCountChildPoint();				//!< 计算设备的坐标
	void OnMoveMaxX(BYTE level,UINT x);		//!< 将该行的横坐标向右移
	CSize GetViewSize();
	CDeviceMapView* GetView();
	void FreshMap();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};
}
}