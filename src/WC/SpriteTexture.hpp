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

#ifndef _SPRITETEXTURE_HPP_
#define _SPRITETEXTURE_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

namespace WC
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class SpriteTexture
	{
	private:
		sf::String					m_textureName;
		const sf::Texture* 			m_texture;
		std::vector<sf::IntRect>	m_textureRects;

	public:
		SpriteTexture(const sf::String& aTextureName, const sf::Texture& aTexture, const sf::IntRect* aTextureRects, int numRects = 1);
		virtual ~SpriteTexture();

		void    setTextureName(const sf::String& aTextureName)	{ m_textureName = aTextureName; }
		sf::String& textureName(void)						    { return m_textureName; }

		void    setTexture(const sf::Texture& aTexture)		    { if (m_texture) delete m_texture; m_texture = &aTexture; }
		const   sf::Texture& texture(void)					    { return *m_texture; }

		int     addRect(sf::IntRect& aRect)					    { m_textureRects.push_back(aRect); return (int)m_textureRects.size(); }
		const   sf::IntRect& getTextureRect(int aRectIdx)		{ return m_textureRects[aRectIdx]; }
		const   int numRects(void)								{ return (int)m_textureRects.size(); }
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class FontTexture : public SpriteTexture
	{
	private:
		sf::String	m_characters;
		char*		m_lookup;
	public:
		FontTexture(const sf::String& aCharacters, const sf::String& aTextureName, const sf::Texture& aTexture, const sf::IntRect* aTextureRects, int numRects = 1) :
			SpriteTexture(aTextureName, aTexture, aTextureRects, numRects),
			m_characters(aCharacters),
			m_lookup(nullptr)
		{
			buildLookupCache();
		}

		int     getCharIndex(char aChar)				{ return m_lookup[aChar]; }
		void    buildLookupCache(void);
		void    setCharacters(sf::String& aCharacters)	{ m_characters = aCharacters; }
		sf::String& getCharacters(void)					{ return m_characters; }
	};
}
#endif //_SPRITETEXTURE_HPP_
