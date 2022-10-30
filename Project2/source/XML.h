#pragma once
#include <string>
#include <pugixml/pugixml.hpp>
#include <map>
#include <vector>
//#include "Hangman.h"

class XML
{
public:
	XML();
	~XML();
	
	std::map<int , std::string> xmlmssglist;
	struct saveData
	{
		std::string xml_theWord;
		int xml_gameState_;
		int xml_errorStatus_;
		int xml_live_;
		std::map<size_t, std::string> xml_foundPos;
		std::map<size_t, std::string> xml_myWord;
		std::map<std::string, size_t> xml_input_;
	};

	bool loadLangSetFromXML(const pugi::char_t* langSet);
	bool saveState(saveData savedata);

	bool loadState(saveData* savedata);

};


