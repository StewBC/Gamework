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

#ifndef _EFFECTSMGR_HPP_
#define _EFFECTSMGR_HPP_

#include "Render.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace WC
{
	//-----------------------------------------------------------------------------
	class Render;

	//-----------------------------------------------------------------------------
	// Abstract base class that all effects inherit from
	//-----------------------------------------------------------------------------
	class Effect : public sf::Sprite
	{
		friend class EffectsManager;

	private:
		sf::Time		m_timeToLive;
		sf::Time		m_timeToFade;
		float			m_fadeAmount;

		bool run(WC::Render& aRenderer, sf::Time elapsedTime);

	public:
		Effect(sf::Time aTimeToLive) : m_timeToLive(aTimeToLive), m_timeToFade(aTimeToLive), m_fadeAmount(0.0f) { ; }
		virtual ~Effect() { ; }

		sf::Time	getTimeToLive(void) { return m_timeToLive; }
		void		setTimeToLive(sf::Time aTime) { m_timeToLive = aTime; }

		sf::Time	getTimeToFade(void) { return m_timeToFade; }
		void		setTimeToFade(sf::Time aTime) { m_timeToFade = aTime; }

		float		getFadeAmount(void) { return m_fadeAmount; }
		void		setFadeAmount(float aFadeAmount) { m_fadeAmount = aFadeAmount; }

		virtual void fade(sf::Time elapsedTime) = 0;
		virtual void animateAndDraw(WC::Render& aRenderer, sf::Time elapsedTime) = 0;
	};

	//-----------------------------------------------------------------------------
	// A vector of effects currently running
	// The effects manager must own the Effects
	//-----------------------------------------------------------------------------
	class EffectsManager
	{
	private:
		std::vector <Effect*>		m_effects;
		std::vector <Effect*>		m_deadEffects;
		WC::Render&					m_renderer;

	public:
		EffectsManager(WC::Render& aRenderer) : m_renderer(aRenderer) { ; }

		void removeAllEffects(void);
		void addEffect(Effect& aEffect)	{ m_effects.push_back(&aEffect); }
		size_t effectsRunning(void)		{ return m_effects.size(); }
		void run(sf::Time elapsedTime);

	};
}
#endif //_EFFECTSMGR_HPP_
