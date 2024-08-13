// CScriptWindowDocument.cpp : implementation file
//

#include "pch.h"
#include "Editor.h"
#include "CScriptWindowDocument.h"


// CScriptWindowDocument

IMPLEMENT_DYNCREATE(CScriptWindowDocument, CDocument)

CScriptWindowDocument::CScriptWindowDocument()
{
}

BOOL CScriptWindowDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CScriptWindowDocument::~CScriptWindowDocument()
{
}


BEGIN_MESSAGE_MAP(CScriptWindowDocument, CDocument)
END_MESSAGE_MAP()


// CScriptWindowDocument diagnostics

#ifdef _DEBUG
void CScriptWindowDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CScriptWindowDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CScriptWindowDocument serialization

void CScriptWindowDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CScriptWindowDocument commands
