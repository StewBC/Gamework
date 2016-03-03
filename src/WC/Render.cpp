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
#include "Render.hpp"
#include <cstdarg>
#include <iostream>

namespace WC
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	RenderContainer::RenderContainer(sf::Sprite& aSprite, int aRenderFlags)
	{
		m_renderFlags = aRenderFlags;
		if (m_renderFlags & RENDER_FLAGS_Copy)
			m_sprite = new sf::Sprite(aSprite);
		else
			m_sprite = &aSprite;
	}

	//-----------------------------------------------------------------------------
	RenderContainer::~RenderContainer()
	{
		if (m_renderFlags & RENDER_FLAGS_Copy)
			delete m_sprite;
	}

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	Render::Render(sf::VideoMode mode, const sf::String& title, const sf::String& aResource, const int aNumLayers, const sf::String& aFontName, sf::Uint32 style, const sf::ContextSettings& settings)
		: RenderWindow(mode, title, style, settings), m_iOwnResMgr(false)
	{
		m_videoMode.x = (float)mode.width;
		m_videoMode.y = (float)mode.height;
		m_pivot.x = m_videoMode.x / 2.0f;
		m_pivot.y = m_videoMode.y / 2.0f;

		m_renderQueue.reserve(aNumLayers);
		for (int i = 0; i < aNumLayers; ++i)
			m_renderQueue.push_back(new SpriteContainerArray);

		if (!aResource.isEmpty())
		{
			m_resourceManager = new SpriteResourceManager(aResource);
			m_iOwnResMgr = true;
		}

		if (!aFontName.isEmpty() && m_resourceManager)
			setDefaultFont(*m_resourceManager->findSpriteTexture(aFontName));
	}

	//-----------------------------------------------------------------------------
	Render::Render(Render& rhs)
	{
		m_videoMode = rhs.m_videoMode;
		m_pivot = rhs.m_pivot;

		size_t numLayers = rhs.m_renderQueue.size();
		m_renderQueue.reserve(numLayers);
		for (int i = 0; i < numLayers; ++i)
			m_renderQueue.push_back(new SpriteContainerArray);

		m_iOwnResMgr = false;
		SetResMgr(rhs.GetResMgr(), false);

		setDefaultFont(*rhs.m_defaultFont);
	}

	//-----------------------------------------------------------------------------
	Render::~Render()
	{ 
		for (std::vector<SpriteContainerArray*>::iterator i = m_renderQueue.begin(); i != m_renderQueue.end(); ++i)
		{
			for (std::vector<RenderContainer*>::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
			{
				delete (*j);
			}
			(*i)->clear();
			delete (*i); 
		}
		m_renderQueue.clear();
	}

	//-----------------------------------------------------------------------------
	void Render::SetResMgr(SpriteResourceManager* aResMgr, bool iOwnResMgr)
	{
		if (m_iOwnResMgr)
			delete m_resourceManager;

		m_resourceManager = aResMgr; 
		m_iOwnResMgr = iOwnResMgr;
	}

	//-----------------------------------------------------------------------------
	int Render::renderSprite(sf::Sprite& aSprite, int onLayer, int aRenderFlags)
	{
		int position = (int)m_renderQueue[onLayer]->size();
		m_renderQueue[onLayer]->push_back(new RenderContainer(aSprite, aRenderFlags));
		return position;
	}

	//-----------------------------------------------------------------------------
	sf::Sprite* Render::getSpriteFromLayerAtPosition(int onLayer, int aPos)
	{
		if (aPos < (int)m_renderQueue[onLayer]->size())
			return m_renderQueue[onLayer]->at(aPos)->m_sprite;

		return nullptr;
	}

	//-----------------------------------------------------------------------------
	sf::Vector2f WC::Render::PrintXY(float x, float y, const sf::String& aTextString, sf::FloatRect* aWinRect, sf::Uint32 flags, float scale, int aLayer, sf::Color aColour, PrintAttrs* aAttribStr, SpriteTexture* aFont)
	{
		const 			sf::Uint32* letter;
		int				commandPos;
		float			right;
		float			bottom;
		sf::Sprite*		commandSprite;
		sf::Sprite 		letterSprite;
		sf::FloatRect	renderRect;
		sf::Vector2f 	renderPos;
		sf::Vector2f 	extents;
		int 			i = 0;
		int				textAnchor = PRINT_Anchor_Bottom;

		if (!aWinRect)
		{
			sf::Vector2f	winSize = getWinSize();
			renderRect.left = renderRect.top = 0.0f;
			renderRect.width = winSize.x;
			renderRect.height = winSize.y;
		}
		else
		{
			renderRect = *aWinRect;
		}

		if (!aFont)
			aFont = m_defaultFont;

		if (flags & PRINT_Ratio)
		{
			if (flags & PRINT_Ratio_X)
				x *= renderRect.width;
			if (flags & PRINT_Ratio_Y)
				y *= renderRect.height;
		}

		if (flags & PRINT_Set_Viewport)
		{
			commandSprite = new sf::Sprite;
			commandSprite->setPosition(renderRect.left, renderRect.top);
			commandSprite->setScale(renderRect.width, renderRect.height);
			renderSprite(*commandSprite, aLayer, RENDER_FLAGS_Copy | RENDER_FLAGS_ClipView);
			renderRect.left = renderRect.top = 0.0f;
			delete commandSprite;
		}
		
		renderPos = sf::Vector2f(renderRect.left + x, renderRect.top + y);

		if (flags & PRINT_Layout)
		{
			commandSprite = new sf::Sprite;
			commandPos = renderSprite(*commandSprite, aLayer, RENDER_FLAGS_Copy | RENDER_FLAGS_Translate);
			delete commandSprite;

			if (flags & (PRINT_Centre_X | PRINT_Centre_Around_X | PRINT_Right))
				renderPos.x = 0;

			if (flags & (PRINT_Centre_Y | PRINT_Centre_Around_Y | PRINT_Top | PRINT_Bottom))
				renderPos.y = 0;
		}

		right = renderRect.left + renderRect.width;
		bottom = renderRect.top + renderRect.height;

		letter = aTextString.getData();

		letterSprite.setTexture(aFont->texture());
		letterSprite.setColor(aColour);
		letterSprite.setScale(scale, scale);

		while (*letter)
		{
			int c = *letter++;
			c =  dynamic_cast<FontTexture*>(aFont)->getCharIndex(c);

			sf::IntRect letterRect = aFont->getTextureRect(c);
			if (flags & PRINT_Wrap_Text && renderPos.x + letterRect.width >= right)
			{
				renderPos.x = (float)renderRect.left;
				renderPos.y += extents.y;
			}

			if (aLayer >= 0)
			{
				if (aAttribStr)
				{
					scale = aAttribStr[i].m_scale;
					letterSprite.setScale(scale, scale);
					letterSprite.setColor(aAttribStr[i++].m_colour);
				}

				letterSprite.setTextureRect(letterRect);
				letterSprite.setPosition(renderPos);

				switch (flags & PRINT_Anchor)
				{
					case PRINT_Anchor_Top:
						letterSprite.setOrigin(0, 0);
						break;
					case PRINT_Anchor_Mid:
						letterSprite.setOrigin(0, letterRect.height / 2.0f);
						break;
					default:
						letterSprite.setOrigin(0, (float)letterRect.height);
						break;
				}

				renderSprite(letterSprite, aLayer, RENDER_FLAGS_Copy);
			}

			renderPos.x += (letterRect.width + 1.0f) * scale;

			float mheight = letterRect.height * scale;
			if (extents.x < renderPos.x)
				extents.x = renderPos.x;
			if (extents.y < mheight)
				extents.y = mheight;
		}

		if (flags & PRINT_Layout)
		{
			renderPos.x = renderPos.y = 0.0f;

			if (flags & PRINT_Centre_X)
				renderPos.x = renderRect.left + renderRect.width / 2 - extents.x / 2;

			if (flags & PRINT_Centre_Y)
				renderPos.y = renderRect.top + renderRect.height / 2 + extents.y / 2;

			if (flags & PRINT_Centre_Around_X)
				renderPos.x = renderRect.left - extents.x / 2;

			if (flags & PRINT_Centre_Around_Y)
				renderPos.y = renderRect.top - extents.y / 2;

			if (flags & PRINT_Right)
				renderPos.x = renderRect.left + renderRect.width - extents.x;

			if (flags & PRINT_Top)
				renderPos.y = renderRect.top + extents.y;

			if (flags & PRINT_Bottom)
				renderPos.y = renderRect.top + renderRect.height;

			if (flags & (PRINT_Centre_X | PRINT_Centre_Around_X | PRINT_Right))
				renderPos.x += x;

			if (flags & (PRINT_Centre_Y | PRINT_Centre_Around_Y | PRINT_Top | PRINT_Bottom))
				renderPos.y += y;

			commandSprite = getSpriteFromLayerAtPosition(aLayer, commandPos);
			commandSprite->setPosition(renderPos);
			renderSprite(*commandSprite, aLayer, RENDER_FLAGS_Pop | RENDER_FLAGS_Copy | RENDER_FLAGS_Translate);
		}

		if (flags & PRINT_Set_Viewport)
		{
			commandSprite = new sf::Sprite;
			renderSprite(*commandSprite, aLayer, RENDER_FLAGS_Pop | RENDER_FLAGS_Copy | RENDER_FLAGS_ClipView);
			delete commandSprite;
		}

		return extents;
	}

	sf::Vector2f Render::Print(const sf::String& aString, const char* format, ...)
	{
		va_list			args;
		float			x = 0.0f;
		float			y = 0.0f;
		PrintAttrs*		attribsStr = nullptr;
		sf::Color		color(255,255,255,255);
		sf::Uint32		flags = 0;
		int				layer = 0;
		float			scale = 1.0f;
		SpriteTexture*	font = m_defaultFont;
		sf::FloatRect	aWinRect(0.0f, 0.0f, m_videoMode.x, m_videoMode.y);

		// Format:
		//		a  					  			  | PrintAttr* argument read
		//		c  						  		  | sf::Color argument read
		//		d<t|m|b> 						  | Text Anchot Top, Mid or Bottom
		//		f  					  			  | FontTexture* argument read
		//		g<x|y|*> 				  		  | Goto x (argument read) or y (argument read) or x and y (2 arguments read)
		//		l  						  		  | Render Layer argument read
		//		p<a<x|y|*>|b|c<x|y|*>|r|t>+<;|\0> | Position text a = around x, y or both; b = bottom; c = centered in x, y or both; r = right; t=top. End with ';' or and of string
		//		r<x|y|*> 				  		  | Goto (as ratio of window) x (argument read) or y (argument read) or x and y (2 arguments read)
		//		s  						  		  | Scale argument read
		//		t 								  | Text Wrap at window end ON
		//		v								  | Viewport - restrict drawing to the rectangle in w
		//		w								  | sf::FloatRect argument read
		va_start(args, format);
		while (*format)
		{
			switch (*format)
			{
				case 'a': // Attributes -args = PrintAttrs:[]
					attribsStr = va_arg(args, PrintAttrs*);
					break;

				case 'c': // Colour -args = sf::Color:1
					color = *va_arg(args, sf::Color*);
					break;

				case 'd':
				{
					++format;
					switch (*format)
					{
					case 'b':
						flags |= PRINT_Anchor_Bottom;
						break;
					case 'm':
						flags |= PRINT_Anchor_Mid;
						break;
					case 't':
						flags |= PRINT_Anchor_Top;
						break;
					default:
						std::cout << "format flag drop must be b, m or t but is " << *format << std::endl;
					}
					break;
				}

				case 'f': // Font -args = s:1
					font = va_arg(args, SpriteTexture*);
					break;

				case 'g':
					++format;
					switch (*format)
					{
					case 'x':
						x = (float)va_arg(args, double);
						break;
					case 'y':
						y = (float)va_arg(args, double);
						break;
					case '*':
						x = (float)va_arg(args, double);
						y = (float)va_arg(args, double);
						break;
					default:
						std::cout << "format goto drop must be x, y or * but is " << *format << std::endl;
					}
					break;

				case 'l': // Layer -args = i:1
					layer = va_arg(args, int);
					break;

				case 'p': // Layout l<a<x|y>|b|c<x|y>|r|t>;
				{
					++format;
					while (*format && ';' != *format)
					{
						switch (*format)
						{
							case 'a':
								++format;
								switch (*format)
								{
									case 'x':
										flags |= PRINT_Centre_Around_X;
										break;
									case 'y':
										flags |= PRINT_Centre_Around_Y;
										break;
									case '*':
										flags |= PRINT_Centre_Around_X;
										flags |= PRINT_Centre_Around_Y;
										break;
									default:
										std::cout << "format flag layout a must be x or y but is " << *format << std::endl;
										break;
								}
								break;

							case 'b':
								flags |= PRINT_Bottom;
								break;

							case 'c':
								++format;
								switch (*format)
								{
									case 'x':
										flags |= PRINT_Centre_X;
										break;
									case 'y':
										flags |= PRINT_Centre_Y;
										break;
									case '*':
										flags |= PRINT_Centre_X;
										flags |= PRINT_Centre_Y;
										break;
									default:
										std::cout << "format flag layout c must be x, y or * but is " << *format << std::endl;
										break;
								}
								break;

							case 'r':
								flags |= PRINT_Right;
								break;

							case 't':
								flags |= PRINT_Top;
								break;

							default:
								std::cout << "Invalid format flag character " << *format << std::endl;
								break;
						}
						++format;
					}
					if (!*format)
						--format;
					break;
				}

				case 'r':
					++format;
					switch (*format)
					{
						case 'x':
							flags |= PRINT_Ratio_X;
							x = (float)va_arg(args, double);
							break;
						case 'y':
							flags |= PRINT_Ratio_Y;
							y = (float)va_arg(args, double);
							break;
						case '*':
							flags |= PRINT_Ratio_X;
							flags |= PRINT_Ratio_Y;
							x = (float)va_arg(args, double);
							y = (float)va_arg(args, double);
							break;
						default:
							std::cout << "format flag layout c must be x, y or * but is " << *format << std::endl;
							break;
					}
					break;

				case 's': // Scale -args = f:1
					scale = (float)va_arg(args, double);
					break;

				case 't': // Scale -args = f:1
					flags |= PRINT_Wrap_Text;
					break;

				case 'v': // Window Rectangle -args = f:4
					flags |= PRINT_Set_Viewport;
					break;

				case 'w': // Window Rectangle -args = f:4
					aWinRect = *va_arg(args, sf::FloatRect*);
					break;

				default:
					std::cout << "Invalid format character " << *format << std::endl;
					break;
			}
			++format;
		}
		va_end(args);

		return PrintXY(x, y, aString, &aWinRect, flags, scale, layer, color, attribsStr, font);
	}

	//-----------------------------------------------------------------------------
	void Render::display(void)
	{
		std::vector<const sf::View*> viewStack;
		sf::Transform t;
		
		sf::View mainView = getView();
		viewStack.push_back(&mainView);

		for (int layer = 0; layer < m_renderQueue.size(); ++layer)
		{
			SpriteContainerArray* layerArray = m_renderQueue[layer];
			for (SpriteContainerArray::iterator spriteContainerItr = layerArray->begin(); spriteContainerItr != layerArray->end(); ++spriteContainerItr)
			{
				sf::Sprite* theSprite = (*spriteContainerItr)->m_sprite;
				int renderFlags = (*spriteContainerItr)->m_renderFlags;

				if (renderFlags & RENDER_Command)
				{
					if (renderFlags & RENDER_FLAGS_ClipView)
					{
						if (renderFlags & RENDER_FLAGS_Pop)
						{
							const sf::View *mView = viewStack.back();
							viewStack.pop_back();
							const sf::View aView = *viewStack.back();
							setView(aView);
							delete mView;
						}
						else
						{
							sf::Vector2f pos = theSprite->getPosition();
							sf::Vector2f size = theSprite->getScale();
							
							sf::View* mView = new sf::View(sf::Vector2f(size.x/2, size.y/2), sf::Vector2f(size.x, size.y));
							viewStack.push_back(mView);
							sf::FloatRect viewPort = { pos.x / m_videoMode.x, pos.y / m_videoMode.y, size.x / m_videoMode.x, size.y / m_videoMode.y };
							mView->setViewport(viewPort);
							setView(*mView);
						}
					}
					else
					{
						sf::Transform t1;
						if (renderFlags & RENDER_FLAGS_Rotate)
						{
							t1.translate(m_pivot);
							t1.rotate(theSprite->getRotation());
							t1.translate(-m_pivot);
						}

						if (renderFlags & RENDER_FLAGS_Translate)
							t1.translate(theSprite->getPosition());

						if (renderFlags & RENDER_FLAGS_Scale)
						{
							t1.translate(m_pivot);
							t1.scale(theSprite->getScale());
							t1.translate(-m_pivot);
						}

						if (renderFlags & RENDER_FLAGS_Pop)
							t *= t1.getInverse();
						else
							t *= t1;
					}
					delete (*spriteContainerItr);
					continue;
				}

				draw(*theSprite, t);
				delete (*spriteContainerItr);
			}
			layerArray->clear();
		}
		RenderWindow::display();
		viewStack.clear();
	}
}
