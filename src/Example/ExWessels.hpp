#ifndef _EXWESSELS_HPP_
#define _EXWESSELS_HPP_

#include "EffectsMgr.hpp"

namespace Examples
{
	//-----------------------------------------------------------------------------
	// This shows a sprite in different colours and fades it out at the end
	//-----------------------------------------------------------------------------
	class Wessels : public WC::Effect
	{
	private:
		sf::Time m_animTimeAcc;
		sf::Time m_fadeTimeAcc;
	public:
		Wessels(sf::Time elapsedTime);
		virtual void fade(sf::Time elapsedTime);
		virtual void animateAndDraw(WC::Render& aRenderer, sf::Time elapsedTime);
	};
}

#endif //_EXWESSELS_HPP_
