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

// The betround-calculator is not a normal symbol-engine,
// because changes in the betround need to be known
// by the symbol-engine-container / all other symbol-engines
// in advance (just like hand-resets).

#define BETROUND BetroundCalculator()->betround()

class CBetroundCalculator
{
public:
	CBetroundCalculator();
	~CBetroundCalculator();
public:
	void OnNewHeartbeat();
public:
  int betround();
  int PreviousRound();
public:
	bool IsNewBetround();
private:
	int _betround;
	int _betround_previous_heartbeat;
};

CBetroundCalculator* BetroundCalculator(); 