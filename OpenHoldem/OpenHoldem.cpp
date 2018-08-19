//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "OpenHoldem.h"
#include <afxadv.h>
#include <afxdisp.h>
#include <afxwin.h>
#include <assert.h>
#include "..\DLLs\Debug_DLL\debug.h"
#include "..\DLLs\Files_DLL\Files.h"
#include "..\DLLs\Formula_DLL\CFormula.h"
#include "..\DLLs\Formula_DLL\CFormulaParser.h"
#include "..\DLLs\GUI_DLL\CGUI.h"
#include "..\DLLs\GUI_DLL\MainFrame\MainFrm.h"
#include "..\DLLs\GUI_DLL\MainFrame\OpenHoldemDoc.h"
#include "..\DLLs\GUI_DLL\MainFrame\OpenHoldemView.h"
#include "..\DLLs\GUI_DLL\resource.h"
#include "..\DLLs\HeartbeatThread_DLL\CHeartbeatThread.h"
#include "..\DLLs\Preferences_DLL\Preferences.h"
#include "..\DLLs\SessionCounter_DLL\CSessionCounter.h"
#include "..\Shared\MagicNumbers\MagicNumbers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Supports MRU
AFX_STATIC_DATA const TCHAR _afxFileSection[] = _T("Recent File List");
AFX_STATIC_DATA const TCHAR _afxFileEntry[] = _T("File%d");
AFX_STATIC_DATA const TCHAR _afxPreviewSection[] = _T("Settings");
AFX_STATIC_DATA const TCHAR _afxPreviewEntry[] = _T("PreviewPages");

// COpenHoldemApp construction

COpenHoldemApp::COpenHoldemApp() {
}

// COpenHoldemApp destruction
COpenHoldemApp::~COpenHoldemApp() {
}

// COpenHoldemApp initialization
BOOL COpenHoldemApp::InitInstance() {
  // InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
  //
  // This code should probably be called at the VERY beginning,
  // especially to support UNICODE-filenames on Win7/8,
  // which might be as early as the ini-file.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=17579&p=122399#p122398
  // http://stackoverflow.com/questions/6633515/mfc-app-assert-fail-at-crecentfilelistadd-on-command-line-fileopen
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls); 
	// Since OH 4.0.0 we always use an ini-file,
	// the one and only in our OH-directory,
	// no matter how it is named.
	// For the technical details please see:
	// http://msdn.microsoft.com/de-de/library/xykfyy20(v=vs.80).aspx
	// Change class name of Dialog
	WNDCLASS wc;
	GetClassInfo(AfxGetInstanceHandle(), "#32770", &wc);
	wc.lpszClassName = "OpenHoldemFormula";
	wc.hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	RegisterClass(&wc);
   CWinApp::InitInstance();
 	// Initialize OLE libraries
	// Mandatory to call those initialisations. 
	// This will also help win7/8 compatibility 
	// those line are automatically inserted if you create a new MFC project with VS2010
	// http://stackoverflow.com/questions/6633515/mfc-app-assert-fail-at-crecentfilelistadd-on-command-line-fileopen
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=17579&start=150#p122418
	if (!AfxOleInit())
		return FALSE;
	AfxEnableControlContainer();
	// Classes
  // First we have to read the pre4ferences,
  // as start_log() needs to know if the old log has to be deleted...
  free((void*)m_pszProfileName);
  m_pszProfileName = _strdup(IniFilePath().GetString());
  Preferences()->LoadPreferences();
	// Start logging immediatelly after the loading the preferences
	// and initializing the sessioncounter, which is necessary for 
	// the filename of the log (oh_0.log, etc).
  bool log_needs_to_be_deleted = false;
  struct stat file_stats = { 0 };
  if (stat(LogFilePath(SessionCounter()->session_id()).GetString(), &file_stats) == 0) {
    unsigned long int max_file_size = 1E06 * Preferences()->log_max_logsize();
    size_t file_size = file_stats.st_size;
    if (file_size > max_file_size) {
      log_needs_to_be_deleted = true;
    }
  }
	start_log(SessionCounter()->session_id(), log_needs_to_be_deleted);
  // ...then re-Load the preferences immediately after creation 
  // of the log-file again, as We might want to to log the preferences too,
  // which was not yet possible some lines above.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=124&t=20281&p=142334#p142334
  Preferences()->LoadPreferences();
	LoadLastRecentlyUsedFileList();
	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	// Document template and doc/view
  // https://msdn.microsoft.com/en-us/library/hts9a4xz.aspx
	// https://msdn.microsoft.com/en-us/library/d1e9fe7d.aspx
	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to create CSingleDocTemplate()\n");
  pDocTemplate = GUI()->CreateCSingleDocTemplate();
	AddDocTemplate(pDocTemplate);
	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to EnableShellOpen()\n");
	EnableShellOpen();
	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to RegisterShellFileTypes(false)\n");
	///RegisterShellFileTypes(false);
  write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to InitializeThreads()\n");
  InitializeThreads();
  write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to OpenLastRecentlyUsedFile()\n");
  Formula()->FormulaParser()->ParseDefaultLibraries(); 
	OpenLastRecentlyUsedFile();
	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] m_pMainWnd = %i\n",
		m_pMainWnd);
	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Posting message that finishes initialization later\n");
  write_log(Preferences()->debug_openholdem(), "[OpenHoldem] FinishInitialization()\n");
  write_log(Preferences()->debug_openholdem(), "[OpenHoldem] m_pMainWnd = %i\n",
    m_pMainWnd);
  // The one and only window has been initialized, so show and update it
  if (Preferences()->gui_first_visible() && (SessionCounter()->session_id() == 0)) {
    m_pMainWnd->ShowWindow(SW_SHOW);
  } else {
    m_pMainWnd->ShowWindow(SW_MINIMIZE);
  }
  m_pMainWnd->UpdateWindow();
  // call DragAcceptFiles only if there's a suffix
  // In an SDI app, this should occur after ProcessShellCommand
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();
  // Bring main window to front
  m_pMainWnd->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  m_pMainWnd->SetActiveWindow();
  m_pMainWnd->SetFocus();
  m_pMainWnd->SetForegroundWindow();
	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] InitInstance done\n");
	return TRUE;
}

int COpenHoldemApp::ExitInstance() {
  // timers and threads are already stopped 
  // by CMainFrame::DestroyWindow().
  // Now we cancontinue with singletons.
  stop_log();
	return CWinApp::ExitInstance();
}

void COpenHoldemApp::LoadLastRecentlyUsedFileList() {
	// Added due to inability to get standard LoadStdProfileSettings working properly
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList == NULL);

	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to load file history\n");
	if (kNumberOfLastRecentlyUsedFilesInFileMenu > 0) 
	{
		// create file MRU since nMaxMRU not zero
		m_pRecentFileList = new CRecentFileList(0, _afxFileSection, _afxFileEntry, 
			kNumberOfLastRecentlyUsedFilesInFileMenu);
		m_pRecentFileList->ReadList();
	}
	// 0 by default means not set
	m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);
}

void COpenHoldemApp::StoreLastRecentlyUsedFileList() {
	m_pRecentFileList->WriteList();
}

void COpenHoldemApp::OpenLastRecentlyUsedFile() {
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to open last recently used file\n");
	// Open the most recently used file. (First on the MRU list.) Get the last
	// file from the registry. We need not account for cmdInfo.m_bRunAutomated and
	// cmdInfo.m_bRunEmbedded as they are processed before we get here.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)	{
		CString sLastPath(GetProfileString(_afxFileSection, "File1"));
    if (!sLastPath.IsEmpty())	{
			write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Last path: %s\n", 
				sLastPath);
			CFile f;
			// If file is there, set to open!
			if (f.Open(sLastPath, CFile::modeRead | CFile::shareDenyWrite))	{
				cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
				cmdInfo.m_strFileName = sLastPath;
				f.Close();
			}
		}
	}
	write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to dispatch command-line\n");
	// Dispatch commands specified on the command line.  Will fail if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.	
	if (!ProcessShellCommand(cmdInfo)) {
		write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Dispatching command-line failed\n");
	}
}

CHeartbeatThread hbt; ///!!!!!

void COpenHoldemApp::InitializeThreads() {
  // Heartbeat thread cares about everything: connecting, scraping, playing
  write_log(Preferences()->debug_openholdem(), "[OpenHoldem] Going to start heartbeat thread\n");
  assert(_p_heartbeat_thread == NULL);
  _p_heartbeat_thread = &hbt; ///!!!new CHeartbeatThread();
  assert(_p_heartbeat_thread != NULL);
  _p_heartbeat_thread->StartThread();
}