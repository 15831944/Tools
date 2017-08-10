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
		COL_ID = 0,									//!< ˵��
		COL_TAG = COL_ID + 1,						//!< ���
		COL_RECORD = COL_TAG + 1,					//!< �Ƿ�����¼�ƹ���
		//COL_AUTOSAVE = COL_RECORD + 1,				//!< �Ƿ��Զ�������Ƶ�ļ�
		COL_PATH = COL_RECORD + 1,					//!< ��Ƶ�ļ��洢λ��,����'\',Ĭ���ҵ��ĵ�
		COL_ENCODE = COL_PATH + 1,					//!< ��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
		COL_DISTINGUISH = COL_ENCODE + 1,			//!< ¼�Ʒֱ���,160*120,176*144,......,720*480,Ĭ��320*240
		COL_FPS = COL_DISTINGUISH + 1,				//!< ¼��֡����29.97,15,10,5,1,0.5 fps,Ĭ��1fps
		COL_BROAD = COL_FPS + 1,					//!< �Ƿ���������㲥
		COL_PORT = COL_BROAD + 1,					//!< �㲥�˿ں�,8080
		COL_MAXCLIENT = COL_PORT + 1,				//!< ���������,Ĭ��5,��Χ0~50
		COL_BROADENCODE = COL_MAXCLIENT + 1,		//!< �㲥��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
		COL_BROADDISTINGUISH = COL_BROADENCODE + 1,	//!< �㲥�ֱ���,160*120,176*144,......,720*480
		COL_BROADFPS = COL_BROADDISTINGUISH + 1,	//!< �㲥֡����29.97,15,10,5,1,0.5 fps
	};

private:
	bool m_bWatch;					//!< �������Ƿ���
	BOOL m_bSortType;				//!< ��¼��ǰ�е�������
	int m_iArrowIndex;				//!< �������Ǻš�С��־
	int m_iSortCol;
	CUGSortArrowType m_sortArrow;
	CFont m_ItemFont, m_HeadFont;
	std::map<UINT, CString> m_mpColName;	//!< ��¼���е�����

private:
	friend class CCamView;
	bool IsGridWatch(){return m_bWatch;}
	void SetGridWatch(bool b){m_bWatch = b;}
	static bool SortItem(UINT& lth, UINT& rth);	//!< �Ը�����ID�ı�����������

private:
	void InitCol();							//!< ��ʼ���б���
	void InitAddRow();						//!< ��ʼ��Ĭ�ϡ���ӱ�����һ��
	void SortGrid();						//!< ������
	void SetMyCell(int nCol, long lRow, CUGCell* cell, UINT id);

	void EditItem(std::shared_ptr<CCamera> item);					//!< �༭������������
//	void EditItems(std::list<std::shared_ptr<CCamera> > ltItem);	//!< ͳһ�༭������������
	void ItemRemove(std::list<std::shared_ptr<CCamera> > ltItem);	//!< ɾ������

	CString GetFloatStr(float flt);			//!< ��ø����ַ���

public:
	CCamGrid();
	~CCamGrid();
	void OnEditEnd();						//!< �༭������ͳһ����
	void InitAll();
	bool InitItemOne(UINT id, UINT row);	//!< ��ʼ������һ���������޸�Ҳ�����
	void ShowItem(std::list<UINT> ltItemID);//!< ��ʾָ���ı���
	void SelectRowAt(long row);				//!< ѡ��ĳһ��
	void SelectAllItem();					//!< ѡ�����б���

	//void ItemEditAll();						//!< ͳһ�޸ı���������
	void ItemEdit();						//!< �޸ı���
	std::shared_ptr<CCamera> AddNewItem();	//!< �����±���
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

	DECLARE_MESSAGE_MAP()
	//***** Over-ridable Notify Functions *****
	virtual void OnSetup();
	virtual void OnTH_LClicked( int iCol, long lRow, int iUpdn, RECT* pRect, POINT* pPoint, BOOL bProcessed = 0 );
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);		//!< ˫��ĳһ��
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);		//!< ���������
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);		//!< ����Ҽ����
	virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed);		//!< ����ƶ��¼�
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);										//!< ���¼��̼�
	virtual int  OnMenuStart(int iCol, long lRow, int iSection);							//!< 
	virtual int OnSortEvaluate(CUGCell* pCell1, CUGCell* pCell2, int iFlags);				//!< 
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);										//!< �ػ�ѡ�п�ʹ�����Ŀ
	virtual void OnMenuCommand( int iCol, long lRow, int iSection, int iItem );				//!< �����Ҽ��˵�����Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
}
}
