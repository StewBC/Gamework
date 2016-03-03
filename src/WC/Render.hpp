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

#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include "SpriteTexture.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace WC
{
	#define SET_BIT(x)			(1<<(x))
	//-----------------------------------------------------------------------------
	enum
	{
		//renderSpriteflags
		RENDER_FLAGS_None		= SET_BIT(0),
		RENDER_FLAGS_Copy		= SET_BIT(1),
		RENDER_FLAGS_Rotate		= SET_BIT(2),
		RENDER_FLAGS_Scale		= SET_BIT(3),
		RENDER_FLAGS_Translate	= SET_BIT(4),
		RENDER_FLAGS_ClipView	= SET_BIT(5),
		RENDER_FLAGS_Pop		= SET_BIT(6),
	};						 
								 
	enum					 
	{						 
		// Text Positioning (Layout)
		PRINT_Centre_X			= SET_BIT(0),
		PRINT_Centre_Y			= SET_BIT(1),
		PRINT_Centre_Around_X	= SET_BIT(2),
		PRINT_Centre_Around_Y	= SET_BIT(3),
		PRINT_Right				= SET_BIT(4),
		PRINT_Top				= SET_BIT(5),
		PRINT_Bottom			= SET_BIT(6),

		// Text Wrap
		PRINT_Wrap_Text			= SET_BIT(7),
								 
		// X|Y is a ratio of the render rect width|height
		PRINT_Ratio_X			= SET_BIT(8),
		PRINT_Ratio_Y			= SET_BIT(9),
								 
		// Text Anchor Position
		PRINT_Anchor_Top		= SET_BIT(10),
		PRINT_Anchor_Mid		= SET_BIT(11),
		PRINT_Anchor_Bottom		= SET_BIT(12),

		// Restrict viewing to the drawing rectangle (Clip)
		PRINT_Set_Viewport		= SET_BIT(13),
	};

	const sf::Uint32 RENDER_Command = RENDER_FLAGS_Rotate | RENDER_FLAGS_Scale | RENDER_FLAGS_Translate | RENDER_FLAGS_ClipView | RENDER_FLAGS_Pop;
	const sf::Uint32 PRINT_Layout 	= PRINT_Centre_X | PRINT_Centre_Y | PRINT_Centre_Around_X | PRINT_Centre_Around_Y | PRINT_Right | PRINT_Top | PRINT_Bottom;
	const sf::Uint32 PRINT_Ratio	= PRINT_Ratio_X | PRINT_Ratio_Y;
	const sf::Uint32 PRINT_Anchor	= PRINT_Anchor_Top | PRINT_Anchor_Mid | PRINT_Anchor_Bottom;

	//-----------------------------------------------------------------------------
	class SpriteResourceManager;
	class Render;
	class RenderContainer;

	typedef std::vector<RenderContainer*>		SpriteContainerArray;
	typedef std::vector<SpriteContainerArray*>	RenderQueues;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	typedef struct _tagTextAttr
	{
		sf::Color	m_colour;
		float		m_scale;
	} PrintAttrs;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class RenderContainer
	{
		friend class Render;
	private:
		int		    m_renderFlags;
		sf::Sprite*	m_sprite;
	public:
		RenderContainer(sf::Sprite& Sprite, int aRenderFlags);
		~RenderContainer();

		sf::Sprite* getSprite(void) { return m_sprite; }
		int			getFlags(void)  { return m_renderFlags; }
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class Render : public sf::RenderWindow
	{
	private:
		SpriteResourceManager*	m_resourceManager;
		bool		            m_iOwnResMgr;
		RenderQueues	        m_renderQueue;
		sf::Vector2f	        m_videoMode;
		sf::Vector2f	        m_pivot;
		SpriteTexture*	        m_defaultFont;

	public:
		Render(sf::VideoMode mode, const sf::String& title, const sf::String& aResource = sf::String(), const int aNumLayers = 1, const sf::String& aFont = sf::String(), sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
		Render(Render& rhs);
		~Render();

		sf::Vector2f getWinSize(void)              { return m_videoMode; }
		RenderQueues& getRenderQueues(void)        { return m_renderQueue; }

		void setDefaultFont(SpriteTexture& aFont)  { m_defaultFont = &aFont; }

		void SetResMgr(SpriteResourceManager* aResMgr, bool iOwnResMgr = true);
		SpriteResourceManager* GetResMgr(void)     { return m_resourceManager; }

		int renderSprite(sf::Sprite& aSprite, int onLayer, int aRenderFlags = RENDER_FLAGS_None);
		sf::Sprite* getSpriteFromLayerAtPosition(int onLayer, int aPos);

		sf::Vector2f PrintXY(float x, float y, const sf::String& aTextString, sf::FloatRect* winRect = nullptr, sf::Uint32 flags = 0, float scale = 1.0f, int aLayer = 0, sf::Color aColour = sf::Color(255,255,255,255), PrintAttrs* aAttribStr = nullptr, SpriteTexture* aFont = nullptr);
		sf::Vector2f Print(const sf::String& aString, const char* format, ...);

		void display(void);
	};
}

#endif //_RENDER_HPP_
