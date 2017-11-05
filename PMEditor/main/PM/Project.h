#pragma once

namespace MVC{namespace Item{class CItemDoc;}}
namespace Tool{class CDTreeCtrl;}

class CProject
{
private:
	bool m_bCompiled;			// �Ƿ񱻱����
	bool m_bModify;				// �Ƿ��޸Ĺ�
	int m_nProjectType;			// �������ͣ�0-����/Web���̣�1-��ƽ̨����
	CString m_strFileName;		// �����ļ�������������չ��,"projOne"
	CString m_strProjName;		// ������, "����x"
	CString m_strPath;			// ����·��������'\'��"C:\Proj\"
	CString m_strAuthor;		// ������
	CString m_strDescription;	// ���̱�ע��Ϣ
	CString m_strVersion;		// ���̰汾��
	SYSTEMTIME m_ctBuildTime;	// ���̴���ʱ��
	SYSTEMTIME m_ctUpdateTime;	// �����޸�ʱ��

	HTREEITEM m_hProjectItem;	// ���������еĽڵ�
	HTREEITEM m_hItemItem;		// ���������еĽڵ�

public:
	bool IsCompiled(){return m_bCompiled;}
	CString GetProjFileName(){return m_strFileName;}
	CString GetProjName(){return m_strProjName;}
	CString GetPath(){return m_strPath;}
	CString GetAuthor(){return m_strAuthor;}
	CString GetDescription(){return m_strDescription;}
	CString GetVersion(){return m_strVersion;}

	void SetModify(bool b){m_bModify = b;}
	void SetCompiled(bool b){m_bCompiled = b;}
	void SetName(CString name){m_strFileName = name;}
	void SetPath(CString path){m_strPath = path;}
	void SetAuthor(CString auth){m_strAuthor = auth;}
	void SetDescription(CString desc){m_strDescription = desc;}
	void SetVersion(CString vers){m_strVersion = vers;}

private:
	void ReName(CString nameNew);										// �����̶���Ϊ�������
	bool SerializeXml(TiXmlElement* pNode,bool bRead=true);				// ��д�����ļ��ڵ�

public:
	CProject();
	CProject(CString name,CString path,CString author,CString description,CString version);
	CProject(int type, CString name, CString path, CString author, CString description, CString version);
	~CProject(void);

	CString GetWholePathName();											// ����ļ���ȫ·��·��

	bool IsModify();
	bool CreateProject();												// �������̵��ڲ���Ϣ
	bool OnClose();														// �رչ���ʱҪ���������
	bool OpenProject(CString title, CString name, CString path);		// ��һ������
	bool SaveProject();													// ���汾����
	bool BackUpProject(CString name,CString pathname);					// ���ݹ��̵���ĵط�
	bool OnReName();													// ����Ҫ��������
	void ShowInfo();													// ��ʾ�Լ������ԶԻ������û��޸�

	void OnTreeDblClick(CTreeCtrl* treeCtrl, HTREEITEM item);			// ��Ӧ˫�������¼�
	void OnTreeRClick(CTreeCtrl* treeCtrl, HTREEITEM item,CPoint point);// ��Ӧ�Ҽ�����¼�
	void OnTreeEnter(CTreeCtrl* treeCtrl, HTREEITEM item);				// ��Ӧ���̻س��¼�
	void UpdateProjView(Tool::CDTreeCtrl& pTreeCtrl);					// ˢ�¹�����
	void OnHelpInfo(HTREEITEM hItem);									// ���F1����
};
