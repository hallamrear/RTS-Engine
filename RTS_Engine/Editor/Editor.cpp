
// Editor.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "resource.h"

#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Editor.h"
#include "Windows/MainFrame.h"

#include "Dialogs/CAboutDlg.h"

#include <Windows/Child_Windows/Main_Render/CRenderWindowFormView.h>
#include <Windows/Child_Windows/Main_Render/CRenderWindowFrame.h>
#include <Windows/Child_Windows/Main_Render/CRenderWindowDoc.h>

#include <Windows/Child_Windows/Script_Editor/CScriptWindowDocument.h>
#include <Windows/Child_Windows/Script_Editor/CScriptWindowEditView.h>
#include <Windows/Child_Windows/Script_Editor/CScriptWindowFrame.h>

#include <Rendering/Window.h>
#include <CRenderThread.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <Defines/BennettInputDefines.h>


// CEditorApp

BEGIN_MESSAGE_MAP(CEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CEditorApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEditorApp construction

CEditorApp::CEditorApp() noexcept
{


	m_BennettWindowObject = nullptr;
	m_RenderWindowDocTemplate = nullptr;
	m_ScriptEditorDocTemplate = nullptr;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Editor.AppID.NoVersion"));
}

// The one and only CEditorApp object

CEditorApp theApp;
CWinThread* gRenderThread;

// CEditorApp initialization

BOOL CEditorApp::InitInstance()
{

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	m_RenderWindowDocTemplate = new CMultiDocTemplate(IDR_RenderWindow,
		RUNTIME_CLASS(CRenderWindowDoc),
		RUNTIME_CLASS(CRenderWindowFrame),
		RUNTIME_CLASS(CRenderWindowView));
	if (!m_RenderWindowDocTemplate)
		return FALSE;
	AddDocTemplate(m_RenderWindowDocTemplate);

	m_ScriptEditorDocTemplate = new CMultiDocTemplate(IDR_ScriptEditor,
		RUNTIME_CLASS(CScriptWindowDocument),
		RUNTIME_CLASS(CScriptWindowFrame),
		RUNTIME_CLASS(CScriptWindowEditView));
	if (!m_ScriptEditorDocTemplate)
		return FALSE;
	AddDocTemplate(m_ScriptEditorDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MainEditor))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//Open editor form windows.

	if (m_ScriptEditorDocTemplate)
	{
		m_ScriptEditorDocTemplate->OpenDocumentFile(NULL);
	}

	if (m_RenderWindowDocTemplate)
	{
		m_RenderWindowDocTemplate->OpenDocumentFile(NULL);
	}
	

	// this code is working
	CMDIChildWnd* child = pMainFrame->MDIGetActive();
	do
	{
		CString str;
		child->GetWindowText(str);
		if (str == "Game Render")
		{
			child->MDIActivate(); // or MDIActivate(child);
			break;
		}

		child = (CMDIChildWnd*)child->GetWindow(GW_HWNDNEXT);
	} while (child);






	//Creating Engine bits.
	if (m_BennettWindowObject)
	{
		Bennett::Window::Destroy(m_BennettWindowObject);
		m_BennettWindowObject = nullptr;
	}

	assert(child);

	if (child)
	{
		m_BennettWindowObject = Bennett::Window::CreateFromExistingHandle(child->GetSafeHwnd());
	}

	//Bennett Engine initialisation.
	bool result = Initialise();

	if (result == false)
	{
		Log(LOG_CRITICAL, "Failed to initialise engine systems.");
		return FALSE;
	}

	gRenderThread = AfxBeginThread(RUNTIME_CLASS(CRenderThread), NULL, 0, CREATE_SUSPENDED);
	gRenderThread->m_bAutoDelete = false;
	gRenderThread->ResumeThread();

	return TRUE;
}

bool CEditorApp::Initialise()
{
	std::vector<int> keys =
	{
		BENNETT_KEY_F1,
		BENNETT_KEY_F2,
		BENNETT_KEY_F3,
		BENNETT_KEY_F4,
		BENNETT_KEY_F5,
		BENNETT_KEY_F6,
		BENNETT_KEY_F7,
		BENNETT_KEY_F8,
		BENNETT_KEY_F9,
		BENNETT_KEY_F10,
		BENNETT_KEY_F11,
		BENNETT_KEY_F12
	};

	if (!InitialiseEngineSystems(*m_BennettWindowObject))
	{
		Log(LOG_CRITICAL, "Failed to initialise engine systems.");
		return FALSE;
	}

	Bennett::AssetManager& am = Bennett::ServiceLocator::GetAssetManager();
	
	//std::vector<glm::ivec2> ids
	//{
	//	glm::ivec2(-1, -1), glm::ivec2(+0, -1), glm::ivec2(+1, -1),
	//	glm::ivec2(-1, +0), glm::ivec2(+0, +0), glm::ivec2(+1, +0),
	//	glm::ivec2(-1, +1), glm::ivec2(+0, +1), glm::ivec2(+1, +1),
	//};
	//
	//GetWorld().PreloadChunks(ids);
	//GetCameraController().SetCamera(Bennett::CAMERA_MODE::STANDARD_CAM);
	//GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	//GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);
	//
	GetCameraController().SetCamera(Bennett::CAMERA_MODE::FREE_CAM);
	GetCameraController().GetCurrentCamera().SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	GetCameraController().GetCurrentCamera().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	GetCameraController().GetCurrentCamera().SetMovementSpeed(10.0f);
	GetCameraController().GetCurrentCamera().SetMouseLookEnabled(false);
	
	GetWorld().SpawnEntity("ChunkLoader");
	GetWorld().SpawnTESTEntity("HeightTester", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3())->SetModel(am.GetModel("1x1_Cube"));

	m_EngineState = Bennett::ENGINE_STATE::PAUSED;

	return TRUE;
}

int CEditorApp::ExitInstance()
{
	gRenderThread->SuspendThread();	
	delete gRenderThread;
	gRenderThread = nullptr;

	//Destroying Engine bits.
	if (m_BennettWindowObject)
	{
		Bennett::Window::Destroy(m_BennettWindowObject);
		m_BennettWindowObject = nullptr;
	}

	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}


// App command to run the dialog
void CEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CEditorApp::OnFileNew()
{
	if (m_ScriptEditorDocTemplate)
	{
		m_ScriptEditorDocTemplate->OpenDocumentFile(NULL);
	}
}