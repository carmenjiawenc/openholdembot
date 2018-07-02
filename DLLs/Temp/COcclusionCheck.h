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
// Purpose: Heuristic occlusion-check for rebuy.
//
//******************************************************************************

///#include "CSpaceOptimizedGlobalObject.h"

class COcclusionCheck/*#: public CSpaceOptimizedGlobalObject */{
public:
	COcclusionCheck();
	~COcclusionCheck();
public:
	bool UserBalanceOccluded();
private:
	bool UserChairKnown();
	bool UserBalanceNonZero();
	bool UserNameKnown();
	bool AnyOpponentNameKnown();
	bool AnyApponentBalanceNonZero();
};