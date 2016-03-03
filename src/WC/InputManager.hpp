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

#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include <SFML/Graphics.hpp>

namespace WC
{
	//-----------------------------------------------------------------------------
	// This class makes a bit-field Uint32 that consists of
	// The Joystick number 0..7
	// a bool to indicate if it's an axis (1) or button (0)
	// an Index to the axis (0..7) or button (0..31) number
	// a bool to indicate if the axis is triggered positively(1) or negatively (0)
	//-----------------------------------------------------------------------------
	class JoyKeyVal
	{
	public:
		sf::Uint32 m_joy : 3, m_axis : 1, m_index : 5, m_dir : 1;
		JoyKeyVal(int aJoyNum, bool isAxis, int aIndex, bool isPositive) : m_joy(aJoyNum), m_axis(isAxis), m_index(aIndex), m_dir(isPositive) { ; }
	};
	inline bool operator==(const JoyKeyVal lhs, const JoyKeyVal rhs) { return lhs.m_joy == rhs.m_joy && lhs.m_axis == rhs.m_axis && lhs.m_index == rhs.m_index && lhs.m_dir == rhs.m_dir; }

	//-----------------------------------------------------------------------------
	inline bool operator<(const JoyKeyVal lhs, const JoyKeyVal rhs)
	{
		return *(sf::Uint32*)&lhs < *(sf::Uint32*)&rhs;
	}

	//-----------------------------------------------------------------------------
	typedef std::map<sf::Uint32, sf::Uint32> InputKeyMap;
	typedef std::map<JoyKeyVal, sf::Uint32> InputJoyMap;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class InputManager
	{
	private:
		sf::Uint32			m_inputState;
		sf::Uint32			m_previousinputState;
		sf::Uint32			m_repeatCode;
		sf::Time			m_initialDelay;
		sf::Time			m_repeatDelay;
		sf::Time			m_activeDelay;
		float				m_deadZone[sf::Joystick::AxisCount];

	public:
		InputManager();

		void setRepeatForCode(sf::Uint32 aCode);
		void clearRepeatForCode(sf::Uint32 aCode);
		void serviceInputs(InputKeyMap& inputMap, InputJoyMap& inputJoyMap);

		sf::Uint32 getInputState(void) { return m_inputState; }
		sf::Uint32 getManagedInputState(sf::Time elapsedTime);
	};
}

#endif //_INPUTMANAGER_HPP_
