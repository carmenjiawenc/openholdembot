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

#include "StdAfx.h"
#include "CStringMatch.h"
#include "..\CTablemap\CTablemap.h"

CStringMatch *p_string_match = NULL;

CStringMatch::CStringMatch(void) {
}

CStringMatch::~CStringMatch(void) {
}

const bool CStringMatch::IsNumeric(const CString t) { // !!! needs to be changed
  int num_dots = 0;
  int nums_after_dot = 0;
  // Check for bad parameters
  if (!t || t == "") {
    return false;
  }
	for (int i=0; i<t.GetLength(); ++i)	{
    if (CString(t[i]).FindOneOf("$0123456789,.¢ckm") == kUndefined) {
      return false;
    }
    // Rest of IsNumeric significantly simplified
    // * there are casinos with 1 or 2 digits after the last dot
    // * there are casinos with multiple dots like $3.456.123.8
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19658
  }
  return true;
}

const bool CStringMatch::IsStringSeated(const CString s) {
  // Check for bad parameters
  if (!s || s == "") {
    return false;
  }
  CString s_lower_case = s;
  s_lower_case.MakeLower();
	if (s_lower_case.Left(5) == "false" || s_lower_case.Left(8) == "unseated"){
		return false;
	}	else {
		return (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "seated");
	}
}

const bool CStringMatch::IsStringActive(const CString s) {
	// Check for bad parameters
  if (!s || s == "") {
    return false;
  }
	CString s_lower_case = s;
  s_lower_case.MakeLower();
  if (s_lower_case.Left(5) == "false"
			|| s_lower_case.Left(8) == "inactive"
			|| s_lower_case.Left(3) == "out"
			|| s_lower_case.Left(6) == "sitout"
			|| s_lower_case.Left(4) == "away") {
    return false;
  }
	// old method: inactive unless pXactive returns true/active
  if (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "active") {
    return true;
  }
  assert(kThisMustNotHappen);
  return false;
}

const bool CStringMatch::IsStringCardback(const CString s) {
	// Check for bad parameters
  if (!s || s == "") {
    return false;
  }
  CString s_lower_case = s;
	s_lower_case.MakeLower();
	return (s_lower_case.Left(4) == "true" || s_lower_case.Left(8) == "cardback");
}

const bool CStringMatch::IsStringDealer(const CString s) {
  // Check for bad parameters
  if (!s || s == "") {
    return false;
  }
	CString s_lower_case = s;
  s_lower_case.MakeLower();
	return (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "dealer");
}

const bool CStringMatch::IsStringAllin(const CString s) {
  // Check for bad parameters
  if (!s || s == "") {
    return false;
  }
  CString s_lower_case = s;
  s_lower_case.MakeLower();
  s_lower_case.Remove(' ');
  s_lower_case.Remove('-');
  return (s_lower_case.Left(5) == "allin"
    || s_lower_case.Left(5) == "a11in"
    || s_lower_case.Left(5) == "allln"
    || s_lower_case.Left(5) == "a111n"
    || s_lower_case.Left(5) == "aiiin"
    || s_lower_case.Left(5) == "buyin"
    || s_lower_case.Left(3) == "max"
    || MatchByLevenshteinDistance(s_lower_case) == "allin");
}

const int CStringMatch::uiLevenshteinDistance(const std::string& s1, const std::string& s2) {
    const int m(s1.size());
    const int n(s2.size());

    if (m == 0) return n;
    if (n == 0) return m;

    auto* costs = new int[n + 1];

    for (int k = 0; k <= n; k++) costs[k] = k;

    int i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i) {
        costs[0] = i + 1;
        int corner = i;

        int j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j) {
            int upper = costs[j + 1];
            if (*it1 == *it2) {
                costs[j + 1] = corner;
            }
            else {
                int t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}

const CString CStringMatch::MatchByLevenshteinDistance(const CString s) {
    CString action_types[] = {"bet", "raise", "fold", "check", "call", "allin", "confirm"};
    int min = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < 6; i++) {
        auto dist = uiLevenshteinDistance(action_types[i].GetString(), s.GetString());
        if (dist < min) {
            min_index = i;
            min = dist;
        }
    }
    return action_types[min_index];
}