#pragma once
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

#include <afxwin.h>

class CGUI;
class CHeartbeatThread;

class COpenHoldemApp : public CWinApp {
 public:
	COpenHoldemApp();
	~COpenHoldemApp();
	void MyLoadStdProfileSettings(UINT nMaxMRU);
	virtual BOOL InitInstance();
	afx_msg void OnAppAbout();
	///DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
 public:
	void StoreLastRecentlyUsedFileList();
 private:
	void LoadLastRecentlyUsedFileList();
	void OpenLastRecentlyUsedFile();
public:
  CHeartbeatThread* HeartBeatThread(); //!!!!!
 private:
  void InitializeThreads();
private:
  CHeartbeatThread *_p_heartbeat_thread;
};

COpenHoldemApp* OpenHoldem(); //!!!!! not needed, GetInstance