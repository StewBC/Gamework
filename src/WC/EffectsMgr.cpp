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

#include "EffectsMgr.hpp"

namespace WC
{
	//-----------------------------------------------------------------------------
	class Render;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	bool Effect::run(WC::Render& aRenderer, sf::Time elapsedTime)
	{
		if (m_timeToLive <= elapsedTime)
		{
			delete this;
			return false;
		}
		m_timeToLive -= elapsedTime;

		if (m_timeToFade > elapsedTime)
			m_timeToFade -= elapsedTime;
		else
			fade(elapsedTime);

		animateAndDraw(aRenderer, elapsedTime);

		return true;
	}

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	void EffectsManager::removeAllEffects(void)
	{
		
		while (m_effects.size())
 		{
			Effect* aEffect = m_effects.back();
			m_effects.pop_back();
			delete aEffect;
		}
	}

	//-----------------------------------------------------------------------------
	void EffectsManager::run(sf::Time elapsedTime)
	{
		m_effects.erase(std::remove_if(m_effects.begin(), m_effects.end(), [&](Effect* aEffect){ return !aEffect->run(m_renderer, elapsedTime); }), m_effects.end());
 	}
}
