#pragma once
class CThreadWorker
{
private:
	bool m_working;
	std::mutex m_mtx;
	std::shared_ptr<std::thread> m_thread;

private:
	void SetWorking(bool b){ m_working = b; }

public:
	CThreadWorker();
	~CThreadWorker();

	void DoWork(CString& strPathXml, CString& strPathCSV);
};

