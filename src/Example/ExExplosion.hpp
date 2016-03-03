#ifndef _EXEXPLOSION_HPP_
#define _EXEXPLOSION_HPP_

#include "EffectsMgr.hpp"

namespace Examples
{
	//-----------------------------------------------------------------------------
	// This particle is for the explosion particles
	//-----------------------------------------------------------------------------
	class ParticleSprite : public sf::Sprite
	{
	private:
		sf::Vector2f	m_speed;

	public:
		ParticleSprite(sf::Vector2f point, sf::Color startColour, sf::Vector2f travelSpeed, float scale);

		sf::Vector2f getSpeed(void) { return m_speed; }
	};


	//-----------------------------------------------------------------------------
	// The explosion is a bomb effect, a bit like a firework
	//-----------------------------------------------------------------------------
	class Explosion : public WC::Effect
	{
	private:
		std::vector<ParticleSprite*>	m_particles;
		sf::Time						m_animTimeAcc;
		sf::Time						m_fadeTimeAcc;
		sf::Color						m_decayColour;

	public:
		Explosion(sf::Vector2f point, sf::Time ttl, sf::Color startColour, sf::Color decayColour);

		virtual void fade(sf::Time elapsedTime);
		virtual void animateAndDraw(WC::Render& aRenderer, sf::Time elapsedTime);
	};
}

#endif //_EXEXPLOSION_HPP_
