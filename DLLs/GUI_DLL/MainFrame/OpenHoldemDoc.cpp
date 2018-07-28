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

// OpenHoldemDoc.cpp : implementation of the COpenHoldemDoc class
//

#define GUI_DLL_EXPORTS

#include "CAutoplayer.h"
#include "CFormulaParser.h"
#include "..\DLLs\Symbols_DLL\CFunctionCollection.h"
#include "COpenHoldemHopperCommunication.h"
#include "COpenHoldemTitle.h"

#include "DialogFormulaScintilla.h"
#include "MainFrm.h"
#include "..\DLLs\WindowFunctions_DLL\window_functions.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"

// COpenHoldemDoc
IMPLEMENT_DYNCREATE(COpenHoldemDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenHoldemDoc, CDocument)
END_MESSAGE_MAP()

// COpenHoldemDoc construction/destruction
COpenHoldemDoc::COpenHoldemDoc() {
	write_log(Preferences()->debug_openholdem(), "[COpenHoldemDoc] Going to call  FunctionCollection()->DeleteAll\n");
	FunctionCollection()->DeleteAll(false, true);
}

COpenHoldemDoc::~COpenHoldemDoc() {
}

BOOL COpenHoldemDoc::SaveModified()
{
	if (GUI()->DlgFormulaScintilla())
	{
		if (GUI()->DlgFormulaScintilla()->m_dirty)
		{
			if (MessageBox_Interactive(
				"The Formula Editor has un-applied changes.\n"
				"Really exit?", 
				"Formula Editor", MB_ICONWARNING|MB_YESNO) == IDNO)
			{
				return false;
			}
		}

		// Kill the formula dialog
    if (GUI()->DlgFormulaScintilla()) {
      GUI()->DlgFormulaScintilla()->DestroyWindow();
    }
	}
	return CDocument::SaveModified();
}

BOOL COpenHoldemDoc::OnNewDocument() {
  if (!CDocument::OnNewDocument()) {
    return FALSE;
  }
	// Default bot
	FunctionCollection()->SetEmptyDefaultBot();
	SetModifiedFlag(false);
	GUI()->OpenHoldemTitle()->UpdateTitle();
	return true;
}


// COpenHoldemDoc serialization
void COpenHoldemDoc::Serialize(CArchive& ar) 
{
	// Writing a file
	if (ar.IsStoring()) 
	{
		// Store archive in the new OpenHoldem format
		FunctionCollection()->Save(ar);
		// Do not close this archive here.
		// It's expected to stay open at this point!
	}
	// Reading a file
	else 
	{
		// Extra caution, in case we want to load a formula,
		// while the autoplayer is engaged.
		// This currently can only happen via the MRU-list.
		// The alternative way would be to disable that list...
		//
		// MainFrame.cpp:
		// ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, &CMainFrame::OnUpdateLRUList)
		//
		// void CMainFrame::OnUpdateLRUList(CCmdUI *pCmdUI)
		// {
		//	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
		// }
		//
		// Unfortunatelly this does not work, 
		// but removes the list and replaces it once with the default: "Recent file".
		//
		// And there's very little information about both
		// ON_UPDATE_COMMAND_UI_RANGE and temporary disabling of MRU-lists.
		//
		// So we decided to go that route.
		//
		if (p_autoplayer->autoplayer_engaged())
		{
			MessageBox_Interactive("Can't load formula while autoplayer engaged.", "ERROR", 0);
			return;
		}
		// Read ohf file
    assert(OpenHoldem()->FormulaParser() != NULL);
		write_log(Preferences()->debug_openholdem(), "[COpenHoldemDoc::Serialize] Going to call OpenHoldem()->FormulaParser()->ParseFormulaFileWithUserDefinedBotLogic \n");
		OpenHoldem()->FormulaParser()->ParseFormulaFileWithUserDefinedBotLogic(ar);
		SetModifiedFlag(false);
		GUI()->OpenHoldemTitle()->UpdateTitle();
	}
}

COpenHoldemDoc * COpenHoldemDoc::GetDocument() 
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	return (COpenHoldemDoc *) pFrame->GetActiveDocument();
}

