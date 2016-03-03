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

#ifndef _SPRITEANIMATION_HPP_
#define _SPRITEANIMATION_HPP_

#include "SpriteTexture.hpp"
#include <SFML/Graphics.hpp>

namespace WC
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class SpriteAnimaion
	{
		sf::String						m_animName;
		SpriteTexture*					m_spriteTexture;
		std::vector<int>				m_frameIndicies;
	public:
		SpriteAnimaion(SpriteTexture& aSpriteTexture, const sf::String& aAnimName, const int* aFrameInicies, int numIndicies = 1);
		~SpriteAnimaion();

		void setAnimName(const sf::String& aAnimName)			{ m_animName = aAnimName; }
		sf::String& animName(void)								{ return m_animName; }

		void setSpriteTexture(SpriteTexture& aSpriteTexture)	{ m_spriteTexture = &aSpriteTexture; }
		SpriteTexture& spriteTexture(void)						{ return *m_spriteTexture; }

		// Get the frames of the animation and animation length
		int addFrame(int aFrame)								{ m_frameIndicies.push_back(aFrame); return (int)m_frameIndicies.size(); }
		const int frame(int aFrame)								{ return m_frameIndicies[aFrame]; }
		const int numFrames(void)								{ return (int)m_frameIndicies.size(); }

		// Access the m_spriteTexture members right from here as a shortcut
		int  addRect(sf::IntRect aRect)							{ return addFrame(m_spriteTexture->addRect(aRect)); }
		const sf::IntRect& getTextureRect(int aFrame)			{ return m_spriteTexture->getTextureRect(m_frameIndicies[aFrame]); }
		const sf::Texture& texture(void)						{ return m_spriteTexture->texture(); }
	};
}
#endif //_SPRITEANIMATION_HPP_
