
// CRenderWindowView.cpp : implementation of the CRenderWindowView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Editor.h"
#endif

#include <chrono>
#include <Windows/Child_Windows/Main_Render/CRenderWindowDoc.h>
#include <Windows/Child_Windows/Main_Render/CRenderWindowFormView.h>
#include "Dialogs/CAboutDlg.h"

#include <System/InputMonitor.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <System/ServiceLocator.h>

// CRenderWindowView

IMPLEMENT_DYNCREATE(CRenderWindowView, CFormView)

BEGIN_MESSAGE_MAP(CRenderWindowView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_COMMAND(ID_VIEW_WIREFRAME, &CRenderWindowView::OnViewWireframe)
	ON_COMMAND(ID_VIEW_SOLIDRENDER, &CRenderWindowView::OnViewSolidRender)
	ON_COMMAND(ID_TOOLBAR_PLAY, &CRenderWindowView::OnToolbarPlay)
	ON_COMMAND(ID_TOOLBAR_PAUSE, &CRenderWindowView::OnToolbarPause)
	ON_COMMAND(ID_TOOLBAR_STOP, &CRenderWindowView::OnToolbarStop)
END_MESSAGE_MAP()

// CRenderWindowView construction/destruction

Bennett::Engine* gEngine = nullptr;

CRenderWindowView::CRenderWindowView() noexcept
	: CFormView(IDD_EDITOR_FORM)
{
	m_IsEngineRendering = false;
}

CRenderWindowView::~CRenderWindowView()
{
}

void CRenderWindowView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CRenderWindowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = cs.style & (~FWS_ADDTOTITLE);
	cs.lpszName = "TEST";

	return CFormView::PreCreateWindow(cs);
}

void CRenderWindowView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
}


// CRenderWindowView printing

BOOL CRenderWindowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRenderWindowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRenderWindowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRenderWindowView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}


// CRenderWindowView diagnostics

#ifdef _DEBUG
void CRenderWindowView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRenderWindowView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRenderWindowDoc* CRenderWindowView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRenderWindowDoc)));
	return (CRenderWindowDoc*)m_pDocument;
}
#endif //_DEBUG

void CRenderWindowView::UpdateThreadFunction()
{
	auto lTime = std::chrono::steady_clock::now();
	auto cTime = lTime;
	std::chrono::duration<double> clockDelta = { };
	float dTime = 0.0f;

	if (gEngine)
	{
		while (gEngine->GetEngineState() != Bennett::ENGINE_STATE::STOPPED)
		{
			cTime = std::chrono::steady_clock::now();
			clockDelta = std::chrono::duration_cast<std::chrono::milliseconds>(cTime - lTime);
			dTime = clockDelta.count();

			if (dTime > TIMESTEP_CAP)
			{
				Log(LOG_STATUS::LOG_MINIMAL, "Capped timestep to %f\n", TIMESTEP_CAP);
				dTime = TIMESTEP_CAP;
			}

			gEngine->ProcessInput(dTime);

			if (gEngine->GetEngineState() == Bennett::ENGINE_STATE::RUNNING)
			{
				gEngine->Update(dTime);
			}
			else
			{
				gEngine->GetCameraController().GetCurrentCamera().Update(dTime);
			}
		}
	}
}

void CRenderWindowView::OnViewWireframe()
{
	Bennett::ServiceLocator::GetRenderer().SetCustomGraphicsPipelineNextFrame(Bennett::ServiceLocator::GetRenderer().GetWireframeGraphicsPipeline());
}

void CRenderWindowView::OnViewSolidRender()
{
	Bennett::ServiceLocator::GetRenderer().SetCustomGraphicsPipelineNextFrame(Bennett::ServiceLocator::GetRenderer().GetSolidGraphicsPipeline());
}

void CRenderWindowView::OnToolbarPlay()
{
	gEngine = Bennett::Engine::GetEngineInstance();
	if (gEngine)
	{
		if (gEngine->GetEngineState() != Bennett::ENGINE_STATE::RUNNING)
		{
			gEngine->SetEngineState(Bennett::ENGINE_STATE::RUNNING);
			m_UpdateThread = std::thread(&CRenderWindowView::UpdateThreadFunction, this);
		}
	}
}

void CRenderWindowView::OnToolbarPause()
{
	gEngine->SetEngineState(Bennett::ENGINE_STATE::PAUSED);
}

void CRenderWindowView::OnToolbarStop()
{
	gEngine = Bennett::Engine::GetEngineInstance();
	if (gEngine)
	{
		if (gEngine->GetEngineState() != Bennett::ENGINE_STATE::STOPPED)
		{
			gEngine->SetEngineState(Bennett::ENGINE_STATE::STOPPED);
			m_UpdateThread.join();
		}
	}
}

LRESULT CRenderWindowView::DefWindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	Bennett::InputMonitor::Win32InputCallback(msg, lParam, wParam);
	return CWnd::DefWindowProc(msg, wParam, lParam);
}

/*
afx_msg LRESULT CRenderWindowView::OnStartRenderThread(WPARAM wParam, LPARAM lParam)
{
	std::mutex mutex;
	mutex.lock();
	m_IsEngineRendering = true;
	mutex.unlock();
	m_RenderThread = std::thread(&CRenderWindowView::RenderThreadFunction, this);

	return 0;
}

afx_msg LRESULT CRenderWindowView::OnStopRenderThread(WPARAM wParam, LPARAM lParam)
{
	assert(gEngine);

	std::mutex mutex;
	mutex.lock();
	m_IsEngineRendering = true;
	mutex.unlock();

	m_RenderThread.join();

	return 0;
}
*/