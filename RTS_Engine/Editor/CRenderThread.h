#pragma once

#include <System/Engine.h>

// CRenderThread

class CRenderThread : public CWinThread
{
	DECLARE_DYNCREATE(CRenderThread)

private:
	Bennett::Engine* m_EnginePtr;

protected:
	CRenderThread();           // protected constructor used by dynamic creation
	virtual ~CRenderThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	virtual BOOL OnIdle(LONG lCount);
};


