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

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "InputManager.hpp"
#include <SFML/Graphics.hpp>

namespace Examples
{
	class ReadInitials;
	class HighScoreScreen;
}

namespace Gamework
{
	// Utility
	#define SET_BIT(x)	(1<<(x))

	//-----------------------------------------------------------------------------
	typedef enum 
	{
		StatePreFrontEnd,
		StateFrontEnd,
		StateNewGame,
		StateInitNewLevel,
		StateReinitLevel,
		StatePlaying,
		StateLevelEnd,
		StateLevelStats,
		StatePlayerDie,
		StateNextPlayer,
		StateGameOver,
		StatePreEnterInitials,
		StateEnterInitials,
	} GameState;

	enum
	{
		INPUT_UP		= SET_BIT(0),
		INPUT_DOWN		= SET_BIT(1),
		INPUT_LEFT		= SET_BIT(2),
		INPUT_RIGHT		= SET_BIT(3),
		INPUT_ACTION	= SET_BIT(4),
		INPUT_1PLAYER	= SET_BIT(5),
		INPUT_2PLAYER	= SET_BIT(6),
		INPUT_SELECT	= SET_BIT(7),
		INPUT_CANCEL	= SET_BIT(8),
	};

	enum
	{
		RenderLayerBackground,
		RenderLayerGame,
		RenderLayerFrontEnd,

		RenderLayerNumLayers,
	};

	//-----------------------------------------------------------------------------
	class FrontEnd;
	class LevelManager;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class Game
	{
	private:
		GameState					m_gameState;
		FrontEnd*					m_frontEnd;
		Examples::ReadInitials*		m_exampleEnterInitials;
		Examples::HighScoreScreen*	m_exampleShowHighScores;
		LevelManager*				m_levelManager;
		WC::InputKeyMap				m_inputKeyMapFE;
		WC::InputKeyMap				m_inputKeyMapGame;
		WC::InputJoyMap				m_inputJoyMapFE;
		WC::InputJoyMap				m_inputJoyMapGame;

		void initGame(void);

	public:
		Game();
		~Game();

		void setupDefaultKeyMappings(WC::InputKeyMap& aKeyMap);
		void setupDefaultJoyMappings(WC::InputJoyMap& aJoyMap);
			
		WC::InputKeyMap& getInputKeyMapFE(void)   { return m_inputKeyMapFE; }
		WC::InputKeyMap& getInputKeyMapGame(void) { return m_inputKeyMapGame; }
		WC::InputJoyMap& getInputJoyMapFE(void)   { return m_inputJoyMapFE; }
		WC::InputJoyMap& getInputJoyMapGame(void) { return m_inputJoyMapGame; }

		bool run(sf::Time elapsedTime);
	};
}

#endif //_GAME_HPP_
