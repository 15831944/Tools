#pragma once
#include "DTreeCtrl.h"

class CProject;
// class CDTreeCtrl;
// Struct IDTreeOwner;
/*!
	���̹�����
*/
class CProjectMgr
{
private:
	std::shared_ptr<CProject> m_CulProject;				// ��ǰ���̵�����ָ��
	bool m_bWatch;						// ��ǰ�Ƿ��ڼ��״̬

private:
	CProjectMgr(void);
	~CProjectMgr(void);

public:
	bool IsWatch(){return m_bWatch && m_CulProject;}

public:
	std::shared_ptr<CProject> GetProj(){return m_CulProject;}
	static CProjectMgr& GetMe();
	bool IsModify();					// ���������Ƿ񱻸���
	bool NewProject();					// �½����̵ĺ������
	bool OpenProject(CString path = _T(""));	// �򿪹��̵ĺ�����ڣ�������Ҫ���̾���·����
	bool SaveProject();					// ���湤�̵ĺ������
	bool BackUpProject();				// ���ݹ��̵ĺ������
	bool CloseProject();				// �رչ��̵ĺ������
	void EditProjInfo();				// �༭������Ϣ
	void SetWatch(const bool b);		// ����/ȡ�����

	bool SayWatch();					// ������״̬���ܲ�������ʾ
	void UpdateTreeView();				// ˢ����
};
