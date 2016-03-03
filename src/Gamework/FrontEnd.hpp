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

#ifndef _FRONTEND_HPP_
#define _FRONTEND_HPP_

#include "HighScoreTable.hpp"
#include "ExConfigInput.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>

//-----------------------------------------------------------------------------
namespace Examples
{
	class HighScoreScreen;
}

//-----------------------------------------------------------------------------
namespace Gamework
{
	//-----------------------------------------------------------------------------
	enum
	{
		FEStateInit,
		FEStateTitleScreen,
		FEStateShowScores,
		FEStateDemoAttract,
		FEStateConfigInputs,
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class FrontEnd
	{
	private:
		int							m_feState;
		int							m_feSavedState;
		Examples::ConfigInput*		m_exampleConfigInput;
		Examples::HighScoreScreen*	m_exampleHighScoreScreen;
		sf::Time					m_stateTimeAcc;

	public:
		FrontEnd() : 
			m_feState(FEStateInit), 
			m_feSavedState(FEStateInit),
			m_exampleConfigInput(nullptr),
			m_exampleHighScoreScreen(nullptr),
			m_stateTimeAcc(sf::Time::Zero)
		{ 
		}
		~FrontEnd() { ; }

		void preProcess(void);
		bool run(sf::Time elapsedTime);
	};
}

#endif //_FRONTEND_HPP_
