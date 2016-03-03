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

#include "SpriteAnimation.hpp"

namespace WC
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	SpriteAnimaion::SpriteAnimaion(SpriteTexture& aSpriteTexture, const sf::String& aAnimName, const int* aFrameInicies, int numIndicies)
	{
		int newSize = numIndicies + (int)m_frameIndicies.size();

		setSpriteTexture(aSpriteTexture);
		setAnimName(aAnimName);

		m_frameIndicies.reserve(newSize);
		for (int i = 0; i < numIndicies; ++i)
			m_frameIndicies.push_back(aFrameInicies[i]);
	}

	//-----------------------------------------------------------------------------
	SpriteAnimaion::~SpriteAnimaion()
	{
		// m_animName
		delete m_spriteTexture;
		m_frameIndicies.clear();
	}

}
