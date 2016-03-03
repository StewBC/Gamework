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

#ifndef _HIGHSCORETABLE_HPP_
#define _HIGHSCORETABLE_HPP_

#include <vector>

//-----------------------------------------------------------------------------
namespace plist
{
	class XML_array;
}

namespace Gamework
{
	//-----------------------------------------------------------------------------
	const int NumHighScoresInTable = 10;
	const int NumInitialsCharacters = 3;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	typedef struct
	{
	public:
		int		m_score;
		char	m_initials[NumInitialsCharacters + 1];
	} HighScoreEntry;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class HighScoreTable
	{
	private:
		std::vector<HighScoreEntry>	m_highScores;

		int processLoadedScoresArray(plist::XML_array* aArray);

	public:
		HighScoreTable();
		~HighScoreTable() { ; }

		const int score(int aScoreIndex) { return m_highScores[aScoreIndex].m_score; }
		const char* initials(int aInitialsIndex) { return m_highScores[aInitialsIndex].m_initials; }

		void setupDefaultsFromArray(HighScoreEntry* aArray);
		bool makesItOntoTable(int aScore);
		int insertInTable(HighScoreEntry& aEntry);

		int loadScores(const char* aFileName);
		void saveScores(const char* aFileName);

	};
}
#endif //_HIGHSCORETABLE_HPP_
