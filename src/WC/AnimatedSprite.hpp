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

#ifndef _ANIMATEDSPRITE_HPP_
#define _ANIMATEDSPRITE_HPP_

#include "SpriteAnimation.hpp"

namespace WC
{
	//-----------------------------------------------------------------------------
	enum
	{
		ANIM_Once		= 0,
		ANIM_Reverse	= 1,
		ANIM_Loop		= 2,
		ANIM_PingPong	= 4,
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class AnimatedSprite : public sf::Sprite
	{
	private:
		SpriteAnimaion*	m_spriteAnimation;
		sf::Time		m_updateTime;
		sf::Time		m_timeAcc;
		int				m_curFrame;
		int				m_numFrames;
		int				m_animFlags;
		bool			m_paused;

	public:
		AnimatedSprite(SpriteAnimaion& aSpriteAnimation, sf::Time aUpdateTime, int aAnimFlags = ANIM_Loop, bool aPaused = false) :
			m_spriteAnimation(&aSpriteAnimation), m_updateTime(aUpdateTime), m_timeAcc(), m_curFrame(0), m_numFrames(aSpriteAnimation.numFrames()), m_animFlags(aAnimFlags), m_paused(aPaused), Sprite(aSpriteAnimation.texture(), aSpriteAnimation.getTextureRect(0))
		{
			sf::IntRect aRect = aSpriteAnimation.getTextureRect(0);
			setOrigin(aRect.width / 2.0f, aRect.height / 2.0f);
		}
		~AnimatedSprite() { ; }

		void setAnimation(SpriteAnimaion& aSpriteAnimation, sf::Time aUpdateTime, int aAnimFlags = ANIM_Loop, bool aPaused = false);
		void pause(bool bShouldPause) { m_paused = bShouldPause; }
		bool isPaused(void) { return m_paused; }

		bool frameUpdate(sf::Time aElapsedTime);
	};
}

#endif //_ANIMATEDSPRITE_HPP_
