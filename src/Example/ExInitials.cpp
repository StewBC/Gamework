#include "ExInitials.hpp"
#include "ExGlobals.hpp"
#include "Game.hpp"
#include "SpriteResourceManager.hpp"
#include "Render.hpp"
#include "Globals.hpp"
#include "InputManager.hpp"
#include <string.h>

namespace Examples
{
	//-----------------------------------------------------------------------------
	// Deals with showing high-scores and getting initials for a new high score
	//-----------------------------------------------------------------------------
	const char* InitialsFont = "consolas.png";
	const int FontFirstChar = 0;

	//-----------------------------------------------------------------------------
	void ReadInitials::setupInitials(void)
	{
		m_initialsFont = dynamic_cast<WC::FontTexture*>(Gamework::gRenderer->GetResMgr()->findSpriteTexture(InitialsFont));
		memset(&m_scoreEntry.m_initials[1], 0, Gamework::NumInitialsCharacters);
		m_initialsIndex = 0;
		m_currentInitial = FontFirstChar;
		m_scoreEntry.m_initials[0] = m_initialsFont->getCharacters()[m_currentInitial];
	}

	//-----------------------------------------------------------------------------
	#define InitialsEntryTimeInSeconds		8
	bool ReadInitials::run(sf::Time elapsedTime)
	{
		sf::Uint32 inputState = Gamework::gInputMgr->getManagedInputState(elapsedTime);
		const sf::String& fontCharacters = m_initialsFont->getCharacters();
		const int fontLastChar = (int)fontCharacters.getSize() - 1;

		// If there's input, reset the timeout clock
		if (m_prevInputState != inputState)
		{
			m_stateInitalTimeAcc = sf::Time::Zero;
			m_prevInputState = inputState;
		}
		else
		{
			m_stateInitalTimeAcc += elapsedTime;
		}

		bool timeOut = m_stateInitalTimeAcc >= sf::seconds(InitialsEntryTimeInSeconds);

		if (inputState & Gamework::INPUT_UP)
		{
			if (++m_currentInitial > fontLastChar)
				m_currentInitial = FontFirstChar;
			m_scoreEntry.m_initials[m_initialsIndex] = fontCharacters[m_currentInitial];
		}
		else if (inputState & Gamework::INPUT_DOWN)
		{
			if (--m_currentInitial < FontFirstChar)
				m_currentInitial = fontLastChar;
			m_scoreEntry.m_initials[m_initialsIndex] = fontCharacters[m_currentInitial];
		}
		else if (inputState & Gamework::INPUT_LEFT)
		{
			if (m_initialsIndex)
				--m_initialsIndex;
		}
		else if (inputState & Gamework::INPUT_RIGHT)
		{
			if (m_initialsIndex < Gamework::NumInitialsCharacters - 1)
			{
				++m_initialsIndex;
				if (!m_scoreEntry.m_initials[m_initialsIndex])
					m_scoreEntry.m_initials[m_initialsIndex] = m_scoreEntry.m_initials[m_initialsIndex - 1];
			}
		}
		else if (inputState & (Gamework::INPUT_ACTION | Gamework::INPUT_SELECT) || timeOut)
		{
			if ((inputState & Gamework::INPUT_SELECT) || m_initialsIndex + 1 >= Gamework::NumInitialsCharacters || timeOut)
			{
				for (int i = 1; i < Gamework::NumInitialsCharacters; ++i)
				{
					if (!m_scoreEntry.m_initials[i])
						m_scoreEntry.m_initials[i] = ' ';
				}
				m_scoreEntry.m_score = Gamework::gScorePerPlayer[Gamework::gActivePlayerNumber];
				gTodaysHighScores.insertInTable(m_scoreEntry);
				if (gAllTimeHighScores.insertInTable(m_scoreEntry) < Gamework::NumHighScoresInTable)
					gLoadedScoresHaveChanged = true;

				setupInitials();
				m_stateInitalTimeAcc = sf::Time::Zero;

				return false;
			}

			++m_initialsIndex;
			m_scoreEntry.m_initials[m_initialsIndex] = m_scoreEntry.m_initials[m_initialsIndex - 1];
		}

		// Render initials
		static WC::PrintAttrs initialAttribs[Gamework::NumInitialsCharacters];
		std::string initials = std::string(m_scoreEntry.m_initials) + std::string(Gamework::NumInitialsCharacters, '_');
		std::string score = std::string("Your Score : ") + std::to_string(Gamework::gScorePerPlayer[Gamework::gActivePlayerNumber]);
		initials = initials.substr(0, Gamework::NumInitialsCharacters);
		for (int i = 0; i < Gamework::NumInitialsCharacters; ++i)
		{
			initialAttribs[i].m_colour = m_initialsIndex == i ? gColours[ColourRed] : gColours[ColourYellow];
			initialAttribs[i].m_scale = 1.4f;
		}
		Gamework::gRenderer->Print("Enter your initials", "r*lcpax", 0.25f, 0.85f, Gamework::RenderLayerFrontEnd, &gColours[ColourCyan]);
		Gamework::gRenderer->Print(score, "r*lcpax", 0.25f, 0.90f, Gamework::RenderLayerFrontEnd, &gColours[ColourCyan]);
		Gamework::gRenderer->Print(initials, "r*lafpax", 0.50f, 0.85f, Gamework::RenderLayerFrontEnd, initialAttribs, m_initialsFont);

		return true;
	}
}
