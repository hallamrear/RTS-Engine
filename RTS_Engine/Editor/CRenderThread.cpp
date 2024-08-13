// CRenderThread.cpp : implementation file
//

#include "pch.h"
#include "Editor.h"
#include "CRenderThread.h"


// CRenderThread

IMPLEMENT_DYNCREATE(CRenderThread, CWinThread)

CRenderThread::CRenderThread()
{
	m_EnginePtr = nullptr;
}

CRenderThread::~CRenderThread()
{
	m_EnginePtr = nullptr;
}

BOOL CRenderThread::InitInstance()
{
	m_EnginePtr = Bennett::Engine::GetEngineInstance();
	return TRUE;
}

int CRenderThread::ExitInstance()
{
	m_EnginePtr = nullptr;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRenderThread, CWinThread)
END_MESSAGE_MAP()

int CRenderThread::Run()
{
	while (true)
	{
		if (m_EnginePtr)
		{
			m_EnginePtr->Render();
		}
	}

	return CWinThread::Run();
}

BOOL CRenderThread::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinThread::OnIdle(lCount);
}
