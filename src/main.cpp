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

#include "Game.hpp"
#include "ExGlobals.hpp"
#include "Globals.hpp"
#include "SpriteTexture.hpp"
#include "EffectsMgr.hpp"
#include "Render.hpp"
#include <math.h>

#define SHOW_FPS

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// Make the window
	Gamework::gRenderer		= new WC::Render(sf::VideoMode(640, 480), "Gamework", "sprites.plist", Gamework::RenderLayerNumLayers, "consolas.png");
	Gamework::gEffectsMgr	= new WC::EffectsManager(*Gamework::gRenderer);
	Gamework::gInputMgr		= new WC::InputManager();
	Gamework::gGame			= new Gamework::Game();

	Gamework::gRenderer->setFramerateLimit(60);
	sf::Clock gameClock;

	srand(time(nullptr));

#ifdef SHOW_FPS
	int loops = 0;
	sf::Clock fpsClock;
	std::string fpsStr = "FPS: ===";
#endif

	Examples::loadHighScores();

	while (Gamework::gRenderer->isOpen())
	{
#ifdef SHOW_FPS
		++loops;
		if (fpsClock.getElapsedTime() >= sf::seconds(1))
		{
			fpsStr = "FPS: " + std::to_string((int)ceil(loops / fpsClock.getElapsedTime().asSeconds()));
			loops = 0;
			fpsClock.restart();
		}
		Gamework::gRenderer->Print(fpsStr, "dtc", &sf::Color::Green);
#endif

		sf::Event event;
		while (Gamework::gRenderer->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				Gamework::gRenderer->close();
		}

		// Calculate elapsedTime time since last run
		sf::Time elapsedTime = gameClock.getElapsedTime();
		gameClock.restart();

		Gamework::gRenderer->clear();
		if (!Gamework::gGame->run(elapsedTime))
			Gamework::gRenderer->close();

		Gamework::gRenderer->display();
	}

	if (Examples::gLoadedScoresHaveChanged)
		Examples::gAllTimeHighScores.saveScores(Examples::HighScoreFileName);

	delete Gamework::gGame;
	delete Gamework::gEffectsMgr;
	delete Gamework::gInputMgr;
	delete Gamework::gRenderer;

	return 0;
}
