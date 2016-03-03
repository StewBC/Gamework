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

#include "FrontEnd.hpp"
#include "ExExplosion.hpp"
#include "ExHighScreen.hpp"
#include "ExInitials.hpp"
#include "ExWessels.hpp"
#include "ExGlobals.hpp"
#include "Game.hpp"
#include "Render.hpp"

namespace Gamework
{
	//-----------------------------------------------------------------------------
	#define TitleScreenTimeInSeconds		6
	#define HighScoreScreenTimeInSeconds	6
	#define DemoAttractTimeInSeconds		6

	//-----------------------------------------------------------------------------
	void FrontEnd::preProcess(void)
	{
		m_stateTimeAcc = sf::Time::Zero;
		if (FEStateInit != m_feState)
			m_feState = FEStateShowScores;
		else
			m_feState = FEStateTitleScreen;

		m_exampleHighScoreScreen = new Examples::HighScoreScreen;
	}

	//-----------------------------------------------------------------------------
	bool FrontEnd::run(sf::Time elapsedTime)
	{
		sf::Uint32 keyState = gInputMgr->getManagedInputState(elapsedTime);
		m_stateTimeAcc += elapsedTime;

		if (((keyState & INPUT_1PLAYER) || (keyState & INPUT_2PLAYER)) && m_feState != FEStateConfigInputs)
		{
			gEffectsMgr->removeAllEffects();

			delete m_exampleHighScoreScreen;
			m_exampleHighScoreScreen = nullptr;

			// Assumes INPUT_1PLAYER is SET_BIT(5)
			gNumPlayersThisGame = (keyState & (INPUT_1PLAYER | INPUT_2PLAYER)) >> 5;
			return false;
		}
		else if ((keyState & INPUT_ACTION) && (m_feState != FEStateConfigInputs))
		{
			gEffectsMgr->removeAllEffects();
			m_exampleConfigInput = new Examples::ConfigInput(gGame->getInputKeyMapFE(), gGame->getInputJoyMapFE());
			m_feSavedState = m_feState;
			m_feState = FEStateConfigInputs;
		}

		switch (m_feState)
		{
			case FEStateTitleScreen:
				if (!gEffectsMgr->effectsRunning())
					gEffectsMgr->addEffect(*(new Examples::Wessels(sf::seconds(TitleScreenTimeInSeconds/3.5f))));

				if (m_stateTimeAcc >= sf::seconds(TitleScreenTimeInSeconds))
				{
					gEffectsMgr->removeAllEffects();
					m_feState = FEStateShowScores;
					m_stateTimeAcc = sf::Time::Zero;
				}
				gRenderer->Print("Title screen goes here", "slpc*", 2.0f, RenderLayerFrontEnd);
				break;

			case FEStateShowScores:
				if (m_stateTimeAcc >= sf::seconds(HighScoreScreenTimeInSeconds))
				{
					m_feState = FEStateDemoAttract;
					m_stateTimeAcc = sf::Time::Zero;
				}
				m_exampleHighScoreScreen->run(elapsedTime);
				break;

			case FEStateDemoAttract:
				if (!gEffectsMgr->effectsRunning())
				{
					sf::Vector2f size = gRenderer->getWinSize();
					sf::Vector2f point((float)(rand() % (int)size.x), (float)(rand() % (int)size.y));
					gEffectsMgr->addEffect(*(new Examples::Explosion(point, sf::seconds(1), Examples::gColours[rand() & Examples::NumColours], Examples::gColours[rand() & Examples::NumColours])));
					point = sf::Vector2f((float)(rand() % (int)size.x), (float)(rand() % (int)size.y));
					gEffectsMgr->addEffect(*(new Examples::Explosion(point, sf::seconds(1), Examples::gColours[rand() & Examples::NumColours], Examples::gColours[rand() & Examples::NumColours])));
				}

				if (m_stateTimeAcc >= sf::seconds(DemoAttractTimeInSeconds))
				{
					m_feState = FEStateTitleScreen;
					m_stateTimeAcc = sf::Time::Zero;
				}
				gRenderer->Print("Demo attract goes here", "slpc*", 2.0f, RenderLayerFrontEnd);
				break;

			case FEStateConfigInputs:
			{
				bool retVal = m_exampleConfigInput->run(elapsedTime, keyState);
				if (!retVal)
				{
					m_feState = m_feSavedState;
					delete m_exampleConfigInput;
					m_exampleConfigInput = nullptr;
				}
			}
		}
		if (m_feState != FEStateConfigInputs)
		{
			std::string helpText = "Press " + std::string(Examples::gKeyNames[Examples::getKeyForKeyValueInMap(INPUT_ACTION, gGame->getInputKeyMapFE())]) + " to configure inputs";
			gRenderer->Print(helpText, "lcpcxb", RenderLayerFrontEnd, &Examples::gColours[Examples::ColourViolet]);
		}

		return true;
	}
}
