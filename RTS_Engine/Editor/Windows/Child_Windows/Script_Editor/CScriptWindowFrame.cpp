// CScriptWindowFrame.cpp : implementation file
//

#include "pch.h"
#include "Editor.h"
#include "CScriptWindowFrame.h"

// CScriptWindowFrame

IMPLEMENT_DYNCREATE(CScriptWindowFrame, CMDIChildWnd)

CScriptWindowFrame::CScriptWindowFrame()
{

}

CScriptWindowFrame::~CScriptWindowFrame()
{
}


BEGIN_MESSAGE_MAP(CScriptWindowFrame, CMDIChildWnd)
END_MESSAGE_MAP()

// CScriptWindowFrame message handlers


BOOL CScriptWindowFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = cs.style & (~FWS_ADDTOTITLE);
	cs.lpszName = "Script Editor";

	return CMDIChildWnd::PreCreateWindow(cs);
}
