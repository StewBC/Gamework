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

#include "Game.hpp"
#include "ExInitials.hpp"
#include "ExHighScreen.hpp"
#include "ExGlobals.hpp"
#include "FrontEnd.hpp"
#include "LevelManager.hpp"
#include "EffectsMgr.hpp"

namespace Gamework
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	Game::Game() :
		m_gameState(StatePreFrontEnd),
		m_frontEnd(new FrontEnd),
		m_levelManager(new LevelManager),
		m_inputKeyMapFE(),
		m_inputKeyMapGame()
	{
		setupDefaultKeyMappings(m_inputKeyMapFE);
		m_inputKeyMapGame = m_inputKeyMapFE;

		setupDefaultJoyMappings(m_inputJoyMapFE);
		m_inputJoyMapGame = m_inputJoyMapFE;
	}

	//-----------------------------------------------------------------------------
	Game::~Game()
	{ 
		delete m_frontEnd; 
		delete m_levelManager; 
	}

	//-----------------------------------------------------------------------------
	void Game::setupDefaultKeyMappings(WC::InputKeyMap& aKeyMap)
	{

		WC::InputKeyMap defaultMap;

		// Setup default key actions
		defaultMap[sf::Keyboard::Up] = INPUT_UP;
		defaultMap[sf::Keyboard::Down] = INPUT_DOWN;
		defaultMap[sf::Keyboard::Left] = INPUT_LEFT;
		defaultMap[sf::Keyboard::Right] = INPUT_RIGHT;
		defaultMap[sf::Keyboard::F1] = INPUT_ACTION;
		defaultMap[sf::Keyboard::Num1] = INPUT_1PLAYER;
		defaultMap[sf::Keyboard::Num2] = INPUT_2PLAYER;
		defaultMap[sf::Keyboard::Return] = INPUT_SELECT;
		defaultMap[sf::Keyboard::Escape] = INPUT_CANCEL;

		aKeyMap.swap(defaultMap);
	}

	//-----------------------------------------------------------------------------
	void Game::setupDefaultJoyMappings(WC::InputJoyMap& aJoyMap)
	{
		WC::InputJoyMap defaultMap;

		// Joystick buttons - This is for an XBox joystick
		for (int i = 0; i < 2 ; ++i)
		{
			defaultMap[WC::JoyKeyVal(i, 1, 1, 0)] = INPUT_UP;
			defaultMap[WC::JoyKeyVal(i, 1, 1, 1)] = INPUT_DOWN;
			defaultMap[WC::JoyKeyVal(i, 1, 0, 0)] = INPUT_LEFT;
			defaultMap[WC::JoyKeyVal(i, 1, 0, 1)] = INPUT_RIGHT;
			defaultMap[WC::JoyKeyVal(i, 0, 3, 0)] = INPUT_ACTION;
			defaultMap[WC::JoyKeyVal(i, 0, 6, 0)] = INPUT_1PLAYER;
			defaultMap[WC::JoyKeyVal(i, 0, 7, 0)] = INPUT_2PLAYER;
			defaultMap[WC::JoyKeyVal(i, 0, 0, 0)] = INPUT_SELECT;
			defaultMap[WC::JoyKeyVal(i, 0, 1, 0)] = INPUT_CANCEL;
		}

		aJoyMap.swap(defaultMap);
	}

	//-----------------------------------------------------------------------------
	bool Game::run(sf::Time elapsedTime)
	{
		WC::InputKeyMap& serviceKeyMap = m_inputKeyMapGame;
		WC::InputJoyMap& serviceJoyMap = m_inputJoyMapGame;
		if (m_gameState == StateEnterInitials || m_gameState == StateFrontEnd)
		{
			serviceKeyMap = m_inputKeyMapFE;
			serviceJoyMap = m_inputJoyMapFE;
		}
		gInputMgr->serviceInputs(serviceKeyMap, serviceJoyMap);

		switch (m_gameState)
		{
		case StatePreFrontEnd:
			m_frontEnd->preProcess();
			m_gameState = StateFrontEnd;
			// Fall through intentionally

		case StateFrontEnd:
			if (m_frontEnd->run(elapsedTime))
				break;
			m_gameState = StateNewGame;
			gActivePlayerNumber = 0;
			// Fall through intentionally

		case StateNewGame:
		{
			for (int i = 0; i<gNumPlayersThisGame; ++i)
			{
				gScorePerPlayer[i] = 0;
				gLivesLeftPerPlayer[i] = Examples::PlayersStartLives;
			}
			gActivePlayerNumber = 0;
			m_gameState = StateInitNewLevel;
			// Fall through intentionally
		}

		case StateInitNewLevel:
			m_levelManager->loadlevel(gCurrentLevelPerPlayer[gActivePlayerNumber]);
			m_gameState = StateReinitLevel;
			// Fall through intentionally

		case StateReinitLevel:
			m_gameState = StatePlaying;

			// Fall through intentionally

		case StatePlaying:
			m_gameState = StatePlayerDie;
			// Take this out - just makes a score to show
			// entering initials
			gScorePerPlayer[gActivePlayerNumber] = rand() % 100;
			break;

		case StateLevelEnd:
			m_gameState = StateLevelStats;
			break;

		case StateLevelStats:
			gCurrentLevelPerPlayer[gActivePlayerNumber]++;
			m_gameState = StateInitNewLevel;
			break;

		case StatePlayerDie:
		{
			if (!(--gLivesLeftPerPlayer[gActivePlayerNumber]))
			{
				m_gameState = StateGameOver;
				break;
			}
			m_gameState = StateNextPlayer;
			// Fall through intentionally
		}

		case StateNextPlayer:
			for (int i = 0; i < gNumPlayersThisGame; ++i)
			{
				if (++gActivePlayerNumber >= gNumPlayersThisGame)
					gActivePlayerNumber = 0;

				if (gLivesLeftPerPlayer[gActivePlayerNumber])
				{
					m_gameState = StateReinitLevel;
					break;
				}
			}
			// If this is true, no players are alive anymore
			if (StateNextPlayer == m_gameState)
				m_gameState = StatePreFrontEnd;
			break;

		case StateGameOver:
			if (Examples::gTodaysHighScores.makesItOntoTable(gScorePerPlayer[gActivePlayerNumber]))
				m_gameState = StatePreEnterInitials;
			else
				m_gameState = StateNextPlayer;
			break;

		case StatePreEnterInitials:
			m_exampleEnterInitials = new Examples::ReadInitials;
			m_exampleShowHighScores = new Examples::HighScoreScreen;
			m_gameState = StateEnterInitials;
			// Fall through intentionally

		case StateEnterInitials:
			if (!m_exampleEnterInitials->run(elapsedTime))
			{
				delete m_exampleEnterInitials;
				delete m_exampleShowHighScores;
				m_gameState = StateNextPlayer;
			}
			else
			{
				m_exampleShowHighScores->run(elapsedTime);
			}
			break;
		}
		gEffectsMgr->run(elapsedTime);
		return true;
	}
}
