#pragma once

namespace Servers{
namespace Compile{
typedef struct tagNeedRunObj
{
	CString m_strTitle;							//!< ����
	tagNeedRunObj(CString title){m_strTitle = title;}
	virtual void OnRun() = 0;
	virtual CString GetTitle(){return m_strTitle;}
}SNeedRunObj;

class CCompiler
{
public:
	enum{
//		COMPILE_OVERTIME = 100000,
		COMPILE_OVERTIME_ID = 1000
	};

private:
	bool m_bCompiling;						//!< �Ƿ����ڱ����ڼ�
	std::list<SNeedRunObj*> m_ltNeedRunObj;	//!< �ȴ����е��ⲿ����,��ȵ�����ɹ��󷽿�����

private:
	bool IsCompiling(){return m_bCompiling;}//!< �Ƿ����ڱ����ڼ�

	CCompiler(void);
	~CCompiler(void);
	void OnRunObj();						//!< ���������,�������������е��ⲿģ����
	void SetCompiling(bool b);				//!< ���õ�ǰ������״̬
	void CompileInfo(CString cmdLine);		//!< ��������,��������CMainFrame��д,���������뻹��ɨ�����ȫ��cmdLine������

public:
	static CCompiler& GetMe();

	void Check(CString cmdLine){;}			//!< ���
	void AddRunObj(SNeedRunObj* obj);		//!< ��Ӵ�������
	void CompileProj();						//!< ���빤��
	void CompileScan();						//!< ����ɨ��
	void RunScan();

	bool IsINeed(UINT dwData);
	void OnReceive(COPYDATASTRUCT* pCopyDataStruct);
	bool SerializeXml(TiXmlElement* pNode, bool bRead);		//!< ����xml��ʽ�ı༭��Ϣ
	void OnCompileOverTime();				//!< �����볬ʱʱ
};
}}