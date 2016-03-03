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

#ifndef _EXGLOBALS_HPP_
#define _EXGLOBALS_HPP_

#include "HighScoreTable.hpp"
#include "InputManager.hpp"
#include <SFML/Graphics.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace Examples
{ 
	// High Score Related
	extern const char* HighScoreFileName;
	extern Gamework::HighScoreTable gTodaysHighScores;
	extern Gamework::HighScoreTable gAllTimeHighScores;

	//// Initial values for high-score tables
	extern Gamework::HighScoreEntry gTodaysDefaultHighScores[Gamework::NumHighScoresInTable];
	extern Gamework::HighScoreEntry gAllTimeDefaultHighScores[Gamework::NumHighScoresInTable];

	// Set true when a global high-score is recorded
	extern bool gLoadedScoresHaveChanged;

	// Player related
	//const int MaxPlayersPerGame = 2;
	const int PlayersStartLives = 3;
	//extern int gNumPlayersThisGame;
	//extern int gActivePlayerNumber;
	//extern int gCurrentLevelPerPlayer[MaxPlayersPerGame];
	//extern int gLivesLeftPerPlayer[MaxPlayersPerGame];
	//extern int gScorePerPlayer[MaxPlayersPerGame];

	// Call once on startup to load the high scores from disk
	void loadHighScores(void);

	// Utility Functions
	sf::Uint32 getKeyForKeyValueInMap(sf::Uint32 aValue, WC::InputKeyMap& aMap);
	WC::JoyKeyVal getKeyForJoyValueInMap(sf::Uint32 aValue, WC::InputJoyMap& aMap);
	sf::Uint32 getValueForKeyKeyInMap(sf::Uint32 aValue, WC::InputKeyMap& aMap);
	sf::Uint32 getValueForJoyKeyInMap(WC::JoyKeyVal aValue, WC::InputJoyMap& aMap);
	bool isCloseEnough(float a, float b, float epsilon);
	void evalCubic(float& y0, float& m0, float y1, float t);

	// Helper Objects - Colours
	enum
	{
		ColourMaroon,
		ColourDarkRed,
		ColourBrown,
		ColourFirebrick,
		ColourCrimson,
		ColourRed,
		ColourTomato,
		ColourCoral,
		ColourIndianRed,
		ColourLightCoral,
		ColourDarkSalmon,
		ColourSalmon,
		ColourLightSalmon,
		ColourOrangeRed,
		ColourDarkOrange,
		ColourOrange,
		ColourGold,
		ColourDarkGoldenRod,
		ColourGoldenRod,
		ColourPaleGoldenRod,
		ColourDarkKhaki,
		ColourKhaki,
		ColourOlive,
		ColourYellow,
		ColourYellowGreen,
		ColourDarkOliveGreen,
		ColourOliveDrab,
		ColourLawnGreen,
		ColourChartReuse,
		ColourGreenYellow,
		ColourDarkGreen,
		ColourGreen,
		ColourForestGreen,
		ColourLime,
		ColourLimeGreen,
		ColourLightGreen,
		ColourPaleGreen,
		ColourDarkseaGreen,
		ColourMediumSpringGreen,
		ColourSpringGreen,
		ColourSeaGreen,
		ColourMediumAquaMarine,
		ColourMediumSeaGreen,
		ColourLightSeaGreen,
		ColourDarkSlateGray,
		ColourTeal,
		ColourDarkCyan,
		ColourCyan,
		ColourLightCyan,
		ColourDarkTurquoise,
		ColourTurquoise,
		ColourMediumTurquoise,
		ColourPaleTurquoise,
		ColourAquaMarine,
		ColourPowderBlue,
		ColourCadetBlue,
		ColourSteelBlue,
		ColourCornFlowerBlue,
		ColourDeepskyBlue,
		ColourDodgerBlue,
		ColourLightBlue,
		ColourSkyBlue,
		ColourLightskyBlue,
		ColourMidnightBlue,
		ColourNavy,
		ColourDarkBlue,
		ColourMediumBlue,
		ColourBlue,
		ColourRoyalBlue,
		ColourBlueViolet,
		ColourIndigo,
		ColourDarkSlateBlue,
		ColourSlateBlue,
		ColourMediumSlateBlue,
		ColourMediumPurple,
		ColourDarkMagenta,
		ColourDarkViolet,
		ColourDarkOrchid,
		ColourMediumOrchid,
		ColourPurple,
		ColourThistle,
		ColourPlum,
		ColourViolet,
		ColourMagenta,
		ColourOrchid,
		ColourMediumVioletRed,
		ColourPaleVioletRed,
		ColourDeepPink,
		ColourHotPink,
		ColourLightPink,
		ColourPink,
		ColourAntiqueWhite,
		ColourBeige,
		ColourBisque,
		ColourBlanchedAlmond,
		ColourWheat,
		ColourCornSilk,
		ColourLemonChiffon,
		ColourLightGoldenRodYellow,
		ColourLightYellow,
		ColourSaddleBrown,
		ColourSienna,
		ColourChocolate,
		ColourPeru,
		ColourSandyBrown,
		ColourBurlyWood,
		ColourTan,
		ColourRosyBrown,
		ColourMoccasin,
		ColourNavajoWhite,
		ColourPeachPuff,
		ColourMistyRose,
		ColourLavenderBlush,
		ColourLinen,
		ColourOldLace,
		ColourPapayaWhip,
		ColourSeaShell,
		ColourMintCream,
		ColourSlateGray,
		ColourLightSlateGray,
		ColourLightSteelBlue,
		ColourLavender,
		ColourFloralWhite,
		ColourAliceBlue,
		ColourGhostWhite,
		ColourHoneydew,
		ColourIvory,
		ColourAzure,
		ColourSnow,
		ColourBlack,
		ColourDimGray,
		ColourGray,
		ColourDarkGray,
		ColourSilver,
		ColourLightGray,
		ColourGainsboro,
		ColourWhiteSmoke,
		ColourWhite, 
		NumColours,
	};

	// The actual array of colours matching the above enums
	extern sf::Color gColours[NumColours];

	// Labels for joystick and keyboard
	extern const char* gJoyNames[];
	extern const char* gKeyNames[];
}

#endif //_EXGLOBALS_HPP_
