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

// CDlgSAPrefs14 dialog

class CDlgSAPrefs14 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs14)

public:
	CDlgSAPrefs14(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs14();

private:
// Dialog Data
	enum { IDD = IDD_SAPREFS14 };
	CButton _autoconnector_connect_never;
	CButton _autoconnector_connect_permanent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};