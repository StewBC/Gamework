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

#include "LevelManager.hpp"

namespace Gamework
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	bool LevelManager::loadlevel(int aLevel)
	{
		if (isLevelLoaded(aLevel))
			return true;

		loadedLevels |= SET_BIT(aLevel);
		return true;
	}
}
