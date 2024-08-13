
// Editor.h : main header file for the Editor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "System/Engine.h"
#include "Rendering/Window.h"

// CEditorApp:
// See Editor.cpp for the implementation of this class
//

class CEditorApp : public CWinApp, public Bennett::Engine
{
private:
	CMultiDocTemplate* m_RenderWindowDocTemplate;
	CMultiDocTemplate* m_ScriptEditorDocTemplate;
	Bennett::Window* m_BennettWindowObject;

public:
	CEditorApp() noexcept;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	bool Initialise() override;

// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
};

extern CEditorApp theApp;
