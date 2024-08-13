#pragma once


// CScriptWindowFrame frame

class CScriptWindowFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CScriptWindowFrame)
protected:
	CScriptWindowFrame();           // protected constructor used by dynamic creation
	virtual ~CScriptWindowFrame();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


