#pragma once


// CScriptWindowEditView view

class CScriptWindowEditView : public CEditView
{
	DECLARE_DYNCREATE(CScriptWindowEditView)

protected:
	CScriptWindowEditView();           // protected constructor used by dynamic creation
	virtual ~CScriptWindowEditView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


