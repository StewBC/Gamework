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

#ifndef _LEVELMANAGER_HPP_
#define _LEVELMANAGER_HPP_

namespace Gamework
{
	// Utility
	#define SET_BIT(x)	(1<<(x))

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class LevelManager
	{
	private:
		int	loadedLevels;

	public:
		LevelManager()  { ; }
		~LevelManager() { ; }

		bool isLevelLoaded(int aLevel) { return 0 != (SET_BIT(aLevel) & loadedLevels); }
		bool loadlevel(int aLevel);

	};
}
#endif //_LEVELMANAGER_HPP_
