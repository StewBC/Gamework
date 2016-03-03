#include "ExHighScreen.hpp"
#include "ExGlobals.hpp"
#include "Game.hpp"
#include "Render.hpp"
#include "Globals.hpp"
#include <sstream>
#include <iomanip>

namespace Examples
{
	//-----------------------------------------------------------------------------
	HighScoreScreen::~HighScoreScreen()
	{
		if (m_scoreRenderCached)
		{
			m_scoreRenderCached = false;
			delete m_cacheRenderer;
			m_cacheRenderer = nullptr;
		}
	}

	//-----------------------------------------------------------------------------
	void HighScoreScreen::run(sf::Time elapsedTime)
	{
		static int colourCounter = 0;
		static sf::Time colorCycleTime = sf::Time::Zero;

		Gamework::gRenderer->Print("HIGH SCORES", "r*slcpax", 0.5f, 0.15f, 2.0f, Gamework::RenderLayerFrontEnd, &gColours[ColourRed]);
		Gamework::gRenderer->Print("Today's Best", "r*slcpax", 0.25f, 0.3f, 1.3f, Gamework::RenderLayerFrontEnd, &gColours[ColourDarkMagenta]);
		Gamework::gRenderer->Print("All Time Best", "r*slcpax", 0.75f, 0.3f, 1.3f, Gamework::RenderLayerFrontEnd, &gColours[ColourYellow]);

		// Cache the scores into a seperate renderer
		if (!m_scoreRenderCached)
		{
			m_cacheRenderer = new WC::Render(*Gamework::gRenderer);

			std::ostringstream stringStream;

			for (int i = 0; i < Gamework::NumHighScoresInTable; ++i)
			{
				stringStream.str("");
				stringStream << std::setfill('0') << std::setw(6) << gTodaysHighScores.score(i) << " " << gTodaysHighScores.initials(i);
				std::string copyOfStr = stringStream.str();
				m_cacheRenderer->Print(copyOfStr, "r*lpax", 0.25f, 0.40f + i * 0.04f, Gamework::RenderLayerFrontEnd);

				stringStream.str("");
				stringStream << std::setfill('0') << std::setw(6) << gAllTimeHighScores.score(i) << " " << gAllTimeHighScores.initials(i);
				copyOfStr = stringStream.str();
				m_cacheRenderer->Print(copyOfStr, "r*lpax", 0.75f, 0.40f + i * 0.04f, Gamework::RenderLayerFrontEnd);
			}
			m_scoreRenderCached = true;
		}

		colorCycleTime += elapsedTime;
		if (colorCycleTime > sf::milliseconds(75))
		{
			colorCycleTime = sf::Time::Zero;
			if (++colourCounter > ColourLinen)
				colourCounter = 0;
		}
		int colourIndex[2] = { colourCounter, colourCounter + NumColours / 2 };

		// Clone the sprite containers from the seperate renderer into the main renderer and adjust the colours as you go
		// This runs faster by a factor of about 1.65 in my tests than just re-rendering to the main renderer
		int layer = 0;
		int flop = 0;
		WC::RenderQueues& cacheRenderQueue = m_cacheRenderer->getRenderQueues();
		for (WC::RenderQueues::iterator i = cacheRenderQueue.begin(); i != cacheRenderQueue.end(); ++i)
		{
			for (WC::SpriteContainerArray::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
			{
				sf::Sprite *sprite = (*j)->getSprite();
				int flags = (*j)->getFlags();

				// Clear the copy flag since the sprites are not owned by the main renderer but by the cache renderer
				// Clearing the copy flag prevents the main renderer deleting the sprites
				Gamework::gRenderer->renderSprite(*sprite, layer, flags & ~WC::RENDER_FLAGS_Copy);

				// The pop indicates the end of a score line so cycle the colours
				if (flags & WC::RENDER_FLAGS_Pop)
				{
					flop = 1 - flop;
					if (++colourIndex[flop] >= ColourLinen)
						colourIndex[flop] -= ColourLinen;
				}
				sprite->setColor(gColours[colourIndex[flop]]);
			}
			++layer;
		}
	}
}
