#pragma once
#include "PropertyCam.h"

namespace MVC{namespace Camera{
class CCamera;
class CCamDoc;
class CCamMgr
{
public:
	std::vector<boost::shared_ptr<CCamera> > m_vtCam;			//!< ����ͷ����
	std::list<boost::shared_ptr<CCamera> > m_ltItemClipBoard;	//!< �����ļ�����
	std::list<UINT> m_ltEditItemID;								//!< ��ǰ�ոձ��޸ĵı���ID�б�
	CCamDoc* m_pItemDoc;
	CPropertyCamera m_ItemProperty;

private:
	bool m_bModify;					//!< �Ƿ��޸���
	bool m_bActive;					//!< �Ƿ�������Ƶ������
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
	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< ����xml��ʽ�ı༭��Ϣ
	void OnCreate();
	void OnClose();
	void OpenDoc();
	void SaveFile();											//!< �����ļ�
	bool OpenFile(CString name, CString pathall, CString ver);	//!< ����Ƶ�����ļ������� �� ���� + ·�� + �汾��
	boost::shared_ptr<CCamera> GetCam(ULONG id);				//!< ���id�ű���
	boost::shared_ptr<CCamera> GetFirstItem();					//!< ��õ�һ������
	boost::shared_ptr<CCamera> GetLastItem();					//!< ������һ������
	bool FindItem(ULONG id);									//!< ���ұ��Ϊid�ı���
	bool AddItem(boost::shared_ptr<CCamera> item);				//!< ��ӱ���,��ʱ������û��ID�����ڱ�����������
	void DeleteItem(ULONG id);									//!< ɾ�����Ϊid�ı���
	UINT GetItemSize();											//!< ��ñ�������ʵ����
	void ExChangeItem(UINT id1, UINT id2);						//!< ��������������
	void ShowItem(UINT id);										//!< ��ʾ�����Ϊid�ı���
	int SearchItem(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex = false);	//!< ���ҷ��ϸ��ַ����ı���������ƥ�������
	int GetDifferentPort(int nDefPort);							//!< ���һ����ͬ�Ķ˿ں�

	void AddEditItem(UINT id);									//!< ��Ӹոձ��޸ĵı���
	void SetEditEnd();											//!< �����������������
	void FreshAllGrid();										//!< ˢ�����б����ʾ
};
}}