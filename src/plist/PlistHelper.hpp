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

#ifndef _PLISTLOADER_HPP_
#define _PLISTLOADER_HPP_

#include "rapidxml.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace plist
{
	//-----------------------------------------------------------------------------
	// Types of nodes supported.  Containers must be first, before key
	// which has to be the first non-container
	typedef enum
	{
		TYPE_ignore,
		TYPE_array,
		TYPE_dict,
		TYPE_key,
		TYPE_string,
		TYPE_integer,
	} XML_Type;

	//-----------------------------------------------------------------------------
	// Base class for all types so anything can live in a vector or map
	//-----------------------------------------------------------------------------
	class XML_base
	{
	public:
		XML_base() { ; }
		virtual ~XML_base() { ; }
		virtual XML_Type isA(void) = 0;
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class XML_array : public XML_base
	{
	public:
		std::vector<XML_base*>	m_array;
		XML_Type isA(void) { return TYPE_array; }

		~XML_array()
		{
			for (std::vector<XML_base*>::iterator i = m_array.begin(); i != m_array.end(); ++i)
				delete (*i);

			m_array.clear();
		}
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class XML_key : public XML_base
	{
	public:
		std::string			m_string;
		XML_Type isA(void) { return TYPE_key; }
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class XML_dict : public XML_base
	{
	public:
		std::map<XML_key*, XML_base*> m_dict;
		XML_Type isA(void) { return TYPE_dict; }

		~XML_dict()
		{
			for (std::map<XML_key*, XML_base*>::iterator i = m_dict.begin(); i != m_dict.end(); ++i)
			{
				delete i->first;
				delete i->second;
			}
			m_dict.clear();
		}

		XML_base* valueForKey(std::string aKeyString);
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class XML_string : public XML_key
	{
		XML_Type isA(void) { return TYPE_string; }
	};


	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class XML_integer : public XML_key
	{
		XML_Type isA(void) { return TYPE_integer; }
	};

	//-----------------------------------------------------------------------------
	// The parser has a m_root array which will contain the parsed file info
	//-----------------------------------------------------------------------------
	class XML_parser
	{
	private:
		std::fstream			m_fileStream;

		XML_Type get_type(const char* pName);
		void close_node(const char* aString);
		void build_data(rapidxml::xml_node<>* p);

		void saveIndent(int level);
		std::string& saveXMLReadyString(std::string& aString);
		void saveNode(XML_base* node, int level);
		void saveArray(XML_array* node, int level);
		void saveDict(XML_dict* node, int level);

	public:
		XML_array				m_root;
		XML_array				m_keys;

		XML_parser() :m_root(), m_keys() { ; }
		virtual ~XML_parser() { ; }

		bool Parse(char* data);
		bool ParseFile(const char* data);

		void addArray(void) { m_root.m_array.push_back(new XML_array); }
		void closeArray(void) { close_node(nullptr); }
		void addDict(void) { m_root.m_array.push_back(new XML_dict); }
		void closeDict(void) { close_node(nullptr); }
		void addKey(const char* aCString) { m_root.m_array.push_back(new XML_key); close_node(aCString); }
		void addString(const char* aCString) { m_root.m_array.push_back(new XML_string); close_node(aCString); }
		void addInteger(const int aInt) { m_root.m_array.push_back(new XML_integer); close_node(std::to_string(aInt).c_str()); }

		void save(const char* aFileName);
	};
}
#endif //_PLISTLOADER_HPP_
