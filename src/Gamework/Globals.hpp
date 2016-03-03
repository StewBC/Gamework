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

#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

#include <SFML/Graphics.hpp>

//-----------------------------------------------------------------------------
namespace WC
{
	class Render;
	class EffectsManager;
	class InputManager;
}

//-----------------------------------------------------------------------------
namespace Gamework
{
	class Game;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace Gamework
{ 
	// Access to Game, InputManager EffectsManager and Renderer from anywhere
	extern WC::Render*			gRenderer;
	extern WC::InputManager*	gInputMgr;
	extern WC::EffectsManager*	gEffectsMgr;
	extern Gamework::Game*		gGame;

	// Player related
	const int MaxPlayersPerGame = 2;
	extern int gNumPlayersThisGame;
	extern int gActivePlayerNumber;
	extern int gCurrentLevelPerPlayer[MaxPlayersPerGame];
	extern int gLivesLeftPerPlayer[MaxPlayersPerGame];
	extern int gScorePerPlayer[MaxPlayersPerGame];
}

#endif //_GLOBALS_HPP_
