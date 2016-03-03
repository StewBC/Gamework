#include "ExExplosion.hpp"
#include "ExGlobals.hpp"
#include "Game.hpp"
#include "SpriteResourceManager.hpp"
#include "Render.hpp"
#include "Globals.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Examples
{
	//-----------------------------------------------------------------------------
	// This particle is for the explosion particles
	//-----------------------------------------------------------------------------
	ParticleSprite::ParticleSprite(sf::Vector2f point, sf::Color startColour, sf::Vector2f travelSpeed, float scale) :
		m_speed(travelSpeed)
	{
		setPosition(point);
		setColor(startColour);
		setScale(scale, scale);
	}

	//-----------------------------------------------------------------------------
	// The explosion is a bomb effect, a bit like a firework
	//-----------------------------------------------------------------------------
	#define ExplosionFPSLimiter				16 // 60fps == 16ms

	Explosion::Explosion(sf::Vector2f point, sf::Time ttl, sf::Color startColour, sf::Color decayColour) :
		Effect(ttl), m_decayColour(decayColour)
	{
#define RADS(x)						((float)(x)*(float)(M_PI/180.0))

		float angle = 15.0f;
		float baseSpeed = 2.5f;
		sf::Uint32 slices = 4;
		float pScale = 8.0f;
		sf::Uint32 pInSlice = (sf::Uint32)(360.0f / angle);
		WC::SpriteAnimaion* whiteAnim = Gamework::gRenderer->GetResMgr()->findSpriteAnimation("sprites.png", "colors");

		setTimeToFade(0.5f * ttl);
		setFadeAmount(256.0f / (getTimeToFade().asMilliseconds() / ExplosionFPSLimiter));

		m_fadeTimeAcc = m_animTimeAcc = sf::Time::Zero;

		m_particles.reserve(pInSlice * slices);

		float theta = RADS(0);
		for (float i = 0; i < pInSlice; ++i)
		{
			for (sf::Uint32 s = 0; s < slices; ++s)
			{
				float x = cos(theta);
				float y = sin(theta);

				float sliceSpeed = (rand() % 64) / 16.0f;

				sf::Vector2f speed = sf::Vector2f(x*baseSpeed*(s + 1) + sliceSpeed, y*baseSpeed*(s + 1) + sliceSpeed);

				ParticleSprite *expl = new ParticleSprite(point, startColour, speed, pScale);
				expl->setTexture(whiteAnim->texture());
				expl->setTextureRect(whiteAnim->getTextureRect(6)); // 6 is the white colour in the colors "animation"
				m_particles.push_back(expl);
			}
			theta += RADS(angle);
		}
	}

	//-----------------------------------------------------------------------------
	void Explosion::fade(sf::Time elapsedTime)
	{
		m_fadeTimeAcc += elapsedTime;
		if (m_fadeTimeAcc > sf::milliseconds(ExplosionFPSLimiter))
		{
			m_fadeTimeAcc = sf::Time::Zero;
			m_decayColour.a -= (int)getFadeAmount();
			for (std::vector<ParticleSprite*>::iterator i = m_particles.begin(); i != m_particles.end(); ++i)
			{
				(*i)->setColor(m_decayColour);
			}
		}
	}

	//-----------------------------------------------------------------------------
	void Explosion::animateAndDraw(WC::Render& aRenderer, sf::Time elapsedTime)
	{
		bool update = false;
		m_animTimeAcc += elapsedTime;
		if (m_animTimeAcc > sf::milliseconds(ExplosionFPSLimiter))
		{
			m_animTimeAcc = sf::Time::Zero;
			update = true;
		}

		for (std::vector<ParticleSprite*>::iterator i = m_particles.begin(); i != m_particles.end(); ++i)
		{
			if (update)
			{
				sf::Vector2f point = (*i)->getPosition();
				sf::Vector2f speed = (*i)->getSpeed();
				point += speed;
				(*i)->setPosition(point);
			}
			aRenderer.renderSprite(*(*i), Gamework::RenderLayerBackground);
		}
	}
}
