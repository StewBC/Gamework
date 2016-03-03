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

#include "SpriteResourceManager.hpp"

#ifdef WIN32
	#define wc_sscanf	sscanf_s
#else
	#define wc_sscanf	sscanf
#endif


namespace WC
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	SpriteTexture* SpriteResourceManager::makeSpriteTextureFromDict(plist::XML_dict* aDict, sf::Texture& aTexture, std::string& aFileName)
	{
		SpriteTexture*	aSpriteTexture;
		sf::IntRect*	textureRects;
		int				numTextureRects;

		plist::XML_base* frames = aDict->valueForKey("frames");
		if (frames)
		{
			// Allocate a placehoder C style array of frame rects
			std::vector<plist::XML_base*>& framesVect = dynamic_cast<plist::XML_array*>(frames)->m_array;
			numTextureRects = (int)framesVect.size();
			textureRects = new sf::IntRect[numTextureRects];

			// extract the frame rects from the c strings in the array into the placeholder memory
			for (int frame = 0; frame < numTextureRects; ++frame)
			{
				std::string& frameString = dynamic_cast<plist::XML_string*>(framesVect[frame])->m_string;
				wc_sscanf(frameString.c_str(), "{{%d, %d}, {%d, %d}}", &textureRects[frame].left, &textureRects[frame].top, &textureRects[frame].width, &textureRects[frame].height);
			}
		}
		else
		{
			// No frames means the whole texture is a frame, so make that
			textureRects = new sf::IntRect[1];
			sf::Vector2u textureSize = aTexture.getSize();
			*textureRects = { 0, 0, (int)textureSize.x, (int)textureSize.y };
			numTextureRects = 1;
		}

		// Create the spriteTexture
		plist::XML_base* fontChars = aDict->valueForKey("fontcharacters");
		if (fontChars)
			aSpriteTexture = new FontTexture(dynamic_cast<plist::XML_string*>(fontChars)->m_string, aFileName, aTexture, textureRects, numTextureRects);
		else
			aSpriteTexture = new SpriteTexture(aFileName, aTexture, textureRects, numTextureRects);

		// clean up intermediate storage
		delete[] textureRects;

		return aSpriteTexture;
	}

	//-----------------------------------------------------------------------------
	void SpriteResourceManager::loadAnimFromDict(plist::XML_dict* aDict, SpriteTexture* aSpriteTexture)
	{
		// See if there are animations for this texture
		plist::XML_base* textureAnims = aDict->valueForKey("anims");
		if (textureAnims)
		{
			// Get the array of animations
			std::vector<plist::XML_base*>& animsVect = dynamic_cast<plist::XML_array*>(textureAnims)->m_array;

			// Each element of the array is a dictionary - iterate over them
			for (std::vector<plist::XML_base*>::iterator anim = animsVect.begin(); anim != animsVect.end(); ++anim)
			{
				// Get the dictionary
				plist::XML_dict* animDict = dynamic_cast<plist::XML_dict*>((*anim));

				// Find the name of the animation and the array with the frame indicies.  These indicies point into the
				// array of frames held in aSpriteTexture
				plist::XML_base* animName = animDict->valueForKey("animName");
				if (animName)
				{
					std::string& anim_name = dynamic_cast<plist::XML_string*>(animName)->m_string;

					plist::XML_base* animFrames = animDict->valueForKey("frameIndex");
					if (animFrames)
					{
						std::vector<plist::XML_base*>& frameIndiciesVect = dynamic_cast<plist::XML_array*>(animFrames)->m_array;
						int numFrameIndicies = (int)frameIndiciesVect.size();
						int* frameIndicies = new int[numFrameIndicies];

						for (int frame = 0; frame < numFrameIndicies; ++frame)
						{
							std::string& frameString = dynamic_cast<plist::XML_integer*>(frameIndiciesVect[frame])->m_string;
							wc_sscanf(frameString.c_str(), "%d", &frameIndicies[frame]);
						}

						// Create the sprite animation and clean up intermediate storage
						SpriteAnimaion* aSpriteAnimation = new SpriteAnimaion(*aSpriteTexture, anim_name.c_str(), frameIndicies, numFrameIndicies);
						delete[] frameIndicies;

						// For the mapped Texture, add an animation to the array
						m_resourceMap[aSpriteTexture].push_back(aSpriteAnimation);
					}
				}
			}
		}
	}

	//-----------------------------------------------------------------------------
	int SpriteResourceManager::loadFromXMLDict(plist::XML_dict* aDict)
	{
		plist::XML_base* bitmap = aDict->valueForKey("bitmap");
		if (bitmap)
		{
			sf::Texture* aTexture = new sf::Texture;

			std::string& fileName = dynamic_cast<plist::XML_string*>(bitmap)->m_string;
			// Load the bitmap file from disk
			if (aTexture->loadFromFile(fileName))
			{
				// Load the texture and its frame data
				SpriteTexture* aSpriteTexture = makeSpriteTextureFromDict(aDict, *aTexture, fileName);

				// Add to the resource list
				m_resourceMap[aSpriteTexture];

				//  Load any animations that may be present in this node
				loadAnimFromDict(aDict, aSpriteTexture);

				// Loaded one resource
				return 1;
			}
		}
		return 0;
	}

	//-----------------------------------------------------------------------------
	int SpriteResourceManager::loadFromXMLArray(plist::XML_array* aArray)
	{
		int loaded = 0;

		for (std::vector<plist::XML_base*>::iterator i = aArray->m_array.begin(); i != aArray->m_array.end(); ++i)
		{
			switch ((*i)->isA())
			{
			case plist::TYPE_dict:
				loaded += loadFromXMLDict(dynamic_cast<plist::XML_dict*>(*i));
				break;

			case plist::TYPE_array:
				loaded += loadFromXMLArray(dynamic_cast<plist::XML_array*>(*i));
				break;

			default:
				break;
			}
		}
		return loaded;
	}

	//-----------------------------------------------------------------------------
	SpriteTexture* SpriteResourceManager::findSpriteTexture(const sf::String& aTextureName)
	{
		for (std::map<SpriteTexture*, std::vector<SpriteAnimaion*> >::iterator i = m_resourceMap.begin(); i != m_resourceMap.end(); ++i)
		{
			if ((*i).first->textureName() == aTextureName)
				return (*i).first;
		}
		return nullptr;
	}

	//-----------------------------------------------------------------------------
	SpriteAnimaion* SpriteResourceManager::findSpriteAnimation(const sf::String& aTextureName, const sf::String& aAnimationName)
	{
		for (std::map<SpriteTexture*, std::vector<SpriteAnimaion*> >::iterator i = m_resourceMap.begin(); i != m_resourceMap.end(); ++i)
		{
			if ((*i).first->textureName() == aTextureName)
			{
				for (std::vector<SpriteAnimaion*>::iterator j = (*i).second.begin(); j != (*i).second.end(); ++j)
				{
					if ((*j)->animName() == aAnimationName)
						return (*j);
				}
				return nullptr;
			}
		}
		return nullptr;
	}

	//-----------------------------------------------------------------------------
	int SpriteResourceManager::loadFromPListFile(const sf::String& file)
	{
		plist::XML_parser aParser;
		std::string ansiStr(file);

		if (aParser.ParseFile(ansiStr.c_str()))
			return loadFromXMLArray(&aParser.m_root);

		return false;
	}
}
