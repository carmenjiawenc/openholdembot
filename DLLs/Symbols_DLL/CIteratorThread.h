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
// Purpose: PrWin-simulation, HoldEm only, not Omaha
//
//******************************************************************************

#include <afxwin.h>
#include "inlines/eval.h"
///#include "CSpaceOptimizedGlobalObject.h"
#include "..\..\pokereval\include\poker_defs.h"
#include "..\..\pokereval\include\rules_std.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

//prwin 1326 chair structure    Matrix 2008-04-29
struct sprw1326_chair {
  int			level;			// indicates significance level for 'always consider'
  int			limit;			// max index into significance array - used for computational efficiency
  int			ignore;			// if non-zero no weighting will be applied to this chair
  int			rankhi[k_number_of_pocketcard_combinations_without_order];	// hi card number in pocket cards
  int			ranklo[k_number_of_pocketcard_combinations_without_order];	// lo card number in pocket cards
  int			weight[k_number_of_pocketcard_combinations_without_order];	// the significance value for this hand
  double		scratch;		// for future reference
};

//prwin 1326 structure			Matrix 2008-04-29
struct sprw1326
{
  int			useme;				// unless set to 1326 the normal OH prwin will be used
  int			preflop;			// unless set to 1326 the normal OH prwin will be used pre-flop
  int			usecallback;		// unless set to 1326 the callback function will not be invoked
  double(*prw_callback)(); // if enabled will be invoked before the prwin calculation pass
  double	scratch;			// for future reference
  sprw1326_chair	vanilla_chair;	// will be precalculated by OH at startup - convenience values
  sprw1326_chair  chair[kMaxNumberOfPlayers];  // structures for each chair
};

class CIteratorThread /*!!!!!: public CSpaceOptimizedGlobalObject */{
 public:
	// public functions
	CIteratorThread();
	~CIteratorThread();
 public:
  // Public accessors()
  double prwin()   { return _prwin; }
  double prtie()   { return _prtie; }
  double prlos()   { return _prlos; }
 public:
  static bool IteratorThreadComplete() { return _iterations_calculated >= _iterations_required; }
  bool IteratorThreadWorking()    { return ((_iterations_calculated > 0) && (_iterations_calculated < _iterations_required)); }
  int  IteratorThreadProgress()   { return _iterations_calculated; }
 public:
	void StartPrWinComputationsIfNeeded();
	///void set_prw1326_useme(const int i)	{ _prw1326.useme = i;}
	///const	sprw1326 *prw1326()	          { return &_prw1326; }
 public:
  // For the DLL "cmd$recalc"
  void RestartPrWinComputations();
public:
  void* prw1326(); /// to do !!!
#undef ENT
 private:
	// private functions and variables - not available via accessors or mutators
	static UINT IteratorThreadFunction(LPVOID pParam);
	static void AdjustPrwinVariablesIfNecessary();
	static void StandardDealingAlgorithm(int nopponents);
	static void SwapDealingAlgorithmForMoreThan13Opponents(int nopponents);
	static void StandardDealingAlgorithmForUpTo13Opponents(int nopponents);
	static int EnhancedDealingAlgorithm();
	static bool UseEnhancedPrWin();
	static int  GetRandomCard();	
 private:
	static void UpdateIteratorVarsForDisplay();
	static void ResetIteratorVars();
	static void ResetGlobalVariables();
	static void CalculateTotalWeights();
 private:
	void InitIteratorLoop();
	void InitHandranktTableForPrwin();
	void CloneVanillaChairToAllOtherChairs();
	void InitNumberOfIterations();
 private:
	// variables for iterator loop
	CardMask		_plCards, _comCards;
	HANDLE			_m_wait_thread;
  static HANDLE			_m_stop_thread; //!!!!!
 private:
  static int _iterations_calculated;
  static int _iterations_required;
  static int _total_weight[kMaxNumberOfPlayers];
  static int _nopponents;;
 private:
  static double _prwin, _prtie, _prlos;
};