#pragma once

namespace Servers{
namespace Compile{
typedef struct tagNeedRunObj
{
	CString m_strTitle;							//!< 名称
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
	bool m_bCompiling;						//!< 是否正在编译期间
	std::list<SNeedRunObj*> m_ltNeedRunObj;	//!< 等待运行的外部对象,需等到编译成功后方可启动

private:
	bool IsCompiling(){return m_bCompiling;}//!< 是否正在编译期间

	CCompiler(void);
	~CCompiler(void);
	void OnRunObj();						//!< 编译结束了,可以启动待运行的外部模块了
	void SetCompiling(bool b);				//!< 设置当前按编译状态
	void CompileInfo(CString cmdLine);		//!< 启动编译,命令行由CMainFrame来写,是正常编译还是扫描编译全看cmdLine的内容

public:
	static CCompiler& GetMe();

	void Check(CString cmdLine){;}			//!< 检查
	void AddRunObj(SNeedRunObj* obj);		//!< 添加待启动项
	void CompileProj();						//!< 编译工程
	void CompileScan();						//!< 编译扫描
	void RunScan();

	bool IsINeed(UINT dwData);
	void OnReceive(COPYDATASTRUCT* pCopyDataStruct);
	bool SerializeXml(TiXmlElement* pNode, bool bRead);		//!< 保存xml格式的编辑信息
	void OnCompileOverTime();				//!< 当编译超时时
};
}}