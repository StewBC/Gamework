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

#include "ExConfigInput.hpp"
#include "ExGlobals.hpp"
#include "Game.hpp"
#include "Render.hpp"
#include "Globals.hpp"
#include "SpriteResourceManager.hpp"

namespace Examples
{
	const float Epsilon = 0.002f;
	const float LabelSpacing = 0.06f;
	const float SwapAnimSpeed = 0.08f;
	const float SideBarLeft = 0.152f;
	const float SideBarRight = 0.653f;
	const float SideBarY = 0.19f;
	const float SideLabelY = 0.18f;
	const float BarWidth = 120.0f;
	const float BarHeight = 30.0f;
	const float CursorTop = 0.31f;
	const float CursorWidth = 375.0f;
	const float BlockTop = 0.3f;
	const float LabelBlockX = 0.25f;
	const float ValueOnX = 0.6f;
	const float ValueOffX = 1.1f;
	const float CancelCursorLeft = 0.165f;
	const float CancelCursorRight = 0.665f;

	const unsigned int actions[] =
	{
		Gamework::INPUT_UP,
		Gamework::INPUT_DOWN,
		Gamework::INPUT_LEFT,
		Gamework::INPUT_RIGHT,
		Gamework::INPUT_ACTION,
		Gamework::INPUT_1PLAYER,
		Gamework::INPUT_2PLAYER,
		Gamework::INPUT_SELECT,
		Gamework::INPUT_CANCEL,
		0
	};

	const char* labels[] = 
	{ 
		"Up", 
		"Down", 
		"Left", 
		"Right", 
		"Action", 
		"1 Player", 
		"2 Player", 
		"Select", 
		"Cancel",
		"Reset to defaults"
	};
	const int numLabels = sizeof(labels) / sizeof(labels[0]);

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	ConfigInput::ConfigInput(WC::InputKeyMap& aKeyMap, WC::InputJoyMap& aJoyMap) :
		m_configKeyMap(aKeyMap),
		m_configJoyMap(aJoyMap),
		m_realKeyMap(aKeyMap),
		m_realJoyMap(aJoyMap),
		m_keySide(true),
		m_currSelection(0),
		m_isSetting(false),
		m_isDone(false),
		m_cancelButton(false)
	{
		m_transition[KeyBlock] = sf::Vector2f(ValueOnX, ValueOnX);
		m_transition[JoyBlock] = sf::Vector2f(ValueOffX, ValueOffX);
		m_sideSelectorBar = sf::Vector2f(SideBarLeft, SideBarLeft);
		m_cursorBar = sf::Vector2f(CursorTop, CursorTop);
		m_cancelBar = sf::Vector2f(CancelCursorLeft, CancelCursorLeft);
	}

	//-----------------------------------------------------------------------------
	void ConfigInput::drawBlocks(int aDrawType, int aSelection, bool aActive)
	{
		int colourIndex;
		
		for (int label = 0; label < numLabels; ++label)
		{
			colourIndex = (aActive && label == aSelection) ? ColourRed : ColourCyan;

			switch (aDrawType)
			{
				case KeyBlock:
					if (actions[label])
						Gamework::gRenderer->Print(gKeyNames[getKeyForKeyValueInMap(actions[label], m_configKeyMap)], "r*lc", m_transition[KeyBlock].x, BlockTop + label * LabelSpacing, Gamework::RenderLayerFrontEnd, &gColours[colourIndex]);
						break;

				case JoyBlock:
				{
					if (actions[label])
					{
						WC::JoyKeyVal jkv = getKeyForJoyValueInMap(actions[label], m_configJoyMap);
						std::string textStr;
						if (jkv.m_axis)
							textStr = gJoyNames[jkv.m_index] + std::string(jkv.m_dir ? " +" : " -");
						else
							textStr = "Button " + std::to_string(jkv.m_index);
						Gamework::gRenderer->Print(textStr, "r*lc", m_transition[JoyBlock].x, BlockTop + label * LabelSpacing, Gamework::RenderLayerFrontEnd, &gColours[colourIndex]);
					}
					break;
				}

				case LabelBlock:
					Gamework::gRenderer->Print(labels[label], "r*lc", LabelBlockX, BlockTop + label * LabelSpacing, Gamework::RenderLayerFrontEnd, &gColours[colourIndex]);
					break;
			}
		}
	}

	//-----------------------------------------------------------------------------
	void ConfigInput::drawScreen(void)
	{
		if (m_keySide)
		{
			if (!isCloseEnough(m_sideSelectorBar.x, SideBarLeft, Epsilon))
				evalCubic(m_sideSelectorBar.x, m_sideSelectorBar.y, SideBarLeft, SwapAnimSpeed);
			if (!isCloseEnough(m_transition[KeyBlock].x, ValueOnX, Epsilon))
				evalCubic(m_transition[KeyBlock].x, m_transition[KeyBlock].y, ValueOnX, SwapAnimSpeed);
			if (!isCloseEnough(m_transition[JoyBlock].x, ValueOffX, Epsilon))
				evalCubic(m_transition[JoyBlock].x, m_transition[JoyBlock].y, ValueOffX, SwapAnimSpeed);
		}
		else
		{
			if (!isCloseEnough(m_sideSelectorBar.x, SideBarRight, Epsilon))
				evalCubic(m_sideSelectorBar.x, m_sideSelectorBar.y, SideBarRight, SwapAnimSpeed);
			if (!isCloseEnough(m_transition[KeyBlock].x, ValueOffX, Epsilon))
				evalCubic(m_transition[KeyBlock].x, m_transition[KeyBlock].y, ValueOffX, SwapAnimSpeed);
			if (!isCloseEnough(m_transition[JoyBlock].x, ValueOnX, Epsilon))
				evalCubic(m_transition[JoyBlock].x, m_transition[JoyBlock].y, ValueOnX, SwapAnimSpeed);
		}

		sf::RectangleShape sideBar(sf::Vector2f(BarWidth, BarHeight));
		sideBar.setPosition(640.0f * m_sideSelectorBar.x, 480.0f * SideBarY);
		sideBar.setOrigin(0.0f, BarHeight);
		sideBar.setFillColor(gColours[ColourYellow]);
		Gamework::gRenderer->draw(sideBar);

		float cursorY = CursorTop + (LabelSpacing * m_currSelection);
		if (!isCloseEnough(m_cursorBar.y, cursorY, Epsilon))
			evalCubic(m_cursorBar.y, m_cursorBar.x, cursorY, SwapAnimSpeed);

		sf::RectangleShape cursor(sf::Vector2f(CursorWidth, BarHeight));
		cursor.setPosition(640.0f * 0.2f, 480.0f * m_cursorBar.y);
		cursor.setOrigin(0.0f, BarHeight);
		cursor.setFillColor(gColours[ColourYellow]);
		Gamework::gRenderer->draw(cursor);

		Gamework::gRenderer->Print("Config Screen", "r*slcpax", 0.5f, 0.08f, 1.5f, Gamework::RenderLayerFrontEnd, &gColours[ColourViolet]);
		Gamework::gRenderer->Print("Keyboard", "r*slcpax", 0.25f, SideLabelY, 1.05f, Gamework::RenderLayerFrontEnd, &gColours[ColourViolet]);
		Gamework::gRenderer->Print("Joystick", "r*slcpax", 0.75f, SideLabelY, 1.05f, Gamework::RenderLayerFrontEnd, &gColours[ColourViolet]);

		std::string helpText = "Press " + std::string(gKeyNames[getKeyForKeyValueInMap(Gamework::INPUT_CANCEL, m_realKeyMap)]) + " to end";
		Gamework::gRenderer->Print(helpText, "lcpcxb", Gamework::RenderLayerFrontEnd, &gColours[ColourViolet]);

		drawBlocks(LabelBlock, m_currSelection, m_isSetting);

		if (m_transition[KeyBlock].x < 640.0f)
			drawBlocks(KeyBlock, m_currSelection, m_isSetting);

		if (m_transition[JoyBlock].x < 640.0f)
			drawBlocks(JoyBlock, m_currSelection, m_isSetting);
	}

	//-----------------------------------------------------------------------------
	bool ConfigInput::handleCancelScreen(sf::Uint32 keyState)
	{
		WC::SpriteAnimaion* spriteAnimaion = Gamework::gRenderer->GetResMgr()->findSpriteAnimation("sprites.png", "colors");

		if (m_cancelButton && keyState & Gamework::INPUT_LEFT)
			m_cancelButton = false;
		else if (!m_cancelButton && keyState & Gamework::INPUT_RIGHT)
			m_cancelButton = true;
		else if (keyState & Gamework::INPUT_CANCEL)
		{
			m_isDone = false;
			return true;
		}
		else if (keyState & Gamework::INPUT_SELECT)
		{
			if (!m_cancelButton)
			{
				m_realKeyMap.swap(m_configKeyMap);
				m_realJoyMap.swap(m_configJoyMap);
			}
			return false;
		}

		sf::Sprite* background = new sf::Sprite(spriteAnimaion->spriteTexture().texture(), spriteAnimaion->getTextureRect(0));
		background->setPosition(0.0, 240.0f-45.0f);
		background->scale(640.f, 90.f);
		Gamework::gRenderer->renderSprite(*background, Gamework::RenderLayerFrontEnd, WC::RENDER_FLAGS_Copy);
		delete background;

		float cursorX = m_cancelButton ? CancelCursorRight : CancelCursorLeft;
		if (!isCloseEnough(m_cancelBar.x, cursorX, Epsilon))
			evalCubic(m_cancelBar.x, m_cancelBar.y, cursorX, SwapAnimSpeed);

		sf::Sprite* cursor = new sf::Sprite(spriteAnimaion->spriteTexture().texture(), spriteAnimaion->getTextureRect(2));
		cursor->setPosition(640.0f * m_cancelBar.x, 240.0f - 0.5f * BarHeight);
		cursor->scale(105.0f, BarHeight);
		Gamework::gRenderer->renderSprite(*cursor, Gamework::RenderLayerFrontEnd, WC::RENDER_FLAGS_Copy);
		delete cursor;

		Gamework::gRenderer->Print("Apply", "r*slcpaxcy", 0.25f, 0.0f, 1.05f, Gamework::RenderLayerFrontEnd, &gColours[ColourDarkBlue]);
		Gamework::gRenderer->Print("Discard", "r*slcpaxcy", 0.75f, 0.0f, 1.05f, Gamework::RenderLayerFrontEnd, &gColours[ColourDarkBlue]);

		return true;
	}

	//-----------------------------------------------------------------------------
	bool ConfigInput::handleSetting(void)
	{
		while (m_isSetting)
		{
			sf::Event event;
			while (Gamework::gRenderer->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					Gamework::gRenderer->close();
					return false;
				}
				if (m_keySide && event.type == sf::Event::KeyPressed)
				{
					// See if the key is already bound
					sf::Uint32 used = getValueForKeyKeyInMap(event.key.code, m_configKeyMap);
					// Get the key that existed for the action being assigned
					sf::Uint32 old = getKeyForKeyValueInMap(actions[m_currSelection], m_configKeyMap);
					// Erase the old key assignment
					m_configKeyMap.erase(old);
					// Install the action being assigned onto the new key
					m_configKeyMap[event.key.code] = actions[m_currSelection];
					// if the old key was in use, assign the already used action to the old key
					// one key can only be bound to one action at a time
					if(-1 != used)
						m_configKeyMap[old] = used;

					m_isSetting = false;
					// Loop while the key is still down
					while (Gamework::gRenderer->pollEvent(event) || event.type != sf::Event::KeyReleased)
					{
						// But break if the window closes
						if (event.type == sf::Event::Closed)
						{
							Gamework::gRenderer->close();
							return false;
						}
					}
				}
				if (!m_keySide && event.type == sf::Event::JoystickButtonPressed)
				{
					WC::JoyKeyVal jkv(event.joystickButton.joystickId, 0, event.joystickButton.button, 0);
					sf::Uint32 used = getValueForJoyKeyInMap(jkv, m_configJoyMap);
					WC::JoyKeyVal old = getKeyForJoyValueInMap(actions[m_currSelection], m_configJoyMap);
					m_configJoyMap.erase(old);
					m_configJoyMap[jkv] = actions[m_currSelection];
					if (-1 != used)
						m_configJoyMap[old] = used;

					m_isSetting = false;
					while (Gamework::gRenderer->pollEvent(event) || event.type != sf::Event::JoystickButtonReleased)
					{
						if (event.type == sf::Event::Closed)
						{
							Gamework::gRenderer->close();
							return false;
						}
					}
				}
				if (!m_keySide && event.type == sf::Event::JoystickMoved)
				{

				}
			}
		}
		return true;
	}

	//-----------------------------------------------------------------------------
	void ConfigInput::handleKey(sf::Uint32 keyState)
	{
		if (keyState & Gamework::INPUT_RIGHT)
			m_keySide = false;

		if (keyState & Gamework::INPUT_LEFT)
			m_keySide = true;

		if (keyState & Gamework::INPUT_DOWN)
		{
			if (++m_currSelection >= numLabels)
				m_currSelection = 0;
		}

		if (keyState & Gamework::INPUT_UP)
		{
			if (--m_currSelection < 0)
				m_currSelection = numLabels - 1;
		}

		if (keyState & Gamework::INPUT_SELECT)
		{
			if (m_currSelection == numLabels - 1)
			{
				if (m_keySide)
					Gamework::gGame->setupDefaultKeyMappings(m_configKeyMap);
				else
					Gamework::gGame->setupDefaultJoyMappings(m_configJoyMap);
			}
			else
			{
				m_isSetting = true;
			}
		}

		if (keyState & Gamework::INPUT_CANCEL)
			m_isDone = true;
	}

	//-----------------------------------------------------------------------------
	bool ConfigInput::run(sf::Time elapsedTime, sf::Uint32 keyState)
	{
		if (!m_isDone)
		{
			if (keyState && !m_isSetting)
			{
				handleKey(keyState);
				// Only show apply / discard if a change was made
				if(m_isDone && m_configKeyMap == m_realKeyMap && m_configJoyMap == m_realJoyMap)
					return false;
			}
			else if (m_isSetting)
			{
				// Handles closing windown when in settings mode
				if (!handleSetting())
					return false;
			}
			keyState = 0;
		}

		drawScreen();

		if (m_isDone)
			return handleCancelScreen(keyState);

		return true;
	}
}
