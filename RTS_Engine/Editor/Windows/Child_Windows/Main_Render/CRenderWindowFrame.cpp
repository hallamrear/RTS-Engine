
// CRenderWindowFrame.cpp : implementation of the CRenderWindowFrame class
//

#include "pch.h"
#include "framework.h"
#include "Editor.h"

#include "CRenderWindowFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRenderWindowFrame

IMPLEMENT_DYNCREATE(CRenderWindowFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CRenderWindowFrame, CMDIChildWnd)
END_MESSAGE_MAP()

// CRenderWindowFrame construction/destruction

CRenderWindowFrame::CRenderWindowFrame() noexcept
{

}

CRenderWindowFrame::~CRenderWindowFrame()
{
}


BOOL CRenderWindowFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = cs.style & (~FWS_ADDTOTITLE);
	cs.lpszName = "Game Render";

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CRenderWindowFrame diagnostics

#ifdef _DEBUG
void CRenderWindowFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CRenderWindowFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG