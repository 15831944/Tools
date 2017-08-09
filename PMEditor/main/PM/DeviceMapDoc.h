#pragma once

// CDeviceMapDoc �ĵ�
namespace MVC{
namespace Device{
class CDeviceMapView;
class CDeviceOne;

typedef struct tagDevUndo
{
	boost::shared_ptr<CDeviceOne> m_Device;	//!< ��¼���������豸��ָ�룬ɾ����ʱ����
	UINT m_uiEditType;						//!< �������ͣ�1��ӣ�2ɾ����3�޸ġ�
	bool m_bEnd;							//!< �Ƿ��ǵ��β����Ľ�β��һ�β������ԶԶ��������������һ����β��־

	tagDevUndo(){m_uiEditType = 0;		m_bEnd = false;}
	tagDevUndo(UINT type, boost::shared_ptr<CDeviceOne> device){m_uiEditType = type;	m_Device = device;		m_bEnd = false;}
	void SetEnd(bool b = true){m_bEnd = true;}
}SDevUndo, *PSDevUndo;

class CDeviceMapDoc : public CDocument
{
	DECLARE_DYNCREATE(CDeviceMapDoc)
public:
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	enum DevDraw{
		DEV_WIDTH=80,					//!< �豸��Ŀ��
		DEV_HEIGH=60,					//!< �豸��ĸ߶�
		DEV_DIS_X=160,					//!< �豸������
		DEV_DIS_Y=150,					//!< �豸������
		DEV_MAXNUM=256,					//!< �豸�������
		DEV_EDGE=30,					//!< �豸��ԭ��ľ��룬������������ͬ�ľ���
		DEV_HOSTLINE_X=25,				//!< ĸ�ߵ���ʼx����
		DEV_HOSTLINE_Y=25				//!< ĸ�ߵ���ʼy����
	};
	UINT m_MaxX[DEV_MAXNUM];			//!< ÿ�����Ҳ�ĺ�����ֵ��������ʾʱ��
	UINT m_MaxY[DEV_MAXNUM];			//!< ÿ�����²��������ֵ��������ʾʱ��

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

	void OnCountChildPoint();				//!< �����豸������
	void OnMoveMaxX(BYTE level,UINT x);		//!< �����еĺ�����������
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