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

#include "HighScoreTable.hpp"
#include "PlistHelper.hpp"
#include <string.h>

#ifdef WIN32
#define wc_sscanf	sscanf_s
#define wc_strncpy	strncpy_s
#else
#define wc_sscanf	sscanf
#define wc_strncpy	strncpy
#endif

namespace Gamework
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	HighScoreTable::HighScoreTable()
	{
		HighScoreEntry anEntry = { 0, "" };

		m_highScores.reserve(NumHighScoresInTable);
		for (int i = 0; i < NumHighScoresInTable; ++i)
			m_highScores.push_back(anEntry);
	}

	//-----------------------------------------------------------------------------
	void HighScoreTable::setupDefaultsFromArray(HighScoreEntry* aArray)
	{
		for (int i = 0; i < NumHighScoresInTable; ++i)
		{
			m_highScores[i].m_score = aArray[i].m_score;
			wc_strncpy(m_highScores[i].m_initials, aArray[i].m_initials, NumInitialsCharacters);
		}
	}

	//-----------------------------------------------------------------------------
	int HighScoreTable::processLoadedScoresArray(plist::XML_array* aArray)
	{
		static int index = 0;
		for (std::vector<plist::XML_base*>::iterator i = aArray->m_array.begin(); i != aArray->m_array.end(); ++i)
		{
			if (plist::TYPE_array == (*i)->isA())
			{
				processLoadedScoresArray(dynamic_cast<plist::XML_array*>(*(i)));
			}
			else if (plist::TYPE_dict == (*i)->isA())
			{
				plist::XML_dict* dict = dynamic_cast<plist::XML_dict*>((*i));
				plist::XML_string* initials = dynamic_cast<plist::XML_string*>(dict->valueForKey("initials"));
				plist::XML_integer* score = dynamic_cast<plist::XML_integer*>(dict->valueForKey("score"));
				if (initials && score)
				{
					wc_sscanf(score->m_string.c_str(), "%d", &m_highScores[index].m_score);
					wc_strncpy(m_highScores[index].m_initials, initials->m_string.c_str(), NumInitialsCharacters);
				}
				if (++index >= NumHighScoresInTable)
					return index;
			}
		}
		return index;
	}

	//-----------------------------------------------------------------------------
	int HighScoreTable::loadScores(const char* aFileName)
	{
		plist::XML_parser aParser;

		aParser.ParseFile(aFileName);
		return processLoadedScoresArray(&aParser.m_root);
	}

	//-----------------------------------------------------------------------------
	void HighScoreTable::saveScores(const char* aFileName)
	{
		plist::XML_parser aParser;

		aParser.addArray();
		{
			for (int i = 0; i < m_highScores.size(); ++i)
			{
				aParser.addDict();
				{
					aParser.addKey("score");
					aParser.addInteger(m_highScores[i].m_score);
					aParser.addKey("initials");
					aParser.addString(m_highScores[i].m_initials);
				}
				aParser.closeDict();
			}
		}
		aParser.closeArray();

		aParser.save(aFileName);
	}

	//-----------------------------------------------------------------------------
	bool HighScoreTable::makesItOntoTable(int aScore)
	{
		return aScore > m_highScores[NumHighScoresInTable - 1].m_score;
	}

	//-----------------------------------------------------------------------------
	int HighScoreTable::insertInTable(HighScoreEntry& aEntry)
	{
		int i = 0;
		for (std::vector<HighScoreEntry>::iterator index = m_highScores.begin(); index != m_highScores.end(); ++index, ++i)
		{
			if ((*index).m_score <= aEntry.m_score)
			{
				m_highScores.insert(index, aEntry);
				m_highScores.resize(NumHighScoresInTable);
				return i;
			}
		}
		return -1;
	}
}
