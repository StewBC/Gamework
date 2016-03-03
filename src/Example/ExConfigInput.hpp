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

#ifndef _EXCONFIGINPUT_HPP_
#define _EXCONFIGINPUT_HPP_

#include "InputManager.hpp"
#include <SFML/Graphics.hpp>

namespace Examples
{
	//-----------------------------------------------------------------------------
	enum
	{
		KeyBlock,
		JoyBlock,
		LabelBlock,
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class ConfigInput
	{
	private:
		WC::InputKeyMap  m_configKeyMap;
		WC::InputJoyMap  m_configJoyMap;
		WC::InputKeyMap& m_realKeyMap;
		WC::InputJoyMap& m_realJoyMap;
		bool			 m_keySide;
		int				 m_currSelection;
		bool			 m_isSetting;
		bool			 m_isDone;
		bool			 m_cancelButton;
		sf::Vector2f	 m_transition[LabelBlock];
		sf::Vector2f	 m_sideSelectorBar;
		sf::Vector2f	 m_cursorBar;
		sf::Vector2f	 m_cancelBar;

		void drawBlocks(int aDrawType, int aSelection, bool aActive);
		void drawScreen(void);
		bool handleCancelScreen(sf::Uint32 keyState);
		bool handleSetting(void);
		void handleKey(sf::Uint32 keyState);

	public:
		ConfigInput(WC::InputKeyMap& aKeyMap, WC::InputJoyMap& aJoyMap);
		~ConfigInput() { ; }

		bool run(sf::Time elapsedTime, sf::Uint32 keyState);
	};
}

#endif //_EXCONFIGINPUT_HPP_
