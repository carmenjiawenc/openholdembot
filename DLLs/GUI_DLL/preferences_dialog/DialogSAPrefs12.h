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

#include "afxwin.h"
#include "SAPrefsDialog.h"
#include "..\resource.h"

// CDlgSAPrefs12 dialog

class CDlgSAPrefs12 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs12)

public:
	CDlgSAPrefs12(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs12();

// Dialog Data
	enum { IDD = IDD_SAPREFS12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();   
};
