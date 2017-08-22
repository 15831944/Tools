#include "stdafx.h"
#include "ThreadWorker.h"

#include "LoadDB.h"
#include "Gbl.h"

CThreadWorker::CThreadWorker()
	: m_working(false)
{
}

CThreadWorker::~CThreadWorker()
{
}

void WorkingThread(CThreadWorker* worker, CString strPathXml, CString strPathCSV)
{
	CLoadDB db;
	db.LoadDB(strPathXml);
	db.SaveCSV(strPathCSV);
}

void CThreadWorker::DoWork(CString& strPathXml, CString& strPathCSV)
{
	PrintTime pt;
	m_thread = std::shared_ptr<std::thread>(new std::thread(WorkingThread, this, strPathXml, strPathCSV));
	//BOOL bSet = ::SetThreadPriority(m_thread->native_handle(), THREAD_PRIORITY_HIGHEST);
	//ASSERT(bSet);
	m_thread->join();
	pt.MessageBoxTime();
}
