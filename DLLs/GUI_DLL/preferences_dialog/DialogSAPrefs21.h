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

// CDlgSAPrefs21 dialog

class CDlgSAPrefs21 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs21)

public:
	CDlgSAPrefs21(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs21();

// Dialog Data
	enum { IDD = IDD_SAPREFS21 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CButton _position_tables_never_button;
	CButton _position_tables_tiled_button;
	CButton _position_tables_cascaded_button;
	CButton _position_always_keep_position;

protected:
	virtual void OnOK();
};