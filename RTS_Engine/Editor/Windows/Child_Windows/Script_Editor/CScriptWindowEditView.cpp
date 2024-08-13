// CScriptWindowEditView.cpp : implementation file
//

#include "pch.h"
#include "Editor.h"
#include "CScriptWindowEditView.h"

// CScriptWindowEditView

IMPLEMENT_DYNCREATE(CScriptWindowEditView, CEditView)

CScriptWindowEditView::CScriptWindowEditView()
{

}

CScriptWindowEditView::~CScriptWindowEditView()
{
}

BEGIN_MESSAGE_MAP(CScriptWindowEditView, CEditView)
END_MESSAGE_MAP()

// CScriptWindowEditView diagnostics

#ifdef _DEBUG
void CScriptWindowEditView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CScriptWindowEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScriptWindowEditView message handlers
