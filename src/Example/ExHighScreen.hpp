#ifndef _EXHIGHSCREEN_HPP_
#define _EXHIGHSCREEN_HPP_

#include "HighScoreTable.hpp"
#include <SFML/Graphics.hpp>

namespace WC
{
	class Render;
}

namespace Examples
{
	//-----------------------------------------------------------------------------
	// Show the high scores to the user
	//-----------------------------------------------------------------------------
	class HighScoreScreen
	{
	private:
		bool			 		   m_scoreRenderCached;
		WC::Render*		 		   m_cacheRenderer;

	public:
		HighScoreScreen() :
			m_scoreRenderCached(false),
			m_cacheRenderer(nullptr)
		{ ; }
		~HighScoreScreen();

		void run(sf::Time elapsedTime);
	};
}

#endif //_EXHIGHSCREEN_HPP_
