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

#include "ExGlobals.hpp"
#include <math.h>

namespace Examples
{
	//-----------------------------------------------------------------------------
	const char* HighScoreFileName = "highscores.plist";

	Gamework::HighScoreTable gTodaysHighScores;
	Gamework::HighScoreTable gAllTimeHighScores;

	Gamework::HighScoreEntry gAllTimeDefaultHighScores[Gamework::NumHighScoresInTable] =
	{
		{ 100, "SQW" },
		{ 90 , "NV " },
		{ 80 , "RJW" },
		{ 70 , "AIW" },
		{ 60 , "SQW" },
		{ 50 , "NV " },
		{ 40 , "RJW" },
		{ 30 , "AIW" },
		{ 20 , "SQW" },
		{ 10 , "NV " },
};
	Gamework::HighScoreEntry gTodaysDefaultHighScores[Gamework::NumHighScoresInTable] =
	{
		{ 9, "SQW" },
		{ 8, "SQW" },
		{ 7, "SQW" },
		{ 6, "SQW" },
		{ 5, "SQW" },
		{ 4, "SQW" },
		{ 3, "SQW" },
		{ 2, "SQW" },
		{ 1, "SQW" },
		{ 0, "SQW" },
	};

	//// Initialized right here
	//int gNumPlayersThisGame = 0;
	//int gActivePlayerNumber = 0;
	bool gLoadedScoresHaveChanged = false;

	//// Need to be initialized in InitGlobals
	//int gCurrentLevelPerPlayer[MaxPlayersPerGame];
	//int gLivesLeftPerPlayer[MaxPlayersPerGame];
	//int gScorePerPlayer[MaxPlayersPerGame];

	//-----------------------------------------------------------------------------
	void loadHighScores(void)
	{
		int numLoadedScores = gAllTimeHighScores.loadScores(HighScoreFileName);
		if (numLoadedScores != Gamework::NumHighScoresInTable)
			gAllTimeHighScores.setupDefaultsFromArray(gAllTimeDefaultHighScores);

		gTodaysHighScores.setupDefaultsFromArray(gTodaysDefaultHighScores);
	}


	//-----------------------------------------------------------------------------
	sf::Uint32 getKeyForKeyValueInMap(sf::Uint32 aValue, WC::InputKeyMap& aMap)
	{
		for (WC::InputKeyMap::iterator i = aMap.begin(); i != aMap.end(); ++i)
		{
			if ((*i).second == aValue)
				return (*i).first;
		}
		return 0;
	}

	//-----------------------------------------------------------------------------
	WC::JoyKeyVal getKeyForJoyValueInMap(sf::Uint32 aValue, WC::InputJoyMap& aMap)
	{
		for (WC::InputJoyMap::iterator i = aMap.begin(); i != aMap.end(); ++i)
		{
			if ((*i).second == aValue)
				return (*i).first;
		}
		return WC::JoyKeyVal(-1, 0, -1, 0);
	}

	//-----------------------------------------------------------------------------
	sf::Uint32 getValueForKeyKeyInMap(sf::Uint32 aValue, WC::InputKeyMap& aMap)
	{
		for (WC::InputKeyMap::iterator i = aMap.begin(); i != aMap.end(); ++i)
		{
			if ((*i).first == aValue)
				return (*i).second;
		}
		return -1;
	}

	//-----------------------------------------------------------------------------
	sf::Uint32 getValueForJoyKeyInMap(WC::JoyKeyVal aValue, WC::InputJoyMap& aMap)
	{
		for (WC::InputJoyMap::iterator i = aMap.begin(); i != aMap.end(); ++i)
		{
			if ((*i).first == aValue)
				return (*i).second;
		}
		return -1;
	}

	//-----------------------------------------------------------------------------
	bool isCloseEnough(float a, float b, float epsilon)
	{
		return fabs(a - b) < epsilon;
	}

	//-----------------------------------------------------------------------------
	void evalCubic(float& y0, float& m0, float y1, float t)
	{
		float a, b, c;

		c = y1 - y0;
		a = m0 - c - c;
		b = c + c + c - m0 - m0;

		y0 = ((((a * t + b) * t) + m0) * t) + y0;
		m0 = ((((a + a + a) * t) + (b + b)) * t) + m0;
	}

	//-----------------------------------------------------------------------------
	sf::Color gColours[NumColours] =
	{
		{ 128, 0  , 0   }, 	// maroon
		{ 139, 0  , 0   }, 	// dark red
		{ 165, 42 , 42  }, 	// brown
		{ 178, 34 , 34  }, 	// firebrick
		{ 220, 20 , 60  }, 	// crimson
		{ 255, 0  , 0   }, 	// red
		{ 255, 99 , 71  }, 	// tomato
		{ 255, 127, 80  }, 	// coral
		{ 205, 92 , 92  }, 	// indian red
		{ 240, 128, 128 }, 	// light coral
		{ 233, 150, 122 }, 	// dark salmon
		{ 250, 128, 114 }, 	// salmon
		{ 255, 160, 122 }, 	// light salmon
		{ 255, 69 , 0   }, 	// orange red
		{ 255, 140, 0   }, 	// dark orange
		{ 255, 165, 0   }, 	// orange
		{ 255, 215, 0   }, 	// gold
		{ 184, 134, 11  }, 	// dark golden rod
		{ 218, 165, 32  }, 	// golden rod
		{ 238, 232, 170 }, 	// pale golden rod
		{ 189, 183, 107 }, 	// dark khaki
		{ 240, 230, 140 }, 	// khaki
		{ 128, 128, 0   }, 	// olive
		{ 255, 255, 0   }, 	// yellow
		{ 154, 205, 50  }, 	// yellow green
		{ 85 , 107, 47  }, 	// dark olive green
		{ 107, 142, 35  }, 	// olive drab
		{ 124, 252, 0   }, 	// lawn green
		{ 127, 255, 0   }, 	// chart reuse
		{ 173, 255, 47  }, 	// green yellow
		{ 0  , 100, 0   }, 	// dark green
		{ 0  , 128, 0   }, 	// green
		{ 34 , 139, 34  }, 	// forest green
		{ 0  , 255, 0   }, 	// lime
		{ 50 , 205, 50  }, 	// lime green
		{ 144, 238, 144 }, 	// light green
		{ 152, 251, 152 }, 	// pale green
		{ 143, 188, 143 }, 	// dark sea green
		{ 0  , 250, 154 }, 	// medium spring green
		{ 0  , 255, 127 }, 	// spring green
		{ 46 , 139, 87  }, 	// sea green
		{ 102, 205, 170 }, 	// medium aqua marine
		{ 60 , 179, 113 }, 	// medium sea green
		{ 32 , 178, 170 }, 	// light sea green
		{ 47 , 79 , 79  }, 	// dark slate gray
		{ 0  , 128, 128 }, 	// teal
		{ 0  , 139, 139 }, 	// dark cyan
		{ 0  , 255, 255 }, 	// cyan
		{ 224, 255, 255 }, 	// light cyan
		{ 0  , 206, 209 }, 	// dark turquoise
		{ 64 , 224, 208 }, 	// turquoise
		{ 72 , 209, 204 }, 	// medium turquoise
		{ 175, 238, 238 }, 	// pale turquoise
		{ 127, 255, 212 }, 	// aqua marine
		{ 176, 224, 230 }, 	// powder blue
		{ 95 , 158, 160 }, 	// cadet blue
		{ 70 , 130, 180 }, 	// steel blue
		{ 100, 149, 237 }, 	// corn flower blue
		{ 0  , 191, 255 }, 	// deep sky blue
		{ 30 , 144, 255 }, 	// dodger blue
		{ 173, 216, 230 }, 	// light blue
		{ 135, 206, 235 }, 	// sky blue
		{ 135, 206, 250 }, 	// light sky blue
		{ 25 , 25 , 112 }, 	// midnight blue
		{ 0  , 0  , 128 }, 	// navy
		{ 0  , 0  , 139 }, 	// dark blue
		{ 0  , 0  , 205 }, 	// medium blue
		{ 0  , 0  , 255 }, 	// blue
		{ 65 , 105, 225 }, 	// royal blue
		{ 138, 43 , 226 }, 	// blue violet
		{ 75 , 0  , 130 }, 	// indigo
		{ 72 , 61 , 139 }, 	// dark slate blue
		{ 106, 90 , 205 }, 	// slate blue
		{ 123, 104, 238 }, 	// medium slate blue
		{ 147, 112, 219 }, 	// medium purple
		{ 139, 0  , 139 }, 	// dark magenta
		{ 148, 0  , 211 }, 	// dark violet
		{ 153, 50 , 204 }, 	// dark orchid
		{ 186, 85 , 211 }, 	// medium orchid
		{ 128, 0  , 128 }, 	// purple
		{ 216, 191, 216 }, 	// thistle
		{ 221, 160, 221 }, 	// plum
		{ 238, 130, 238 }, 	// violet
		{ 255, 0  , 255 }, 	// magenta
		{ 218, 112, 214 }, 	// orchid
		{ 199, 21 , 133 }, 	// medium violet red
		{ 219, 112, 147 }, 	// pale violet red
		{ 255, 20 , 147 }, 	// deep pink
		{ 255, 105, 180 }, 	// hot pink
		{ 255, 182, 193 }, 	// light pink
		{ 255, 192, 203 }, 	// pink
		{ 250, 235, 215 }, 	// antique white
		{ 245, 245, 220 }, 	// beige
		{ 255, 228, 196 }, 	// bisque
		{ 255, 235, 205 }, 	// blanched almond
		{ 245, 222, 179 }, 	// wheat
		{ 255, 248, 220 }, 	// corn silk
		{ 255, 250, 205 }, 	// lemon chiffon
		{ 250, 250, 210 }, 	// light golden rod yellow
		{ 255, 255, 224 }, 	// light yellow
		{ 139, 69 , 19  }, 	// saddle brown
		{ 160, 82 , 45  }, 	// sienna
		{ 210, 105, 30  }, 	// chocolate
		{ 205, 133, 63  }, 	// peru
		{ 244, 164, 96  }, 	// sandy brown
		{ 222, 184, 135 }, 	// burly wood
		{ 210, 180, 140 }, 	// tan
		{ 188, 143, 143 }, 	// rosy brown
		{ 255, 228, 181 }, 	// moccasin
		{ 255, 222, 173 }, 	// navajo white
		{ 255, 218, 185 }, 	// peach puff
		{ 255, 228, 225 }, 	// misty rose
		{ 255, 240, 245 }, 	// lavender blush
		{ 250, 240, 230 }, 	// linen
		{ 253, 245, 230 }, 	// old lace
		{ 255, 239, 213 }, 	// papaya whip
		{ 255, 245, 238 }, 	// sea shell
		{ 245, 255, 250 }, 	// mint cream
		{ 112, 128, 144 }, 	// slate gray
		{ 119, 136, 153 }, 	// light slate gray
		{ 176, 196, 222 }, 	// light steel blue
		{ 230, 230, 250 }, 	// lavender
		{ 255, 250, 240 }, 	// floral white
		{ 240, 248, 255 }, 	// alice blue
		{ 248, 248, 255 }, 	// ghost white
		{ 240, 255, 240 }, 	// honeydew
		{ 255, 255, 240 }, 	// ivory
		{ 240, 255, 255 }, 	// azure
		{ 255, 250, 250 }, 	// snow
		{ 0  , 0  , 0   }, 	// black
		{ 105, 105, 105 }, 	// dim gray
		{ 128, 128, 128 }, 	// gray
		{ 169, 169, 169 }, 	// dark gray
		{ 192, 192, 192 }, 	// silver
		{ 211, 211, 211 }, 	// light gray
		{ 220, 220, 220 }, 	// gainsboro
		{ 245, 245, 245 }, 	// white smoke
		{ 255, 255, 255 }, 	// white	
	};

	//-----------------------------------------------------------------------------
	const char* gJoyNames[] =
	{
		"X",
		"Y",
		"Z",
		"R",
		"U",
		"V",
		"PovX",
		"PovY",
	};

	//-----------------------------------------------------------------------------
	const char* gKeyNames[] =
	{
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"Escape",
		"LControl",
		"LShift",
		"LAlt",
		"LSystem",
		"RControl",
		"RShift",
		"RAlt",
		"RSystem",
		"Menu",
		"[",
		"]",
		";",
		",",
		".",
		"'",
		"\\",
		"/",
		"~",
		"=",
		"-",
		"Space",
		"Return",
		"BackSpace",
		"Tab",
		"PageUp",
		"PageDown",
		"End",
		"Home",
		"Insert",
		"Delete",
		"+",
		"-",
		"*",
		"/",
		"Left",
		"Right",
		"Up",
		"Down",
		"Numpad0",
		"Numpad1",
		"Numpad2",
		"Numpad3",
		"Numpad4",
		"Numpad5",
		"Numpad6",
		"Numpad7",
		"Numpad8",
		"Numpad9",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"F13",
		"F14",
		"F15",
		"Pause"
	};
}
