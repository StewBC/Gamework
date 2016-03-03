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

#include "AnimatedSprite.hpp"

namespace WC
{
	//-----------------------------------------------------------------------------
	// AnimatedSprite implementation
	//-----------------------------------------------------------------------------
	void AnimatedSprite::setAnimation(SpriteAnimaion& aSpriteAnimation, sf::Time aUpdateTime, int aAnimFlags, bool aPaused)
	{
		m_spriteAnimation = &aSpriteAnimation;
		setTexture(m_spriteAnimation->texture(), false);
		setTextureRect(m_spriteAnimation->getTextureRect(0));
	}

	//-----------------------------------------------------------------------------
	bool AnimatedSprite::frameUpdate(sf::Time aElapsedTime)
	{
		if (!m_paused)
		{
			// Accumulate time
			m_timeAcc += aElapsedTime;
			// is the accumulated time is more than the time when this anim triggers
			if (m_timeAcc > m_updateTime)
			{
				// See how many frames to advance the animation
				int frames = m_timeAcc.asMilliseconds() / m_updateTime.asMilliseconds();
				sf::Time frameTime = sf::milliseconds(frames * m_updateTime.asMilliseconds());
				// the accumulated time is the time left since the last frame would have become active
				m_timeAcc -= frameTime;

				// Don't need to worry about large frame shifts in "once" animations
				if (!(m_animFlags & ANIM_Once))
				{
					// if enough time elapsedTime that the frames go out the window, and maybe even roll over
					if (m_animFlags & ANIM_PingPong)
					{
						// Ping Pong anims are actually 1 less than the number of frames
						// If there are 3 frames, each colunm shows only 2 frames between
						// pings and pongs
						// 0   4   8
						//   1 3 5 7 9
						//   2   6   0
						frames %= (m_numFrames - 1);
					}
					else
					{
						frames %= m_numFrames;
					}
				}

				if (m_animFlags & ANIM_Reverse)
				{
					m_curFrame -= frames;
					if (m_curFrame < 0)
					{
						if (m_animFlags & ANIM_Loop)
						{
							m_curFrame += m_numFrames;
						}
						else if (m_animFlags & ANIM_PingPong)
						{
							m_curFrame = -m_curFrame;
							m_animFlags &= ~ANIM_Reverse;
						}
						else // ANIM_Once
						{
							m_curFrame = 0;
						}
					}
				}
				else
				{
					m_curFrame += frames;
					if (m_curFrame >= m_numFrames)
					{
						if (m_animFlags & ANIM_Loop)
						{
							m_curFrame -= m_numFrames;
						}
						else if (m_animFlags & ANIM_PingPong)
						{
							m_curFrame = m_numFrames - 1 - ((m_curFrame + 1) - m_numFrames);
							m_animFlags |= ANIM_Reverse;
						}
						else // ANIM_Once
						{
							m_curFrame = m_numFrames - 1;
						}
					}
				}

				setTextureRect(m_spriteAnimation->getTextureRect(m_curFrame));
				// return true if the sprite updated (interval fired)
				return true;
			}
		}
		return false;
	}
}
