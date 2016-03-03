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

#include "InputManager.hpp"

namespace WC
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	InputManager::InputManager() :
		m_inputState(0),
		m_previousinputState(0),
		m_repeatCode(0xffffffff),
		m_initialDelay(sf::milliseconds(350)),
		m_activeDelay(sf::milliseconds(350)),
		m_repeatDelay(sf::milliseconds(50))
	{
		for (int i = 0; i < sf::Joystick::AxisCount; ++i)
			m_deadZone[i] = 50.0f;
	}

	//-----------------------------------------------------------------------------
	void InputManager::setRepeatForCode(sf::Uint32 aCode)
	{
		m_repeatCode |= aCode;
	}

	//-----------------------------------------------------------------------------
	void InputManager::clearRepeatForCode(sf::Uint32 aCode)
	{
		m_repeatCode &= ~aCode;
	}

	//-----------------------------------------------------------------------------
	sf::Uint32 InputManager::getManagedInputState(sf::Time elapsedTime)
	{
		sf::Uint32 inputState = m_inputState;

		if (m_previousinputState == inputState)
		{
			// Mask off inputs that need debouncing
			inputState &= m_repeatCode;
			m_activeDelay -= elapsedTime;
			if (m_activeDelay > sf::Time::Zero)
				return 0;
			else
				m_activeDelay = m_repeatDelay;
		}
		else
		{
			m_previousinputState = inputState;
			m_activeDelay = m_initialDelay;
		}

		return inputState;
	}

	//-----------------------------------------------------------------------------
	void InputManager::serviceInputs(InputKeyMap& inputKeyMap, InputJoyMap& inputJoyMap)
	{
		m_inputState = 0;
		for (InputKeyMap::iterator keyI = inputKeyMap.begin(); keyI != inputKeyMap.end(); ++keyI)
		{
			if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(keyI->first)))
				m_inputState |= keyI->second;
		}

		for (InputJoyMap::iterator joyI = inputJoyMap.begin(); joyI != inputJoyMap.end(); ++joyI)
		{
			JoyKeyVal jkv = joyI->first;
			int joyStick = jkv.m_joy;
			int index = jkv.m_index;

			// This call is just a lookup in an array
			if (sf::Joystick::isConnected(joyStick))
			{
				if (jkv.m_axis)
				{
					float axisVal = sf::Joystick::getAxisPosition(joyStick, static_cast<sf::Joystick::Axis>(index));
					if (jkv.m_dir)
					{
						// Positive values trigger past the deadzone
						if (axisVal > m_deadZone[index])
							m_inputState |= joyI->second;
					}
					else
					{
						// Negative trigger so make sure it's negative but past the deadzone
						if (axisVal < 0 && -axisVal > m_deadZone[index])
							m_inputState |= joyI->second;
					}
				}
				else
				{
					if (sf::Joystick::isButtonPressed(joyStick, index))
						m_inputState |= joyI->second;
				}
			}
		}
	}
}
