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

#include <afxcmn.h>
#include "SAPrefsDialog.h"
#include "..\resource.h"

// CDlgSAPrefs8 dialog

class CDlgSAPrefs8 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs8)

public:
	CDlgSAPrefs8(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs8();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	enum { IDD = IDD_SAPREFS8 };
public:
	CButton m_RecordNever;
	CButton m_RecordMyTurn;
  CButton m_RecordEveryChangePlaying;
	CButton m_RecordEveryChange;
	CEdit m_MaxFrames;
	CSpinButtonCtrl m_MaxFramesSpin;
};
