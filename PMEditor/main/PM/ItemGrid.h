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
		COL_NAME = 0,					//!< ����
		COL_TAG = COL_NAME + 1,			//!< ��ǩ
		COL_ID = COL_TAG + 1,			//!< ���
		COL_SRC = COL_ID + 1,			//!< ����Դ����
		COL_ACCESS = COL_SRC + 1,		//!< ����Ȩ��
		COL_TYPE = COL_ACCESS + 1,		//!< ��������
		COL_IOTYPE = COL_TYPE + 1,		//!< IO����
		COL_VALUE = COL_IOTYPE + 1,		//!< ����ֵ
		COL_IOVALUE = COL_VALUE + 1,	//!< ԭʼֵ
		COL_CONVERT = COL_IOVALUE + 1,	//!< ת��
		COL_GROUP = COL_CONVERT + 1,	//!< ������
		COL_DESCRIPT = COL_GROUP + 1,	//!< ��ע��Ϣ
		COL_SCRIPT = COL_DESCRIPT + 1,	//!< �ű�
		COL_DEVICE = COL_SCRIPT + 1,	//!< �豸
		COL_ADDR = COL_DEVICE + 1,		//!< ��ַ
		COL_FRESHTIME = COL_ADDR + 1,	//!< ˢ��ʱ��
		COL_BIT = COL_FRESHTIME + 1,	//!< ��������
		COL_LOLO = COL_BIT + 1,			//!< ������
		COL_LOW = COL_LOLO + 1,			//!< ����
		COL_HIGH = COL_LOW + 1,			//!< ����
		COL_HIHI = COL_HIGH + 1,		//!< ������
		COL_AIM = COL_HIHI + 1,			//!< Ŀ��
		COL_SHIFT = COL_AIM + 1,		//!< �仯��
		COL_MODBUS485 = COL_SHIFT + 1,	//!< Modbus485
		COL_REV_DB = COL_MODBUS485 + 1,	//!< ������ʷ
	};
	UINT m_uiShowType;				//!< ��ʾ���ͣ�0���б�����1�ڴ棬2IO��3�������飬4���豸��ʾ
	UINT m_uiCulGroupID;			//!< ������ʾ����ţ�ֻ�� m_uiShowType = 3 ʱ��Ч

private:
	bool m_bWatch;					//!< �������Ƿ���
	BOOL m_bSortType;				//!< ��¼��ǰ�е�������
	int m_iArrowIndex;				//!< �������Ǻš�С��־
	int m_iSortCol;
//	static CString m_strSortCol;
	CUGSortArrowType m_sortArrow;
	CFont m_ItemFont,m_HeadFont;
	UINT m_uiCulDevID;				//!< ������ʾ���豸�ţ�ֻ�� m_uiShowType = 4 ʱ��Ч
	int m_nProjCol;					//!< ����ֵ���ڵ��У�Ϊ��ˢ���ݷ��㣬����Ҫ�ȼ�¼������
	int m_nIOCol;					//!< IOֵ���ڵ���

	std::map<CString, UINT> m_mpValType;
	std::map<UINT, CString> m_mpColName;	//!< ��¼���е�����

	bool m_bHoldToClone;			//!< ͨ����갴�½��ж��ظ��Ƶı�־
	int m_nHoldToCloneID;			//!< �ȴ���¡�ı���ID
	int m_nHoldToCloneRow;			//!< �ȴ���¡�ı����ڵڼ���
	int m_nHoleToCloneCurCount;		//!< �ȴ���¡�ı����ĵ�ǰ����

public:
	UINT GetGroupID(){return m_uiCulGroupID;}

private:
	friend class CItemView;
	bool IsGridWatch(){return m_bWatch;}
	void SetGridWatch(bool b){m_bWatch = b;}
	static bool SortItem(UINT& lth, UINT& rth);	//!< �Ը�����ID�ı�����������

private:
	void InitCol();							//!< ��ʼ���б���
	void InitAddRow();						//!< ��ʼ��Ĭ�ϡ���ӱ�����һ��
	void SortGrid();						//!< ������
	void SetMyCell(int nCol, long lRow, CUGCell* cell, UINT id, bool bAlarm = false);

	void WriteValue();						//!< д������ʵʱֵ
	void EditItem(std::shared_ptr<CItem> item);					//!< �༭������������
	void EditItems(std::list<std::shared_ptr<CItem> > ltItem);	//!< ͳһ�༭������������
	void ItemRemove(std::list<std::shared_ptr<CItem> > ltItem);	//!< ɾ������


	CString GetFloatStr(float flt);			//!< ��ø����ַ���

public:
	CItemGrid();
	~CItemGrid();
	void OnEditEnd();						//!< �༭������ͳһ����
	void InitAll();
	bool InitItemOne(UINT id, UINT row);	//!< ��ʼ������һ���������޸�Ҳ�����
	void InitItemValue(UINT id, UINT row, CUGCell* cell);			//!< ˢ�±���ֵ
	void ShowItem(std::list<UINT> ltItemID);//!< ��ʾָ���ı���
	void SelectRowAt(long row);				//!< ѡ��ĳһ��
	void SelectAllItem();					//!< ѡ�����б���
	void OutItemAll();						//!< �������б���
	void OutItemCurrent();					//!< ������ǰ���б���
	void OutItemSelect();					//!< ��������ѡ�б���
	void FreshData();						//!< ˢ��һ�������ֵ
	void ReadItemValue(bool bProj);			//!< ��ȡ����ֵ

	void ItemEditAll();						//!< ͳһ�޸ı���������
	void ItemEdit();						//!< �޸ı���
	std::shared_ptr<CItem> AddNewItem();	//!< �����±���
	void ItemClone();						//!< ������¡
	void ItemUp();
	void ItemDown();
	void ItemRemove(bool bAsk = true);		//!< �Ƴ�������������ʾ�Ƿ�Ҫѯ��һ���û�������ʱ��Ҫɾ�����Ͳ���Ҫ֪ͨ�û�
	void ItemCopy();						//!< ��������
	void ItemCut();							//!< ��������
	void ItemPaste();						//!< ����ճ��

	long GetItemRow(UINT id);				//!< �ҵ��������ڵ��У����û�ҵ�������-1
	UINT GetRowItem(long row);				//!< �ҵ���������ʾ�ı���id�����û�ҵ�������-1
	void RedrawGrid();						//!< ���»��Ʊ�񣬴�0��ʼ����
	void DeleteRowAll();					//!< ɾ��������
	void DeleteColAll();					//!< ɾ��������

	void FreshItemRows(std::list<UINT> ltItemID);	//!< ˢ����Щ����

	void OnLButtonDown(int col, long row, RECT *rect, POINT *point, BOOL processed);		//!< ����������
	void OnLButtonUp(int col, long row, RECT *rect, POINT *point, BOOL processed);			//!< ������̧��

	void OnDrawFocusBackRect(CDC *dc);														//!< �ػ�ѡ�п�ʹ�����Ŀ
	void OnDrawFocusHodeText(CDC *dc);														//!< �ػ�ѡ�п�ʹ�����Ŀ

	DECLARE_MESSAGE_MAP()
	//***** Over-ridable Notify Functions *****
	virtual void OnSetup();
	virtual void OnTH_LClicked( int iCol, long lRow, int iUpdn, RECT* pRect, POINT* pPoint, BOOL bProcessed = 0 );
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);		//!< ˫��ĳһ��
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);		//!< ���������
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);		//!< ����Ҽ����
	virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed);		//!< ����ƶ��¼�
	virtual void OnMouseMoveOver(int col,long row,BOOL bUp);								//!< ��갴ס�ƶ�������Ļ���¼�
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);										//!< ���¼��̼�
	virtual int  OnMenuStart(int iCol, long lRow, int iSection);							//!< 
	virtual int OnSortEvaluate(CUGCell* pCell1, CUGCell* pCell2, int iFlags);				//!< 
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);										//!< �ػ�ѡ�п�ʹ�����Ŀ
	virtual void OnDrawBackBitmap(CDC *dc);													//!< ������
	virtual void OnDrawBackBitmap(CDC *dc, int l ,int t, int r, int b);						//!< ������
	virtual void OnMenuCommand( int iCol, long lRow, int iSection, int iItem );				//!< �����Ҽ��˵�����Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
}
}
