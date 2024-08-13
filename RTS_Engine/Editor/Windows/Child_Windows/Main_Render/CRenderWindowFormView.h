
// CRenderWindowView.h : interface of the CRenderWindowView class
//

#pragma once
#include <thread>
#include <mutex>

class CRenderWindowView : public CFormView
{
protected: // create from serialization only
	CRenderWindowView() noexcept;
	DECLARE_DYNCREATE(CRenderWindowView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_EDITOR_FORM };
#endif

// Attributes
public:
	class CRenderWindowDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRenderWindowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
private:
	bool m_IsEngineRendering;
	std::thread m_UpdateThread;
	void UpdateThreadFunction();

public:
	afx_msg void OnViewWireframe();
	afx_msg void OnViewSolidRender();
	afx_msg void OnToolbarPlay();
	afx_msg void OnToolbarPause();
	afx_msg void OnToolbarStop();

	LRESULT DefWindowProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
protected:
};

#ifndef _DEBUG  // debug version in EditorView.cpp
inline CRenderWindowDoc* CRenderWindowView::GetDocument() const
   { return reinterpret_cast<CRenderWindowDoc*>(m_pDocument); }
#endif

