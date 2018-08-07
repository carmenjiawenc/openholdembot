//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: checking the game-state for inconsistencies
//
//******************************************************************************

#include "CValidator.h"
#include "CAutoplayer.h"
#include "..\CasinoInterface_DLL\CCasinoInterface.h"
#include "..\Preferences_DLL\Preferences.h"
#include "..\Symbols_DLL\CEngineContainer.h"
#include "..\Symbols_DLL\CSymbolEngineIsTournament.h"
#include "..\Symbols_DLL\CSymbolEngineReplayFrameController.h"
#include "..\Symbols_DLL\CSymbolEngineVersus.h"
#include "..\GamestateValidation_DLL\GamestateValidation.h"

CValidator::CValidator() {
  _enabled_manually = false;
}

CValidator::~CValidator() {
}

void CValidator::SetEnabledManually(bool Enabled) {
	_enabled_manually = Enabled;
}

void CValidator::Validate() {
  assert(EngineContainer() != nullptr);
  if (!CasinoInterface()->IsMyTurn()) {
    // Validate only if it is my turn.
    //   * because then we have stable frames
    //   * because then it matters most
    return;
  }
	if (Preferences()->validator_enabled()
		  // Manually enabled via toolbar?
		  || (_enabled_manually)) {
    bool success = ValidateGamestate(Preferences()->validator_use_heuristic_rules(),
      EngineContainer()->symbol_engine_istournament()->istournament(),
      EngineContainer()->symbol_engine_versus()->VersusBinLoaded());
    if (success) {
      return;
    }
  }
  // Failed consistence-checks
  if (Preferences()->validator_shoot_replayframe_on_error()) {
    EngineContainer()->symbol_engine_replayframe_controller()->ShootReplayFrameIfNotYetDone();
  }
  if (Preferences()->validator_stop_on_error()) {
    ///assert(p_autoplayer != nullptr);
    ///p_autoplayer->EngageAutoplayer(false);
  }
}
