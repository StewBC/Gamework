#ifndef _EXINITIALS_HPP_
#define _EXINITIALS_HPP_

#include "HighScoreTable.hpp"
#include <SFML/Graphics.hpp>

//-----------------------------------------------------------------------------
namespace WC
{
	class FontTexture;
}

//-----------------------------------------------------------------------------
namespace Examples
{
	//-----------------------------------------------------------------------------
	// Everything needed get initials on a new score
	//-----------------------------------------------------------------------------
	class ReadInitials
	{
	private:
		Gamework::HighScoreEntry   m_scoreEntry;
		int			     		   m_initialsIndex;
		int				 		   m_currentInitial;
		sf::Uint32       		   m_prevInputState;
		sf::Time	     		   m_stateInitalTimeAcc;
		WC::FontTexture* 		   m_initialsFont;

		void setupInitials(void);

	public:
		ReadInitials() :
			m_initialsIndex(0),
			m_currentInitial(0),
			m_prevInputState(0),
			m_stateInitalTimeAcc(sf::Time::Zero),
			m_initialsFont(nullptr)
			{
				setupInitials();
			}
		~ReadInitials() { ; }

		bool run(sf::Time elapsedTime);
	};
}

#endif //_EXINITIALS_HPP_
