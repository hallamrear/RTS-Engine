
// CRenderWindowFrame.h : interface of the CRenderWindowFrame class
//

#pragma once

class CRenderWindowFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CRenderWindowFrame)
public:
	CRenderWindowFrame() noexcept;

// Attributes
protected:

public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CRenderWindowFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
};
