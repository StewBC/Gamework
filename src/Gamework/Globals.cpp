//
//  Gamework - Arcade game building framework for SFML
//
//  Created by Stefan Wessels.
//  Copyright (c) 2016 Wessels Consulting Ltd. All rights reserved.
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely.
//

#include "Globals.hpp"

//-----------------------------------------------------------------------------
namespace Gamework
{
	// Global access pointers
	WC::Render* gRenderer = 0;
	WC::InputManager* gInputMgr = 0;
	WC::EffectsManager* gEffectsMgr = 0;
	Gamework::Game* gGame = 0;

	// Initialized right here
	int gNumPlayersThisGame = 0;
	int gActivePlayerNumber = 0;

	// The game needs to init these
	int gCurrentLevelPerPlayer[MaxPlayersPerGame];
	int gLivesLeftPerPlayer[MaxPlayersPerGame];
	int gScorePerPlayer[MaxPlayersPerGame];
}
