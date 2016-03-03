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

#include "PlistHelper.hpp"
#include <string.h>
#include <algorithm>
#include <iostream>

#ifdef WIN32
	#define wc_stricmp	_stricmp
#else
	#define wc_stricmp	strcmp
#endif

namespace plist
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	XML_base* XML_dict::valueForKey(std::string aKeyString)
	{
		class keyCompare
		{
		private:
			const std::string& rhs;

		public:
			explicit keyCompare(const std::string& str) : rhs(str) { }
			inline bool operator()(std::map<XML_key*, XML_base*>::value_type const& itr) const { return itr.first->m_string == rhs; }
		};

		auto found = std::find_if(m_dict.begin(), m_dict.end(), keyCompare(aKeyString));
		if (found != m_dict.end())
			return found->second;

		return nullptr;
	}

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	XML_Type XML_parser::get_type(const char* pName)
	{
		static struct
		{
			XML_Type	m_type;
			const char*	m_typeName;
		} xml_type_table[] =
		{
			TYPE_string,	"string",
			TYPE_integer,	"integer",
			TYPE_key,		"key",
			TYPE_dict,		"dict",
			TYPE_array,		"array",
			TYPE_ignore,	"ignore",
		};
		static const int num_xml_type_table = sizeof(xml_type_table) / sizeof(xml_type_table[0]);

		//	const char* pName = p->name();
		if (*pName)
		{
			for (int i = 0; i < num_xml_type_table - 1; ++i)
			{
				if (0 == wc_stricmp(xml_type_table[i].m_typeName, pName))
					return xml_type_table[i].m_type;
			}
		}
		return TYPE_ignore;
	}

	//-----------------------------------------------------------------------------
	// Finalizes a node (closes it) and adds it to its parent container 
	void XML_parser::close_node(const char* aString)
	{
		XML_base* closing, *parent;

		// Get the node that's being close
		closing = m_root.m_array.back();
		// Pop it off the stack
		m_root.m_array.pop_back();
		// Get the node that the "closing node" will be added to
		if (m_root.m_array.size())
			parent = m_root.m_array.back();
		else
			parent = &m_root;

		// If the closing node isn't a container, get the value as a string
		if (closing->isA() >= TYPE_key)
			dynamic_cast<XML_key*>(closing)->m_string = aString;

		// add the closing node to the parent container
		switch (parent->isA())
		{
		case TYPE_array:
			dynamic_cast<XML_array*>(parent)->m_array.push_back(closing);
			break;

		case TYPE_dict:
			// If it's a key, save the key to add when the value is available
			// but if it isn't a key, use the last key as the key for this value
			if (closing->isA() != TYPE_key)
			{
				XML_base* key = m_keys.m_array.back();
				dynamic_cast<XML_dict*>(parent)->m_dict.insert(std::pair<XML_key*, XML_base*>(dynamic_cast<XML_key*>(key), closing));
				m_keys.m_array.pop_back();
			}
			else
			{
				m_keys.m_array.push_back(closing);
			}
			break;

		default:
			assert(0);
			break;
		}
	}

	//-----------------------------------------------------------------------------
	// Recursively called by itself on nodes to parse the tree
	void XML_parser::build_data(rapidxml::xml_node<>* p)
	{
		XML_Type pXMLType = get_type(p->name());

		// Type 2 is a close on a node </...>
		if (p->type() == 2)
		{
			close_node(p->value());
		}
		else
		{
			if (pXMLType != TYPE_ignore)
			{
				XML_base* newNode;

				switch (pXMLType)
				{
				case TYPE_array:
					newNode = new XML_array;
					break;

				case TYPE_key:
					newNode = new XML_key;
					break;

				case TYPE_dict:
					newNode = new XML_dict;
					break;

				case TYPE_string:
					newNode = new XML_string;
					break;

				case TYPE_integer:
					newNode = new XML_integer;
					break;

				default:
					std::cout << "Unhandled type encountered in build_data" << std::endl;
					newNode = nullptr;
					break;
				}
				m_root.m_array.push_back(newNode);
			}
		}

		// Iterate over all the children
		for (rapidxml::xml_node<>* child = p->first_node(); child; child = child->next_sibling())
			build_data(child);

		// Close out containers after dealing with their children (they don't get a type 2 node)
		if (pXMLType == TYPE_array || pXMLType == TYPE_dict)
			close_node(p->value());
	}

	//-----------------------------------------------------------------------------
	// Parser to parse a file in memory
	bool XML_parser::Parse(char* data)
	{
		rapidxml::xml_document<> doc;
		try
		{
			doc.parse<0>(data);
			build_data(doc.first_node());
		}
		catch (std::exception)
		{
			return false;
		}
		return true;
	}

	//-----------------------------------------------------------------------------
	// Parser to load a file into memory and then call Parse(char* data)
	bool XML_parser::ParseFile(const char* name)
	{
		std::ifstream loadFile(name, std::ios::ate | std::ios::binary);
		std::streampos fsize = loadFile.tellg();
		if (-1 == fsize)
			return false;

		long size = (long)fsize;
		char* buffer = new char[size + 1];
		loadFile.seekg(0, std::ios::beg);
		loadFile.read(buffer, fsize);
		buffer[size] = 0;
		loadFile.close();

		bool ret = Parse(buffer);

		delete[] buffer;

		return ret;
	}

	//-----------------------------------------------------------------------------
	// do indentation for nesting
	void XML_parser::saveIndent(int level)
	{
		for (int i = 0; i < level; ++i)
			m_fileStream << "\t";
	}

	//-----------------------------------------------------------------------------
	// Clean up the string and quote what needs to be quoted
	std::string& XML_parser::saveXMLReadyString(std::string& aString)
	{
		std::string buffer;
		buffer.reserve(aString.size());
		for (int i = 0; i != aString.size(); ++i)
		{
			switch (aString[i])
			{
			case '&':
				buffer.append("&amp;");
				break;
			case '\"':
				buffer.append("&quot;");
				break;
			case '\'':
				buffer.append("&apos;");
				break;
			case '<':
				buffer.append("&lt;");
				break;
			case '>':
				buffer.append("&gt;");
				break;
			default:
				buffer.append(&aString[i], 1);
				break;
			}
		}
		aString.swap(buffer);
		return aString;
	}

	//-----------------------------------------------------------------------------
	// save the contents of a node
	void XML_parser::saveNode(XML_base* node, int level)
	{
		saveIndent(level);
		XML_Type aType = node->isA();
		switch (aType)
		{
		case TYPE_array:
			m_fileStream << "<array>" << std::endl;
			saveArray(dynamic_cast<XML_array*>(node), level + 1);
			saveIndent(level);
			m_fileStream << "</array>" << std::endl;
			break;

		case TYPE_dict:
			m_fileStream << "<dict>" << std::endl;
			saveDict(dynamic_cast<XML_dict*>(node), level + 1);
			saveIndent(level);
			m_fileStream << "</dict>" << std::endl;
			break;

		case TYPE_key:
			m_fileStream << "<key>" << saveXMLReadyString(dynamic_cast<XML_key*>(node)->m_string) << "</key>" << std::endl;
			break;

		case TYPE_string:
			m_fileStream << "<string>" << saveXMLReadyString(dynamic_cast<XML_string*>(node)->m_string) << "</string>" << std::endl;
			break;

		case TYPE_integer:
			m_fileStream << "<integer>" << saveXMLReadyString(dynamic_cast<XML_integer*>(node)->m_string) << "</integer>" << std::endl;
			break;

		default:
			std::cout << "Unhandled type encountered in saveNode" << std::endl;
			break;
		}
	}

	//-----------------------------------------------------------------------------
	// save all elenments in an array
	void XML_parser::saveArray(XML_array* array, int level)
	{
		for (std::vector<XML_base*>::iterator i = array->m_array.begin(); i != array->m_array.end(); ++i)
			saveNode(*i, level);
	}

	//-----------------------------------------------------------------------------
	// save all elenments in an dict
	void XML_parser::saveDict(XML_dict* dict, int level)
	{
		for (std::map<XML_key*, XML_base*>::iterator i = dict->m_dict.begin(); i != dict->m_dict.end(); ++i)
		{
			saveNode(i->first, level);
			saveNode(i->second, level);
		}
	}

	//-----------------------------------------------------------------------------
	// save the whole tree
	void XML_parser::save(const char* aFileName)
	{
		m_fileStream.open(aFileName, std::ofstream::out);
		m_fileStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
		m_fileStream << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" << std::endl;
		m_fileStream << "<plist version=\"1.0\">" << std::endl;

		saveArray(&m_root, 0);

		m_fileStream << "</plist>" << std::endl;

		m_fileStream.close();
	}
}
