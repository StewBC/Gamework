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

#ifndef _SPRITERESOURCEMANAGER_HPP_
#define _SPRITERESOURCEMANAGER_HPP_

#include "SpriteAnimation.hpp"
#include "PlistHelper.hpp"
#include <SFML/Graphics.hpp>

namespace WC
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class SpriteResourceManager
	{
	private:
		std::map<SpriteTexture*, std::vector<SpriteAnimaion*> > m_resourceMap;
		
		void loadAnimFromDict(plist::XML_dict* aDict, SpriteTexture* aSpriteTexture);
		SpriteTexture* makeSpriteTextureFromDict(plist::XML_dict* aDict, sf::Texture& aTexture, std::string& aFileName);
		int loadFromXMLDict(plist::XML_dict* aDict);
		int loadFromXMLArray(plist::XML_array* aArray);

	public:
		SpriteResourceManager()													{ ; }
		SpriteResourceManager(const sf::String& aResource) : m_resourceMap()	{ loadFromPListFile(aResource.getData()); }

		SpriteTexture* findSpriteTexture(const sf::String& aTextureName);
		SpriteAnimaion* findSpriteAnimation(const sf::String& aTextureName, const sf::String& aAnimationName);

		int loadFromPListFile(const sf::String& file);
	};
}

#endif //_SPRITERESOURCEMANAGER_HPP_
