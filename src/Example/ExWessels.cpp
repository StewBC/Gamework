#include "ExWessels.hpp"
#include "ExGlobals.hpp"
#include "Game.hpp"
#include "SpriteResourceManager.hpp"
#include "Render.hpp"
#include "Globals.hpp"

namespace Examples
{
	#define FadeFrequency	16 // ms

	//-----------------------------------------------------------------------------
	// This shows a sprite in different colours and fades it out at the end
	//-----------------------------------------------------------------------------
	Wessels::Wessels(sf::Time ttl) : Effect(ttl)
	{
		sf::Vector2f winSize = Gamework::gRenderer->getWinSize();
		WC::SpriteTexture *theTexture = Gamework::gRenderer->GetResMgr()->findSpriteTexture("wessels.png");
		setTimeToFade(ttl / 2.0f);
		setFadeAmount(5.0f);
		setTexture(theTexture->texture(), true);
		setScale(2.0f, 2.0f);
		winSize.x = (float)(rand() % (int)(winSize.x - getTexture()->getSize().x * getScale().x));
		winSize.y = (float)(rand() % (int)(winSize.y - getTexture()->getSize().y * getScale().y));
		setPosition(winSize.x, winSize.y);
		m_fadeTimeAcc = m_animTimeAcc = sf::Time::Zero;
	}

	//-----------------------------------------------------------------------------
	void Wessels::fade(sf::Time elapsedTime)
	{
		m_fadeTimeAcc += elapsedTime;
		if (m_fadeTimeAcc > sf::milliseconds(FadeFrequency))
		{
			m_fadeTimeAcc = sf::Time::Zero;

			sf::Color myColor = getColor();
			int fade = (int)getFadeAmount();
			if (myColor.a > fade)
				myColor.a -= fade;
			else
				myColor.a = 0;

			setColor(myColor);
		}
	}

	//-----------------------------------------------------------------------------
	void Wessels::animateAndDraw(WC::Render& aRenderer, sf::Time elapsedTime)
	{
		m_animTimeAcc += elapsedTime;
		if (m_animTimeAcc > sf::milliseconds(66))
		{
			sf::Color myColor = getColor();
			sf::Color newColor = gColours[ColourYellow + (rand() % (ColourLightskyBlue - ColourYellow))];
			newColor.a = myColor.a;
			setColor(newColor);
			m_animTimeAcc = sf::Time::Zero;
		}

		aRenderer.renderSprite(*this, Gamework::RenderLayerBackground);
	}
}
