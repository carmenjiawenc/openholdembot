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
// Purpose: Preferences dialog for auto-starter and auto-shutdown
//
//******************************************************************************

#include "SAPrefsDialog.h"
#include "..\resource.h"

// DialogSAPrefs9 dialog

class CDlgSAPrefs9 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs9)
public:
	CDlgSAPrefs9(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs9();
// Dialog Data
	enum { IDD = IDD_SAPREFS9 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
public:
	CButton m_use_auto_starter;
	CButton m_use_auto_shutdown;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
