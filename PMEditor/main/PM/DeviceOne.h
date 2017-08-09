#pragma once

#include "XTooltip.h"

namespace XmlInfo{class CXmlDevice;}
namespace MVC{
namespace Device{
class CDeviceMapDoc;
class CDeviceParam;
class CDeviceInterface;
class CDeviceOne
{
protected:
	UINT m_uiID;					//!< �豸ID��
	UINT m_uiType;					//!< �豸�����ͺţ���Ӧ�����ļ������ͱ��
	CString m_strID;				//!< �豸�����л���ţ���ɨ������ж�ȡ������
	CString m_strType;				//!< �豸���������������¼��ϣ����û�еõ�������Ϣ��ʱ�򣬾��Ѿ�֪�����豸��������������ʾ�û�
	CString m_strName;				//!< �豸�����ƣ��û�����
	CString m_strTag;				//!< �豸�ı�ǩ
	CString m_strPathName;			//!< �豸�ı����ļ���ַ
	UINT m_uiParentID;				//!< ���豸��ţ�Ĭ��-1
	UINT m_uiLevel;					//!< �豸���ڼ��𣬴�0��ʼ�������豸����Ϊ-1
	BOOL m_bInterfaceSort;			//!< �Ƿ���豸�ӿڵ������Զ�����

	CPoint m_Point;					//!< �Լ���λ��
	bool m_bModify;					//!< �Ƿ�������޸���
	bool m_bSelect;					//!< �Ƿ�ѡ����
	bool m_bExpand;					//!< ��ʾ�豸�Ƿ�����չ״̬
	bool m_bShow;					//!< �Ƿ񻭣����豸��������ʱ�����豸�Ͳ���
	boost::shared_ptr<XmlInfo::CXmlDevice> m_spXmlDevice;				//!< �豸��Ӧ�������ļ�

	bool m_bLoadDcfg;				//!< �Ƿ�������豸���������ĵ�*.dcfg
	bool m_bProj;					//!< �Ƿ��ǹ����豸��true�ǹ����豸��false����

public:
	std::list<UINT> m_ltChildID;	//!< ���豸����
	std::vector< boost::shared_ptr<CDeviceInterface> > m_vtInterface;	//!< �豸��ʹ�õĽӿ��б�
	std::vector< boost::shared_ptr<CDeviceParam> > m_vtParam;			//!< �����б�

public:
	bool IsSelect(){return m_bSelect;}
	bool IsExpand(){return m_bExpand;}
	bool IsShow(){return m_bShow;}
	bool IsProj(){return m_bProj;}										//!< �ж��Լ��ǲ��ǹ����е��豸
	bool IsModify(){return m_bModify;}									//!< �Ƿ�������޸���
	UINT getID(){return m_uiID;}
	UINT getDevType(){return m_uiType;}
	CString getStrType(){return m_strType;}
	CString getStrID(){return m_strID;}
	CString getStrParentID();											//!< ��ø��豸��ID
	CString getName(){return m_strName;}
	CString getTag(){return m_strTag;}
	CString getPath(){return m_strPathName;}
	UINT getParentID(){return m_uiParentID;}
	UINT getLevel(){return m_uiLevel;}
	BOOL getInterfaceSort(){return m_bInterfaceSort;}
	long getState();													//!< �����豸��ǰ״̬��0���ߣ�1���ߣ�2����δ���ã�3���߳�ͻ

	void setID(UINT id){m_uiID = id;}
	bool setDevType(UINT type);											//!< �����Ҫ���봦������һ�´洢��Ӧ���豸����
	void setStrID(CString strID){m_strID = strID;}
	void setTag(CString tag){m_strTag = tag;}
	void setPath(CString path){m_strPathName = path;}
	void setParentID(UINT id){m_uiParentID = id;}
	void setLevel(UINT level){m_uiLevel = level;}
	void setInterfaceSort(BOOL sort){m_bInterfaceSort = sort;}
	void setProj(bool bProj){m_bProj = bProj;}							//!< �����Լ��ǲ��ǹ����е��豸
	void SetModify(){m_bModify = true;}									//!< �Ƿ�������޸���

	CString GetTypeName();												//!< ����豸��������
	void SetSelect(bool select){m_bSelect = select;}
	void SetExpand(bool expand){m_bExpand = expand;}
	void SetShow(bool show){m_bShow = show;}
	void SetLTPt(LONG x, LONG y){m_Point.x = x; m_Point.y = y;}
	CPoint GetLTPt(){return m_Point;}

	boost::shared_ptr<XmlInfo::CXmlDevice> GetXmlInfo(){return m_spXmlDevice;}

protected:
	virtual void OnDrawFrame(CDC* pDC);		//!< ���豸�ı߿�
	virtual void OnDrawWord(CDC* pDC);		//!< �����ֲ���
	virtual void OnDrawPic(CDC* pDC);		//!< ��ͼƬ
	virtual void OnDrawToParent(CDC* pDC);	//!< ���豸
	virtual void OnDrawToHostLine(CDC* pDC);//!< ����ĸ�ߵ�����

public:
	CDeviceOne();
	~CDeviceOne(void);
	virtual void OnAddNew();				//!< ��Ϊ���豸��ӵ������е��¼�
	virtual bool InitDevType(UINT devType);	//!< �������豸Ҫ��ʼ�����������Ҫ
	virtual void ResizeInterface(UINT num);	//!< ���¶���ӿ������Ԫ������
	virtual void setName(CString name);

	virtual int IsInMyRect(CPoint point);	//!< �ж������Ƿ�������Χ��,0��ʾûѡ�У�1��ʾѡ���ˣ�2��ʾѡ���˼Ӽ���
	virtual void OnSetLevel(UINT level);	//!< �ݹ�����һ���豸�ļ���
	virtual void OnCountLevel();			//!< �����Լ�����ZigBee�ṹ�еĲ��
	virtual void OnCountPoint();			//!< �����Լ����ڵ�����
	virtual void OnDraw(CDC* pDC);			//!< ���豸
	virtual void ShowHideChild(bool show);	//!< ��ʾ���������Լ��ͺ���
	virtual void OnRButtonUp(UINT nFlags, CPoint point, CWnd* wnd);		//!< ���豸�Ҽ�

	virtual bool SerializeXmlGeneral(TiXmlElement* pNode, bool bRead);	//!< �����豸��Ҫ��Ϣ���������ļ��н���
	virtual bool LoadXmlFile();
	virtual bool SaveXmlFile();				//!< �����ļ��ĵ�ַ����Ե�ַ + ����Ŀ¼�ĵ�ַ

	virtual bool DevOut(CString path, CString name);					//!< �����豸����
	virtual bool DevIn(CString path, CString name);						//!< �����豸����

	virtual void DeleteChild(UINT id);		//!< ɾ���������
	virtual void AddChild(UINT id);			//!< ��Ӻ���
	virtual void OnCopy();					//!< ��������
	virtual void OnCopyWithChild();			//!< �������ˣ��Լ������к���Ҳ��������
	virtual void OnCut(CDeviceMapDoc* pDoc);//!< �������ˣ�����Ҳ�ñ�����
	virtual void OnPaste();					//!< ��ճ�������Լ��ŵ�������

	virtual boost::shared_ptr<CDeviceInterface> GetInterface(UINT id);	//!< ��õ�id���ӿ�
	virtual boost::shared_ptr<CDeviceInterface> GetInterfaceFromType(UINT type);	//!< ���ݽӿڵ������ҵ���һ������������͵Ľӿ�
	virtual boost::shared_ptr<CDeviceParam> GetParam(UINT id);						//!< �Ҳ���

	virtual long GetOnLineInf();										//!< �豸�Ƿ����ߣ��������ߵĽӿ�ID�����ڷ���-1
	virtual void SetOnLineInf(UINT infID, bool bOnLine);				//!< �����豸����״̬�������ǽӿ�ID
	virtual void SetOffLine();											//!< ֱ����������

	virtual void GetWatchInfo(Tool::TooltipText& toolTip);				//!< ��ü��ʱ����Ϣ
	virtual void LoadXml();												//!< ����xml�ĵ�

	virtual bool IsMyParent(UINT id);									//!< id�豸�Ƿ����Լ����ϴ�
	virtual bool CanAddSlave(bool bSayWhy = true);						//!< �ж��Ƿ��������豸�����������˵��ԭ��
	virtual bool CanAddSlaveType(UINT devType);							//!< �ж��Ƿ����������͵Ĵ��豸

	virtual void UpLoadBehavior(UINT id, CComVariant cvrIndex);			//!< ����һ����Ϊ�Ĳ�������������
	virtual void DownLoadBehavior(UINT id, CComVariant cvrIndex);		//!< ����һ����Ϊ�Ĳ�������������

	virtual void CopyFrom(CDeviceOne& device);
	virtual bool operator == (CDeviceOne& device) const;				//!< �ж������豸�Ƿ�һ��
	virtual bool DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex = false);//!< ���ұ�������Ҫƥ����ı���ȫ�ַ�ƥ�䣬��Сдƥ��

	void GetWholeAddr(std::list<UINT>& ltAddr, bool bOnLine = true);	//!< ��ø��豸�Ľӿ����ͣ�bOnLine��ʾ�Ƿ���Ҫ��һ�����ߵģ�����-1��ʾû��
	int GetInterfaceSize();												//!< ��ÿ����ɵĽӿ�����

	void OnProgram();													//!< ����������

protected:
	virtual bool IsInExpandRect(CPoint point);							//!< �ж������Ƿ�����������Χ��
	virtual bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< �����豸��Ϣ�����豸�����ļ��н���
};
}
}
