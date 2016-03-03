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

#include "SpriteTexture.hpp"
#include <string.h>

namespace WC
{
	//-----------------------------------------------------------------------------
	// SpriteTexture implementation
	//-----------------------------------------------------------------------------
	SpriteTexture::SpriteTexture(const sf::String& aTextureName, const sf::Texture& aTexture, const sf::IntRect* aTextureRects, int numRects) :
		m_texture(&aTexture)
	{
		int newSize = numRects + (int)m_textureRects.size();

		setTextureName(aTextureName);

		m_textureRects.reserve(newSize);
		for (int i = 0; i < numRects; ++i)
			m_textureRects.push_back(aTextureRects[i]);
	}

	//-----------------------------------------------------------------------------
	SpriteTexture::~SpriteTexture()
	{
		// free m_textureName
		delete m_texture;
		m_textureRects.clear();
	}

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	void FontTexture::buildLookupCache(void)
	{
		const sf::Uint32* characters = m_characters.getData();
		size_t length = m_characters.getSize();

		if (m_lookup)
			delete[] m_lookup;

		if (length)
		{
			m_lookup = new char[256];
			memset(m_lookup, 0, 256);
		}
		else
		{
			m_lookup = nullptr;
		}

		for (sf::Uint32 charPos = 0; charPos < length; ++charPos)
		{
			char thisChar = (char)characters[charPos];
			m_lookup[thisChar] = charPos;
		}
	}

}
