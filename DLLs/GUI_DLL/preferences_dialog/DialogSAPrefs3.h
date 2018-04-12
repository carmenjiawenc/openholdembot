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

#include "SAPrefsDialog.h"
#include "..\resource.h"

// CDlgSAPrefs3 dialog

class CDlgSAPrefs3 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs3)

public:
	CDlgSAPrefs3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs3();

// Dialog Data
	enum { IDD = IDD_SAPREFS3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
};